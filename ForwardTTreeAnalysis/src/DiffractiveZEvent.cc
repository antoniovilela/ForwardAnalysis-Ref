#include "ForwardAnalysis/ForwardTTreeAnalysis/interface/DiffractiveZEvent.h"
#include <cstdio>

const char* DiffractiveZEvent::name = "DiffractiveZEvent";

DiffractiveZEvent::DiffractiveZEvent() {}

DiffractiveZEvent::~DiffractiveZEvent() {}

void DiffractiveZEvent::reset(){

  //size_t len_hltTrigResults = sizeof(hltTrigResults_)/sizeof(int);
  //for(size_t k = 0; k < len_hltTrigResults; ++k) hltTrigResults_[k] = 0;

  LeadingElectronPt_=-999;
  LeadingElectronEta_=-999;
  LeadingElectronPhi_=-999;
  LeadingElectronCharge_=-999;
  SecondElectronPt_=-999;
  SecondElectronEta_=-999;
  SecondElectronPhi_=-999;
  SecondElectronCharge_=-999;
  DiElectronMass_=-999;
  ElectronsN_=-999;
 

  LeadingMuonPt_=-999;
  LeadingMuonEta_=-999;
  LeadingMuonPhi_=-999;
  LeadingMuonCharge_=-999;
  SecondMuonPt_=-999;
  SecondMuonEta_=-999;
  SecondMuonPhi_=-999;
  SecondMuonCharge_=-999;
  DiMuonMass_=-999;
  MuonsN_=-999;
 
}
