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
#include "ForwardAnalysis/ForwardTTreeAnalysis/interface/DiffractiveZEvent.h"
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
  diffZ = tr->GetBranch("DiffractiveZAnalysis");
  info = tr->GetBranch("EventInfo");
  diff->SetAddress(&eventdiff);
  diffZ->SetAddress(&eventdiffZ);
  info->SetAddress(&eventinfo);

}

void DiffractiveZComp::Run(std::string filein_, std::string ttreename_, std::string savehistofile_, bool switchTrigger_, int optTrigger_, bool switchPreSel_, double lepton1pt_, double lepton2pt_, int nVertex_, bool switchPUMultiple_, bool switchmcweight_, float mcweight_){

  filein = filein_;
  ttreename = ttreename_;
  savehistofile = savehistofile_;
  switchTrigger = switchTrigger_;
  optTrigger = optTrigger_;
  switchPreSel = switchPreSel_;
  lepton1pt = lepton1pt_;
  lepton2pt = lepton2pt_;
  nVertex = nVertex_;
  switchPUMultiple = switchPUMultiple_;
  switchmcweight = switchmcweight_;
  mcweight = mcweight_;

  std::cout << "" << std::endl;
  std::cout << "Running..." << std::endl;
  std::cout << "" << std::endl;
  std::cout << "<< INPUTS >>" << std::endl;
  std::cout << " " << std::endl;
  std::cout << "Input file: " << filein << std::endl;
  std::cout << "Output file: " << savehistofile << std::endl;
  std::cout << "TTree Name: " << ttreename << std::endl;
  std::cout << "MC Weight: " << mcweight << std::endl;
  std::cout << " " << std::cout;
  std::cout << "Leading Electron pT [GeV]: " << lepton1pt << std::endl;
  std::cout << "Second Electron pT [GeV]: " << lepton2pt << std::endl; 
  std::cout << "# Vertex: " << nVertex << std::endl;
  std::cout << "Trigger Option: " << optTrigger << std::endl;
  std::cout << " " << std::endl;
  std::cout << "--> TRUE = 1 FALSE = 0" << std::endl;
  std::cout << "Trigger Switch: " << switchTrigger << std::endl;
  std::cout << "Pre Selection Switch: " << switchPreSel << std::endl;
  std::cout << "PU Multiple Histograms: " << switchPUMultiple << std::endl;
  std::cout << "" << std::endl;

  if (nVertex == 0){

    std::cout << "---------------------------------------------------------------" << std::endl;
    std::cout << "Please, restart your setup. Respect the Condition # Vertex > 0)" << std::endl;
    std::cout << "---------------------------------------------------------------" << std::endl;
    return;

  }

  LoadFile(filein,ttreename);

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

  // Reco Variables
  double aSumE = -999.;
  double deltaphielectrons = -999.;
  double deltaphimuons = -999.;

  // Each Cut Name Histogram tag.
  std::vector <std::string> Folders;
  Folders.push_back("without_cuts");
  Folders.push_back("step1");
  Folders.push_back("step2");
  Folders.push_back("step3");
  Folders.push_back("step4");
  Folders.push_back("step5");
  Folders.push_back("step6");
  Folders.push_back("step7");

  int nloop;
  int indexV;

  // nloop is number of PU events for each MC PU Histogram name. 
  if (switchPUMultiple){
    nloop = 21;
  }

  else {
    nloop = 1;
  }

  // j is the number of cuts or Histogram tag name.
  for (int j=0; j<8; j++){

    m_hVector_DiElectron.push_back( std::vector<TH1D*>() );
    m_hVector_LeadingElectronPt.push_back( std::vector<TH1D*>() );
    m_hVector_LeadingElectronEta.push_back( std::vector<TH1D*>() );
    m_hVector_LeadingElectronPhi.push_back( std::vector<TH1D*>() );
    m_hVector_LeadingElectronCharge.push_back( std::vector<TH1D*>() );
    m_hVector_SecondElectronPt.push_back( std::vector<TH1D*>() );
    m_hVector_SecondElectronEta.push_back( std::vector<TH1D*>() );
    m_hVector_SecondElectronPhi.push_back( std::vector<TH1D*>() );
    m_hVector_SecondElectronCharge.push_back( std::vector<TH1D*>() );
    m_hVector_ElectronsN.push_back( std::vector<TH1D*>() );

    m_hVector_DiMuon.push_back( std::vector<TH1D*>() );
    m_hVector_LeadingMuonPt.push_back( std::vector<TH1D*>() );
    m_hVector_LeadingMuonEta.push_back( std::vector<TH1D*>() );
    m_hVector_LeadingMuonPhi.push_back( std::vector<TH1D*>() );
    m_hVector_LeadingMuonCharge.push_back( std::vector<TH1D*>() );
    m_hVector_SecondMuonPt.push_back( std::vector<TH1D*>() );
    m_hVector_SecondMuonEta.push_back( std::vector<TH1D*>() );
    m_hVector_SecondMuonPhi.push_back( std::vector<TH1D*>() );
    m_hVector_SecondMuonCharge.push_back( std::vector<TH1D*>() );
    m_hVector_MuonsN.push_back( std::vector<TH1D*>() );

    m_hVector_sumEHFplus.push_back( std::vector<TH1D*>() );
    m_hVector_sumEHFminus.push_back( std::vector<TH1D*>() );
    m_hVector_sumEHEplus.push_back( std::vector<TH1D*>() );
    m_hVector_sumEHEminus.push_back( std::vector<TH1D*>() );

    m_hVector_lumi.push_back( std::vector<TH1D*>() );
    m_hVector_asumE.push_back( std::vector<TH1D*>() );
    m_hVector_multhf.push_back( std::vector<TH2F*>() );
    m_hVector_etcalos.push_back( std::vector<TH2F*>() );
    m_hVector_tracks.push_back( std::vector<TH1D*>() );
    m_hVector_pfetamax.push_back( std::vector<TH1D*>() );
    m_hVector_pfetamin.push_back( std::vector<TH1D*>() );
    m_hVector_asumE.push_back( std::vector<TH1D*>() );
    m_hVector_vertex.push_back( std::vector<TH1D*>() );
    m_hVector_deltaphielectrons.push_back( std::vector<TH1D*>() );
    m_hVector_deltaphimuons.push_back( std::vector<TH1D*>() );
    m_hVector_vertexvslumi.push_back( std::vector<TH2F*>() );

    for (int k=0;k<nloop;k++){

      char tag[300];

      if (!switchPUMultiple){
	sprintf(tag,"Complete");
      }
      else {
	sprintf(tag,"PU_%i",k);
      }

      char name1[300];
      sprintf(name1,"DiElectron_%s_%s",tag,Folders.at(j).c_str());
      TH1D *histo_DiElectron = new TH1D(name1,"Dielectron Invariant Mass Distribution; M_{ee} [GeV]; N events",500,0,500);
      m_hVector_DiElectron[j].push_back(histo_DiElectron);

      char name2[300];
      sprintf(name2,"LeadingElectronPt_%s_%s",tag,Folders.at(j).c_str());
      TH1D *histo_LeadingElectronPt = new TH1D(name2,"Leading Electron - P_{T} Distribution; P_{T} [GeV.c^{-1}]; N events",200,0,1000);
      m_hVector_LeadingElectronPt[j].push_back(histo_LeadingElectronPt);

      char name3[300];
      sprintf(name3,"LeadingElectronEta_%s_%s",tag,Folders.at(j).c_str());
      TH1D *histo_LeadingElectronEta = new TH1D(name3,"Leading Electron - #eta Distribution; #eta; N events",50,-5.2,5.2);
      m_hVector_LeadingElectronEta[j].push_back(histo_LeadingElectronEta);

      char name4[300];
      sprintf(name4,"LeadingElectronPhi_%s_%s",tag,Folders.at(j).c_str());
      TH1D *histo_LeadingElectronPhi = new TH1D(name4,"Leading Electron - #phi Distribution; #phi [rad]; N events",50,-3.3,3.3);
      m_hVector_LeadingElectronPhi[j].push_back(histo_LeadingElectronPhi);

      char name5[300];
      sprintf(name5,"LeadingElectronCharge_%s_%s",tag,Folders.at(j).c_str());
      TH1D *histo_LeadingElectronCharge = new TH1D(name5,"Leading Electron - Charge Distribution; Charge; N events",50,-3,3);
      m_hVector_LeadingElectronCharge[j].push_back(histo_LeadingElectronCharge);

      char name6[300];
      sprintf(name6,"SecondElectronPt_%s_%s",tag,Folders.at(j).c_str());
      TH1D *histo_SecondElectronPt = new TH1D(name6,"Second Electron - P_{T} Distribution; P_{T} [GeV.c^{-1}]; N events",200,0,1000);
      m_hVector_SecondElectronPt[j].push_back(histo_SecondElectronPt);

      char name7[300];
      sprintf(name7,"SecondElectronEta_%s_%s",tag,Folders.at(j).c_str());
      TH1D *histo_SecondElectronEta = new TH1D(name7,"Second Electron - #eta Distribution; #eta; N events",50,-5.2,5.2);
      m_hVector_SecondElectronEta[j].push_back(histo_SecondElectronEta);

      char name8[300];
      sprintf(name8,"SecondElectronPhi_%s_%s",tag,Folders.at(j).c_str());
      TH1D *histo_SecondElectronPhi = new TH1D(name8,"Second Electron - #phi Distribution; #phi [rad]; N events",50,-3.3,3.3);
      m_hVector_SecondElectronPhi[j].push_back(histo_SecondElectronPhi);

      char name9[300];
      sprintf(name9,"SecondElectronCharge_%s_%s",tag,Folders.at(j).c_str());
      TH1D *histo_SecondElectronCharge = new TH1D(name9,"Second Electron - Charge Distribution; Charge; N events",50,-3,3);
      m_hVector_SecondElectronCharge[j].push_back(histo_SecondElectronCharge);

      char name10[300];
      sprintf(name10,"ElectronsN_%s_%s",tag,Folders.at(j).c_str());
      TH1D *histo_ElectronsN = new TH1D(name10,"Electrons per Event Distribution; Number of Electrons; N events",100,0,100);
      m_hVector_ElectronsN[j].push_back(histo_ElectronsN);

      char name11[300];
      sprintf(name11,"DiMuon_%s_%s",tag,Folders.at(j).c_str());
      TH1D *histo_DiMuon = new TH1D(name11,"DiMuon Invariant Mass Distribution; M_{#mu#mu} [GeV]; N events",500,0,500);
      m_hVector_DiMuon[j].push_back(histo_DiMuon);

      char name12[300];
      sprintf(name12,"LeadingMuonPt_%s_%s",tag,Folders.at(j).c_str());
      TH1D *histo_LeadingMuonPt = new TH1D(name12,"Leading Muon - P_{T} Distribution; P_{T} [GeV.c^{-1}]; N events",200,0,1000);
      m_hVector_LeadingMuonPt[j].push_back(histo_LeadingMuonPt);

      char name13[300];
      sprintf(name13,"LeadingMuonEta_%s_%s",tag,Folders.at(j).c_str());
      TH1D *histo_LeadingMuonEta = new TH1D(name13,"Leading Muon - #eta Distribution; #eta; N events",50,-5.2,5.2);
      m_hVector_LeadingMuonEta[j].push_back(histo_LeadingMuonEta);

      char name14[300];
      sprintf(name14,"LeadingMuonPhi_%s_%s",tag,Folders.at(j).c_str());
      TH1D *histo_LeadingMuonPhi = new TH1D(name14,"Leading Muon - #phi Distribution; #phi [rad]; N events",50,-3.3,3.3);
      m_hVector_LeadingMuonPhi[j].push_back(histo_LeadingMuonPhi);

      char name15[300];
      sprintf(name15,"LeadingMuonCharge_%s_%s",tag,Folders.at(j).c_str());
      TH1D *histo_LeadingMuonCharge = new TH1D(name15,"Leading Muon - Charge Distribution; Charge; N events",50,-3,3);
      m_hVector_LeadingMuonCharge[j].push_back(histo_LeadingMuonCharge);

      char name16[300];
      sprintf(name16,"SecondMuonPt_%s_%s",tag,Folders.at(j).c_str());
      TH1D *histo_SecondMuonPt = new TH1D(name16,"Second Muon - P_{T} Distribution; P_{T} [GeV.c^{-1}]; N events",200,0,1000);
      m_hVector_SecondMuonPt[j].push_back(histo_SecondMuonPt);

      char name17[300];
      sprintf(name17,"SecondMuonEta_%s_%s",tag,Folders.at(j).c_str());
      TH1D *histo_SecondMuonEta = new TH1D(name17,"Second Muon - #eta Distribution; #eta; N events",50,-5.2,5.2);
      m_hVector_SecondMuonEta[j].push_back(histo_SecondMuonEta);

      char name18[300];
      sprintf(name18,"SecondMuonPhi_%s_%s",tag,Folders.at(j).c_str());
      TH1D *histo_SecondMuonPhi = new TH1D(name18,"Second Muon - #phi Distribution; #phi [rad]; N events",50,-3.3,3.3);
      m_hVector_SecondMuonPhi[j].push_back(histo_SecondMuonPhi);

      char name19[300];
      sprintf(name19,"SecondMuonCharge_%s_%s",tag,Folders.at(j).c_str());
      TH1D *histo_SecondMuonCharge = new TH1D(name19,"Second Muon - Charge Distribution; Charge; N events",50,-3,3);
      m_hVector_SecondMuonCharge[j].push_back(histo_SecondMuonCharge);

      char name20[300];
      sprintf(name20,"MuonsN_%s_%s",tag,Folders.at(j).c_str());
      TH1D *histo_MuonsN = new TH1D(name20,"Muons per Event Distribution; Number of Muons; N events",100,0,100);
      m_hVector_MuonsN[j].push_back(histo_MuonsN);

      char name21[300];
      sprintf(name21,"sumEHFplus_%s_%s",tag,Folders.at(j).c_str());
      TH1D *histo_sumEHFplus = new TH1D(name21,"HF^{+} - Sum of Energy; #sum E_{HF^{+}} [GeV]; N events",200,0,2000);
      m_hVector_sumEHFplus[j].push_back(histo_sumEHFplus);

      char name22[300];
      sprintf(name22,"sumEHFminus_%s_%s",tag,Folders.at(j).c_str());
      TH1D *histo_sumEHFminus = new TH1D(name22,"HF^{-} - Sum of Energy; #sum E_{HF^{-}} [GeV]; N events",200,0,2000);
      m_hVector_sumEHFminus[j].push_back(histo_sumEHFminus);

      char name23[300];
      sprintf(name23,"sumEHEplus_PU_%s_%s",tag,Folders.at(j).c_str());
      TH1D *histo_sumEHEplus = new TH1D(name23,"HE^{+} - Sum of Energy; #sum E_{HE^{+}} [GeV]; N events",200,0,2000);
      m_hVector_sumEHEplus[j].push_back(histo_sumEHEplus);

      char name24[300];
      sprintf(name24,"sumEHEminus_%s_%s",tag,Folders.at(j).c_str());
      TH1D *histo_sumEHEminus = new TH1D(name24,"HE^{-} - Sum of Energy; #sum E_{HE^{-}} [GeV]; N events",200,0,2000);
      m_hVector_sumEHEminus[j].push_back(histo_sumEHEminus);

      char name25[300];
      sprintf(name25,"lumi_%s_%s",tag,Folders.at(j).c_str());
      TH1D *histo_lumi = new TH1D(name25,"Luminosity per Bunch; L_{Bunch} [#mub^{-1}s^{-1}]; N events",25,0,2);
      m_hVector_lumi[j].push_back(histo_lumi);      

      char name26[300];
      sprintf(name26,"aEnergy_%s_%s",tag,Folders.at(j).c_str());
      TH1D *histo_aSumE = new TH1D(name26,"Forward Backward Asymmetry Distribution ; (#sum HF^{+} - #sum HF^{-})x(#sum HF^{+} + #sum HF^{-})^{-1}; N events",50,-1,1);
      m_hVector_asumE[j].push_back(histo_aSumE);

      char name27[300];
      sprintf(name27,"mHF_%s_%s",tag,Folders.at(j).c_str());
      TH2F *histo_MultHF = new TH2F(name27,"HF^{+} and HF^{-} Multiplicity; n HF^{+}; n HF^{-}; N events", 10,  0., 10., 10,  0., 10. );
      m_hVector_multhf[j].push_back(histo_MultHF);

      char name28[300];
      sprintf(name28,"ETCalos_%s_%s",tag,Folders.at(j).c_str());
      TH2F *histo_ET_Calos = new TH2F(name28,"HF^{+} and Castor; #sum Energy HF^{+}; log_{10} #sum Castor Signal [fC]; N events", 10,  0., 10., 50,  0., 50. );
      m_hVector_etcalos[j].push_back(histo_ET_Calos);

      char name29[300];
      sprintf(name29,"Tracks_%s_%s",tag,Folders.at(j).c_str());
      TH1D *histo_Tracks = new TH1D(name29,"Tracks Multiplicity; n Tracks; N events",50,0,150);
      m_hVector_tracks[j].push_back(histo_Tracks);

      char name30[300];
      sprintf(name30,"pfetamax_%s_%s",tag,Folders.at(j).c_str());
      TH1D *histo_PFEtamax = new TH1D(name30,"Particle Flow #eta_{max} Distribution; #eta; N events",20,0,5.5);
      m_hVector_pfetamax[j].push_back(histo_PFEtamax);

      char name31[300];
      sprintf(name31,"pfetamin_%s_%s",tag,Folders.at(j).c_str());
      TH1D *histo_PFEtamin = new TH1D(name31,"Particle Flow #eta_{min} Distribution; #eta; N events",20,-5.5,0);
      m_hVector_pfetamin[j].push_back(histo_PFEtamin);

      char name32[300];
      sprintf(name32,"vertex_%s_%s",tag,Folders.at(j).c_str());
      TH1D *histo_vertex = new TH1D(name32,"Number of Vertex; # Vertex; N events",25,0,25);
      m_hVector_vertex[j].push_back(histo_vertex);

      char name33[300];
      sprintf(name33,"deltaphiElectrons_%s_%s",tag,Folders.at(j).c_str());
      TH1D *histo_deltaphielectrons = new TH1D(name33,"#Delta#phi_{ee} Distribution; #Delta#phi_{ee}; N events",20,0.0,3.2);
      m_hVector_deltaphielectrons[j].push_back(histo_deltaphielectrons);

      char name34[300];
      sprintf(name34,"deltaphiMuons_%s_%s",tag,Folders.at(j).c_str());
      TH1D *histo_deltaphimuons = new TH1D(name34,"#Delta#phi_{#mu#mu} Distribution; #Delta#phi_{#mu#mu}; N events",20,0.0,3.2);
      m_hVector_deltaphimuons[j].push_back(histo_deltaphimuons);

      char name35[300];
      sprintf(name35,"VertexVsLuminosity_%s_%s",tag,Folders.at(j).c_str());
      TH2F *histo_vertexvslumi = new TH2F(name35,"Vertex vs Luminosity; # Vertex; Luminosity per Bunch [#mub^{-1}s^{-1}]", 25.,  0., 25., 25,  0., 2.);
      m_hVector_vertexvslumi[j].push_back(histo_vertexvslumi);

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

    if (switchPUMultiple){
      indexV = eventinfo->GetNPileUpBx0();
    }
    else {
      indexV = 0;
    }

    if (!switchmcweight){
      mcweight=1.;
    }

    if (!switchPUMultiple || (switchPUMultiple && eventinfo->GetNPileUpBx0() < 21)){
      // Some Reco Variables
      aSumE = (eventdiff->GetSumEnergyHFPlus() - eventdiff->GetSumEnergyHFMinus())/(eventdiff->GetSumEnergyHFPlus() + eventdiff->GetSumEnergyHFMinus());
      deltaphielectrons = fabs(eventdiffZ->GetLeadingElectronPhi() - eventdiffZ->GetSecondElectronPhi());
      deltaphimuons = fabs(eventdiffZ->GetLeadingMuonPhi() - eventdiffZ->GetSecondMuonPhi());

      //No Cuts
      m_hVector_DiElectron[0].at(indexV)->Fill(eventdiffZ->GetDiElectronMass(),mcweight);
      m_hVector_LeadingElectronPt[0].at(indexV)->Fill(eventdiffZ->GetLeadingElectronPt(),mcweight);
      m_hVector_LeadingElectronEta[0].at(indexV)->Fill(eventdiffZ->GetLeadingElectronEta(),mcweight);
      m_hVector_LeadingElectronPhi[0].at(indexV)->Fill(eventdiffZ->GetLeadingElectronPhi(),mcweight);
      m_hVector_LeadingElectronCharge[0].at(indexV)->Fill(eventdiffZ->GetLeadingElectronCharge(),mcweight);
      m_hVector_SecondElectronPt[0].at(indexV)->Fill(eventdiffZ->GetSecondElectronPt(),mcweight);
      m_hVector_SecondElectronEta[0].at(indexV)->Fill(eventdiffZ->GetSecondElectronEta(),mcweight);
      m_hVector_SecondElectronPhi[0].at(indexV)->Fill(eventdiffZ->GetSecondElectronPhi(),mcweight);
      m_hVector_SecondElectronCharge[0].at(indexV)->Fill(eventdiffZ->GetSecondElectronCharge(),mcweight);
      m_hVector_ElectronsN[0].at(indexV)->Fill(eventdiffZ->GetElectronsN(),mcweight);
      m_hVector_DiMuon[0].at(indexV)->Fill(eventdiffZ->GetDiMuonMass(),mcweight);
      m_hVector_LeadingMuonPt[0].at(indexV)->Fill(eventdiffZ->GetLeadingMuonPt(),mcweight);
      m_hVector_LeadingMuonEta[0].at(indexV)->Fill(eventdiffZ->GetLeadingMuonEta(),mcweight);
      m_hVector_LeadingMuonPhi[0].at(indexV)->Fill(eventdiffZ->GetLeadingMuonPhi(),mcweight);
      m_hVector_LeadingMuonCharge[0].at(indexV)->Fill(eventdiffZ->GetLeadingMuonCharge(),mcweight);
      m_hVector_SecondMuonPt[0].at(indexV)->Fill(eventdiffZ->GetSecondMuonPt(),mcweight);
      m_hVector_SecondMuonEta[0].at(indexV)->Fill(eventdiffZ->GetSecondMuonEta(),mcweight);
      m_hVector_SecondMuonPhi[0].at(indexV)->Fill(eventdiffZ->GetSecondMuonPhi(),mcweight);
      m_hVector_SecondMuonCharge[0].at(indexV)->Fill(eventdiffZ->GetSecondMuonCharge(),mcweight);
      m_hVector_MuonsN[0].at(indexV)->Fill(eventdiffZ->GetMuonsN(),mcweight);
      m_hVector_sumEHFplus[0].at(indexV)->Fill(eventdiff->GetSumEnergyHFPlus(),mcweight);
      m_hVector_sumEHFminus[0].at(indexV)->Fill(eventdiff->GetSumEnergyHFMinus(),mcweight);
      m_hVector_sumEHEplus[0].at(indexV)->Fill(eventdiff->GetSumEnergyHEPlus(),mcweight);
      m_hVector_sumEHEminus[0].at(indexV)->Fill(eventdiff->GetSumEnergyHEMinus(),mcweight);
      m_hVector_lumi[0].at(indexV)->Fill(eventinfo->GetInstLumiBunch(),mcweight);
      m_hVector_asumE[0].at(indexV)->Fill(aSumE,mcweight);
      m_hVector_multhf[0].at(indexV)->Fill(eventdiff->GetMultiplicityHFPlus(),eventdiff->GetMultiplicityHFMinus(),mcweight);
      m_hVector_etcalos[0].at(indexV)->Fill(eventdiff->GetSumEnergyHFPlus(),log10(fabs(eventdiff->GetSumETotCastor())),mcweight);
      m_hVector_tracks[0].at(indexV)->Fill(eventdiff->GetMultiplicityTracks(),mcweight);
      m_hVector_pfetamax[0].at(indexV)->Fill(eventdiff->GetEtaMaxFromPFCands(),mcweight);
      m_hVector_pfetamin[0].at(indexV)->Fill(eventdiff->GetEtaMinFromPFCands(),mcweight);
      m_hVector_vertex[0].at(indexV)->Fill(eventdiff->GetNVertex(),mcweight);
      m_hVector_deltaphielectrons[0].at(indexV)->Fill(deltaphielectrons,mcweight);
      m_hVector_deltaphimuons[0].at(indexV)->Fill(deltaphimuons,mcweight);
      m_hVector_vertexvslumi[0].at(indexV)->Fill(eventdiff->GetNVertex(),eventinfo->GetInstLumiBunch(),mcweight);

      // Step1
      if (!switchTrigger || (switchTrigger && eventdiffZ->GetHLTPath(optTrigger)) ){
	m_hVector_DiElectron[1].at(indexV)->Fill(eventdiffZ->GetDiElectronMass(),mcweight);
	m_hVector_LeadingElectronPt[1].at(indexV)->Fill(eventdiffZ->GetLeadingElectronPt(),mcweight);
	m_hVector_LeadingElectronEta[1].at(indexV)->Fill(eventdiffZ->GetLeadingElectronEta(),mcweight);
	m_hVector_LeadingElectronPhi[1].at(indexV)->Fill(eventdiffZ->GetLeadingElectronPhi(),mcweight);
	m_hVector_LeadingElectronCharge[1].at(indexV)->Fill(eventdiffZ->GetLeadingElectronCharge(),mcweight);
	m_hVector_SecondElectronPt[1].at(indexV)->Fill(eventdiffZ->GetSecondElectronPt(),mcweight);
	m_hVector_SecondElectronEta[1].at(indexV)->Fill(eventdiffZ->GetSecondElectronEta(),mcweight);
	m_hVector_SecondElectronPhi[1].at(indexV)->Fill(eventdiffZ->GetSecondElectronPhi(),mcweight);
	m_hVector_SecondElectronCharge[1].at(indexV)->Fill(eventdiffZ->GetSecondElectronCharge(),mcweight);
	m_hVector_ElectronsN[1].at(indexV)->Fill(eventdiffZ->GetElectronsN(),mcweight);
	m_hVector_DiMuon[1].at(indexV)->Fill(eventdiffZ->GetDiMuonMass(),mcweight);
	m_hVector_LeadingMuonPt[1].at(indexV)->Fill(eventdiffZ->GetLeadingMuonPt(),mcweight);
	m_hVector_LeadingMuonEta[1].at(indexV)->Fill(eventdiffZ->GetLeadingMuonEta(),mcweight);
	m_hVector_LeadingMuonPhi[1].at(indexV)->Fill(eventdiffZ->GetLeadingMuonPhi(),mcweight);
	m_hVector_LeadingMuonCharge[1].at(indexV)->Fill(eventdiffZ->GetLeadingMuonCharge(),mcweight);
	m_hVector_SecondMuonPt[1].at(indexV)->Fill(eventdiffZ->GetSecondMuonPt(),mcweight);
	m_hVector_SecondMuonEta[1].at(indexV)->Fill(eventdiffZ->GetSecondMuonEta(),mcweight);
	m_hVector_SecondMuonPhi[1].at(indexV)->Fill(eventdiffZ->GetSecondMuonPhi(),mcweight);
	m_hVector_SecondMuonCharge[1].at(indexV)->Fill(eventdiffZ->GetSecondMuonCharge(),mcweight);
	m_hVector_MuonsN[1].at(indexV)->Fill(eventdiffZ->GetMuonsN(),mcweight);
	m_hVector_sumEHFplus[1].at(indexV)->Fill(eventdiff->GetSumEnergyHFPlus(),mcweight);
	m_hVector_sumEHFminus[1].at(indexV)->Fill(eventdiff->GetSumEnergyHFMinus(),mcweight);
	m_hVector_sumEHEplus[1].at(indexV)->Fill(eventdiff->GetSumEnergyHEPlus(),mcweight);
	m_hVector_sumEHEminus[1].at(indexV)->Fill(eventdiff->GetSumEnergyHEMinus(),mcweight);
	m_hVector_lumi[1].at(indexV)->Fill(eventinfo->GetInstLumiBunch(),mcweight);
	m_hVector_asumE[1].at(indexV)->Fill(aSumE,mcweight);
	m_hVector_multhf[1].at(indexV)->Fill(eventdiff->GetMultiplicityHFPlus(),eventdiff->GetMultiplicityHFMinus(),mcweight);
	m_hVector_etcalos[1].at(indexV)->Fill(eventdiff->GetSumEnergyHFPlus(),log10(fabs(eventdiff->GetSumETotCastor())),mcweight);
	m_hVector_tracks[1].at(indexV)->Fill(eventdiff->GetMultiplicityTracks(),mcweight);
	m_hVector_pfetamax[1].at(indexV)->Fill(eventdiff->GetEtaMaxFromPFCands(),mcweight);
	m_hVector_pfetamin[1].at(indexV)->Fill(eventdiff->GetEtaMinFromPFCands(),mcweight);
	m_hVector_vertex[1].at(indexV)->Fill(eventdiff->GetNVertex(),mcweight);
	m_hVector_deltaphielectrons[1].at(indexV)->Fill(deltaphielectrons,mcweight);
	m_hVector_deltaphimuons[1].at(indexV)->Fill(deltaphimuons,mcweight);
	m_hVector_vertexvslumi[1].at(indexV)->Fill(eventdiff->GetNVertex(),eventinfo->GetInstLumiBunch(),mcweight);

	//Step2
	if (!switchPreSel || (switchPreSel && (eventdiffZ->GetLeadingElectronPt() > lepton1pt && eventdiffZ->GetSecondElectronPt() > lepton2pt) )) {
	  m_hVector_DiElectron[2].at(indexV)->Fill(eventdiffZ->GetDiElectronMass(),mcweight);
	  m_hVector_LeadingElectronPt[2].at(indexV)->Fill(eventdiffZ->GetLeadingElectronPt(),mcweight);
	  m_hVector_LeadingElectronEta[2].at(indexV)->Fill(eventdiffZ->GetLeadingElectronEta(),mcweight);
	  m_hVector_LeadingElectronPhi[2].at(indexV)->Fill(eventdiffZ->GetLeadingElectronPhi(),mcweight);
	  m_hVector_LeadingElectronCharge[2].at(indexV)->Fill(eventdiffZ->GetLeadingElectronCharge(),mcweight);
	  m_hVector_SecondElectronPt[2].at(indexV)->Fill(eventdiffZ->GetSecondElectronPt(),mcweight);
	  m_hVector_SecondElectronEta[2].at(indexV)->Fill(eventdiffZ->GetSecondElectronEta(),mcweight);
	  m_hVector_SecondElectronPhi[2].at(indexV)->Fill(eventdiffZ->GetSecondElectronPhi(),mcweight);
	  m_hVector_SecondElectronCharge[2].at(indexV)->Fill(eventdiffZ->GetSecondElectronCharge(),mcweight);
	  m_hVector_ElectronsN[2].at(indexV)->Fill(eventdiffZ->GetElectronsN(),mcweight);
	  m_hVector_DiMuon[2].at(indexV)->Fill(eventdiffZ->GetDiMuonMass(),mcweight);
	  m_hVector_LeadingMuonPt[2].at(indexV)->Fill(eventdiffZ->GetLeadingMuonPt(),mcweight);
	  m_hVector_LeadingMuonEta[2].at(indexV)->Fill(eventdiffZ->GetLeadingMuonEta(),mcweight);
	  m_hVector_LeadingMuonPhi[2].at(indexV)->Fill(eventdiffZ->GetLeadingMuonPhi(),mcweight);
	  m_hVector_LeadingMuonCharge[2].at(indexV)->Fill(eventdiffZ->GetLeadingMuonCharge(),mcweight);
	  m_hVector_SecondMuonPt[2].at(indexV)->Fill(eventdiffZ->GetSecondMuonPt(),mcweight);
	  m_hVector_SecondMuonEta[2].at(indexV)->Fill(eventdiffZ->GetSecondMuonEta(),mcweight);
	  m_hVector_SecondMuonPhi[2].at(indexV)->Fill(eventdiffZ->GetSecondMuonPhi(),mcweight);
	  m_hVector_SecondMuonCharge[2].at(indexV)->Fill(eventdiffZ->GetSecondMuonCharge(),mcweight);
	  m_hVector_MuonsN[2].at(indexV)->Fill(eventdiffZ->GetMuonsN(),mcweight);
	  m_hVector_sumEHFplus[2].at(indexV)->Fill(eventdiff->GetSumEnergyHFPlus(),mcweight);
	  m_hVector_sumEHFminus[2].at(indexV)->Fill(eventdiff->GetSumEnergyHFMinus(),mcweight);
	  m_hVector_sumEHEplus[2].at(indexV)->Fill(eventdiff->GetSumEnergyHEPlus(),mcweight);
	  m_hVector_sumEHEminus[2].at(indexV)->Fill(eventdiff->GetSumEnergyHEMinus(),mcweight);
	  m_hVector_lumi[2].at(indexV)->Fill(eventinfo->GetInstLumiBunch(),mcweight);
	  m_hVector_asumE[2].at(indexV)->Fill(aSumE,mcweight);
	  m_hVector_multhf[2].at(indexV)->Fill(eventdiff->GetMultiplicityHFPlus(),eventdiff->GetMultiplicityHFMinus(),mcweight);
	  m_hVector_etcalos[2].at(indexV)->Fill(eventdiff->GetSumEnergyHFPlus(),log10(fabs(eventdiff->GetSumETotCastor())),mcweight);
	  m_hVector_tracks[2].at(indexV)->Fill(eventdiff->GetMultiplicityTracks(),mcweight);
	  m_hVector_pfetamax[2].at(indexV)->Fill(eventdiff->GetEtaMaxFromPFCands(),mcweight);
	  m_hVector_pfetamin[2].at(indexV)->Fill(eventdiff->GetEtaMinFromPFCands(),mcweight);
	  m_hVector_vertex[2].at(indexV)->Fill(eventdiff->GetNVertex(),mcweight);
	  m_hVector_deltaphielectrons[2].at(indexV)->Fill(deltaphielectrons,mcweight);
	  m_hVector_deltaphimuons[2].at(indexV)->Fill(deltaphimuons,mcweight);
	  m_hVector_vertexvslumi[2].at(indexV)->Fill(eventdiff->GetNVertex(),eventinfo->GetInstLumiBunch(),mcweight);

	  // Step3
	  if(eventdiff->GetNVertex() > 0 && eventdiff->GetNVertex()<= nVertex){
	    m_hVector_DiElectron[3].at(indexV)->Fill(eventdiffZ->GetDiElectronMass(),mcweight);
	    m_hVector_LeadingElectronPt[3].at(indexV)->Fill(eventdiffZ->GetLeadingElectronPt(),mcweight);
	    m_hVector_LeadingElectronEta[3].at(indexV)->Fill(eventdiffZ->GetLeadingElectronEta(),mcweight);
	    m_hVector_LeadingElectronPhi[3].at(indexV)->Fill(eventdiffZ->GetLeadingElectronPhi(),mcweight);
	    m_hVector_LeadingElectronCharge[3].at(indexV)->Fill(eventdiffZ->GetLeadingElectronCharge(),mcweight);
	    m_hVector_SecondElectronPt[3].at(indexV)->Fill(eventdiffZ->GetSecondElectronPt(),mcweight);
	    m_hVector_SecondElectronEta[3].at(indexV)->Fill(eventdiffZ->GetSecondElectronEta(),mcweight);
	    m_hVector_SecondElectronPhi[3].at(indexV)->Fill(eventdiffZ->GetSecondElectronPhi(),mcweight);
	    m_hVector_SecondElectronCharge[3].at(indexV)->Fill(eventdiffZ->GetSecondElectronCharge(),mcweight);
	    m_hVector_ElectronsN[3].at(indexV)->Fill(eventdiffZ->GetElectronsN(),mcweight);
	    m_hVector_DiMuon[3].at(indexV)->Fill(eventdiffZ->GetDiMuonMass(),mcweight);
	    m_hVector_LeadingMuonPt[3].at(indexV)->Fill(eventdiffZ->GetLeadingMuonPt(),mcweight);
	    m_hVector_LeadingMuonEta[3].at(indexV)->Fill(eventdiffZ->GetLeadingMuonEta(),mcweight);
	    m_hVector_LeadingMuonPhi[3].at(indexV)->Fill(eventdiffZ->GetLeadingMuonPhi(),mcweight);
	    m_hVector_LeadingMuonCharge[3].at(indexV)->Fill(eventdiffZ->GetLeadingMuonCharge(),mcweight);
	    m_hVector_SecondMuonPt[3].at(indexV)->Fill(eventdiffZ->GetSecondMuonPt(),mcweight);
	    m_hVector_SecondMuonEta[3].at(indexV)->Fill(eventdiffZ->GetSecondMuonEta(),mcweight);
	    m_hVector_SecondMuonPhi[3].at(indexV)->Fill(eventdiffZ->GetSecondMuonPhi(),mcweight);
	    m_hVector_SecondMuonCharge[3].at(indexV)->Fill(eventdiffZ->GetSecondMuonCharge(),mcweight);
	    m_hVector_MuonsN[3].at(indexV)->Fill(eventdiffZ->GetMuonsN(),mcweight);
	    m_hVector_sumEHFplus[3].at(indexV)->Fill(eventdiff->GetSumEnergyHFPlus(),mcweight);
	    m_hVector_sumEHFminus[3].at(indexV)->Fill(eventdiff->GetSumEnergyHFMinus(),mcweight);
	    m_hVector_sumEHEplus[3].at(indexV)->Fill(eventdiff->GetSumEnergyHEPlus(),mcweight);
	    m_hVector_sumEHEminus[3].at(indexV)->Fill(eventdiff->GetSumEnergyHEMinus(),mcweight);
	    m_hVector_lumi[3].at(indexV)->Fill(eventinfo->GetInstLumiBunch(),mcweight);
	    m_hVector_asumE[3].at(indexV)->Fill(aSumE,mcweight);
	    m_hVector_multhf[3].at(indexV)->Fill(eventdiff->GetMultiplicityHFPlus(),eventdiff->GetMultiplicityHFMinus(),mcweight);
	    m_hVector_etcalos[3].at(indexV)->Fill(eventdiff->GetSumEnergyHFPlus(),log10(fabs(eventdiff->GetSumETotCastor())),mcweight);
	    m_hVector_tracks[3].at(indexV)->Fill(eventdiff->GetMultiplicityTracks(),mcweight);
	    m_hVector_pfetamax[3].at(indexV)->Fill(eventdiff->GetEtaMaxFromPFCands(),mcweight);
	    m_hVector_pfetamin[3].at(indexV)->Fill(eventdiff->GetEtaMinFromPFCands(),mcweight);
	    m_hVector_vertex[3].at(indexV)->Fill(eventdiff->GetNVertex(),mcweight);
	    m_hVector_deltaphielectrons[3].at(indexV)->Fill(deltaphielectrons,mcweight);
	    m_hVector_deltaphimuons[3].at(indexV)->Fill(deltaphimuons,mcweight);
	    m_hVector_vertexvslumi[3].at(indexV)->Fill(eventdiff->GetNVertex(),eventinfo->GetInstLumiBunch(),mcweight);

            // Step4
	    if((eventdiffZ->GetLeadingElectronCharge()*eventdiffZ->GetSecondElectronCharge()==-1)){
	      m_hVector_DiElectron[4].at(indexV)->Fill(eventdiffZ->GetDiElectronMass(),mcweight);
	      m_hVector_LeadingElectronPt[4].at(indexV)->Fill(eventdiffZ->GetLeadingElectronPt(),mcweight);
	      m_hVector_LeadingElectronEta[4].at(indexV)->Fill(eventdiffZ->GetLeadingElectronEta(),mcweight);
	      m_hVector_LeadingElectronPhi[4].at(indexV)->Fill(eventdiffZ->GetLeadingElectronPhi(),mcweight);
	      m_hVector_LeadingElectronCharge[4].at(indexV)->Fill(eventdiffZ->GetLeadingElectronCharge(),mcweight);
	      m_hVector_SecondElectronPt[4].at(indexV)->Fill(eventdiffZ->GetSecondElectronPt(),mcweight);
	      m_hVector_SecondElectronEta[4].at(indexV)->Fill(eventdiffZ->GetSecondElectronEta(),mcweight);
	      m_hVector_SecondElectronPhi[4].at(indexV)->Fill(eventdiffZ->GetSecondElectronPhi(),mcweight);
	      m_hVector_SecondElectronCharge[4].at(indexV)->Fill(eventdiffZ->GetSecondElectronCharge(),mcweight);
	      m_hVector_ElectronsN[4].at(indexV)->Fill(eventdiffZ->GetElectronsN(),mcweight);
	      m_hVector_DiMuon[4].at(indexV)->Fill(eventdiffZ->GetDiMuonMass(),mcweight);
	      m_hVector_LeadingMuonPt[4].at(indexV)->Fill(eventdiffZ->GetLeadingMuonPt(),mcweight);
	      m_hVector_LeadingMuonEta[4].at(indexV)->Fill(eventdiffZ->GetLeadingMuonEta(),mcweight);
	      m_hVector_LeadingMuonPhi[4].at(indexV)->Fill(eventdiffZ->GetLeadingMuonPhi(),mcweight);
	      m_hVector_LeadingMuonCharge[4].at(indexV)->Fill(eventdiffZ->GetLeadingMuonCharge(),mcweight);
	      m_hVector_SecondMuonPt[4].at(indexV)->Fill(eventdiffZ->GetSecondMuonPt(),mcweight);
	      m_hVector_SecondMuonEta[4].at(indexV)->Fill(eventdiffZ->GetSecondMuonEta(),mcweight);
	      m_hVector_SecondMuonPhi[4].at(indexV)->Fill(eventdiffZ->GetSecondMuonPhi(),mcweight);
	      m_hVector_SecondMuonCharge[4].at(indexV)->Fill(eventdiffZ->GetSecondMuonCharge(),mcweight);
	      m_hVector_MuonsN[4].at(indexV)->Fill(eventdiffZ->GetMuonsN(),mcweight);
	      m_hVector_sumEHFplus[4].at(indexV)->Fill(eventdiff->GetSumEnergyHFPlus(),mcweight);
	      m_hVector_sumEHFminus[4].at(indexV)->Fill(eventdiff->GetSumEnergyHFMinus(),mcweight);
	      m_hVector_sumEHEplus[4].at(indexV)->Fill(eventdiff->GetSumEnergyHEPlus(),mcweight);
	      m_hVector_sumEHEminus[4].at(indexV)->Fill(eventdiff->GetSumEnergyHEMinus(),mcweight);
	      m_hVector_lumi[4].at(indexV)->Fill(eventinfo->GetInstLumiBunch(),mcweight);
	      m_hVector_asumE[4].at(indexV)->Fill(aSumE,mcweight);
	      m_hVector_multhf[4].at(indexV)->Fill(eventdiff->GetMultiplicityHFPlus(),eventdiff->GetMultiplicityHFMinus(),mcweight);
	      m_hVector_etcalos[4].at(indexV)->Fill(eventdiff->GetSumEnergyHFPlus(),log10(fabs(eventdiff->GetSumETotCastor())),mcweight);
	      m_hVector_tracks[4].at(indexV)->Fill(eventdiff->GetMultiplicityTracks(),mcweight);
	      m_hVector_pfetamax[4].at(indexV)->Fill(eventdiff->GetEtaMaxFromPFCands(),mcweight);
	      m_hVector_pfetamin[4].at(indexV)->Fill(eventdiff->GetEtaMinFromPFCands(),mcweight);
	      m_hVector_vertex[4].at(indexV)->Fill(eventdiff->GetNVertex(),mcweight);
	      m_hVector_deltaphielectrons[4].at(indexV)->Fill(deltaphielectrons,mcweight);
	      m_hVector_deltaphimuons[4].at(indexV)->Fill(deltaphimuons,mcweight);
	      m_hVector_vertexvslumi[4].at(indexV)->Fill(eventdiff->GetNVertex(),eventinfo->GetInstLumiBunch(),mcweight);


	      // Step5
	      if( eventdiffZ->GetDiElectronMass() > 60. && eventdiffZ->GetDiElectronMass() < 110. ){
		m_hVector_DiElectron[5].at(indexV)->Fill(eventdiffZ->GetDiElectronMass(),mcweight);
		m_hVector_LeadingElectronPt[5].at(indexV)->Fill(eventdiffZ->GetLeadingElectronPt(),mcweight);
		m_hVector_LeadingElectronEta[5].at(indexV)->Fill(eventdiffZ->GetLeadingElectronEta(),mcweight);
		m_hVector_LeadingElectronPhi[5].at(indexV)->Fill(eventdiffZ->GetLeadingElectronPhi(),mcweight);
		m_hVector_LeadingElectronCharge[5].at(indexV)->Fill(eventdiffZ->GetLeadingElectronCharge(),mcweight);
		m_hVector_SecondElectronPt[5].at(indexV)->Fill(eventdiffZ->GetSecondElectronPt(),mcweight);
		m_hVector_SecondElectronEta[5].at(indexV)->Fill(eventdiffZ->GetSecondElectronEta(),mcweight);
		m_hVector_SecondElectronPhi[5].at(indexV)->Fill(eventdiffZ->GetSecondElectronPhi(),mcweight);
		m_hVector_SecondElectronCharge[5].at(indexV)->Fill(eventdiffZ->GetSecondElectronCharge(),mcweight);
		m_hVector_ElectronsN[5].at(indexV)->Fill(eventdiffZ->GetElectronsN(),mcweight);
		m_hVector_DiMuon[5].at(indexV)->Fill(eventdiffZ->GetDiMuonMass(),mcweight);
		m_hVector_LeadingMuonPt[5].at(indexV)->Fill(eventdiffZ->GetLeadingMuonPt(),mcweight);
		m_hVector_LeadingMuonEta[5].at(indexV)->Fill(eventdiffZ->GetLeadingMuonEta(),mcweight);
		m_hVector_LeadingMuonPhi[5].at(indexV)->Fill(eventdiffZ->GetLeadingMuonPhi(),mcweight);
		m_hVector_LeadingMuonCharge[5].at(indexV)->Fill(eventdiffZ->GetLeadingMuonCharge(),mcweight);
		m_hVector_SecondMuonPt[5].at(indexV)->Fill(eventdiffZ->GetSecondMuonPt(),mcweight);
		m_hVector_SecondMuonEta[5].at(indexV)->Fill(eventdiffZ->GetSecondMuonEta(),mcweight);
		m_hVector_SecondMuonPhi[5].at(indexV)->Fill(eventdiffZ->GetSecondMuonPhi(),mcweight);
		m_hVector_SecondMuonCharge[5].at(indexV)->Fill(eventdiffZ->GetSecondMuonCharge(),mcweight);
		m_hVector_MuonsN[5].at(indexV)->Fill(eventdiffZ->GetMuonsN(),mcweight);
		m_hVector_sumEHFplus[5].at(indexV)->Fill(eventdiff->GetSumEnergyHFPlus(),mcweight);
		m_hVector_sumEHFminus[5].at(indexV)->Fill(eventdiff->GetSumEnergyHFMinus(),mcweight);
		m_hVector_sumEHEplus[5].at(indexV)->Fill(eventdiff->GetSumEnergyHEPlus(),mcweight);
		m_hVector_sumEHEminus[5].at(indexV)->Fill(eventdiff->GetSumEnergyHEMinus(),mcweight);
		m_hVector_lumi[5].at(indexV)->Fill(eventinfo->GetInstLumiBunch(),mcweight);
		m_hVector_asumE[5].at(indexV)->Fill(aSumE,mcweight);
		m_hVector_multhf[5].at(indexV)->Fill(eventdiff->GetMultiplicityHFPlus(),eventdiff->GetMultiplicityHFMinus(),mcweight);
		m_hVector_etcalos[5].at(indexV)->Fill(eventdiff->GetSumEnergyHFPlus(),log10(fabs(eventdiff->GetSumETotCastor())),mcweight);
		m_hVector_tracks[5].at(indexV)->Fill(eventdiff->GetMultiplicityTracks(),mcweight);
		m_hVector_pfetamax[5].at(indexV)->Fill(eventdiff->GetEtaMaxFromPFCands(),mcweight);
		m_hVector_pfetamin[5].at(indexV)->Fill(eventdiff->GetEtaMinFromPFCands(),mcweight);
		m_hVector_vertex[5].at(indexV)->Fill(eventdiff->GetNVertex(),mcweight);
		m_hVector_deltaphielectrons[5].at(indexV)->Fill(deltaphielectrons,mcweight);
		m_hVector_deltaphimuons[5].at(indexV)->Fill(deltaphimuons,mcweight);
		m_hVector_vertexvslumi[5].at(indexV)->Fill(eventdiff->GetNVertex(),eventinfo->GetInstLumiBunch(),mcweight);

		// Step6
		if(eventdiffZ->GetElectronsN() == 2){
		  m_hVector_DiElectron[6].at(indexV)->Fill(eventdiffZ->GetDiElectronMass(),mcweight);
		  m_hVector_LeadingElectronPt[6].at(indexV)->Fill(eventdiffZ->GetLeadingElectronPt(),mcweight);
		  m_hVector_LeadingElectronEta[6].at(indexV)->Fill(eventdiffZ->GetLeadingElectronEta(),mcweight);
		  m_hVector_LeadingElectronPhi[6].at(indexV)->Fill(eventdiffZ->GetLeadingElectronPhi(),mcweight);
		  m_hVector_LeadingElectronCharge[6].at(indexV)->Fill(eventdiffZ->GetLeadingElectronCharge(),mcweight);
		  m_hVector_SecondElectronPt[6].at(indexV)->Fill(eventdiffZ->GetSecondElectronPt(),mcweight);
		  m_hVector_SecondElectronEta[6].at(indexV)->Fill(eventdiffZ->GetSecondElectronEta(),mcweight);
		  m_hVector_SecondElectronPhi[6].at(indexV)->Fill(eventdiffZ->GetSecondElectronPhi(),mcweight);
		  m_hVector_SecondElectronCharge[6].at(indexV)->Fill(eventdiffZ->GetSecondElectronCharge(),mcweight);
		  m_hVector_ElectronsN[6].at(indexV)->Fill(eventdiffZ->GetElectronsN(),mcweight);
		  m_hVector_DiMuon[6].at(indexV)->Fill(eventdiffZ->GetDiMuonMass(),mcweight);
		  m_hVector_LeadingMuonPt[6].at(indexV)->Fill(eventdiffZ->GetLeadingMuonPt(),mcweight);
		  m_hVector_LeadingMuonEta[6].at(indexV)->Fill(eventdiffZ->GetLeadingMuonEta(),mcweight);
		  m_hVector_LeadingMuonPhi[6].at(indexV)->Fill(eventdiffZ->GetLeadingMuonPhi(),mcweight);
		  m_hVector_LeadingMuonCharge[6].at(indexV)->Fill(eventdiffZ->GetLeadingMuonCharge(),mcweight);
		  m_hVector_SecondMuonPt[6].at(indexV)->Fill(eventdiffZ->GetSecondMuonPt(),mcweight);
		  m_hVector_SecondMuonEta[6].at(indexV)->Fill(eventdiffZ->GetSecondMuonEta(),mcweight);
		  m_hVector_SecondMuonPhi[6].at(indexV)->Fill(eventdiffZ->GetSecondMuonPhi(),mcweight);
		  m_hVector_SecondMuonCharge[6].at(indexV)->Fill(eventdiffZ->GetSecondMuonCharge(),mcweight);
		  m_hVector_MuonsN[6].at(indexV)->Fill(eventdiffZ->GetMuonsN(),mcweight);
		  m_hVector_sumEHFplus[6].at(indexV)->Fill(eventdiff->GetSumEnergyHFPlus(),mcweight);
		  m_hVector_sumEHFminus[6].at(indexV)->Fill(eventdiff->GetSumEnergyHFMinus(),mcweight);
		  m_hVector_sumEHEplus[6].at(indexV)->Fill(eventdiff->GetSumEnergyHEPlus(),mcweight);
		  m_hVector_sumEHEminus[6].at(indexV)->Fill(eventdiff->GetSumEnergyHEMinus(),mcweight);
		  m_hVector_lumi[6].at(indexV)->Fill(eventinfo->GetInstLumiBunch(),mcweight);
		  m_hVector_asumE[6].at(indexV)->Fill(aSumE,mcweight);
		  m_hVector_multhf[6].at(indexV)->Fill(eventdiff->GetMultiplicityHFPlus(),eventdiff->GetMultiplicityHFMinus(),mcweight);
		  m_hVector_etcalos[6].at(indexV)->Fill(eventdiff->GetSumEnergyHFPlus(),log10(fabs(eventdiff->GetSumETotCastor())),mcweight);
		  m_hVector_tracks[6].at(indexV)->Fill(eventdiff->GetMultiplicityTracks(),mcweight);
		  m_hVector_pfetamax[6].at(indexV)->Fill(eventdiff->GetEtaMaxFromPFCands(),mcweight);
		  m_hVector_pfetamin[6].at(indexV)->Fill(eventdiff->GetEtaMinFromPFCands(),mcweight);
		  m_hVector_vertex[6].at(indexV)->Fill(eventdiff->GetNVertex(),mcweight);
		  m_hVector_deltaphielectrons[6].at(indexV)->Fill(deltaphielectrons,mcweight);
		  m_hVector_deltaphimuons[6].at(indexV)->Fill(deltaphimuons,mcweight);
		  m_hVector_vertexvslumi[6].at(indexV)->Fill(eventdiff->GetNVertex(),eventinfo->GetInstLumiBunch(),mcweight);

		  // Step7          
		  if( (eventdiff->GetEtaMaxFromPFCands() < 3.) || (eventdiff->GetEtaMinFromPFCands() > -3.) ){
		    m_hVector_DiElectron[7].at(indexV)->Fill(eventdiffZ->GetDiElectronMass(),mcweight);
		    m_hVector_LeadingElectronPt[7].at(indexV)->Fill(eventdiffZ->GetLeadingElectronPt(),mcweight);
		    m_hVector_LeadingElectronEta[7].at(indexV)->Fill(eventdiffZ->GetLeadingElectronEta(),mcweight);
		    m_hVector_LeadingElectronPhi[7].at(indexV)->Fill(eventdiffZ->GetLeadingElectronPhi(),mcweight);
		    m_hVector_LeadingElectronCharge[7].at(indexV)->Fill(eventdiffZ->GetLeadingElectronCharge(),mcweight);
		    m_hVector_SecondElectronPt[7].at(indexV)->Fill(eventdiffZ->GetSecondElectronPt(),mcweight);
		    m_hVector_SecondElectronEta[7].at(indexV)->Fill(eventdiffZ->GetSecondElectronEta(),mcweight);
		    m_hVector_SecondElectronPhi[7].at(indexV)->Fill(eventdiffZ->GetSecondElectronPhi(),mcweight);
		    m_hVector_SecondElectronCharge[7].at(indexV)->Fill(eventdiffZ->GetSecondElectronCharge(),mcweight);
		    m_hVector_ElectronsN[7].at(indexV)->Fill(eventdiffZ->GetElectronsN(),mcweight);
		    m_hVector_DiMuon[7].at(indexV)->Fill(eventdiffZ->GetDiMuonMass(),mcweight);
		    m_hVector_LeadingMuonPt[7].at(indexV)->Fill(eventdiffZ->GetLeadingMuonPt(),mcweight);
		    m_hVector_LeadingMuonEta[7].at(indexV)->Fill(eventdiffZ->GetLeadingMuonEta(),mcweight);
		    m_hVector_LeadingMuonPhi[7].at(indexV)->Fill(eventdiffZ->GetLeadingMuonPhi(),mcweight);
		    m_hVector_LeadingMuonCharge[7].at(indexV)->Fill(eventdiffZ->GetLeadingMuonCharge(),mcweight);
		    m_hVector_SecondMuonPt[7].at(indexV)->Fill(eventdiffZ->GetSecondMuonPt(),mcweight);
		    m_hVector_SecondMuonEta[7].at(indexV)->Fill(eventdiffZ->GetSecondMuonEta(),mcweight);
		    m_hVector_SecondMuonPhi[7].at(indexV)->Fill(eventdiffZ->GetSecondMuonPhi(),mcweight);
		    m_hVector_SecondMuonCharge[7].at(indexV)->Fill(eventdiffZ->GetSecondMuonCharge(),mcweight);
		    m_hVector_MuonsN[7].at(indexV)->Fill(eventdiffZ->GetMuonsN(),mcweight);
		    m_hVector_sumEHFplus[7].at(indexV)->Fill(eventdiff->GetSumEnergyHFPlus(),mcweight);
		    m_hVector_sumEHFminus[7].at(indexV)->Fill(eventdiff->GetSumEnergyHFMinus(),mcweight);
		    m_hVector_sumEHEplus[7].at(indexV)->Fill(eventdiff->GetSumEnergyHEPlus(),mcweight);
		    m_hVector_sumEHEminus[7].at(indexV)->Fill(eventdiff->GetSumEnergyHEMinus(),mcweight);
		    m_hVector_lumi[7].at(indexV)->Fill(eventinfo->GetInstLumiBunch(),mcweight);
		    m_hVector_asumE[7].at(indexV)->Fill(aSumE,mcweight);
		    m_hVector_multhf[7].at(indexV)->Fill(eventdiff->GetMultiplicityHFPlus(),eventdiff->GetMultiplicityHFMinus(),mcweight);
		    m_hVector_etcalos[7].at(indexV)->Fill(eventdiff->GetSumEnergyHFPlus(),log10(fabs(eventdiff->GetSumETotCastor())),mcweight);
		    m_hVector_tracks[7].at(indexV)->Fill(eventdiff->GetMultiplicityTracks(),mcweight);
		    m_hVector_pfetamax[7].at(indexV)->Fill(eventdiff->GetEtaMaxFromPFCands(),mcweight);
		    m_hVector_pfetamin[7].at(indexV)->Fill(eventdiff->GetEtaMinFromPFCands(),mcweight);
		    m_hVector_vertex[7].at(indexV)->Fill(eventdiff->GetNVertex(),mcweight);
		    m_hVector_deltaphielectrons[7].at(indexV)->Fill(deltaphielectrons,mcweight);
		    m_hVector_deltaphimuons[7].at(indexV)->Fill(deltaphimuons,mcweight);
		    m_hVector_vertexvslumi[7].at(indexV)->Fill(eventdiff->GetNVertex(),eventinfo->GetInstLumiBunch(),mcweight);	       
		  }
		}
	      }
	    }
	  }
	}
      }
    }
  }

  outf->Write();
  outf->Close();

}

#if !defined(__CINT__) || defined(__MAKECINT__)
#include "FWCore/FWLite/interface/AutoLibraryLoader.h"
int main(int argc, char **argv){

  AutoLibraryLoader::enable();

  const char *s1="*";
  std::string filein_;
  std::string ttreename_;  
  std::string savehistofile_;
  double lepton1pt_;
  double lepton2pt_;
  int nVertex_;
  int optTrigger_;
  bool switchPreSel_;
  bool switchTrigger_;
  bool switchPUMultiple_;
  bool switchmcweight_;
  float mcweight_;

  if (argc > 1 && strcmp(s1,argv[1]) != 0)  filein_ = argv[1];
  if (argc > 2 && strcmp(s1,argv[2]) != 0)  ttreename_ = argv[2];
  if (argc > 3 && strcmp(s1,argv[3]) != 0)  savehistofile_  = argv[3];
  if (argc > 4 && strcmp(s1,argv[4]) != 0)  switchTrigger_ = atoi(argv[4]);
  if (argc > 5 && strcmp(s1,argv[5]) != 0)  optTrigger_   = atoi(argv[5]);
  if (argc > 6 && strcmp(s1,argv[6]) != 0)  switchPreSel_ = atoi(argv[6]);
  if (argc > 7 && strcmp(s1,argv[7]) != 0)  lepton1pt_ = atoi(argv[7]);
  if (argc > 8 && strcmp(s1,argv[8]) != 0)  lepton2pt_ = atoi(argv[8]);
  if (argc > 9 && strcmp(s1,argv[9]) != 0)  nVertex_ = atoi(argv[9]);
  if (argc > 10 && strcmp(s1,argv[10]) != 0)  switchPUMultiple_ = atoi(argv[10]);
  if (argc > 11 && strcmp(s1,argv[11]) != 0)  switchmcweight_ = atoi(argv[11]);
  if (argc > 12 && strcmp(s1,argv[12]) != 0)  mcweight_ = atoi(argv[12]);

  DiffractiveZComp* diffZRun = new DiffractiveZComp();   
  diffZRun->Run(filein_, ttreename_, savehistofile_, switchTrigger_, optTrigger_, switchPreSel_, lepton1pt_, lepton2pt_, nVertex_, switchPUMultiple_, switchmcweight_, mcweight_);

  return 0;

}
#endif
