#ifndef CastorThreshold_h
#define CastorThreshold_h

#include <TFile.h>
#include <TTree.h>

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>

class DiffractiveEvent;
class ExclusiveDijetsEvent;
class EventInfoEvent;

class CastorThreshold {

  TFile* inf;
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

  std::string filein;
  std::string processname;
  std::string savehistofile;
  std::string type;
  bool switchPreSel;

  std::vector<TH1D*> m_hVector_tracks;
  std::vector<TH1D*> m_hVector_pfetamax;
  std::vector<TH1D*> m_hVector_pfetamin;
  std::vector<TH1D*> m_hVector_sumEHFplus;
  std::vector<TH1D*> m_hVector_sumEHFminus;
  std::vector<TH1D*> m_hVector_sumEHFpfplus;
  std::vector<TH1D*> m_hVector_sumEHFpfminus;
  std::vector<TH1D*> m_hVector_sumECastor;
  std::vector<TH1D*> m_hVector_vertex;
  std::vector<TH1D*> m_hVector_lumi;
  std::vector<TH2D*> m_hVector_sumEHFplusVsetaMax;
  std::vector<TH2D*> m_hVector_sumEHFminusVsetaMin;
  std::vector<TH2D*> m_hVector_sumEHFpfplusVsetaMax;
  std::vector<TH2D*> m_hVector_sumEHFpfminusVsetaMin; 

  std::vector <std::string> Folders;

  //
  public :
  CastorThreshold() {}
  ~CastorThreshold() { inf->Close(); }

  void Run(std::string, std::string, std::string, std::string);
  void LoadFile(std::string,std::string);
  void CreateHistos();
  void FillHistos(int);
  void SaveHistos();

};
#endif
