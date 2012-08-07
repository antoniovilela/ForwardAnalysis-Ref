#ifndef EffMacroDijetsTrigger_h
#define EffMacroDijetsTrigger_h

#include <TFile.h>
#include <TTree.h>

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>



class DijetsTriggerEvent;

class EffMacroDijetsTrigger {

   TFile* inf;
   TTree* tr;

   TBranch *trigger;

   DijetsTriggerEvent *eventtrigger;

   std::string filein;
   std::string fileinput;
   std::string processname;
   std::string processinput; 
   std::string savehistofile;
   double jet1PT;
   double jet2PT;
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
   EffMacroDijetsTrigger() {}
   ~EffMacroDijetsTrigger() { inf->Close(); }
   
   void Run(std::string, std::string, std::string,double, double, int, double, double, int, bool, bool, bool, bool, bool, bool, bool);
   void LoadFile(std::string,std::string);
   void FillHistograms();

};
#endif

