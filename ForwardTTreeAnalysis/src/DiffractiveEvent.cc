#include "ForwardAnalysis/ForwardTTreeAnalysis/interface/DiffractiveEvent.h"
#include <cstdio>

DiffractiveEvent::DiffractiveEvent() {}

DiffractiveEvent::~DiffractiveEvent() {}

void DiffractiveEvent::reset(){
  /*size_t len_hltTrigResults = sizeof(hltTrigResults_)/sizeof(int);
  for(size_t k = 0; k < len_hltTrigResults; ++k) hltTrigResults_[k] = 0;*/

  processId_ = -1;
  lumiWeight_ = -1.;
  eventNumber_ = -1;
  runNumber_ = -1;
  lumiSection_ = -1;
  bunchCrossing_ = -1;

  LooseNoiseFilter_ = -1;
  TightNoiseFilter_ = -1;
  BeamHaloLooseId_ = -1;
  BeamHaloTightId_ = -1;

  HLTPath_ = -1;
  TTBit_ = -1;

  nVertex_ = -1;
  posXPrimVtx_ = -999.;
  posYPrimVtx_ = -999.;
  posZPrimVtx_ = -999.;

  multiplicityTracks_ = -1;
  sumPtTracks_ = -1.;

  multiplicityTracksGen_ = -1;
  sumPtTracksGen_ = -1.;

  sumET_ = -999.; 
  MET_ = -999.;
  MEx_ = -999.;
  MEy_ = -999.;

  leadingJetPt_ = -999.;
  leadingJetEta_ = -999.;
  leadingJetPhi_ = -999.;

  multiplicityHEPlus_ = -1;
  multiplicityHEMinus_ = -1;
  multiplicityHFPlus_ = -1; 
  multiplicityHFMinus_ = -1;
  sumEnergyHEPlus_ = -999.;
  sumEnergyHEMinus_ = -999.;
  sumETHEPlus_ = -999.;
  sumETHEMinus_ = -999.;
  sumEnergyHFPlus_ = -999.;
  sumEnergyHFMinus_ = -999.;
  sumETHFPlus_ = -999.;
  sumETHFMinus_ = -999.;

  size_t len_multiplicityHFPlusVsiEta = sizeof(multiplicityHFPlusVsiEta_)/sizeof(int);
  for(size_t k = 0; k < len_multiplicityHFPlusVsiEta; ++k){
     multiplicityHFPlusVsiEta_[k] = -1;
     multiplicityHFMinusVsiEta_[k] = -1;
     sumEHFPlusVsiEta_[k] = -999.;
     sumEHFMinusVsiEta_[k] = -999.;
     sumETHFPlusVsiEta_[k] = -999.;
     sumETHFMinusVsiEta_[k] = -999.;
  }

  xiGenPlus_ = -1.;
  xiGenMinus_ = -1.;
  MxGen_ = -1.;
  MxGenDiss_ = -1.;
  MxGenRange_ = -1.;
  MxGenPlus_ = -1.;
  MxGenMinus_ = -1.;
  deltaEtaGen_ = -1.;
  etaGapLow_ = -999.;
  etaGapHigh_ = -999.;
  sumEnergyHEPlusGen_ = -1.;
  sumEnergyHEMinusGen_ = -1.;
  sumEnergyHFPlusGen_ = -1.;
  sumEnergyHFMinusGen_ = -1.;
  etaMaxGen_ = -999.;
  etaMinGen_ = -999.;

  MxFromTowers_ = -999.;
  MxFromPFCands_ = -999.;
  xiPlusFromTowers_ = -999.;
  xiMinusFromTowers_ = -999.;
  xiPlusFromPFCands_ = -999.;
  xiMinusFromPFCands_ = -999.;
  EPlusPzFromTowers_ = -999.;
  EMinusPzFromTowers_ = -999.;
  EPlusPzFromPFCands_ = -999.;
  EMinusPzFromPFCands_ = -999.;
  etaMaxFromPFCands_ = -999.;
  etaMinFromPFCands_ = -999.;

  sumETotCastor_ = -999.;
}
