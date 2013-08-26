#include "ForwardAnalysis/ForwardTTreeAnalysis/interface/PFCandInfoEvent.h"
#include "ForwardAnalysis/ForwardTTreeAnalysis/interface/PFCand.h"
#include "TLorentzVector.h"

const char* PFCandInfoEvent::name = "PFCandInfoEvent";

PFCandInfoEvent::PFCandInfoEvent() {}

PFCandInfoEvent::~PFCandInfoEvent() {}

void PFCandInfoEvent::reset(){

   //particleId = X;
   //PFCandVector.SetPxPyPzE(0.,0.,0.,0.);
   //Charge = 0;

   PFCandList.clear();
}
