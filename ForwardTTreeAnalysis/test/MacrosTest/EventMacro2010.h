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
//            dataOn = true (if is running data). This apply weightpu = 1, false (if is running mc);
//            weight = luminosity weight (L_data/L_MC) for MC;
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
    void RunExclusive(string savehistofile, double jet1PT, double jet2PT, int optnVertex, int optTrigger, bool dataon, double weightlumi, double triggereff);
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

}

EventMacro::LoadFiles(int entering)
{

  // FWLite libraries
  gSystem->Load("libFWCoreFWLite.so");
  gSystem->Load("libPhysicsToolsUtilities.so");
  AutoLibraryLoader::enable();

   if (entering == 1) {

   OpenPattuple("/storage2/dmf/Common/OutputFiles/MCPythia6/QCD_Pt-15to30-Z2_PUS3.root");
   
   cout << "\n MC Sample: Jets pT -> [15,30] GeV/c\n" << endl;
   
   RunExclusive("hltexcldijet30Uand-qcd_15to30.root",60,60,1,0,false,21247.8864948501,  0.059207885504661);
   RunExclusive("hltexcldijet30Uor-qcd_15to30.root",60,60,1,1,false,21247.8864948501,  0.95513643211874);
   RunExclusive("hltjet30U-qcd_15to30.root",60,60,1,2,false,21247.8864948501,  0.021555176752449);
    
   }

  if (entering == 2) {

   OpenPattuple("/storage2/dmf/Common/OutputFiles/MCPythia6/QCD_Pt-30to50-Z2_PUS3.root");
   
   cout << "\n MC Sample: Jets pT -> [30,50] GeV/c\n" << endl;
   
   RunExclusive("hltexcldijet30Uand-qcd_30to50.root",60,60,1,0,false,1341.86432000002,  0.059207885504661);
   RunExclusive("hltexcldijet30Uor-qcd_30to50.root",60,60,1,1,false,1341.86432000002,  0.95513643211874);
   RunExclusive("hltjet30U-qcd_30to50.root",60,60,1,2,false,1341.86432000002,  0.021555176752449);
  

   }
   
   if (entering == 3) {

   OpenPattuple("/storage2/dmf/Common/OutputFiles/MCPythia6/QCD_Pt-50to80-Z2_PUS3.root");
   
   cout << "\n MC Sample: Jets pT -> [50,80] GeV/c\n" << endl;
   
   RunExclusive("hltexcldijet30Uand-qcd_50to80.root",60,60,1,0,false,162.257271717172,  0.059207885504661);
   RunExclusive("hltexcldijet30Uor-qcd_50to80.root",60,60,1,1,false,162.257271717172,  0.95513643211874);
   RunExclusive("hltjet30U-qcd_50to80.root",60,60,1,2,false,162.257271717172,  0.021555176752449);
  
   }
   
   if (entering == 4) {

   OpenPattuple("/storage2/dmf/Common/OutputFiles/MCPythia6/QCD_Pt-80to120-Z2_PUS3.root");
   
   cout << "\n MC Sample: Jets pT -> [80,120] GeV/c\n" << endl;
   
   RunExclusive("hltexcldijet30Uand-qcd_80to120.root",60,60,1,0,false,19.8122023,  0.059207885504661);
   RunExclusive("hltexcldijet30Uor-qcd_80to120.root",60,60,1,1,false,19.8122023,  0.95513643211874);
   RunExclusive("hltjet30U-qcd_80to120.root",60,60,1,2,false,19.8122023,  0.021555176752449);

   }
   
   if (entering == 5) {

   OpenPattuple("/storage2/dmf/Common/OutputFiles/MCPythia6/QCD_Pt-120to170-Z2_PUS3.root");
   
   cout << "\n MC Sample: Jets pT -> [120,170] GeV/c\n" << endl;
   
   RunExclusive("hltexcldijet30Uand-qcd_120to170.root",60,60,1,0,false,2.9075411,  0.059207885504661);
   RunExclusive("hltexcldijet30Uor-qcd_120to170.root",60,60,1,1,false,2.9075411,  0.95513643211874);
   RunExclusive("hltjet30U-qcd_120to170.root",60,60,1,2,false,2.9075411,  0.021555176752449);
   

   }
   
   if (entering == 6) {

   OpenPattuple("/storage2/dmf/Common/OutputFiles/MCPythia6/QCD_Pt-170to300-Z2_PUS3.root");
   
   cout << "\n MC Sample: Jets pT -> [170,300] GeV/c\n" << endl;
   
   RunExclusive("hltexcldijet30Uand-qcd_170to300.root",60,60,1,0,false,0.61283186,  0.059207885504661);
   RunExclusive("hltexcldijet30Uor-qcd_170to300.root",60,60,1,1,false,0.61283186,  0.95513643211874);
   RunExclusive("hltjet30U-qcd_170to300.root",60,60,1,2,false,0.61283186,  0.021555176752449);
 
   }
   
   if (entering == 7) {

   OpenPattuple("/storage2/dmf/Common/OutputFiles/MCPythia6/QCD_Pt-300to470-Z2_PUS3.root");
   
   cout << "\n MC Sample: Jets pT -> [300,470] GeV/c\n" << endl;
   
   RunExclusive("hltexcldijet30Uand-qcd_300to470.root",60,60,1,0,false,0.029504848,  0.059207885504661);
   RunExclusive("hltexcldijet30Uor-qcd_300to470.root",60,60,1,1,false,0.029504848,  0.95513643211874);
   RunExclusive("hltjet30U-qcd_300to470.root",60,60,1,2,false,0.029504848,  0.021555176752449);
   
   }
   
   if (entering == 8) {

   OpenPattuple("/storage2/dmf/Common/OutputFiles/MCPythia6/QCD_Pt-470to600-Z2_PUS3.root");
   
   cout << "\n MC Sample: Jets pT -> [470,600] GeV/c\n" << endl;

   RunExclusive("hltexcldijet30Uand-qcd_470to600.root",60,60,1,0,false,0.00177382742,  0.059207885504661);
   RunExclusive("hltexcldijet30Uor-qcd_470to600.root",60,60,1,1,false,0.00177382742,  0.95513643211874);
   RunExclusive("hltjet30U-qcd_470to600.root",60,60,1,2,false,0.00177382742,  0.021555176752449);

   }
   
   if (entering == 9) {

   OpenPattuple("/storage2/dmf/Common/OutputFiles/MCPythia6/QCD_Pt-600to800-Z2_PUS3.root");
   
   cout << "\n MC Sample: Jets pT -> [600,800] GeV/c\n" << endl;
   
   RunExclusive("hltexcldijet30Uand-qcd_600to800.root",60,60,1,0,false,0.00039280855,  0.059207885504661);
   RunExclusive("hltexcldijet30Uor-qcd_600to800.root",60,60,1,1,false,0.00039280855,  0.95513643211874);
   RunExclusive("hltjet30U-qcd_600to800.root",60,60,1,2,false,0.00039280855,  0.021555176752449);

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
