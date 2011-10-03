#ifndef PATInfoEvent_h
#define PATInfoEvent_h

#include "ForwardAnalysis/ForwardTTreeAnalysis/interface/PATL1Trigger.h"
#include "ForwardAnalysis/ForwardTTreeAnalysis/interface/PATHLTTrigger.h"
#include <string>
#include <vector>

namespace patInfo {
  class PATInfo;
}

class PATInfoEvent {
   public:
      typedef patInfo::PATInfo type;

      PATInfoEvent();
      ~PATInfoEvent();
      //------------ Set FWD methods ------------------------------


      void SetNBit(int fnbit)                                              { nbit_  =  fnbit;}
      void SetL1Trigger(const PATL1Trigger& l1trigInfo)                     { l1Triggers_.push_back(l1trigInfo); }  

      void SetNHLTBit(int fnHLTbit)                                                { nHLTbit_  =  fnHLTbit;}
      void SetHLTTrigger(const PATHLTTrigger& HLTtrigInfo)                         { HLTTriggers_.push_back(HLTtrigInfo); }  
     
      //
      //------------ Get FWD methods ------------------------------

      int GetNBit()                   const{ return nbit_;}
      int GetNHLTBit()                   const{ return nHLTbit_;}

      PATL1Trigger GetL1Trigger(int itrig) const { return l1Triggers_[itrig]; }
      PATHLTTrigger GetHLTTrigger(int iHLTtrig) const { return HLTTriggers_[iHLTtrig]; }
      

   private:
      friend class patInfo::PATInfo;
      void reset();

      int nbit_, nHLTbit_;

      std::vector<PATL1Trigger> l1Triggers_;
      std::vector<PATHLTTrigger> HLTTriggers_;

     

     
      

      //////////////////////////////////
};//end code

#endif
