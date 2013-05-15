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

  DiElectronMassPF_=-999.;
  DiMuonMassPF_=-999.;

  SumEHFPlus_ = -999.; 
  SumEHF_SPlus_ = -999.; 
  SumEHF_LPlus_ = -999.; 
  SumEtHFPlus_ = -999.; 
  SumEHFMinus_ = -999.; 
  SumEHF_SMinus_ = -999.; 
  SumEHF_LMinus_ = -999.; 
  SumEtHFMinus_ = -999.; 
  SumEHEPlus_ = -999.; 
  SumEtHEPlus_ = -999.; 
  SumEHEMinus_ = -999.; 
  SumEtHEMinus_ = -999.; 
  SumEHBPlus_ = -999.; 
  SumEtHBPlus_ = -999.;   
  SumEHBMinus_ = -999.; 
  SumEtHBMinus_ = -999.; 
  SumEEEPlus_ = -999.;    
  SumEtEEPlus_ = -999.; 
  SumEEEMinus_ = -999.; 
  SumEtEEMinus_ = -999.; 
  SumEEBPlus_ = -999.; 
  SumEtEBPlus_ = -999.; 
  SumEEBMinus_ = -999.; 
  SumEtEBMinus_ = -999.; 
  EPZCaloPlus_ = -999.; 
  EPZCaloMinus_ = -999.; 
  XiCaloPlus_ = -999.; 
  XiCaloMinus_ = -999.; 
  EtaCaloMax_ = -999.; 
  EtaCaloMin_ = -999.; 

  Vertex_ = -999;
  Xi_PF_minus_ = -999.;
  Xi_PF_plus_ = -999.;
  Epz_PF_minus_ = -999.;
  Epz_PF_plus_ = -999.;
  MultiplicityPF_ = -999;
  SumEtaTimesEnergyPF_ = -999.;
  SumpxModulePF_ = -999.;
  SumpyModulePF_ = -999.;
  SumpzModulePF_ = -999.;
  SumpxPF_ = -999.;
  SumpyPF_ = -999.;
  SumpzPF_ = -999.;
  SumEnergyPF_ = -999.;
  MuEnergyPF_ = -999.;
  ElectronEnergyPF_ = -999.;
  MaxGapPF_ = -999.;
  SecondMaxGapPF_ = -999.;
  LimPlusGapPF_ = -999.;
  LimMinusGapPF_ = -999.;
  MultiplicityGapPlusPF_ = -999;
  MultiplicityGapMinusPF_ = -999;

}
