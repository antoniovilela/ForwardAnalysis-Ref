#ifndef DiffractiveZ_h
#define DiffractiveZ_h

#include <TFile.h>
#include <TTree.h>

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>

#include "PhysicsTools/Utilities/interface/LumiReWeighting.h"

class DiffractiveEvent;
class DiffractiveZEvent;
class EventInfoEvent;

class DiffractiveZ {

  TFile* effcut;
  TFile* efftrigger;
  TFile* inf;
  TFile* pudata;
  TFile* pumc;
  TTree* tr;
  TBranch *diff;
  TBranch *diffZ;
  TBranch *info;
  DiffractiveEvent *eventdiff;
  DiffractiveZEvent *eventdiffZ;
  EventInfoEvent *eventinfo;

  std::string fileinput;
  std::string processinput;
  int index;
  int pileup;
  int totalweight;
  double aSumE;

  double deltaphielectrons;
  double deltaphimuons;
  double deltaetaelectrons;
  double deltaetamuons;
  double deltapTelectrons;
  double deltapTmuons;

  std::string filein;
  std::string processname;
  std::string savehistofile;
  std::string switchtrigger;
  std::string type;
  std::string switchlumiweight;
  double mcweight;
  int nVertex;
  int optTrigger;
  double lepton1pt;
  double lepton2pt;

  std::vector<std::vector<TH1D*> > m_hVector_DiElectron;
  std::vector<std::vector<TH1D*> > m_hVector_LeadingElectronPt;
  std::vector<std::vector<TH1D*> > m_hVector_LeadingElectronEta;
  std::vector<std::vector<TH1D*> > m_hVector_LeadingElectronPhi;
  std::vector<std::vector<TH1D*> > m_hVector_LeadingElectronCharge;
  std::vector<std::vector<TH1D*> > m_hVector_SecondElectronPt;
  std::vector<std::vector<TH1D*> > m_hVector_SecondElectronEta;
  std::vector<std::vector<TH1D*> > m_hVector_SecondElectronPhi;
  std::vector<std::vector<TH1D*> > m_hVector_SecondElectronCharge;
  std::vector<std::vector<TH1D*> > m_hVector_ElectronsN;

  std::vector<std::vector<TH1D*> > m_hVector_DiMuon;
  std::vector<std::vector<TH1D*> > m_hVector_LeadingMuonPt;
  std::vector<std::vector<TH1D*> > m_hVector_LeadingMuonEta;
  std::vector<std::vector<TH1D*> > m_hVector_LeadingMuonPhi;
  std::vector<std::vector<TH1D*> > m_hVector_LeadingMuonCharge;
  std::vector<std::vector<TH1D*> > m_hVector_SecondMuonPt;
  std::vector<std::vector<TH1D*> > m_hVector_SecondMuonEta;
  std::vector<std::vector<TH1D*> > m_hVector_SecondMuonPhi;
  std::vector<std::vector<TH1D*> > m_hVector_SecondMuonCharge;
  std::vector<std::vector<TH1D*> > m_hVector_MuonsN;

  std::vector<std::vector<TH1D*> > m_hVector_sumEHFplus;
  std::vector<std::vector<TH1D*> > m_hVector_sumEHFminus;
  std::vector<std::vector<TH1D*> > m_hVector_sumEHEplus;
  std::vector<std::vector<TH1D*> > m_hVector_sumEHEminus;

  std::vector<std::vector<TH1D*> > m_hVector_lumi;
  std::vector<std::vector<TH1D*> > m_hVector_asumE;
  std::vector<std::vector<TH2F*> > m_hVector_multhf;
  std::vector<std::vector<TH2F*> > m_hVector_etcalos;
  std::vector<std::vector<TH1D*> > m_hVector_tracks;
  std::vector<std::vector<TH1D*> > m_hVector_pfetamax;
  std::vector<std::vector<TH1D*> > m_hVector_pfetamin;
  std::vector<std::vector<TH1D*> > m_hVector_vertex;

  std::vector<std::vector<TH1D*> > m_hVector_deltaphielectrons;
  std::vector<std::vector<TH1D*> > m_hVector_deltaphimuons;
  std::vector<std::vector<TH1D*> > m_hVector_deltapTelectrons;
  std::vector<std::vector<TH1D*> > m_hVector_deltapTmuons;
  std::vector<std::vector<TH1D*> > m_hVector_deltaetaelectrons;
  std::vector<std::vector<TH1D*> > m_hVector_deltaetamuons;

  std::vector<std::vector<TH2F*> > m_hVector_vertexvslumi;

  std::vector<std::vector<TH1F*> > m_hVector_patNElectron;
  std::vector<std::vector<TH1F*> > m_hVector_patElectron1Pt;
  std::vector<std::vector<TH1F*> > m_hVector_patElectron1Eta;
  std::vector<std::vector<TH1F*> > m_hVector_patElectron1Phi;
  std::vector<std::vector<TH1D*> > m_hVector_patElectron1Charge;
  std::vector<std::vector<TH1F*> > m_hVector_patElectron1Et;
  std::vector<std::vector<TH1F*> > m_hVector_patNMuon;
  std::vector<std::vector<TH1F*> > m_hVector_patMuon1Pt;
  std::vector<std::vector<TH1F*> > m_hVector_patMuon1Eta;
  std::vector<std::vector<TH1F*> > m_hVector_patMuon1Phi;
  std::vector<std::vector<TH1D*> > m_hVector_patMuon1Charge;
  std::vector<std::vector<TH1F*> > m_hVector_patMuon1Et;
  std::vector<std::vector<TH1F*> > m_hVector_patElectron2Pt;
  std::vector<std::vector<TH1F*> > m_hVector_patElectron2Eta;
  std::vector<std::vector<TH1F*> > m_hVector_patElectron2Phi;
  std::vector<std::vector<TH1D*> > m_hVector_patElectron2Charge;
  std::vector<std::vector<TH1F*> > m_hVector_patElectron2Et;
  std::vector<std::vector<TH1F*> > m_hVector_patMuon2Pt;
  std::vector<std::vector<TH1F*> > m_hVector_patMuon2Eta;
  std::vector<std::vector<TH1F*> > m_hVector_patMuon2Phi;
  std::vector<std::vector<TH1D*> > m_hVector_patMuon2Charge;
  std::vector<std::vector<TH1F*> > m_hVector_patMuon2Et;
  std::vector<std::vector<TH1F*> > m_hVector_patDiElectronMass;
  std::vector<std::vector<TH1F*> > m_hVector_patDiMuonMass;
  std::vector<std::vector<TH1F*> > m_hVector_patElectron1TkDr03;
  std::vector<std::vector<TH1F*> > m_hVector_patElectron1TkDr04;
  std::vector<std::vector<TH1F*> > m_hVector_patElectron1EcalDr03;
  std::vector<std::vector<TH1F*> > m_hVector_patElectron1EcalDr04;
  std::vector<std::vector<TH1F*> > m_hVector_patElectron1HcalDr03;
  std::vector<std::vector<TH1F*> > m_hVector_patElectron1HcalDr04;
  std::vector<std::vector<TH1F*> > m_hVector_patElectron2TkDr03;
  std::vector<std::vector<TH1F*> > m_hVector_patElectron2TkDr04;
  std::vector<std::vector<TH1F*> > m_hVector_patElectron2EcalDr03;
  std::vector<std::vector<TH1F*> > m_hVector_patElectron2EcalDr04;
  std::vector<std::vector<TH1F*> > m_hVector_patElectron2HcalDr03;
  std::vector<std::vector<TH1F*> > m_hVector_patElectron2HcalDr04;
  std::vector<std::vector<TH1F*> > m_hVector_patElectron1relIsoDr03;
  std::vector<std::vector<TH1F*> > m_hVector_patElectron1relIsoDr04;
  std::vector<std::vector<TH1F*> > m_hVector_patElectron2relIsoDr03;
  std::vector<std::vector<TH1F*> > m_hVector_patElectron2relIsoDr04;
  std::vector<std::vector<TH1F*> > m_hVector_patMuon1SumPtR03;
  std::vector<std::vector<TH1F*> > m_hVector_patMuon1SumPtR05;
  std::vector<std::vector<TH1F*> > m_hVector_patMuon1EmEtR03;
  std::vector<std::vector<TH1F*> > m_hVector_patMuon1EmEtR05;
  std::vector<std::vector<TH1F*> > m_hVector_patMuon1HadEtR03;
  std::vector<std::vector<TH1F*> > m_hVector_patMuon1HadEtR05;
  std::vector<std::vector<TH1F*> > m_hVector_patMuon2SumPtR03;
  std::vector<std::vector<TH1F*> > m_hVector_patMuon2SumPtR05;
  std::vector<std::vector<TH1F*> > m_hVector_patMuon2EmEtR03;
  std::vector<std::vector<TH1F*> > m_hVector_patMuon2EmEtR05;
  std::vector<std::vector<TH1F*> > m_hVector_patMuon2HadEtR03;
  std::vector<std::vector<TH1F*> > m_hVector_patMuon2HadEtR05;
  std::vector<std::vector<TH1F*> > m_hVector_patMuon1relIsoDr03;
  std::vector<std::vector<TH1F*> > m_hVector_patMuon1relIsoDr05;
  std::vector<std::vector<TH1F*> > m_hVector_patMuon2relIsoDr03;
  std::vector<std::vector<TH1F*> > m_hVector_patMuon2relIsoDr05;
  std::vector<std::vector<TH1F*> > m_hVector_patMuon1relIso;
  std::vector<std::vector<TH1F*> > m_hVector_patMuon2relIso;

  std::vector <std::string> Folders;

  public :
  DiffractiveZ() {}
  ~DiffractiveZ() {
    inf->Close();
  }

  void Run(std::string, std::string, std::string, std::string, int, double, double, int, std::string, std::string, float);
  void LoadFile(std::string,std::string);
  void CreateHistos(std::string);
  void FillHistos(int, int, double);
  void SaveHistos(std::string);

};
#endif
