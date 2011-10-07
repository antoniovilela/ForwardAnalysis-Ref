
#include "ForwardAnalysis/ForwardTTreeAnalysis/interface/DijetsTriggerEvent.h"

const char* DijetsTriggerEvent::name = "DijetsTriggerEvent";

DijetsTriggerEvent::DijetsTriggerEvent() {}

DijetsTriggerEvent::~DijetsTriggerEvent() {}

void DijetsTriggerEvent::reset(){
   nL1Bits_ = -1;

   l1Decision_.clear();
   l1Prescale_.clear();
   l1AlgoName_.clear();
   
   hfRingEtSum_.clear();
   hfRingBitCount_.clear();
   hfRingEtSum_ = std::vector<int>(4,-1);
   hfRingBitCount_ = std::vector<int>(4,-1);
}
