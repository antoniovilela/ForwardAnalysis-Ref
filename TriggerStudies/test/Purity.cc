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

#include "Purity.h"
#include "ForwardAnalysis/ForwardTTreeAnalysis/interface/ExclusiveDijetsEvent.h"
#include "ForwardAnalysis/ForwardTTreeAnalysis/interface/DiffractiveEvent.h"
#include "ForwardAnalysis/ForwardTTreeAnalysis/interface/EventInfoEvent.h"
#include "PhysicsTools/Utilities/interface/LumiReWeighting.h"

using namespace diffractiveAnalysis;
using namespace exclusiveDijetsAnalysis;
using namespace eventInfo;
using namespace reweight;

void Purity::LoadFile(std::string fileinput, std::string processinput){

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

void Purity::Run(std::string filein_, std::string savehistofile_, std::string processname_, int optnVertex_, int optTrigger_, bool switchPreSel_, bool switchVertex_, bool switchTrigger_){

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

  double deltaphi_ = 0.;
  double TotalE = 0.;
  double counterTrigger = 0.;
  double counterPreSel=0.;
  double counterVertex = 0.;
  double counterdijets = 0.;
  double counterdeltaphi = 0.;
  double counterAllstep4_4 = 0.;
  double counterAllstep4_3 = 0.;
  double counterAllstep4_2 = 0.;
  double counterAllstep4_1 = 0.;

  std::vector <std::string> FoldersLum;
  FoldersLum.push_back("without_cuts");
  FoldersLum.push_back("with_trigger");
  FoldersLum.push_back("with_trigger_presel");
  FoldersLum.push_back("with_trigger_presel_vertex");
  FoldersLum.push_back("with_trigger_dijets");
  FoldersLum.push_back("with_trigger_deltaphi");
  FoldersLum.push_back("All_step4_4");
  FoldersLum.push_back("All_step4_3");
  FoldersLum.push_back("All_step4_2");
  FoldersLum.push_back("All_step4_1");

  std::vector <std::string> Folders;
  Folders.push_back("without_cuts");
  Folders.push_back("with_trigger");
  Folders.push_back("with_trigger_presel");
  Folders.push_back("with_trigger_presel_vertex");
  Folders.push_back("with_trigger_dijets");
  Folders.push_back("with_trigger_deltaphi");

  for (std::vector<std::string>::size_type j=0; j<FoldersLum.size(); j++)
  {
    char name1[300];
    sprintf(name1,"Events_Lumi_%s",FoldersLum.at(j).c_str());
    TH1D *histo_m_Evt_lumis = new TH1D(name1,"; Lumis; N events",100,0,2.0);
    m_hVector_Evt_lumis.push_back(histo_m_Evt_lumis);

    char name2[300];
    sprintf(name2,"Eff_Lumi_%s",FoldersLum.at(j).c_str());
    TH1D *histo_m_Eff_lumis = new TH1D(name2,"; Lumis; Efficiency",100,0,2.0);
    m_hVector_Eff_lumis.push_back(histo_m_Eff_lumis);
  }


  for (std::vector<std::string>::size_type j=0; j<Folders.size(); j++)
  {

    char name3[300];
    sprintf(name3,"Events_preplus_%s",Folders.at(j).c_str());
    TH1D *histo_m_Evt_preplus = new TH1D(name3,"; #sum E_{HF^{+}} [GeV]; N events",100,0,2000);
    m_hVector_Evt_preplus.push_back(histo_m_Evt_preplus);

    char name4[300];
    sprintf(name4,"Eff_preplus_%s",Folders.at(j).c_str());
    TH1D *histo_m_Eff_preplus = new TH1D(name4,"; #sum E_{HF^{+}} [GeV]; Efficiency",100,0,2000);
    m_hVector_Eff_preplus.push_back(histo_m_Eff_preplus);

    char name5[300];
    sprintf(name5,"Events_preminus_%s",Folders.at(j).c_str());
    TH1D *histo_m_Evt_preminus = new TH1D(name5,"; #sum E_{HF^{-}} [GeV]; N events",100,0,2000);
    m_hVector_Evt_preminus.push_back(histo_m_Evt_preminus);

    char name6[300];
    sprintf(name6,"Eff_preminus_%s",Folders.at(j).c_str());
    TH1D *histo_m_Eff_preminus = new TH1D(name6,"; #sum E_{HF^{-}} [GeV]; Efficiency",100,0,2000);
    m_hVector_Eff_preminus.push_back(histo_m_Eff_preminus);

    char name7[300];
    sprintf(name7,"Events_vertex_%s",Folders.at(j).c_str());
    TH1D *histo_m_Evt_vertex = new TH1D(name7,"; # Vertex; N events",25,0,25);
    m_hVector_Evt_vertex.push_back(histo_m_Evt_vertex);

    char name8[300];
    sprintf(name8,"Eff_vertex_%s",Folders.at(j).c_str());
    TH1D *histo_m_Eff_vertex = new TH1D(name8,"; # Vertex; Efficiency",25,0,25);
    m_hVector_Eff_vertex.push_back(histo_m_Eff_vertex);

    char name9[300];
    sprintf(name9,"Events_jet1pt_%s",Folders.at(j).c_str());
    TH1D *histo_m_Evt_jet1pt = new TH1D(name9,"; P_{T} [GeV.c^{-1}]; N events",100,0,2000);
    m_hVector_Evt_jet1pt.push_back(histo_m_Evt_jet1pt);

    char name10[300];
    sprintf(name10,"Eff_jet1pt_%s",Folders.at(j).c_str());
    TH1D *histo_m_Eff_jet1pt = new TH1D(name10,"; P_{T} [GeV.c^{-1}]; Efficiency",100,0,2000);
    m_hVector_Eff_jet1pt.push_back(histo_m_Eff_jet1pt);

    char name11[300];
    sprintf(name11,"Events_jet1phi_%s",Folders.at(j).c_str());
    TH1D *histo_m_Evt_jet1phi = new TH1D(name11,"; #phi [rad]; N events",50,-3.3,3.3);
    m_hVector_Evt_jet1phi.push_back(histo_m_Evt_jet1phi);

    char name12[300];
    sprintf(name12,"Eff_jet1phi_%s",Folders.at(j).c_str());
    TH1D *histo_m_Eff_jet1phi = new TH1D(name12,"; #phi [rad]; Efficiency",50,-3.3,3.3);
    m_hVector_Eff_jet1phi.push_back(histo_m_Eff_jet1phi);

    char name13[300];
    sprintf(name13,"Events_jet1eta_%s",Folders.at(j).c_str());
    TH1D *histo_m_Evt_jet1eta = new TH1D(name13,"; #eta; N events",50,-5.2,5.2);
    m_hVector_Evt_jet1eta.push_back(histo_m_Evt_jet1eta);

    char name14[300];
    sprintf(name14,"Eff_jet1eta_%s",Folders.at(j).c_str());
    TH1D *histo_m_Eff_jet1eta = new TH1D(name14,"; #eta; Efficiency",50,-5.2,5.2);
    m_hVector_Eff_jet1eta.push_back(histo_m_Eff_jet1eta);

    char name15[300];
    sprintf(name15,"Events_jet2pt_%s",Folders.at(j).c_str());
    TH1D *histo_m_Evt_jet2pt = new TH1D(name15,"; P_{T} [GeV.c^{-1}]; N events",100,0,2000);
    m_hVector_Evt_jet2pt.push_back(histo_m_Evt_jet2pt);

    char name16[300];
    sprintf(name16,"Eff_jet2pt_%s",Folders.at(j).c_str());
    TH1D *histo_m_Eff_jet2pt = new TH1D(name16,"; P_{T} [GeV.c^{-1}]; Efficiency",100,0,2000);
    m_hVector_Eff_jet2pt.push_back(histo_m_Eff_jet2pt);

    char name17[300];
    sprintf(name17,"Events_jet2phi_%s",Folders.at(j).c_str());
    TH1D *histo_m_Evt_jet2phi = new TH1D(name17,"; #phi [rad]; N events",50,-3.3,3.3);
    m_hVector_Evt_jet2phi.push_back(histo_m_Evt_jet2phi);

    char name18[300];
    sprintf(name18,"Eff_jet2phi_%s",Folders.at(j).c_str());
    TH1D *histo_m_Eff_jet2phi = new TH1D(name18,"; #phi [rad]; Efficiency",50,-3.3,3.3);
    m_hVector_Eff_jet2phi.push_back(histo_m_Eff_jet2phi);

    char name19[300];
    sprintf(name19,"Events_jet2eta_%s",Folders.at(j).c_str());
    TH1D *histo_m_Evt_jet2eta = new TH1D(name19,"; #eta; N events",50,-5.2,5.2);
    m_hVector_Evt_jet2eta.push_back(histo_m_Evt_jet2eta);

    char name20[300];
    sprintf(name20,"Eff_jet2eta_%s",Folders.at(j).c_str());
    TH1D *histo_m_Eff_jet2eta = new TH1D(name20,"; #eta; Efficiency",50,-5.2,5.2);
    m_hVector_Eff_jet2eta.push_back(histo_m_Eff_jet2eta);

    char name21[300];
    sprintf(name21,"Events_deltaphi_%s",Folders.at(j).c_str());
    TH1D *histo_m_Evt_deltaphi = new TH1D(name21,"; #Delta#phi_{jj}; N events",20,0.0,3.2);
    m_hVector_Evt_deltaphi.push_back(histo_m_Evt_deltaphi);

    char name22[300];
    sprintf(name22,"Eff_deltaphi_%s",Folders.at(j).c_str());
    TH1D *histo_m_Eff_deltaphi = new TH1D(name22,"; #Delta#phi_{jj}; Efficiency",20,0.0,3.2);
    m_hVector_Eff_deltaphi.push_back(histo_m_Eff_deltaphi);

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
    deltaphi_ = fabs(eventexcl->GetLeadingJetPhi()-eventexcl->GetSecondJetPhi());

    m_hVector_Evt_lumis.at(0)->Fill(eventinfo->GetInstLumiBunch());
    m_hVector_Eff_lumis.at(0)->Fill(eventinfo->GetInstLumiBunch());
    m_hVector_Evt_preplus.at(0)->Fill(eventdiff->GetSumEnergyHFPlus());
    m_hVector_Eff_preplus.at(0)->Fill(eventdiff->GetSumEnergyHFPlus());
    m_hVector_Evt_preminus.at(0)->Fill(eventdiff->GetSumEnergyHFMinus());
    m_hVector_Eff_preminus.at(0)->Fill(eventdiff->GetSumEnergyHFMinus());
    m_hVector_Evt_vertex.at(0)->Fill(eventexcl->GetNVertex());
    m_hVector_Eff_vertex.at(0)->Fill(eventexcl->GetNVertex());
    m_hVector_Evt_jet1pt.at(0)->Fill(eventexcl->GetLeadingJetPt());
    m_hVector_Eff_jet1pt.at(0)->Fill(eventexcl->GetLeadingJetPt());
    m_hVector_Evt_jet1phi.at(0)->Fill(eventexcl->GetLeadingJetPhi());
    m_hVector_Eff_jet1phi.at(0)->Fill(eventexcl->GetLeadingJetPhi());
    m_hVector_Evt_jet1eta.at(0)->Fill(eventexcl->GetLeadingJetEta());
    m_hVector_Eff_jet1eta.at(0)->Fill(eventexcl->GetLeadingJetEta());
    m_hVector_Evt_jet2pt.at(0)->Fill(eventexcl->GetSecondJetPt());
    m_hVector_Eff_jet2pt.at(0)->Fill(eventexcl->GetSecondJetPt());
    m_hVector_Evt_jet2phi.at(0)->Fill(eventexcl->GetSecondJetPhi());
    m_hVector_Eff_jet2phi.at(0)->Fill(eventexcl->GetSecondJetPhi());
    m_hVector_Evt_jet2eta.at(0)->Fill(eventexcl->GetSecondJetEta());
    m_hVector_Eff_jet2eta.at(0)->Fill(eventexcl->GetSecondJetEta());
    m_hVector_Evt_deltaphi.at(0)->Fill(deltaphi_);
    m_hVector_Eff_deltaphi.at(0)->Fill(deltaphi_);

    if (!switchTrigger || (switchTrigger && eventexcl->GetHLTPath(optTrigger))){

      ++counterTrigger;     
      m_hVector_Evt_lumis.at(1)->Fill(eventinfo->GetInstLumiBunch());
      m_hVector_Eff_lumis.at(1)->Fill(eventinfo->GetInstLumiBunch());
      m_hVector_Evt_preplus.at(1)->Fill(eventdiff->GetSumEnergyHFPlus());
      m_hVector_Eff_preplus.at(1)->Fill(eventdiff->GetSumEnergyHFPlus());
      m_hVector_Evt_preminus.at(1)->Fill(eventdiff->GetSumEnergyHFMinus());
      m_hVector_Eff_preminus.at(1)->Fill(eventdiff->GetSumEnergyHFMinus());
      m_hVector_Evt_vertex.at(1)->Fill(eventexcl->GetNVertex());
      m_hVector_Eff_vertex.at(1)->Fill(eventexcl->GetNVertex());
      m_hVector_Evt_jet1pt.at(1)->Fill(eventexcl->GetLeadingJetPt());
      m_hVector_Eff_jet1pt.at(1)->Fill(eventexcl->GetLeadingJetPt());
      m_hVector_Evt_jet1phi.at(1)->Fill(eventexcl->GetLeadingJetPhi());
      m_hVector_Eff_jet1phi.at(1)->Fill(eventexcl->GetLeadingJetPhi());
      m_hVector_Evt_jet1eta.at(1)->Fill(eventexcl->GetLeadingJetEta());
      m_hVector_Eff_jet1eta.at(1)->Fill(eventexcl->GetLeadingJetEta());
      m_hVector_Evt_jet2pt.at(1)->Fill(eventexcl->GetSecondJetPt());
      m_hVector_Eff_jet2pt.at(1)->Fill(eventexcl->GetSecondJetPt());
      m_hVector_Evt_jet2phi.at(1)->Fill(eventexcl->GetSecondJetPhi());
      m_hVector_Eff_jet2phi.at(1)->Fill(eventexcl->GetSecondJetPhi());
      m_hVector_Evt_jet2eta.at(1)->Fill(eventexcl->GetSecondJetEta());
      m_hVector_Eff_jet2eta.at(1)->Fill(eventexcl->GetSecondJetEta());
      m_hVector_Evt_deltaphi.at(1)->Fill(deltaphi_);
      m_hVector_Eff_deltaphi.at(1)->Fill(deltaphi_);

      if ( !switchPreSel || (switchPreSel && eventdiff->GetSumEnergyHFMinus() < 30 && eventdiff->GetSumEnergyHFPlus() < 30 )){

	++counterPreSel;
	m_hVector_Evt_lumis.at(2)->Fill(eventinfo->GetInstLumiBunch());
	m_hVector_Eff_lumis.at(2)->Fill(eventinfo->GetInstLumiBunch());
	m_hVector_Evt_preplus.at(2)->Fill(eventdiff->GetSumEnergyHFPlus());
	m_hVector_Eff_preplus.at(2)->Fill(eventdiff->GetSumEnergyHFPlus());
	m_hVector_Evt_preminus.at(2)->Fill(eventdiff->GetSumEnergyHFMinus());
	m_hVector_Eff_preminus.at(2)->Fill(eventdiff->GetSumEnergyHFMinus());
	m_hVector_Evt_vertex.at(2)->Fill(eventexcl->GetNVertex());
	m_hVector_Eff_vertex.at(2)->Fill(eventexcl->GetNVertex());
	m_hVector_Evt_jet1pt.at(2)->Fill(eventexcl->GetLeadingJetPt());
	m_hVector_Eff_jet1pt.at(2)->Fill(eventexcl->GetLeadingJetPt());
	m_hVector_Evt_jet1phi.at(2)->Fill(eventexcl->GetLeadingJetPhi());
	m_hVector_Eff_jet1phi.at(2)->Fill(eventexcl->GetLeadingJetPhi());
	m_hVector_Evt_jet1eta.at(2)->Fill(eventexcl->GetLeadingJetEta());
	m_hVector_Eff_jet1eta.at(2)->Fill(eventexcl->GetLeadingJetEta());
	m_hVector_Evt_jet2pt.at(2)->Fill(eventexcl->GetSecondJetPt());
	m_hVector_Eff_jet2pt.at(2)->Fill(eventexcl->GetSecondJetPt());
	m_hVector_Evt_jet2phi.at(2)->Fill(eventexcl->GetSecondJetPhi());
	m_hVector_Eff_jet2phi.at(2)->Fill(eventexcl->GetSecondJetPhi());
	m_hVector_Evt_jet2eta.at(2)->Fill(eventexcl->GetSecondJetEta());
	m_hVector_Eff_jet2eta.at(2)->Fill(eventexcl->GetSecondJetEta());
	m_hVector_Evt_deltaphi.at(2)->Fill(deltaphi_);
	m_hVector_Eff_deltaphi.at(2)->Fill(deltaphi_);

	if( !switchVertex || (switchVertex && eventexcl->GetNVertex()<= optnVertex )){

	  ++counterVertex;
	  m_hVector_Evt_lumis.at(3)->Fill(eventinfo->GetInstLumiBunch());
	  m_hVector_Eff_lumis.at(3)->Fill(eventinfo->GetInstLumiBunch());
	  m_hVector_Evt_preplus.at(3)->Fill(eventdiff->GetSumEnergyHFPlus());
	  m_hVector_Eff_preplus.at(3)->Fill(eventdiff->GetSumEnergyHFPlus());
	  m_hVector_Evt_preminus.at(3)->Fill(eventdiff->GetSumEnergyHFMinus());
	  m_hVector_Eff_preminus.at(3)->Fill(eventdiff->GetSumEnergyHFMinus());
	  m_hVector_Evt_vertex.at(3)->Fill(eventexcl->GetNVertex());
	  m_hVector_Eff_vertex.at(3)->Fill(eventexcl->GetNVertex());
	  m_hVector_Evt_jet1pt.at(3)->Fill(eventexcl->GetLeadingJetPt());
	  m_hVector_Eff_jet1pt.at(3)->Fill(eventexcl->GetLeadingJetPt());
	  m_hVector_Evt_jet1phi.at(3)->Fill(eventexcl->GetLeadingJetPhi());
	  m_hVector_Eff_jet1phi.at(3)->Fill(eventexcl->GetLeadingJetPhi());
	  m_hVector_Evt_jet1eta.at(3)->Fill(eventexcl->GetLeadingJetEta());
	  m_hVector_Eff_jet1eta.at(3)->Fill(eventexcl->GetLeadingJetEta());
	  m_hVector_Evt_jet2pt.at(3)->Fill(eventexcl->GetSecondJetPt());
	  m_hVector_Eff_jet2pt.at(3)->Fill(eventexcl->GetSecondJetPt());
	  m_hVector_Evt_jet2phi.at(3)->Fill(eventexcl->GetSecondJetPhi());
	  m_hVector_Eff_jet2phi.at(3)->Fill(eventexcl->GetSecondJetPhi());
	  m_hVector_Evt_jet2eta.at(3)->Fill(eventexcl->GetSecondJetEta());
	  m_hVector_Eff_jet2eta.at(3)->Fill(eventexcl->GetSecondJetEta());
	  m_hVector_Evt_deltaphi.at(3)->Fill(deltaphi_);
	  m_hVector_Eff_deltaphi.at(3)->Fill(deltaphi_);

	  if(eventexcl->GetSecondJetPt() > 10. ){
	    ++counterdijets;
	    m_hVector_Evt_lumis.at(4)->Fill(eventinfo->GetInstLumiBunch());
	    m_hVector_Eff_lumis.at(4)->Fill(eventinfo->GetInstLumiBunch());
	    m_hVector_Evt_preplus.at(4)->Fill(eventdiff->GetSumEnergyHFPlus());
	    m_hVector_Eff_preplus.at(4)->Fill(eventdiff->GetSumEnergyHFPlus());
	    m_hVector_Evt_preminus.at(4)->Fill(eventdiff->GetSumEnergyHFMinus());
	    m_hVector_Eff_preminus.at(4)->Fill(eventdiff->GetSumEnergyHFMinus());
	    m_hVector_Evt_vertex.at(4)->Fill(eventexcl->GetNVertex());
	    m_hVector_Eff_vertex.at(4)->Fill(eventexcl->GetNVertex());
	    m_hVector_Evt_jet1pt.at(4)->Fill(eventexcl->GetLeadingJetPt());
	    m_hVector_Eff_jet1pt.at(4)->Fill(eventexcl->GetLeadingJetPt());
	    m_hVector_Evt_jet1phi.at(4)->Fill(eventexcl->GetLeadingJetPhi());
	    m_hVector_Eff_jet1phi.at(4)->Fill(eventexcl->GetLeadingJetPhi());
	    m_hVector_Evt_jet1eta.at(4)->Fill(eventexcl->GetLeadingJetEta());
	    m_hVector_Eff_jet1eta.at(4)->Fill(eventexcl->GetLeadingJetEta());
	    m_hVector_Evt_jet2pt.at(4)->Fill(eventexcl->GetSecondJetPt());
	    m_hVector_Eff_jet2pt.at(4)->Fill(eventexcl->GetSecondJetPt());
	    m_hVector_Evt_jet2phi.at(4)->Fill(eventexcl->GetSecondJetPhi());
	    m_hVector_Eff_jet2phi.at(4)->Fill(eventexcl->GetSecondJetPhi());
	    m_hVector_Evt_jet2eta.at(4)->Fill(eventexcl->GetSecondJetEta());
	    m_hVector_Eff_jet2eta.at(4)->Fill(eventexcl->GetSecondJetEta());
	    m_hVector_Evt_deltaphi.at(4)->Fill(deltaphi_);
	    m_hVector_Eff_deltaphi.at(4)->Fill(deltaphi_);

	    if(deltaphi_>M_PI) deltaphi_=2.0*M_PI-deltaphi_;
	    if(deltaphi_>0.5*M_PI) {

	      ++counterdeltaphi;
	      m_hVector_Evt_lumis.at(5)->Fill(eventinfo->GetInstLumiBunch());
	      m_hVector_Eff_lumis.at(5)->Fill(eventinfo->GetInstLumiBunch());
	      m_hVector_Evt_preplus.at(5)->Fill(eventdiff->GetSumEnergyHFPlus());
	      m_hVector_Eff_preplus.at(5)->Fill(eventdiff->GetSumEnergyHFPlus());
	      m_hVector_Evt_preminus.at(5)->Fill(eventdiff->GetSumEnergyHFMinus());
	      m_hVector_Eff_preminus.at(5)->Fill(eventdiff->GetSumEnergyHFMinus());
	      m_hVector_Evt_vertex.at(5)->Fill(eventexcl->GetNVertex());
	      m_hVector_Eff_vertex.at(5)->Fill(eventexcl->GetNVertex());
	      m_hVector_Evt_jet1pt.at(5)->Fill(eventexcl->GetLeadingJetPt());
	      m_hVector_Eff_jet1pt.at(5)->Fill(eventexcl->GetLeadingJetPt());
	      m_hVector_Evt_jet1phi.at(5)->Fill(eventexcl->GetLeadingJetPhi());
	      m_hVector_Eff_jet1phi.at(5)->Fill(eventexcl->GetLeadingJetPhi());
	      m_hVector_Evt_jet1eta.at(5)->Fill(eventexcl->GetLeadingJetEta());
	      m_hVector_Eff_jet1eta.at(5)->Fill(eventexcl->GetLeadingJetEta());
	      m_hVector_Evt_jet2pt.at(5)->Fill(eventexcl->GetSecondJetPt());
	      m_hVector_Eff_jet2pt.at(5)->Fill(eventexcl->GetSecondJetPt());
	      m_hVector_Evt_jet2phi.at(5)->Fill(eventexcl->GetSecondJetPhi());
	      m_hVector_Eff_jet2phi.at(5)->Fill(eventexcl->GetSecondJetPhi());
	      m_hVector_Evt_jet2eta.at(5)->Fill(eventexcl->GetSecondJetEta());
	      m_hVector_Eff_jet2eta.at(5)->Fill(eventexcl->GetSecondJetEta());
	      m_hVector_Evt_deltaphi.at(5)->Fill(deltaphi_);
	      m_hVector_Eff_deltaphi.at(5)->Fill(deltaphi_);



	      // Eta max and Eta min cut
	      if ((eventdiff->GetEtaMinFromPFCands() > -4. && eventdiff->GetEtaMaxFromPFCands() < 4.) || (eventdiff->GetEtaMinFromPFCands() < -990 && eventdiff->GetEtaMaxFromPFCands() < -990) ){

		++counterAllstep4_4;
		m_hVector_Evt_lumis.at(6)->Fill(eventinfo->GetInstLumiBunch());
		m_hVector_Eff_lumis.at(6)->Fill(eventinfo->GetInstLumiBunch());
	      }

	      if ((eventdiff->GetEtaMinFromPFCands() > -3. && eventdiff->GetEtaMaxFromPFCands() < 3.) || (eventdiff->GetEtaMinFromPFCands() < -990 && eventdiff->GetEtaMaxFromPFCands() < -990)){

		++counterAllstep4_3;
		m_hVector_Evt_lumis.at(7)->Fill(eventinfo->GetInstLumiBunch());
		m_hVector_Eff_lumis.at(7)->Fill(eventinfo->GetInstLumiBunch());
	      }

	      if ((eventdiff->GetEtaMinFromPFCands() > -2. && eventdiff->GetEtaMaxFromPFCands() < 2.) || (eventdiff->GetEtaMinFromPFCands() < -990 && eventdiff->GetEtaMaxFromPFCands() < -990)){

		++counterAllstep4_2;
		m_hVector_Evt_lumis.at(8)->Fill(eventinfo->GetInstLumiBunch());
		m_hVector_Eff_lumis.at(8)->Fill(eventinfo->GetInstLumiBunch());
	      }

	      if ((eventdiff->GetEtaMinFromPFCands() > -1. && eventdiff->GetEtaMaxFromPFCands() < 1.) || (eventdiff->GetEtaMinFromPFCands() < -990 && eventdiff->GetEtaMaxFromPFCands() < -990)){

		++counterAllstep4_1;
		m_hVector_Evt_lumis.at(9)->Fill(eventinfo->GetInstLumiBunch());
		m_hVector_Eff_lumis.at(9)->Fill(eventinfo->GetInstLumiBunch());
	      }

	    }  

	  }

	}

      } 

    }   

  }


  //Scalling Plots
  for (std::vector<std::string>::size_type k=0; k<FoldersLum.size(); k++){
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
  outstring << "Trigger + Pre Sel. + Vertex + dijets: " << counterdijets << std::endl;
  outstring << "Trigger + Pre Sel. + Vertex + dijets + deltaphi : " << counterdeltaphi << std::endl;
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


  Purity* purity = new Purity();   
  purity->Run(filein_, savehistofile_, processname_, optnVertex_, optTrigger_, switchPreSel_, switchVertex_, switchTrigger_);

  return 0;
}
#endif
