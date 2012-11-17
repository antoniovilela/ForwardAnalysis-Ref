#include "ForwardAnalysis/ForwardTTreeAnalysis/interface/DiffractiveZEvent.h"
#include <cstdio>

const char* DiffractiveZEvent::name = "DiffractiveZEvent";

DiffractiveZEvent::DiffractiveZEvent() {}

DiffractiveZEvent::~DiffractiveZEvent() {}

void DiffractiveZEvent::reset(){

  size_t len_hltTrigResults = sizeof(hltTrigResults_)/sizeof(int);
  for(size_t k = 0; k < len_hltTrigResults; ++k) hltTrigResults_[k] = 0;

  LeadingElectronPt_=9999;
  LeadingElectronEta_=9999;
  LeadingElectronPhi_=9999;
  LeadingElectronCharge_=9999;
  SecondElectronPt_=9999;
  SecondElectronEta_=9999;
  SecondElectronPhi_=9999;
  SecondElectronCharge_=9999;
  DiElectronMass_=9999;
  ElectronsN_=9999;
 

  LeadingMuonPt_=9999;
  LeadingMuonEta_=9999;
  LeadingMuonPhi_=9999;
  LeadingMuonCharge_=9999;
  SecondMuonPt_=9999;
  SecondMuonEta_=9999;
  SecondMuonPhi_=9999;
  SecondMuonCharge_=9999;
  DiMuonMass_=9999;
  MuonsN_=9999;
 
}
