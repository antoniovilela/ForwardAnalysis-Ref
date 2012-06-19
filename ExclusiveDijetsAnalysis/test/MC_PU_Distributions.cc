//-------------------------------------------------------------------------------------------------------->>
// UNIVERSIDADE DO ESTADO DO RIO DE JANEIRO - CMS/Brasil
//-------------------------------------------------------------------------------------------------------->>
// Project: Exclusive Dijets Analysis
//-------------------------------------------------------------------------------------------------------->>
//
// Instructions to run the code: (a) using script ro run multiple parameters or (b) bash command line. 
//
// First Step: compile the code.
//             ?> scram b -r
//
// Second Step: copy from $CMSSW_BASE/test/slc.../Binary file to your own area with pileup root files.
//
// (A) SCRIPT TO RUN MULTIPLE PARAMETERS
// -------------------------------------
// $> python RunExclAnalysis.py
//
//
// (B) COMMAND LINE
// ----------------
// $> ./MC_PU_DistributionsCom "Inputfile.root" "outputfile.root" "CMSSW Process_Name/TTree_name"<pT(Jet1) Cut> <pT(Jet2) Cut> <Number of Vertex Cut> <Trigger Option> <Turn on(off) Luminosity Reweight> <Turn on(off) event-per-event Weight> <Turn on(off) Pre Selection> <Turn on(off) Trigger> <Luminosity Weight Factor> <# bins PU Distribution>
//
// TURN ON  = 1
// TURN OFF = 0
//
// I)   If you turn off PU reweight, the default weight will be 1;
// II)  If you turn off Luminosity reweight, the default weight will be 1;
// III) If you turn off Trigger Efficiency, the default weight will be 1;
// IV)  If you turn off event-per-event weight (some MC sample), the default weight will be 1;
//
// EXAMPLE: ./MC_PU_DistributionsCom "inputfile.root" "outputfile.root" "forwardQCDTTreeAnalysis/ProcessedTree" 60 55 1 1 1 1 1 1 2.3 25
//
// Twiki: https://twiki.cern.ch/twiki/bin/view/CMS/FwdPhysicsExclusiveDijetsAnalysis#For_MonteCarlo
//

//#if !defined(__CINT__) || defined(__MAKECINT__)
#include <TROOT.h>
#include <TChain.h>
#include <TString.h>
#include <TH1.h>
#include <TH2.h>
#include <TTree.h>
#include <TMath.h>

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <fstream>

#include "MC_PU_Distributions.h"
#include "ForwardAnalysis/ForwardTTreeAnalysis/interface/ExclusiveDijetsEvent.h"
#include "ForwardAnalysis/ForwardTTreeAnalysis/interface/DiffractiveEvent.h"
#include "KKousour/QCDAnalysis/interface/QCDEvent.h"
#include "PhysicsTools/Utilities/interface/LumiReWeighting.h"

using namespace diffractiveAnalysis;
using namespace exclusiveDijetsAnalysis;
using namespace reweight;

void MC_PU_Distributions::LoadFile(std::string fileinput, std::string processinput){

   inf = NULL;
   tr  = NULL;
   inf = TFile::Open(fileinput.c_str(),"read");
   tr = (TTree*)inf->Get(processinput.c_str());
   eventdiff = new DiffractiveEvent();
   eventexcl = new ExclusiveDijetsEvent();
   eventqcd = new QCDEvent();
   diff = tr->GetBranch("DiffractiveAnalysis");
   excl = tr->GetBranch("ExclusiveDijetsAnalysis");
   qcd = tr->GetBranch("QCDAnalysis");
   diff->SetAddress(&eventdiff);
   excl->SetAddress(&eventexcl);
   qcd->SetAddress(&eventqcd);

}

void MC_PU_Distributions::Run(std::string filein_, std::string savehistofile_, std::string processname_, double jet1PT_, double jet2PT_, int optnVertex_, int optTrigger_, bool switchWeightLumi_, bool switchWeightePw_, bool switchPreSel_, bool switchTrigger_, double weightlumipass_, int nbins_){

   filein = filein_;
   savehistofile = savehistofile_;
   processname = processname_;
   filein = filein_;
   jet1PT = jet1PT_;
   jet2PT = jet2PT_;
   optnVertex = optnVertex_;
   optTrigger = optTrigger_;
   switchWeightLumi = switchWeightLumi_;
   switchWeightePw = switchWeightePw_;
   switchPreSel = switchPreSel_;
   switchTrigger = switchTrigger_;
   weightlumipass = weightlumipass_;
   nbins = nbins_;

   std::cout << "" << std::endl;
   std::cout << "Running..." << std::endl;
   std::cout << "" << std::endl;
   std::cout << "<< INPUTS >>" << std::endl;
   std::cout << " " << std::endl;
   std::cout << "Input file: " << filein << std::endl;
   std::cout << " " << std::cout;
   std::cout << "Output file: " << savehistofile << std::endl;
   std::cout << " " << std::cout; 
   std::cout << "pT(Jet1): " << jet1PT << "GeV" << std::endl;
   std::cout << "pT(Jet2): " << jet2PT << "GeV" << std::endl;
   std::cout << "# Vertex: " << optnVertex << std::endl;
   std::cout << "Trigger Option: " << optTrigger << std::endl;
   std::cout << " " << std::endl;
   std::cout << "--> TRUE = 1 FALSE = 0" << std::endl;
   std::cout << "Lumi. Weight: " << switchWeightLumi << std::endl;
   std::cout << "Evt. - Evt. Weight: " << switchWeightePw << std::endl;
   std::cout << "Trigger Switch: " << switchTrigger << std::endl;
   std::cout << "Pre-Selection Switch: " << switchPreSel << std::endl;
   std::cout << "N Bins PU Distributions: " << nbins << std::endl;
   std::cout << " " << std::endl;
   std::cout << "--> Factors" << std::endl;
   std::cout << "Lumi. Weight: " << weightlumipass << std::endl;
   std::cout << "" << std::endl;


   // Code Protection
   if (optnVertex == 0){

      std::cout << "---------------------------------------------------------------" << std::endl;
      std::cout << "Please, restart your setup. Respect the Condition # Vertex > 0)" << std::endl;
      std::cout << "---------------------------------------------------------------" << std::endl;
      return;

   }

   TFile check1(filein.c_str());

   if (check1.IsZombie()){

      std::cout << "----------------------------------------------" << std::endl;
      std::cout << " There is no PatTuple file or the"   << std::endl;
      std::cout << " path is not correct." << std::endl;
      std::cout << " Edit the source and recompile." << std::endl;
      std::cout << "----------------------------------------------" << std::endl;
      return;

   }
   //--------------------------------------------------------------------------------------------------------------------------

   LoadFile(filein,processname);

   std::cout << " " << std::endl;
   std::cout << "pT(jet1) > " << jet1PT << std::endl;
   std::cout << "pT(jet2) > " << jet2PT << std::endl;
   std::cout << " " << std::endl;

   // Root file with histograms
   TFile *outf = new TFile(savehistofile.c_str(),"RECREATE");
   
   // File with Number of Events
   TString outtxt = savehistofile;
   outtxt.ReplaceAll("root","txt");  
   std::ofstream outstring(outtxt); 
   
   //int NEVENTS = tr->GetEntriesFast();
   int NEVENTS = tr->GetEntries();

   TH1::SetDefaultSumw2(true);
   TH2::SetDefaultSumw2(true);

   unsigned NEntries = tr->GetEntries();
   std::cout << "" << std::endl;
   std::cout<< "Reading Tree: "<< NEntries << " events"<<std::endl;
   std::cout << "" << std::endl;

   int decade = 0;

   //Protection Code
   for(int m=0;m<2;m++) {

       tr->GetEntry(m);

       if (eventexcl->GetNPileUpBx0()==-1 && eventexcl->GetNPileUpBxm1()==-1 && eventexcl->GetNPileUpBxp1()==-1 ){
        std::cout << "--------------------------------------------------------------" << std::endl;
        std::cout << " There is no Pile Up TTree information in your PATTuplefile."   << std::endl;
        std::cout << " Please, use another PATTuple with PU information to run mul- " << std::endl;
        std::cout << " tiple PU option." << std::endl;
        std::cout << "--------------------------------------------------------------" << std::endl;
        return;
       }

    }
    //--------------------------------------------------------------------------------------------------------------------------


   // Event by Event Analysis
   //////////////////////////

   int TotalE = 0;
   double counterTrigger = 0.;
   double counterJetsstep1 = 0.;
   double counterJetsstep2 = 0.;
   double counterJetsAllstep3 = 0.;
   double counterJetsAllstep4_4 = 0.;
   double counterJetsAllstep4_3 = 0.;
   double counterJetsAllstep4_2 = 0.;
   double counterJetsAllstep4_1 = 0.;
   double counterJetsTrackerstep3 = 0.;
   double counterJetsTrackerstep4_4 = 0.;
   double counterJetsTrackerstep4_3 = 0.;
   double counterJetsTrackerstep4_2 = 0.;
   double counterJetsTrackerstep4_1 = 0.;
   double counterJetsEta2step3 = 0.;
   double counterJetsEta2step4_4 = 0.;
   double counterJetsEta2step4_3 = 0.;
   double counterJetsEta2step4_2 = 0.;
   double counterJetsEta2step4_1 = 0.;

   std::vector <std::string> Folders;
   Folders.push_back("without_cuts");
   Folders.push_back("with_trigger");
   Folders.push_back("step1");
   Folders.push_back("step2");
   Folders.push_back("All_step3");
   Folders.push_back("All_step4_4");
   Folders.push_back("All_step4_3");
   Folders.push_back("All_step4_2");
   Folders.push_back("All_step4_1");
   Folders.push_back("Tracker_step3");
   Folders.push_back("Tracker_step4_4");
   Folders.push_back("Tracker_step4_3");
   Folders.push_back("Tracker_step4_2");
   Folders.push_back("Tracker_step4_1");
   Folders.push_back("JetsAtEta2_step3");
   Folders.push_back("JetsAtEta2_step4_4");
   Folders.push_back("JetsAtEta2_step4_3");
   Folders.push_back("JetsAtEta2_step4_2");
   Folders.push_back("JetsAtEta2_step4_1");

   for (int j=0; j<19; j++)
    {

       //m_hVector_pumcbx0.push_back(<TH1D*>);
       //m_hVector_pumcbxm1.push_back(<TH1D*>);
       //m_hVector_pumcbxp1.push_back(<TH1D*>);

       char name1[300];
       sprintf(name1,"pileUpBx0_complete_%s",Folders.at(j).c_str());
       TH1D *histo_pumcbx0 = new TH1D(name1,"PileUp Monte Carlo; # Pile Up; N events",nbins,0,nbins);
       m_hVector_pumcbx0.push_back(histo_pumcbx0);

       char name2[300];
       sprintf(name2,"pileUpBxm1_complete_%s",Folders.at(j).c_str());
       TH1D *histo_pumcbxm1 = new TH1D(name2,"PileUp Monte Carlo; # Pile Up; N events",nbins,0,nbins);
       m_hVector_pumcbxm1.push_back(histo_pumcbxm1);

       char name3[300];
       sprintf(name3,"pileUpBxp1_complete_%s",Folders.at(j).c_str());
       TH1D *histo_pumcbxp1 = new TH1D(name3,"PileUp Monte Carlo; # Pile Up; N events",nbins,0,nbins);
       m_hVector_pumcbxp1.push_back(histo_pumcbxp1);
       
    }

  for(int i=0;i<NEVENTS;i++) {

      double totalweight = -999.;
      ++TotalE;
        
      //----------- Progress Report -------------
      
      double progress = 10.0*i/(1.0*NEVENTS);
      int l = TMath::FloorNint(progress); 
      if (l > decade){
         std::cout <<"\n<<<<<< STATUS >>>>>>" << std::endl; 
	 std::cout<<10*l<<" % completed." << std::endl;
         std::cout <<"<<<<<<<<<<>>>>>>>>>>\n" << std::endl;
      }
      decade = l;          

      //----------- Read the Event --------------
      tr->GetEntry(i);

      double weightlumi;
      double weightepw;

      if (switchWeightLumi) { weightlumi = weightlumipass; }
      else { weightlumi = 1.0;}

      if (switchWeightePw) { weightepw = eventqcd->evtHdr().weight();}
      else { weightepw = 1.0;}

      totalweight = weightlumi*weightepw;

      if( i % 1000 == 0 ){
	    std::cout << "\nEvent " << i << std::endl
            << "Nr. events Bx 0   : " << eventexcl->GetNPileUpBx0() << std::endl
            << "Nr. events Bxm1   : " << eventexcl->GetNPileUpBxm1() << std::endl
            << "Nr. events Bxp1   : " << eventexcl->GetNPileUpBxp1() << std::endl
	    << "Lumi corr.        : " << weightlumi << std::endl
	    << "MC event weight   : " << weightepw << std::endl
	    << "Total weight  : " << totalweight << std::endl;
      }

      // Without Cuts          
      ////////////////////////////////////////////////
       m_hVector_pumcbx0.at(0)->Fill(eventexcl->GetNPileUpBx0(),totalweight);
       m_hVector_pumcbxm1.at(0)->Fill(eventexcl->GetNPileUpBxm1(),totalweight);
       m_hVector_pumcbxp1.at(0)->Fill(eventexcl->GetNPileUpBxp1(),totalweight);
      //////////////////////////////////////////////////


      //------------------------------------------------------------------------------------------
      //
      // SET TRIGGER OR TRIGGER EMULATOR!!!!
      //
      //
      // SIMULATED TRIGGER
      if ( !switchPreSel || (switchPreSel && eventexcl->GetSumEHFPFlowPlus() < 30 && eventexcl->GetSumEHFPFlowMinus() < 30 && eventexcl->GetLeadingJetP4().Pt() > 30 && eventexcl->GetSecondJetP4().Pt() > 30)){

         // TRIGGER
	 if (!switchTrigger || (switchTrigger && eventexcl->GetHLTPath(optTrigger))){
	 //
	 //------------------------------------------------------------------------------------------

	 counterTrigger+=totalweight;

	 // With Trigger: online or emulate          
	 ////////////////////////////////////////////////
         m_hVector_pumcbx0.at(1)->Fill(eventexcl->GetNPileUpBx0(),totalweight);
         m_hVector_pumcbxm1.at(1)->Fill(eventexcl->GetNPileUpBxm1(),totalweight);
         m_hVector_pumcbxp1.at(1)->Fill(eventexcl->GetNPileUpBxp1(),totalweight);
         //////////////////////////////////////////////////


	 if(eventexcl->GetNVertex() > 0 && eventexcl->GetNVertex()<= optnVertex){

            counterJetsstep1+=totalweight; 

	    // STEP1        
	    //////////////////////////////////////////////////
            m_hVector_pumcbx0.at(2)->Fill(eventexcl->GetNPileUpBx0(),totalweight);
            m_hVector_pumcbxm1.at(2)->Fill(eventexcl->GetNPileUpBxm1(),totalweight);
            m_hVector_pumcbxp1.at(2)->Fill(eventexcl->GetNPileUpBxp1(),totalweight);
            //////////////////////////////////////////////////


	    // Setting Analysis Cut
	    if (eventexcl->GetLeadingJetP4().Pt() > jet1PT && eventexcl->GetSecondJetP4().Pt() > jet2PT ){

	       counterJetsstep2+=totalweight;

	       // STEP2         
	       ////////////////////////////////////////////////
               m_hVector_pumcbx0.at(3)->Fill(eventexcl->GetNPileUpBx0(),totalweight);
               m_hVector_pumcbxm1.at(3)->Fill(eventexcl->GetNPileUpBxm1(),totalweight);
               m_hVector_pumcbxp1.at(3)->Fill(eventexcl->GetNPileUpBxp1(),totalweight);
               ////////////////////////////////////////////////
 
	       if (eventexcl->GetLeadingJetP4().Eta() < 5.2 && eventexcl->GetSecondJetP4().Eta() < 5.2 && eventexcl->GetLeadingJetP4().Eta() > -5.2 && eventexcl->GetSecondJetP4().Eta() > -5.2){

		  counterJetsAllstep3+=totalweight;

		  // ALL - STEP3         
		  ////////////////////////////////////////////////
                  m_hVector_pumcbx0.at(4)->Fill(eventexcl->GetNPileUpBx0(),totalweight);
		  m_hVector_pumcbxm1.at(4)->Fill(eventexcl->GetNPileUpBxm1(),totalweight);
		  m_hVector_pumcbxp1.at(4)->Fill(eventexcl->GetNPileUpBxp1(),totalweight);
		  //////////////////////////////////////////////////////////////////////////////////////////

		  // Eta max and Eta min cut
		  if (eventdiff->GetEtaMinFromPFCands() > -4. && eventdiff->GetEtaMaxFromPFCands() < 4.){

                     counterJetsAllstep4_4+=totalweight;
		     m_hVector_pumcbx0.at(5)->Fill(eventexcl->GetNPileUpBx0(),totalweight);
		     m_hVector_pumcbxm1.at(5)->Fill(eventexcl->GetNPileUpBxm1(),totalweight);
		     m_hVector_pumcbxp1.at(5)->Fill(eventexcl->GetNPileUpBxp1(),totalweight);
		  }

		  if (eventdiff->GetEtaMinFromPFCands() > -3. && eventdiff->GetEtaMaxFromPFCands() < 3.){

                     counterJetsAllstep4_3+=totalweight;
		     m_hVector_pumcbx0[6]->Fill(eventexcl->GetNPileUpBx0(),totalweight);
                     m_hVector_pumcbxm1[6]->Fill(eventexcl->GetNPileUpBxm1(),totalweight);
                     m_hVector_pumcbxp1[6]->Fill(eventexcl->GetNPileUpBxp1(),totalweight);
                  }

		  if (eventdiff->GetEtaMinFromPFCands() > -2. && eventdiff->GetEtaMaxFromPFCands() < 2.){

                     counterJetsAllstep4_2+=totalweight;
		     m_hVector_pumcbx0.at(7)->Fill(eventexcl->GetNPileUpBx0(),totalweight);
                     m_hVector_pumcbxm1.at(7)->Fill(eventexcl->GetNPileUpBxm1(),totalweight);
                     m_hVector_pumcbxp1.at(7)->Fill(eventexcl->GetNPileUpBxp1(),totalweight);
                  }

		  if (eventdiff->GetEtaMinFromPFCands() > -1. && eventdiff->GetEtaMaxFromPFCands() < 1.){

                     counterJetsAllstep4_1+=totalweight;
		     m_hVector_pumcbx0.at(8)->Fill(eventexcl->GetNPileUpBx0(),totalweight);
                     m_hVector_pumcbxm1.at(8)->Fill(eventexcl->GetNPileUpBxm1(),totalweight);
                     m_hVector_pumcbxp1.at(8)->Fill(eventexcl->GetNPileUpBxp1(),totalweight);
                   }

	       }  
	       // end Jets at all CMS Acceptance

	       if (eventexcl->GetLeadingJetP4().Eta() < 2.9 && eventexcl->GetSecondJetP4().Eta() < 2.9 && eventexcl->GetLeadingJetP4().Eta() > -2.9 && eventexcl->GetSecondJetP4().Eta() > -2.9){

		  counterJetsTrackerstep3+=totalweight;

		  // Tracker - STEP3         
		  ////////////////////////////////////////////////
                  m_hVector_pumcbx0.at(9)->Fill(eventexcl->GetNPileUpBx0(),totalweight);
                  m_hVector_pumcbxm1.at(9)->Fill(eventexcl->GetNPileUpBxm1(),totalweight);
                  m_hVector_pumcbxp1.at(9)->Fill(eventexcl->GetNPileUpBxp1(),totalweight);
                
                 // Eta max and Eta min cut
		  if (eventdiff->GetEtaMinFromPFCands() > -4. && eventdiff->GetEtaMaxFromPFCands() < 4.){

                     counterJetsTrackerstep4_4+=totalweight;
                     m_hVector_pumcbx0.at(10)->Fill(eventexcl->GetNPileUpBx0(),totalweight);
                     m_hVector_pumcbxm1.at(10)->Fill(eventexcl->GetNPileUpBxm1(),totalweight);
                     m_hVector_pumcbxp1.at(10)->Fill(eventexcl->GetNPileUpBxp1(),totalweight);
                  }

		  if (eventdiff->GetEtaMinFromPFCands() > -3. && eventdiff->GetEtaMaxFromPFCands() < 3.){

                     counterJetsTrackerstep4_3+=totalweight;
                     m_hVector_pumcbx0.at(11)->Fill(eventexcl->GetNPileUpBx0(),totalweight);
                     m_hVector_pumcbxm1.at(11)->Fill(eventexcl->GetNPileUpBxm1(),totalweight);
                     m_hVector_pumcbxp1.at(11)->Fill(eventexcl->GetNPileUpBxp1(),totalweight);
                  }

		  if (eventdiff->GetEtaMinFromPFCands() > -2. && eventdiff->GetEtaMaxFromPFCands() < 2.){

                     counterJetsTrackerstep4_2+=totalweight;
                     m_hVector_pumcbx0.at(12)->Fill(eventexcl->GetNPileUpBx0(),totalweight);
                     m_hVector_pumcbxm1.at(12)->Fill(eventexcl->GetNPileUpBxm1(),totalweight);
                     m_hVector_pumcbxp1.at(12)->Fill(eventexcl->GetNPileUpBxp1(),totalweight);
                  }

		  if (eventdiff->GetEtaMinFromPFCands() > -1. && eventdiff->GetEtaMaxFromPFCands() < 1.){

                     counterJetsTrackerstep4_1+=totalweight;
                     m_hVector_pumcbx0.at(13)->Fill(eventexcl->GetNPileUpBx0(),totalweight);
                     m_hVector_pumcbxm1.at(13)->Fill(eventexcl->GetNPileUpBxm1(),totalweight);
                     m_hVector_pumcbxp1.at(13)->Fill(eventexcl->GetNPileUpBxp1(),totalweight);
                  }

	       } // end jets at tracker


	       if (eventexcl->GetLeadingJetP4().Eta() < 2. && eventexcl->GetSecondJetP4().Eta() < 2. && eventexcl->GetLeadingJetP4().Eta() > -2. && eventexcl->GetSecondJetP4().Eta() > -2.){

		  counterJetsEta2step3+=totalweight;

		  // JetsEta2 - STEP3         
		  ////////////////////////////////////////////////
		  m_hVector_pumcbx0.at(14)->Fill(eventexcl->GetNPileUpBx0(),totalweight);
                  m_hVector_pumcbxm1.at(14)->Fill(eventexcl->GetNPileUpBxm1(),totalweight);
                  m_hVector_pumcbxp1.at(14)->Fill(eventexcl->GetNPileUpBxp1(),totalweight);
                
		  // Eta max and Eta min cut
		  if (eventdiff->GetEtaMinFromPFCands() > -4. && eventdiff->GetEtaMaxFromPFCands() < 4.){

                     counterJetsEta2step4_4+=totalweight;
                     m_hVector_pumcbx0.at(15)->Fill(eventexcl->GetNPileUpBx0(),totalweight);
                     m_hVector_pumcbxm1.at(15)->Fill(eventexcl->GetNPileUpBxm1(),totalweight);
                     m_hVector_pumcbxp1.at(15)->Fill(eventexcl->GetNPileUpBxp1(),totalweight);
                  }

		  if (eventdiff->GetEtaMinFromPFCands() > -3. && eventdiff->GetEtaMaxFromPFCands() < 3.){

                     counterJetsEta2step4_3+=totalweight;
                     m_hVector_pumcbx0.at(16)->Fill(eventexcl->GetNPileUpBx0(),totalweight);
                     m_hVector_pumcbxm1.at(16)->Fill(eventexcl->GetNPileUpBxm1(),totalweight);
                     m_hVector_pumcbxp1.at(16)->Fill(eventexcl->GetNPileUpBxp1(),totalweight);
                  }

		  if (eventdiff->GetEtaMinFromPFCands() > -2. && eventdiff->GetEtaMaxFromPFCands() < 2.){

                     counterJetsEta2step4_2+=totalweight;
                     m_hVector_pumcbx0.at(17)->Fill(eventexcl->GetNPileUpBx0(),totalweight);
                     m_hVector_pumcbxm1.at(17)->Fill(eventexcl->GetNPileUpBxm1(),totalweight);
                     m_hVector_pumcbxp1.at(17)->Fill(eventexcl->GetNPileUpBxp1(),totalweight);
                  }

		  if (eventdiff->GetEtaMinFromPFCands() > -1. && eventdiff->GetEtaMaxFromPFCands() < 1.){

                     counterJetsEta2step4_1+=totalweight;
                     m_hVector_pumcbx0.at(18)->Fill(eventexcl->GetNPileUpBx0(),totalweight);
                     m_hVector_pumcbxm1.at(18)->Fill(eventexcl->GetNPileUpBxm1(),totalweight);
                     m_hVector_pumcbxp1.at(18)->Fill(eventexcl->GetNPileUpBxp1(),totalweight);
                  }

	       } // end jets at JetsEta2

	    } // Jets Cuts

	 } // If nVertex
       
       } // Trigger  

     } // PreSelection

   }// Run All Events

     outstring << "" << std::endl;
     outstring << "<< INPUTS >>" << std::endl;
     outstring << " " << std::endl;
     outstring << "Input file: " << filein << std::endl;
     outstring << "Output file: " << savehistofile << std::endl;
     outstring << " " << std::endl;
     outstring << "pT(Jet1): " << jet1PT << "GeV" << std::endl;
     outstring << "pT(Jet2): " << jet2PT << "GeV" << std::endl;
     outstring << "# Vertex: " << optnVertex << std::endl;
     outstring << "Trigger Option: " << optTrigger << std::endl;
     outstring << " " << std::endl;
     outstring << "--> TRUE = 1 FALSE = 0" << std::endl;
     outstring << "Lumi. Weight: " << switchWeightLumi << std::endl;
     outstring << "Evt. - Evt. Weight: " << switchWeightePw << std::endl;
     outstring << "Trigger Switch: " << switchTrigger << std::endl;
     outstring << "Pre-Selection Switch: " << switchPreSel << std::endl;
     outstring << "N Bins PU Distributions: " << nbins << std::endl;
     outstring << " " << std::endl;
     outstring << "--> Factors" << std::endl;
     outstring << "Lumi. Weight: " << weightlumipass << std::endl;
     outstring << "" << std::endl;
     outstring << "<< EVENT INFO >> " << std::endl;
     outstring << " " << std::endl;
     outstring << "Total # of Events without Weight: " << TotalE << std::endl;
     outstring << " " << std::endl;
     outstring << "Triggered: " << counterTrigger << std::endl;
     outstring << "STEP1: " << counterJetsstep1 << std::endl;
     outstring << "STEP2: " << counterJetsstep2 << std::endl;
     outstring << "STEP3 (CMS Acceptance): " << counterJetsAllstep3 << std::endl;
     outstring << "STEP4_4 (CMS Acceptance): " << counterJetsAllstep4_4 << std::endl;
     outstring << "STEP4_3 (CMS Acceptance): " << counterJetsAllstep4_3 << std::endl;
     outstring << "STEP4_2 (CMS Acceptance): " << counterJetsAllstep4_2 << std::endl;
     outstring << "STEP4_1 (CMS Acceptance): " << counterJetsAllstep4_1 << std::endl;
     outstring << " " << std::endl;
     outstring << "STEP3 (Tracker Acceptance): " << counterJetsTrackerstep3 << std::endl;
     outstring << "STEP4_4 (Tracker Acceptance): " << counterJetsTrackerstep4_4 << std::endl;
     outstring << "STEP4_3 (Tracker Acceptance): " << counterJetsTrackerstep4_3 << std::endl;
     outstring << "STEP4_2 (Tracker Acceptance): " << counterJetsTrackerstep4_2 << std::endl;
     outstring << "STEP4_1 (Tracker Acceptance): " << counterJetsTrackerstep4_1 << std::endl;
     outstring << " " << std::endl;
     outstring << "STEP3 (eta < 2 and eta > -2 Acceptance): " << counterJetsEta2step3 << std::endl;
     outstring << "STEP4_4 (eta < 2 and eta > -2 Acceptance): " << counterJetsEta2step4_4 << std::endl;
     outstring << "STEP4_3 (eta < 2 and eta > -2 Acceptance): " << counterJetsEta2step4_3 << std::endl;
     outstring << "STEP4_2 (eta < 2 and eta > -2 Acceptance): " << counterJetsEta2step4_2 << std::endl;
     outstring << "STEP4_1 (eta < 2 and eta > -2 Acceptance): " << counterJetsEta2step4_1 << std::endl;
     outstring << " " << std::endl;
     outstring << "<< LEGEND >> " << std::endl;
     outstring << "STEP1: # Vertex selection." << std::endl;
     outstring << "STEP2: # Vertex selection + Two Jets." << std::endl;
     outstring << "STEP3: # Vertex selection + Two Jets + Jets in Tracker Acceptance or All CMS." << std::endl;
     outstring << "STEP4_X: # Vertex selection + Two Jets + Jets in Tracker Acceptance or All CMS + Eta_max < X and Eta_min > X." << std::endl;
     outstring << " " << std::endl;

   outf->Write();
   outf->Close();
   outstring.close();

}

#if !defined(__CINT__) || defined(__MAKECINT__)
#include "FWCore/FWLite/interface/AutoLibraryLoader.h"
int main(int argc, char **argv)
{
   AutoLibraryLoader::enable();

   const char *s1="*";
   std::string filein_;
   std::string savehistofile_;
   std::string processname_;
   double jet1PT_;
   double jet2PT_;
   int optnVertex_;
   int optTrigger_;
   bool switchWeightLumi_;
   bool switchWeightePw_;
   bool switchPreSel_;
   bool switchTrigger_;
   double weightlumipass_;
   int nbins_;

   if (argc > 1 && strcmp(s1,argv[1]) != 0)  filein_ = argv[1];
   if (argc > 2 && strcmp(s1,argv[2]) != 0)  savehistofile_  = argv[2];
   if (argc > 3 && strcmp(s1,argv[3]) != 0)  processname_  = argv[3];
   if (argc > 4 && strcmp(s1,argv[4]) != 0)  jet1PT_  = atoi(argv[4]);
   if (argc > 5 && strcmp(s1,argv[5]) != 0)  jet2PT_ = atoi(argv[5]);
   if (argc > 6 && strcmp(s1,argv[6]) != 0)  optnVertex_ = atoi(argv[6]);
   if (argc > 7 && strcmp(s1,argv[7]) != 0)  optTrigger_   = atoi(argv[7]);
   if (argc > 8 && strcmp(s1,argv[8]) != 0)  switchWeightLumi_ = atoi(argv[8]);
   if (argc > 9 && strcmp(s1,argv[9]) != 0)  switchWeightePw_   = atoi(argv[9]);
   if (argc > 10 && strcmp(s1,argv[10]) != 0)  switchPreSel_   = atoi(argv[10]);
   if (argc > 11 && strcmp(s1,argv[11]) != 0)  switchTrigger_   = atoi(argv[11]);
   if (argc > 12 && strcmp(s1,argv[12]) != 0)  weightlumipass_  = atof(argv[12]);
   if (argc > 13 && strcmp(s1,argv[13]) != 0)  nbins_ = atoi(argv[13]);


   MC_PU_Distributions* exclDijets = new MC_PU_Distributions();   
   exclDijets->Run(filein_, savehistofile_, processname_, jet1PT_, jet2PT_, optnVertex_, optTrigger_, switchWeightLumi_, switchWeightePw_, switchPreSel_, switchTrigger_, weightlumipass_, nbins_);

   return 0;
}
#endif
