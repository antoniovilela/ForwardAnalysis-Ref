#include "TROOT.h"
#include "TChain.h"

#include <vector>
#include <string>

#include "MinimumBiasAnalysis/MinimumBiasAnalysis/interface/RootTools.h"

void setFileNamesZeroBiasRun132605(std::vector<std::string>& fileNames){
   fileNames.push_back();
}

void setFileNamesZeroBiasRun135528(std::vector<std::string>& fileNames){
   fileNames.push_back("");
}

void setFileNamesMinimumBiasRun135528(std::vector<std::string>& fileNames){
   fileNames.push_back("");
}

void setFileNamesMinimumBiasRun132605(std::vector<std::string>& fileNames){
   fileNames.push_back("");
}

void setFileNamesMinimumBiasVeryLowPU2012(std::vector<std::string>& fileNames){
   fileNames.push_back("/storage1/antoniov/data1/AnalysisResults/LP_MinBias2_Run2012A-PromptReco-v1-VeryLowPUMay2012/diffractiveAnalysis-v3/diffractiveAnalysisTTree_10_1_QF3.root");
   fileNames.push_back("/storage1/antoniov/data1/AnalysisResults/LP_MinBias2_Run2012A-PromptReco-v1-VeryLowPUMay2012/diffractiveAnalysis-v3/diffractiveAnalysisTTree_11_1_5CD.root");
   fileNames.push_back("/storage1/antoniov/data1/AnalysisResults/LP_MinBias2_Run2012A-PromptReco-v1-VeryLowPUMay2012/diffractiveAnalysis-v3/diffractiveAnalysisTTree_12_1_fiE.root");
   fileNames.push_back("/storage1/antoniov/data1/AnalysisResults/LP_MinBias2_Run2012A-PromptReco-v1-VeryLowPUMay2012/diffractiveAnalysis-v3/diffractiveAnalysisTTree_13_1_rGB.root");
   fileNames.push_back("/storage1/antoniov/data1/AnalysisResults/LP_MinBias2_Run2012A-PromptReco-v1-VeryLowPUMay2012/diffractiveAnalysis-v3/diffractiveAnalysisTTree_14_1_syu.root");
   fileNames.push_back("/storage1/antoniov/data1/AnalysisResults/LP_MinBias2_Run2012A-PromptReco-v1-VeryLowPUMay2012/diffractiveAnalysis-v3/diffractiveAnalysisTTree_1_1_Svk.root");
   fileNames.push_back("/storage1/antoniov/data1/AnalysisResults/LP_MinBias2_Run2012A-PromptReco-v1-VeryLowPUMay2012/diffractiveAnalysis-v3/diffractiveAnalysisTTree_2_1_sNH.root");
   fileNames.push_back("/storage1/antoniov/data1/AnalysisResults/LP_MinBias2_Run2012A-PromptReco-v1-VeryLowPUMay2012/diffractiveAnalysis-v3/diffractiveAnalysisTTree_3_1_Xur.root");
   fileNames.push_back("/storage1/antoniov/data1/AnalysisResults/LP_MinBias2_Run2012A-PromptReco-v1-VeryLowPUMay2012/diffractiveAnalysis-v3/diffractiveAnalysisTTree_4_1_aV3.root");
   fileNames.push_back("/storage1/antoniov/data1/AnalysisResults/LP_MinBias2_Run2012A-PromptReco-v1-VeryLowPUMay2012/diffractiveAnalysis-v3/diffractiveAnalysisTTree_5_1_dGa.root");
   fileNames.push_back("/storage1/antoniov/data1/AnalysisResults/LP_MinBias2_Run2012A-PromptReco-v1-VeryLowPUMay2012/diffractiveAnalysis-v3/diffractiveAnalysisTTree_6_1_gUm.root");
   fileNames.push_back("/storage1/antoniov/data1/AnalysisResults/LP_MinBias2_Run2012A-PromptReco-v1-VeryLowPUMay2012/diffractiveAnalysis-v3/diffractiveAnalysisTTree_7_1_eJ2.root");
   fileNames.push_back("/storage1/antoniov/data1/AnalysisResults/LP_MinBias2_Run2012A-PromptReco-v1-VeryLowPUMay2012/diffractiveAnalysis-v3/diffractiveAnalysisTTree_8_1_a0a.root");   
   fileNames.push_back("/storage1/antoniov/data1/AnalysisResults/LP_MinBias2_Run2012A-PromptReco-v1-VeryLowPUMay2012/diffractiveAnalysis-v3/diffractiveAnalysisTTree_9_1_032.root");
}

void setFileNamesMinimumBias(std::vector<std::string>& fileNames){
   fileNames.push_back("analysisMinBias_TTree_MinimumBias.root");
}

void runDiffractiveTTreeAnalysis(){
   gROOT->ProcessLine(".L diffractiveTTreeAnalysis.C+");

   //run_range_t runRange = Data900GeV;
   //run_range_t runRange = Data2360GeV;
   //run_range_t runRange = Data7TeV;
   run_range_t runRange = Data8TeV;

   std::vector<std::string> fileNames(0);
   //setFileNamesMinimumBias(fileNames);
   //setFileNamesMinimumBiasRun132605(fileNames);
   //setFileNamesMinimumBiasRun135528(fileNames);
   //setFileNamesZeroBiasRun135528(fileNames);
   setFileNamesMinimumBiasVeryLowPU2012(fileNames);
 
   std::string outDir = "root/8TeV/Data/VeryLowPU2012/diffractiveAnalysis-v3";
   bool verbose = false;

   std::vector<std::string> selections;
   /*selections.push_back("eventSelectionBPTX");
   selections.push_back("eventSelectionL1Tech4");
   selections.push_back("eventSelectionL1Tech4BeamHaloVeto");
   selections.push_back("eventSelectionL1Tech4BscOr");
   selections.push_back("eventSelectionL1Tech4HLTBscMinBiasOR");*/
   /*selections.push_back("eventSelectionBscMinBiasOR");
   selections.push_back("eventSelectionBscMinBiasOREtaMaxFilter");
   selections.push_back("eventSelectionBscMinBiasOREtaMinFilter");
   selections.push_back("eventSelectionBscMinBiasOREtaMinFilterCastorVeto");
   selections.push_back("eventSelectionBscMinBiasOREtaMinFilterCastorTag");*/
   /*selections.push_back("eventSelectionMinBias");
   selections.push_back("eventSelectionMinBiasEtaMaxFilter");
   selections.push_back("eventSelectionMinBiasEtaMinFilter");*/
   selections.push_back("eventSelectionMinBiasNoVertex");
   selections.push_back("eventSelectionMinBiasEtaMaxFilterNoVertex");
   selections.push_back("eventSelectionMinBiasEtaMinFilterNoVertex");

   for(size_t k = 0; k < selections.size(); ++k){
      //std::string treeName = "minimumBiasTTreeAnalysis_" + selections[k] + "/data";
      std::string treeName = "diffractiveAnalysisTTree_" + selections[k] + "/ProcessedTree";
      std::string histosFileName = outDir + "/";
      histosFileName += getHistosFileName(runRange,selections[k]);
 
      TChain* chain = new TChain(treeName.c_str()); 
      for(size_t ifile = 0; ifile < fileNames.size(); ++ifile) chain->Add(fileNames[ifile].c_str());

      //minimumBiasTTreeAnalysis(fileName,treeName,histosFileName,false,false,-1,-1,-1,verbose);
      //minimumBiasTTreeAnalysis(chain,histosFileName,false,false,-1,-1,-1,verbose);
      diffractiveTTreeAnalysis(chain,histosFileName,false,false,-1,-1,-1,verbose);

      //std::cout << fileName << "  " << treeName << "  " << histosFileName << std::endl;
   }
}
