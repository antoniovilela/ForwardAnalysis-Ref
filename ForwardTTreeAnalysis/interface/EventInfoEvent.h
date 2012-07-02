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

  void SetNPileUpBxm1(int fnPileUpBxm1)                    { nPileUpBxm1_          = fnPileUpBxm1;}
  void SetNPileUpBx0(int fnPileUpBx0)                      { nPileUpBx0_           = fnPileUpBx0;}
  void SetNPileUpBxp1(int fnPileUpBxp1)                    { nPileUpBxp1_          = fnPileUpBxp1;}
  void SetTrueNumInteractions(int fnTrueInteractions)  { nTrueInteractions_ = fnTrueInteractions;}

  void SetRunNumber(int frunNumber)             { runNumber_        = frunNumber;}   
  void SetEventNumber(int feventNumber)         { eventNumber_      = feventNumber;}
  void SetLumiSection(int flumiSection)         { lumiSection_      = flumiSection;}
  void SetBunchCrossing(int fbunchCrossing)     { bunchCrossing_    = fbunchCrossing;}
  void SetLumiWeight(double flumiWeight)        { lumiWeight_       = flumiWeight;}
  void SetInstDelLumiLS(float finstLumiLS)      { instLumiLS_       = finstLumiLS;}
  void SetInstLumiBunch(float finstLumiBunchOCC1) {instLumiBunchOCC1_ = finstLumiBunchOCC1;}

  //---------------------------
  int GetNPileUpBxm1()                       const { return nPileUpBxm1_;}
  int GetNPileUpBx0()                        const { return nPileUpBx0_;}
  int GetNPileUpBxp1()                       const { return nPileUpBxp1_;}
  int GetTrueNumInteractions()              const { return nTrueInteractions_;}

  int  GetRunNumber()                        const {return runNumber_;}
  int  GetEventNumber()                      const {return eventNumber_;}
  int  GetBunchCrossing()                    const {return bunchCrossing_;}
  double GetLumiWeight()                     const { return lumiWeight_;}
  float GetInstLumiBunch()                   const {return instLumiBunchOCC1_;}
  float GetInstDelLumiLS()                   const {return instLumiLS_;}
 
  private:
     friend class eventInfo::EventInfo;

     void reset();
     
     int nPileUpBxm1_;
     int nPileUpBx0_;
     int nPileUpBxp1_;
     int nTrueInteractions_; 

     int runNumber_;
     int eventNumber_;
     int lumiSection_;
     int bunchCrossing_; 
     double lumiWeight_;
     float instLumiLS_;
     float instLumiBunchOCC1_;
};

#endif
