#ifndef MC_PU_Distributions_h
#define MC_PU_Distributions_h

#include <TFile.h>
#include <TTree.h>

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>

class DiffractiveEvent;
class ExclusiveDijetsEvent;
class QCDEvent;

class MC_PU_Distributions {

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
   bool switchWeightLumi;
   bool switchWeightePw;
   bool switchPreSel;
   bool switchTrigger;
   double weightlumipass;
   int nbins;
   std::vector<TH1D*> m_hVector_pumcbx0;
   std::vector<TH1D*> m_hVector_pumcbxm1;
   std::vector<TH1D*> m_hVector_pumcbxp1;

   public :
   MC_PU_Distributions() {}
   ~MC_PU_Distributions() { inf->Close(); }
   
   void Run(std::string, std::string, std::string, double, double, int, int, bool, bool, bool, bool, double, int);
   void LoadFile(std::string,std::string);
   void FillHistograms();

};
#endif
