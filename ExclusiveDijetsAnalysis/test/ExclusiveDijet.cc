//-------------------------------------------------------------------------------------------------------->>
// UNIVERSIDADE DO ESTADO DO RIO DE JANEIRO - CMS/Brasil
//-------------------------------------------------------------------------------------------------------->>
//
// Twiki: https://twiki.cern.ch/twiki/bin/view/CMS/FwdPhysicsExclusiveDijetsAnalysis#Macro_Analysis
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

#include "ExclusiveDijet.h"
#include "ForwardAnalysis/ForwardTTreeAnalysis/interface/EventInfoEvent.h"
#include "ForwardAnalysis/ForwardTTreeAnalysis/interface/ExclusiveDijetsEvent.h"
#include "ForwardAnalysis/ForwardTTreeAnalysis/interface/DiffractiveEvent.h"
#include "PhysicsTools/Utilities/interface/LumiReWeighting.h"

using namespace diffractiveAnalysis;
using namespace exclusiveDijetsAnalysis;
using namespace eventInfo;
using namespace reweight;

void ExclusiveDijet::LoadFile(std::string fileinput, std::string processinput){

  inf = NULL;
  tr  = NULL;
  inf = TFile::Open(fileinput.c_str(),"read");
  std::string fdirectory = processinput + "/ProcessedTree";
  tr = (TTree*)inf->Get(fdirectory.c_str());
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

void ExclusiveDijet::CreateHistos(std::string type){

  std::string step0 = "without_cuts";
  std::string step1 = "with_type";  
  std::string step2 = "d_eta4";
  std::string step3 = "d_eta3";
  std::string step4 = "d_eta2";
  std::string step5 = "d_eta1";

  Folders.push_back(step0);
  Folders.push_back(step1);
  Folders.push_back(step2);
  Folders.push_back(step3);
  Folders.push_back(step4);
  Folders.push_back(step5);

  int nloop=-999;

  if (type=="multiple_pileup") nloop=21;
  else if (type=="no_multiple_pileup") nloop=1;


  char tag[300];

  for (std::vector<std::string>::size_type j=0; j<Folders.size(); j++){

    m_hVector_lumi.push_back( std::vector<TH1D*>() );
    m_hVector_rjj.push_back( std::vector<TH1D*>() );
    m_hVector_detagen.push_back( std::vector<TH1D*>() );
    m_hVector_mxGen.push_back( std::vector<TH1D*>() );
    m_hVector_multhf.push_back( std::vector<TH2F*>() );
    m_hVector_etcalos.push_back( std::vector<TH2F*>() );
    m_hVector_tracks.push_back( std::vector<TH1D*>() );
    m_hVector_pfetamax.push_back( std::vector<TH1D*>() );
    m_hVector_pfetamin.push_back( std::vector<TH1D*>() );
    m_hVector_asumE.push_back( std::vector<TH1D*>() );
    m_hVector_deltaetajets.push_back( std::vector<TH1D*>() );
    m_hVector_deltaphijets.push_back( std::vector<TH1D*>() );
    m_hVector_deltaptjets.push_back( std::vector<TH1D*>() );
    m_hVector_dijetmass.push_back( std::vector<TH1D*>() );
    m_hVector_ptjet1.push_back( std::vector<TH1D*>() );
    m_hVector_ptjet2.push_back( std::vector<TH1D*>() );
    m_hVector_etajet1.push_back( std::vector<TH1D*>() );
    m_hVector_etajet2.push_back( std::vector<TH1D*>() );
    m_hVector_phijet1.push_back( std::vector<TH1D*>() );
    m_hVector_phijet2.push_back( std::vector<TH1D*>() );
    m_hVector_sumEHFplus.push_back( std::vector<TH1D*>() );
    m_hVector_sumEHFminus.push_back( std::vector<TH1D*>() );
    m_hVector_sumEHEplus.push_back( std::vector<TH1D*>() );
    m_hVector_sumEHEminus.push_back( std::vector<TH1D*>() );
    m_hVector_sumEHFpfplus.push_back( std::vector<TH1D*>() );
    m_hVector_sumEHFpfminus.push_back( std::vector<TH1D*>() );
    m_hVector_sumECastor.push_back( std::vector<TH1D*>() );
    m_hVector_deltaEtaPF.push_back( std::vector<TH1D*>() );
    m_hVector_absdeltaEtaPF.push_back( std::vector<TH1D*>() );
    m_hVector_vertex.push_back( std::vector<TH1D*>() );
    m_hVector_sumEHFplusVsetaMax.push_back( std::vector<TH2D*>() );
    m_hVector_sumEHFminusVsetaMin.push_back( std::vector<TH2D*>() );
    m_hVector_sumEHFpfplusVsetaMax.push_back( std::vector<TH2D*>() );
    m_hVector_sumEHFpfminusVsetaMin.push_back( std::vector<TH2D*>() );
    m_hVector_sumEHFplusVsiEta.push_back( std::vector<TH1D*>() );
    m_hVector_sumEHFminusVsiEta.push_back( std::vector<TH1D*>() );


    for (int k=0;k<nloop;k++){

      if (type=="multiple_pileup"){
	sprintf(tag,"multiple_pileup_%i",k);
      }
      else {
	sprintf(tag,"no_multiple_pileup");
      }

      char name1[300];
      sprintf(name1,"RJJ_%s_%s",tag,Folders.at(j).c_str());
      TH1D *histo_rjj = new TH1D(name1,"R_{jj} Distribution; R_{jj}; N events",20,0,1.2);
      m_hVector_rjj[j].push_back(histo_rjj);

      char name2[300];
      sprintf(name2,"DeltaEtaGen_%s_%s",tag,Folders.at(j).c_str());
      TH1D *histo_detagen = new TH1D(name2,"#Delta#eta_{Gen} Distribution; #Delta#eta_{Gen}; N events",20,0.0,5.2);
      m_hVector_detagen[j].push_back(histo_detagen);

      char name3[300];
      sprintf(name3,"MxGenRange_%s_%s",tag,Folders.at(j).c_str());
      TH1D *histo_MxGenRange = new TH1D(name3,"Mass_{X} Gen Distribution; M_{x} Gen [GeV]; N events",50,0,5000);
      m_hVector_mxGen[j].push_back(histo_MxGenRange);

      char name4[300];
      sprintf(name4,"mHF_%s_%s",tag,Folders.at(j).c_str());
      TH2F *histo_MultHF = new TH2F(name4,"HF^{+} and HF^{-} Multiplicity; n HF^{+}; n HF^{-}; N events", 10,  0., 10., 10,  0., 10. );
      m_hVector_multhf[j].push_back(histo_MultHF);

      char name5[300];
      sprintf(name5,"ETCalos_%s_%s",tag,Folders.at(j).c_str());
      TH2F *histo_ET_Calos = new TH2F(name5,"HF^{+} and Castor; #sum Energy HF^{+}; #sum Energy Castor [GeV]; N events", 400,  0., 2000., 500,  0., 5000. );
      m_hVector_etcalos[j].push_back(histo_ET_Calos);

      char name6[300];
      sprintf(name6,"Tracks_%s_%s",tag,Folders.at(j).c_str());
      TH1D *histo_Tracks = new TH1D(name6,"Tracks Multiplicity; n Tracks; N events",50,0,150);
      m_hVector_tracks[j].push_back(histo_Tracks);

      char name7[300];
      sprintf(name7,"pfetamax_%s_%s",tag,Folders.at(j).c_str());
      TH1D *histo_PFEtamax = new TH1D(name7,"Particle Flow #eta_{max} Distribution; #eta; N events",20,0,5.5);
      m_hVector_pfetamax[j].push_back(histo_PFEtamax);

      char name8[300];
      sprintf(name8,"pfetamin_%s_%s",tag,Folders.at(j).c_str());
      TH1D *histo_PFEtamin = new TH1D(name8,"Particle Flow #eta_{min} Distribution; #eta; N events",20,-5.5,0);
      m_hVector_pfetamin[j].push_back(histo_PFEtamin);

      char name9[300];
      sprintf(name9,"aEnergy_%s_%s",tag,Folders.at(j).c_str());
      TH1D *histo_aSumE = new TH1D(name9,"Forward Backward Asymmetry Distribution ; (#sum HF^{+} - #sum HF^{-})x(#sum HF^{+} + #sum HF^{-})^{-1}; N events",50,-1,1);
      m_hVector_asumE[j].push_back(histo_aSumE);

      char name10[300];
      sprintf(name10,"deltaEtaJets_%s_%s",tag,Folders.at(j).c_str());
      TH1D *histo_deltaetajets = new TH1D(name10,"#Delta#eta_{jj} Distribution; #Delta#eta_{jj}; N events",20,0.0,5.2);
      m_hVector_deltaetajets[j].push_back(histo_deltaetajets);

      char name11[300];
      sprintf(name11,"deltaPhiJets_%s_%s",tag,Folders.at(j).c_str());
      TH1D *histo_deltaphijets = new TH1D(name11,"#Delta#phi_{jj} Distribution; #Delta#phi_{jj}; N events",20,0.0,3.2);
      m_hVector_deltaphijets[j].push_back(histo_deltaphijets);

      char name12[300];
      sprintf(name12,"deltaPtJets_%s_%s",tag,Folders.at(j).c_str());
      TH1D *histo_deltaptjets = new TH1D(name12,"#Delta P_{T} Distribution; #Delta P_{T} [GeV.c^{-1}]; N events",40,0,400);
      m_hVector_deltaptjets[j].push_back(histo_deltaptjets);

      char name13[300];
      sprintf(name13,"DijetMass_%s_%s",tag,Folders.at(j).c_str());
      TH1D *histo_dijetmass = new TH1D(name13,"Dijet Invariant Mass Distribution; M_{jj} [GeV]; N events",80,0,800);
      m_hVector_dijetmass[j].push_back(histo_dijetmass);

      char name14[300];
      sprintf(name14,"pTJet1_%s_%s",tag,Folders.at(j).c_str());
      TH1D *histo_ptjet1 = new TH1D(name14,"Leading Jet - P_{T} Distribution; P_{T} [GeV.c^{-1}]; N events",100,0,2000);
      m_hVector_ptjet1[j].push_back(histo_ptjet1);

      char name15[300];
      sprintf(name15,"pTJet2_%s_%s",tag,Folders.at(j).c_str());
      TH1D *histo_ptjet2 = new TH1D(name15,"Second Jet - P_{T} Distribution; P_{T} [GeV.c^{-1}]; N events",100,0,2000);
      m_hVector_ptjet2[j].push_back(histo_ptjet2);

      char name16[300];
      sprintf(name16,"etaJet1_%s_%s",tag,Folders.at(j).c_str());
      TH1D *histo_etajet1 = new TH1D(name16,"Leading Jet - #eta Distribution; #eta; N events",50,-5.2,5.2);
      m_hVector_etajet1[j].push_back(histo_etajet1);

      char name17[300];
      sprintf(name17,"etaJet2_%s_%s",tag,Folders.at(j).c_str());
      TH1D *histo_etajet2 = new TH1D(name17,"Second Jet - #eta Distribution; #eta; N events",50,-5.2,5.2);
      m_hVector_etajet2[j].push_back(histo_etajet2);

      char name18[300];
      sprintf(name18,"phiJet1_%s_%s",tag,Folders.at(j).c_str());
      TH1D *histo_phijet1 = new TH1D(name18,"Leading Jet - #phi Distribution; #phi [rad]; N events",50,-3.3,3.3);
      m_hVector_phijet1[j].push_back(histo_phijet1);

      char name19[300];
      sprintf(name19,"phiJet2_%s_%s",tag,Folders.at(j).c_str());
      TH1D *histo_phijet2 = new TH1D(name19,"Second Jet - #phi Distribution; #phi [rad]; N events",50,-3.3,3.3);
      m_hVector_phijet2[j].push_back(histo_phijet2);

      char name20[300];
      sprintf(name20,"sumEHFplus_%s_%s",tag,Folders.at(j).c_str());
      TH1D *histo_sumEHFplus = new TH1D(name20,"HF^{+} - Sum of Energy; #sum E_{HF^{+}} [GeV]; N events",100,0,2000);
      m_hVector_sumEHFplus[j].push_back(histo_sumEHFplus);

      char name21[300];
      sprintf(name21,"sumEHFminus_%s_%s",tag,Folders.at(j).c_str());
      TH1D *histo_sumEHFminus = new TH1D(name21,"HF^{-} - Sum of Energy; #sum E_{HF^{-}} [GeV]; N events",100,0,2000);
      m_hVector_sumEHFminus[j].push_back(histo_sumEHFminus);

      char name22[300];
      sprintf(name22,"sumEHEplus_%s_%s",tag,Folders.at(j).c_str());
      TH1D *histo_sumEHEplus = new TH1D(name22,"HE^{+} - Sum of Energy; #sum E_{HE^{+}} [GeV]; N events",100,0,2000);
      m_hVector_sumEHEplus[j].push_back(histo_sumEHEplus);

      char name23[300];
      sprintf(name23,"sumEHEminus_%s_%s",tag,Folders.at(j).c_str());
      TH1D *histo_sumEHEminus = new TH1D(name23,"HE^{-} - Sum of Energy; #sum E_{HE^{-}} [GeV]; N events",100,0,2000);
      m_hVector_sumEHEminus[j].push_back(histo_sumEHEminus);

      char name24[300];
      sprintf(name24,"sumEHFpfplus_%s_%s",tag,Folders.at(j).c_str());
      TH1D *histo_sumEHFpfplus = new TH1D(name24,"HF^{+} - Sum of PF Energy; #sum E_{HF^{+}_{PF}} [GeV]; N events",100,0,2000);
      m_hVector_sumEHFpfplus[j].push_back(histo_sumEHFpfplus);

      char name25[300];
      sprintf(name25,"sumEHFpfminus_%s_%s",tag,Folders.at(j).c_str());
      TH1D *histo_sumEHFpfminus = new TH1D(name25,"HF^{-} - Sum of PF Energy; #sum E_{HF^{-}_{PF}} [GeV]; N events",100,0,2000);
      m_hVector_sumEHFpfminus[j].push_back(histo_sumEHFpfminus);

      char name26[300];
      sprintf(name26,"deltaEtamaxminPF_%s_%s",tag,Folders.at(j).c_str());
      TH1D *histo_deltaEtaPF = new TH1D(name26,"#Delta#eta_{PF} Distribution; #eta_{max}-#eta_{min}; N events",20,0,10);
      m_hVector_deltaEtaPF[j].push_back(histo_deltaEtaPF);

      char name27[300];
      sprintf(name27,"absdeltaEtamaxminPF_%s_%s",tag,Folders.at(j).c_str());
      TH1D *histo_absdeltaEtaPF = new TH1D(name27,"|#Delta#eta_{PF}| Distribution; |#eta_{max}-#eta_{min}|; N events",20,0,10);
      m_hVector_absdeltaEtaPF[j].push_back(histo_absdeltaEtaPF);

      char name28[300];
      sprintf(name28,"vertex_%s_%s",tag,Folders.at(j).c_str());
      TH1D *histo_vertex = new TH1D(name28,"Number of Vertex; # Vertex; N events",25,0,25);
      m_hVector_vertex[j].push_back(histo_vertex);

      char name29[300];
      sprintf(name29,"lumi_%s_%s",tag,Folders.at(j).c_str());
      TH1D *histo_lumi = new TH1D(name29,"Luminosity per Bunch; L_{Bunch} [#mub^{-1}s^{-1}]; N events",25,0,2);
      m_hVector_lumi[j].push_back(histo_lumi);

      char name30[300];
      sprintf(name30,"sumEHFpfplusVspfetamax_%s_%s",tag,Folders.at(j).c_str());
      TH2D *histo_sumEHFpfplusVsPFEtamax = new TH2D(name30,"#sum E_{HF^{+}} Vs #eta_{max} Distribution; #eta_{Max};#sum E_{HF^{+}} [GeV]; N events",20,0,5.5,100, 0., 2000.);
      m_hVector_sumEHFpfplusVsetaMax[j].push_back(histo_sumEHFpfplusVsPFEtamax);

      char name31[300];
      sprintf(name31,"sumEHFpfminusVspfetamin_%s_%s",tag,Folders.at(j).c_str());
      TH2D *histo_sumEHFpfminusVsPFEtamin = new TH2D(name31,"#sum E_{HF^{-}} Vs #eta_{min} Distribution; #eta_{Min};#sum E_{HF^{-}} [GeV]; N events",20,-5.5,0,100, 0., 2000);
      m_hVector_sumEHFpfminusVsetaMin[j].push_back(histo_sumEHFpfminusVsPFEtamin);

      char name32[300];
      sprintf(name32,"sumEHFplusVspfetamax_%s_%s",tag,Folders.at(j).c_str());
      TH2D *histo_sumEHFplusVsPFEtamax = new TH2D(name32,"#sum E_{HF^{+}} Vs #eta_{max} Distribution; #eta_{Max};#sum E_{HF^{+}} [GeV]; N events",20,0,5.5,100, 0., 2000.);
      m_hVector_sumEHFplusVsetaMax[j].push_back(histo_sumEHFplusVsPFEtamax);

      char name33[300];
      sprintf(name33,"sumEHFminusVspfetamin_%s_%s",tag,Folders.at(j).c_str());
      TH2D *histo_sumEHFminusVsPFEtamin = new TH2D(name33,"#sum E_{HF^{-}} Vs #eta_{min} Distribution; #eta_{Min};#sum E_{HF^{-}} [GeV]; N events",20,-5.5,0,100, 0., 2000);
      m_hVector_sumEHFminusVsetaMin[j].push_back(histo_sumEHFminusVsPFEtamin);

      char name34[300];
      sprintf(name34,"sumECastor_%s_%s",tag,Folders.at(j).c_str());
      TH1D *histo_sumECastor = new TH1D(name34,"Castor - Total Energy; #sum E_{Castor} [GeV]; N events",100,0,2000);
      m_hVector_sumECastor[j].push_back(histo_sumECastor);


      char name_ieta[300];
      for(int ieta = 29; ieta <= 41; ++ieta){
	sprintf(name_ieta,"sumEHFplus_iEta_%d_%s_%s",ieta,tag,Folders.at(j).c_str());
	TH1D *histo_sumEHFplus_ieta = new TH1D(name_ieta,"HF^{+} - Sum of Energy; #sum E_{HF^{+}} [GeV]; N events",500,0,500);
	m_hVector_sumEHFplusVsiEta[j].push_back(histo_sumEHFplus_ieta);

	sprintf(name_ieta,"sumEHFminus_iEta_%d_%s_%s",ieta,tag,Folders.at(j).c_str());
	TH1D *histo_sumEHFminus_ieta = new TH1D(name_ieta,"HF^{-} - Sum of Energy; #sum E_{HF^{-}} [GeV]; N events",500,0,500);
	m_hVector_sumEHFminusVsiEta[j].push_back(histo_sumEHFminus_ieta);
      }


    }
  }
}


void ExclusiveDijet::FillHistos(int index, int pileup, double totalweight){

  m_hVector_rjj[index].at(pileup)->Fill(eventexcl->GetRjjFromJets(),totalweight);
  m_hVector_detagen[index].at(pileup)->Fill(eventexcl->GetDeltaEtaGen(),totalweight);
  m_hVector_mxGen[index].at(pileup)->Fill(eventexcl->GetMxGenRange(),totalweight);
  m_hVector_multhf[index].at(pileup)->Fill(eventdiff->GetMultiplicityHFPlus(),eventdiff->GetMultiplicityHFMinus(),totalweight);
  m_hVector_etcalos[index].at(pileup)->Fill(eventdiff->GetSumEnergyHFPlus(),eventdiff->GetSumETotCastor(),totalweight);
  m_hVector_tracks[index].at(pileup)->Fill(eventdiff->GetMultiplicityTracks(),totalweight);
  m_hVector_pfetamax[index].at(pileup)->Fill(eventdiff->GetEtaMaxFromPFCands(),totalweight);
  m_hVector_pfetamin[index].at(pileup)->Fill(eventdiff->GetEtaMinFromPFCands(),totalweight);
  m_hVector_asumE[index].at(pileup)->Fill(aSumE,totalweight);
  m_hVector_deltaetajets[index].at(pileup)->Fill(eventexcl->GetJetsDeltaEta(),totalweight);
  m_hVector_deltaphijets[index].at(pileup)->Fill(deltaphi,totalweight);
  m_hVector_deltaptjets[index].at(pileup)->Fill(eventexcl->GetJetsDeltaPt(),totalweight);
  m_hVector_dijetmass[index].at(pileup)->Fill(eventexcl->GetMassDijets(),totalweight);
  m_hVector_ptjet1[index].at(pileup)->Fill(ptJet1,totalweight);
  m_hVector_ptjet2[index].at(pileup)->Fill(ptJet2,totalweight);
  m_hVector_etajet1[index].at(pileup)->Fill(eventexcl->GetLeadingJetEta(),totalweight);
  m_hVector_etajet2[index].at(pileup)->Fill(eventexcl->GetSecondJetEta(),totalweight);
  m_hVector_phijet1[index].at(pileup)->Fill(eventexcl->GetLeadingJetPhi(),totalweight);
  m_hVector_phijet2[index].at(pileup)->Fill(eventexcl->GetSecondJetPhi(),totalweight);
  m_hVector_sumEHFplus[index].at(pileup)->Fill(eventdiff->GetSumEnergyHFPlus(),totalweight);
  m_hVector_sumEHFminus[index].at(pileup)->Fill(eventdiff->GetSumEnergyHFMinus(),totalweight);
  m_hVector_sumEHEplus[index].at(pileup)->Fill(eventdiff->GetSumEnergyHEPlus(),totalweight);
  m_hVector_sumEHEminus[index].at(pileup)->Fill(eventdiff->GetSumEnergyHEMinus(),totalweight);
  m_hVector_sumEHFpfplus[index].at(pileup)->Fill(eventexcl->GetSumEHFPFlowPlus(),totalweight);
  m_hVector_sumEHFpfminus[index].at(pileup)->Fill(eventexcl->GetSumEHFPFlowMinus(),totalweight);
  m_hVector_deltaEtaPF[index].at(pileup)->Fill(deltaetapf,totalweight);
  m_hVector_absdeltaEtaPF[index].at(pileup)->Fill(absdeltaetapf,totalweight);
  m_hVector_vertex[index].at(pileup)->Fill(eventexcl->GetNVertex(),totalweight);
  m_hVector_lumi[index].at(pileup)->Fill(eventinfo->GetInstLumiBunch());
  m_hVector_sumEHFpfplusVsetaMax[index].at(pileup)->Fill(eventdiff->GetEtaMaxFromPFCands(),eventexcl->GetSumEHFPFlowPlus(),totalweight);
  m_hVector_sumEHFpfminusVsetaMin[index].at(pileup)->Fill(eventdiff->GetEtaMinFromPFCands(),eventexcl->GetSumEHFPFlowMinus(),totalweight);
  m_hVector_sumEHFplusVsetaMax[index].at(pileup)->Fill(eventdiff->GetEtaMaxFromPFCands(),eventdiff->GetSumEnergyHFPlus(),totalweight);
  m_hVector_sumEHFminusVsetaMin[index].at(pileup)->Fill(eventdiff->GetEtaMinFromPFCands(),eventdiff->GetSumEnergyHFMinus(),totalweight);     
  m_hVector_sumECastor[index].at(pileup)->Fill(eventdiff->GetSumETotCastor(),totalweight);

}

void ExclusiveDijet::SaveHistos(std::string type){


  int ipileup;

  if (type=="multiple_pileup") ipileup=21;
  else ipileup=1;

  for (int i = 0; i < ipileup; i++){
    for (std::vector<std::string>::size_type j=0; j<Folders.size(); j++){
      m_hVector_rjj[j].at(i)->Write();
      m_hVector_detagen[j].at(i)->Write();
      m_hVector_mxGen[j].at(i)->Write();
      m_hVector_multhf[j].at(i)->Write();
      m_hVector_etcalos[j].at(i)->Write();
      m_hVector_tracks[j].at(i)->Write();
      m_hVector_pfetamax[j].at(i)->Write();
      m_hVector_pfetamin[j].at(i)->Write();
      m_hVector_asumE[j].at(i)->Write();
      m_hVector_deltaetajets[j].at(i)->Write();
      m_hVector_deltaphijets[j].at(i)->Write();
      m_hVector_deltaptjets[j].at(i)->Write();
      m_hVector_dijetmass[j].at(i)->Write();
      m_hVector_ptjet1[j].at(i)->Write();
      m_hVector_ptjet2[j].at(i)->Write();
      m_hVector_etajet1[j].at(i)->Write();
      m_hVector_etajet2[j].at(i)->Write();
      m_hVector_phijet1[j].at(i)->Write();
      m_hVector_phijet2[j].at(i)->Write();
      m_hVector_sumEHFplus[j].at(i)->Write();
      m_hVector_sumEHFminus[j].at(i)->Write();
      m_hVector_sumEHEplus[j].at(i)->Write();
      m_hVector_sumEHEminus[j].at(i)->Write();
      m_hVector_sumEHFpfplus[j].at(i)->Write();
      m_hVector_sumEHFpfminus[j].at(i)->Write();
      m_hVector_deltaEtaPF[j].at(i)->Write();
      m_hVector_absdeltaEtaPF[j].at(i)->Write();
      m_hVector_vertex[j].at(i)->Write();
      m_hVector_lumi[j].at(i)->Write();
      m_hVector_sumEHFpfplusVsetaMax[j].at(i)->Write();
      m_hVector_sumEHFpfminusVsetaMin[j].at(i)->Write();
      m_hVector_sumEHFplusVsetaMax[j].at(i)->Write();
      m_hVector_sumEHFminusVsetaMin[j].at(i)->Write();
      m_hVector_sumECastor[j].at(i)->Write();
    }
  }

}

/*
   double ExclusiveDijet::cutCorrection(std::string filecor, ){

   TFile *l1  = TFile::Open(filecor.c_str());
   TH1F* h_eff_excl = (TH1F*)l1->Get("RatioPreSel");
   TH1F* h_eff_vertex = (TH1F*)l1->Get("RatioVertex");
   TH1F* h_eff_step_4_4 = (TH1F*)l1->Get("RatioStep4_4");
   TH1F* h_eff_step_4_3 = (TH1F*)l1->Get("RatioStep4_3");
   TH1F* h_eff_step_4_2 = (TH1F*)l1->Get("RatioStep4_2");
   TH1F* h_eff_step_4_1 = (TH1F*)l1->Get("RatioStep4_1");

   }

   double ExclusiveDijet::triggerCorrection(std::string ){

   TFile *l2  = TFile::Open(filetrigger.c_str());
   TH1F* h_eff_trigger_eta1 = (TH1F*)l2->Get("Events_with_RefTriggerCutsOffLineAndTrigger_eta1");
   TH1F* h_eff_trigger_eta2 = (TH1F*)l2->Get("Events_with_RefTriggerCutsOffLineAndTrigger_eta2");
   TH1F* h_eff_trigger_eta3 = (TH1F*)l2->Get("Events_with_RefTriggerCutsOffLineAndTrigger_eta3");
   TH1F* h_eff_trigger_eta4 = (TH1F*)l2->Get("Events_with_RefTriggerCutsOffLineAndTrigger_eta4");

   }
 */

void ExclusiveDijet::Run(std::string filein_, std::string savehistofile_, std::string processname_, std::string type_, std::string jetunc_, std::string switchpucorr_, std::string pudatafile_, std::string pumcfile_){

  TH1::SetDefaultSumw2(true);
  TH2::SetDefaultSumw2(true);

  filein = filein_;
  savehistofile = savehistofile_;
  processname = processname_;
  type = type_;
  jetunc = jetunc_;
  switchpucorr = switchpucorr_;
  pudatafile = pudatafile_;
  pumcfile = pumcfile_;

  TFile check1(filein.c_str());

  if (check1.IsZombie()){

    std::cout << "\n----------------------------------------------" << std::endl;
    std::cout << " There is no the file " << filein << " or the"   << std::endl;
    std::cout << " path is not correct." << std::endl;
    std::cout << "------------------------------------------------" << std::endl;
    return;

  }

  if (check1.GetDirectory(processname.c_str())){
    LoadFile(filein,processname);
  }

  else {
    std::cout << "\n-------------------------------------------------" << std::endl;
    std::cout << " There is no directory/path " << processname << std::endl;
    std::cout << " in the file." << std::endl;
    std::cout << "---------------------------------------------------" << std::endl;
    return;
  }

  TFile *outf = new TFile(savehistofile.c_str(),"RECREATE");
  TString outtxt = savehistofile;
  outtxt.ReplaceAll("root","txt");
  std::ofstream outstring(outtxt);

  int NEVENTS = tr->GetEntries();
  int decade = 0;
  int pileup = -999;
  int triggercounter[20]={0};

  TH1::SetDefaultSumw2(true);
  TH2::SetDefaultSumw2(true);

  unsigned NEntries = tr->GetEntries();
  std::cout << "" << std::endl;
  std::cout<< "Reading Tree: "<< NEntries << " events"<<std::endl;
  std::cout << "" << std::endl;

  std::string status, jetstatus;  

  edm::LumiReWeighting LumiWeights_(pumcfile.c_str(),pudatafile.c_str(),"pileUpBx0_complete_without_cuts","pileup");

  for(int i=0;i<NEVENTS;i++){

    deltaphi = -999.;
    aSumE = -999.;
    absdeltaetapf = -999.;
    deltaetapf = -999.;
    ptJet1 = -999.;
    ptJet2 = -999.;
    totalweight = -999;

    double progress = 10.0*i/(1.0*NEVENTS);
    int l = TMath::FloorNint(progress); 

    if (l > decade){
      std::cout <<"\n<<<<<< STATUS >>>>>>" << std::endl; 
      std::cout<<10*l<<" % completed." << std::endl;
      std::cout <<"<<<<<<<<<<>>>>>>>>>>\n" << std::endl;
    }
    decade = l;          

    tr->GetEntry(i);

    if ( type=="multiple_pileup" && (eventexcl->GetNPileUpBx0()==-1 && eventexcl->GetNPileUpBxm1()==-1 && eventexcl->GetNPileUpBxp1()==-1 )){
      std::cout << "--------------------------------------------------------------" << std::endl;
      std::cout << " There is no pile-up TTree information in your PATTuplefile."   << std::endl;
      std::cout << " Please, use another PATTuple with PU information to run mul- " << std::endl;
      std::cout << " tiple PU option." << std::endl;
      std::cout << "--------------------------------------------------------------" << std::endl;
      return;
    }

    if (type=="multiple_pileup"){
      pileup = eventexcl->GetNPileUpBx0();
    }
    else{
      pileup = 0;
    }

    for (int nt=0;nt<20;nt++){
      if(eventexcl->GetHLTPath(nt)){
	triggercounter[nt]++;
      }
    }

    deltaphi = fabs(eventexcl->GetLeadingJetPhi()-eventexcl->GetSecondJetPhi());
    aSumE = (eventdiff->GetSumEnergyHFPlus() - eventdiff->GetSumEnergyHFMinus())/(eventdiff->GetSumEnergyHFPlus() + eventdiff->GetSumEnergyHFMinus());
    absdeltaetapf = fabs(eventdiff->GetEtaMaxFromPFCands()-eventdiff->GetEtaMinFromPFCands());
    deltaetapf = (eventdiff->GetEtaMaxFromPFCands()-eventdiff->GetEtaMinFromPFCands());

    if (jetunc == "plus"){
      ptJet1 = eventexcl->GetLeadingJetPt() + eventexcl->GetUnc1()*eventexcl->GetLeadingJetPt(); 
      ptJet2 = eventexcl->GetSecondJetPt() + eventexcl->GetUnc2()*eventexcl->GetSecondJetPt();
      jetstatus = "Jets Unc. Plus";
    }

    else if (jetunc == "minus"){
      ptJet1 = eventexcl->GetLeadingJetPt() - eventexcl->GetUnc1()*eventexcl->GetLeadingJetPt();             
      ptJet2 = eventexcl->GetSecondJetPt() - eventexcl->GetUnc2()*eventexcl->GetSecondJetPt();
      jetstatus = "Jets Unc. Minus";
    }

    else if (jetunc == "none"){
      ptJet1 = eventexcl->GetLeadingJetPt();
      ptJet2 = eventexcl->GetSecondJetPt();
      jetstatus = "Normal";
    } 

    else {
      ptJet1 = eventexcl->GetLeadingJetPt();
      ptJet2 = eventexcl->GetSecondJetPt();
      jetstatus = "Unrecognized jet energy scale correction. Defined without it.";
    }

    double totalweight = 1.; 
    double weightpu = 1.;

    if (switchpucorr=="pileup_correction") weightpu = LumiWeights_.weight(eventexcl->GetNPileUpBx0());

    totalweight = weightpu;


    if(pileup<21){
      FillHistos(0,pileup,totalweight);
    }




    /*

       bool triggerZeroBias = false;
       bool triggerHLTPlus = false;
       bool triggerHLTMinus = false;
       bool vertex = false;
       bool tracks = false;
       bool d_eta4 = false;
       bool d_eta3 = false;
       bool d_eta2 = false;
       bool d_eta1 = false;
       bool collisions = false;
       bool unpaired = false;
       bool presel = false;

       if (eventexcl->GetHLTPath(0)) triggerZeroBias = true;
       if (eventexcl->GetHLTPath(1)) triggerHLTPlus = true;
       if (eventexcl->GetHLTPath(2)) triggerHLTMinus = true;
       if ((eventdiff->GetSumEnergyHFMinus() < 30 && eventdiff->GetSumEnergyHFPlus() < 30) || (eventdiff->GetEtaMinFromPFCands() < -990 && eventdiff->GetEtaMaxFromPFCands() < -990)) presel = true;
       if (eventdiff->GetMultiplicityTracks() > 0) tracks = true;  
       if (eventexcl->GetNVertex() > 0) vertex = true;
       if ((eventdiff->GetEtaMinFromPFCands() > -4. && eventdiff->GetEtaMaxFromPFCands() < 4.) || (eventdiff->GetEtaMinFromPFCands() < -990 && eventdiff->GetEtaMaxFromPFCands() < -990) ) d_eta4 = true;
       if ((eventdiff->GetEtaMinFromPFCands() > -3. && eventdiff->GetEtaMaxFromPFCands() < 3.) || (eventdiff->GetEtaMinFromPFCands() < -990 && eventdiff->GetEtaMaxFromPFCands() < -990) ) d_eta3 = true; 
       if ((eventdiff->GetEtaMinFromPFCands() > -2. && eventdiff->GetEtaMaxFromPFCands() < 2.) || (eventdiff->GetEtaMinFromPFCands() < -990 && eventdiff->GetEtaMaxFromPFCands() < -990) ) d_eta2 = true;
       if ((eventdiff->GetEtaMinFromPFCands() > -1. && eventdiff->GetEtaMaxFromPFCands() < 1.) || (eventdiff->GetEtaMinFromPFCands() < -990 && eventdiff->GetEtaMaxFromPFCands() < -990) ) d_eta1 = true;

       if (type == "collisions"){
       if (triggerZeroBias && vertex && tracks) collisions = true;
       status = "collisions";
       FillHistos(0); 
       if (collisions) FillHistos(1);
       if (collisions && d_eta4) FillHistos(2);
       if (collisions && d_eta3) FillHistos(3);
       if (collisions && d_eta2) FillHistos(4);
       if (collisions && d_eta1) FillHistos(5);
       }

       else if (type == "unpaired"){
       if((triggerHLTPlus || triggerHLTMinus) && !vertex && !tracks) unpaired = true; 
       status = "unpaired";
       FillHistos(0);
       if (unpaired) FillHistos(1);
       if (unpaired && d_eta4) FillHistos(2);
       if (unpaired && d_eta3) FillHistos(3);
       if (unpaired && d_eta2) FillHistos(4);
       if (unpaired && d_eta1) FillHistos(5);
       }

       else {
       std::cout << "\n Unrecognized Type of Selection." << std::endl;
       return;
       }

     */
  }   



  outstring << "" << std::endl;
  outstring << "<< INPUTS >>" << std::endl;
  outstring << " " << std::endl;
  outstring << "Input file: " << filein << std::endl;
  outstring << "Output file: " << savehistofile << std::endl;
  outstring << " " << std::endl;
  outstring << "Type: " << status << std::endl;
  outstring << "Jet Uncertainty: " << jetstatus << std::endl; 
  outstring << "" << std::endl;
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
  std::string savehistofile_;
  std::string processname_;
  std::string type_;
  std::string jetunc_;
  std::string switchpucorr_;
  std::string pudatafile_;
  std::string pumcfile_;
  std::string cutcorrfile_;
  std::string triggercorrfile_;
  std::string switchcutcorr_;
  std::string switchtriggercorr_;

  if (argc > 1 && strcmp(s1,argv[1]) != 0)  filein_ = argv[1];
  if (argc > 2 && strcmp(s1,argv[2]) != 0)  savehistofile_  = argv[2];
  if (argc > 3 && strcmp(s1,argv[3]) != 0)  processname_  = argv[3];
  if (argc > 4 && strcmp(s1,argv[4]) != 0)  type_  = argv[4];
  if (argc > 5 && strcmp(s1,argv[5]) != 0)  jetunc_ = argv[5];
  if (argc > 6 && strcmp(s1,argv[6]) != 0)  switchpucorr_  = argv[6];
  if (argc > 7 && strcmp(s1,argv[7]) != 0)  pudatafile_ = argv[7];
  if (argc > 8 && strcmp(s1,argv[8]) != 0)  pumcfile_ = argv[8];
  if (argc > 9 && strcmp(s1,argv[9]) != 0)  cutcorrfile_ = argv[9];
  if (argc > 10 && strcmp(s1,argv[10]) != 0) triggercorrfile_ = argv[10];
  if (argc > 11 && strcmp(s1,argv[11]) != 0) switchcutcorr_ = argv[11];
  if (argc > 12 && strcmp(s1,argv[12]) != 0) switchtriggercorr_ = argv[12];

  if (type_=="multiple_pileup" || type_=="no_multiple_pileup") {
    ExclusiveDijet* exclusive = new ExclusiveDijet();
    exclusive->CreateHistos(type_);

    TFile pudata(pudatafile_.c_str());
    TFile pumc(pumcfile_.c_str());
    if (pudata.IsZombie() || pumc.IsZombie()){
      std::cout << "----------------------------------------------" << std::endl;
      std::cout << " There is no Pile-Up data/mc file or the"   << std::endl;
      std::cout << " path is not correct." << std::endl;
      std::cout << "----------------------------------------------" << std::endl; 
      return 0;
    }
    else if (!pudata.GetListOfKeys()->Contains("pileup") || !pumc.GetListOfKeys()->Contains("pileUpBx0_complete_without_cuts") ){
      std::cout << "----------------------------------------------" << std::endl;
      std::cout << " There is no Pile-Up data/mc histograms: "   << std::endl;
      std::cout << " data: pileup" << std::endl;
      std::cout << " mc: pileUpBx0_complete_without_cuts" << std::endl;
      std::cout << "----------------------------------------------" << std::endl;
      return 0;
    }

    if(switchcutcorr_ == "cut_correction"){
      TFile effcut(cutcorrfile_.c_str());
      if (effcut.IsZombie()){
	std::cout << "---------------------------------------" << std::endl;
	std::cout << " There is no Efficiency cut file or the"   << std::endl;
	std::cout << " path is not correct." << std::endl;
	std::cout << "---------------------------------------" << std::endl;
	return 0;
      }
      else if (!effcut.GetListOfKeys()->Contains("RatioPreSel") || !effcut.GetListOfKeys()->Contains("RatioVertex") || !effcut.GetListOfKeys()->Contains("RatioStep4_4") || !effcut.GetListOfKeys()->Contains("RatioStep4_3") || !effcut.GetListOfKeys()->Contains("RatioStep4_2")| !effcut.GetListOfKeys()->Contains("RatioStep4_1")){
	std::cout << "----------------------------------------------" << std::endl;
	std::cout << " There is no Eff. Cuts histograms " << std::endl; 
	std::cout << "----------------------------------------------" << std::endl;
	return 0;
      }

    }
    if(switchtriggercorr_ == "trigger_correction"){
      TFile efftrigger(triggercorrfile_.c_str());
      if (efftrigger.IsZombie()){
	std::cout << "-------------------------------------------" << std::endl;
	std::cout << " There is no trigger efficiency file or the"   << std::endl;
	std::cout << " path is not correct." << std::endl;
	std::cout << "-------------------------------------------" << std::endl;
	return 0;
      }
      else if (!efftrigger.GetListOfKeys()->Contains("Events_with_RefTriggerCutsOffLineAndTrigger_eta4") || !efftrigger.GetListOfKeys()->Contains("Events_with_RefTriggerCutsOffLineAndTrigger_eta3") || !efftrigger.GetListOfKeys()->Contains("Events_with_RefTriggerCutsOffLineAndTrigger_eta2") || !efftrigger.GetListOfKeys()->Contains("Events_with_RefTriggerCutsOffLineAndTrigger_eta1")){
	std::cout << "----------------------------------------------" << std::endl;
	std::cout << " There is no Eff. Trigger histograms " << std::endl;
	std::cout << "----------------------------------------------" << std::endl;
	return 0;
      }
    }
    exclusive->Run(filein_, savehistofile_, processname_, type_, jetunc_, switchpucorr_, pudatafile_, pumcfile_);
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
