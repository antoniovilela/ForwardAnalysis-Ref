#include "ForwardAnalysis/ForwardTTreeAnalysis/interface/DiffractiveZEvent.h"
#include <cstdio>

const char* DiffractiveZEvent::name = "DiffractiveZEvent";

DiffractiveZEvent::DiffractiveZEvent() {}

DiffractiveZEvent::~DiffractiveZEvent() {}

void DiffractiveZEvent::reset(){
 
  leadingJetPt_ = -999.;
  leadingJetEta_ = -999.;
  leadingJetPhi_ = -999.;
}
