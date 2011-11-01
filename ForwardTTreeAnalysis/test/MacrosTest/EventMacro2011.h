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

#ifndef EventMacro2011_h
#define EventMacro2011_h

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
LoadFiles(0);
//LoadFiles(1);
//LoadFiles(2);
//LoadFiles(3);
//LoadFiles(4);
//LoadFiles(5);
//LoadFiles(6);
//LoadFiles(7);
//LoadFiles(8);
//LoadFiles(9);


}

EventMacro::LoadFiles(int entering)
{

  // FWLite libraries
  gSystem->Load("libFWCoreFWLite.so");
  gSystem->Load("libPhysicsToolsUtilities.so");
  AutoLibraryLoader::enable();

   if (entering == 0) {

   OpenPattuple("teste.root");

   cout << "\n Teste \n" << endl;

   RunExclusive("out-teste.root",1,1,1,0,false,1286120.9,  0.987);

   }


   if (entering == 1) {

   OpenPattuple("/storage2/dmf/Common/OutputFiles/MCPythia6/QCD_Pt-15to30-Z2_PUS3.root");
   
   cout << "\n MC Sample: Jets pT -> [15,30] GeV/c\n" << endl;
   
   RunExclusive("v4hltjet60-qcd_15to30.root",60,60,1,0,false,1286120.9,  0.987);
   RunExclusive("v4hltexcldijet60and-qcd_15to30.root",60,60,1,1,false,1286120.9,  0.0154);
   RunExclusive("v4hltexcldijet60Uor-qcd_15to30.root",60,60,1,2,false,1286120.9,  0.0016);
   
   RunExclusive("v5hltjet60-qcd_15to30.root",60,60,1,0,false,1286120.9,  0.99315852102092);
   RunExclusive("v5hltexcldijet60and-qcd_15to30.root",60,60,1,1,false,1286120.9,  0.010051078366032);
   RunExclusive("v5hltexcldijet60or-qcd_15to30.root",60,60,1,2,false,1286120.9,  0.000275224159285);
   
   RunExclusive("v6hltjet60-qcd_15to30.root",60,60,1,0,false,1286120.9,  0.99466333611528);
   RunExclusive("v6hltexcldijet60and-qcd_15to30.root",60,60,1,1,false,1286120.9,  0.007451719650059);
   RunExclusive("v6hltexcldijet60or-qcd_15to30.root",60,60,1,2,false,1286120.9,  0.000462230730173);
   
   }

  if (entering == 2) {

   OpenPattuple("/storage2/dmf/Common/OutputFiles/MCPythia6/QCD_Pt-30to50-Z2_PUS3.root");
   
   cout << "\n MC Sample: Jets pT -> [30,50] GeV/c\n" << endl;
   
   RunExclusive("v4hltjet60-qcd_30to50.root",60,60,1,0,false,81222.1798400012,  0.987);
   RunExclusive("v4hltexcldijet60and-qcd_30to50.root",60,60,1,1,false,81222.1798400012,  0.0154);
   RunExclusive("v4hltexcldijet60Uor-qcd_30to50.root",60,60,1,2,false,81222.1798400012,  0.0016);
   
   RunExclusive("v5hltjet60-qcd_30to50.root",60,60,1,0,false,81222.1798400012,  0.99315852102092);
   RunExclusive("v5hltexcldijet60and-qcd_30to50.root",60,60,1,1,false,81222.1798400012,  0.010051078366032);
   RunExclusive("v5hltexcldijet60or-qcd_30to50.root",60,60,1,2,false,81222.1798400012,  0.000275224159285);
   
   RunExclusive("v6hltjet60-qcd_30to50.root",60,60,1,0,false,81222.1798400012,  0.99466333611528);
   RunExclusive("v6hltexcldijet60and-qcd_30to50.root",60,60,1,1,false,81222.1798400012,  0.007451719650059);
   RunExclusive("v6hltexcldijet60or-qcd_30to50.root",60,60,1,2,false,81222.1798400012,  0.000462230730173);
   

   }
   
   if (entering == 3) {

   OpenPattuple("/storage2/dmf/Common/OutputFiles/MCPythia6/QCD_Pt-50to80-Z2_PUS3.root");
   
   cout << "\n MC Sample: Jets pT -> [50,80] GeV/c\n" << endl;
   
   RunExclusive("v4hltjet60-qcd_50to80.root",60,60,1,0,false,9821.32776565658,  0.987);
   RunExclusive("v4hltexcldijet60and-qcd_50to80.root",60,60,1,1,false,9821.32776565658,  0.0154);
   RunExclusive("v4hltexcldijet60Uor-qcd_50to80.root",60,60,1,2,false,9821.32776565658,  0.0016);
   
   RunExclusive("v5hltjet60-qcd_50to80.root",60,60,1,0,false,9821.32776565658,  0.99315852102092);
   RunExclusive("v5hltexcldijet60and-qcd_50to80.root",60,60,1,1,false,9821.32776565658,  0.010051078366032);
   RunExclusive("v5hltexcldijet60or-qcd_50to80.root",60,60,1,2,false,9821.32776565658,  0.000275224159285);
   
   RunExclusive("v6hltjet60-qcd_50to80.root",60,60,1,0,false,9821.32776565658,  0.99466333611528);
   RunExclusive("v6hltexcldijet60and-qcd_50to80.root",60,60,1,1,false,9821.32776565658,  0.007451719650059);
   RunExclusive("v6hltexcldijet60or-qcd_50to80.root",60,60,1,2,false,9821.32776565658,  0.000462230730173);
   

   }
   
   if (entering == 4) {

   OpenPattuple("/storage2/dmf/Common/OutputFiles/MCPythia6/QCD_Pt-80to120-Z2_PUS3.root");
   
   cout << "\n MC Sample: Jets pT -> [80,120] GeV/c\n" << endl;
   
   RunExclusive("v4hltjet60-qcd_80to120.root",60,60,1,0,false,1199.2197976,  0.987);
   RunExclusive("v4hltexcldijet60and-qcd_80to120.root",60,60,1,1,false,1199.2197976,  0.0154);
   RunExclusive("v4hltexcldijet60Uor-qcd_80to120.root",60,60,1,2,false,1199.2197976,  0.0016);
   
   RunExclusive("v5hltjet60-qcd_80to120.root",60,60,1,0,false,1199.2197976,  0.99315852102092);
   RunExclusive("v5hltexcldijet60and-qcd_80to120.root",60,60,1,1,false,1199.2197976,  0.010051078366032);
   RunExclusive("v5hltexcldijet60or-qcd_80to120.root",60,60,1,2,false,1199.2197976,  0.000275224159285);
   
   RunExclusive("v6hltjet60-qcd_80to120.root",60,60,1,0,false,1199.2197976,  0.99466333611528);
   RunExclusive("v6hltexcldijet60and-qcd_80to120.root",60,60,1,1,false,1199.2197976,  0.007451719650059);
   RunExclusive("v6hltexcldijet60or-qcd_80to120.root",60,60,1,2,false,1199.2197976,  0.000462230730173);
   

   }
   
   if (entering == 5) {

   OpenPattuple("/storage2/dmf/Common/OutputFiles/MCPythia6/QCD_Pt-120to170-Z2_PUS3.root");
   
   cout << "\n MC Sample: Jets pT -> [120,170] GeV/c\n" << endl;
   
   RunExclusive("v4hltjet60-qcd_120to170.root",60,60,1,0,false,175.9915832,  0.987);
   RunExclusive("v4hltexcldijet60and-qcd_120to170.root",60,60,1,1,false,175.9915832,  0.0154);
   RunExclusive("v4hltexcldijet60Uor-qcd_120to170.root",60,60,1,2,false,175.9915832,  0.0016);
   
   RunExclusive("v5hltjet60-qcd_120to170.root",60,60,1,0,false,175.9915832,  0.99315852102092);
   RunExclusive("v5hltexcldijet60and-qcd_120to170.root",60,60,1,1,false,175.9915832,  0.010051078366032);
   RunExclusive("v5hltexcldijet60or-qcd_120to170.root",60,60,1,2,false,175.9915832,  0.000275224159285);
   
   RunExclusive("v6hltjet60-qcd_120to170.root",60,60,1,0,false,175.9915832,  0.99466333611528);
   RunExclusive("v6hltexcldijet60and-qcd_120to170.root",60,60,1,1,false,175.9915832,  0.007451719650059);
   RunExclusive("v6hltexcldijet60or-qcd_120to170.root",60,60,1,2,false,175.9915832,  0.000462230730173);
   

   }
   
   if (entering == 6) {

   OpenPattuple("/storage2/dmf/Common/OutputFiles/MCPythia6/QCD_Pt-170to300-Z2_PUS3.root");
   
   cout << "\n MC Sample: Jets pT -> [170,300] GeV/c\n" << endl;
   
   RunExclusive("v4hltjet60-qcd_170to300.root",60,60,1,0,false,37.09431632,  0.987);
   RunExclusive("v4hltexcldijet60and-qcd_170to300.root",60,60,1,1,false,37.09431632,  0.0154);
   RunExclusive("v4hltexcldijet60Uor-qcd_170to300.root",60,60,1,2,false,37.09431632,  0.0016);
   
   RunExclusive("v5hltjet60-qcd_170to300.root",60,60,1,0,false,37.09431632,  0.99315852102092);
   RunExclusive("v5hltexcldijet60and-qcd_170to300.root",60,60,1,1,false,37.09431632,  0.010051078366032);
   RunExclusive("v5hltexcldijet60or-qcd_170to300.root",60,60,1,2,false,37.09431632,  0.000275224159285);
   
   RunExclusive("v6hltjet60-qcd_170to300.root",60,60,1,0,false,37.09431632,  0.99466333611528);
   RunExclusive("v6hltexcldijet60and-qcd_170to300.root",60,60,1,1,false,37.09431632,  0.007451719650059);
   RunExclusive("v6hltexcldijet60or-qcd_170to300.root",60,60,1,2,false,37.09431632,  0.000462230730173);
   

   }
   
   if (entering == 7) {

   OpenPattuple("/storage2/dmf/Common/OutputFiles/MCPythia6/QCD_Pt-300to470-Z2_PUS3.root");
   
   cout << "\n MC Sample: Jets pT -> [300,470] GeV/c\n" << endl;
   
   RunExclusive("v4hltjet60-qcd_300to470.root",60,60,1,0,false,1.785909376,  0.987);
   RunExclusive("v4hltexcldijet60and-qcd_300to470.root",60,60,1,1,false,1.785909376,  0.0154);
   RunExclusive("v4hltexcldijet60Uor-qcd_300to470.root",60,60,1,2,false,1.785909376,  0.0016);
   
   RunExclusive("v5hltjet60-qcd_300to470.root",60,60,1,0,false,1.785909376,  0.99315852102092);
   RunExclusive("v5hltexcldijet60and-qcd_300to470.root",60,60,1,1,false,1.785909376,  0.010051078366032);
   RunExclusive("v5hltexcldijet60or-qcd_300to470.root",60,60,1,2,false,1.785909376,  0.000275224159285);
   
   RunExclusive("v6hltjet60-qcd_300to470.root",60,60,1,0,false,1.785909376,  0.99466333611528);
   RunExclusive("v6hltexcldijet60and-qcd_300to470.root",60,60,1,1,false,1.785909376,  0.007451719650059);
   RunExclusive("v6hltexcldijet60or-qcd_300to470.root",60,60,1,2,false,1.785909376,  0.000462230730173);
   

   }
   
   if (entering == 8) {

   OpenPattuple("/storage2/dmf/Common/OutputFiles/MCPythia6/QCD_Pt-470to600-Z2_PUS3.root");
   
   cout << "\n MC Sample: Jets pT -> [470,600] GeV/c\n" << endl;
   
   RunExclusive("v4hltjet60-qcd_470to600.root",60,60,1,0,false,0.10736862704,  0.987);
   RunExclusive("v4hltexcldijet60and-qcd_470to600.root",60,60,1,1,false,0.10736862704,  0.0154);
   RunExclusive("v4hltexcldijet60Uor-qcd_470to600.root",60,60,1,2,false,0.10736862704,  0.0016);
   
   RunExclusive("v5hltjet60-qcd_470to600.root",60,60,1,0,false,0.10736862704,  0.99315852102092);
   RunExclusive("v5hltexcldijet60and-qcd_470to600.root",60,60,1,1,false,0.10736862704,  0.010051078366032);
   RunExclusive("v5hltexcldijet60or-qcd_470to600.root",60,60,1,2,false,0.10736862704,  0.000275224159285);
   
   RunExclusive("v6hltjet60-qcd_470to600.root",60,60,1,0,false,0.10736862704,  0.99466333611528);
   RunExclusive("v6hltexcldijet60and-qcd_470to600.root",60,60,1,1,false,0.10736862704,  0.007451719650059);
   RunExclusive("v6hltexcldijet60or-qcd_470to600.root",60,60,1,2,false,0.10736862704,  0.000462230730173);
   

   }
   
   if (entering == 9) {

   OpenPattuple("/storage2/dmf/Common/OutputFiles/MCPythia6/QCD_Pt-600to800-Z2_PUS3.root");
   
   cout << "\n MC Sample: Jets pT -> [600,800] GeV/c\n" << endl;
   
   RunExclusive("v4hltjet60-qcd_600to800.root",60,60,1,0,false,0.0237764476,  0.987);
  /RunExclusive("v4hltexcldijet60and-qcd_600to800.root",60,60,1,1,false,0.0237764476,  0.0154);
   RunExclusive("v4hltexcldijet60Uor-qcd_600to800.root",60,60,1,2,false,0.0237764476,  0.0016);
   
   RunExclusive("v5hltjet60-qcd_600to800.root",60,60,1,0,false,0.0237764476,  0.99315852102092);
  /RunExclusive("v5hltexcldijet60and-qcd_600to800.root",60,60,1,1,false,0.0237764476,  0.010051078366032);
   RunExclusive("v5hltexcldijet60or-qcd_600to800.root",60,60,1,2,false,0.0237764476,  0.000275224159285);
   
   RunExclusive("v6hltjet60-qcd_600to800.root",60,60,1,0,false,0.0237764476,  0.99466333611528);
   RunExclusive("v6hltexcldijet60and-qcd_600to800.root",60,60,1,1,false,0.0237764476,  0.007451719650059);
   RunExclusive("v6hltexcldijet60or-qcd_600to800.root",60,60,1,2,false,0.0237764476,  0.000462230730173);
   

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
