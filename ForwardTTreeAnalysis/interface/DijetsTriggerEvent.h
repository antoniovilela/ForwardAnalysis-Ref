#ifndef DijetsTriggerEvent_h
#define DijetsTriggerEvent_h

#include <string>
#include <vector>

namespace dijetsTriggerAnalysis {
   class DijetsTriggerAnalysis;
}

class DijetsTriggerEvent {
   public:
      typedef dijetsTriggerAnalysis::DijetsTriggerAnalysis analysis_type;
      static const char* name;

      DijetsTriggerEvent();
      ~DijetsTriggerEvent();
      //------------ Set FWD methods ------------------------------

      void SetRunNumber(int frunNumber)             { runNumber_        = frunNumber;}
      void SetEventNumber(int feventNumber)         { eventNumber_      = feventNumber;}
      void SetLumiSection(int flumiSection)         { lumiSection_      = flumiSection;}
      void SetBunchCrossing(int fbunchCrossing)     { bunchCrossing_    = fbunchCrossing;}
      //void SetInstDelLumiLS(float finstLumiLS)      { instLumiLS_       = finstLumiLS;}
      //void SetInstLumiBunch(float finstLumiBunchOCC1) {instLumiBunchOCC1_ = finstLumiBunchOCC1;}

      void SetNBits(int nBits) { nL1Bits_ = nBits; }
      void SetL1Bit(int idx, int decision) { l1Decision_[idx] = decision; }
      void SetL1Prescale(int idx, int prescale) { l1Prescale_[idx] = prescale; }
      void SetL1AlgoName(int idx, std::string const& name) { l1AlgoName_[idx] = name; }


      void SetHLTPath(int idx, int fHLTBit)                 { hltTrigResults_[idx] = fHLTBit;}
      void SetHLTPathName(int idx, std::string const& path) { hltTrigNames_[idx] = path;}


      void SetEtSum(int ring,int etSum) { hfRingEtSum_[ring] = etSum;}
      void SetBitCount(int ring,int bitCount) { hfRingBitCount_[ring] = bitCount;}

      void SetLeadingJetPt(double fleadingJetPt)    { leadingJetPt_     = fleadingJetPt;}
      void SetLeadingJetEta(double fleadingJetEta)  { leadingJetEta_     = fleadingJetEta;}
      void SetLeadingJetPhi(double fleadingJetPhi)  { leadingJetPhi_    = fleadingJetPhi;}

      void SetSecondJetPt(double fsecondJetPt)      { secondJetPt_      = fsecondJetPt;}
      void SetSecondJetEta(double fsecondJetEta)    { secondJetEta_     = fsecondJetEta;}
      void SetSecondJetPhi(double fsecondJetPhi)    { secondJetPhi_     = fsecondJetPhi;}

      void SetThirdJetPt(double fthirdJetPt)        { thirdJetPt_        = fthirdJetPt;}
      void SetThirdJetEta(double fthirdJetEta)      { thirdJetEta_       = fthirdJetEta;}
      void SetThirdJetPhi(double fthirdJetPhi)      { thirdJetPhi_       = fthirdJetPhi;}

      void SetSumEHFPlus(double fSumEHFPlus)       { SumEHFPlus_ =fSumEHFPlus;}
      void SetSumEHFMinus(double fSumEHFMinus)     { SumEHFMinus_ =fSumEHFMinus;}



      void SetNJet(int fnJet)                       { nJet_ =  fnJet;}
    
      //
      //------------ Get FWD methods ------------------------------

      int  GetRunNumber()                        const {return runNumber_;}
      int  GetEventNumber()                      const {return eventNumber_;}
      int  GetBunchCrossing()                    const {return bunchCrossing_;}
      int GetLumiSection()                       const {return  lumiSection_;}
      //float GetInstDelLumiLS()                   const {return instLumiLS_;}
      //float GetInstLumiBunch()                   const {return instLumiBunchOCC1_;}

      int GetNBits() const { return nL1Bits_; }
      int GetL1Bit(int idx) const { return l1Decision_[idx]; }
      int GetL1Prescale(int idx) const { return l1Prescale_[idx]; }
      std::string GetL1AlgoName(int idx) const { return l1AlgoName_[idx]; }

      int GetHLTPath(int idx)                    const { return hltTrigResults_[idx]; }
      std::string GetHLTPathName(int idx)                const { return hltTrigNames_[idx]; }

      int GetEtSum(int ring) const { return hfRingEtSum_[ring]; }
      int GetBitCount(int ring) const { return hfRingBitCount_[ring]; }

      double GetLeadingJetPt()        const { return leadingJetPt_ ;}
      double GetLeadingJetEta()       const { return leadingJetEta_ ;}
      double GetLeadingJetPhi()       const { return leadingJetPhi_ ;}

      double GetSecondJetPt()         const { return secondJetPt_ ;}
      double GetSecondJetEta()        const { return secondJetEta_;}
      double GetSecondJetPhi()        const { return secondJetPhi_;}

      double GetThirdJetPt()          const { return thirdJetPt_;}
      double GetThirdJetEta()         const { return thirdJetEta_;}
      double GetThirdJetPhi()         const { return thirdJetPhi_;}

      double GetSumEHFPlus()                const {return SumEHFPlus_;}
      double GetSumEHFMinus()               const {return SumEHFMinus_;}

      int GetNJet()                   const { return nJet_;}

   private:
      friend class dijetsTriggerAnalysis::DijetsTriggerAnalysis;
      void reset();
      
      int runNumber_;
      int eventNumber_;
      int lumiSection_;
      int bunchCrossing_;
      //float instLumiLS_;
      //float instLumiBunchOCC1_;
 
      int nL1Bits_;
      std::vector<int> l1Decision_;
      std::vector<int> l1Prescale_;
      std::vector<std::string> l1AlgoName_;

      std::vector<int> hltTrigResults_;
      std::vector<std::string> hltTrigNames_;

      std::vector<int> hfRingEtSum_;
      std::vector<int> hfRingBitCount_;

      double leadingJetPt_;
      double leadingJetEta_;
      double leadingJetPhi_;

      double secondJetPt_;
      double secondJetEta_;
      double secondJetPhi_;

      double thirdJetPt_;
      double thirdJetEta_;
      double thirdJetPhi_;

      int nJet_ ;

      double SumEHFPlus_;
      double SumEHFMinus_; 

};//end code

#endif
