#ifndef EffMacroFWDTrigger_h
#define EffMacroFWDTrigger_h

#include <TFile.h>
#include <TTree.h>

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>


//class DijetsTriggerEvent;
class DiffractiveEvent;
class ExclusiveDijetsEvent;
class QCDEvent;


class EffMacroFWDTrigger {

   TFile* inf;
   TTree* tr;
   TBranch *diff;
   TBranch *excl;
   TBranch *qcd;
   DiffractiveEvent *eventdiff;
   ExclusiveDijetsEvent *eventexcl;
   QCDEvent *eventqcd;
   //TBranch *trigger;

   //DijetsTriggerEvent *eventtrigger;

   std::string filein;
   std::string fileinput;
   std::string processname;
   std::string processinput; 
   std::string savehistofile;
   double jet1PT;
   double jet2PT;
   int optnVertex;
   int triggereffpass;
   double sumEHFPlus;
   double sumEHFMinus;
   int optTrigger;    
   bool switchSelJet1Eff;
   bool switchSelJet2Eff;
   bool switchSelDijetsEff;
   bool switchSelHFPlusEff;
   bool switchSelHFMinusEff;
   bool switchSelHFOREff;
   bool switchSelHFANDEff; 



   public :
   EffMacroFWDTrigger() {}
   ~EffMacroFWDTrigger() { inf->Close(); }
   
   void Run(std::string, std::string, std::string, double, double, int, int, double, double,int, bool, bool, bool, bool, bool, bool, bool);
   void LoadFile(std::string,std::string);
   void FillHistograms();

};
#endif
