#ifndef EventInfo_h
#define EventInfo_h

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

class EventInfoEvent;

namespace eventInfo {

class EventInfo {
  public:
     typedef EventInfoEvent event_type;
     static const char* name;

     EventInfo() { }
     EventInfo(const edm::ParameterSet&);
     ~EventInfo();

     void begin();
     void begin(const edm::Run&, const edm::EventSetup&);
     void fill(EventInfoEvent&, const edm::Event&, const edm::EventSetup&);
     void end();
  private:
    
     void fillEventInfo(EventInfoEvent&, const edm::Event&, const edm::EventSetup&);
     void fillPileUpInfo(EventInfoEvent&, const edm::Event&, const edm::EventSetup&);

     bool runOnData_;
     bool runWithMCPU_;
     bool runWithGen_;
};

} // namespace
#endif 
