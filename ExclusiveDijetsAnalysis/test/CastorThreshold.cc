//-------------------------------------------------------------------------------------------------------->>
// UNIVERSIDADE DO ESTADO DO RIO DE JANEIRO - CMS/Brasil
//-------------------------------------------------------------------------------------------------------->>
//
// Twiki: https://twiki.cern.ch/twiki/bin/view/CMS/FwdPhysicsExclusiveDijetsAnalysis#Example_Analysis_Macro
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

#include "CastorThreshold.h"
#include "ForwardAnalysis/ForwardTTreeAnalysis/interface/EventInfoEvent.h"
#include "ForwardAnalysis/ForwardTTreeAnalysis/interface/ExclusiveDijetsEvent.h"
#include "ForwardAnalysis/ForwardTTreeAnalysis/interface/DiffractiveEvent.h"

using namespace diffractiveAnalysis;
using namespace exclusiveDijetsAnalysis;
using namespace eventInfo;

void CastorThreshold::LoadFile(std::string fileinput, std::string processinput){

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

void CastorThreshold::CreateHistos(){

  Folders.push_back("without_cuts");
  Folders.push_back("with_type");
  Folders.push_back("d_eta4");
  Folders.push_back("d_eta3");
  Folders.push_back("d_eta2");
  Folders.push_back("d_eta1");

  for (std::vector<std::string>::size_type j=0; j<Folders.size(); j++){

    char name1[300];
    sprintf(name1,"Tracks_%s",Folders.at(j).c_str());
    TH1D *histo_Tracks = new TH1D(name1,"Tracks Multiplicity; n Tracks; N events",50,0,150);
    m_hVector_tracks.push_back(histo_Tracks);

    char name2[300];
    sprintf(name2,"pfetamax_%s",Folders.at(j).c_str());
    TH1D *histo_PFEtamax = new TH1D(name2,"Particle Flow #eta_{max} Distribution; #eta; N events",20,0,5.5);
    m_hVector_pfetamax.push_back(histo_PFEtamax);

    char name3[300];
    sprintf(name3,"pfetamin_%s",Folders.at(j).c_str());
    TH1D *histo_PFEtamin = new TH1D(name3,"Particle Flow #eta_{min} Distribution; #eta; N events",20,-5.5,0);
    m_hVector_pfetamin.push_back(histo_PFEtamin);

    char name4[300];
    sprintf(name4,"sumEHFplus_%s",Folders.at(j).c_str());
    TH1D *histo_sumEHFplus = new TH1D(name4,"HF^{+} - Sum of Energy; #sum E_{HF^{+}} [GeV]; N events",100,0,2000);
    m_hVector_sumEHFplus.push_back(histo_sumEHFplus);

    char name5[300];
    sprintf(name5,"sumEHFminus_%s",Folders.at(j).c_str());
    TH1D *histo_sumEHFminus = new TH1D(name5,"HF^{-} - Sum of Energy; #sum E_{HF^{-}} [GeV]; N events",100,0,2000);
    m_hVector_sumEHFminus.push_back(histo_sumEHFminus);

    char name6[300];
    sprintf(name6,"sumEHFpfplus_%s",Folders.at(j).c_str());
    TH1D *histo_sumEHFpfplus = new TH1D(name6,"HF^{+} - Sum of PF Energy; #sum E_{HF^{+}_{PF}} [GeV]; N events",100,0,2000);
    m_hVector_sumEHFpfplus.push_back(histo_sumEHFpfplus);

    char name7[300];
    sprintf(name7,"sumEHFpfminus_%s",Folders.at(j).c_str());
    TH1D *histo_sumEHFpfminus = new TH1D(name7,"HF^{-} - Sum of PF Energy; #sum E_{HF^{-}_{PF}} [GeV]; N events",100,0,2000);
    m_hVector_sumEHFpfminus.push_back(histo_sumEHFpfminus);

    char name8[300];
    sprintf(name8,"vertex_%s",Folders.at(j).c_str());
    TH1D *histo_vertex = new TH1D(name8,"Number of Vertex; # Vertex; N events",25,0,25);
    m_hVector_vertex.push_back(histo_vertex);

    char name9[300];
    sprintf(name9,"lumi_%s",Folders.at(j).c_str());
    TH1D *histo_lumi = new TH1D(name9,"Luminosity per Bunch; L_{Bunch} [#mub^{-1}s^{-1}]; N events",25,0,2);
    m_hVector_lumi.push_back(histo_lumi);

    char name10[300];
    sprintf(name10,"sumEHFpfplusVspfetamax_%s",Folders.at(j).c_str());
    TH2D *histo_sumEHFpfplusVsPFEtamax = new TH2D(name10,"#sum E_{HF^{+}} Vs #eta_{max} Distribution; #eta_{Max};#sum E_{HF^{+}} [GeV]; N events",20,0,5.5,100, 0., 2000.);
    m_hVector_sumEHFpfplusVsetaMax.push_back(histo_sumEHFpfplusVsPFEtamax);

    char name11[300];
    sprintf(name11,"sumEHFpfminusVspfetamin_%s",Folders.at(j).c_str());
    TH2D *histo_sumEHFpfminusVsPFEtamin = new TH2D(name11,"#sum E_{HF^{-}} Vs #eta_{min} Distribution; #eta_{Min};#sum E_{HF^{-}} [GeV]; N events",20,-5.5,0,100, 0., 2000);
    m_hVector_sumEHFpfminusVsetaMin.push_back(histo_sumEHFpfminusVsPFEtamin);

    char name12[300];
    sprintf(name12,"sumEHFplusVspfetamax_%s",Folders.at(j).c_str());
    TH2D *histo_sumEHFplusVsPFEtamax = new TH2D(name12,"#sum E_{HF^{+}} Vs #eta_{max} Distribution; #eta_{Max};#sum E_{HF^{+}} [GeV]; N events",20,0,5.5,100, 0., 2000.);
    m_hVector_sumEHFplusVsetaMax.push_back(histo_sumEHFplusVsPFEtamax);

    char name13[300];
    sprintf(name13,"sumEHFminusVspfetamin_%s",Folders.at(j).c_str());
    TH2D *histo_sumEHFminusVsPFEtamin = new TH2D(name13,"#sum E_{HF^{-}} Vs #eta_{min} Distribution; #eta_{Min};#sum E_{HF^{-}} [GeV]; N events",20,-5.5,0,100, 0., 2000);
    m_hVector_sumEHFminusVsetaMin.push_back(histo_sumEHFminusVsPFEtamin);

    char name14[300];
    sprintf(name14,"sumECastor_%s",Folders.at(j).c_str());
    TH1D *histo_sumECastor = new TH1D(name14,"Castor - Total Energy; #sum E_{Castor} [GeV]; N events",100,0,2000);
    m_hVector_sumECastor.push_back(histo_sumECastor);

  }

}

void CastorThreshold::FillHistos(int index){

  m_hVector_tracks[index]->Fill(eventdiff->GetMultiplicityTracks());
  m_hVector_pfetamax[index]->Fill(eventdiff->GetEtaMaxFromPFCands());
  m_hVector_pfetamin[index]->Fill(eventdiff->GetEtaMinFromPFCands());
  m_hVector_sumEHFplus[index]->Fill(eventdiff->GetSumEnergyHFPlus());
  m_hVector_sumEHFminus[index]->Fill(eventdiff->GetSumEnergyHFMinus());
  m_hVector_sumEHFpfplus[index]->Fill(eventexcl->GetSumEHFPFlowPlus());
  m_hVector_sumEHFpfminus[index]->Fill(eventexcl->GetSumEHFPFlowMinus());
  m_hVector_vertex[index]->Fill(eventexcl->GetNVertex());
  m_hVector_lumi[index]->Fill(eventinfo->GetInstLumiBunch());
  m_hVector_sumEHFpfplusVsetaMax[index]->Fill(eventdiff->GetEtaMaxFromPFCands(),eventexcl->GetSumEHFPFlowPlus());
  m_hVector_sumEHFpfminusVsetaMin[index]->Fill(eventdiff->GetEtaMinFromPFCands(),eventexcl->GetSumEHFPFlowMinus());
  m_hVector_sumEHFplusVsetaMax[index]->Fill(eventdiff->GetEtaMaxFromPFCands(),eventdiff->GetSumEnergyHFPlus());
  m_hVector_sumEHFminusVsetaMin[index]->Fill(eventdiff->GetEtaMinFromPFCands(),eventdiff->GetSumEnergyHFMinus());  
  m_hVector_sumECastor[index]->Fill(eventdiff->GetSumETotCastor());

}

void CastorThreshold::SaveHistos(){

  for (std::vector<std::string>::size_type j=0; j<Folders.size(); j++){
    m_hVector_tracks[j]->Write();
    m_hVector_pfetamax[j]->Write();
    m_hVector_pfetamin[j]->Write();
    m_hVector_sumEHFplus[j]->Write();
    m_hVector_sumEHFminus[j]->Write();
    m_hVector_sumEHFpfplus[j]->Write();
    m_hVector_sumEHFpfminus[j]->Write();
    m_hVector_vertex[j]->Write();
    m_hVector_lumi[j]->Write();
    m_hVector_sumEHFpfplusVsetaMax[j]->Write();
    m_hVector_sumEHFpfminusVsetaMin[j]->Write();
    m_hVector_sumEHFplusVsetaMax[j]->Write();
    m_hVector_sumEHFminusVsetaMin[j]->Write();
    m_hVector_sumECastor[j]->Write();
  }

}

void CastorThreshold::Run(std::string filein_, std::string savehistofile_, std::string processname_, std::string type_){

  TH1::SetDefaultSumw2(true);
  TH2::SetDefaultSumw2(true);

  filein = filein_;
  savehistofile = savehistofile_;
  processname = processname_;
  type = type_;

  TFile check1(filein.c_str());

  if (check1.IsZombie()){

    std::cout << "\n----------------------------------------------" << std::endl;
    std::cout << " There is no PatTuple file or the"   << std::endl;
    std::cout << " path is not correct." << std::endl;
    std::cout << " Edit the source and recompile." << std::endl;
    std::cout << "----------------------------------------------" << std::endl;
    return;

  }

  LoadFile(filein,processname);

  TFile *outf = new TFile(savehistofile.c_str(),"RECREATE");

  int NEVENTS = tr->GetEntries();
  int decade = 0;

  TH1::SetDefaultSumw2(true);
  TH2::SetDefaultSumw2(true);

  unsigned NEntries = tr->GetEntries();
  std::cout << "" << std::endl;
  std::cout<< "Reading Tree: "<< NEntries << " events"<<std::endl;
  std::cout << "" << std::endl;

  std::string status;  

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

  }   

std::cout << "Type: " << status << std::endl;
SaveHistos();
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

  if (argc > 1 && strcmp(s1,argv[1]) != 0)  filein_ = argv[1];
  if (argc > 2 && strcmp(s1,argv[2]) != 0)  savehistofile_  = argv[2];
  if (argc > 3 && strcmp(s1,argv[3]) != 0)  processname_  = argv[3];
  if (argc > 4 && strcmp(s1,argv[4]) != 0)  type_  = argv[4];

  CastorThreshold* castor = new CastorThreshold();   
  castor->CreateHistos();
  castor->Run(filein_, savehistofile_, processname_, type_);

  return 0;
}
#endif
