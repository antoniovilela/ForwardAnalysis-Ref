#include "ForwardAnalysis/ForwardTTreeAnalysis/interface/DiffractiveEvent.h"
#include "ForwardAnalysis/ForwardTTreeAnalysis/interface/ExclusiveDijetsEvent.h"
#include "ForwardAnalysis/ForwardTTreeAnalysis/interface/DiffractiveZEvent.h"
#include "ForwardAnalysis/ForwardTTreeAnalysis/interface/DijetsTriggerEvent.h"
#include "ForwardAnalysis/ForwardTTreeAnalysis/interface/HeavyFlavorEvent.h"
#include "ForwardAnalysis/ForwardTTreeAnalysis/interface/EventInfoEvent.h"
#include "ForwardAnalysis/ForwardTTreeAnalysis/interface/PATTriggerInfoEvent.h"
#include "ForwardAnalysis/ForwardTTreeAnalysis/interface/PATL1Trigger.h"
#include "ForwardAnalysis/ForwardTTreeAnalysis/interface/PATHLTTrigger.h"

/*DiffractiveEvent diffractiveEvent;
ExclusiveDijetsEvent exclusiveDijetsEvent;
DijetsTriggerEvent dijetsTriggerEvent;
HeavyFlavorEvent heavyFlavorEvent;
PATTriggerInfoEvent patTriggerInfoEvent;
PATL1Trigger patL1Trigger;
PATHLTTrigger patHLTTrigger;*/

#include "DataFormats/Common/interface/Wrapper.h"
#include <vector>
namespace {
   struct dictionary {
      DiffractiveEvent diffractiveEvent;
      DiffractiveZEvent diffractiveZEvent;
      ExclusiveDijetsEvent exclusiveDijetsEvent;
      DijetsTriggerEvent dijetsTriggerEvent;
      HeavyFlavorEvent heavyFlavorEvent;
      EventInfoEvent eventInfoEvent;
      PATTriggerInfoEvent patTriggerInfoEvent;
      PATL1Trigger patL1Trigger;
      PATHLTTrigger patHLTTrigger;
      std::vector<PATL1Trigger> vec_patL1Trigger;
      std::vector<PATHLTTrigger> vec_patHLTTrigger;
      edm::Wrapper<DiffractiveEvent> diffractiveEvent_wr;
      edm::Wrapper<DiffractiveZEvent> diffractiveZEvent_wr;
      edm::Wrapper<ExclusiveDijetsEvent> exclusiveDijetsEvent_wr;
      edm::Wrapper<DijetsTriggerEvent> dijetsTriggerEvent_wr;
      edm::Wrapper<HeavyFlavorEvent> heavyFlavorEvent_wr;
      edm::Wrapper<EventInfoEvent> eventInfoEvent_wr;
      edm::Wrapper<PATTriggerInfoEvent> patTriggerInfoEvent_wr;
      edm::Wrapper<PATL1Trigger> patL1Trigger_wr;
      edm::Wrapper<PATHLTTrigger> patHLTTrigger_wr;
      edm::Wrapper<std::vector<PATL1Trigger> > vec_patL1Trigger_wr;
      edm::Wrapper<std::vector<PATHLTTrigger> > vec_patHLTTrigger_wr;
   };
} 
