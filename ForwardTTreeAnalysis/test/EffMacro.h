#ifndef EffMacro_h
#define EffMacro_h

#include <TFile.h>
#include <TTree.h>

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>

class DiffractiveEvent;
class ExclusiveDijetsEvent;
class QCDEvent;

class EffMacro {

   TFile* inf;
   TTree* tr;
   TBranch *diff;
   TBranch *excl;
   TBranch *qcd;
   DiffractiveEvent *eventdiff;
   ExclusiveDijetsEvent *eventexcl;
   QCDEvent *eventqcd;

   std::string filein;
   std::string fileinput;
   std::string processname;
   std::string processinput; 
   std::string savehistofile;
   double jet1PT;
   double jet2PT;
   int optnVertex;
   int optTrigger;
   bool switchPreSel;
   bool switchTrigger;
   int nbins;
   std::vector<TH1D*> m_hVector_Evt_lumis;
   std::vector<TH1D*> m_hVector_Eff_lumis;

   public :
   EffMacro() {}
   ~EffMacro() { inf->Close(); }
   
   void Run(std::string, std::string, std::string, double, double, int, int, bool, bool, int);
   void LoadFile(std::string,std::string);
   void FillHistograms();

};
#endif
