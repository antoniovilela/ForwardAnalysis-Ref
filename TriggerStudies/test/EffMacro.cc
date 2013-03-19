//-------------------------------------------------------------------------------------------------------->>
// UNIVERSIDADE DO ESTADO DO RIO DE JANEIRO - CMS/Brasil
//-------------------------------------------------------------------------------------------------------->>
// Project: Exclusive Dijets Analysis
//-------------------------------------------------------------------------------------------------------->>
//
// Twiki: https://twiki.cern.ch/twiki/bin/view/CMS/FwdPhysicsExclusiveDijetsAnalysis
//

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

#include "EffMacro.h"
#include "ForwardAnalysis/ForwardTTreeAnalysis/interface/ExclusiveDijetsEvent.h"
#include "ForwardAnalysis/ForwardTTreeAnalysis/interface/DiffractiveEvent.h"
#include "ForwardAnalysis/ForwardTTreeAnalysis/interface/EventInfoEvent.h"
#include "PhysicsTools/Utilities/interface/LumiReWeighting.h"

using namespace diffractiveAnalysis;
using namespace exclusiveDijetsAnalysis;
using namespace eventInfo;
using namespace reweight;

void EffMacro::LoadFile(std::string fileinput, std::string processinput){

  inf = NULL;
  tr  = NULL;
  inf = TFile::Open(fileinput.c_str(),"read");
  tr = (TTree*)inf->Get(processinput.c_str());
  eventdiff = new DiffractiveEvent();
  eventexcl = new ExclusiveDijetsEvent();
  eventinfo = new EventInfoEvent();
  diff = tr->GetBranch("DiffractiveAnalysis");
  excl = tr->GetBranch("ExclusiveDijetsAnalysis");
  info = tr->GetBranch("EventInfo");
  diff->SetAddress(&eventdiff);
  excl->SetAddress(&eventexcl);
  info->SetAddress(&eventinfo);

}

void EffMacro::Run(std::string filein_, std::string savehistofile_, std::string processname_, int optnVertex_, int optTrigger_, bool switchPreSel_, bool switchVertex_, bool switchTrigger_){

  filein = filein_;
  savehistofile = savehistofile_;
  processname = processname_;
  filein = filein_;
  optnVertex = optnVertex_;
  optTrigger = optTrigger_;
  switchPreSel = switchPreSel_;
  switchVertex = switchVertex_;
  switchTrigger = switchTrigger_;

  std::cout << "" << std::endl;
  std::cout << "Running..." << std::endl;
  std::cout << "" << std::endl;
  std::cout << "<< INPUTS >>" << std::endl;
  std::cout << " " << std::endl;
  std::cout << "Input file: " << filein << std::endl;
  std::cout << " " << std::cout;
  std::cout << "Output file: " << savehistofile << std::endl;
  std::cout << " " << std::cout; 
  std::cout << "# Vertex: " << optnVertex << std::endl;
  std::cout << "Trigger Option: " << optTrigger << std::endl;
  std::cout << " " << std::endl;
  std::cout << "--> TRUE = 1 FALSE = 0" << std::endl;
  std::cout << "Vertex Switch: " << switchVertex << std::endl;
  std::cout << "Trigger Switch: " << switchTrigger << std::endl;
  std::cout << "Pre-Selection Switch: " << switchPreSel << std::endl;
  std::cout << " " << std::endl;


  // Code Protection
  if (optnVertex == 0){

    std::cout << "---------------------------------------------------------------" << std::endl;
    std::cout << "Please, restart your setup. Respect the Condition # Vertex > 0)" << std::endl;
    std::cout << "---------------------------------------------------------------" << std::endl;
    return;

  }

  TFile check1(filein.c_str());

  if (check1.IsZombie()){

    std::cout << "----------------------------------------------" << std::endl;
    std::cout << " There is no PatTuple file or the"   << std::endl;
    std::cout << " path is not correct." << std::endl;
    std::cout << " Edit the source and recompile." << std::endl;
    std::cout << "----------------------------------------------" << std::endl;
    return;

  }
  //--------------------------------------------------------------------------------------------------------------------------

  LoadFile(filein,processname);

  // Root file with histograms
  TFile *outf = new TFile(savehistofile.c_str(),"RECREATE");

  // File with Number of Events
  TString outtxt = savehistofile;
  outtxt.ReplaceAll("root","txt");  
  std::ofstream outstring(outtxt); 

  int NEVENTS = tr->GetEntries();

  TH1::SetDefaultSumw2(true);
  TH2::SetDefaultSumw2(true);

  unsigned NEntries = tr->GetEntries();
  std::cout << "" << std::endl;
  std::cout<< "Reading Tree: "<< NEntries << " events"<<std::endl;
  std::cout << "" << std::endl;

  int decade = 0;

  double TotalE = 0.;
  double counterTrigger = 0.;
  double counterPreSel=0.;
  double counterVertex = 0.;
  double counterAllstep4_4 = 0.;
  double counterAllstep4_3 = 0.;
  double counterAllstep4_2 = 0.;
  double counterAllstep4_1 = 0.;

  std::vector <std::string> Folders;
  Folders.push_back("without_cuts");
  Folders.push_back("with_trigger");
  Folders.push_back("with_trigger_presel");
  Folders.push_back("with_trigger_presel_vertex");
  Folders.push_back("All_step4_4");
  Folders.push_back("All_step4_3");
  Folders.push_back("All_step4_2");
  Folders.push_back("All_step4_1");

  for (int j=0; j<8; j++)
  {

    char name1[300];
    sprintf(name1,"Events_%s",Folders.at(j).c_str());
    TH1D *histo_m_Evt_lumis = new TH1D(name1,"; Lumis; N events",100,0,2.0);
    m_hVector_Evt_lumis.push_back(histo_m_Evt_lumis);

    char name2[300];
    sprintf(name2,"Eff_%s",Folders.at(j).c_str());
    TH1D *histo_m_Eff_lumis = new TH1D(name2,"; Lumis; Efficiency",100,0,2.0);
    m_hVector_Eff_lumis.push_back(histo_m_Eff_lumis);

  }

  for(int i=0;i<NEVENTS;i++) {

    ++TotalE;

    double progress = 10.0*i/(1.0*NEVENTS);
    int l = TMath::FloorNint(progress); 
    if (l > decade){
      std::cout <<"\n<<<<<< STATUS >>>>>>" << std::endl; 
      std::cout<<10*l<<" % completed." << std::endl;
      std::cout <<"<<<<<<<<<<>>>>>>>>>>\n" << std::endl;
    }
    decade = l;          

    tr->GetEntry(i);

    if( i % 1000 == 0 ){
      std::cout << "\nEvent " << i << std::endl;
    }

    m_hVector_Evt_lumis.at(0)->Fill(eventinfo->GetInstLumiBunch());
    m_hVector_Eff_lumis.at(0)->Fill(eventinfo->GetInstLumiBunch());


    if (!switchTrigger || (switchTrigger && eventexcl->GetHLTPath(optTrigger))){

      ++counterTrigger;     
      m_hVector_Evt_lumis.at(1)->Fill(eventinfo->GetInstLumiBunch());
      m_hVector_Eff_lumis.at(1)->Fill(eventinfo->GetInstLumiBunch());


      if ( !switchPreSel || (switchPreSel && eventdiff->GetSumEnergyHFMinus() < 30 && eventdiff->GetSumEnergyHFPlus() < 30 )){

	++counterPreSel;
	m_hVector_Evt_lumis.at(2)->Fill(eventinfo->GetInstLumiBunch());
	m_hVector_Eff_lumis.at(2)->Fill(eventinfo->GetInstLumiBunch());


	if( !switchVertex || (switchVertex && eventexcl->GetNVertex()<= optnVertex )){

	  ++counterVertex;
	  m_hVector_Evt_lumis.at(3)->Fill(eventinfo->GetInstLumiBunch());
	  m_hVector_Eff_lumis.at(3)->Fill(eventinfo->GetInstLumiBunch());


	  // Eta max and Eta min cut
	  if ((eventdiff->GetEtaMinFromPFCands() > -4. && eventdiff->GetEtaMaxFromPFCands() < 4.) || (eventdiff->GetEtaMinFromPFCands() < -990 && eventdiff->GetEtaMaxFromPFCands() < -990) ){

	    ++counterAllstep4_4;
	    m_hVector_Evt_lumis.at(4)->Fill(eventinfo->GetInstLumiBunch());
	    m_hVector_Eff_lumis.at(4)->Fill(eventinfo->GetInstLumiBunch());
	  }

	  if ((eventdiff->GetEtaMinFromPFCands() > -3. && eventdiff->GetEtaMaxFromPFCands() < 3.) || (eventdiff->GetEtaMinFromPFCands() < -990 && eventdiff->GetEtaMaxFromPFCands() < -990)){

	    ++counterAllstep4_3;
	    m_hVector_Evt_lumis[5]->Fill(eventinfo->GetInstLumiBunch());
	    m_hVector_Eff_lumis[5]->Fill(eventinfo->GetInstLumiBunch());
	  }

	  if ((eventdiff->GetEtaMinFromPFCands() > -2. && eventdiff->GetEtaMaxFromPFCands() < 2.) || (eventdiff->GetEtaMinFromPFCands() < -990 && eventdiff->GetEtaMaxFromPFCands() < -990)){

	    ++counterAllstep4_2;
	    m_hVector_Evt_lumis.at(6)->Fill(eventinfo->GetInstLumiBunch());
	    m_hVector_Eff_lumis.at(6)->Fill(eventinfo->GetInstLumiBunch());
	  }

	  if ((eventdiff->GetEtaMinFromPFCands() > -1. && eventdiff->GetEtaMaxFromPFCands() < 1.) || (eventdiff->GetEtaMinFromPFCands() < -990 && eventdiff->GetEtaMaxFromPFCands() < -990)){

	    ++counterAllstep4_1;
	    m_hVector_Evt_lumis.at(7)->Fill(eventinfo->GetInstLumiBunch());
	    m_hVector_Eff_lumis.at(7)->Fill(eventinfo->GetInstLumiBunch());
	  }

	}  

      } 

    }   

  }


  //Scalling Plots
  for (int k=0; k<8; k++){
    m_hVector_Eff_lumis.at(k)->Scale(1./TotalE);
  }

  outstring << "" << std::endl;
  outstring << "<< INPUTS >>" << std::endl;
  outstring << " " << std::endl;
  outstring << "Input file: " << filein << std::endl;
  outstring << "Output file: " << savehistofile << std::endl;
  outstring << " " << std::endl;
  outstring << "# Vertex: " << optnVertex << std::endl;
  outstring << "Trigger Option: " << optTrigger << std::endl;
  outstring << " " << std::endl;
  outstring << "--> TRUE = 1 FALSE = 0" << std::endl;
  outstring << "Trigger Switch: " << switchTrigger << std::endl;
  outstring << "Vertex  Switch: " << switchVertex << std::endl;
  outstring << "Pre-Selection Switch: " << switchPreSel << std::endl;
  outstring << "" << std::endl;
  outstring << "<< EVENT INFO >> " << std::endl;
  outstring << " " << std::endl;
  outstring << "Total # of Events without Weight: " << TotalE << std::endl;
  outstring << " " << std::endl;
  outstring << "Trigger: " << counterTrigger << std::endl;
  outstring << "Trigger + Pre Sel.: " << counterPreSel << std::endl;
  outstring << "Trigger + Pre Sel. + Vertex: " << counterVertex << std::endl;
  outstring << "STEP4_4 (CMS Acceptance): " << counterAllstep4_4 << std::endl;
  outstring << "STEP4_3 (CMS Acceptance): " << counterAllstep4_3 << std::endl;
  outstring << "STEP4_2 (CMS Acceptance): " << counterAllstep4_2 << std::endl;
  outstring << "STEP4_1 (CMS Acceptance): " << counterAllstep4_1 << std::endl;
  outstring << " " << std::endl;
  outstring << "<< LEGEND >> " << std::endl;
  outstring << "STEP4_X: Trigger + Pre Sel. + Vertex + Eta_max < X and Eta_min > X." << std::endl;
  outstring << " " << std::endl;

  outf->Write();
  outf->Close();
  outstring.close();

}

#if !defined(__CINT__) || defined(__MAKECINT__)
#include "FWCore/FWLite/interface/AutoLibraryLoader.h"
int main(int argc, char **argv)
{
  AutoLibraryLoader::enable();

  const char *s1="*";
  std::string filein_;
  std::string savehistofile_;
  std::string processname_;
  int optnVertex_;
  int optTrigger_;
  bool switchPreSel_;
  bool switchVertex_;
  bool switchTrigger_;

  if (argc > 1 && strcmp(s1,argv[1]) != 0)  filein_ = argv[1];
  if (argc > 2 && strcmp(s1,argv[2]) != 0)  savehistofile_  = argv[2];
  if (argc > 3 && strcmp(s1,argv[3]) != 0)  processname_  = argv[3];
  if (argc > 4 && strcmp(s1,argv[4]) != 0)  optnVertex_ = atoi(argv[4]);
  if (argc > 5 && strcmp(s1,argv[5]) != 0)  optTrigger_   = atoi(argv[5]);
  if (argc > 6 && strcmp(s1,argv[6]) != 0)  switchPreSel_   = atoi(argv[6]);
  if (argc > 7 && strcmp(s1,argv[7]) != 0)  switchVertex_   = atoi(argv[7]);
  if (argc > 8 && strcmp(s1,argv[8]) != 0)  switchTrigger_   = atoi(argv[8]);


  EffMacro* exclDijets = new EffMacro();   
  exclDijets->Run(filein_, savehistofile_, processname_, optnVertex_, optTrigger_, switchPreSel_, switchVertex_, switchTrigger_);

  return 0;
}
#endif
