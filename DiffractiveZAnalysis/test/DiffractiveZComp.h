#ifndef DiffractiveZComp_h
#define DiffractiveZComp_h

#include <TFile.h>
#include <TTree.h>

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>

class DiffractiveEvent;
class DiffractiveZEvent;
class EventInfoEvent;

class DiffractiveZComp {

   TFile* inf;
   TTree* tr;
   TBranch *diff;
   TBranch *diffZ;
   TBranch *info;
   DiffractiveEvent *eventdiff;
   DiffractiveZEvent *eventdiffZ;
   EventInfoEvent *eventinfo;

   std::string filein;
   std::string savehistofile;
   std::string ttreename;
   int nVertex;
   int optTrigger;
   bool switchPUMultiple;
   bool switchPreSel;
   bool switchTrigger;

   std::vector<std::vector<TH1D*> > m_hVector_mll;
   
   public :
   DiffractiveZComp() {}
   ~DiffractiveZComp() { inf->Close(); }
   
   void Run(std::string, std::string, std::string, bool, int, bool, int, bool);
   void LoadFile(std::string,std::string);
   void FillHistograms();

};
#endif
