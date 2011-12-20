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
//

#ifndef EventMacroData2011_h
#define EventMacroData2011_h

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
}

EventMacro::LoadFiles(int entering)
{

  // FWLite libraries
  gSystem->Load("libFWCoreFWLite.so");
  gSystem->Load("libPhysicsToolsUtilities.so");
  AutoLibraryLoader::enable();

  if (entering == 1) {

   OpenPattuple("/storage2/sfonseca/CMSSW/FWD_production_2011_Oct2011/CMSSW_4_2_8/src/ForwardAnalysis/ForwardTTreeAnalysis/test/crab_MultiJet_Run2011A_v4_PAT_ROOTTREE_CASTOR_JSON/res/forwardQCDTTreeAnalysis_v4.root");
   RunExclusive("datav4_2011_HFAND60.root",100,100,1,0,false,false,false,1,1);
   RunExclusive("datav4_2011_HFOR60.root",100,100,1,1,false,false,false,1,1);
   RunExclusive("datav4_2011_Jet60.root",100,100,1,2,false,false,false,1,1);

   }

    if (entering == 2) {

   OpenPattuple("/storage2/sfonseca/CMSSW/FWD_production_2011_Oct2011/CMSSW_4_2_8/src/ForwardAnalysis/ForwardTTreeAnalysis/test/crab_MultiJet_Run2011A_v5_PAT_ROOTTREE_CASTOR_JSON/res/forwardQCDTTreeAnalysis_v5.root");
   RunExclusive("datav5_2011_HFAND60.root",100,100,1,0,false,false,false,1,1);
   RunExclusive("datav5_2011_HFOR60.root",100,100,1,1,false,false,false,1,1);
   RunExclusive("datav5_2011_Jet60.root",100,100,1,2,false,false,false,1,1);

   }

     if (entering == 3) {

   OpenPattuple("/storage2/sfonseca/CMSSW/FWD_production_2011_Oct2011/CMSSW_4_2_8/src/ForwardAnalysis/ForwardTTreeAnalysis/test/crab_MultiJet_Run2011A_v6_PAT_ROOTTREE_CASTOR_JSON/res/forwardQCDTTreeAnalysis_v6.root");
   RunExclusive("datav6_2011_HFAND60.root",100,100,1,0,false,false,false,1,1);
   RunExclusive("datav6_2011_HFOR60.root",100,100,1,1,false,false,false,1,1);
   RunExclusive("datav6_2011_Jet60.root",100,100,1,2,false,false,false,1,1);

   }

}

void EventMacro::OpenPattuple(string filein_){

TTree *tr;

  inf = TFile::Open(filein_.c_str());
  // 2011
  tr = (TTree*)inf->Get("forwardQCDTTreeAnalysis/ProcessedTree");
            DiffractiveEvent *eventdiff = new DiffractiveEvent();
            ExclusiveDijetsEvent *eventexcl = new ExclusiveDijetsEvent();
            //QCDEvent *eventqcd = new QCDEvent();
            TBranch *diff = tr->GetBranch("DiffractiveAnalysis");
            TBranch *excl = tr->GetBranch("ExclusiveDijetsAnalysis");
            //TBranch *qcd = tr->GetBranch("QCDAnalysis");
            diff->SetAddress(&eventdiff);
            excl->SetAddress(&eventexcl);
            //qcd->SetAddress(&eventqcd);
}
