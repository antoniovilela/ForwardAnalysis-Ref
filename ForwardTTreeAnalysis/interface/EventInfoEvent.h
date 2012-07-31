#ifndef EventInfoEvent_h
#define EventInfoEvent_h

namespace eventInfo {
  class EventInfo;
}

class EventInfoEvent {
  public:

  typedef eventInfo::EventInfo analysis_type;
  static const char* name;

  EventInfoEvent();
  ~EventInfoEvent();

  void SetPthat(double fptHat)                             { ptHat = fptHat; }
  void SetGeneratorWeight(double fgenWeight)               { generatorWeight = fgenWeight; } 
  void SetNPileUpBxm1(int fnPileUpBxm1)                    { nPileUpBxm1_          = fnPileUpBxm1;}
  void SetNPileUpBx0(int fnPileUpBx0)                      { nPileUpBx0_           = fnPileUpBx0;}
  void SetNPileUpBxp1(int fnPileUpBxp1)                    { nPileUpBxp1_          = fnPileUpBxp1;}
  void SetTrueNumInteractions(float fnTrueInteractions)  { nTrueInteractions_ = fnTrueInteractions;}

  void SetRunNumber(int frunNumber)             { runNumber_        = frunNumber;}   
  void SetEventNumber(int feventNumber)         { eventNumber_      = feventNumber;}
  void SetLumiSection(int flumiSection)         { lumiSection_      = flumiSection;}
  void SetBunchCrossing(int fbunchCrossing)     { bunchCrossing_    = fbunchCrossing;}
  void SetOrbitNumber(int forbitNumber)         { orbitNumber_      = forbitNumber;}
  void SetLumiWeight(double flumiWeight)        { lumiWeight_       = flumiWeight;}
  void SetInstDelLumiLS(float finstLumiLS)      { instLumiLS_       = finstLumiLS;}
  void SetInstLumiBunch(float finstLumiBunchOCC1) {instLumiBunchOCC1_ = finstLumiBunchOCC1;}

  //---------------------------
  double GetPthat()                          const { return ptHat; }
  double GetGeneratorWeight()                const { return generatorWeight; } 

  int GetNPileUpBxm1()                       const { return nPileUpBxm1_;}
  int GetNPileUpBx0()                        const { return nPileUpBx0_;}
  int GetNPileUpBxp1()                       const { return nPileUpBxp1_;}
  float GetTrueNumInteractions()             const { return nTrueInteractions_;}

  int  GetRunNumber()                        const {return runNumber_;}
  int  GetEventNumber()                      const {return eventNumber_;}
  int  GetBunchCrossing()                    const {return bunchCrossing_;}
  int  GetOrbitNumber()                      const {return orbitNumber_;}
  double GetLumiWeight()                     const { return lumiWeight_;}
  float GetInstLumiBunch()                   const {return instLumiBunchOCC1_;}
  float GetInstDelLumiLS()                   const {return instLumiLS_;}
 
  private:
     friend class eventInfo::EventInfo;

     void reset();
     
     double ptHat;
     double generatorWeight;

     int nPileUpBxm1_;
     int nPileUpBx0_;
     int nPileUpBxp1_;
     float nTrueInteractions_; 

     int runNumber_;
     int eventNumber_;
     int lumiSection_;
     int bunchCrossing_;
     int orbitNumber_; 
     double lumiWeight_;
     float instLumiLS_;
     float instLumiBunchOCC1_;
};

#endif
