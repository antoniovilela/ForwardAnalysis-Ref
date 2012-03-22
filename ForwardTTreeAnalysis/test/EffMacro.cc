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
// $> ./EffMacroCom "Inputfile.root" "outputfile.root" "CMSSW Process_Name/TTree_name"<pT(Jet1) Cut> <pT(Jet2) Cut> <Number of Vertex Cut> <Trigger Option> <Turn on(off) Pre Selection> <Turn on(off) Trigger> <# bins PU Distribution>
//
// TURN ON  = 1
// TURN OFF = 0
//
//
// EXAMPLE: ./EffMacroCom "inputfile.root" "outputfile.root" "forwardQCDTTreeAnalysis/ProcessedTree" 60 55 1 1 1 1 25
//
// Twiki: https://twiki.cern.ch/twiki/bin/view/CMS/FwdPhysicsExclusiveDijetsAnalysis
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

#include "EffMacro.h"
#include "ForwardAnalysis/ForwardTTreeAnalysis/interface/ExclusiveDijetsEvent.h"
#include "ForwardAnalysis/ForwardTTreeAnalysis/interface/DiffractiveEvent.h"
#include "KKousour/QCDAnalysis/interface/QCDEvent.h"
#include "PhysicsTools/Utilities/interface/LumiReWeighting.h"

using namespace diffractiveAnalysis;
using namespace exclusiveDijetsAnalysis;
using namespace reweight;

void EffMacro::LoadFile(std::string fileinput, std::string processinput){

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

void EffMacro::Run(std::string filein_, std::string savehistofile_, std::string processname_, double jet1PT_, double jet2PT_, int optnVertex_, int optTrigger_, bool switchPreSel_, bool switchTrigger_, int nbins_){

   filein = filein_;
   savehistofile = savehistofile_;
   processname = processname_;
   filein = filein_;
   jet1PT = jet1PT_;
   jet2PT = jet2PT_;
   optnVertex = optnVertex_;
   optTrigger = optTrigger_;
   switchPreSel = switchPreSel_;
   switchTrigger = switchTrigger_;
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
   std::cout << "Trigger Switch: " << switchTrigger << std::endl;
   std::cout << "Pre-Selection Switch: " << switchPreSel << std::endl;
   std::cout << "N Bins: " << nbins << std::endl;
   std::cout << " " << std::endl;


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

   //--------------------------------------------------------------------------------------------------------------------------


   // Event by Event Analysis
   //////////////////////////

   double TotalE = 0.;
   double counterTrigger=0.;
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

       char name1[300];
       sprintf(name1,"Events_Lumis_complete_%s",Folders.at(j).c_str());
       TH1D *histo_m_Evt_lumis = new TH1D(name1,"; Lumis; N events",nbins,0,nbins);
       m_hVector_Evt_lumis.push_back(histo_m_Evt_lumis);

       char name2[300];
       sprintf(name2,"Eff_complete_%s",Folders.at(j).c_str());
       TH1D *histo_m_Eff_lumis = new TH1D(name2,"; Lumis; Efficiency",nbins,0,nbins);
       m_hVector_Eff_lumis.push_back(histo_m_Eff_lumis);
       
    }

  for(int i=0;i<NEVENTS;i++) {

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

      if( i % 1000 == 0 ){
	    std::cout << "\nEvent " << i << std::endl;
      }

      // Without Cuts          
      ////////////////////////////////////////////////
       m_hVector_Evt_lumis.at(0)->Fill(eventexcl->GetInstLumiBunch());
       m_hVector_Eff_lumis.at(0)->Fill(eventexcl->GetInstLumiBunch());
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

	 ++counterTrigger;

	 // With Trigger: online or emulate          
	 ////////////////////////////////////////////////
         m_hVector_Evt_lumis.at(1)->Fill(eventexcl->GetInstLumiBunch());
         m_hVector_Eff_lumis.at(1)->Fill(eventexcl->GetInstLumiBunch());
         //////////////////////////////////////////////////


	 if(eventexcl->GetNVertex() > 0 && eventexcl->GetNVertex()<= optnVertex){

            ++counterJetsstep1; 

	    // STEP1        
	    //////////////////////////////////////////////////
            m_hVector_Evt_lumis.at(2)->Fill(eventexcl->GetInstLumiBunch());
            m_hVector_Eff_lumis.at(2)->Fill(eventexcl->GetInstLumiBunch());
            //////////////////////////////////////////////////


	    // Setting Analysis Cut
	    if (eventexcl->GetLeadingJetP4().Pt() > jet1PT && eventexcl->GetSecondJetP4().Pt() > jet2PT ){

	       ++counterJetsstep2;

	       // STEP2         
	       ////////////////////////////////////////////////
               m_hVector_Evt_lumis.at(3)->Fill(eventexcl->GetInstLumiBunch());
               m_hVector_Eff_lumis.at(3)->Fill(eventexcl->GetInstLumiBunch());
               ////////////////////////////////////////////////
 
	       if (eventexcl->GetLeadingJetP4().Eta() < 5.2 && eventexcl->GetSecondJetP4().Eta() < 5.2 && eventexcl->GetLeadingJetP4().Eta() > -5.2 && eventexcl->GetSecondJetP4().Eta() > -5.2){

		  ++counterJetsAllstep3;

		  // ALL - STEP3         
		  ////////////////////////////////////////////////
                  m_hVector_Evt_lumis.at(4)->Fill(eventexcl->GetInstLumiBunch());
		  m_hVector_Eff_lumis.at(4)->Fill(eventexcl->GetInstLumiBunch());
		  //////////////////////////////////////////////////////////////////////////////////////////

		  // Eta max and Eta min cut
		  if (eventdiff->GetEtaMinFromPFCands() > -4. && eventdiff->GetEtaMaxFromPFCands() < 4.){

                     ++counterJetsAllstep4_4;
		     m_hVector_Evt_lumis.at(5)->Fill(eventexcl->GetInstLumiBunch());
		     m_hVector_Eff_lumis.at(5)->Fill(eventexcl->GetInstLumiBunch());
		  }

		  if (eventdiff->GetEtaMinFromPFCands() > -3. && eventdiff->GetEtaMaxFromPFCands() < 3.){

                     ++counterJetsAllstep4_3;
		     m_hVector_Evt_lumis[6]->Fill(eventexcl->GetInstLumiBunch());
                     m_hVector_Eff_lumis[6]->Fill(eventexcl->GetInstLumiBunch());
                  }

		  if (eventdiff->GetEtaMinFromPFCands() > -2. && eventdiff->GetEtaMaxFromPFCands() < 2.){

                     ++counterJetsAllstep4_2;
		     m_hVector_Evt_lumis.at(7)->Fill(eventexcl->GetInstLumiBunch());
                     m_hVector_Eff_lumis.at(7)->Fill(eventexcl->GetInstLumiBunch());
                  }

		  if (eventdiff->GetEtaMinFromPFCands() > -1. && eventdiff->GetEtaMaxFromPFCands() < 1.){

                     ++counterJetsAllstep4_1;
		     m_hVector_Evt_lumis.at(8)->Fill(eventexcl->GetInstLumiBunch());
                     m_hVector_Eff_lumis.at(8)->Fill(eventexcl->GetInstLumiBunch());
                   }

	       }  
	       // end Jets at all CMS Acceptance

	       if (eventexcl->GetLeadingJetP4().Eta() < 2.9 && eventexcl->GetSecondJetP4().Eta() < 2.9 && eventexcl->GetLeadingJetP4().Eta() > -2.9 && eventexcl->GetSecondJetP4().Eta() > -2.9){

		  ++counterJetsTrackerstep3;

		  // Tracker - STEP3         
		  ////////////////////////////////////////////////
                  m_hVector_Evt_lumis.at(9)->Fill(eventexcl->GetInstLumiBunch());
                  m_hVector_Eff_lumis.at(9)->Fill(eventexcl->GetInstLumiBunch());
                
                 // Eta max and Eta min cut
		  if (eventdiff->GetEtaMinFromPFCands() > -4. && eventdiff->GetEtaMaxFromPFCands() < 4.){

                     ++counterJetsTrackerstep4_4;
                     m_hVector_Evt_lumis.at(10)->Fill(eventexcl->GetInstLumiBunch());
                     m_hVector_Eff_lumis.at(10)->Fill(eventexcl->GetInstLumiBunch());
                  }

		  if (eventdiff->GetEtaMinFromPFCands() > -3. && eventdiff->GetEtaMaxFromPFCands() < 3.){

                     ++counterJetsTrackerstep4_3;
                     m_hVector_Evt_lumis.at(11)->Fill(eventexcl->GetInstLumiBunch());
                     m_hVector_Eff_lumis.at(11)->Fill(eventexcl->GetInstLumiBunch());
                  }

		  if (eventdiff->GetEtaMinFromPFCands() > -2. && eventdiff->GetEtaMaxFromPFCands() < 2.){

                     ++counterJetsTrackerstep4_2;
                     m_hVector_Evt_lumis.at(12)->Fill(eventexcl->GetInstLumiBunch());
                     m_hVector_Eff_lumis.at(12)->Fill(eventexcl->GetInstLumiBunch());
                  }

		  if (eventdiff->GetEtaMinFromPFCands() > -1. && eventdiff->GetEtaMaxFromPFCands() < 1.){

                     ++counterJetsTrackerstep4_1;
                     m_hVector_Evt_lumis.at(13)->Fill(eventexcl->GetInstLumiBunch());
                     m_hVector_Eff_lumis.at(13)->Fill(eventexcl->GetInstLumiBunch());
                  }

	       } // end jets at tracker


	       if (eventexcl->GetLeadingJetP4().Eta() < 2. && eventexcl->GetSecondJetP4().Eta() < 2. && eventexcl->GetLeadingJetP4().Eta() > -2. && eventexcl->GetSecondJetP4().Eta() > -2.){

		  ++counterJetsEta2step3;

		  // JetsEta2 - STEP3         
		  ////////////////////////////////////////////////
		  m_hVector_Evt_lumis.at(14)->Fill(eventexcl->GetInstLumiBunch());
                  m_hVector_Eff_lumis.at(14)->Fill(eventexcl->GetInstLumiBunch());
                
		  // Eta max and Eta min cut
		  if (eventdiff->GetEtaMinFromPFCands() > -4. && eventdiff->GetEtaMaxFromPFCands() < 4.){

                     ++counterJetsEta2step4_4;
                     m_hVector_Evt_lumis.at(15)->Fill(eventexcl->GetInstLumiBunch());
                     m_hVector_Eff_lumis.at(15)->Fill(eventexcl->GetInstLumiBunch());
                  }

		  if (eventdiff->GetEtaMinFromPFCands() > -3. && eventdiff->GetEtaMaxFromPFCands() < 3.){

                     ++counterJetsEta2step4_3;
                     m_hVector_Evt_lumis.at(16)->Fill(eventexcl->GetInstLumiBunch());
                     m_hVector_Eff_lumis.at(16)->Fill(eventexcl->GetInstLumiBunch());
                  }

		  if (eventdiff->GetEtaMinFromPFCands() > -2. && eventdiff->GetEtaMaxFromPFCands() < 2.){

                     ++counterJetsEta2step4_2;
                     m_hVector_Evt_lumis.at(17)->Fill(eventexcl->GetInstLumiBunch());
                     m_hVector_Eff_lumis.at(17)->Fill(eventexcl->GetInstLumiBunch());
                  }

		  if (eventdiff->GetEtaMinFromPFCands() > -1. && eventdiff->GetEtaMaxFromPFCands() < 1.){

                     ++counterJetsEta2step4_1;
                     m_hVector_Evt_lumis.at(18)->Fill(eventexcl->GetInstLumiBunch());
                     m_hVector_Eff_lumis.at(18)->Fill(eventexcl->GetInstLumiBunch());
                  }

	       } // end jets at JetsEta2

	    } // Jets Cuts

	 } // If nVertex
       
       } // Trigger  

     } // PreSelection

   }// Run All Events

  //Scalling Plots
   for (int k=0; k<19; k++){
     m_hVector_Eff_lumis.at(k)->Scale(1./TotalE);
   }

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
     outstring << "Trigger Switch: " << switchTrigger << std::endl;
     outstring << "Pre-Selection Switch: " << switchPreSel << std::endl;
     outstring << "N Bins PU: " << nbins << std::endl;
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
   bool switchPreSel_;
   bool switchTrigger_;
   int nbins_;

   if (argc > 1 && strcmp(s1,argv[1]) != 0)  filein_ = argv[1];
   if (argc > 2 && strcmp(s1,argv[2]) != 0)  savehistofile_  = argv[2];
   if (argc > 3 && strcmp(s1,argv[3]) != 0)  processname_  = argv[3];
   if (argc > 4 && strcmp(s1,argv[4]) != 0)  jet1PT_  = atoi(argv[4]);
   if (argc > 5 && strcmp(s1,argv[5]) != 0)  jet2PT_ = atoi(argv[5]);
   if (argc > 6 && strcmp(s1,argv[6]) != 0)  optnVertex_ = atoi(argv[6]);
   if (argc > 7 && strcmp(s1,argv[7]) != 0)  optTrigger_   = atoi(argv[7]);
   if (argc > 8 && strcmp(s1,argv[8]) != 0)  switchPreSel_   = atoi(argv[8]);
   if (argc > 9 && strcmp(s1,argv[9]) != 0)  switchTrigger_   = atoi(argv[9]);
   if (argc > 10 && strcmp(s1,argv[10]) != 0)  nbins_ = atoi(argv[10]);


   EffMacro* exclDijets = new EffMacro();   
   exclDijets->Run(filein_, savehistofile_, processname_, jet1PT_, jet2PT_, optnVertex_, optTrigger_, switchPreSel_, switchTrigger_, nbins_);

   return 0;
}
#endif
