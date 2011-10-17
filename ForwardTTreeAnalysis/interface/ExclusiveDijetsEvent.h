#ifndef ExclusiveDijetsEvent_h
#define ExclusiveDijetsEvent_h

#include "DataFormats/JetReco/interface/Jet.h"


namespace exclusiveDijetsAnalysis {
  class ExclusiveDijetsAnalysis;
}

class ExclusiveDijetsEvent {
  public:

  typedef exclusiveDijetsAnalysis::ExclusiveDijetsAnalysis analysis_type;
  static const char* name;

  typedef reco::Particle::LorentzVector LorentzVector;

  ExclusiveDijetsEvent();
  ~ExclusiveDijetsEvent();

  //------------ Set FWD methods ------------------------------
  void SetNPileUpBxm1(int fnPileUpBxm1)           { nPileUpBxm1_       = fnPileUpBxm1;}
  void SetNPileUpBx0(int fnPileUpBx0)             { nPileUpBx0_       = fnPileUpBx0;}
  void SetNPileUpBxp1(int fnPileUpBxp1)           { nPileUpBxp1_       = fnPileUpBxp1;}
  void SetNVertex(int fnVertex)                 { nVertex_          = fnVertex;}

  //added by eliza
  void SetRunNumber(int frunNumber)             { runNumber_        = frunNumber;}   
  void SetEventNumber(int feventNumber)         { eventNumber_      = feventNumber;}
  void SetLumiSection(int flumiSection)         { lumiSection_      = flumiSection;}
  void SetBunchCrossing(int fbunchCrossing)     { bunchCrossing_    = fbunchCrossing;}
  void SetLumiWeight(double flumiWeight)        { lumiWeight_       = flumiWeight;}
  //void SetHLT_Jet15U(int fHLT_Jet15U)           { HLT_Jet15U_       = fHLT_Jet15U;}
  //void SetHLT_L1Jet6U(int fHLT_L1Jet6U)         { HLT_L1Jet6U_      = fHLT_L1Jet6U;}
  void SetHLTPath(int idx, int fHLTBit)         { hltTrigResults_[idx] = fHLTBit;}

  void SetSumEnergyHEPlusGen(double fsumEnergyHEPlusGen)                       { sumEnergyHEPlusGen_  =fsumEnergyHEPlusGen;}
  void SetSumEnergyHEMinusGen(double fsumEnergyHEMinusGen)                     { sumEnergyHEMinusGen_ =fsumEnergyHEMinusGen;}
  void SetSumEnergyHFPlusGen(double fsumEnergyHFPlusGen)                       { sumEnergyHFPlusGen_  =fsumEnergyHFPlusGen;}
  void SetSumEnergyHFMinusGen(double fsumEnergyHFMinusGen)                     { sumEnergyHFMinusGen_ =fsumEnergyHFMinusGen;}
  void SetSumETotCastor(double fsumETotCastor)                                 { sumETotCastor_       =fsumETotCastor;}

  void SetMissingMassFromXiFromPFCands(double fmissingMassFromXiFromPFCands)   { missingMassFromXiFromPFCands_ =fmissingMassFromXiFromPFCands;}
  void SetEPlusPzFromPFCands(double fEPlusPzFromPFCands)                       { EPlusPzFromPFCands_ =fEPlusPzFromPFCands;}
  void SetEMinusPzFromPFCands(double fEMinusPzFromPFCands)                     { EMinusPzFromPFCands_ =fEMinusPzFromPFCands;}
  //eliza
  void SetMxGenDiss(double fMxGenDiss)                                         { MxGenDiss_ =fMxGenDiss;} 
  void SetMxGenRange(double fMxGenRange)                                       { MxGenRange_ =fMxGenRange;}
 
  void SetMxGenPlus (double fMxGenPlus)                                        { MxGenPlus_ =fMxGenPlus;}
  void SetMxGenMinus   (double fMxGenMinus)                                    { MxGenMinus_ =fMxGenMinus;}
  void SetDeltaEtaGen(double fdeltaEtaGen)                                     { deltaEtaGen_ =fdeltaEtaGen;}
  void SetEtaGapLow(double fetaGapLow)                                         { etaGapLow_ =fetaGapLow;}
  void SetEtaGapHigh(double fetaGapHigh)                                       { etaGapHigh_ =fetaGapHigh;}
  void SetEtaMaxGen(double fetaMaxGen)                                         { etaMaxGen_ =fetaMaxGen;}
  void SetEtaMinGen(double fetaMinGen)                                         { etaMinGen_ =fetaMinGen;}
  void SetEtaMaxFromPFCands(double fetaMaxFromPFCands)                         { etaMaxFromPFCands_ =fetaMaxFromPFCands;}
  void SetEtaMinFromPFCands(double fetaMinFromPFCands)                         { etaMinFromPFCands_ =fetaMinFromPFCands;}
  void SetEtaMaxGenNew(double fetaMaxGenNew)                                   { etaMaxGenNew_ =fetaMaxGenNew;}
  void SetEtaMinGenNew(double fetaMinGenNew)                                   { etaMinGenNew_ =fetaMinGenNew;}
  void SetEtaMaxFromPFCandsNew(double fetaMaxFromPFCandsNew)                   { etaMaxFromPFCandsNew_ =fetaMaxFromPFCandsNew;}
  void SetEtaMinFromPFCandsNew(double fetaMinFromPFCandsNew)                   { etaMinFromPFCandsNew_ =fetaMinFromPFCandsNew;}

  void SetEtaAllTypes(double fEtaAllTypes)                                     { EtaAllTypes_ =fEtaAllTypes;}
  void SetEnergyAllTypes(double fEnergyAllTypes)                               { EnergyAllTypes_ =fEnergyAllTypes;}
  void SetPtAllTypes(double fPtAllTypes)                                       { PtAllTypes_ =fPtAllTypes;}

  void SetEtaChargedHadron(double fEtaChargedHadron)                           { EtaChargedHadron_ =fEtaChargedHadron;}
  void SetEnergyChargedHadron(double fEnergyChargedHadron)                     { EnergyChargedHadron_ =fEnergyChargedHadron;}
  void SetPtChargedHadron(double fPtChargedHadron)                             { PtChargedHadron_ =fPtChargedHadron;}

  void SetEtaUndefined(double fEtaUndefined)                                   { EtaUndefined_ =fEtaUndefined;}
  void SetEnergyUndefined(double fEnergyUndefined)                             { EnergyUndefined_ =fEnergyUndefined;}
  void SetPtUndefined(double fPtUndefined)                                     { PtUndefined_ =fPtUndefined;}
 
  void SetEtaElectron(double fEtaElectron)                                     { EtaElectron_ =fEtaElectron;}
  void SetEnergyElectron(double fEnergyElectron)                               { EnergyElectron_ =fEnergyElectron;}
  void SetPtElectron(double fPtElectron)                                       { PtElectron_ =fPtElectron;}
 
  void SetEtaMuon(double fEtaMuon)                                             { EtaMuon_ =fEtaMuon;}
  void SetEnergyMuon(double fEnergyMuon)                                       { EnergyMuon_ =fEnergyMuon;}
  void SetPtMuon(double fPtMuon)                                               { PtMuon_ =fPtMuon;}

  void SetEtaGamma(double fEtaGamma)                                           { EtaGamma_ =fEtaGamma;}
  void SetEnergyGamma(double fEnergyGamma)                                     { EnergyGamma_ =fEnergyGamma;}
  void SetPtGamma(double fPtGamma)                                             { PtGamma_ =fPtGamma;}

  void SetEtaNeutralHadron(double fEtaNeutralHadron)                           { EtaNeutralHadron_ =fEtaNeutralHadron;}
  void SetEnergyNeutralHadron(double fEnergyNeutralHadron)                     { EnergyNeutralHadron_ =fEnergyNeutralHadron;}
  void SetPtNeutralHadron(double fPtNeutralHadron)                             { PtNeutralHadron_ =fPtNeutralHadron;}

  void SetEtaHadronHF(double fEtaHadronHF)                                     { EtaHadronHF_ =fEtaHadronHF;}
  void SetEnergyHadronHF(double fEnergyHadronHF)                               { EnergyHadronHF_ =fEnergyHadronHF;}
  void SetPtHadronHF(double fPtHadronHF)                                       { PtHadronHF_ =fPtHadronHF;}
 
  void SetEtaEGammaHF(double fEtaEGammaHF)                                     { EtaEGammaHF_ =fEtaEGammaHF;}
  void SetEnergyEGammaHF(double fEnergyEGammaHF)                               { EnergyEGammaHF_ =fEnergyEGammaHF;}
  void SetPtEGammaHF(double fPtEGammaHF)                                       { PtEGammaHF_ =fPtEGammaHF;}

  void SetSumEHFPFlowPlus(double fSumEHFPFlowPlus)                             { SumEHFPFlowPlus_ =fSumEHFPFlowPlus;}
  void SetSumEHFPFlowMinus(double fSumEHFPFlowMinus)                           { SumEHFPFlowMinus_ =fSumEHFPFlowMinus;}
  //

  //------ jet 4-momentum vector------------------
  void SetLeadingJetP4(LorentzVector fleadingJetP4)             {leadingJetP4_ = fleadingJetP4 ;}
  void SetSecondJetP4(LorentzVector fsecondJetP4)               {secondJetP4_  = fsecondJetP4  ;}
  void SetThirdJetP4(LorentzVector fthirdJetP4)                 {thirdJetP4_   = fthirdJetP4   ;}

  void SetLeadingJetPt(double fleadingJetPt)    { leadingJetPt_     = fleadingJetPt;}
  void SetLeadingJetEta(double fleadingJetEta)  { leadingJetEta_     = fleadingJetEta;}
  void SetLeadingJetPhi(double fleadingJetPhi)  { leadingJetPhi_    = fleadingJetPhi;}

  void SetSecondJetPt(double fsecondJetPt)      { secondJetPt_      = fsecondJetPt;}
  void SetSecondJetEta(double fsecondJetEta)    { secondJetEta_     = fsecondJetEta;}
  void SetSecondJetPhi(double fsecondJetPhi)    { secondJetPhi_     = fsecondJetPhi;}

  void SetThirdJetPt(double fthirdJetPt)        { thirdJetPt_        = fthirdJetPt;}
  void SetThirdJetEta(double fthirdJetEta)      { thirdJetEta_       = fthirdJetEta;}
  void SetThirdJetPhi(double fthirdJetPhi)      { thirdJetPhi_       = fthirdJetPhi;} //added by Diego

  void SetJetsAveEta(double fjetsAveEta)        { jetsAveEta_       = fjetsAveEta;}
  void SetJetsDeltaEta(double fjetsDeltaEta)    { jetsDeltaEta_     = fjetsDeltaEta;}  //redundant
  void SetJetsDeltaPhi(double fjetsDeltaPhi)    { jetsDeltaPhi_     = fjetsDeltaPhi;}  //redundant
  void SetJetsDeltaPt(double fjetsDeltaPt)      { jetsDeltaPt_      = fjetsDeltaPt;}  //redundant
 
  void SetTrackMultiplicity(int ftrackMultiplicity)     {trackMultiplicity_ = ftrackMultiplicity;}

  void SetMultiplicityHFPlus(int fmultiplicityHFPlus)   {multiplicityHFPlus_ = fmultiplicityHFPlus;}
  void SetMultiplicityHFMinus(int fmultiplicityHFMinus) {multiplicityHFMinus_ = fmultiplicityHFMinus;}

  void SetMultiplicityHBPlus(int fmultiplicityHBPlus) {multiplicityHBPlus_ = fmultiplicityHBPlus;}
  void SetMultiplicityHBMinus(int fmultiplicityHBMinus) {multiplicityHBMinus_ = fmultiplicityHBMinus;}

  void SetMultiplicityHEPlus(int fmultiplicityHEPlus) {multiplicityHEPlus_ = fmultiplicityHEPlus;}
  void SetMultiplicityHEMinus(int fmultiplicityHEMinus) {multiplicityHEMinus_ = fmultiplicityHEMinus;}

  void SetSumEnergyHBPlus(double fsumEnergyHBPlus) {sumEnergyHBPlus_ = fsumEnergyHBPlus;}
  void SetSumEnergyHBMinus(double fsumEnergyHBMinus) {sumEnergyHBMinus_ = fsumEnergyHBMinus;}

  void SetSumETHBPlus(double fsumETHBPlus) {sumETHBPlus_ = fsumETHBPlus;}
  void SetSumETHBMinus(double fsumETHBMinus) {sumETHBMinus_ = fsumETHBMinus;}

  void SetSumEnergyHEPlus(double fsumEnergyHEPlus) {sumEnergyHEPlus_ = fsumEnergyHEPlus;}
  void SetSumEnergyHEMinus(double fsumEnergyHEMinus) {sumEnergyHEMinus_ = fsumEnergyHEMinus;}

  void SetSumETHEMinus(double fsumETHEMinus) {sumETHEMinus_ = fsumETHEMinus;}
  void SetSumETHEPlus(double fsumETHEPlus) {sumETHEPlus_ = fsumETHEPlus;}

  void SetSumETHFPlus(double fsumETHFPlus) {sumETHFPlus_ = fsumETHFPlus;}

  void SetSumETHFMinus(double fsumETHFMinus) {sumETHFMinus_ = fsumETHFMinus;}

  void SetSumEnergyHFPlus(double fsumEnergyHFPlus)      {sumEnergyHFPlus_ = fsumEnergyHFPlus;}
  void SetSumEnergyHFMinus(double fsumEnergyHFMinus)    {sumEnergyHFMinus_ = fsumEnergyHFMinus;}

  void SetMultiplicityHFPlusVsiEta(int fmultiplicityHFPlusVsiEta[13])   {multiplicityHFPlusVsiEta_[13] = fmultiplicityHFPlusVsiEta[13];}
  void SetMultiplicityHFMinusVsiEta(int fmultiplicityHFMinusVsiEta[13]) {multiplicityHFMinusVsiEta_[13] = fmultiplicityHFMinusVsiEta[13];}

  void SetXiGenPlus(double fxiGenPlus)                                   { xiGenPlus_  =  fxiGenPlus;}
  void SetXiGenMinus(double fxiGenMinus)                                 { xiGenMinus_ =  fxiGenMinus;}

  void SetXiTowerPlus(double fxiTowerPlus)                               { xiTowerPlus_  =  fxiTowerPlus;}
  void SetXiTowerMinus(double fxiTowerMinus)                             { xiTowerMinus_ =  fxiTowerMinus;}

  void SetXiPlusFromJets(double fxiPlusFromJets)                         { xiPlusFromJets_  =  fxiPlusFromJets;}
  void SetXiMinusFromJets(double fxiMinusFromJets)                       { xiMinusFromJets_ =  fxiMinusFromJets;}

  void SetXiPlusFromPFCands(double fxiPlusFromPFCands)                   { xiPlusFromPFCands_  =  fxiPlusFromPFCands;}
  void SetXiMinusFromPFCands(double fxiMinusFromPFCands)                 { xiMinusFromPFCands_ =  fxiMinusFromPFCands;}

  void SetMissingMassFromXi(double fmissingMassFromXi)                   { missingMassFromXi_ =  fmissingMassFromXi;} //redundant

  void SetMassJets(double fmassJets)                                     { massJets_ = fmassJets;}        //added by eliza
  void SetMassDijets(double fmassDijets)                                 { massDijets_ = fmassDijets;}    
  void SetMassTrijets(double fmassTrijets)                               { massTrijets_ = fmassTrijets;}  //added by eliza

  void SetMassDijetsGen (double fmassDijetsGen)                          { massDijetsGen_ = fmassDijetsGen;}

  void SetMxFromJets(double fMxFromJets)                                 { MxFromJets_ = fMxFromJets;}
  void SetMxFromPFCands(double fMxFromPFCands)                           { MxFromPFCands_ = fMxFromPFCands;}
  void SetMxGen(double fMxGen)                                           { MxGen_  = fMxGen;}

  void SetRjFromJets(double fRjFromJets)                                 { RjFromJets_ = fRjFromJets;}          //eliza  
  void SetRjjFromJets(double fRjjFromJets)                               { RjjFromJets_ = fRjjFromJets;}
  void SetRjjjFromJets(double fRjjjFromJets)                             { RjjjFromJets_ = fRjjjFromJets;}      //eliza
  void SetRjFromPFCands(double fRjFromPFCands)                           { RjFromPFCands_ = fRjFromPFCands;}    //eliza
  void SetRjjFromPFCands(double fRjjFromPFCands)                         { RjjFromPFCands_ = fRjjFromPFCands;}
  void SetRjjjFromPFCands(double fRjjjFromPFCands)                       { RjjjFromPFCands_ = fRjjjFromPFCands;}//eliza
  void SetRjjGen(double fRjjGen)                                         { RjjGen_  = fRjjGen;}

  void SetLeadingJetBDiscriminator(double fleadingJetBDiscriminator)    {leadingJetBDiscriminator_ = fleadingJetBDiscriminator;}
  void SetSecondJetBDiscriminator(double fsecondJetBDiscriminator)      {secondJetBDiscriminator_  = fsecondJetBDiscriminator;}
  void SetThirdJetBDiscriminator(double fthirdJetBDiscriminator)        {thirdJetBDiscriminator_ = fthirdJetBDiscriminator;} // added by Diego 

  //------------ Get FWD methods ------------------------------
  
  int GetNPileUpBxm1()         const { return nPileUpBxm1_;}
  int GetNPileUpBx0()          const { return nPileUpBx0_;}
  int GetNPileUpBxp1()         const { return nPileUpBxp1_;}
  int GetNVertex()             const { return nVertex_ ;}

  //------ jet 4-momentum vector------------------
  const LorentzVector& GetLeadingJetP4()       const {return leadingJetP4_;}
  const LorentzVector& GetSecondJetP4()        const {return secondJetP4_;}
  const LorentzVector& GetThirdJetP4()         const {return thirdJetP4_;} //fixed by Diego  

  double GetLeadingJetPt()        const { return leadingJetPt_ ;}
  double GetLeadingJetEta()       const { return leadingJetEta_ ;}
  double GetLeadingJetPhi()       const { return leadingJetPhi_ ;}

  double GetSecondJetPt()         const { return secondJetPt_ ;}
  double GetSecondJetEta()        const { return secondJetEta_;}
  double GetSecondJetPhi()        const { return secondJetPhi_;}

  double GetThirdJetPt()          const { return thirdJetPt_;}
  double GetThirdJetEta()         const { return thirdJetEta_;}
  double GetThirdJetPhi()         const { return thirdJetPhi_;} // added by Diego

  double GetJetsAveEta()          const { return jetsAveEta_ ;}
  double GetJetsDeltaEta()        const { return  jetsDeltaEta_;}  //redundant
  double GetJetsDeltaPhi()        const { return  jetsDeltaPhi_ ;}  //redundant
  double GetJetsDeltaPt()         const { return  jetsDeltaPt_ ;}  //redundant
 
  int GetTrackMultiplicity()    const { return trackMultiplicity_ ;}
  int GetMultiplicityHFPlus()   const { return multiplicityHFPlus_ ;}
  int GetMultiplicityHFMinus()  const { return multiplicityHFMinus_ ;}
 
  int  GetMultiplicityHBPlus()  const {return multiplicityHBPlus_ ;}
  int  GetMultiplicityHBMinus() const {return multiplicityHBMinus_;}

  int  GetMultiplicityHEPlus()  const {return multiplicityHEPlus_;}
  int  GetMultiplicityHEMinus() const {return multiplicityHEMinus_;}

  double GetSumEnergyHBPlus()     const {return sumEnergyHBPlus_;}
  double GetSumEnergyHBMinus()    const {return sumEnergyHBMinus_;}

  double GetSumETHBPlus()         const {return sumETHBPlus_;}
  double GetSumETHBMinus()        const {return sumETHBMinus_;}

  double GetSumEnergyHEPlus()     const {return sumEnergyHEPlus_;}
  double GetSumEnergyHEMinus()    const {return sumEnergyHEMinus_;}

  double GetSumETHEMinus()        const {return sumETHEMinus_;}
  double GetSumETHEPlus()         const {return sumETHEPlus_ ;}

  double SumETHFPlus()            const {return sumETHFPlus_ ;}
  double SumETHFMinus()           const {return sumETHFMinus_ ;}

  double GetSumEnergyHFPlus()      const { return sumEnergyHFPlus_ ;}
  double GetSumEnergyHFMinus()     const { return sumEnergyHFMinus_ ;}

  int const* GetMultiplicityHFPlusVsiEta()  const  { return multiplicityHFPlusVsiEta_;}
  int const* GetMultiplicityHFMinusVsiEta() const  { return multiplicityHFMinusVsiEta_;}

  double GetXiGenPlus()                 const  { return xiGenPlus_ ;}
  double GetXiGenMinus()                const  { return  xiGenMinus_ ;}

  double GetXiTowerPlus()               const  { return  xiTowerPlus_ ;}
  double GetXiTowerMinus()              const  { return xiTowerMinus_ ;}

  double GetXiPlusFromJets()            const  { return xiPlusFromJets_ ;}
  double GetXiMinusFromJets()           const  { return xiMinusFromJets_;}

  double GetXiPlusFromPFCands()         const  { return xiPlusFromPFCands_ ;}
  double GetXiMinusFromPFCands()        const  { return xiMinusFromPFCands_;}

  double GetMissingMassFromXi()         const  { return missingMassFromXi_ ;} //redundant

  double GetMassJets()                  const  { return massJets_;}    //eliza
  double GetMassDijets()                const  { return massDijets_;}
  double GetMassTrijets()               const  { return massTrijets_;} //eliza
  double GetMassDijetsGen()             const  { return massDijetsGen_ ;}

  double GetMxFromJets()                const  { return MxFromJets_ ;}
  double GetMxFromPFCands()             const  { return MxFromPFCands_ ;}
  double GetMxGen()                     const  { return MxGen_;}

  double GetRjFromJets()                const  { return RjFromJets_ ;}  //eliza
  double GetRjjFromJets()               const  { return RjjFromJets_ ;}
  double GetRjjjFromJets()              const  { return RjjjFromJets_ ;}//eliza

  double GetRjFromPFCands()             const  { return RjFromPFCands_;} //eliza
  double GetRjjFromPFCands()            const  { return RjjFromPFCands_;}
  double GetRjjjFromPFCands()           const  { return RjjjFromPFCands_;}//eliza

  double GetRjjGen()                    const  { return  RjjGen_;}

  double GetLeadingJetBDiscriminator()    const  {return leadingJetBDiscriminator_ ;}
  double GetSecondJetBDiscriminator()     const  {return secondJetBDiscriminator_ ;}
  double GetThirdJetBDiscriminator()      const  {return thirdJetBDiscriminator_ ;} // added by Diego
  //added by eliza
  int  GetRunNumber()                        const {return runNumber_;}
  int  GetEventNumber()                      const {return eventNumber_;}
  int  GetBunchCrossing()                    const {return bunchCrossing_;}
  double GetLumiWeight()                     const { return lumiWeight_;}
  //int  GetHLT_Jet15U()                       const {return HLT_Jet15U_;}
  //int  GetHLT_L1Jet6U()                      const {return HLT_L1Jet6U_;}
  int GetHLTPath(int idx)                    const { return hltTrigResults_[idx]; }  
 
  double GetSumEnergyHEPlusGen()             const {return sumEnergyHEPlusGen_;}
  double GetSumEnergyHEMinusGen()            const {return sumEnergyHEMinusGen_;}
  double GetSumEnergyHFPlusGen()             const {return sumEnergyHFPlusGen_;}
  double GetSumEnergyHFMinusGen()            const {return sumEnergyHFMinusGen_;}
  double GetSumETotCastor()                  const {return sumETotCastor_;}
  double GetMissingMassFromXiFromPFCands()   const {return missingMassFromXiFromPFCands_;}
  double GetEPlusPzFromPFCands()             const {return EPlusPzFromPFCands_;}
  double GetEMinusPzFromPFCands()            const {return EMinusPzFromPFCands_;}
  double GetMxGenDiss()                      const {return MxGenDiss_;} 
  double GetMxGenRange()                     const {return MxGenRange_;}

  double GetMxGenPlus()                      const {return MxGenPlus_;}
  double GetMxGenMinus()                     const {return MxGenMinus_;}
  double GetDeltaEtaGen()                    const {return deltaEtaGen_;}
  double GetEtaGapGenLow()                      const {return etaGapLow_;}
  double GetEtaGapGenHigh()                     const {return etaGapHigh_;}
  double GetEtaMaxGen()                      const {return etaMaxGen_;}
  double GetEtaMinGen()                      const {return etaMinGen_;}

  double GetEtaMaxFromPFCands()              const {return etaMaxFromPFCands_;}
  double GetEtaMinFromPFCands()              const {return etaMinFromPFCands_;}
  /*double GetetaMaxGenNew()                   const {return etaMaxGenNew_;}
  double GetetaMinGenNew()                   const {return etaMinGenNew_;}*/
  double GetEtaMaxFromPFCandsNew()           const {return etaMaxFromPFCandsNew_;}
  double GetEtaMinFromPFCandsNew()           const {return etaMinFromPFCandsNew_;}

  double GetEtaAllTypes()                    const {return EtaAllTypes_;}
  double GetEnergyAllTypes()                 const {return EnergyAllTypes_;}
  double GetPtAllTypes()                     const {return PtAllTypes_;}

  double GetEtaChargedHadron()               const {return EtaChargedHadron_;}
  double GetEnergyChargedHadron()            const {return EnergyChargedHadron_;}
  double GetPtChargedHadron()                const {return PtChargedHadron_;}

  double GetEtaUndefined()                   const {return EtaUndefined_;}
  double GetEnergyUndefined()                const {return EnergyUndefined_;}
  double GetPtUndefined()                    const {return PtUndefined_;}
 
  double GetEtaElectron()                    const {return EtaElectron_;}
  double GetEnergyElectron()                 const {return EnergyElectron_;}
  double GetPtElectron()                     const {return PtElectron_;}
 
  double GetEtaMuon()                        const {return EtaMuon_;}
  double GetEnergyMuon()                     const {return EnergyMuon_;}
  double GetPtMuon()                         const {return PtMuon_;}

  double GetEtaGamma()                       const {return EtaGamma_;}
  double GetEnergyGamma()                    const {return EnergyGamma_;}
  double GetPtGamma()                        const {return PtGamma_;}

  double GetEtaNeutralHadron()               const {return EtaNeutralHadron_;}
  double GetEnergyNeutralHadron()            const {return EnergyNeutralHadron_;}
  double GetPtNeutralHadron()                const {return PtNeutralHadron_;}

  double GetEtaHadronHF()                    const {return EtaHadronHF_;}
  double GetEnergyHadronHF()                 const {return EnergyHadronHF_;}
  double GetPtHadronHF()                     const {return PtHadronHF_;}
 
  double GetEtaEGammaHF()                    const {return EtaEGammaHF_;}
  double GetEnergyEGammaHF()                 const {return EnergyEGammaHF_;}
  double GetPtEGammaHF()                     const {return PtEGammaHF_;}

  double GetSumEHFPFlowPlus()                const {return SumEHFPFlowPlus_;}
  double GetSumEHFPFlowMinus()               const {return SumEHFPFlowMinus_;}
  //

  private:
     friend class exclusiveDijetsAnalysis::ExclusiveDijetsAnalysis;

     void reset();
 
     int nPileUpBxm1_;
     int nPileUpBx0_;
     int nPileUpBxp1_;
     int nVertex_;

     //added by eliza
     int runNumber_;
     int eventNumber_;
     int lumiSection_;
     int bunchCrossing_; 
     double lumiWeight_;
     int hltTrigResults_[20];
//------ jet 4-momentum vector------------------
     LorentzVector leadingJetP4_;
     LorentzVector secondJetP4_;
     LorentzVector thirdJetP4_;
//-------------------------------------------------
     double leadingJetPt_;
     double leadingJetEta_;
     double leadingJetPhi_;
     double leadingJetBDiscriminator_;
     double secondJetPt_;
     double secondJetEta_;
     double secondJetPhi_;
     double secondJetBDiscriminator_;
     double thirdJetPt_;
     double thirdJetEta_;
     double thirdJetPhi_; // added by Diego
     double thirdJetBDiscriminator_; //added by Diego
     double jetsAveEta_; //redundant
     double jetsDeltaEta_; //redundant
     double jetsDeltaPhi_; //redundant
     double jetsDeltaPt_; //redundant
     
     int trackMultiplicity_;
     
     int multiplicityHFPlus_;
     int multiplicityHFMinus_;
     
     double sumEnergyHFPlus_;
     double sumEnergyHFMinus_;
     
     int multiplicityHBPlus_ ;
     int multiplicityHBMinus_ ;
     
     int multiplicityHEPlus_ ;
     int multiplicityHEMinus_ ;
     
     double sumEnergyHBPlus_ ;
     double sumEnergyHBMinus_ ;

     double sumETHBPlus_;
     double sumETHBMinus_;

     double sumEnergyHEPlus_;
     double sumEnergyHEMinus_ ;

     double sumETHEMinus_ ;
     double sumETHEPlus_ ;

     double sumETHFPlus_;
     double sumETHFMinus_;

     int multiplicityHFPlusVsiEta_[13];
     int multiplicityHFMinusVsiEta_[13];

     double sumETHFPlusVsiEta_[13];
     double sumEHFMinusVsiEta_[13];
     double sumETHFMinusVsiEta_[13];

     double xiGenPlus_;
     double xiGenMinus_;

     double xiTowerPlus_;
     double xiTowerMinus_;
     double xiPlusFromJets_;
     double xiMinusFromJets_;
     double xiPlusFromPFCands_;
     double xiMinusFromPFCands_;
     double missingMassFromXi_; //redundant
     double massJets_;        //eliza
     double massDijets_;       
     double massTrijets_;       //eliza
     double massDijetsGen_;
     double MxFromJets_;
     double MxFromPFCands_;
     double MxGen_;
     double RjFromJets_;       //eliza     
     double RjjFromJets_;
     double RjjjFromJets_;     //eliza
     double RjFromPFCands_;    //eliza
     double RjjFromPFCands_;
     double RjjjFromPFCands_;  //eliza
     double RjjGen_;
//////////////////////////////////
/////added by eliza
     double sumEnergyHEPlusGen_;
     double sumEnergyHEMinusGen_;
     double sumEnergyHFPlusGen_;
     double sumEnergyHFMinusGen_; 
     double sumETotCastor_;
     double missingMassFromXiFromPFCands_;
     double EPlusPzFromPFCands_;
     double EMinusPzFromPFCands_;
     double MxGenDiss_; 
     double MxGenRange_;
     double MxGenPlus_;
     double MxGenMinus_;
     double deltaEtaGen_;
     double etaGapLow_;
     double etaGapHigh_;
     double etaMaxGen_;
     double etaMinGen_;
     double etaMaxFromPFCands_;
     double etaMinFromPFCands_;
     double etaMaxGenNew_;
     double etaMinGenNew_;
     double etaMaxFromPFCandsNew_;
     double etaMinFromPFCandsNew_;

     double EtaAllTypes_;
     double EnergyAllTypes_;
     double PtAllTypes_;

     double EtaChargedHadron_;
     double EnergyChargedHadron_;
     double PtChargedHadron_;

     double EtaUndefined_;
     double EnergyUndefined_;
     double PtUndefined_;
 
     double EtaElectron_;
     double EnergyElectron_;
     double PtElectron_;
 
     double EtaMuon_;
     double EnergyMuon_;
     double PtMuon_;

     double EtaGamma_;
     double EnergyGamma_;
     double PtGamma_;

     double EtaNeutralHadron_;
     double EnergyNeutralHadron_;
     double PtNeutralHadron_;

     double EtaHadronHF_;
     double EnergyHadronHF_;
     double PtHadronHF_;
 
     double EtaEGammaHF_;
     double EnergyEGammaHF_;
     double PtEGammaHF_;

     double SumEHFPFlowPlus_;
     double SumEHFPFlowMinus_;
     
//////////////////////////////////
};

#endif
