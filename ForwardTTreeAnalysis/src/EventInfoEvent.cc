
#include "ForwardAnalysis/ForwardTTreeAnalysis/interface/EventInfoEvent.h"

const char* EventInfoEvent::name = "EventInfoEvent";

EventInfoEvent::EventInfoEvent() {}

EventInfoEvent::~EventInfoEvent() {}

void EventInfoEvent::reset(){
   
  nPileUpBx0_           = -1;
  nPileUpBxm1_          = -1;
  nPileUpBxp1_          = -1;
  nTrueInteractions_    = -1.;

  runNumber_            = -1;
  eventNumber_          = -1;
  lumiSection_          = -1;
  bunchCrossing_        = -1;
  lumiWeight_           = -1.;
}
