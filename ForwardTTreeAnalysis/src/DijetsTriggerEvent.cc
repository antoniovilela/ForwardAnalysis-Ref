
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
   hltTrigResults_.clear(); 
   hltTrigNames_.clear(); 
 
   hfRingEtSum_.clear();
   hfRingBitCount_.clear();
   hfRingEtSum_ = std::vector<int>(4,-1);
   hfRingBitCount_ = std::vector<int>(4,-1);
  
   leadingJetPt_ = -999.;
   leadingJetEta_ = -999.;
   leadingJetPhi_ = -999.;
  
   secondJetPt_ = -999.;
   secondJetEta_ = -999.;
   secondJetPhi_ = -999.;

   thirdJetPt_ = -999.;
   thirdJetEta_ = -999.;
   thirdJetPhi_ = -999.; 
  
   //ntowCal_ = 0;
   nJet_ = 0;

}
