#ifndef DiffractiveEvent_h
#define DiffractiveEvent_h

namespace diffractiveAnalysis {
   class DiffractiveAnalysis;
}

class DiffractiveEvent {
   public:
      typedef diffractiveAnalysis::DiffractiveAnalysis analysis_type;
      static const char* name;

      DiffractiveEvent();
      ~DiffractiveEvent();

      int GetProcessId() const {return processId_;}
      int GetRunNumber() const {return runNumber_;}
      int GetEventNumber() const {return eventNumber_;}
      int GetLumiSection() const {return lumiSection_;}
      int GetBunchCrossing() const {return bunchCrossing_;}
      double GetLumiWeight() const {return lumiWeight_;}

      int GetHLTPath() const {return HLTPath_;}
      int GetTTBit() const {return TTBit_;}

      int GetBeamHaloLooseId() const {return BeamHaloLooseId_;}
      int GetBeamHaloTightId() const {return BeamHaloTightId_;}
      int GetLooseNoiseFilter() const {return LooseNoiseFilter_;}
      int GetTightNoiseFilter() const {return TightNoiseFilter_;}

      int GetNVertex() const {return nVertex_;}
      double GetPosXPrimVtx() const {return posXPrimVtx_;}
      double GetPosYPrimVtx() const {return posYPrimVtx_;}
      double GetPosZPrimVtx() const {return posZPrimVtx_;}

      double GetLeadingJetPt() const {return leadingJetPt_;}
      double GetLeadingJetEta() const {return leadingJetEta_;}
      double GetLeadingJetPhi() const {return leadingJetPhi_;}

      double GetSumET() const {return sumET_;}
      double GetMEx() const {return MEx_;}
      double GetMEy() const {return MEy_;}
      double GetMET() const {return MET_;}

      int GetMultiplicityTracks() const {return multiplicityTracks_;}
      int GetMultiplicityTracksGen() const {return multiplicityTracksGen_;}

      double GetSumPtTracks() const {return sumPtTracks_;}
      double GetSumPtTracksGen() const {return sumPtTracksGen_;}

      int GetMultiplicityHEPlus() const {return multiplicityHEPlus_;}
      int GetMultiplicityHEMinus() const {return multiplicityHEMinus_;}
      int GetMultiplicityHFPlus() const {return multiplicityHFPlus_;}
      int GetMultiplicityHFMinus() const {return multiplicityHFMinus_;}

      double GetSumEnergyHEPlus() const {return sumEnergyHEPlus_;}
      double GetSumEnergyHEMinus() const {return sumEnergyHEMinus_;}
      double GetSumEnergyHFPlus() const {return sumEnergyHFPlus_;}
      double GetSumEnergyHFMinus() const {return sumEnergyHFMinus_;}

      double GetSumETHEPlus() const {return sumETHEPlus_;}
      double GetSumETHEMinus() const {return sumETHEMinus_;}
      double GetSumETHFPlus() const {return sumETHFPlus_;}
      double GetSumETHFMinus() const {return sumETHFMinus_;}

      double GetSumETotCastor() const {return sumETotCastor_;}

      int GetMultiplicityHFPlusVsiEta(int ieta) const {return multiplicityHFPlusVsiEta_[ieta];}
      int GetMultiplicityHFMinusVsiEta(int ieta) const {return multiplicityHFMinusVsiEta_[ieta];}

      double GetSumEHFPlusVsiEta(int ieta) const {return sumEHFPlusVsiEta_[ieta];}
      double GetSumEHFMinusVsiEta(int ieta) const {return sumEHFMinusVsiEta_[ieta];}
      double GetSumETHFPlusVsiEta(int ieta) const {return sumETHFPlusVsiEta_[ieta];}
      double GetSumETHFMinusVsiEta(int ieta) const {return sumETHFMinusVsiEta_[ieta];}

      double GetSumEnergyHEPlusGen() const {return sumEnergyHEPlusGen_;}
      double GetSumEnergyHEMinusGen() const {return sumEnergyHEMinusGen_;}
      double GetSumEnergyHFPlusGen() const {return sumEnergyHFPlusGen_;}
      double GetSumEnergyHFMinusGen() const {return sumEnergyHFMinusGen_;}

      double GetXiPlusFromTowers() const {return xiPlusFromTowers_;}
      double GetXiMinusFromTowers() const {return xiMinusFromTowers_;}
      double GetXiPlusFromPFCands() const {return xiPlusFromPFCands_;}
      double GetXiMinusFromPFCands() const {return xiMinusFromPFCands_;}

      double GetEPlusPzFromTowers() const {return EPlusPzFromTowers_;}
      double GetEMinusPzFromTowers() const {return EMinusPzFromTowers_;}
      double GetEPlusPzFromPFCands() const {return EPlusPzFromPFCands_;}
      double GetEMinusPzFromPFCands() const {return EMinusPzFromPFCands_;}

      double GetMxFromTowers() const {return MxFromTowers_;}
      double GetMxFromPFCands() const {return MxFromPFCands_;}

      double GetEtaMaxFromPFCands() const {return etaMaxFromPFCands_;}
      double GetEtaMinFromPFCands() const {return etaMinFromPFCands_;}

      double GetXiGenPlus() const {return xiGenPlus_;}
      double GetXiGenMinus() const {return xiGenMinus_;}

      double GetMxGen() const {return MxGen_;}
      double GetMxGenDiss() const {return MxGenDiss_;}
      double GetMxGenRange() const {return MxGenRange_;}
      double GetMxGenPlus() const {return MxGenPlus_;}
      double GetMxGenMinus() const {return MxGenMinus_;}

      double GetEtaMaxGen() const {return etaMaxGen_;}
      double GetEtaMinGen() const {return etaMinGen_;}

      double GetDeltaEtaGen() const {return deltaEtaGen_;}
      double GetEtaGapLow() const {return etaGapLow_;}
      double GetEtaGapHigh() const {return etaGapHigh_;}

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
