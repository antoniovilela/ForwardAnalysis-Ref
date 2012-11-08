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

      double GetLeadingJetPt() const {return leadingJetPt_;}
      double GetLeadingJetEta() const {return leadingJetEta_;}
      double GetLeadingJetPhi() const {return leadingJetPhi_;}

   private:
      friend class diffractiveZAnalysis::DiffractiveZAnalysis;
      
      void reset();

      double leadingJetPt_;
      double leadingJetEta_;
      double leadingJetPhi_;
};

#endif
