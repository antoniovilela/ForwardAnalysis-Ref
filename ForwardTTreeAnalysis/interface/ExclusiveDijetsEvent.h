#ifndef ExclusiveDijetsEvent_h
#define ExclusiveDijetsEvent_h

namespace exclusiveDijetsAnalysis {
   class ExclusiveDijetsAnalysis;
}

class ExclusiveDijetsEvent {
  public:
     ExclusiveDijetsEvent();
     ~ExclusiveDijetsEvent();


 //------------ Set FWD methods ------------------------------
 void SetNPileUpBx0(int fnPileUpBx0)           { nPileUpBx0_       = fnPileUpBx0;}
 void SetNVertex(int fnVertex)                 { nVertex_          = fnVertex;}

 void SetLeadingJetPt(double fleadingJetPt)    { leadingJetPt_     = fleadingJetPt;}
 void SetLeadingJetEta(double fleadingJetEta)  { leadingJetEta_     = fleadingJetEta;}
 void SetLeadingJetPhi(double fleadingJetPhi)  { leadingJetPhi_    = fleadingJetPhi;}

 void SetSecondJetPt(double fsecondJetPt)      { secondJetPt_      = fsecondJetPt;}
 void SetSecondJetEta(double fsecondJetEta)    { secondJetEta_     = fsecondJetEta;}
 void SetSecondJetPhi(double fsecondJetPhi)    { secondJetPhi_     = fsecondJetPhi;}

 void SetThirdJetPt(double fthirdJetPt)        { thirdJetPt_        = fthirdJetPt;}
 void SetThirdJetEta(double fthirdJetEta)      { thirdJetEta_       = fthirdJetEta;}

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

 void SetMassDijets(double fmassDijets)                                 { massDijets_ = fmassDijets;}
 void SetMassDijetsGen (double fmassDijetsGen)                          { massDijetsGen_ = fmassDijetsGen;}

 void SetMxFromJets(double fMxFromJets)                                 { MxFromJets_ = fMxFromJets;}
 void SetMxFromPFCands(double fMxFromPFCands)                           { MxFromPFCands_ = fMxFromPFCands;}
 void SetMxGen(double fMxGen)                                           { MxGen_  = fMxGen;}

 void SetRjjFromJets(double fRjjFromJets)                               { RjjFromJets_ = fRjjFromJets;}
 void SetRjjFromPFCands(double fRjjFromPFCands)                         { RjjFromPFCands_ = fRjjFromPFCands;}
 void SetRjjGen(double fRjjGen)                                         { RjjGen_  = fRjjGen;}


 void SetLeadingJetBDiscriminator(double fleadingJetBDiscriminator)    {leadingJetBDiscriminator_ = fleadingJetBDiscriminator;}
 void SetSecondJetBDiscriminator(double fsecondJetBDiscriminator)      {secondJetBDiscriminator_  = fsecondJetBDiscriminator;}

 //------------ Get FWD methods ------------------------------

 int GetNPileUpBx0()          const { return nPileUpBx0_;}
 int GetNVertex()             const { return nVertex_ ;}

 double GetLeadingJetPt()        const { return leadingJetPt_ ;}
 double GetLeadingJetEta()       const { return leadingJetEta_ ;}
 double GetLeadingJetPhi()       const { return leadingJetPhi_ ;}

 double GetSecondJetPt()         const { return secondJetPt_ ;}
 double GetSecondJetEta()        const { return secondJetEta_;}
 double GetSecondJetPhi()        const { return secondJetPhi_;}

 double GetThirdJetPt()          const { return thirdJetPt_;}
 double GetThirdJetEta()         const { return thirdJetEta_;}

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

 int GetMultiplicityHFPlusVsiEta()  const  { return multiplicityHFPlusVsiEta_[13] ;}
 int GetMultiplicityHFMinusVsiEta() const  { return multiplicityHFMinusVsiEta_[13];}

 double GetXiGenPlus()                 const  { return xiGenPlus_ ;}
 double GetXiGenMinus()                const  { return  xiGenMinus_ ;}

 double GetXiTowerPlus()               const  { return  xiTowerPlus_ ;}
 double GetXiTowerMinus()              const  { return xiTowerMinus_ ;}

 double GetXiPlusFromJets()            const  { return xiPlusFromJets_ ;}
 double GetXiMinusFromJets()           const  { return xiMinusFromJets_;}

 double GetXiPlusFromPFCands()         const  { return xiPlusFromPFCands_ ;}
 double GetXiMinusFromPFCands()        const  { return xiMinusFromPFCands_;}

 double GetMissingMassFromXi()         const  { return missingMassFromXi_ ;} //redundant

 double GetMassDijets()                const  { return massDijets_;}
 double GetMassDijetsGen()             const  { return massDijetsGen_ ;}

 double GetMxFromJets()                const  { return MxFromJets_ ;}
 double GetMxFromPFCands()             const  { return MxFromPFCands_ ;}
 double GetMxGen()                     const  { return MxGen_;}

 double GetRjjFromJets()               const  { return RjjFromJets_ ;}
 double GetRjjFromPFCands()            const  { return RjjFromPFCands_;}
 double GetRjjGen()                    const  { return  RjjGen_;}


 double GetLeadingJetBDiscriminator()    const  {return leadingJetBDiscriminator_ ;}
 double GetSecondJetBDiscriminator()     const  {return secondJetBDiscriminator_ ;}



  private:
     friend class exclusiveDijetsAnalysis::ExclusiveDijetsAnalysis;

     int nPileUpBx0_;
     int nVertex_;
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
     double massDijets_;
     double massDijetsGen_;
     double MxFromJets_;
     double MxFromPFCands_;
     double MxGen_;
     double RjjFromJets_;
     double RjjFromPFCands_;
     double RjjGen_;



};

#endif
