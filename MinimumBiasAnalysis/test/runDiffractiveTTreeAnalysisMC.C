#include "TROOT.h"
#include "TChain.h"

#include "MinimumBiasAnalysis/MinimumBiasAnalysis/interface/RootTools.h"

#include <vector>
#include <string>
#include <map>

void setFileNamesPythia8Tune4C(std::vector<std::string>& fileNames){
   fileNames.push_back("");
}

void setFileNamesPythia8MBR(std::vector<std::string>& fileNames){
   fileNames.push_back("");
}

void setFileNamesPhojet(std::vector<std::string>& fileNames){
}

void setFileNamesPythia6TuneZ2star(std::vector<std::string>& fileNames){
   fileNames.push_back("/storage1/antoniov/data1/AnalysisResults/MinBias_TuneZ2star_8TeV_pythia6_cff_py_Step3_RECO-jung/diffractiveAnalysis-MC-v2/merged/diffractiveAnalysisTTree_MC_merged_0.root");
   fileNames.push_back("/storage1/antoniov/data1/AnalysisResults/MinBias_TuneZ2star_8TeV_pythia6_cff_py_Step3_RECO-jung/diffractiveAnalysis-MC-v2/merged/diffractiveAnalysisTTree_MC_merged_1.root");
}

void setFileNamesMinBias(std::vector<std::string>& fileNames){
}

void runDiffractiveTTreeAnalysisMC(){
   gROOT->ProcessLine(".L diffractiveTTreeAnalysis.C+");

   //run_range_t runRange = Data900GeV;
   //run_range_t runRange = Data2360GeV;
   //run_range_t runRange = Data7TeV;
   run_range_t runRange = Data8TeV;
   generator_t genType = PYTHIA6;
   generator_t genTune = PYTHIAZ2Star;

   std::string outDir = "root/8TeV/Pythia6TuneZ2star/MinBias_TuneZ2star_8TeV_pythia6_cff_py_Step3_RECO-jung/diffractiveAnalysis-MC-v2";

   std::vector<std::string> fileNames(0);
   //setFileNamesPythia8Tune4C(fileNames);
   setFileNamesPythia6TuneZ2star(fileNames);

   bool verbose = false;

   // CINT is having some problems with operator+
   //std::string fileName = rootDir + "/";
   //fileName += getTTreeFileName(genTune,runRange);

   std::vector<std::string> selections;
   /*selections.push_back("eventSelection");
   selections.push_back("eventSelectionBeamHaloVeto");
   selections.push_back("eventSelectionBscMinBiasOR");
   selections.push_back("eventSelectionBscMinBiasOREtaMaxFilter");
   selections.push_back("eventSelectionBscMinBiasOREtaMinFilter");
   selections.push_back("eventSelectionBscMinBiasOREtaMinFilterCastorVeto");
   selections.push_back("eventSelectionBscMinBiasOREtaMinFilterCastorTag");*/
   selections.push_back("eventSelectionMinBias");
   selections.push_back("eventSelectionMinBiasEtaMaxFilter");
   selections.push_back("eventSelectionMinBiasEtaMinFilter");
   selections.push_back("eventSelectionMinBiasNoVertex");
   selections.push_back("eventSelectionMinBiasEtaMaxFilterNoVertex");
   selections.push_back("eventSelectionMinBiasEtaMinFilterNoVertex");

   std::vector<int> processTypes;
   processTypes.push_back(All);
   processTypes.push_back(SDPlus);
   processTypes.push_back(SDMinus);
   processTypes.push_back(DD);
   //processTypes.push_back(Diff);
   processTypes.push_back(Inelastic);

   for(size_t isel = 0; isel < selections.size(); ++isel){
      //std::string treeName = "minimumBiasTTreeAnalysis_" + selections[isel] + "/data";
      std::string treeName = "diffractiveAnalysisTTree_" + selections[isel] + "/ProcessedTree";

      TChain* chain = new TChain(treeName.c_str());
      for(size_t ifile = 0; ifile < fileNames.size(); ++ifile){
         std::cout << "Adding file " << fileNames[ifile] << std::endl;
         chain->Add(fileNames[ifile].c_str());
      }
 
      for(size_t iproc = 0; iproc < processTypes.size(); ++iproc){
         std::string histosFileName = outDir + "/";
         histosFileName += getHistosFileName(genTune,runRange,processTypes[iproc],selections[isel]);

         /*minimumBiasTTreeAnalysis(fileName,
                                  treeName,
                                  histosFileName,
                                  false,true,
                                  genType,
                                  processTypes[iproc],-1,verbose);*/
         /*minimumBiasTTreeAnalysis(chain,
                                  histosFileName,
                                  false,true,
                                  genType,
                                  processTypes[iproc],-1,verbose);*/ 
         diffractiveTTreeAnalysis(chain,
                                  histosFileName,
                                  false,true,
                                  genType,
                                  processTypes[iproc],-1,verbose); 

         //std::cout << fileName << "  " << treeName << "  " << histosFileName << "  " << genType << "  " << processTypes[iproc] << std::endl;
      }
   }
}
