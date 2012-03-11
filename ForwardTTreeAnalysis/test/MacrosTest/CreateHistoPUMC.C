#include "TH1F.h"
#include "TH2F.h"
#include "TProfile.h"
#include "TChain.h"
#include "TTree.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TGraphAsymmErrors.h"
#include "TMath.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TChain.h"
#include "TH1F.h"

#include "ForwardAnalysis/ForwardTTreeAnalysis/interface/DiffractiveEvent.h"
#include "ForwardAnalysis/ForwardTTreeAnalysis/interface/ExclusiveDijetsEvent.h"

#include <cmath>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <algorithm>

void CreateHistoPUMC(){

  //gSystem->Load("libFWCoreFWLite.so");
  //gSystem->Load("libPhysicsToolsUtilities.so");
  //AutoLibraryLoader::enable();

  // Loading Files
  TFile *inf = TFile::Open("/storage2/dmf/Common/OutputFiles/MCPythia6/QCD_All_Pt-15to3000-Z2_PUS3.root");
  TTree *tr = (TTree*)inf->Get("forwardQCDTTreeAnalysis/ProcessedTree"); 

  // Define Branch Name
  DiffractiveEvent *eventdiff = new DiffractiveEvent();
  ExclusiveDijetsEvent *eventexcl = new ExclusiveDijetsEvent();

  TBranch *diff = tr->GetBranch("DiffractiveAnalysis");
  TBranch *excl = tr->GetBranch("ExclusiveDijetsAnalysis");

  diff->SetAddress(&eventdiff);
  excl->SetAddress(&eventexcl);

  TFile *outf = new TFile("pileup15to3000mc.root","RECREATE");
  int NEVENTS = tr->GetEntriesFast();

TH1D *h_puBxm1 = new TH1D("pileupmcBxm1","PileUp Monte Carlo; # Pile Up; N events",25,0,25);
TH1D *h_puBx0 = new TH1D("pileupmcBx0","PileUp Monte Carlo; # Pile Up; N events",25,0,25);
TH1D *h_puBxp1 = new TH1D("pileupmcBxp1","PileUp Monte Carlo; # Pile Up; N events",25,0,25);
// Tree Reading 
unsigned NEntries = tr->GetEntries();
int decade = 0;
/*
             cout << "BEFORE TRIGGER" << endl;
             cout << "PileUp-1: " << eventexcl->GetNPileUpBxm1() << endl; 
             cout << "PileUp0: " << eventexcl->GetNPileUpBx0() << endl;
             cout << "PileUp+1: " << eventexcl->GetNPileUpBxp1() << endl;
             cout << "==============" << endl;   
*/


// Event by Event Analysis
//////////////////////////

for(unsigned i=0;i<NEVENTS;i++) {

    //----------- Progress Report -------------
    double progress = 10.0*i/(1.0*NEVENTS);
    int k = TMath::FloorNint(progress); 
    if (k > decade) 
     std::cout<<10*k<<" %"<<std::endl;
    decade = k;          

    //----------- Read the Event --------------
    tr->GetEntry(i);
    h_puBxm1->Fill(eventexcl->GetNPileUpBxm1());
    h_puBx0->Fill(eventexcl->GetNPileUpBx0());
    h_puBxp1->Fill(eventexcl->GetNPileUpBxp1());

}// Run All Events

outf->Write();

}
