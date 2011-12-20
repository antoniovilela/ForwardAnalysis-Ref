// State University of Rio de Janeiro - UERJ
// Authors: exclusive dijets subgroup
// contact: dmf@cern.ch
//
//------------------------------------------
// Version: 1.0
// 
// How Execute: root -l EventMacro.C
// 
// Functions: LoadFiles(i) - load a specific file (i) and run your analysis with your cuts defined in RunExclusive 
//            RunExclusive( pT[jet1], pT[jet2], Number of Vertex, Trigger Reference, dataOn, weight, triggereff)
//            
//            pT[jet1] = Jet1 pT cut;
//            pT[jet2] = Jet2 pT cut;
//            optnVertex = Selecting Events with number of vertex < optnVertex;
//            optTrigger = Trigger option (0,1,2...). Same order defined in your PF2PAT(...).py or forwardQCD(...).py which generate your Pattuple. See hltpath variable.
//            switchWeightPU_ = true (if is running data). This apply weightpu = 1, false (if is running mc);
//            switchWeightEff_ = true (if is running data). This apply weightpu = 1, false (if is running mc);
//            switchWeightLumi_ = true (if is running data). This apply weightpu = 1, false (if is running mc);
//            weightlumi = luminosity weight (L_data/L_MC) for MC;
//            triggereff = trigger efficiency;
//

#ifndef EventMacro2010_h
#define EventMacro2010_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include "LumiReweightingStandAlone.h"

class EventMacro {
public :

    EventMacro();
   ~EventMacro();
    void OpenPattuple(string filein_);
    void RunExclusive(string savehistofile, double jet1PT, double jet2PT, int optnVertex, int optTrigger, bool switchWeightPU_, bool switchWeightLumi_, bool switchWeightEff_, double weightlumi_, double triggereff_);
    void LoadFiles(int entering);
};


EventMacro::EventMacro()
{

cout << " " << endl;
cout << "Exclusive Dijets Analysis Macro" << endl;
cout << "-------------------------------" << endl;
cout << " " << endl;

//Loading Files and Running Analysis
LoadFiles(1);
LoadFiles(2);
LoadFiles(3);
LoadFiles(4);
LoadFiles(5);
LoadFiles(6);
LoadFiles(7);
LoadFiles(8);
LoadFiles(9);
//LoadFiles(10);

}

EventMacro::LoadFiles(int entering)
{

  // FWLite libraries
  gSystem->Load("libFWCoreFWLite.so");
  gSystem->Load("libPhysicsToolsUtilities.so");
  AutoLibraryLoader::enable();

   if (entering == 1) {

   OpenPattuple("/storage2/sfonseca/CMSSW/pythia_QCD_exclusive/CMSSW_4_2_8/src/ForwardAnalysis/ForwardTTreeAnalysis/test/QCD_pythia/QCD_Pt-15to30_TuneZ2_7TeV_pythia6/res/Exclusive_DijetsAnalysis_QCD_15to30.root");
   
   cout << "\n MC Sample: Jets pT -> [15,30] GeV/c\n" << endl;
   
   RunExclusive("Output_MC_Pythia6/hltexcldijet30Uand-qcd_15to30.root",60,60,1,0, false, true, false, 1959.14686403915,1.0);
   RunExclusive("Output_MC_Pythia6/hltexcldijet30Uor-qcd_15to30.root",60,60,1,1, false, true, false, 840.244416543085, 1.0 );
   RunExclusive("Output_MC_Pythia6/hltexcld30U-qcd_15to30.root",60,60,1,2, false, true, false,1.62528519418743,  1.0);
    
   }

  if (entering == 2) {

   OpenPattuple("/storage2/sfonseca/CMSSW/pythia_QCD_exclusive/CMSSW_4_2_8/src/ForwardAnalysis/ForwardTTreeAnalysis/test/QCD_pythia/QCD_Pt-30to50_TuneZ2_7TeV_pythia6/res/Exclusive_DijetsAnalysis_30to50.root");
   
   cout << "\n MC Sample: Jets pT -> [30,50] GeV/c\n" << endl;
   
   RunExclusive("Output_MC_Pythia6/hltexcldijet30Uand-qcd_30to50.root",60,60,1,0, false, true, false, 203.609769791228,  1.0);
   RunExclusive("Output_MC_Pythia6/hltexcldijet30Uor-qcd_30to50.root",60,60,1,1, false, true, false, 87.3247306574991,  1.0);
   RunExclusive("Output_MC_Pythia6/hltexcld30U-qcd_30to50.root",60,60,1,2, false, true, false, 0.1689122700844,  1.0);


   }
   
   if (entering == 3) {

   OpenPattuple("/storage2/sfonseca/CMSSW/pythia_QCD_exclusive/CMSSW_4_2_8/src/ForwardAnalysis/ForwardTTreeAnalysis/test/QCD_pythia/QCD_Pt-50to80_TuneZ2_7TeV_pythia6/res/Exclusive_DijetsAnalysis_50to80.root");
   
   cout << "\n MC Sample: Jets pT -> [50,80] GeV/c\n" << endl;
   
   RunExclusive("Output_MC_Pythia6/hltexcldijet30Uand-qcd_50to80.root",60,60,1,0, false, true, false, 24.3116131818182,  1.0);
   RunExclusive("Output_MC_Pythia6/hltexcldijet30Uor-qcd_50to80.root",60,60,1,1, false, true, false, 10.426833030303,  1.0);
   RunExclusive("Output_MC_Pythia6/hltexcld30U-qcd_50to80.root",60,60,1,2, false, true, false, 0.020168628333333,  1.0);
  

   }
   
   if (entering == 4) {

   OpenPattuple("/storage2/sfonseca/CMSSW/pythia_QCD_exclusive/CMSSW_4_2_8/src/ForwardAnalysis/ForwardTTreeAnalysis/test/QCD_pythia/QCD_Pt-80to120_TuneZ2_7TeV_pythia6/res/Exclusive_DijetsAnalysis_80to120.root");
   
   cout << "\n MC Sample: Jets pT -> [80,120] GeV/c\n" << endl;
   
   RunExclusive("Output_MC_Pythia6/hltexcldijet30Uand-qcd_80to120.root",60,60,1,0, false, true, false, 3.74475039339267, 1.0);
   RunExclusive("Output_MC_Pythia6/hltexcldijet30Uor-qcd_80to120.root",60,60,1,1, false, true, false, 1.6060590796693,  1.0);
   RunExclusive("Output_MC_Pythia6/hltexcld30U-qcd_80to120.root",60,60,1,2, false, true, false, 0.003106600879201,  1.0);
//3.74475039339267	1.6060590796693	        0.003106600879201
   }
   
   if (entering == 5) {

   OpenPattuple("/storage2/sfonseca/CMSSW/pythia_QCD_exclusive/CMSSW_4_2_8/src/ForwardAnalysis/ForwardTTreeAnalysis/test/QCD_pythia/QCD_Pt-120to170_TuneZ2_7TeV_pythia6/res/Exclusive_DijetsAnalysis_120to170.root");
   
   cout << "\n MC Sample: Jets pT -> [120,170] GeV/c\n" << endl;
   
   RunExclusive("Output_MC_Pythia6/hltexcldijet30Uand-qcd_120to170.root",60,60,1,0, false, true, false, 0.47445320115444, 1.0);
   RunExclusive("Output_MC_Pythia6/hltexcldijet30Uor-qcd_120to170.root",60,60,1,1, false, true, false, 0.20348482316385,  1.0);
   RunExclusive("Output_MC_Pythia6/hltexcld30U-qcd_120to170.root",60,60,1,2, false, true, false, 0.000393600794981,  1.0);
//0.47445320115444	0.20348482316385	0.000393600794981   

   }
   
   if (entering == 6) {

   OpenPattuple("/storage2/sfonseca/CMSSW/pythia_QCD_exclusive/CMSSW_4_2_8/src/ForwardAnalysis/ForwardTTreeAnalysis/test/QCD_pythia/QCD_Pt-170to300_TuneZ2_7TeV_pythia6/res/Exclusive_DijetsAnalysis_170to300.root");
   
   cout << "\n MC Sample: Jets pT -> [170,300] GeV/c\n" << endl;
   
   RunExclusive("Output_MC_Pythia6/hltexcldijet30Uand-qcd_170to300.root",60,60,1,0, false, true, false, 0.098512792205911,  1.0);
   RunExclusive("Output_MC_Pythia6/hltexcldijet30Uor-qcd_170to300.root",60,60,1,1, false, true, false, 0.042250443357998,  1.0);
   RunExclusive("Output_MC_Pythia6/hltexcld30U-qcd_170to300.root",60,60,1,2, false, true, false, 0.00008172505367,  1.0);
 //0.098512792205911	0.042250443357998	0.00008172505367
   }
   
   if (entering == 7) {

   OpenPattuple("/storage2/sfonseca/CMSSW/pythia_QCD_exclusive/CMSSW_4_2_8/src/ForwardAnalysis/ForwardTTreeAnalysis/test/QCD_pythia/QCD_Pt-300to470_TuneZ2_7TeV_pythia6/res/Exclusive_DijetsAnalysis_300to470.root");
   
   cout << "\n MC Sample: Jets pT -> [300,470] GeV/c\n" << endl;
   
   RunExclusive("Output_MC_Pythia6/hltexcldijet30Uand-qcd_300to470.root",60,60,1,0, false, true, false, 0.00458163539893,  1.0);
   RunExclusive("Output_MC_Pythia6/hltexcldijet30Uor-qcd_300to470.root",60,60,1,1, false, true, false, 0.001964984674324,  1.0);
   RunExclusive("Output_MC_Pythia6/hltexcld30U-qcd_300to470.root",60,60,1,2, false, true, false, 0.000003800870836,  1.0);
   //0.00458163539893	0.001964984674324	0.000003800870836 
   }
   
   if (entering == 8) {

   OpenPattuple("/storage2/sfonseca/CMSSW/pythia_QCD_exclusive/CMSSW_4_2_8/src/ForwardAnalysis/ForwardTTreeAnalysis/test/QCD_pythia/QCD_Pt-470to600_TuneZ2_7TeV_pythia6/res/Exclusive_DijetsAnalysis_470to600.root");
   
   cout << "\n MC Sample: Jets pT -> [470,600] GeV/c\n" << endl;

   RunExclusive("Output_MC_Pythia6/hltexcldijet30Uand-qcd_470to600.root",60,60,1,0, false, true, false, 0.000444510544781,  1.0);
   RunExclusive("Output_MC_Pythia6/hltexcldijet30Uor-qcd_470to600.root",60,60,1,1, false, true, false, 0.000190642932494,  1.0);
   RunExclusive("Output_MC_Pythia6/hltexcld30U-qcd_470to600.root",60,60,1,2, false, true, false, 0.000000368760719,  1.0);
   //0.000444510544781	0.000190642932494	0.000000368760719
   }
   
   if (entering == 9) {

   OpenPattuple("/storage2/sfonseca/CMSSW/pythia_QCD_exclusive/CMSSW_4_2_8/src/ForwardAnalysis/ForwardTTreeAnalysis/test/QCD_pythia/QCD_Pt-600to800_TuneZ2_7TeV_pythia6/res/Exclusive_DijetsAnalysis_600to800.root");
   
   cout << "\n MC Sample: Jets pT -> [600,800] GeV/c\n" << endl;
   
   RunExclusive("Output_MC_Pythia6/hltexcldijet30Uand-qcd_600to800.root",60,60,1,0, false, true, false, 0.000092416706805,  1.0);
   RunExclusive("Output_MC_Pythia6/hltexcldijet30Uor-qcd_600to800.root",60,60,1,1,false, true, false, 0.000039635937108,  1.0);
   RunExclusive("Output_MC_Pythia6/hltexcld30U-qcd_600to800.root",60,60,1,2, false, true, false, 0.000000076667813,  1.0);
   //0.000092416706805	0.000039635937108	0.000000076667813
   }


}

void EventMacro::OpenPattuple(string filein_){

TTree *tr;

  inf = TFile::Open(filein_.c_str());
  tr = (TTree*)inf->Get("forwardQCDTTreeAnalysis/ProcessedTree");

            DiffractiveEvent *eventdiff = new DiffractiveEvent();
            ExclusiveDijetsEvent *eventexcl = new ExclusiveDijetsEvent();
            TBranch *diff = tr->GetBranch("DiffractiveAnalysis");
            TBranch *excl = tr->GetBranch("ExclusiveDijetsAnalysis");
            diff->SetAddress(&eventdiff);
            excl->SetAddress(&eventexcl);

}
