
#include "ForwardAnalysis/ForwardTTreeAnalysis/interface/PATTriggerInfoEvent.h"

const char* PATTriggerInfoEvent::name = "PATTriggerInfoEvent";

PATTriggerInfoEvent::PATTriggerInfoEvent() {}

PATTriggerInfoEvent::~PATTriggerInfoEvent() {}

void PATTriggerInfoEvent::reset(){
   l1Triggers_.clear();

   HLTTriggers_.clear();

   nHLTbit_ = -1;

   nbit_ = -1;
   //...
}
