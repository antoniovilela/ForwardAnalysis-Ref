#ifndef EffMacroTrigger2012_h
#define EffMacroTrigger2012_h

#include <TFile.h>
#include <TTree.h>

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>


class DijetsTriggerEvent;

class EffMacroTrigger2012 {

   TFile* inf;
   TTree* tr;

   TBranch *trigger;

   DijetsTriggerEvent *eventtrigger;

   std::string filein;
   std::string fileinput;
   std::string processname;
   std::string processinput; 
   std::string savehistofile;
  


   public :
   EffMacroTrigger2012() {}
   ~EffMacroTrigger2012() { inf->Close(); }
   
   void Run(std::string, std::string, std::string);
   void LoadFile(std::string,std::string);
   void FillHistograms();

};
#endif
