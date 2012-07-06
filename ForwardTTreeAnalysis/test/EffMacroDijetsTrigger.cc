//#if !defined(__CINT__) || defined(__MAKECINT__)
#include <TROOT.h>
#include <TChain.h>
#include <TString.h>
#include "TH1F.h"
#include "TH2F.h"
#include <TH1.h>
#include <TH2.h>
#include "TProfile.h"
#include <TTree.h>
#include <TMath.h>
#include "TFile.h"
#include "TCanvas.h"
#include "TGraphAsymmErrors.h"

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <fstream>

#include "EffMacroDijetsTrigger.h"
#include "ForwardAnalysis/ForwardTTreeAnalysis/interface/DijetsTriggerEvent.h"
#include "PhysicsTools/Utilities/interface/LumiReWeighting.h"


using namespace dijetsTriggerAnalysis;
using namespace reweight;

void EffMacroDijetsTrigger::LoadFile(std::string fileinput, std::string processinput){

   inf = NULL;
   tr  = NULL;
   inf = TFile::Open(fileinput.c_str(),"read");
   tr = (TTree*)inf->Get(processinput.c_str());

   eventtrigger = new DijetsTriggerEvent();

   trigger = tr ->GetBranch("DijetsTriggerAnalysis");

   trigger->SetAddress(&eventtrigger);

}

void EffMacroDijetsTrigger::Run(std::string filein_, std::string savehistofile_, std::string processname_, double jet1PT_, double jet2PT_, int triggereffpass_, double sumEHFPlus_, double sumEHFMinus_, int optTrigger_, bool switchSelJet1Eff_, bool switchSelJet2Eff_, bool switchSelDijetsEff_, bool switchSelHFPlusEff_, bool switchSelHFMinusEff_, bool switchSelHFOREff_, bool switchSelHFANDEff_){

   filein = filein_;
   savehistofile = savehistofile_;
   processname = processname_;
   jet1PT = jet1PT_;
   jet2PT = jet2PT_;
   triggereffpass = triggereffpass_;
   sumEHFPlus = sumEHFPlus_;
   sumEHFMinus = sumEHFMinus_;
   optTrigger = optTrigger_;
   switchSelJet1Eff = switchSelJet1Eff_;
   switchSelJet2Eff = switchSelJet2Eff_;
   switchSelDijetsEff = switchSelDijetsEff_;
   switchSelHFPlusEff = switchSelHFPlusEff_;
   switchSelHFMinusEff = switchSelHFMinusEff_;
   switchSelHFOREff = switchSelHFOREff_;
   switchSelHFANDEff = switchSelHFANDEff_; 

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
   std::cout << "Trigger Ref Option: " << triggereffpass << std::endl;
   std::cout << "sumE(HFPlus): " << sumEHFPlus << "GeV" << std::endl;
   std::cout << "sumE(HFMinus): " << sumEHFMinus << "GeV" << std::endl;
   std::cout << "Trigger Option: " << optTrigger << std::endl;
   std::cout << " " << std::endl;
   std::cout << "--> TRUE = 1 FALSE = 0" << std::endl;
   std::cout << "Sel Jet1: " << switchSelJet1Eff << std::endl;
   std::cout << "Sel Jet2: " << switchSelJet2Eff << std::endl;
   std::cout << "Sel DiJets: " << switchSelDijetsEff << std::endl;
   std::cout << "Sel HFPlus: " << switchSelHFPlusEff << std::endl;
   std::cout << "Sel HFMinus: " << switchSelHFMinusEff << std::endl;
   std::cout << "Sel HFOR: " << switchSelHFOREff << std::endl;
   std::cout << "Sel HFAND: " << switchSelHFANDEff << std::endl;
  
   std::cout << "" << std::endl;



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

   // Root file with histograms
   TFile *outf = new TFile(savehistofile.c_str(),"RECREATE");
   
   // File with Number of Events
   TString outtxt = savehistofile;
   outtxt.ReplaceAll("root","txt");  
   std::ofstream outstring(outtxt); 
   //
   // Creating Histograms
   std::vector<TH1F*> histosAll;
   std::vector<TH2F*> histos2DAll;

   //TH1F* hLumiBx = new TH1F("HLTExclANDVsLumiBx","HLTExclAND Vs BX Lumi; [*10^{30}cm^{-2}s^{-1}]; Efficiency",20,0,5);
   TH1F* hSumEHFPlus = new TH1F("HLTExclANDVsSumEHFPlus","HLTExclAND Vs HF^{+} - Sum of Energy;#sum E_{HF^{+}} [GeV]; Efficiency",20,0.,200.);
   TH1F* hSumEHFMinus = new TH1F("HLTExclANDVsSumEHFMinus","HLTExclAND Vs HF^{-} - Sum of Energy;#sum E_{HF^{-}} [GeV]; Efficiency",20,0.,200.);
   TH2F* hSumEHFPlusVsSumEHFMinus = new TH2F("HLTExclANDVsSumEHFPlusVsSumEHFMinus","HLTExclANDVsSumEHFPlusVsSumEHFMinus",10,0.,100.,10,0.,100.);
   TH1F* hLeadingJetPt = new TH1F("HLTExclANDVsJet1Pt","HLTExclAND Vs Leading Jet - P_{T};P_{T} Jet1 [GeV.c^{-1}]; Efficiency ",20,0.,400.);
   TH1F* hSecondJetPt = new TH1F("HLTExclANDVsJet2Pt","HLTExclAND Vs Second Jet - P_{T};P_{T} Jet2 [GeV.c^{-1}]; Efficiency ",20,0.,400.);
   //TH1F* hThirdJetPt = new TH1F("HLTExclANDVsJet3Pt","HLTExclAND Vs Third Jet - P_{T};P_{T} Jet3 [GeV.c^{-1}]; Efficiency ",20,0.,400.);

   //histosAll.push_back(hLumiBx);
   histosAll.push_back(hSumEHFPlus);
   histosAll.push_back(hSumEHFMinus);
   histos2DAll.push_back(hSumEHFPlusVsSumEHFMinus);
   histosAll.push_back(hLeadingJetPt);
   histosAll.push_back(hSecondJetPt);
   //histosAll.push_back(hThirdJetPt);


   std::vector<TH1F*> histosEff;
   std::vector<TH1F*>::const_iterator it_histoAll = histosAll.begin();
   std::vector<TH1F*>::const_iterator it_histosAll_end = histosAll.end();
   for(; it_histoAll != it_histosAll_end; ++it_histoAll){
      std::string hname = (*it_histoAll)->GetName();
      hname += "_eff";
      histosEff.push_back(static_cast<TH1F*>((*it_histoAll)->Clone(hname.c_str())));
   }
   std::vector<TH2F*> histos2DEff;
   std::vector<TH2F*>::const_iterator it_histo2DAll = histos2DAll.begin();
   std::vector<TH2F*>::const_iterator it_histos2DAll_end = histos2DAll.end();
   for(; it_histo2DAll != it_histos2DAll_end; ++it_histo2DAll){
      std::string hname = (*it_histo2DAll)->GetName();
      hname += "_eff";
      histos2DEff.push_back(static_cast<TH2F*>((*it_histo2DAll)->Clone(hname.c_str())));
   }

   //
   //int NEVENTS = tr->GetEntriesFast();
   int NEVENTS = tr->GetEntries();

   TH1::SetDefaultSumw2(true);
   TH2::SetDefaultSumw2(true);
    
   unsigned NEntries = tr->GetEntries();
   std::cout << "" << std::endl;
   std::cout<< "Reading Tree: "<< NEntries << " events"<<std::endl;
   std::cout << "" << std::endl;

   int decade = 0;
   int TotalE = 0;
   int nAcc = 0.;
   int nAll = 0.;
   //int nAllVetoE = 0.;
   //int nAllPt =0.;
    
  //--------------------------------------------------------------------------------------------------------------------------
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

     //['HLT_ExclDiJet80_HFAND_v*','HLT_ExclDiJet35_HFAND_v*','HLT_ExclDiJet35_HFOR_v*','HLT_L1SingleJet16_v*','HLT_DiPFJetAve80_v*','HLT_L1SingleJet36_v*']
     int triggerRef = eventtrigger->GetHLTPath(triggereffpass);
     int triggerAna = eventtrigger->GetHLTPath(optTrigger);     

     //double lumiBx = eventtrigger->GetInstLumiBunch();
     double sumEnergyHFPlus = eventtrigger->GetSumEHFPlus();
     double sumEnergyHFMinus = eventtrigger->GetSumEHFMinus();
     double JetPt1 = eventtrigger->GetLeadingJetPt();
     double JetPt2 = eventtrigger->GetSecondJetPt();
     //double JetPt3 = eventtrigger->GetThirdJetPt();
     bool selDijets = false; bool selJet1 = false; bool selJet2 = false;
     bool selHFAND = false; bool selHFOR = false; bool selHFPlus = false; bool selHFMinus = false; 
     //bool selJet1HFAND = false; bool selJet2HFAND = false; bool selJet1HFOR = false; bool selJet2HFOR = false;
     ////////////////////////////////////////////
      
        //if(triggerRef == 1 ){
        if(eventtrigger->GetHLTPath(triggereffpass)){
         ++nAll;


     //Setting Analysis Cut
         if(switchSelDijetsEff && (JetPt1 > jet1PT && JetPt2 > jet2PT)){  
            selDijets = true;}
         else if(switchSelJet1Eff && (JetPt2 > jet2PT)){
            selJet1 = true;}
         else if(switchSelJet2Eff && (JetPt1 > jet1PT)){
            selJet2 = true;}
          //++nAllPt; 
          //if (selDijets && !selJet1 && !selJet2){
            if(switchSelHFANDEff && ((sumEnergyHFPlus < sumEHFPlus)&&(sumEnergyHFMinus < sumEHFMinus))){
               selHFAND = true;}
            else if(switchSelHFOREff && ((sumEnergyHFPlus < sumEHFPlus)||(sumEnergyHFMinus < sumEHFMinus))){
               selHFOR = true;} 
            else if(switchSelHFPlusEff && ((sumEnergyHFMinus < sumEHFMinus))){
               selHFPlus = true;}
            else if(switchSelHFMinusEff && ((sumEnergyHFPlus < sumEHFPlus))){
               selHFMinus = true;}
            //} 

              //++nAllVetoE;
              if(selDijets && !selJet1 && !selJet2){ 
                if(selHFPlus && !selHFMinus && !selHFAND && !selHFOR)
                   histosAll[0]->Fill(sumEnergyHFPlus);
                else if(!selHFPlus && selHFMinus && !selHFAND && !selHFOR)
                   histosAll[1]->Fill(sumEnergyHFMinus);
                else if((!selHFPlus && !selHFMinus) && (selHFAND || selHFOR))
                   histos2DAll[0]->Fill(sumEnergyHFPlus,sumEnergyHFMinus);   
              }
              else if(!selDijets && selJet1 && !selJet2) { 
                if(!selHFPlus && !selHFMinus && selHFAND && !selHFOR)
                   histosAll[2]->Fill(JetPt1);}
              else if(!selDijets && !selJet1 && selJet2) {
                if(!selHFPlus && !selHFMinus && selHFAND && !selHFOR)
                   histosAll[3]->Fill(JetPt2);}
              //histosAll[4]->Fill(lumiBx);
              //histosAll[5]->Fill(JetPt3);
             
              
             
              //if(triggerAna == 1){
              if(eventtrigger->GetHLTPath(optTrigger)){
                ++nAcc;
                //std::cout << "nAcc = " << nAcc << std::endl;
               if(selDijets && !selJet1 && !selJet2){
                if(selHFPlus && !selHFMinus && !selHFAND && !selHFOR)
                   histosEff[0]->Fill(sumEnergyHFPlus);
                else if(!selHFPlus && selHFMinus && !selHFAND && !selHFOR)
                   histosEff[1]->Fill(sumEnergyHFMinus);
                else if((!selHFPlus && selHFMinus) && (selHFAND || selHFOR))
                   histos2DEff[0]->Fill(sumEnergyHFPlus,sumEnergyHFMinus);
              }
              else if(!selDijets && selJet1 && !selJet2) {               
                if(!selHFPlus && !selHFMinus && selHFAND && !selHFOR)
                   histosEff[2]->Fill(JetPt1);}
              else if(!selDijets && !selJet1 && selJet2) {
                if(!selHFPlus && !selHFMinus && selHFAND && !selHFOR)
                   histosEff[3]->Fill(JetPt2);}
                /*histosEff[0]->Fill(sumEnergyHFPlus);
                histosEff[1]->Fill(sumEnergyHFMinus);
                histosEff[2]->Fill(JetPt1);
                histosEff[3]->Fill(JetPt2);
                histosEff[4]->Fill(lumiBx);
                histosEff[5]->Fill(JetPt3);
                histos2DEff[0]->Fill(sumEnergyHFPlus,sumEnergyHFMinus);   
                */
                
       
                
            
             } //If trigger Eff
           // } // cuts HF 
         //} //cuts Pt
       //} //L1
      }// If triggers Ref.
   }// All Events

//////////////////////////////////////////////////////////////////////////////
  double effAve = nAcc/nAll;
  double effError = sqrt(nAll*effAve*(1. - effAve))/nAll;
  std::cout << "Efficiency = " << effAve << " +/- " << effError << std::endl;
  std::cout << ">>> nall = " << nAll << std::endl;
  //std::cout << ">>> nAllPt = " << nAllPt << std::endl; 
  std::cout << ">>> nAcc = " << nAcc << std::endl;
  //std::cout << ">>> nAllVetoE = " << nAllVetoE << std::endl;
 
  std::vector<TGraphAsymmErrors*> graphsEff(histosEff.size());  
  for(size_t k = 0; k < histosAll.size(); ++k){
     TH1F* hAll = histosAll[k];
     TH1F* hEff = histosEff[k];
     std::cout << "================================" << std::endl;
     std::cout << " Histogram " << hAll->GetName() << std::endl;
     graphsEff[k] = new TGraphAsymmErrors(hEff,hAll);
     graphsEff[k]->SetName( (std::string(hEff->GetName()) + "_graph" ).c_str() );
     std::cout << "  Created eff. graph " << graphsEff[k]->GetName() << std::endl;

     for(int ibin = 0; ibin < hAll->GetNbinsX(); ++ibin){
        int binNumber = ibin + 1;
        double nAllBin = hAll->GetBinContent(binNumber);
        double effBin = hEff->GetBinContent(binNumber)/nAllBin;
        double errBin = sqrt(nAllBin*effBin*(1. - effBin))/nAllBin;

        hEff->SetBinContent(binNumber,effBin);
        hEff->SetBinError(binNumber,errBin);

        //std::cout << " Bin " << binNumber << ": " << effBin << " +/- " << errBin << std::endl;
     }

     /*TCanvas* canvas = new TCanvas(("canvas_" + std::string(hAll->GetName())).c_str(),hAll->GetName());
     canvas->cd();
     hEff->Draw();*/
  }

  outf->cd();
  for(size_t k = 0; k < graphsEff.size(); ++k) graphsEff[k]->Write();
  outf->Write(); 
  outf->Close();
  
   //outf->Write();
   //outf->Close();
//////////////////////////////////////////////////////////////////////////////
 

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
   int triggereffpass_;
   double sumEHFPlus_;
   double sumEHFMinus_;
   int optTrigger_;
   bool switchSelJet1Eff_;
   bool switchSelJet2Eff_;
   bool switchSelDijetsEff_;
   bool switchSelHFPlusEff_;
   bool switchSelHFMinusEff_;
   bool switchSelHFOREff_;
   bool switchSelHFANDEff_; 


   if (argc > 1 && strcmp(s1,argv[1]) != 0)  filein_ = argv[1];
   if (argc > 2 && strcmp(s1,argv[2]) != 0)  savehistofile_  = argv[2];
   if (argc > 3 && strcmp(s1,argv[3]) != 0)  processname_  = argv[3];
   if (argc > 4 && strcmp(s1,argv[4]) != 0)  jet1PT_  = atoi(argv[4]);
   if (argc > 5 && strcmp(s1,argv[5]) != 0)  jet2PT_ = atoi(argv[5]);
   if (argc > 6 && strcmp(s1,argv[6]) != 0)  triggereffpass_ = atoi(argv[6]);
   if (argc > 7 && strcmp(s1,argv[7]) != 0)  sumEHFPlus_  = atoi(argv[7]);
   if (argc > 8 && strcmp(s1,argv[8]) != 0)  sumEHFMinus_ = atoi(argv[8]);
   if (argc > 9 && strcmp(s1,argv[9]) != 0)  optTrigger_   = atoi(argv[9]);
   if (argc > 10 && strcmp(s1,argv[10]) != 0)  switchSelJet1Eff_   = atoi(argv[10]);
   if (argc > 11 && strcmp(s1,argv[11]) != 0)  switchSelJet2Eff_   = atoi(argv[11]);
   if (argc > 12 && strcmp(s1,argv[12]) != 0)  switchSelDijetsEff_   = atoi(argv[12]);
   if (argc > 13 && strcmp(s1,argv[13]) != 0)  switchSelHFPlusEff_   = atoi(argv[13]);
   if (argc > 14 && strcmp(s1,argv[14]) != 0)  switchSelHFMinusEff_   = atoi(argv[14]);
   if (argc > 15 && strcmp(s1,argv[15]) != 0)  switchSelHFOREff_   = atoi(argv[15]);
   if (argc > 16 && strcmp(s1,argv[16]) != 0)  switchSelHFANDEff_   = atoi(argv[16]); 


   EffMacroDijetsTrigger* exclDijets = new EffMacroDijetsTrigger();   
   exclDijets->Run(filein_, savehistofile_, processname_,jet1PT_, jet2PT_, triggereffpass_, sumEHFPlus_, sumEHFMinus_,optTrigger_,  switchSelJet1Eff_, switchSelJet2Eff_, switchSelDijetsEff_, switchSelHFPlusEff_, switchSelHFMinusEff_, switchSelHFOREff_, switchSelHFANDEff_);

   return 0;
}
#endif





