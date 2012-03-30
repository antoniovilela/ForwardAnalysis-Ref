#ifndef HeavyFlavorEvent_h
#define HeavyFlavorEvent_h

namespace heavyFlavorAnalysis {
   class HeavyFlavorAnalysis;
}

class HeavyFlavorEvent {
   public:
      typedef heavyFlavorAnalysis::HeavyFlavorAnalysis analysis_type;
      static const char* name;

      HeavyFlavorEvent();
      ~HeavyFlavorEvent();

      double GetLeadingJetPt() const {return leadingJetPt_;}
      double GetLeadingJetEta() const {return leadingJetEta_;}
      double GetLeadingJetPhi() const {return leadingJetPhi_;}

   private:
      friend class heavyFlavorAnalysis::HeavyFlavorAnalysis;
      
      void reset();

      double leadingJetPt_;
      double leadingJetEta_;
      double leadingJetPhi_;
};

#endif
