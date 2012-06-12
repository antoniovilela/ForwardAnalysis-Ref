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
// $> ./EffMacroTriggerCom "Inputfile.root" "outputfile.root" "CMSSW Process_Name/TTree_name"<pT(Jet1) Cut> <pT(Jet2) Cut> <Number of Vertex Cut> <Trigger Option> <Turn on(off) Pre Selection> <Turn on(off) Trigger> <# bins PU Distribution>
//
// TURN ON  = 1
// TURN OFF = 0
//
//
// EXAMPLE: ./EffMacroTriggerCom "inputfile.root" "outputfile.root" "forwardQCDTTreeAnalysis/ProcessedTree" 60 55 1 1 1 1 25
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

#include "EffMacroTrigger.h"
//#include "ForwardAnalysis/ForwardTTreeAnalysis/interface/ExclusiveDijetsEvent.h"
//#include "ForwardAnalysis/ForwardTTreeAnalysis/interface/DiffractiveEvent.h"
//#include "KKousour/QCDAnalysis/interface/QCDEvent.h"
#include "ForwardAnalysis/ForwardTTreeAnalysis/interface/DijetsTriggerEvent.h"
#include "PhysicsTools/Utilities/interface/LumiReWeighting.h"

//using namespace diffractiveAnalysis;
//using namespace exclusiveDijetsAnalysis;
using namespace dijetsTriggerAnalysis;
using namespace reweight;

void EffMacroTrigger::LoadFile(std::string fileinput, std::string processinput){

   inf = NULL;
   tr  = NULL;
   inf = TFile::Open(fileinput.c_str(),"read");
   tr = (TTree*)inf->Get(processinput.c_str());

   eventtrigger = new DijetsTriggerEvent();

   trigger = tr ->GetBranch("DijetsTriggerAnalysis");

   trigger->SetAddress(&eventtrigger);

}

void EffMacroTrigger::Run(std::string filein_, std::string savehistofile_, std::string processname_, double jet1PT_, double jet2PT_, int optL1Ref_, int optTriggerRef_, int optTrigger_,  bool switchPreSel_, bool switchTriggerRef_, bool switchTrigger_, bool switchL1Ref_, int nbins_){

   filein = filein_;
   savehistofile = savehistofile_;
   processname = processname_;
   filein = filein_;
   jet1PT = jet1PT_;
   jet2PT = jet2PT_;
   //optnVertex = optnVertex_;
   optL1Ref=optL1Ref_;
   optTriggerRef= optTriggerRef_;
   optTrigger = optTrigger_;
   switchPreSel = switchPreSel_;
   switchTriggerRef = switchTriggerRef_;
   switchTrigger = switchTrigger_;
   switchL1Ref = switchL1Ref_;
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
   //std::cout << "# Vertex: " << optnVertex << std::endl;
   std::cout << "L1 Ref. Option: " << optL1Ref << std::endl;
   std::cout << "Trigger Ref. Option: " << optTriggerRef << std::endl;
   std::cout << "Trigger Option: " << optTrigger << std::endl;
   std::cout << " " << std::endl;
   std::cout << "--> TRUE = 1 FALSE = 0" << std::endl;
   std::cout << "Trigger Ref. Switch: " << switchTriggerRef << std::endl;
   std::cout << "Trigger Switch: " << switchTrigger << std::endl;
   std::cout << "Pre-Selection Switch: " << switchPreSel << std::endl;
   std::cout << "L1 Ref. Switch: " << switchL1Ref << std::endl;
   std::cout << "N Bins: " << nbins << std::endl;
   std::cout << " " << std::endl;



   TFile check1(filein.c_str());

   if (check1.IsZombie()){

      std::cout << "----------------------------------------------" << std::endl;
      std::cout << " There is no nTuple file or the"   << std::endl;
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

   int TotalE = 0;
   int counterL1Trigger=0;
   int counterTriggerRef = 0;
   int counterPreSel = 0;
   int counterJetsstep1 = 0;
   int counterJetsstep2 = 0;
   int counterJetsAllstep3 = 0;
   int counterJetsEta2step3 = 0;
   double deltaphi_ = 0.;

   std::vector <std::string> Folders;
   Folders.push_back("without_cuts");
   Folders.push_back("with_trigger");
   Folders.push_back("step1");
   Folders.push_back("step2");
   Folders.push_back("All_step3");
   Folders.push_back("JetsAtEta2_step3");


   for (int j=0; j<6; j++)
    {

       char name1[300];
       sprintf(name1,"Events_Lumis_complete_%s",Folders.at(j).c_str());
       TH1D *histo_m_Evt_lumis = new TH1D(name1,"; Lumis; N events",nbins,0,nbins);
       m_hVector_Evt_lumis.push_back(histo_m_Evt_lumis);

       char name2[300];
       sprintf(name2,"Eff_complete_%s",Folders.at(j).c_str());
       TH1D *histo_m_Eff_lumis = new TH1D(name2,"; Lumis; Efficiency",nbins,0,nbins);
       m_hVector_Eff_lumis.push_back(histo_m_Eff_lumis);

       char name3[300];
       sprintf(name3,"pTJet1_complete_%s",Folders.at(j).c_str());
       TH1D *histo_ptjet1 = new TH1D(name3,"Leading Jet - P_{T} Distribution; P_{T} [GeV.c^{-1}]; N events",100,0,2000);
       m_hVector_ptjet1.push_back(histo_ptjet1);

       char name4[300];
       sprintf(name4,"pTJet2_complete_%s",Folders.at(j).c_str());
       TH1D *histo_ptjet2 = new TH1D(name4,"Second Jet - P_{T} Distribution; P_{T} [GeV.c^{-1}]; N events",100,0,2000);
       m_hVector_ptjet2.push_back(histo_ptjet2);

       char name5[300];
       sprintf(name5,"etaJet1_complete_%s",Folders.at(j).c_str());
       TH1D *histo_etajet1 = new TH1D(name5,"Leading Jet - #eta Distribution; #eta; N events",50,-5.2,5.2);
       m_hVector_etajet1.push_back(histo_etajet1);

       char name6[300];
       sprintf(name6,"etaJet2_complete_%s",Folders.at(j).c_str());
       TH1D *histo_etajet2 = new TH1D(name6,"Second Jet - #eta Distribution; #eta; N events",50,-5.2,5.2);
       m_hVector_etajet2.push_back(histo_etajet2);

       char name7[300];
       sprintf(name7,"phiJet1_complete_%s",Folders.at(j).c_str());
       TH1D *histo_phijet1 = new TH1D(name7,"Leading Jet - #phi Distribution; #phi [rad]; N events",50,-3.3,3.3);
       m_hVector_phijet1.push_back(histo_phijet1);

       char name8[300];
       sprintf(name8,"phiJet2_complete_%s",Folders.at(j).c_str());
       TH1D *histo_phijet2 = new TH1D(name8,"Second Jet - #phi Distribution; #phi [rad]; N events",50,-3.3,3.3);
       m_hVector_phijet2.push_back(histo_phijet2);
  
       char name9[300];
       sprintf(name9,"deltaPhiJets_complete_%s",Folders.at(j).c_str());
       TH1D *histo_deltaphijets = new TH1D(name9,"#Delta#phi_{jj} Distribution; #Delta#phi_{jj}; N events",20,0.0,3.2);
       m_hVector_deltaphijets.push_back(histo_deltaphijets);
       
       char name10[300];
       sprintf(name10,"sumEHFplus_complete_%s",Folders.at(j).c_str());
       TH1D *histo_sumEHFplus = new TH1D(name10,"HF^{+} - Sum of Energy; #sum E_{HF^{+}} [GeV]; N events",100,0,2000);
       m_hVector_sumEHFplus.push_back(histo_sumEHFplus);
   
       char name11[300];
       sprintf(name11,"sumEHFminus_complete_%s",Folders.at(j).c_str());
       TH1D *histo_sumEHFminus = new TH1D(name11,"HF^{-} - Sum of Energy; #sum E_{HF^{-}} [GeV]; N events",100,0,2000);
       m_hVector_sumEHFminus.push_back(histo_sumEHFminus);
       
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
     
      //
      ////////////////////////////////////////////////
      deltaphi_ = fabs(eventtrigger->GetLeadingJetPhi()-eventtrigger->GetSecondJetPhi());
      ////////////////////////////////////////////////

      // Without Cuts          
      ////////////////////////////////////////////////
       m_hVector_Evt_lumis.at(0)->Fill(eventtrigger->GetInstLumiBunch());
       m_hVector_Eff_lumis.at(0)->Fill(eventtrigger->GetInstLumiBunch());
       m_hVector_ptjet1.at(0)->Fill(eventtrigger->GetLeadingJetPt());
       m_hVector_ptjet2.at(0)->Fill(eventtrigger->GetSecondJetPt());
       m_hVector_etajet1.at(0)->Fill(eventtrigger->GetLeadingJetEta());
       m_hVector_etajet2.at(0)->Fill(eventtrigger->GetSecondJetEta());
       m_hVector_phijet1.at(0)->Fill(eventtrigger->GetLeadingJetPhi());
       m_hVector_phijet2.at(0)->Fill(eventtrigger->GetSecondJetPhi());
       m_hVector_sumEHFplus.at(0)->Fill(eventtrigger->GetSumEHFPlus());
       m_hVector_sumEHFminus.at(0)->Fill(eventtrigger->GetSumEHFMinus());
       m_hVector_deltaphijets.at(0)->Fill(deltaphi_);
      //////////////////////////////////////////////////
       
      //------------------------------------------------------------------------------------------
      //
      // SET TRIGGER OR TRIGGER EMULATOR!!!!
      //
      //

      // REF. TRIGGER
      if (!switchTriggerRef || (switchTriggerRef && eventtrigger->GetHLTPath(optTriggerRef))){

         ++counterTriggerRef;

         // L1 TRIGGER
         if (!switchL1Ref || (switchL1Ref && eventtrigger->GetL1Bit(optL1Ref) == 1)){

           ++counterL1Trigger;

         // Pre Selection
           if ( !switchPreSel || (switchPreSel && eventtrigger->GetSumEHFPlus() < 35 && eventtrigger->GetSumEHFMinus() < 35)){
 
   	     ++counterPreSel;

	 // With Trigger: online or emulate          
	 ////////////////////////////////////////////////
         m_hVector_Evt_lumis.at(1)->Fill(eventtrigger->GetInstLumiBunch());
         m_hVector_Eff_lumis.at(1)->Fill(eventtrigger->GetInstLumiBunch());
         m_hVector_ptjet1.at(1)->Fill(eventtrigger->GetLeadingJetPt());
         m_hVector_ptjet2.at(1)->Fill(eventtrigger->GetSecondJetPt());
         m_hVector_etajet1.at(1)->Fill(eventtrigger->GetLeadingJetEta());
         m_hVector_etajet2.at(1)->Fill(eventtrigger->GetSecondJetEta());
         m_hVector_phijet1.at(1)->Fill(eventtrigger->GetLeadingJetPhi());
         m_hVector_phijet2.at(1)->Fill(eventtrigger->GetSecondJetPhi());
         m_hVector_sumEHFplus.at(1)->Fill(eventtrigger->GetSumEHFPlus());
         m_hVector_sumEHFminus.at(1)->Fill(eventtrigger->GetSumEHFMinus());
         m_hVector_deltaphijets.at(1)->Fill(deltaphi_);
         //////////////////////////////////////////////////


	 //if(eventtrigger->GetNVertex() > 0 && eventtrigger->GetNVertex()<= optnVertex){
         //if(eventtrigger->GetL1Bit(0) == 1){
         

            ++counterJetsstep1; 

	    // STEP1        
	    //////////////////////////////////////////////////
            m_hVector_Evt_lumis.at(2)->Fill(eventtrigger->GetInstLumiBunch());
            m_hVector_Eff_lumis.at(2)->Fill(eventtrigger->GetInstLumiBunch());
            m_hVector_ptjet1.at(2)->Fill(eventtrigger->GetLeadingJetPt());
            m_hVector_ptjet2.at(2)->Fill(eventtrigger->GetSecondJetPt());
            m_hVector_etajet1.at(2)->Fill(eventtrigger->GetLeadingJetEta());
            m_hVector_etajet2.at(2)->Fill(eventtrigger->GetSecondJetEta());
            m_hVector_phijet1.at(2)->Fill(eventtrigger->GetLeadingJetPhi());
            m_hVector_phijet2.at(2)->Fill(eventtrigger->GetSecondJetPhi());
            m_hVector_sumEHFplus.at(2)->Fill(eventtrigger->GetSumEHFPlus());
            m_hVector_sumEHFminus.at(2)->Fill(eventtrigger->GetSumEHFMinus());
            m_hVector_deltaphijets.at(2)->Fill(deltaphi_);
            //////////////////////////////////////////////////


	    // Setting Analysis Cut
	    if (eventtrigger->GetLeadingJetPt() > jet1PT && eventtrigger->GetSecondJetPt() > jet2PT ){

	       ++counterJetsstep2;

	       // STEP2         
	       ////////////////////////////////////////////////
               m_hVector_Evt_lumis.at(3)->Fill(eventtrigger->GetInstLumiBunch());
               m_hVector_Eff_lumis.at(3)->Fill(eventtrigger->GetInstLumiBunch());
               m_hVector_ptjet1.at(3)->Fill(eventtrigger->GetLeadingJetPt());
               m_hVector_ptjet2.at(3)->Fill(eventtrigger->GetSecondJetPt());
               m_hVector_etajet1.at(3)->Fill(eventtrigger->GetLeadingJetEta());
               m_hVector_etajet2.at(3)->Fill(eventtrigger->GetSecondJetEta());
               m_hVector_phijet1.at(3)->Fill(eventtrigger->GetLeadingJetPhi());
               m_hVector_phijet2.at(3)->Fill(eventtrigger->GetSecondJetPhi());
               m_hVector_sumEHFplus.at(3)->Fill(eventtrigger->GetSumEHFPlus());
               m_hVector_sumEHFminus.at(3)->Fill(eventtrigger->GetSumEHFMinus());
               m_hVector_deltaphijets.at(3)->Fill(deltaphi_);
               ////////////////////////////////////////////////
 
	       //if (eventtrigger->GetLeadingJetEta() < 5.2 && eventtrigger->GetSecondJetEta() < 5.2 && eventtrigger->GetLeadingJetEta() > -5.2 && eventtrigger->GetSecondJetEta() > -5.2){
               if(eventtrigger->GetSumEHFPlus() < 35 && eventtrigger->GetSumEHFMinus() < 35){

		  ++counterJetsAllstep3;

		  // ALL - STEP3         
		  ////////////////////////////////////////////////
                  m_hVector_Evt_lumis.at(4)->Fill(eventtrigger->GetInstLumiBunch());
		  m_hVector_Eff_lumis.at(4)->Fill(eventtrigger->GetInstLumiBunch());
                  m_hVector_ptjet1.at(4)->Fill(eventtrigger->GetLeadingJetPt());
                  m_hVector_ptjet2.at(4)->Fill(eventtrigger->GetSecondJetPt());
                  m_hVector_etajet1.at(4)->Fill(eventtrigger->GetLeadingJetEta());
                  m_hVector_etajet2.at(4)->Fill(eventtrigger->GetSecondJetEta());
                  m_hVector_phijet1.at(4)->Fill(eventtrigger->GetLeadingJetPhi());
                  m_hVector_phijet2.at(4)->Fill(eventtrigger->GetSecondJetPhi());
                  m_hVector_sumEHFplus.at(4)->Fill(eventtrigger->GetSumEHFPlus());
                  m_hVector_sumEHFminus.at(4)->Fill(eventtrigger->GetSumEHFMinus());
                  m_hVector_deltaphijets.at(4)->Fill(deltaphi_);
		  //////////////////////////////////////////////////////////////////////////////////////////

		 
	       }  
	       // end Jets at all CMS Acceptance




	       if (eventtrigger->GetLeadingJetEta() < 2. && eventtrigger->GetSecondJetEta() < 2. && eventtrigger->GetLeadingJetEta() > -2. && eventtrigger->GetSecondJetEta() > -2.){

		  ++counterJetsEta2step3;

		  // JetsEta2 - STEP3         
		  ////////////////////////////////////////////////
		  m_hVector_Evt_lumis.at(5)->Fill(eventtrigger->GetInstLumiBunch());
                  m_hVector_Eff_lumis.at(5)->Fill(eventtrigger->GetInstLumiBunch());
                  m_hVector_ptjet1.at(5)->Fill(eventtrigger->GetLeadingJetPt());
                  m_hVector_ptjet2.at(5)->Fill(eventtrigger->GetSecondJetPt());
                  m_hVector_etajet1.at(5)->Fill(eventtrigger->GetLeadingJetEta());
                  m_hVector_etajet2.at(5)->Fill(eventtrigger->GetSecondJetEta());
                  m_hVector_phijet1.at(5)->Fill(eventtrigger->GetLeadingJetPhi());
                  m_hVector_phijet2.at(5)->Fill(eventtrigger->GetSecondJetPhi());
                  m_hVector_sumEHFplus.at(5)->Fill(eventtrigger->GetSumEHFPlus());
                  m_hVector_sumEHFminus.at(5)->Fill(eventtrigger->GetSumEHFMinus());
                  m_hVector_deltaphijets.at(5)->Fill(deltaphi_);
		  /////////////////////////////////////////////////////
                
		  

	       } // end jets at JetsEta2

	    } // Jets Cuts

	 } // If Ref. L1
       
       } // Preselection 

     //} // Trigger

    } // Ref. Trigger 

   }// Run All Events

  //Scalling Plots
   for (int k=0; k<6; k++){
     m_hVector_Eff_lumis.at(k)->Scale(1./TotalE);
     //m_hVector_EffVspTjet1 = static_cast<std::vector<TH1D*>>(m_hVector_ptjet1->Clone("EffVspTjet1"));
     //std::vector<TH1D*> 
     //m_hVector_EffVspTjet1->Scale(1./TotalE);
   }

     outstring << "" << std::endl;
     outstring << "<< INPUTS >>" << std::endl;
     outstring << " " << std::endl;
     outstring << "Input file: " << filein << std::endl;
     outstring << "Output file: " << savehistofile << std::endl;
     outstring << " " << std::endl;
     outstring << "pT(Jet1): " << jet1PT << "GeV" << std::endl;
     outstring << "pT(Jet2): " << jet2PT << "GeV" << std::endl;
     outstring << "L1 Ref. Option: " << optL1Ref << std::endl;
     outstring << "Trigger Ref. Option: " << optTriggerRef << std::endl;
     outstring << "Trigger Option: " << optTrigger << std::endl;
     outstring << " " << std::endl;
     outstring << "--> TRUE = 1 FALSE = 0" << std::endl;
     outstring << "Trigger Ref. Switch: " << switchTriggerRef << std::endl;
     outstring << "Trigger Switch: " << switchTrigger << std::endl;
     outstring << "Pre-Selection Switch: " << switchPreSel << std::endl;
     outstring << "L1 Ref. Switch: " << switchL1Ref << std::endl;
     outstring << "N Bins PU: " << nbins << std::endl;
     outstring << "" << std::endl;
     outstring << "<< EVENT INFO >> " << std::endl;
     outstring << " " << std::endl;
     outstring << "Total # of Events without Weight: " << TotalE << std::endl;
     outstring << " " << std::endl;
     outstring << "Ref. Triggered: " << counterTriggerRef << std::endl;
     //outstring << "Triggered: " << counterTrigger << std::endl;
     outstring << "Pre. Selection: " << counterPreSel << std::endl;
     outstring << "STEP1: " << counterJetsstep1 << std::endl;
     outstring << "STEP2: " << counterJetsstep2 << std::endl;
     outstring << "STEP3 (CMS Acceptance): " << counterJetsAllstep3 << std::endl;
     
     outstring << " " << std::endl;
     outstring << "STEP3 (eta < 2 and eta > -2 Acceptance): " << counterJetsEta2step3 << std::endl;
     
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
   int optL1Ref_;
   int optTriggerRef_;
   int optTrigger_;
   bool switchPreSel_;
   bool switchTrigger_;
   bool switchTriggerRef_;
   bool switchL1Ref_;
   int nbins_;

   if (argc > 1 && strcmp(s1,argv[1]) != 0)  filein_ = argv[1];
   if (argc > 2 && strcmp(s1,argv[2]) != 0)  savehistofile_  = argv[2];
   if (argc > 3 && strcmp(s1,argv[3]) != 0)  processname_  = argv[3];
   if (argc > 4 && strcmp(s1,argv[4]) != 0)  jet1PT_  = atoi(argv[4]);
   if (argc > 5 && strcmp(s1,argv[5]) != 0)  jet2PT_ = atoi(argv[5]);
   if (argc > 6 && strcmp(s1,argv[6]) != 0)  optL1Ref_ = atoi(argv[6]);
   if (argc > 7 && strcmp(s1,argv[7]) != 0)  optTriggerRef_  = atoi(argv[7]);
   if (argc > 8 && strcmp(s1,argv[8]) != 0)  optTrigger_   = atoi(argv[8]);
   if (argc > 9 && strcmp(s1,argv[9]) != 0)  switchPreSel_   = atoi(argv[9]);
   if (argc > 10 && strcmp(s1,argv[10]) != 0)  switchTriggerRef_   = atoi(argv[10]);
   if (argc > 11 && strcmp(s1,argv[11]) != 0)  switchTrigger_   = atoi(argv[11]);
   if (argc > 11 && strcmp(s1,argv[12]) != 0)  switchL1Ref_   = atoi(argv[12]);
   if (argc > 12 && strcmp(s1,argv[13]) != 0)  nbins_ = atoi(argv[13]);


   EffMacroTrigger* exclDijets = new EffMacroTrigger();   
   exclDijets->Run(filein_, savehistofile_, processname_, jet1PT_, jet2PT_, optL1Ref_, optTriggerRef_, optTrigger_, switchPreSel_, switchTriggerRef_, switchTrigger_, switchL1Ref_, nbins_);

   return 0;
}
#endif
