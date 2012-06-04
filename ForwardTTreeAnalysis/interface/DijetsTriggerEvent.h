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

      void SetHLTPath(int idx, int fHLTBit)         { hltTrigResults_[idx] = fHLTBit;}

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

      /*void SetNtowCal(int fntowCal)                 { ntowCal_ = fntowCal;}


      void SetTowET(int jtow, double ftowET)         { towET_[jtow]        = ftowET;}
      void SetTowEta(int jtow, double ftowEta)       { towEta_[jtow]        = ftowEta;}
      void SetTowPhi(int jtow, double ftowPhi)       { towPhi_[jtow]        = ftowPhi;}
      void SetTowE(int jtow, double ftowE)           { towE_[jtow]        = ftowE;}
      void SetTowEm(int jtow, double ftowEm)         { towEm_[jtow]        = ftowEm;}
      void SetTowHad(int jtow, double ftowHad)       { towHad_[jtow]        = ftowHad;}
      void SetTowOe(int jtow, double ftowOe)         { towOe_[jtow]        = ftowOe;}*/

   
      //
      //------------ Get FWD methods ------------------------------

      int GetNBits() const { return nL1Bits_; }
      int GetL1Bit(int idx) const { return l1Decision_[idx]; }
      int GetL1Prescale(int idx) const { return l1Prescale_[idx]; }
      std::string GetL1AlgoName(int idx) const { return l1AlgoName_[idx]; }

      int GetHLTPath(int idx)                    const { return hltTrigResults_[idx]; }        

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
      /*int GetNtowCal()                const { return ntowCal_;} 

      double GetTowET(int jtow)       const { return towET_[jtow];}
      double GetTowEta(int jtow)      const { return towEta_[jtow];}
      double GetTowPhi(int jtow)      const { return towPhi_[jtow];}     
      double GetTowE(int jtow)        const { return towE_[jtow];}
      double GetTowEm(int jtow)       const { return towEm_[jtow];}
      double GetTowHad(int jtow)      const { return towHad_[jtow];}
      double GetTowOe(int jtow)       const { return towOe_[jtow];}*/

   private:
      friend class dijetsTriggerAnalysis::DijetsTriggerAnalysis;
      void reset();

      int nL1Bits_;
      std::vector<int> l1Decision_;
      std::vector<int> l1Prescale_;
      std::vector<std::string> l1AlgoName_;
      int hltTrigResults_[20]; 
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
      
      int nJet_ , jtow;
      /*int ntowCal_;
      std::vector<double> towET_;
      std::vector<double> towEta_;
      std::vector<double> towPhi_;
      std::vector<double> towE_;
      std::vector<double> towEm_;
      std::vector<double> towHad_;
      std::vector<double> towOe_;
      //int nMaxTower = 100000000;
      //double towET_[100000];
     */

      /*float towEta_[nMaxTower];
      float towPhi_[nMaxTower];
      float towE_[nMaxTower];
      float towEm_[nMaxTower];
      float towHad_[nMaxTower];
      float towOe_[nMaxTower];*/

     double SumEHFPlus_;
     double SumEHFMinus_;
      

     

};//end code

#endif
