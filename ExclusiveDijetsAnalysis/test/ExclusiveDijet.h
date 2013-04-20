#ifndef ExclusiveDijet_h
#define ExclusiveDijet_h

#include <TFile.h>
#include <TTree.h>

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>

#include "PhysicsTools/Utilities/interface/LumiReWeighting.h"

class DiffractiveEvent;
class ExclusiveDijetsEvent;
class EventInfoEvent;

class ExclusiveDijet {

  TFile* effcut;
  TFile* efftrigger;
  TFile* inf;
  TFile* pudata;
  TFile* pumc;
  TTree* tr;
  TBranch *diff;
  TBranch *excl;
  TBranch *info;
  DiffractiveEvent *eventdiff;
  ExclusiveDijetsEvent *eventexcl;
  EventInfoEvent *eventinfo;

  std::string fileinput;
  std::string processinput;
  int index;
  int pileup;
  int totalweight;
  double deltaphi, aSumE, absdeltaetapf, deltaetapf, ptJet1, ptJet2;

  std::string filein;
  std::string processname;
  std::string savehistofile;
  std::string switchtrigger;
  std::string type;
  std::string jetunc;
  std::string switchpucorr;
  std::string pudatafile;
  std::string pumcfile;
  std::string cutcorrfile;
  std::string triggercorrfile;
  std::string switchcutcorr;
  std::string switchtriggercorr;
  std::string switchlumiweight;
  double lumiweight;
  std::string switchmceventweight;
  int optnVertex;
  int optTrigger;
  double jet1pT;
  double jet2pT;

  std::vector<std::vector<TH1D*> > m_hVector_rjj;
  std::vector<std::vector<TH1D*> > m_hVector_detagen;
  std::vector<std::vector<TH1D*> > m_hVector_mxGen;
  std::vector<std::vector<TH2F*> > m_hVector_multhf;
  std::vector<std::vector<TH2F*> > m_hVector_etcalos;
  std::vector<std::vector<TH1D*> > m_hVector_tracks;
  std::vector<std::vector<TH1D*> > m_hVector_pfetamax;
  std::vector<std::vector<TH1D*> > m_hVector_pfetamin;
  std::vector<std::vector<TH1D*> > m_hVector_asumE;
  std::vector<std::vector<TH1D*> > m_hVector_deltaetajets;
  std::vector<std::vector<TH1D*> > m_hVector_deltaphijets;
  std::vector<std::vector<TH1D*> > m_hVector_deltaptjets;
  std::vector<std::vector<TH1D*> > m_hVector_dijetmass;
  std::vector<std::vector<TH1D*> > m_hVector_ptjet1;
  std::vector<std::vector<TH1D*> > m_hVector_ptjet2;
  std::vector<std::vector<TH1D*> > m_hVector_etajet1;
  std::vector<std::vector<TH1D*> > m_hVector_etajet2;
  std::vector<std::vector<TH1D*> > m_hVector_phijet1;
  std::vector<std::vector<TH1D*> > m_hVector_phijet2;
  std::vector<std::vector<TH1D*> > m_hVector_sumEHFplus;
  std::vector<std::vector<TH1D*> > m_hVector_sumEHFminus;
  std::vector<std::vector<TH1D*> > m_hVector_sumEHEplus;
  std::vector<std::vector<TH1D*> > m_hVector_sumEHEminus;
  std::vector<std::vector<TH1D*> > m_hVector_sumEHFpfplus;
  std::vector<std::vector<TH1D*> > m_hVector_sumEHFpfminus;
  std::vector<std::vector<TH1D*> > m_hVector_sumECastor;
  std::vector<std::vector<TH1D*> > m_hVector_deltaEtaPF;
  std::vector<std::vector<TH1D*> > m_hVector_absdeltaEtaPF;
  std::vector<std::vector<TH1D*> > m_hVector_vertex;
  std::vector<std::vector<TH1D*> > m_hVector_sumEHFplusVsiEta;
  std::vector<std::vector<TH1D*> > m_hVector_sumEHFminusVsiEta;
  std::vector<std::vector<TH1D*> > m_hVector_lumi;
  std::vector<std::vector<TH2D*> > m_hVector_sumEHFplusVsetaMax;
  std::vector<std::vector<TH2D*> > m_hVector_sumEHFminusVsetaMin;
  std::vector<std::vector<TH2D*> > m_hVector_sumEHFpfplusVsetaMax;
  std::vector<std::vector<TH2D*> > m_hVector_sumEHFpfminusVsetaMin;
  std::vector<std::vector<TH1D*> > m_hVector_uncJet1;
  std::vector<std::vector<TH1D*> > m_hVector_uncJet2;

  std::vector <std::string> Folders;

  public :
  ExclusiveDijet() {}
  ~ExclusiveDijet() {
    inf->Close();
  }

  void Run(std::string, std::string, std::string, std::string, std::string, std::string, std::string, std::string, std::string, std::string, std::string, std::string, std::string, std::string, double, std::string, int, int, double, double);
  void LoadFile(std::string,std::string);
  void CreateHistos(std::string);
  void FillHistos(int, int, double);
  void SaveHistos(std::string);
  double* cutCorrection();
  double* triggerCorrection();

};
#endif
