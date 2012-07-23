#include <TFile.h>
#include <TCanvas.h>
#include <TLegend.h>
#include "TStyle.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TTree.h"

#include "Utilities/PlottingTools/interface/PlottingTools.h"
#include "Utilities/PlottingTools/interface/Plotter.h"

#include "MinimumBiasAnalysis/MinimumBiasAnalysis/interface/RootTools.h"

#include <iostream>
#include <sstream>
#include <vector>

//std::string getDataFile(int runRange);
//std::string getMCFile(int genType,int runRange);
void printOptions(){
   std::vector<std::string> options;
   options.push_back("setDirsPYTHIAPHOJET");
   options.push_back("setDirsMCComponents");
   options.push_back("setDirsDataMC");
   options.push_back("setDirsDataMCGenSel");
   options.push_back("setDirsDataMCComponents");
   options.push_back("setDirsCompareData");
   std::ostringstream oss;
   oss << "--- Options:" << std::endl;
   for(size_t k = 0; k < options.size(); ++k) oss << "  " << options[k] << std::endl;
   std::cout << oss.str(); 
}

void setDirsMCComponents(std::string const& selection, std::string const& rootDir, std::vector<std::pair<std::string,TDirectory*> >& dirs, std::vector<double>& normFactors);
void setDirsPYTHIAPHOJET(std::vector<std::pair<std::string,TDirectory*> >& dirs, std::vector<double>& normFactors);
void setDirsDataMC(std::string const& selection,std::vector<std::pair<std::string,TDirectory*> >& dirs, std::vector<double>& normFactors);
void setDirsDataMCComponents(std::string const& selection,std::vector<std::pair<std::string,TDirectory*> >& dirs, std::vector<double>& normFactors);
void setDirsCompareData(std::vector<std::pair<std::string,TDirectory*> >& dirs, std::vector<double>& normFactors);
void setDirsDataMCGenSel(std::string const& selection,std::vector<std::pair<std::string,TDirectory*> >& dirs, std::vector<double>& normFactors);

void plot(std::string const& selection, std::string const& mode = "setDirsDataMC", std::string const& rootDir = "", const char* drawOption = "", int rebin = 1){
   std::vector<std::string> variables;
   variables.push_back("nVertex");
   /*variables.push_back("posXPrimVtx");
   variables.push_back("posYPrimVtx");
   variables.push_back("posZPrimVtx");*/
   variables.push_back("multiplicityHFPlus");
   variables.push_back("multiplicityHFMinus");
   variables.push_back("sumEnergyHFPlus");
   variables.push_back("sumEnergyHFMinus");
   /*variables.push_back("xiPlusFromPFCands");
   variables.push_back("xiMinusFromPFCands");
   variables.push_back("EPlusPzFromPFCands");
   variables.push_back("EMinusPzFromPFCands");
   variables.push_back("MxFromPFCands");
   variables.push_back("etaMaxFromPFCandsVarBin");
   variables.push_back("etaMinFromPFCandsVarBin");*/
   /*variables.push_back("multiplicityTracks");
   variables.push_back("multiplicityHFPlusVarBin_dist");
   variables.push_back("multiplicityHFMinusVarBin_dist");
   variables.push_back("sumEnergyHFPlusVarBin_dist");
   variables.push_back("sumEnergyHFMinusVarBin_dist");
   variables.push_back("xiPlusFromPFCands_dist");
   variables.push_back("xiMinusFromPFCands_dist");
   variables.push_back("logXiPlusFromPFCands_dist");
   variables.push_back("logXiMinusFromPFCands_dist");
   variables.push_back("logXiPlusFromPFCandsVarBin_dist");
   variables.push_back("logXiMinusFromPFCandsVarBin_dist");
   variables.push_back("EPlusPzFromPFCands_dist");
   variables.push_back("EMinusPzFromPFCands_dist");
   variables.push_back("MxFromPFCands_dist");
   variables.push_back("etaMaxFromPFCandsVarBin_dist");
   variables.push_back("etaMinFromPFCandsVarBin_dist");*/
   variables.push_back("multiplicityTracks");
   variables.push_back("sumEnergyHFPlusVarBin");
   variables.push_back("sumEnergyHFMinusVarBin");
   variables.push_back("sumEnergyCASTOR"); 
   variables.push_back("logXiPlusFromPFCands");
   variables.push_back("logXiMinusFromPFCands");
   variables.push_back("logXiPlusFromPFCandsVarBin");
   variables.push_back("logXiMinusFromPFCandsVarBin");
   //variables.push_back("MxFromPFCands");
   variables.push_back("etaMaxFromPFCandsVarBin");
   variables.push_back("etaMinFromPFCandsVarBin");
   /*variables.push_back("logXiGenPlus");
   variables.push_back("logXiGenMinus");*/
   /*variables.push_back("xiGenPlus");
   variables.push_back("xiGenMinus");
   variables.push_back("etaMaxGen");
   variables.push_back("etaMinGen");*/
  
   //Plotter<NumberEntriesNorm> plotter;
   Plotter<DefaultNorm> plotter;
   plotter.SetVerbose(true);
   //plotter.SetRefHisto(true);
   plotter.SetStats(true);
   plotter.SetRebin(rebin);
   plotter.SetMarkerSizes(std::vector<float>(1,1.4));
   plotter.SetHeader("#eta_{min} > -1 (BSC OR and Vertex)");
   //plotter.SetHeader("#eta_{min} > -1 (BSC OR and Vertex and CASTOR tag)");

   plotter.SetTitleX("multiplicityTracks","N_{trk}");
   plotter.SetTitleX("sumEnergyHFPlusVarBin_dist","#sum E (HF+) (GeV)");
   plotter.SetTitleX("sumEnergyHFMinusVarBin_dist","#sum E (HF-) (GeV)");
   plotter.SetTitleX("multiplicityHFPlusVarBin_dist","N(HF+)");
   plotter.SetTitleX("multiplicityHFMinusVarBin_dist","N(HF-)");
   plotter.SetTitleX("EPlusPzFromPFCands_dist","#sum(E+pz) (GeV)");
   plotter.SetTitleX("EMinusPzFromPFCands_dist","#sum(E-pz) (GeV)");
   plotter.SetTitleX("xiPlusFromPFCands_dist","#xi^{+}");
   plotter.SetTitleX("xiMinusFromPFCands_dist","#xi^{-}");
   plotter.SetTitleX("logXiPlusFromPFCands","log#xi^{+}");
   plotter.SetTitleX("logXiMinusFromPFCands","log#xi^{-}");
   plotter.SetTitleX("logXiPlusFromPFCands_dist","log#xi^{+}");
   plotter.SetTitleX("logXiMinusFromPFCands_dist","log#xi^{-}");
   plotter.SetTitleX("logXiPlusFromPFCandsVarBin_dist","log#xi^{+}");
   plotter.SetTitleX("logXiMinusFromPFCandsVarBin_dist","log#xi^{-}");
   plotter.SetTitleX("MxFromPFCands_dist","M_{X} (GeV)");
   plotter.SetTitleX("etaMaxFromPFCandsVarBin","#eta^{max}");
   plotter.SetTitleX("etaMinFromPFCandsVarBin","#eta^{min}"); 
   plotter.SetTitleX("etaMaxFromPFCandsVarBin_dist","#eta^{max}");
   plotter.SetTitleX("etaMinFromPFCandsVarBin_dist","#eta^{min}"); 
   plotter.SetTitleX("logXiGenPlus","log#xi^{+}");
   plotter.SetTitleX("logXiGenMinus","log#xi^{-}");
   
   plotter.SetTitleY("multiplicityTracks","dN/dN_{trk}");
   plotter.SetTitleY("sumEnergyHFPlusVarBin_dist","dN/dE_{HF} (GeV^{-1})");
   plotter.SetTitleY("sumEnergyHFMinusVarBin_dist","dN/dE_{HF} (GeV^{-1})");
   plotter.SetTitleY("multiplicityHFPlusVarBin_dist","dN/dN_{HF}");
   plotter.SetTitleY("multiplicityHFMinusVarBin_dist","dN/dN_{HF}");
   plotter.SetTitleY("EPlusPzFromPFCands_dist","dN/d(#sumE+pz) (GeV^{-1})");
   plotter.SetTitleY("EMinusPzFromPFCands_dist","dN/d(#sumE-pz) (GeV^{-1})");
   plotter.SetTitleY("xiPlusFromPFCands_dist","dN/d#xi^{+}");
   plotter.SetTitleY("xiMinusFromPFCands_dist","dN/d#xi^{-}");
   plotter.SetTitleY("logXiPlusFromPFCands_dist","dN/dlog#xi^{+}");
   plotter.SetTitleY("logXiMinusFromPFCands_dist","dN/dlog#xi^{-}");
   plotter.SetTitleY("logXiPlusFromPFCandsVarBin_dist","dN/dlog#xi^{+}");
   plotter.SetTitleY("logXiMinusFromPFCandsVarBin_dist","dN/dlog#xi^{-}");
   plotter.SetTitleY("MxFromPFCands_dist","dN/dM_{X} (GeV^{-1})");
   plotter.SetTitleY("etaMaxFromPFCandsVarBin_dist","dN/d#eta^{max}");
   plotter.SetTitleY("etaMinFromPFCandsVarBin_dist","dN/d#eta^{min}");
   
   std::vector<std::pair<std::string,TDirectory*> > dirs;
   std::vector<double> normFactors;

   //setDirsPYTHIAPHOJET(dirs,normFactors);
   //setDirsMCComponents(selection,dirs,normFactors);
   //setDirsDataMC(selection,dirs,normFactors);
   //setDirsDataMCGenSel(selection,dirs,normFactors);
   //setDirsDataMCComponents(selection,dirs,normFactors);
   //setDirsCompareData(dirs,normFactors);
   if(mode == "setDirsPYTHIAPHOJET"){
      setDirsPYTHIAPHOJET(dirs,normFactors);
      plotter.plot(variables,dirs,normFactors,drawOption);
   } else if(mode == "setDirsMCComponents"){
      setDirsMCComponents(selection,rootDir,dirs,normFactors);
      int colors[] = {kBlack,kBlue,kMagenta,kOrange,kRed};
      int linestyles[] = {1,2,3,9,10};
      int markerstyles[] = {1,1,1,1,1};
      int fillcolors[] = {0,kBlue,kMagenta,kOrange,kRed};
      int fillstyles[] = {0,3004,3006,3005,3007,3013};
      std::vector<int> histColors(colors,colors + sizeof(colors)/sizeof(int));
      std::vector<int> histLineStyles(linestyles,linestyles + sizeof(linestyles)/sizeof(int));
      std::vector<int> histMarkerStyles = std::vector<int>(markerstyles,markerstyles + sizeof(markerstyles)/sizeof(int));
      std::vector<int> histFillColors(fillcolors,fillcolors + sizeof(fillcolors)/sizeof(int));
      std::vector<int> histFillStyles(fillstyles,fillstyles + sizeof(fillstyles)/sizeof(int));
      plotter.SetLineColors(histColors);
      plotter.SetLineStyles(histLineStyles);
      plotter.SetFillColors(histFillColors);
      plotter.SetFillStyles(histFillStyles);
      plotter.SetMarkerColors(histColors);
      plotter.SetMarkerStyles(histMarkerStyles);
      plotter.SetRefHisto(true);
      plotter.plot(variables,dirs,normFactors,drawOption);
   } else if(mode == "setDirsDataMC"){
      setDirsDataMC(selection,dirs,normFactors);
      plotter.plot(variables,dirs,normFactors,drawOption);
   } else if(mode == "setDirsDataMCGenSel"){
      setDirsDataMCGenSel(selection,dirs,normFactors);
      int colors[] = {kBlack,kBlack,kBlue};
      int linestyles[] = {kSolid,kSolid,kDashed};
      int markerstyles[] = {20,1,1};
      std::vector<int> histColors(colors,colors + sizeof(colors)/sizeof(int));
      std::vector<int> histLineStyles(linestyles,linestyles + sizeof(linestyles)/sizeof(int));
      std::vector<int> histMarkerStyles = std::vector<int>(markerstyles,markerstyles + sizeof(markerstyles)/sizeof(int));
      plotter.SetLineColors(histColors);
      plotter.SetLineStyles(histLineStyles);
      plotter.SetFillColors(std::vector<int>(1,0));
      plotter.SetFillStyles(std::vector<int>(1,0));
      plotter.SetMarkerColors(histColors);
      plotter.SetMarkerStyles(histMarkerStyles);
      plotter.SetRefHisto(true);
      plotter.plot(variables,dirs,normFactors,drawOption);
   } else if(mode == "setDirsDataMCComponents"){
      setDirsDataMCComponents(selection,dirs,normFactors);
      /*int colors[] = {kBlue,kOrange,kRed};
      int linestyles[] = {1,9,10};
      int markerstyles[] = {1,1,1};*/
      int colors[] = {kBlack,kBlack,kBlue,kMagenta,kOrange,kRed};
      int linestyles[] = {1,1,2,9,10,12};
      int markerstyles[] = {20,1,1,1,1,1};
      int fillcolors[] = {0,0,kBlue,kMagenta,kOrange,kRed};
      int fillstyles[] = {0,0,3004,3006,3005,3007,3013};
      std::vector<int> histColors(colors,colors + sizeof(colors)/sizeof(int));
      std::vector<int> histLineStyles(linestyles,linestyles + sizeof(linestyles)/sizeof(int));
      std::vector<int> histMarkerStyles = std::vector<int>(markerstyles,markerstyles + sizeof(markerstyles)/sizeof(int));
      std::vector<int> histFillColors(fillcolors,fillcolors + sizeof(fillcolors)/sizeof(int));
      std::vector<int> histFillStyles(fillstyles,fillstyles + sizeof(fillstyles)/sizeof(int));
      plotter.SetLineColors(histColors);
      plotter.SetLineStyles(histLineStyles);
      //plotter.SetFillColors(histColors);
      plotter.SetFillColors(histFillColors);
      //plotter.SetFillColors(std::vector<int>(1,0));
      //plotter.SetFillStyles(std::vector<int>(1,0));
      plotter.SetFillStyles(histFillStyles);
      plotter.SetMarkerColors(histColors);
      plotter.SetMarkerStyles(histMarkerStyles);
      //plotter.plotComponents(variables,dirs,normFactors,drawOption);         
      plotter.SetRefHisto(true);
      plotter.plot(variables,dirs,normFactors,drawOption);
   } else if(mode == "setDirsCompareData"){
      setDirsCompareData(dirs,normFactors);
      plotter.plot(variables,dirs,normFactors,drawOption);
   }
}

void setDirsMCComponents(std::string const& selection, std::string const& dir, std::vector<std::pair<std::string,TDirectory*> >& dirs, std::vector<double>& normFactors){
   run_range_t runRange = Data7TeV;
   generator_t genType = PYTHIA8;
   std::string eventSelection = selection;
   //std::string dir = "root/7TeV/Pythia6D6T/eventSelection";   
   //std::string dir = "root/7TeV/Pythia8/eventSelection-v6";
   //std::string dir = "root/7TeV/Pythia8MBR/eventSelection-v1";

   TFile* fileMC_All = TFile::Open((dir + "/" + getHistosFileName(genType,runRange,All,eventSelection)).c_str());
   TH1F* h_EventSelection_All = static_cast<TH1F*>(fileMC_All->Get("EventSelection"));
   double nEvents_All = h_EventSelection_All->GetBinContent(1);

   /*TFile* fileMC_SD = TFile::Open((dir + "/" + getHistosFileName(genType,runRange,SD,eventSelection)).c_str());
   TH1F* h_EventSelection_SD = static_cast<TH1F*>(fileMC_SD->Get("EventSelection"));
   double nEvents_SD = h_EventSelection_SD->GetBinContent(1);*/
  
   TFile* fileMC_SDPlus = TFile::Open((dir + "/" + getHistosFileName(genType,runRange,SDPlus,eventSelection)).c_str());
   TH1F* h_EventSelection_SDPlus = static_cast<TH1F*>(fileMC_SDPlus->Get("EventSelection"));
   double nEvents_SDPlus = h_EventSelection_SDPlus->GetBinContent(1);

   TFile* fileMC_SDMinus = TFile::Open((dir + "/" + getHistosFileName(genType,runRange,SDMinus,eventSelection)).c_str());
   TH1F* h_EventSelection_SDMinus = static_cast<TH1F*>(fileMC_SDMinus->Get("EventSelection"));
   double nEvents_SDMinus = h_EventSelection_SDMinus->GetBinContent(1);

   TFile* fileMC_DD = TFile::Open((dir + "/" + getHistosFileName(genType,runRange,DD,eventSelection)).c_str());
   TH1F* h_EventSelection_DD = static_cast<TH1F*>(fileMC_DD->Get("EventSelection"));
   double nEvents_DD = h_EventSelection_DD->GetBinContent(1);

   TFile* fileMC_QCD = TFile::Open((dir + "/" + getHistosFileName(genType,runRange,Inelastic,eventSelection)).c_str());
   TH1F* h_EventSelection_QCD = static_cast<TH1F*>(fileMC_QCD->Get("EventSelection"));
   double nEvents_QCD = h_EventSelection_QCD->GetBinContent(1);

   dirs.push_back(std::make_pair("PYTHIA8 MBR",fileMC_All));
   //dirs.push_back(std::make_pair("Single-diffractive",fileMC_SD));
   dirs.push_back(std::make_pair("Single-diffractive (pp #rightarrow pX)",fileMC_SDPlus));
   dirs.push_back(std::make_pair("Single-diffractive (pp #rightarrow Xp)",fileMC_SDMinus));
   dirs.push_back(std::make_pair("Double-diffractive",fileMC_DD));
   dirs.push_back(std::make_pair("Inel. non-diffractive",fileMC_QCD));
   normFactors.push_back(1./nEvents_All);
   //normFactors.push_back(1./nEvents_SD);
   normFactors.push_back(1./nEvents_SDPlus);
   normFactors.push_back(1./nEvents_SDMinus);
   normFactors.push_back(1./nEvents_DD);
   normFactors.push_back(1./nEvents_QCD);
}

void setDirsPYTHIAPHOJET(std::vector<std::pair<std::string,TDirectory*> >& dirs, std::vector<double>& normFactors){
   TFile* filePYTHIA = TFile::Open("analysisMinBiasFWLite_histos_PYTHIA_MinBias_STARTUP3X_V8K_900GeV_new_highQualityTracks.root");
   TH1F* h_EventSelectionPYTHIA = static_cast<TH1F*>(filePYTHIA->Get("EventSelection"));
   double nEventsPreSelPYTHIA = h_EventSelectionPYTHIA->GetBinContent(7);

   TFile* filePHOJET = TFile::Open("analysisMinBiasFWLite_histos_PHOJET_MinBias_STARTUP3X_V8K_900GeV_new_highQualityTracks.root");
   TH1F* h_EventSelectionPHOJET = static_cast<TH1F*>(filePHOJET->Get("EventSelection"));
   double nEventsPreSelPHOJET = h_EventSelectionPHOJET->GetBinContent(7);

   dirs.push_back(std::make_pair("MinBias PYTHIA",filePYTHIA));
   dirs.push_back(std::make_pair("MinBias PHOJET",filePHOJET));
   normFactors.push_back(1./nEventsPreSelPYTHIA);
   normFactors.push_back(1./nEventsPreSelPHOJET);
}

void setDirsDataMC(std::string const& selection, std::vector<std::pair<std::string,TDirectory*> >& dirs, std::vector<double>& normFactors){
   run_range_t runRange = Data900GeV;
   std::string eventSelection = "eventSelectionMinBiasBSCOR";
   std::string dir = "root/900GeV/" + selection;
   
   //TFile* fileData = TFile::Open((dir + "/" + getHistosFileName(runRange,eventSelection)).c_str());
   TFile* fileDataRef = TFile::Open((dir + "/" + getHistosFileName(runRange,eventSelection)).c_str());
   //TFile* fileData = TFile::Open("histosErrorBand_MinimumBias_Runs124009-124030_eventSelectionMinBiasBSCOR_EnergyThresholdHFVar.root");
   TFile* fileData = fileDataRef; 
   //TH1F* h_EventSelectionData = static_cast<TH1F*>(fileData->Get("EventSelection"));
   TH1F* h_EventSelectionData = static_cast<TH1F*>(fileDataRef->Get("EventSelection"));
   double nEventsDataFullSel = h_EventSelectionData->GetBinContent(11);

   TFile* fileMC_PYTHIA = TFile::Open((dir + "/" + getHistosFileName(PYTHIA,runRange,All,eventSelection)).c_str());
   TH1F* h_EventSelectionMC_PYTHIA = static_cast<TH1F*>(fileMC_PYTHIA->Get("EventSelection"));
   double nEventsMCFullSel_PYTHIA = h_EventSelectionMC_PYTHIA->GetBinContent(11);

   TFile* fileMC_PHOJET = TFile::Open((dir + "/" + getHistosFileName(PHOJET,runRange,All,eventSelection)).c_str());
   TH1F* h_EventSelectionMC_PHOJET = static_cast<TH1F*>(fileMC_PHOJET->Get("EventSelection"));
   double nEventsMCFullSel_PHOJET = h_EventSelectionMC_PHOJET->GetBinContent(11);

   TFile* fileMC_PYTHIADW = TFile::Open((dir + "/" + getHistosFileName(PYTHIADW,runRange,All,eventSelection)).c_str());
   TH1F* h_EventSelectionMC_PYTHIADW = static_cast<TH1F*>(fileMC_PYTHIADW->Get("EventSelection"));
   double nEventsMCFullSel_PYTHIADW = h_EventSelectionMC_PYTHIADW->GetBinContent(11);

   TFile* fileMC_PYTHIAP0 = TFile::Open((dir + "/" + getHistosFileName(PYTHIAP0,runRange,All,eventSelection)).c_str());
   TH1F* h_EventSelectionMC_PYTHIAP0 = static_cast<TH1F*>(fileMC_PYTHIAP0->Get("EventSelection"));
   double nEventsMCFullSel_PYTHIAP0 = h_EventSelectionMC_PYTHIAP0->GetBinContent(11);

   TFile* fileMC_PYTHIAProQ20 = TFile::Open((dir + "/" + getHistosFileName(PYTHIAProQ20,runRange,All,eventSelection)).c_str());
   TH1F* h_EventSelectionMC_PYTHIAProQ20 = static_cast<TH1F*>(fileMC_PYTHIAProQ20->Get("EventSelection"));
   double nEventsMCFullSel_PYTHIAProQ20 = h_EventSelectionMC_PYTHIAProQ20->GetBinContent(11);

   dirs.push_back(std::make_pair("Data 900 GeV",fileData));
   dirs.push_back(std::make_pair("MinBias PYTHIA D6T 900 GeV",fileMC_PYTHIA));
   dirs.push_back(std::make_pair("MinBias PYTHIA DW 900 GeV",fileMC_PYTHIADW));
   dirs.push_back(std::make_pair("MinBias PYTHIA P0 900 GeV",fileMC_PYTHIAP0));
   dirs.push_back(std::make_pair("MinBias PYTHIA ProQ20 900 GeV",fileMC_PYTHIAProQ20)); 
   dirs.push_back(std::make_pair("MinBias PHOJET 900 GeV",fileMC_PHOJET));
   normFactors.push_back(1.);
   normFactors.push_back(nEventsDataFullSel/nEventsMCFullSel_PYTHIA);
   normFactors.push_back(nEventsDataFullSel/nEventsMCFullSel_PYTHIADW);
   normFactors.push_back(nEventsDataFullSel/nEventsMCFullSel_PYTHIAP0);
   normFactors.push_back(nEventsDataFullSel/nEventsMCFullSel_PYTHIAProQ20);
   normFactors.push_back(nEventsDataFullSel/nEventsMCFullSel_PHOJET);
}

void setDirsDataMCComponents(std::string const& selection, std::vector<std::pair<std::string,TDirectory*> >& dirs, std::vector<double>& normFactors){
   //run_range_t runRange = Data7TeV;
   run_range_t runRange = Data8TeV;
   //generator_t genType = PYTHIA8;
   generator_t genType = PYTHIAZ2Star;
   //generator_t genType = PHOJET;
   std::string eventSelection = selection;
   //std::string dirData = "root/7TeV/Data/Run135528/minimumBiasTTreeAnalysis-v8";
   std::string dirData = "root/8TeV/Data/VeryLowPU2012/diffractiveAnalysis-v3";

   //std::string dirMC = "root/7TeV/Pythia8Tune4C/eventSelectionAnalysis-v2/LogXiGenPlusMax-5.5";
   //std::string dirMC = "root/7TeV/Pythia8Tune4C/eventSelectionAnalysis-v2";
   std::string dirMC = "root/8TeV/Pythia6TuneZ2star/MinBias_TuneZ2star_8TeV_pythia6_cff_py_Step3_RECO-jung/diffractiveAnalysis-MC-v2/";

   //double normMC = 71260.*49.156/499596;
   //double normMC = 71260.*49.156/1816992.;
   //double normMC = 71260.*49.156/9975000.;
   double normMC = 71260000.*3.632/1008990.;

   TFile* file_Data = TFile::Open((dirData + "/" + getHistosFileName(runRange,eventSelection)).c_str());
   TH1F* h_EventSelection_Data = static_cast<TH1F*>(file_Data->Get("EventSelection"));
   int binNumber = h_EventSelection_Data->GetNbinsX();
   double nEventsFullSel_Data = h_EventSelection_Data->GetBinContent(binNumber);

   TFile* fileMC_All = TFile::Open((dirMC + "/" + getHistosFileName(genType,runRange,All,eventSelection)).c_str());
   TH1F* h_EventSelection_All = static_cast<TH1F*>(fileMC_All->Get("EventSelection"));
   double nEvents_All = h_EventSelection_All->GetBinContent(1);
   double nEventsFullSel_All = h_EventSelection_All->GetBinContent(binNumber);

   /*TFile* fileMC_SD = TFile::Open((dirMC + "/" + getHistosFileName(genType,runRange,SD,eventSelection)).c_str());
   TH1F* h_EventSelection_SD = static_cast<TH1F*>(fileMC_SD->Get("EventSelection"));
   double nEvents_SD = h_EventSelection_SD->GetBinContent(1);*/
   
   TFile* fileMC_SDPlus = TFile::Open((dirMC + "/" + getHistosFileName(genType,runRange,SDPlus,eventSelection)).c_str());
   TH1F* h_EventSelection_SDPlus = static_cast<TH1F*>(fileMC_SDPlus->Get("EventSelection"));
   double nEvents_SDPlus = h_EventSelection_SDPlus->GetBinContent(1);

   TFile* fileMC_SDMinus = TFile::Open((dirMC + "/" + getHistosFileName(genType,runRange,SDMinus,eventSelection)).c_str());
   TH1F* h_EventSelection_SDMinus = static_cast<TH1F*>(fileMC_SDMinus->Get("EventSelection"));
   double nEvents_SDMinus = h_EventSelection_SDMinus->GetBinContent(1);

   TFile* fileMC_DD = TFile::Open((dirMC + "/" + getHistosFileName(genType,runRange,DD,eventSelection)).c_str());
   TH1F* h_EventSelection_DD = static_cast<TH1F*>(fileMC_DD->Get("EventSelection"));
   double nEvents_DD = h_EventSelection_DD->GetBinContent(1);

   TFile* fileMC_QCD = TFile::Open((dirMC + "/" + getHistosFileName(genType,runRange,Inelastic,eventSelection)).c_str());
   TH1F* h_EventSelection_QCD = static_cast<TH1F*>(fileMC_QCD->Get("EventSelection"));
   double nEvents_QCD = h_EventSelection_QCD->GetBinContent(1);

   dirs.push_back(std::make_pair("p+p (7 TeV)",file_Data));
   dirs.push_back(std::make_pair("PYTHIA8",fileMC_All));
   //dirs.push_back(std::make_pair("PYTHIA6 Z1",fileMC_All));
   /*dirs.push_back(std::make_pair("PYTHIA8 - SD",fileMC_SD));
   dirs.push_back(std::make_pair("PYTHIA8 - DD",fileMC_DD)); 
   dirs.push_back(std::make_pair("PYTHIA8 - Inel. non-diffractive",fileMC_QCD));*/
   dirs.push_back(std::make_pair("Single-diffractive (pp #rightarrow pX)",fileMC_SDPlus));
   dirs.push_back(std::make_pair("Single-diffractive (pp #rightarrow Xp)",fileMC_SDMinus));
   dirs.push_back(std::make_pair("Double-diffractive",fileMC_DD));
   dirs.push_back(std::make_pair("Inel. non-diffractive",fileMC_QCD));
   /*dirs.push_back(std::make_pair("MinBias PHOJET - SD",fileMC_SD));
   dirs.push_back(std::make_pair("MinBias PHOJET - DD",fileMC_DD)); 
   dirs.push_back(std::make_pair("MinBias PHOJET - Inel. non-diffractive",fileMC_QCD));*/

   normFactors.push_back(1.);
   normFactors.push_back(nEventsFullSel_Data/nEventsFullSel_All); 
   //normFactors.push_back((nEventsFullSel_Data/nEventsFullSel_All)*(nEvents_All/nEvents_SD));
   normFactors.push_back((nEventsFullSel_Data/nEventsFullSel_All)*(nEvents_All/nEvents_SDPlus));
   normFactors.push_back((nEventsFullSel_Data/nEventsFullSel_All)*(nEvents_All/nEvents_SDMinus));
   normFactors.push_back((nEventsFullSel_Data/nEventsFullSel_All)*(nEvents_All/nEvents_DD));
   normFactors.push_back((nEventsFullSel_Data/nEventsFullSel_All)*(nEvents_All/nEvents_QCD));

   /*normFactors.push_back(1.);
   normFactors.push_back(normMC);
   normFactors.push_back(normMC);
   normFactors.push_back(normMC);
   normFactors.push_back(normMC);
   normFactors.push_back(normMC);*/
}

void setDirsCompareData(std::vector<std::pair<std::string,TDirectory*> >& dirs, std::vector<double>& normFactors){
   //TFile* file_Ref = TFile::Open("root/7TeV/Data/Run135528/minimumBiasTTreeAnalysis-v8/analysisMinBiasTTree_MinimumBias_7TeV_eventSelectionBscMinBiasOR_histos.root");
   TFile* file_Ref = TFile::Open("root/7TeV/Data/Run135528/minimumBiasTTreeAnalysis-v8/analysisMinBiasTTree_MinimumBias_7TeV_eventSelectionBscMinBiasOREtaMinFilter_histos.root");
   TH1F* h_EventSelection_Ref = static_cast<TH1F*>(file_Ref->Get("EventSelection"));
   double nEventsPreSel_Ref = h_EventSelection_Ref->GetBinContent(11);

   //TFile* file_Comp = TFile::Open("root/7TeV/Data/Run132605/minimumBiasTTreeAnalysis-v2/analysisMinBiasTTree_MinimumBias_7TeV_eventSelectionBscMinBiasOR_histos.root");
   TFile* file_Comp = TFile::Open("root/7TeV/Data/Run132605/minimumBiasTTreeAnalysis-v2/analysisMinBiasTTree_MinimumBias_7TeV_eventSelectionBscMinBiasOREtaMinFilter_histos.root");
   TH1F* h_EventSelection_Comp = static_cast<TH1F*>(file_Comp->Get("EventSelection"));
   double nEventsPreSel_Comp = h_EventSelection_Comp->GetBinContent(11);

   double intLumi_Comp = 20.322;
   double intLumi_Ref = 49.156;
 
   /*dirs.push_back(std::make_pair("Dec. 14th Re-reco",file_Ref));
   dirs.push_back(std::make_pair("Dec. 19th Re-reco",file_Comp));*/
   dirs.push_back(std::make_pair("Run 135528",file_Ref));
   dirs.push_back(std::make_pair("Run 132605",file_Comp));
   /*normFactors.push_back(1./nEventsPreSel_Ref);
   normFactors.push_back(1./nEventsPreSel_Comp);*/
   normFactors.push_back(1.);
   normFactors.push_back( intLumi_Ref/intLumi_Comp );
}

void setDirsDataMCGenSel(std::string const& selection, std::vector<std::pair<std::string,TDirectory*> >& dirs, std::vector<double>& normFactors){
   run_range_t runRange = Data7TeV;
   generator_t genType = PYTHIA8;
   //generator_t genType = PYTHIAD6T;
   //generator_t genType = PHOJET;
   //std::string eventSelection = "minimumBiasTTreeAnalysisHBHENoiseFilterHcalNoiseSelection";
   std::string eventSelection = selection;
   std::string genSelection = "EtaMaxGen";
   std::string dirData = "root/7TeV/Data/Run132605/eventSelection";
   std::string dirMC = "root/7TeV/Pythia8/eventSelection";
   //std::string dirMC = "root/7TeV/Pythia6D6T/eventSelection";
   //std::string dirMC = "root/7TeV/Phojet/eventSelection";

   TFile* file_Data = TFile::Open((dirData + "/" + getHistosFileName(runRange,eventSelection)).c_str());
   TH1F* h_EventSelection_Data = static_cast<TH1F*>(file_Data->Get("EventSelection"));
   int binNumber = h_EventSelection_Data->GetNbinsX();
   double nEventsFullSel_Data = h_EventSelection_Data->GetBinContent(binNumber);

   TFile* fileMC_All = TFile::Open((dirMC + "/" + getHistosFileName(genType,runRange,All,eventSelection)).c_str());
   TH1F* h_EventSelection_All = static_cast<TH1F*>(fileMC_All->Get("EventSelection"));
   double nEvents_All = h_EventSelection_All->GetBinContent(1);
   double nEventsFullSel_All = h_EventSelection_All->GetBinContent(binNumber);

   std::string histosFileName = "analysisMinBiasTTree_";
   histosFileName += getDataLabel(genType,runRange);
   histosFileName += "_" + eventSelection + "_histos_";
   histosFileName += getProcessCategoryName(All) + "_" + genSelection + ".root";
   TFile* fileMC_GenSel = TFile::Open((dirMC + "/" + histosFileName).c_str());
   TH1F* h_EventSelection_GenSel = static_cast<TH1F*>(fileMC_GenSel->Get("EventSelection"));
   double nEvents_GenSel = h_EventSelection_GenSel->GetBinContent(1);
   double nEventsFullSel_GenSel = h_EventSelection_GenSel->GetBinContent(binNumber);

   dirs.push_back(std::make_pair("p+p (7 TeV)",file_Data));
   dirs.push_back(std::make_pair("PYTHIA8",fileMC_All));
   dirs.push_back(std::make_pair("PYTHIA8 - #eta^{Gen}_{max} < 0",fileMC_GenSel));

   normFactors.push_back(1.);
   normFactors.push_back(nEventsFullSel_Data/nEventsFullSel_All);
   //normFactors.push_back(nEventsFullSel_Data/nEventsFullSel_All);
   normFactors.push_back((nEventsFullSel_Data/nEventsFullSel_All)*(nEvents_All/nEvents_GenSel));   
   /*normFactors.push_back(1./nEventsFullSel_All);
   normFactors.push_back((1./nEventsFullSel_All)*(nEvents_All/nEvents_GenSel));*/
   
}

/*
std::string getDataFile(int runRange){
   std::string fileName;
   if(runRange == Data900GeV) fileName = "analysisMinBiasTTree_MinimumBias_Runs124009-124030_eventSelectionMinBiasBSCOR_histos.root";
   else if(runRange == Data2360GeV) fileName = "analysisMinBiasTTree_MinimumBias_Run124120_eventSelectionMinBiasBSCOR_histos.root";
   else throw RootException("ERROR: Invalid option");

   return fileName;
}

std::string getMCFile(int genType, int runRange){
   std::string fileName;
   if(genType == PYTHIA){
      if(runRange == Data900GeV) fileName = "analysisMinBiasTTree_PYTHIA_MinBias_900GeV_eventSelectionMinBiasBSCOR_histos_All.root";
      else if(runRange == Data2360GeV) fileName = "analysisMinBiasTTree_PYTHIA_MinBias_2360GeV_eventSelectionMinBiasBSCOR_histos_All.root";
      else throw RootException("ERROR: Invalid option");
   } else if(genType == PHOJET){
      if(runRange == Data900GeV) fileName = "analysisMinBiasTTree_PHOJET_MinBias_900GeV_eventSelectionMinBiasBSCOR_histos_All.root";
      else if(runRange == Data2360GeV) fileName = "analysisMinBiasTTree_PHOJET_MinBias_2360GeV_eventSelectionMinBiasBSCOR_histos_All.root";
      else throw RootException("ERROR: Invalid option");
   } else throw RootException("ERROR: Invalid option");      

   return fileName;
}
*/
