//-------------------------------------------------------------------------------------------------------->>
// Forward Group 
//-------------------------------------------------------------------------------------------------------->>
// Project: Diffractive Z Analysis
//-------------------------------------------------------------------------------------------------------->>


//#if !defined(__CINT__) || defined(__MAKECINT__)
#include <TROOT.h>
#include <TChain.h>
#include <TString.h>
#include <TH1.h>
#include <TH2.h>
#include <TTree.h>
#include <TMath.h>

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <fstream>

#include "DiffractiveZComp.h"
#include "ForwardAnalysis/ForwardTTreeAnalysis/interface/EventInfoEvent.h"
#include "ForwardAnalysis/ForwardTTreeAnalysis/interface/ExclusiveDijetsEvent.h"
#include "ForwardAnalysis/ForwardTTreeAnalysis/interface/DiffractiveEvent.h"

using namespace diffractiveAnalysis;
using namespace diffractiveZAnalysis;
using namespace eventInfo;

void DiffractiveZComp::LoadFile(std::string fileinput, std::string processinput){

  inf = NULL;
  tr  = NULL;
  inf = TFile::Open(fileinput.c_str(),"read");
  tr = (TTree*)inf->Get(processinput.c_str());
  eventdiff = new DiffractiveEvent();
  eventdiffZ = new DiffractiveZEvent();
  eventinfo = new EventInfoEvent();
  diff = tr->GetBranch("DiffractiveAnalysis");
  diffZ = tr->GetBranch("DifractiveZAnalysis");
  info = tr->GetBranch("EventInfo");
  diff->SetAddress(&eventdiff);
  diffZ->SetAddress(&eventdiffZ);
  info->SetAddress(&eventinfo);

}

void DiffractiveZComp::Run(std::string filein_, std::string savehistofile_){

  filein = filein_;
  savehistofile = savehistofile_;

  std::cout << "" << std::endl;
  std::cout << "Running..." << std::endl;
  std::cout << "" << std::endl;
  std::cout << "<< INPUTS >>" << std::endl;
  std::cout << " " << std::endl;
  std::cout << "Input file: " << filein << std::endl;
  std::cout << "Output file: " << savehistofile << std::endl;
  std::cout << " " << std::cout; 
  std::cout << "# Vertex: " << optnVertex << std::endl;
  std::cout << "Trigger Option: " << optTrigger << std::endl;
  std::cout << " " << std::endl;
  std::cout << "--> TRUE = 1 FALSE = 0" << std::endl;
  std::cout << "Trigger Switch: " << switchTrigger << std::endl;
  std::cout << "Pre Selection Switch: " << switchPreSel << std::endl;
  std::cout << "" << std::endl;

  if (nVertex == 0){

    std::cout << "---------------------------------------------------------------" << std::endl;
    std::cout << "Please, restart your setup. Respect the Condition # Vertex > 0)" << std::endl;
    std::cout << "---------------------------------------------------------------" << std::endl;
    return;

  }

  LoadFile(filein,processname);

  TFile *outf = new TFile(savehistofile.c_str(),"RECREATE");

  //int NEVENTS = tr->GetEntriesFast();
  int NEVENTS = tr->GetEntries();

  TH1::SetDefaultSumw2(true);
  TH2::SetDefaultSumw2(true);

  unsigned NEntries = tr->GetEntries();

  std::cout << "" << std::endl;
  std::cout<< "Reading Tree: "<< NEntries << " events"<<std::endl;
  std::cout << "" << std::endl;

  int decade = 0;

  std::vector <std::string> Folders;
  Folders.push_back("without_cuts");
  Folders.push_back("with_trigger");

  int nloop;
  int indexV;

  if (switchMultiple){
   nloop = 21;
  }
  
  else {
   nloop = 1;
  }


  for (int j=0; j<2; j++){

    m_hVector_mll.push_back( std::vector<TH1D*>() );

     for (int k=0;k<nloop;k++){

    char tag[300];

    if (!switchMultiple){
      sprintf(tag,"Complete");
    }
    else {
      sprintf(tag,"%i",k);
    }

    char name1[300];
    sprintf(name1,"Mll_%s",Folders.at(j).c_str());
    TH1D *histo_mll = new TH1D(name1,"M_{ll} Distribution; M_{ll} [GeV]; N events",40,0,500);
    m_hVector_mll[j].push_back(histo_mll);

  }

  }


  for(int i=0;i<NEVENTS;i++){

    double progress = 10.0*i/(1.0*NEVENTS);
    int l = TMath::FloorNint(progress); 

    if (l > decade){
      std::cout <<"\n<<<<<< STATUS >>>>>>" << std::endl; 
      std::cout<<10*l<<" % completed." << std::endl;
      std::cout <<"<<<<<<<<<<>>>>>>>>>>\n" << std::endl;
    }

    decade = l;          

    tr->GetEntry(i);


    //No Cuts
    //m_hVector_rjj[0].at(indexV)->Fill(eventdiffZ->GetRjjFromJets());


    // Trigger
    if (switchPreSel || (switchTrigger && eventdiffZ->GetHLTPath(optTrigger)) ){

      if (!switchPreSel || (switchPreSel && eventdiffZ->GetLeadingElectronPt() > 20 )) {

        if (GetElectronsN() >= 2) m_hVector_mll[1].at(indexV)->Fill(eventdiffZ->GetDiElectronMassPF() );

	/*if(eventdiffZ->GetNVertex() > 0 && eventdiffZ->GetNVertex()<= nVertex){

	  m_hVector_rjj[1].at(0)->Fill(eventdiffZ->GetRjjFromJets());

	}*/

      }
    }

  }

}

#if !defined(__CINT__) || defined(__MAKECINT__)
#include "FWCore/FWLite/interface/AutoLibraryLoader.h"
int main(int argc, char **argv){

  AutoLibraryLoader::enable();

  const char *s1="*";
  std::string filein_;
  std::string savehistofile_;
  int nVertex_;
  int optTrigger_;
  bool switchPreSel_;
  bool switchTrigger_;
  bool switchPUMultiple_;

  if (argc > 1 && strcmp(s1,argv[1]) != 0)  filein_ = argv[1];
  if (argc > 2 && strcmp(s1,argv[2]) != 0)  savehistofile_  = argv[2];
  if (argc > 3 && strcmp(s1,argv[3]) != 0)  switchTrigger_ = atoi(argv[3]);
  if (argc > 4 && strcmp(s1,argv[4]) != 0)  optTrigger_   = atoi(argv[4]);
  if (argc > 5 && strcmp(s1,argv[5]) != 0)  switchPreSel_ = atoi(argv[5]);
  if (argc > 6 && strcmp(s1,argv[6]) != 0)  nVertex_ = atoi(argv[6]);
  if (argc > 7 && strcmp(s1,argv[7]) != 0)  switchPUMultiple_ = atoi(argv[7]);

  DiffractiveZComp* diffZDijets = new DiffractiveZComp();   
  diffZDijets->Run(filein_, savehistofile_, switchTrigger_, optTrigger_, switchPreSel_, nVertex_, switchPUMultiple_);

  return 0;

}
#endif
