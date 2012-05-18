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

      void SetNBits(int nBits) { nL1Bits_ = nBits; }
      void SetL1Bit(int idx, int decision) { l1Decision_[idx] = decision; }
      void SetL1Prescale(int idx, int prescale) { l1Prescale_[idx] = prescale; }
      void SetL1AlgoName(int idx, std::string const& name) { l1AlgoName_[idx] = name; }

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

      void SetNJet(int fnJet)                       { nJet_ =  fnJet;}

      void SetNtowCal(int fntowCal)                 { ntowCal_ = fntowCal;}


      void SetTowET(float ftowET)                   { towET_        = ftowET;}
      void SetTowEta(float ftowEta)                 { towEta_        = ftowEta;}
      void SetTowPhi(float ftowPhi)                 { towPhi_        = ftowPhi;}
      void SetTowE(float ftowE)                     { towE_        = ftowE;}
      void SetTowEm(float ftowEm)                   { towEm_        = ftowEm;}
      void SetTowHad(float ftowHad)                 { towHad_        = ftowHad;}
      void SetTowOe(float ftowOe)                   { towOe_        = ftowOe;}

   
      //
      //------------ Get FWD methods ------------------------------

      int GetNBits() const { return nL1Bits_; }
      int GetL1Bit(int idx) const { return l1Decision_[idx]; }
      int GetL1Prescale(int idx) const { return l1Prescale_[idx]; }
      std::string GetL1AlgoName(int idx) const { return l1AlgoName_[idx]; }

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
      
      int GetNJet()                   const { return nJet_;}
      int GetNtowCal()                const { return ntowCal_;} 

      float GetTowET()                const { return towET_;}
      float GetTowEta()               const { return towEta_;}
      float GetTowPhi()               const { return towPhi_;}     
      float GetTowE()                 const { return towE_;}
      float GetTowEm()                const { return towEm_;}
      float GetTowHad()               const { return towHad_;}
      float GetTowOe()                const { return towOe_;}

   private:
      friend class dijetsTriggerAnalysis::DijetsTriggerAnalysis;
      void reset();

      int nL1Bits_;
      std::vector<int> l1Decision_;
      std::vector<int> l1Prescale_;
      std::vector<std::string> l1AlgoName_;
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
      
      float ntowCal_;
      float towET_;
      float towEta_;
      float towPhi_;
      float towE_;
      float towEm_;
      float towHad_;
      float towOe_;

      int nJet_; 



};//end code

#endif
