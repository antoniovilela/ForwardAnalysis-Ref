//-------------------------------------------------------------------------------------------------------->>
// UNIVERSIDADE DO ESTADO DO RIO DE JANEIRO - CMS/Brasil
//-------------------------------------------------------------------------------------------------------->>
//
// Twiki: https://twiki.cern.ch/twiki/bin/view/CMS/FwdPhysicsDiffractiveZsAnalysis#Macro_Analysis
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

#include "DiffractiveZ.h"
#include "ForwardAnalysis/ForwardTTreeAnalysis/interface/EventInfoEvent.h"
#include "ForwardAnalysis/ForwardTTreeAnalysis/interface/DiffractiveZEvent.h"
#include "ForwardAnalysis/ForwardTTreeAnalysis/interface/DiffractiveEvent.h"
#include "PhysicsTools/Utilities/interface/LumiReWeighting.h"

using namespace diffractiveAnalysis;
using namespace diffractiveZAnalysis;
using namespace eventInfo;
using namespace reweight;


static inline void loadBar(int x, int n, int r, int w)
{
  // Modified
  // http://www.rosshemsley.co.uk/2011/02/creating-a-progress-bar-in-c-or-any-other-console-app/

  // Only update r times.
  if ( x % (n/r) != 0 ) return;

  // Calculuate the ratio of complete-to-incomplete.
  float ratio = x/(float)n;
  int   c     = ratio * w;

  // Show the percentage complete.
  printf("%3d%%[", (int)(ratio*100) );

  // Show the load bar.
  for (int x=0; x<c; x++)
    printf("=");

  for (int x=c; x<w; x++)
    printf(" ");

  // ANSI Control codes to go back to the
  // previous line and clear it.
  // printf("]\n33[F33[J");

  printf("\r"); // Move to the first column
  fflush(stdout);
}


void DiffractiveZ::LoadFile(std::string fileinput, std::string processinput){

  inf = NULL;
  tr  = NULL;
  inf = TFile::Open(fileinput.c_str(),"read");
  std::string fdirectory = processinput + "/ProcessedTree";
  tr = (TTree*)inf->Get(fdirectory.c_str());
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

void DiffractiveZ::CreateHistos(std::string type){

  std::string step0 = "without_cuts"; 
  std::string step1 = "with_trigger"; 
  std::string step2 = "step1"; 
  std::string step3 = "step2"; 
  std::string step4 = "step3"; 
  std::string step5 = "step4"; 
  std::string step6 = "step5"; 
  std::string step7 = "step6"; 
  std::string step8 = "step7"; 

  Folders.push_back(step0);
  Folders.push_back(step1);
  Folders.push_back(step2);
  Folders.push_back(step3);
  Folders.push_back(step4);
  Folders.push_back(step5);
  Folders.push_back(step6);
  Folders.push_back(step7);
  Folders.push_back(step8);

  int nloop=-999;

  if (type=="multiple_pileup") nloop=21;
  else if (type=="no_multiple_pileup") nloop=1;

  char tag[300];

  for (std::vector<std::string>::size_type j=0; j<Folders.size(); j++){

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
    m_hVector_deltapTelectrons.push_back( std::vector<TH1D*>() );
    m_hVector_deltapTmuons.push_back( std::vector<TH1D*>() );
    m_hVector_deltaetaelectrons.push_back( std::vector<TH1D*>() );
    m_hVector_deltaetamuons.push_back( std::vector<TH1D*>() );
    m_hVector_vertexvslumi.push_back( std::vector<TH2F*>() );

    for (int k=0;k<nloop;k++){

      if (type=="multiple_pileup"){
	sprintf(tag,"multiple_pileup_%i",k);
      }
      else {
	sprintf(tag,"no_multiple_pileup");
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
      sprintf(name35,"deltapTElectrons_%s_%s",tag,Folders.at(j).c_str());
      TH1D *histo_deltapTelectrons = new TH1D(name35,"#Delta pT_{ee} Distribution; #Delta pT_{ee} [GeV.c^{-1}]; N events",50,0.0,150);
      m_hVector_deltapTelectrons[j].push_back(histo_deltapTelectrons);

      char name36[300];
      sprintf(name36,"deltapTMuons_%s_%s",tag,Folders.at(j).c_str());
      TH1D *histo_deltapTmuons = new TH1D(name36,"#Delta pT_{#mu#mu} Distribution; #Delta pT_{#mu#mu} [GeV.c^{-1}]; N events",50,0.0,150);
      m_hVector_deltapTmuons[j].push_back(histo_deltapTmuons);

      char name37[300];
      sprintf(name37,"deltaetaElectrons_%s_%s",tag,Folders.at(j).c_str());
      TH1D *histo_deltaetaelectrons = new TH1D(name37,"#Delta#eta_{ee} Distribution; #Delta#eta_{ee}; N events",50,-11,11);
      m_hVector_deltaetaelectrons[j].push_back(histo_deltaetaelectrons);

      char name38[300];
      sprintf(name38,"deltaetaMuons_%s_%s",tag,Folders.at(j).c_str());
      TH1D *histo_deltaetamuons = new TH1D(name38,"#Delta#eta_{#mu#mu} Distribution; #Delta#eta_{#mu#mu}; N events",50,-11,11);
      m_hVector_deltaetamuons[j].push_back(histo_deltaetamuons);

      char name39[300];
      sprintf(name39,"VertexVsLuminosity_%s_%s",tag,Folders.at(j).c_str());
      TH2F *histo_vertexvslumi = new TH2F(name39,"Vertex vs Luminosity; # Vertex; Luminosity per Bunch [#mub^{-1}s^{-1}]", 25.,  0., 25., 25,  0., 2.);
      m_hVector_vertexvslumi[j].push_back(histo_vertexvslumi);

      char name40[300];
      sprintf(name40,"patNElectron_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_patNElectron = new TH1F(name40,"Number of Electrons; # Electrons; Multiplicity", 100.,  0., 100.);
      m_hVector_patNElectron[j].push_back(histo_patNElectron);

      char name41[300];
      sprintf(name41,"patElectron1Pt_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_patElectron1Pt = new TH1F(name41,"Leading Electron - P_{T} Distribution; P_{T} [GeV.c^{-1}]; N events",200,0,1000);
      m_hVector_patElectron1Pt[j].push_back(histo_patElectron1Pt);

      char name42[300];
      sprintf(name42,"patElectron1Eta_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_patElectron1Eta = new TH1F(name42,"Leading Electron - #eta Distribution; #eta; N events",50,-5.2,5.2);
      m_hVector_patElectron1Eta[j].push_back(histo_patElectron1Eta);

      char name43[300];
      sprintf(name43,"patElectron1Phi_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_patElectron1Phi = new TH1F(name43,"Leading Electron - #phi Distribution; #phi [rad]; N events",50,-3.3,3.3);
      m_hVector_patElectron1Phi[j].push_back(histo_patElectron1Phi);

      char name44[300];
      sprintf(name44,"patElectron1Charge_%s_%s",tag,Folders.at(j).c_str());
      TH1D *histo_patElectron1Charge= new TH1D(name44,"Leading Electron - Charge Distribution; Charge; N events",50,-3,3);
      m_hVector_patElectron1Charge[j].push_back(histo_patElectron1Charge);

      char name45[300];
      sprintf(name45,"patElectron1Et_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_patElectron1Et = new TH1F(name45,"Leading Electron - E_{T} Distribution; E_{T} [GeV]; N events",200,0,1000);
      m_hVector_patElectron1Et[j].push_back(histo_patElectron1Et);

      char name46[300];
      sprintf(name46,"patNMuon_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_patNMuon = new TH1F(name46,"Number of Muons; # Muons; Multiplicity", 100.,  0., 100.);
      m_hVector_patNMuon[j].push_back(histo_patNMuon);

      char name47[300];
      sprintf(name47,"patMuon1Pt_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_patMuon1Pt = new TH1F(name47,"Leading Muon - P_{T} Distribution; P_{T} [GeV.c^{-1}]; N events",200,0,1000);
      m_hVector_patMuon1Pt[j].push_back(histo_patMuon1Pt);

      char name48[300];
      sprintf(name48,"patMuon1Eta_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_patMuon1Eta = new TH1F(name48,"Leading Muon - #eta Distribution; #eta; N events",50,-5.2,5.2);
      m_hVector_patMuon1Eta[j].push_back(histo_patMuon1Eta);

      char name49[300];
      sprintf(name49,"patMuon1Phi_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_patMuon1Phi = new TH1F(name49,"Leading Muon - #phi Distribution; #phi [rad]; N events",50,-3.3,3.3);
      m_hVector_patMuon1Phi[j].push_back(histo_patMuon1Phi);

      char name50[300];
      sprintf(name50,"patMuon1Charge_%s_%s",tag,Folders.at(j).c_str());
      TH1D *histo_patMuon1Charge= new TH1D(name50,"Leading Muon - Charge Distribution; Charge; N events",50,-3,3);
      m_hVector_patMuon1Charge[j].push_back(histo_patMuon1Charge);

      char name51[300];
      sprintf(name51,"patMuon1Et_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_patMuon1Et = new TH1F(name51,"Leading Muon - E_{T} Distribution; E_{T} [GeV]; N events",200,0,1000);
      m_hVector_patMuon1Et[j].push_back(histo_patMuon1Et);

      char name52[300];
      sprintf(name52,"patElectron2Pt_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_patElectron2Pt = new TH1F(name52,"Second Electron - P_{T} Distribution; P_{T} [GeV.c^{-1}]; N events",200,0,1000);
      m_hVector_patElectron2Pt[j].push_back(histo_patElectron2Pt);

      char name53[300];
      sprintf(name53,"patElectron2Eta_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_patElectron2Eta = new TH1F(name53,"Second Electron - #eta Distribution; #eta; N events",60,-5.2,5.2);
      m_hVector_patElectron2Eta[j].push_back(histo_patElectron2Eta);

      char name54[300];
      sprintf(name54,"patElectron2Phi_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_patElectron2Phi = new TH1F(name54,"Second Electron - #phi Distribution; #phi [rad]; N events",60,-3.3,3.3);
      m_hVector_patElectron2Phi[j].push_back(histo_patElectron2Phi);

      char name55[300];
      sprintf(name55,"patElectron2Charge_%s_%s",tag,Folders.at(j).c_str());
      TH1D *histo_patElectron2Charge= new TH1D(name55,"Second Electron - Charge Distribution; Charge; N events",60,-3,3);
      m_hVector_patElectron2Charge[j].push_back(histo_patElectron2Charge);

      char name56[300];
      sprintf(name56,"patElectron2Et_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_patElectron2Et = new TH1F(name56,"Second Electron - E_{T} Distribution; E_{T} [GeV]; N events",200,0,1000);
      m_hVector_patElectron2Et[j].push_back(histo_patElectron2Et);

      char name57[300];
      sprintf(name57,"patMuon2Pt_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_patMuon2Pt = new TH1F(name57,"Second Muon - P_{T} Distribution; P_{T} [GeV.c^{-1}]; N events",200,0,1000);
      m_hVector_patMuon2Pt[j].push_back(histo_patMuon2Pt);

      char name58[300];
      sprintf(name58,"patMuon2Eta_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_patMuon2Eta = new TH1F(name58,"Second Muon - #eta Distribution; #eta; N events",60,-5.2,5.2);
      m_hVector_patMuon2Eta[j].push_back(histo_patMuon2Eta);

      char name59[300];
      sprintf(name59,"patMuon2Phi_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_patMuon2Phi = new TH1F(name59,"Second Muon - #phi Distribution; #phi [rad]; N events",60,-3.3,3.3);
      m_hVector_patMuon2Phi[j].push_back(histo_patMuon2Phi);

      char name60[300];
      sprintf(name60,"patMuon2Charge_%s_%s",tag,Folders.at(j).c_str());
      TH1D *histo_patMuon2Charge= new TH1D(name60,"Second Muon - Charge Distribution; Charge; N events",60,-3,3);
      m_hVector_patMuon2Charge[j].push_back(histo_patMuon2Charge);

      char name61[300];
      sprintf(name61,"patMuon2Et_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_patMuon2Et = new TH1F(name61,"Second Muon - E_{T} Distribution; E_{T} [GeV]; N events",200,0,1000);
      m_hVector_patMuon2Et[j].push_back(histo_patMuon2Et);

      char name62[300];
      sprintf(name62,"patDiElectronMass_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_patDiElectronMass = new TH1F(name62,"Dielectron Invariant Mass Distribution; M_{ee} [GeV]; N events",500,0,500);
      m_hVector_patDiElectronMass[j].push_back(histo_patDiElectronMass);

      char name63[300];
      sprintf(name63,"patDiMuonMass_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_patDiMuonMass = new TH1F(name63,"DiMuon Invariant Mass Distribution; M_{#mu#mu} [GeV]; N events",500,0,500);
      m_hVector_patDiMuonMass[j].push_back(histo_patDiMuonMass);

      char name64[300];
      sprintf(name64,"patElectron1TkDr03_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_patElectron1TkDr03 = new TH1F(name64,"Leading Electron: Tracker Isolation DR03; # Isolation; [u]", 100.,  0., 1.);
      m_hVector_patElectron1TkDr03[j].push_back(histo_patElectron1TkDr03);

      char name65[300];
      sprintf(name65,"patElectron1TkDr04_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_patElectron1TkDr04 = new TH1F(name65,"Leading Electron: Tracker Isolation DR04; # Isolation; [u]", 100.,  0., 1.);
      m_hVector_patElectron1TkDr04[j].push_back(histo_patElectron1TkDr04);

      char name66[300];
      sprintf(name66,"patElectron1EcalDr03_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_patElectron1EcalDr03 = new TH1F(name66,"Leading Electron: ECAL Isolation DR03; # Isolation; [u]", 100.,  0., 1.);
      m_hVector_patElectron1EcalDr03[j].push_back(histo_patElectron1EcalDr03);

      char name67[300];
      sprintf(name67,"patElectron1EcalDr04_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_patElectron1EcalDr04 = new TH1F(name67,"Leading Electron: ECAL Isolation DR04; # Isolation; [u]", 100.,  0., 1.);
      m_hVector_patElectron1EcalDr04[j].push_back(histo_patElectron1EcalDr04);

      char name68[300];
      sprintf(name68,"patElectron1HcalDr03_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_patElectron1HcalDr03 = new TH1F(name68,"Leading Electron: HCAL Isolation DR03; # Isolation; [u]", 100.,  0., 1.);
      m_hVector_patElectron1HcalDr03[j].push_back(histo_patElectron1HcalDr03);

      char name69[300];
      sprintf(name69,"patElectron1HcalDr04_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_patElectron1HcalDr04 = new TH1F(name69,"Leading Electron: HCAL Isolation DR04; # Isolation; [u]", 100.,  0., 1.);
      m_hVector_patElectron1HcalDr04[j].push_back(histo_patElectron1HcalDr04);

      char name70[300];
      sprintf(name70,"patElectron2TkDr03_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_patElectron2TkDr03 = new TH1F(name70,"Second Electron: Tracker Isolation DR03; # Isolation; [u]", 100.,  0., 1.);
      m_hVector_patElectron2TkDr03[j].push_back(histo_patElectron2TkDr03);

      char name71[300];
      sprintf(name71,"patElectron2TkDr04_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_patElectron2TkDr04 = new TH1F(name71,"Second Electron: Tracker Isolation DR04; # Isolation; [u]", 100.,  0., 1.);
      m_hVector_patElectron2TkDr04[j].push_back(histo_patElectron2TkDr04);

      char name72[300];
      sprintf(name72,"patElectron2EcalDr03_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_patElectron2EcalDr03 = new TH1F(name72,"Second Electron: ECAL Isolation DR03; # Isolation; [u]", 100.,  0., 1.);
      m_hVector_patElectron2EcalDr03[j].push_back(histo_patElectron2EcalDr03);

      char name73[300];
      sprintf(name73,"patElectron2EcalDr04_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_patElectron2EcalDr04 = new TH1F(name73,"Second Electron: ECAL Isolation DR04; # Isolation; [u]", 100.,  0., 1.);
      m_hVector_patElectron2EcalDr04[j].push_back(histo_patElectron2EcalDr04);

      char name74[300];
      sprintf(name74,"patElectron2HcalDr03_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_patElectron2HcalDr03 = new TH1F(name74,"Second Electron: HCAL Isolation DR03; # Isolation; [u]", 100.,  0., 1.);
      m_hVector_patElectron2HcalDr03[j].push_back(histo_patElectron2HcalDr03);

      char name75[300];
      sprintf(name75,"patElectron2HcalDr04_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_patElectron2HcalDr04 = new TH1F(name75,"Second Electron: HCAL Isolation DR04; # Isolation; [u]", 100.,  0., 1.);
      m_hVector_patElectron2HcalDr04[j].push_back(histo_patElectron2HcalDr04);

      char name76[300];
      sprintf(name76,"patElectron1relIsoDr03_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_patElectron1relIsoDr03 = new TH1F(name76,"Leading Electron: Isolation DR03; # Isolation; [u]", 100.,  0., 1.);
      m_hVector_patElectron1relIsoDr03[j].push_back(histo_patElectron1relIsoDr03);

      char name77[300];
      sprintf(name77,"patElectron1relIsoDr04_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_patElectron1relIsoDr04 = new TH1F(name77,"Leading Electron: Isolation DR04; # Isolation; [u]", 100.,  0., 1.);
      m_hVector_patElectron1relIsoDr04[j].push_back(histo_patElectron1relIsoDr04);

      char name78[300];
      sprintf(name78,"patElectron2relIsoDr03_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_patElectron2relIsoDr03 = new TH1F(name78,"Second Electron: Isolation DR03; # Isolation; [u]", 100.,  0., 1.);
      m_hVector_patElectron2relIsoDr03[j].push_back(histo_patElectron2relIsoDr03);

      char name79[300];
      sprintf(name79,"patElectron2relIsoDr04_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_patElectron2relIsoDr04 = new TH1F(name79,"Second Electron: Isolation DR04; # Isolation; [u]", 100.,  0., 1.);
      m_hVector_patElectron2relIsoDr04[j].push_back(histo_patElectron2relIsoDr04);

     char name80[300];
      sprintf(name80,"patMuon1SumPtR03_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_patMuon1SumPtR03 = new TH1F(name80,"Leading Muon: Tracker Isolation DR03; # Isolation; [u]", 100.,  0., 1.);
      m_hVector_patMuon1SumPtR03[j].push_back(histo_patMuon1SumPtR03);

      char name81[300];
      sprintf(name81,"patMuon1SumPtR05_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_patMuon1SumPtR05 = new TH1F(name81,"Leading Muon: Tracker Isolation DR05; # Isolation; [u]", 100.,  0., 1.);
      m_hVector_patMuon1SumPtR05[j].push_back(histo_patMuon1SumPtR05);

      char name82[300];
      sprintf(name82,"patMuon1EmEtR03_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_patMuon1EmEtR03 = new TH1F(name82,"Leading Muon: ECAL Isolation DR03; # Isolation; [u]", 100.,  0., 1.);
      m_hVector_patMuon1EmEtR03[j].push_back(histo_patMuon1EmEtR03);

      char name83[300];
      sprintf(name83,"patMuon1EmEtR05_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_patMuon1EmEtR05 = new TH1F(name83,"Leading Muon: ECAL Isolation DR05; # Isolation; [u]", 100.,  0., 1.);
      m_hVector_patMuon1EmEtR05[j].push_back(histo_patMuon1EmEtR05);

      char name84[300];
      sprintf(name84,"patMuon1HadEtR03_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_patMuon1HadEtR03 = new TH1F(name84,"Leading Muon: HCAL Isolation DR03; # Isolation; [u]", 100.,  0., 1.);
      m_hVector_patMuon1HadEtR03[j].push_back(histo_patMuon1HadEtR03);

      char name85[300];
      sprintf(name85,"patMuon1HadEtR05_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_patMuon1HadEtR05 = new TH1F(name85,"Leading Muon: HCAL Isolation DR05; # Isolation; [u]", 100.,  0., 1.);
      m_hVector_patMuon1HadEtR05[j].push_back(histo_patMuon1HadEtR05);

      char name86[300];
      sprintf(name86,"patMuon2SumPtR03_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_patMuon2SumPtR03 = new TH1F(name86,"Second Muon: Tracker Isolation DR03; # Isolation; [u]", 100.,  0., 1.);
      m_hVector_patMuon2SumPtR03[j].push_back(histo_patMuon2SumPtR03);

      char name87[300];
      sprintf(name87,"patMuon2SumPtR05_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_patMuon2SumPtR05 = new TH1F(name87,"Second Muon: Tracker Isolation DR05; # Isolation; [u]", 100.,  0., 1.);
      m_hVector_patMuon2SumPtR05[j].push_back(histo_patMuon2SumPtR05);

      char name88[300];
      sprintf(name88,"patMuon2EmEtR03_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_patMuon2EmEtR03 = new TH1F(name88,"Second Muon: ECAL Isolation DR03; # Isolation; [u]", 100.,  0., 1.);
      m_hVector_patMuon2EmEtR03[j].push_back(histo_patMuon2EmEtR03);

      char name89[300];
      sprintf(name89,"patMuon2EmEtR05_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_patMuon2EmEtR05 = new TH1F(name89,"Second Muon: ECAL Isolation DR05; # Isolation; [u]", 100.,  0., 1.);
      m_hVector_patMuon2EmEtR05[j].push_back(histo_patMuon2EmEtR05);

      char name90[300];
      sprintf(name90,"patMuon2HadEtR03_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_patMuon2HadEtR03 = new TH1F(name90,"Second Muon: HCAL Isolation DR03; # Isolation; [u]", 100.,  0., 1.);
      m_hVector_patMuon2HadEtR03[j].push_back(histo_patMuon2HadEtR03);

      char name91[300];
      sprintf(name91,"patMuon2HadEtR05_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_patMuon2HadEtR05 = new TH1F(name91,"Second Muon: HCAL Isolation DR05; # Isolation; [u]", 100.,  0., 1.);
      m_hVector_patMuon2HadEtR05[j].push_back(histo_patMuon2HadEtR05);

      char name92[300];
      sprintf(name92,"patMuon1relIsoDr03_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_patMuon1relIsoDr03 = new TH1F(name92,"Leading Muon: Isolation DR03; # Isolation; [u]", 100.,  0., 1.);
      m_hVector_patMuon1relIsoDr03[j].push_back(histo_patMuon1relIsoDr03);

      char name93[300];
      sprintf(name93,"patMuon1relIsoDr05_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_patMuon1relIsoDr05 = new TH1F(name93,"Leading Muon: Isolation DR05; # Isolation; [u]", 100.,  0., 1.);
      m_hVector_patMuon1relIsoDr05[j].push_back(histo_patMuon1relIsoDr05);

      char name94[300];
      sprintf(name94,"patMuon2relIsoDr03_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_patMuon2relIsoDr03 = new TH1F(name94,"Second Muon: Isolation DR03; # Isolation; [u]", 100.,  0., 1.);
      m_hVector_patMuon2relIsoDr03[j].push_back(histo_patMuon2relIsoDr03);

      char name95[300];
      sprintf(name95,"patMuon2relIsoDr05_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_patMuon2relIsoDr05 = new TH1F(name95,"Second Muon: Isolation DR05; # Isolation; [u]", 100.,  0., 1.);
      m_hVector_patMuon2relIsoDr05[j].push_back(histo_patMuon2relIsoDr05);

      char name96[300];
      sprintf(name96,"patMuon1relIso_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_patMuon1relIso = new TH1F(name96,"Leading Muon: Isolation; # Isolation; [u]", 100.,  0., 1.);
      m_hVector_patMuon1relIso[j].push_back(histo_patMuon1relIso);

      char name97[300];
      sprintf(name97,"patMuon2relIsoDr05_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_patMuon2relIso = new TH1F(name97,"Second Muon: Isolation; # Isolation; [u]", 100.,  0., 1.);
      m_hVector_patMuon2relIso[j].push_back(histo_patMuon2relIso);

    }
  }
}

void DiffractiveZ::FillHistos(int index, int pileup, double totalweight){
  m_hVector_DiElectron[index].at(pileup)->Fill(eventdiffZ->GetDiElectronMass(),totalweight);
  m_hVector_LeadingElectronPt[index].at(pileup)->Fill(eventdiffZ->GetLeadingElectronPt(),totalweight);
  m_hVector_LeadingElectronEta[index].at(pileup)->Fill(eventdiffZ->GetLeadingElectronEta(),totalweight);
  m_hVector_LeadingElectronPhi[index].at(pileup)->Fill(eventdiffZ->GetLeadingElectronPhi(),totalweight);
  m_hVector_LeadingElectronCharge[index].at(pileup)->Fill(eventdiffZ->GetLeadingElectronCharge(),totalweight);
  m_hVector_SecondElectronPt[index].at(pileup)->Fill(eventdiffZ->GetSecondElectronPt(),totalweight);
  m_hVector_SecondElectronEta[index].at(pileup)->Fill(eventdiffZ->GetSecondElectronEta(),totalweight);
  m_hVector_SecondElectronPhi[index].at(pileup)->Fill(eventdiffZ->GetSecondElectronPhi(),totalweight);
  m_hVector_SecondElectronCharge[index].at(pileup)->Fill(eventdiffZ->GetSecondElectronCharge(),totalweight);
  m_hVector_ElectronsN[index].at(pileup)->Fill(eventdiffZ->GetElectronsN(),totalweight);
  m_hVector_DiMuon[index].at(pileup)->Fill(eventdiffZ->GetDiMuonMass(),totalweight);
  m_hVector_LeadingMuonPt[index].at(pileup)->Fill(eventdiffZ->GetLeadingMuonPt(),totalweight);
  m_hVector_LeadingMuonEta[index].at(pileup)->Fill(eventdiffZ->GetLeadingMuonEta(),totalweight);
  m_hVector_LeadingMuonPhi[index].at(pileup)->Fill(eventdiffZ->GetLeadingMuonPhi(),totalweight);
  m_hVector_LeadingMuonCharge[index].at(pileup)->Fill(eventdiffZ->GetLeadingMuonCharge(),totalweight);
  m_hVector_SecondMuonPt[index].at(pileup)->Fill(eventdiffZ->GetSecondMuonPt(),totalweight);
  m_hVector_SecondMuonEta[index].at(pileup)->Fill(eventdiffZ->GetSecondMuonEta(),totalweight);
  m_hVector_SecondMuonPhi[index].at(pileup)->Fill(eventdiffZ->GetSecondMuonPhi(),totalweight);
  m_hVector_SecondMuonCharge[index].at(pileup)->Fill(eventdiffZ->GetSecondMuonCharge(),totalweight);
  m_hVector_MuonsN[index].at(pileup)->Fill(eventdiffZ->GetMuonsN(),totalweight);
  m_hVector_sumEHFplus[index].at(pileup)->Fill(eventdiff->GetSumEnergyHFPlus(),totalweight);
  m_hVector_sumEHFminus[index].at(pileup)->Fill(eventdiff->GetSumEnergyHFMinus(),totalweight);
  m_hVector_sumEHEplus[index].at(pileup)->Fill(eventdiff->GetSumEnergyHEPlus(),totalweight);
  m_hVector_sumEHEminus[index].at(pileup)->Fill(eventdiff->GetSumEnergyHEMinus(),totalweight);
  m_hVector_lumi[index].at(pileup)->Fill(eventinfo->GetInstLumiBunch(),totalweight);
  m_hVector_asumE[index].at(pileup)->Fill(aSumE,totalweight);
  m_hVector_multhf[index].at(pileup)->Fill(eventdiff->GetMultiplicityHFPlus(),eventdiff->GetMultiplicityHFMinus(),totalweight);
  m_hVector_etcalos[index].at(pileup)->Fill(eventdiff->GetSumEnergyHFPlus(),log10(fabs(eventdiff->GetSumETotCastor())),totalweight);
  m_hVector_tracks[index].at(pileup)->Fill(eventdiff->GetMultiplicityTracks(),totalweight);
  m_hVector_pfetamax[index].at(pileup)->Fill(eventdiff->GetEtaMaxFromPFCands(),totalweight);
  m_hVector_pfetamin[index].at(pileup)->Fill(eventdiff->GetEtaMinFromPFCands(),totalweight);
  m_hVector_vertex[index].at(pileup)->Fill(eventdiff->GetNVertex(),totalweight);
  m_hVector_deltaphielectrons[index].at(pileup)->Fill(deltaphielectrons,totalweight);
  m_hVector_deltaphimuons[index].at(pileup)->Fill(deltaphimuons,totalweight);
  m_hVector_deltaetaelectrons[index].at(pileup)->Fill(deltaetaelectrons,totalweight);
  m_hVector_deltaetamuons[index].at(pileup)->Fill(deltaetamuons,totalweight);
  m_hVector_deltapTelectrons[index].at(pileup)->Fill(deltapTelectrons,totalweight);
  m_hVector_deltapTmuons[index].at(pileup)->Fill(deltapTmuons,totalweight);
  m_hVector_vertexvslumi[index].at(pileup)->Fill(eventdiff->GetNVertex(),eventinfo->GetInstLumiBunch(),totalweight);
}

void DiffractiveZ::SaveHistos(std::string type){

  int ipileup;

  if (type=="multiple_pileup") ipileup=21;
  else ipileup=1;

  for (int i = 0; i < ipileup; i++){
    for (std::vector<std::string>::size_type j=0; j<Folders.size(); j++){

      m_hVector_DiElectron[j].at(i)->Write();
      m_hVector_LeadingElectronPt[j].at(i)->Write();
      m_hVector_LeadingElectronEta[j].at(i)->Write();
      m_hVector_LeadingElectronPhi[j].at(i)->Write();
      m_hVector_LeadingElectronCharge[j].at(i)->Write();
      m_hVector_SecondElectronPt[j].at(i)->Write();
      m_hVector_SecondElectronEta[j].at(i)->Write();
      m_hVector_SecondElectronPhi[j].at(i)->Write();
      m_hVector_SecondElectronCharge[j].at(i)->Write();
      m_hVector_ElectronsN[j].at(i)->Write();
      m_hVector_DiMuon[j].at(i)->Write();
      m_hVector_LeadingMuonPt[j].at(i)->Write();
      m_hVector_LeadingMuonEta[j].at(i)->Write();
      m_hVector_LeadingMuonPhi[j].at(i)->Write();
      m_hVector_LeadingMuonCharge[j].at(i)->Write();
      m_hVector_SecondMuonPt[j].at(i)->Write();
      m_hVector_SecondMuonEta[j].at(i)->Write();
      m_hVector_SecondMuonPhi[j].at(i)->Write();
      m_hVector_SecondMuonCharge[j].at(i)->Write();
      m_hVector_MuonsN[j].at(i)->Write();
      m_hVector_sumEHFplus[j].at(i)->Write();
      m_hVector_sumEHFminus[j].at(i)->Write();
      m_hVector_sumEHEplus[j].at(i)->Write();
      m_hVector_sumEHEminus[j].at(i)->Write();
      m_hVector_lumi[j].at(i)->Write();
      m_hVector_asumE[j].at(i)->Write();
      m_hVector_multhf[j].at(i)->Write();
      m_hVector_etcalos[j].at(i)->Write();
      m_hVector_tracks[j].at(i)->Write();
      m_hVector_pfetamax[j].at(i)->Write();
      m_hVector_pfetamin[j].at(i)->Write();
      m_hVector_vertex[j].at(i)->Write();
      m_hVector_deltaphielectrons[j].at(i)->Write();
      m_hVector_deltaphimuons[j].at(i)->Write();
      m_hVector_deltaetaelectrons[j].at(i)->Write();
      m_hVector_deltaetamuons[j].at(i)->Write();
      m_hVector_deltapTelectrons[j].at(i)->Write();
      m_hVector_deltapTmuons[j].at(i)->Write();
      m_hVector_vertexvslumi[j].at(i)->Write();

    }
  }

}

void DiffractiveZ::Run(std::string filein_, std::string processname_, std::string savehistofile_, std::string switchtrigger_, int optTrigger_, double lepton1pt_, double lepton2pt_, int nVertex_, std::string type_, std::string switchlumiweight_, float mcweight_){

  bool debug = false;

  TH1::SetDefaultSumw2(true);
  TH2::SetDefaultSumw2(true);

  filein = filein_;
  savehistofile = savehistofile_;
  processname = processname_;
  type = type_;
  switchtrigger = switchtrigger_;
  switchlumiweight = switchlumiweight_;
  mcweight = mcweight_;
  nVertex = nVertex_;
  optTrigger = optTrigger_;
  lepton1pt = lepton1pt_;
  lepton2pt = lepton2pt_;

  TFile check1(filein.c_str());

  if (check1.IsZombie()){

    std::cout << "------------------------------------------------" << std::endl;
    std::cout << " There is no the file " << filein << " or the"   << std::endl;
    std::cout << " path is not correct." << std::endl;
    std::cout << "------------------------------------------------" << std::endl;
    return;

  }

  if (check1.GetDirectory(processname.c_str())){
    LoadFile(filein,processname);
  }

  else {
    std::cout << "---------------------------------------------------" << std::endl;
    std::cout << " There is no directory/path " << processname << std::endl;
    std::cout << " in the file." << std::endl;
    std::cout << "---------------------------------------------------" << std::endl;
    return;
  }

  TFile *outf = new TFile(savehistofile.c_str(),"RECREATE");
  TString outtxt = savehistofile;
  outtxt.ReplaceAll("root","txt");
  std::ofstream outstring(outtxt);

  outstring << "" << std::endl;
  outstring << "<< Gold Events >>" << std::endl;
  outstring << "" << std::endl;
  outstring << "Please, insert this events in another text file to be used by PickEvent Tool. " << std::endl;
  outstring << "Twiki: https://twiki.cern.ch/twiki/bin/view/CMSPublic/WorkBookPickEvents " << std::endl;
  outstring << ">>---------------------------------------------------------------------- " << std::endl;

  int NEVENTS = tr->GetEntries();
  int pileup = -999;
  int triggercounter[20]={0};

  double aSumE = -999.;
  double deltaphielectrons = -999.;
  double deltaphimuons = -999.;
  double deltaetaelectrons = -999;
  double deltaetamuons = -999;
  double deltapTelectrons = -999;
  double deltapTmuons = -999;   

  TH1::SetDefaultSumw2(true);
  TH2::SetDefaultSumw2(true);

  unsigned NEntries = tr->GetEntries();
  std::cout << "" << std::endl;
  std::cout<< "Reading Tree: "<< NEntries << " events"<<std::endl;
  std::cout << "" << std::endl;

  std::string status;  

  std::cout << "" << std::endl;
  std::cout<< "Status Bar" << std::endl;
  std::cout << "" << std::endl;


  for(int i=0;i<NEVENTS;i++){

    if (!debug) {
      loadBar(i,NEVENTS,100,100);
    }

    tr->GetEntry(i);

    if ( type=="multiple_pileup" && (eventinfo->GetNPileUpBx0()==-1 && eventinfo->GetNPileUpBxm1()==-1 && eventinfo->GetNPileUpBxp1()==-1 )){
      std::cout << " " << std::endl; 
      std::cout << "--------------------------------------------------------------" << std::endl;
      std::cout << " There is no pile-up TTree information in your PATTuplefile."   << std::endl;
      std::cout << " Please, use another PATTuple with PU information to run mul- " << std::endl;
      std::cout << " tiple PU option." << std::endl;
      std::cout << "--------------------------------------------------------------" << std::endl;
      return;
    }

    if (type=="multiple_pileup"){
      pileup = eventinfo->GetNPileUpBx0();
    }
    else{
      pileup = 0;
    }

    for (int nt=0;nt<20;nt++){
      if(eventdiffZ->GetHLTPath(nt)){
	triggercounter[nt]++;
      }
    }

    aSumE = (eventdiff->GetSumEnergyHFPlus() - eventdiff->GetSumEnergyHFMinus())/(eventdiff->GetSumEnergyHFPlus() + eventdiff->GetSumEnergyHFMinus());
    deltaphielectrons = fabs(eventdiffZ->GetLeadingElectronPhi() - eventdiffZ->GetSecondElectronPhi());
    deltaphimuons = fabs(eventdiffZ->GetLeadingMuonPhi() - eventdiffZ->GetSecondMuonPhi());
    deltaetaelectrons = eventdiffZ->GetLeadingElectronEta() - eventdiffZ->GetSecondElectronEta();
    deltaetamuons = eventdiffZ->GetLeadingMuonEta() - eventdiffZ->GetSecondMuonEta();
    deltapTelectrons = eventdiffZ->GetLeadingElectronPt() - eventdiffZ->GetSecondElectronPt();
    deltapTmuons = eventdiffZ->GetLeadingMuonPt() - eventdiffZ->GetSecondMuonPt();

    float totalcommon = 1.;
    float mclumiweight = 1.;

    if (switchlumiweight == "mc_lumi_weight"){
      mclumiweight = mcweight;
    }

    if(switchtrigger == "trigger" || switchtrigger == "no_trigger") {}
    else{
      std::cout << " " << std::endl;
      std::cout << "\nPlease Insert type of selection: " << std::endl;
      std::cout << "1) trigger: with trigger. If PATTuple has trigger." << std::endl;
      std::cout << "2) no_trigger: without trigger. If PATTuple has not trigger." << std::endl;
      exit(EXIT_FAILURE);
    }

    totalcommon = mclumiweight;

    if (debug){
      if( i % 1000 == 0 ){
	std::cout << "\nEvent " << i << std::endl;
	std::cout << "Nr. events Bx 0: " << eventinfo->GetNPileUpBx0() << std::endl;
	std::cout << "Lumi per Bunch: " << eventinfo->GetInstLumiBunch() << std::endl;
	std::cout << "Luminosity weight (a): " << mclumiweight <<std::endl;
      }
    }

    bool trigger = false;
    bool vertex = false;
    bool diffsel = false;

    bool presel_el = false;
    bool charge_el = false;
    bool dimass_el = false;
    bool n_el = false;

    bool presel_muon = false;
    bool charge_muon = false;
    bool dimass_muon = false;
    bool n_muon = false;

    bool pat_presel_el = false;
    bool pat_charge_el = false;
    bool pat_dimass_el = false;
    bool pat_n_el = false;

    bool pat_presel_muon = false;
    bool pat_charge_muon = false;
    bool pat_dimass_muon = false;
    bool pat_n_muon = false;

    if (eventdiffZ->GetHLTPath(optTrigger)) trigger = true;
    if (eventdiff->GetNVertex() > 0 && eventdiff->GetNVertex()<= nVertex) vertex = true;
    if ((eventdiff->GetEtaMaxFromPFCands() < 3.) || (eventdiff->GetEtaMinFromPFCands() > -3.)) diffsel = true;

    if (eventdiffZ->GetLeadingElectronPt() > lepton1pt && eventdiffZ->GetSecondElectronPt() > lepton2pt) presel_el = true;
    if (eventdiffZ->GetLeadingElectronCharge()*eventdiffZ->GetSecondElectronCharge()==-1) charge_el = true;
    if (eventdiffZ->GetDiElectronMass() > 60. && eventdiffZ->GetDiElectronMass() < 110.) dimass_el = true;
    if (eventdiffZ->GetElectronsN() == 2) n_el = true; 

    if (eventdiffZ->GetLeadingMuonPt() > lepton1pt && eventdiffZ->GetSecondMuonPt() > lepton2pt) presel_muon = true;
    if (eventdiffZ->GetLeadingMuonCharge()*eventdiffZ->GetSecondMuonCharge()==-1) charge_muon = true;
    if (eventdiffZ->GetDiMuonMass() > 60. && eventdiffZ->GetDiMuonMass() < 110.) dimass_muon = true;
    if (eventdiffZ->GetMuonsN() == 2) n_muon = true;                                                                             
    if (eventdiffZ->GetPatElectron1Pt() > lepton1pt && eventdiffZ->GetPatElectron2Pt() > lepton2pt) pat_presel_el = true;
    if (eventdiffZ->GetPatElectron1Charge()*eventdiffZ->GetPatElectron2Charge()==-1) pat_charge_el = true;
    if (eventdiffZ->GetPatDiElectronMass() > 60. && eventdiffZ->GetPatDiElectronMass() < 110.) pat_dimass_el = true;
    if (eventdiffZ->GetPatNElectron() == 2) pat_n_El = true;                                                                             
    if (eventdiffZ->GetPatMuon1Pt() > lepton1pt && eventdiffZ->GetPatMuon2Pt() > lepton2pt) pat_presel_muon = true;
    if (eventdiffZ->GetPatMuon1Charge()*eventdiffZ->GetPatMuon2Charge()==-1) pat_charge_muon = true;
    if (eventdiffZ->GetPatDiMuonMass() > 60. && eventdiffZ->GetPatDiMuonMass() < 110.) pat_dimass_muon = true;
    if (eventdiffZ->GetPatNMuon() == 2) pat_n_muon = true; 

    if(pileup < 21){ // Never comment this line. It is the program defense.

      if(switchtrigger == "trigger"){ 
	FillHistos(0,pileup,totalcommon); 
	if(trigger) {
	  FillHistos(1,pileup,totalcommon);
	  outstring << eventdiff->GetRunNumber() << ":" << eventdiff->GetLumiSection() << ":" << eventdiff->GetEventNumber() << std::endl;
	}
      }

      else if (switchtrigger =="no_trigger"){
	FillHistos(0,pileup,totalcommon);
	if(presel_el) FillHistos(2,pileup,totalcommon);
      }

      else {
	exit(EXIT_FAILURE);
      }
    }   
  }

  outstring << "" << std::endl;
  outstring << "<< INPUTS >>" << std::endl;
  outstring << " " << std::endl;
  outstring << ">> Input file: " << filein << std::endl;
  outstring << ">> Output file: " << savehistofile << std::endl;
  outstring << ">> TTree Name: " << processname << std::endl;
  outstring << " " << std::endl;
  outstring << "<< OPTIONS >>" << std::endl; 
  outstring << " " << std::endl;
  outstring << ">> Trigger: " << switchtrigger << " | Option: " << optTrigger << std::endl;
  outstring << ">> # Vertex: " << nVertex << std::endl;
  outstring << ">> Lumi. Weight: " << switchlumiweight << " | Weight: " << mcweight << std::endl;
  outstring << ">> Lepton1(pT) > " << lepton1pt <<std::endl;
  outstring << ">> Lepton2(pT) > " << lepton2pt <<std::endl;
  outstring << " " << std::endl;
  outstring << "<< TRIGGER >> " << std::endl;
  outstring << " " << std::endl;
  outstring << "Total Trigger Fired: " <<  std::endl;
  outstring << "Trigger 0: " << triggercounter[0] << std::endl;
  outstring << "Trigger 1: " << triggercounter[1] << std::endl;
  outstring << "Trigger 2: " << triggercounter[2] << std::endl;
  outstring << "Trigger 3: " << triggercounter[3] << std::endl;
  outstring << "Trigger 4: " << triggercounter[4] << std::endl;
  outstring << "Trigger 5: " << triggercounter[5] << std::endl;
  outstring << "Trigger 6: " << triggercounter[6] << std::endl;
  outstring << "Trigger 7: " << triggercounter[7] << std::endl;
  outstring << "Trigger 8: " << triggercounter[8] << std::endl;
  outstring << "Trigger 9: " << triggercounter[9] << std::endl;
  outstring << "Trigger 10: " << triggercounter[10] << std::endl;
  outstring << "Trigger 11: " << triggercounter[11] << std::endl;
  outstring << "Trigger 12: " << triggercounter[12] << std::endl;
  outstring << "Trigger 13: " << triggercounter[13] << std::endl;
  outstring << "Trigger 14: " << triggercounter[14] << std::endl;
  outstring << "Trigger 15: " << triggercounter[15] << std::endl;
  outstring << "Trigger 16: " << triggercounter[16] << std::endl;
  outstring << "Trigger 17: " << triggercounter[17] << std::endl;
  outstring << "Trigger 18: " << triggercounter[18] << std::endl;
  outstring << "Trigger 19: " << triggercounter[19] << std::endl;
  outstring << "" << std::endl;

  outf->cd();
  SaveHistos(type);
  outf->Close();

}

#if !defined(__CINT__) || defined(__MAKECINT__)
#include "FWCore/FWLite/interface/AutoLibraryLoader.h"
int main(int argc, char **argv)
{

  AutoLibraryLoader::enable();

  const char *s1="*";
  std::string filein_;
  std::string processname_;
  std::string savehistofile_;
  double lepton1pt_;
  double lepton2pt_;
  int nVertex_;
  int optTrigger_;
  std::string switchtrigger_;
  float mcweight_;
  std::string switchlumiweight_;
  std::string type_;

  if (argc > 1 && strcmp(s1,argv[1]) != 0)  filein_ = argv[1];
  if (argc > 2 && strcmp(s1,argv[2]) != 0)  processname_ = argv[2];
  if (argc > 3 && strcmp(s1,argv[3]) != 0)  savehistofile_  = argv[3];
  if (argc > 4 && strcmp(s1,argv[4]) != 0)  switchtrigger_ = argv[4];
  if (argc > 5 && strcmp(s1,argv[5]) != 0)  optTrigger_   = atoi(argv[5]);
  if (argc > 6 && strcmp(s1,argv[6]) != 0)  lepton1pt_ = atoi(argv[6]);
  if (argc > 7 && strcmp(s1,argv[7]) != 0)  lepton2pt_ = atoi(argv[7]);
  if (argc > 8 && strcmp(s1,argv[8]) != 0)  nVertex_ = atoi(argv[8]);
  if (argc > 9 && strcmp(s1,argv[9]) != 0)  type_ = argv[9];
  if (argc > 10 && strcmp(s1,argv[10]) != 0) switchlumiweight_ = argv[10];
  if (argc > 11 && strcmp(s1,argv[11]) != 0)  mcweight_ = atoi(argv[11]);

  if (type_=="multiple_pileup" || type_=="no_multiple_pileup") {

    if (nVertex_ <= 0 || optTrigger_ < 0 || mcweight_ <= 0 || lepton1pt_ < 0 || lepton2pt_ < 0 ){
      std::cout << "----------------------------------------------" << std::endl;
      std::cout << " Pay attention on the input numbers parameters" << std::endl;
      std::cout << "----------------------------------------------" << std::endl;
      std::cout << ">> Requirements:                             " << std::endl;
      std::cout << "I)   nVertex_ > 0 " << std::endl;
      std::cout << "II)  optTrigger >= 0" << std::endl;
      std::cout << "III) mcweight_ > 0" << std::endl;
      std::cout << "IV)  Lepton1pt_ and Lepton2pt_ >= 0" << std::endl;  
      std::cout << "----------------------------------------------" << std::endl; 
      return 0;
    }

    DiffractiveZ* diffZRun = new DiffractiveZ();
    diffZRun->CreateHistos(type_);
    diffZRun->Run(filein_, processname_, savehistofile_, switchtrigger_, optTrigger_, lepton1pt_, lepton2pt_, nVertex_, type_, switchlumiweight_, mcweight_);
    return 0;
  }

  else{
    std::cout << "Please Insert type of histograms: " << std::endl;
    std::cout << "1) multiple_pileup: create histograms for each pile-up event. It works only for MC with pile-up." << std::endl;
    std::cout << "2) no_multiple_pileup: create histograms without each pile-up event. It works for data and MC with/without pile-up." << std::endl;
    return 0;
  }
}
#endif
