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

#ifndef EventMacroTest_h
#define EventMacroTest_h

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

}

EventMacro::LoadFiles(int entering)
{

  // FWLite libraries
  gSystem->Load("libFWCoreFWLite.so");
  gSystem->Load("libPhysicsToolsUtilities.so");
  AutoLibraryLoader::enable();

   if (entering == 1) {

   OpenPattuple("QCD.root");
   
   cout << "\n MC Sample Test 1\n" << endl;
   
   RunExclusive("teste1.root",10,20,1,0,false,2,  10);
   RunExclusive("teste1b.root",30,40,1,1,false,3,  100);
    
   }

  if (entering == 2) {

   OpenPattuple("QCD_Pt-5to15-Z2_S3.root");
   
   cout << "\n MC Sample Test 2\n" << endl;
   
   RunExclusive("teste2.root",50,60,1,0,false,4,  1000);
   RunExclusive("teste2b.root",70,80,1,1,false,5,  10000);
 

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
