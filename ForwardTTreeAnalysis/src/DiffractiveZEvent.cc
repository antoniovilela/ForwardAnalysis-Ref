#include "ForwardAnalysis/ForwardTTreeAnalysis/interface/DiffractiveZEvent.h"
#include <cstdio>

const char* DiffractiveZEvent::name = "DiffractiveZEvent";

DiffractiveZEvent::DiffractiveZEvent() {}

DiffractiveZEvent::~DiffractiveZEvent() {}

void DiffractiveZEvent::reset(){

  size_t len_hltTrigResults = sizeof(hltTrigResults_)/sizeof(int);
  for(size_t k = 0; k < len_hltTrigResults; ++k) hltTrigResults_[k] = 0;

  LeadingElectronPt_=-999.;
  LeadingElectronEta_=-999.;
  LeadingElectronPhi_=-999.;
  LeadingElectronCharge_=-999.;
  SecondElectronPt_=-999.;
  SecondElectronEta_=-999.;
  SecondElectronPhi_=-999.;
  SecondElectronCharge_=-999.;
  DiElectronMass_=-999.;
  ElectronsN_=-999;
 
  LeadingMuonPt_=-999.;
  LeadingMuonEta_=-999.;
  LeadingMuonPhi_=-999.;
  LeadingMuonCharge_=-999.;
  SecondMuonPt_=-999.;
  SecondMuonEta_=-999.;
  SecondMuonPhi_=-999.;
  SecondMuonCharge_=-999.;
  DiMuonMass_=-999.;
  MuonsN_=-999;

  VertexMultiplicity_.clear();
  VertexChiNorm_.clear();
  VertexNDOF_.clear();
  Vz_.clear();
  Vx_.clear();
  Vy_.clear();
  TracksPt_.clear();

  PrimaryGapMaxGen_=-999.;
  SecondGapMaxGen_=-999.;
  TracksPtGen_.clear();
  EtaOfTracksPtGen_.clear();
  NTracksGen_=-999;
  Mx2PlusGen_=-999.;
  Mx2MinusGen_=-999.;
  Mx2Gen_=-999.;
  Mx2ZGen_=-999.;
  NMx2PlusGen_=-999;
  NMx2MinusGen_=-999;
  EtaGaplimPlusGen_=-999.;
  EtaGaplimMinusGen_=-999.;
  NParticlesGen_=-999;
  sumECastorMinusGen_=-999.;
  sumECastorPlusGen_=-999.;
  sumEZDCMinusGen_=-999.;
  sumEZDCPlusGen_=-999.;
  EtaOutcomingProtonGen_=-999.;
  xLGen_=-999.;
  xLMostEnergeticGen_=-999.;
  xiZMinusGen_=-999.;
  xiZPlusGen_=-999.;
  EtaZGen_=-999.;
  EnergyZGen_=-999.;
  pDissMassGen_=-999.;
  xLpDissMass_=-999.;

}
