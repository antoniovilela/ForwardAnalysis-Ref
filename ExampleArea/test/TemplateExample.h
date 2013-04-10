#ifndef TemplateExample_h
#define TemplateExample_h

#include <TFile.h>
#include <TTree.h>

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>

class DiffractiveEvent;
class ExclusiveDijetsEvent;
class EventInfoEvent;

class TemplateExample {

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

  std::vector<TH1D*> m_hVector_tracks;
  std::vector<TH1D*> m_hVector_vertex;
  std::vector<TH1D*> m_hVector_lumi;

  std::vector <std::string> Folders;

  //
  public :
  TemplateExample() {}
  ~TemplateExample() { inf->Close(); }

  void Run(std::string, std::string, std::string, std::string);
  void LoadFile(std::string,std::string);
  void CreateHistos(std::string);
  void FillHistos(int);
  void SaveHistos();

};
#endif
