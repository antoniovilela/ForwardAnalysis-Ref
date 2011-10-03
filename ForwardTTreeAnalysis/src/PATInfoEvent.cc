
#include "ForwardAnalysis/ForwardTTreeAnalysis/interface/PATInfoEvent.h"

PATInfoEvent::PATInfoEvent() {}

PATInfoEvent::~PATInfoEvent() {}

void PATInfoEvent::reset(){
   l1Triggers_.clear();

   HLTTriggers_.clear();

   nHLTbit_ = -1;

   nbit_ = -1;
   //...
}
