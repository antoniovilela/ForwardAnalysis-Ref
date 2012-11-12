#include <TFile.h>
#include <TCanvas.h>
#include <TLegend.h>
#include "TStyle.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TTree.h"
#include "TF1.h"

#include "Utilities/PlottingTools/interface/PlottingTools.h"
#include "Utilities/PlottingTools/interface/Plotter.h"

#include <cstdio>
#include <iostream>
#include <vector>
#include <string>

void plot(const std::string& fileNameData1, const std::string& fileNameData2,
          const std::string& fileNameMC1, const std::string& fileNameMC2, bool ratio = false, int rebin = 1){
   /*TFile* fileColl = 0;
   TFile* fileNoColl = 0;
   TFile* fileMC = 0;
   if(fileNameColl != "")   fileColl = TFile::Open(fileNameColl.c_str());
   if(fileNameNoColl != "") fileNoColl = TFile::Open(fileNameNoColl.c_str());
   if(fileNameMC != "")     fileMC = TFile::Open(fileNameMC.c_str());*/
   TFile* fileData1 = 0;
   TFile* fileData2 = 0;
   TFile* fileMC1 = 0;
   TFile* fileMC2 = 0;
   if(fileNameData1 != "")   fileData1 = TFile::Open(fileNameData1.c_str());
   if(fileNameData2 != "")   fileData2 = TFile::Open(fileNameData2.c_str());
   if(fileNameMC1 != "")     fileMC1 = TFile::Open(fileNameMC1.c_str());
   if(fileNameMC2 != "")     fileMC2 = TFile::Open(fileNameMC2.c_str());
   
   std::string labelData1   = "p+p (7 TeV)"; 
   std::string labelData2   = "p+p (7 TeV) unpaired bunches";
   std::string labelMC1     = "PYTHIA8 Tune 4C default (7 TeV)"; 
   std::string labelMC2     = "PYTHIA8 Tune 4C SL (7 TeV)"; 
   /*std::string labelData1   = "p+p (7 TeV) HLT Jet30U"; 
   std::string labelData2   = "";
   std::string labelMC1     = "PYTHIA6 QCD Fall11 no PU"; 
   std::string labelMC2     = "PYTHIA6 QCD SL no PU";*/ 

   std::vector<std::string> variables;
   /*variables.push_back("energyVsEtaGamma_projX");
   variables.push_back("energyVsEtaNeutralHadron_projX");
   variables.push_back("energyVsEtaHadronHF_projX");
   variables.push_back("energyVsEtaEGammaHF_projX");
   variables.push_back("energyVsEtaGamma_profX");
   variables.push_back("energyVsEtaNeutralHadron_profX");
   variables.push_back("energyVsEtaHadronHF_profX");
   variables.push_back("energyVsEtaEGammaHF_profX");*/
   variables.push_back("sumEnergyVsEtaGamma");
   variables.push_back("sumEnergyVsEtaNeutralHadron");
   variables.push_back("sumEnergyVsEtaHadronHF");
   variables.push_back("sumEnergyVsEtaEGammaHF");
   /*variables.push_back("energyVsEtaGamma_proj_Barrel_zplus");
   variables.push_back("energyVsEtaGamma_proj_Barrel_zminus");
   variables.push_back("energyVsEtaGamma_proj_Transition_zplus");
   variables.push_back("energyVsEtaGamma_proj_Transition_zminus");
   variables.push_back("energyVsEtaGamma_proj_Endcap_zplus");
   variables.push_back("energyVsEtaGamma_proj_Endcap_zminus");
   variables.push_back("energyVsEtaNeutralHadron_proj_Barrel_zplus");
   variables.push_back("energyVsEtaNeutralHadron_proj_Barrel_zminus");
   variables.push_back("energyVsEtaNeutralHadron_proj_Transition_zplus");
   variables.push_back("energyVsEtaNeutralHadron_proj_Transition_zminus");
   variables.push_back("energyVsEtaNeutralHadron_proj_Endcap_zplus");
   variables.push_back("energyVsEtaNeutralHadron_proj_Endcap_zminus");*/
   variables.push_back("energyVsEtaHadronHF_proj_Forward_zplus");
   variables.push_back("energyVsEtaHadronHF_proj_Forward_zminus");
   variables.push_back("energyVsEtaHadronHF_proj_Transition_zplus");
   variables.push_back("energyVsEtaHadronHF_proj_Transition_zminus");
   /*variables.push_back("energyVsEtaHadronHFEcalEnergy_proj_Forward_zplus");
   variables.push_back("energyVsEtaHadronHFEcalEnergy_proj_Forward_zminus");
   variables.push_back("energyVsEtaHadronHFEcalEnergy_proj_Transition_zplus");
   variables.push_back("energyVsEtaHadronHFEcalEnergy_proj_Transition_zminus");*/
   variables.push_back("energyVsEtaEGammaHF_proj_Forward_zplus");
   variables.push_back("energyVsEtaEGammaHF_proj_Forward_zminus");
   variables.push_back("energyVsEtaEGammaHF_proj_Transition_zplus");
   variables.push_back("energyVsEtaEGammaHF_proj_Transition_zminus");
   /*char varNameHFRing[50];
   for(int iring = 1; iring <= 13; ++iring){
      sprintf(varNameHFRing,"energyVsEtaHadronHF_proj_HFRing%d_zplus",iring);
      variables.push_back(varNameHFRing);
      sprintf(varNameHFRing,"energyVsEtaHadronHF_proj_HFRing%d_zminus",iring);
      variables.push_back(varNameHFRing);
      sprintf(varNameHFRing,"energyVsEtaEGammaHF_proj_HFRing%d_zplus",iring);
      variables.push_back(varNameHFRing);
      sprintf(varNameHFRing,"energyVsEtaEGammaHF_proj_HFRing%d_zminus",iring);
      variables.push_back(varNameHFRing);
   }
   for(size_t k = 0; k < variables.size(); ++k) std::cout << "Added variable " << variables[k] << std::endl;*/
   
   /*double thresholds_tmp[] = {0.4,0.4,2.9,2.9,1.6,1.6,1.3,1.3,2.6,2.6,2.6,2.6,3.5,3.5,3.0,3.0,2.5,2.5,3.0,3.0};
   std::vector<double> thresholds(thresholds_tmp,thresholds_tmp + sizeof(thresholds_tmp)/sizeof(double));

   assert(variables.size() == thresholds.size() && "Variable information inconsistent.");*/

   std::vector<std::pair<std::string,TDirectory*> > dirs;
   /*if(fileColl)   dirs.push_back(std::make_pair(labelColl,fileColl));
   if(fileNoColl) dirs.push_back(std::make_pair(labelNoColl,fileNoColl));
   if(fileMC)     dirs.push_back(std::make_pair(labelMC,fileMC));*/
   if(fileData1)   dirs.push_back(std::make_pair(labelData1,fileData1));
   if(fileData2)   dirs.push_back(std::make_pair(labelData2,fileData2));
   if(fileMC1)     dirs.push_back(std::make_pair(labelMC1,fileMC1));
   if(fileMC2)     dirs.push_back(std::make_pair(labelMC2,fileMC2));

   //TH1F* h_NEventsColl = static_cast<TH1F*>(file->Get((std::string(dirNameColl) + "/NEvents").c_str()));
   //TH1F* h_NEventsNoColl = static_cast<TH1F*>(file->Get((std::string(dirNameNoColl) + "/NEvents").c_str()));

   //Plotter<NumberEntriesNorm> plotter;
   Plotter<DefaultNorm> plotter;
   int colors[] = {kBlack,kRed,kBlue,kMagenta};
   std::vector<int> histColors(colors,colors + sizeof(colors)/sizeof(int));
   int linestyles[] = {kSolid,kSolid,kSolid,kSolid};
   std::vector<int> histLineStyles(linestyles,linestyles + sizeof(linestyles)/sizeof(int));
   int markerstyles[] = {20,24,25,26};
   std::vector<int> histMarkerStyles(markerstyles,markerstyles + sizeof(markerstyles)/sizeof(int));
   std::string titleX = "Energy (GeV)";
   std::string titleY; 
   if( !ratio ) titleY = "N_{PF}(E) / N_{Ev}";
   else         titleY = "MC / Data";
   for(size_t k; k < variables.size(); ++k) {plotter.SetTitleX(variables[k],titleX);
                                             plotter.SetTitleY(variables[k],titleY);}

   std::string titleY_projX; 
   if( !ratio ) titleY_projX = "N_{PF}(#eta) / N_{Ev}";
   else         titleY_projX = "MC / Data";
   plotter.SetTitleX("energyVsEtaGamma_projX","#eta");
   plotter.SetTitleX("energyVsEtaNeutralHadron_projX","#eta");
   plotter.SetTitleX("energyVsEtaHadronHF_projX","#eta");
   plotter.SetTitleX("energyVsEtaEGammaHF_projX","#eta");
   plotter.SetTitleY("energyVsEtaGamma_projX",titleY_projX);
   plotter.SetTitleY("energyVsEtaNeutralHadron_projX",titleY_projX);
   plotter.SetTitleY("energyVsEtaHadronHF_projX",titleY_projX);
   plotter.SetTitleY("energyVsEtaEGammaHF_projX",titleY_projX);

   std::string titleY_profX; 
   if( !ratio ) titleY_profX = "<E_{PF}>(#eta) / N_{Ev}";
   else         titleY_profX = "MC / Data";
   plotter.SetTitleX("energyVsEtaGamma_profX","#eta");
   plotter.SetTitleX("energyVsEtaNeutralHadron_profX","#eta");
   plotter.SetTitleX("energyVsEtaHadronHF_profX","#eta");
   plotter.SetTitleX("energyVsEtaEGammaHF_profX","#eta");
   plotter.SetTitleY("energyVsEtaGamma_profX",titleY_profX);
   plotter.SetTitleY("energyVsEtaNeutralHadron_profX",titleY_profX);
   plotter.SetTitleY("energyVsEtaHadronHF_profX",titleY_profX);
   plotter.SetTitleY("energyVsEtaEGammaHF_profX",titleY_profX);
   
   std::string titleY_sumEnergy; 
   if( !ratio ) titleY_sumEnergy = "E_{PF}(#eta) / N_{Ev}";
   else         titleY_sumEnergy = "MC / Data";
   plotter.SetTitleX("sumEnergyVsEtaGamma","#eta");
   plotter.SetTitleX("sumEnergyVsEtaNeutralHadron","#eta");
   plotter.SetTitleX("sumEnergyVsEtaHadronHF","#eta");
   plotter.SetTitleX("sumEnergyVsEtaEGammaHF","#eta");
   plotter.SetTitleY("sumEnergyVsEtaGamma",titleY_sumEnergy);
   plotter.SetTitleY("sumEnergyVsEtaNeutralHadron",titleY_sumEnergy);
   plotter.SetTitleY("sumEnergyVsEtaHadronHF",titleY_sumEnergy);
   plotter.SetTitleY("sumEnergyVsEtaEGammaHF",titleY_sumEnergy);

   plotter.SetStats(false);
   plotter.SetRatio(ratio);
   plotter.SetColors(histColors);
   plotter.SetLineStyles(histLineStyles);
   plotter.SetFillColors(std::vector<int>(1,0));
   plotter.SetFillStyles(std::vector<int>(1,0));
   plotter.SetMarkerColors(histColors);
   plotter.SetMarkerStyles(histMarkerStyles);
   plotter.SetMarkerSizes(std::vector<float>(1,1.4));
   plotter.SetRebin(rebin);

   //std::vector<double> normFactors;
   //normFactors.push_back(1./h_NEventsColl->GetBinContent(1));
   //normFactors.push_back(1./h_NEventsNoColl->GetBinContent(1));
   //plotter.plot(variables,dirs,normFactors);
   if( !ratio ) plotter.plot(variables,dirs,"");
   else         plotter.plot(variables,dirs,"HISTP");

}
