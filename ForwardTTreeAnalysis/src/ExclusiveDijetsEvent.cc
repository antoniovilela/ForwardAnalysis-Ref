
#include "ForwardAnalysis/ForwardTTreeAnalysis/interface/ExclusiveDijetsEvent.h"

const char* ExclusiveDijetsEvent::name = "ExclusiveDijetsEvent";

ExclusiveDijetsEvent::ExclusiveDijetsEvent() {}

ExclusiveDijetsEvent::~ExclusiveDijetsEvent() {}

void ExclusiveDijetsEvent::reset(){
  size_t len_hltTrigResults = sizeof(hltTrigResults_)/sizeof(int);
  for(size_t k = 0; k < len_hltTrigResults; ++k) hltTrigResults_[k] = 0;
   
  nPileUpBx0_ = -1;
  nPileUpBxm1_ = -1;
  nPileUpBxp1_ = -1;
  nVertex_=  -1;
  lumiWeight_ = 999.;
  nTracksNonCone_ = -999.;

  leadingJetPt_= 999.;
  leadingJetEta_= 999.;
  leadingJetPhi_ = 999.;

  leadingJetBDiscriminator_= 999.;
  secondJetPt_= 999.;
  secondJetEta_= 999.;
  secondJetPhi_= 999.;

  secondJetBDiscriminator_ = 999.;
  thirdJetPt_ = 999.;
  thirdJetEta_ = 999.;
  thirdJetPhi_ = 999.; 

  thirdJetBDiscriminator_ = 999.; 
  jetsAveEta_ = 999.;
  jetsDeltaEta_ = 999.;
  jetsDeltaPhi_ = 999.; 
  jetsDeltaPt_ = 999.; 
  
  trackMultiplicity_ = -1;
     
  multiplicityHFPlus_  = -1;
  multiplicityHFMinus_ = -1;
     
  sumEnergyHFPlus_ = -1;
  sumEnergyHFMinus_ = -1;
     
  multiplicityHBPlus_  = -1;
  multiplicityHBMinus_  = -1;
     
  multiplicityHEPlus_  = -1;
  multiplicityHEMinus_  = -1;
     
  sumEnergyHBPlus_ = 999.;
  sumEnergyHBMinus_ = 999.;

  sumETHBPlus_= 999.;
  sumETHBMinus_= 999.;

  sumEnergyHEPlus_= 999.;
  sumEnergyHEMinus_ = 999.;

  sumETHEMinus_ = 999.;
  sumETHEPlus_ = 999.;

  sumETHFPlus_ = 999.;
  sumETHFMinus_ = 999.;


  size_t len_multiplicityHFPlusVsiEta = sizeof(multiplicityHFPlusVsiEta_)/sizeof(int);
  for(size_t k = 0; k < len_multiplicityHFPlusVsiEta; ++k){
    multiplicityHFPlusVsiEta_[k] = -1;
    multiplicityHFMinusVsiEta_[k] = -1;
    sumEHFMinusVsiEta_[k] = -999.;
    sumETHFPlusVsiEta_[k] = -999.;
    sumETHFMinusVsiEta_[k] = -999.;
  }

  xiGenPlus_  = 999.;
  xiGenMinus_ = 999.;

  xiTowerPlus_ = 999.;
  xiTowerMinus_= 999. ;
  xiPlusFromJets_= 999.;
  xiMinusFromJets_= 999.;
  xiPlusFromPFCands_= 999.;
  xiMinusFromPFCands_= 999.;
  missingMassFromXi_= 999.;
 
  massJets_= 999.;        
  massDijets_= 999.;       
  massTrijets_= 999.;        
  massDijetsGen_= 999.;

  MxFromJets_= 999.;
  MxFromPFCands_= 999.;
  MxGen_= 999.;
  RjFromJets_= 999.;             
  RjjFromJets_= 999.;
  RjjjFromJets_= 999.;      
  RjFromPFCands_= 999.;     
  RjjFromPFCands_= 999.;
  RjjjFromPFCands_= 999.;   
  RjjGen_ = 999.;
  
  sumEnergyHEPlusGen_= 999.;
  sumEnergyHEMinusGen_= 999.;
  sumEnergyHFPlusGen_= 999.;
  sumEnergyHFMinusGen_= 999.; 
  sumETotCastor_= 999.;
  missingMassFromXiFromPFCands_= 999.;
  EPlusPzFromPFCands_= 999.;
  EMinusPzFromPFCands_= 999.;
  MxGenDiss_= 999.; 
  MxGenRange_= 999.;
  MxGenPlus_= 999.;
  MxGenMinus_= 999.;
  deltaEtaGen_= 999.;
  etaGapLow_= 999.;
  etaGapHigh_= 999.;
  etaMaxGen_= 999.;
  etaMinGen_= 999.;
  etaMaxFromPFCands_= 999.;
  etaMinFromPFCands_= 999.;
  etaMaxGenNew_= 999.;
  etaMinGenNew_= 999.;
  etaMaxFromPFCandsNew_= 999.;
  etaMinFromPFCandsNew_= 999.;

  EtaAllTypes_= 999.;
  EnergyAllTypes_= 999.;
  PtAllTypes_= 999.;

  EtaChargedHadron_= 999.;
  EnergyChargedHadron_= 999.;
  PtChargedHadron_= 999.;

  EtaUndefined_= 999.;
  EnergyUndefined_= 999.;
  PtUndefined_= 999.;
 
  EtaElectron_= 999.;
  EnergyElectron_= 999.;
  PtElectron_ = 999;
 
  EtaMuon_= 999.;
  EnergyMuon_= 999.;
  PtMuon_= 999.;

  EtaGamma_= 999.;
  EnergyGamma_= 999.;
  PtGamma_= 999.;

  EtaNeutralHadron_= 999.;
  EnergyNeutralHadron_= 999.;
  PtNeutralHadron_ = 999.;

  EtaHadronHF_= 999.;
  EnergyHadronHF_= 999.;
  PtHadronHF_= 999.;
 
  EtaEGammaHF_= 999.;
  EnergyEGammaHF_= 999.;
  PtEGammaHF_= 999.;

  SumEHFPFlowPlus_= 999.;
  SumEHFPFlowMinus_= 999.;
  
  SumEHFPFlowPlus_Long_Fiber_= 999.;
  SumEHFPFlowMinus_Long_Fiber_= 999.;

  SumEHFPFlowPlus_Short_Fiber_= 999.;
  SumEHFPFlowMinus_Short_Fiber_= 999.;

  EnergyHadronHFPlus_= 999.;
  EnergyEGammaHFPlus_= 999.;
  EnergyHadronHFMinus_= 999.;
  EnergyEGammaHFMinus_= 999.;

    //...
}
