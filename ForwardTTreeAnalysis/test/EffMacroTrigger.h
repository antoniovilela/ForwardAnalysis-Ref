#ifndef EffMacroTrigger_h
#define EffMacroTrigger_h

#include <TFile.h>
#include <TTree.h>

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>

//class DiffractiveEvent;
//class ExclusiveDijetsEvent;
//class QCDEvent;
class DijetsTriggerEvent;

class EffMacroTrigger {

   TFile* inf;
   TTree* tr;
   //TBranch *diff;
   //TBranch *excl;
   //TBranch *qcd;
   TBranch *trigger;
   //DiffractiveEvent *eventdiff;
   //ExclusiveDijetsEvent *eventexcl;
   //QCDEvent *eventqcd;
   DijetsTriggerEvent *eventtrigger;

   std::string filein;
   std::string fileinput;
   std::string processname;
   std::string processinput; 
   std::string savehistofile;
   double jet1PT;
   double jet2PT;
   int optL1Ref;
   int optTrigger;
   int optTriggerRef;
   bool switchPreSel;
   bool switchTrigger;
   bool switchTriggerRef;
   bool switchL1Ref;
   int nbins;
   std::vector<TH1D*> m_hVector_Evt_lumis;
   std::vector<TH1D*> m_hVector_Eff_lumis;
   std::vector<TH1D*> m_hVector_ptjet1;
   std::vector<TH1D*> m_hVector_ptjet2;
   std::vector<TH1D*> m_hVector_etajet1;
   std::vector<TH1D*> m_hVector_etajet2;
   std::vector<TH1D*> m_hVector_phijet1;
   std::vector<TH1D*> m_hVector_phijet2;
   std::vector<TH1D*> m_hVector_sumEHFplus;
   std::vector<TH1D*> m_hVector_sumEHFminus;
   std::vector<TH1D*> m_hVector_deltaphijets;



   public :
   EffMacroTrigger() {}
   ~EffMacroTrigger() { inf->Close(); }
   
   void Run(std::string, std::string, std::string, double, double, int, int, int, bool, bool, bool, bool, int);
   void LoadFile(std::string,std::string);
   void FillHistograms();

};
#endif
