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

#ifndef EventMacroData2010_h
#define EventMacroData2010_h

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
}

EventMacro::LoadFiles(int entering)
{

  // FWLite libraries
  gSystem->Load("libFWCoreFWLite.so");
  gSystem->Load("libPhysicsToolsUtilities.so");
  AutoLibraryLoader::enable();

  if (entering == 1) {

   OpenPattuple("/storage2/dmf/Common/OutputFiles/PatSamples2010/PATTupleMultiJets2010B.root");
   RunExclusive("dataHisto2010.root",60,60,1,0, false, false, false, 1, 1);

   }


}

void EventMacro::OpenPattuple(string filein_){

TTree *tr;

  inf = TFile::Open(filein_.c_str());
  // 2010
  tr = (TTree*)inf->Get("exclusiveDijetsTTreeAnalysis/ProcessedTree");

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
