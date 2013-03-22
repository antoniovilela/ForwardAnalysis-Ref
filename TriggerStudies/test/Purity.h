#ifndef Purity_h
#define Purity_h

#include <TFile.h>
#include <TTree.h>

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>

class DiffractiveEvent;
class ExclusiveDijetsEvent;
class EventInfoEvent;

class Purity {

  TFile* inf;
  TTree* tr;
  TBranch *diff;
  TBranch *excl;
  TBranch *info;
  DiffractiveEvent *eventdiff;
  ExclusiveDijetsEvent *eventexcl;
  EventInfoEvent *eventinfo;

  std::string filein;
  std::string fileinput;
  std::string processname;
  std::string processinput; 
  std::string savehistofile;
  int optnVertex;
  int optTrigger;
  bool switchPreSel;
  bool switchTrigger;
  bool switchVertex;
  std::vector<TH1D*> m_hVector_Evt_lumis;
  std::vector<TH1D*> m_hVector_Eff_lumis;
  std::vector<TH1D*> m_hVector_Evt_preplus;
  std::vector<TH1D*> m_hVector_Eff_preplus;
  std::vector<TH1D*> m_hVector_Evt_preminus;
  std::vector<TH1D*> m_hVector_Eff_preminus;
  std::vector<TH1D*> m_hVector_Evt_vertex;
  std::vector<TH1D*> m_hVector_Eff_vertex;
  std::vector<TH1D*> m_hVector_Evt_jet1pt;
  std::vector<TH1D*> m_hVector_Eff_jet1pt;
  std::vector<TH1D*> m_hVector_Evt_jet1phi;
  std::vector<TH1D*> m_hVector_Eff_jet1phi;
  std::vector<TH1D*> m_hVector_Evt_jet1eta;
  std::vector<TH1D*> m_hVector_Eff_jet1eta;
  std::vector<TH1D*> m_hVector_Evt_jet2pt;
  std::vector<TH1D*> m_hVector_Eff_jet2pt;
  std::vector<TH1D*> m_hVector_Evt_jet2phi;
  std::vector<TH1D*> m_hVector_Eff_jet2phi;
  std::vector<TH1D*> m_hVector_Evt_jet2eta;
  std::vector<TH1D*> m_hVector_Eff_jet2eta;
  std::vector<TH1D*> m_hVector_Evt_deltaphi;
  std::vector<TH1D*> m_hVector_Eff_deltaphi;


  public :
  Purity() {}
  ~Purity() { inf->Close(); }

  void Run(std::string, std::string, std::string, int, int, bool, bool, bool);
  void LoadFile(std::string,std::string);
  void FillHistograms();

};
#endif
