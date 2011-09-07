#ifndef DiffractiveEvent_h
#define DiffractiveEvent_h

namespace diffractiveAnalysis {
   class DiffractiveAnalysis;
}

class DiffractiveEvent {
   public:
      typedef diffractiveAnalysis::DiffractiveAnalysis type;

      DiffractiveEvent();
      ~DiffractiveEvent();    
   private:
      friend class diffractiveAnalysis::DiffractiveAnalysis;
      
      void reset();

      int processId_;
      int runNumber_;
      int eventNumber_;
      int lumiSection_;
      int bunchCrossing_;
      double lumiWeight_;
      int HLTPath_;
      int TTBit_;
      int nVertex_;
      double posXPrimVtx_;
      double posYPrimVtx_;
      double posZPrimVtx_;
      int multiplicityTracks_;
      double sumPtTracks_;
      int multiplicityTracksGen_;
      double sumPtTracksGen_;
      double leadingJetPt_;
      double leadingJetEta_;
      double leadingJetPhi_;
      int multiplicityHEPlus_;
      int multiplicityHEMinus_;
      double sumEnergyHEPlus_;
      double sumEnergyHEMinus_;
      double sumETHEPlus_;
      double sumETHEMinus_;
      int multiplicityHFPlus_;
      int multiplicityHFMinus_;
      double sumEnergyHFPlus_;
      double sumEnergyHFMinus_;
      double sumETHFPlus_;
      double sumETHFMinus_;
      // 1-13 HF rings, 2.866 < |eta| < 5.205
      int multiplicityHFPlusVsiEta_[13];
      int multiplicityHFMinusVsiEta_[13];
      double sumEHFPlusVsiEta_[13];
      double sumEHFMinusVsiEta_[13];
      double sumETHFPlusVsiEta_[13];
      double sumETHFMinusVsiEta_[13];
      double sumEnergyHEPlusGen_;
      double sumEnergyHEMinusGen_;
      double sumEnergyHFPlusGen_;
      double sumEnergyHFMinusGen_; 
      double sumETotCastor_;
      double xiGenPlus_;
      double xiGenMinus_;
      double xiPlusFromTowers_;
      double xiMinusFromTowers_;
      double xiPlusFromPFCands_;
      double xiMinusFromPFCands_;
      double EPlusPzFromTowers_;
      double EMinusPzFromTowers_;
      double EPlusPzFromPFCands_;
      double EMinusPzFromPFCands_;
      double MxGen_;
      double MxGenDiss_; 
      double MxGenRange_;
      double MxGenPlus_;
      double MxGenMinus_;
      double MxFromTowers_;
      double MxFromPFCands_;
      double deltaEtaGen_;
      double etaGapLow_;
      double etaGapHigh_;
      double etaMaxGen_;
      double etaMinGen_;
      double etaMaxFromPFCands_;
      double etaMinFromPFCands_;
      double sumET_;
      double MEx_;
      double MEy_;
      double MET_;
      int BeamHaloLooseId_;
      int BeamHaloTightId_;
      int LooseNoiseFilter_;
      int TightNoiseFilter_;
};

#endif
