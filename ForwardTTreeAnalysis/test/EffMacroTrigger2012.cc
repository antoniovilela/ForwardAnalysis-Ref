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

#include "EffMacroTrigger2012.h"
#include "ForwardAnalysis/ForwardTTreeAnalysis/interface/DijetsTriggerEvent.h"
#include "PhysicsTools/Utilities/interface/LumiReWeighting.h"


using namespace dijetsTriggerAnalysis;
using namespace reweight;

void EffMacroTrigger2012::LoadFile(std::string fileinput, std::string processinput){

   inf = NULL;
   tr  = NULL;
   inf = TFile::Open(fileinput.c_str(),"read");
   tr = (TTree*)inf->Get(processinput.c_str());

   eventtrigger = new DijetsTriggerEvent();

   trigger = tr ->GetBranch("DijetsTriggerAnalysis");

   trigger->SetAddress(&eventtrigger);

}

void EffMacroTrigger2012::Run(std::string filein_, std::string savehistofile_, std::string processname_){

   filein = filein_;
   savehistofile = savehistofile_;
   processname = processname_;

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

   TH1F* hLumiBx = new TH1F("HLTExclANDVsLumiBx","HLTExclAND Vs BX Lumi; [*10^{30}cm^{-2}s^{-1}]; Efficiency",20,0,5);
   TH1F* hSumEHFPlus = new TH1F("HLTExclANDVsSumEHFPlus","HLTExclAND Vs HF^{+} - Sum of Energy;#sum E_{HF^{+}} [GeV]; Efficiency",20,0.,200.);
   TH1F* hSumEHFMinus = new TH1F("HLTExclANDVsSumEHFMinus","HLTExclAND Vs HF^{-} - Sum of Energy;#sum E_{HF^{-}} [GeV]; Efficiency",20,0.,200.);
   TH2F* hSumEHFPlusVsSumEHFMinus = new TH2F("HLTExclANDVsSumEHFPlusVsSumEHFMinus","HLTExclANDVsSumEHFPlusVsSumEHFMinus",10,0.,100.,10,0.,100.);
   TH1F* hLeadingJetPt = new TH1F("HLTExclANDVsJet1Pt","HLTExclAND Vs Leading Jet - P_{T};P_{T} Jet1 [GeV.c^{-1}]; Efficiency ",20,0.,400.);
   TH1F* hSecondJetPt = new TH1F("HLTExclANDVsJet2Pt","HLTExclAND Vs Second Jet - P_{T};P_{T} Jet2 [GeV.c^{-1}]; Efficiency ",20,0.,400.);
   TH1F* hThirdJetPt = new TH1F("HLTExclANDVsJet3Pt","HLTExclAND Vs Third Jet - P_{T};P_{T} Jet3 [GeV.c^{-1}]; Efficiency ",20,0.,400.);

   histosAll.push_back(hLumiBx);
   histosAll.push_back(hSumEHFPlus);
   histosAll.push_back(hSumEHFMinus);
   histos2DAll.push_back(hSumEHFPlusVsSumEHFMinus);
   histosAll.push_back(hLeadingJetPt);
   histosAll.push_back(hSecondJetPt);
   histosAll.push_back(hThirdJetPt);


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
   int nAllVetoE = 0.;
   int nAllPt =0.;
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
     int trigDijets80AND = eventtrigger->GetHLTPath(0);
     int trigDijets35AND = eventtrigger->GetHLTPath(1);
     int trigDijets35OR = eventtrigger->GetHLTPath(2);
     int trigSingleJet16 = eventtrigger->GetHLTPath(3);
     int trigDiPFJetAve80 = eventtrigger->GetHLTPath(4); 
     int trigL1SingleJet36 = eventtrigger->GetHLTPath(5);

     double lumiBx = eventtrigger->GetInstLumiBunch();
     double sumEnergyHFPlus = eventtrigger->GetSumEHFPlus();
     double sumEnergyHFMinus = eventtrigger->GetSumEHFMinus();
     double JetPt1 = eventtrigger->GetLeadingJetPt();
     double JetPt2 = eventtrigger->GetSecondJetPt();
     double JetPt3 = eventtrigger->GetThirdJetPt();

     double ptMin = 35.;
     //double etaMaxi = 2.5;
     double minHFPe = 30.0;
     double minHFMe = 30.0;
     ////////////////////////////////////////////
      
        if(trigSingleJet16 == 1 ){
        
        //if(trigDijets35OR ==1){
         ++nAll;


     //Setting Analysis Cut
         if(JetPt1 > ptMin && JetPt2 > ptMin){
         //if (JetPt1 > ptMin ){
          ++nAllPt; 
         
            if((sumEnergyHFPlus < minHFPe)&&(sumEnergyHFMinus < minHFMe)){
            //if((sumEnergyHFPlus < minHFPe)||(sumEnergyHFMinus < minHFMe)){
            //if((sumEnergyHFPlus <  minHFPe)){
            //if((sumEnergyHFMinus < minHFPe)){

              ++nAllVetoE;


              histosAll[0]->Fill(sumEnergyHFPlus);
              histosAll[1]->Fill(sumEnergyHFMinus);
              histosAll[2]->Fill(JetPt1);
              histosAll[3]->Fill(JetPt2);
              histosAll[4]->Fill(lumiBx);
              histosAll[5]->Fill(JetPt3);
              histos2DAll[0]->Fill(sumEnergyHFPlus,sumEnergyHFMinus);
             
              //if(trigDijets35OR == 1){
              if(trigDijets35AND == 1){
       
                ++nAcc;
                //std::cout << "nAcc = " << nAcc << std::endl;

                histosEff[0]->Fill(sumEnergyHFPlus);
                histosEff[1]->Fill(sumEnergyHFMinus);
                histosEff[2]->Fill(JetPt1);
                histosEff[3]->Fill(JetPt2);
                histosEff[4]->Fill(lumiBx);
                histosEff[5]->Fill(JetPt3);
                histos2DEff[0]->Fill(sumEnergyHFPlus,sumEnergyHFMinus);   
                
                
       
                
            
              } //If trigger Eff
            } // cuts HF 
         } //cuts Pt
       //} //L1
      }// If triggers Ref.
   }// All Events

//////////////////////////////////////////////////////////////////////////////
  double effAve = nAcc/nAll;
  double effError = sqrt(nAll*effAve*(1. - effAve))/nAll;
  std::cout << "Efficiency = " << effAve << " +/- " << effError << std::endl;
  std::cout << ">>> nall = " << nAll << std::endl;
  std::cout << ">>> nAllPt = " << nAllPt << std::endl; 
  std::cout << ">>> nAcc = " << nAcc << std::endl;
  std::cout << ">>> nAllVetoE = " << nAllVetoE << std::endl;
 
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

   if (argc > 1 && strcmp(s1,argv[1]) != 0)  filein_ = argv[1];
   if (argc > 2 && strcmp(s1,argv[2]) != 0)  savehistofile_  = argv[2];
   if (argc > 3 && strcmp(s1,argv[3]) != 0)  processname_  = argv[3];

   EffMacroTrigger2012* exclDijets = new EffMacroTrigger2012();   
   exclDijets->Run(filein_, savehistofile_, processname_);

   return 0;
}
#endif




