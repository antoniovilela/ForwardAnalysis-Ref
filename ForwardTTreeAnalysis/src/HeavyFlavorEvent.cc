#include "ForwardAnalysis/ForwardTTreeAnalysis/interface/HeavyFlavorEvent.h"
#include <cstdio>

const char* HeavyFlavorEvent::name = "HeavyFlavorEvent";

HeavyFlavorEvent::HeavyFlavorEvent() {}

HeavyFlavorEvent::~HeavyFlavorEvent() {}

void HeavyFlavorEvent::reset(){
 
  leadingJetPt_ = -999.;
  leadingJetEta_ = -999.;
  leadingJetPhi_ = -999.;
}
