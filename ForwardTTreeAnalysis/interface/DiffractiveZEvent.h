#ifndef DiffractiveZEvent_h
#define DiffractiveZEvent_h

namespace diffractiveZAnalysis {
   class DiffractiveZAnalysis;
}

class DiffractiveZEvent {
   public:
      typedef diffractiveZAnalysis::DiffractiveZAnalysis analysis_type;
      static const char* name;

      DiffractiveZEvent();
      ~DiffractiveZEvent();

      void SetHLTPath(int idx, int fHLTBit)         { hltTrigResults_[idx] = fHLTBit;}
      double GetLeadingJetPt() const {return leadingJetPt_;}
      double GetLeadingJetEta() const {return leadingJetEta_;}
      double GetLeadingJetPhi() const {return leadingJetPhi_;}
      int GetHLTPath(int idx)                    const { return hltTrigResults_[idx]; }


   private:
      friend class diffractiveZAnalysis::DiffractiveZAnalysis;
      
      void reset();

      int hltTrigResults_[20];
      double leadingJetPt_;
      double leadingJetEta_;
      double leadingJetPhi_;

};

#endif
