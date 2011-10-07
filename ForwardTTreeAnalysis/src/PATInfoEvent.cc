
#include "ForwardAnalysis/ForwardTTreeAnalysis/interface/PATInfoEvent.h"

const char* PATInfoEvent::name = "PATInfoEvent";

PATInfoEvent::PATInfoEvent() {}

PATInfoEvent::~PATInfoEvent() {}

void PATInfoEvent::reset(){
   l1Triggers_.clear();

   HLTTriggers_.clear();

   nHLTbit_ = -1;

   nbit_ = -1;
   //...
}
