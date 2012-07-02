#include "FWCore/Framework/interface/MakerMacros.h"

#include "ForwardAnalysis/ForwardTTreeAnalysis/interface/PATTriggerInfo.h"
#include "ForwardAnalysis/ForwardTTreeAnalysis/interface/EventInfo.h"
#include "ForwardAnalysis/ForwardTTreeAnalysis/interface/DiffractiveAnalysis.h"
#include "ForwardAnalysis/ForwardTTreeAnalysis/interface/ExclusiveDijetsAnalysis.h"
#include "ForwardAnalysis/ForwardTTreeAnalysis/interface/DijetsTriggerAnalysis.h"
#include "ForwardAnalysis/ForwardTTreeAnalysis/interface/HeavyFlavorAnalysis.h"

#include "ForwardAnalysis/ForwardTTreeAnalysis/interface/PATTriggerInfoEvent.h"
#include "ForwardAnalysis/ForwardTTreeAnalysis/interface/EventInfoEvent.h"
#include "ForwardAnalysis/ForwardTTreeAnalysis/interface/DiffractiveEvent.h"
#include "ForwardAnalysis/ForwardTTreeAnalysis/interface/ExclusiveDijetsEvent.h"
#include "ForwardAnalysis/ForwardTTreeAnalysis/interface/DijetsTriggerEvent.h"
#include "ForwardAnalysis/ForwardTTreeAnalysis/interface/HeavyFlavorEvent.h"

#include "ForwardAnalysis/ForwardTTreeAnalysis/interface/ForwardEDMProducer.h"

typedef forwardEDMAnalysis::ForwardEDMProducer<patTriggerInfo::PATTriggerInfo> PATTriggerInfoEDMProducer;
DEFINE_FWK_MODULE(PATTriggerInfoEDMProducer);

typedef forwardEDMAnalysis::ForwardEDMProducer<eventInfo::EventInfo> EventInfoEDMProducer;
DEFINE_FWK_MODULE(EventInfoEDMProducer);

typedef forwardEDMAnalysis::ForwardEDMProducer<diffractiveAnalysis::DiffractiveAnalysis> DiffractiveAnalysisEDMProducer;
DEFINE_FWK_MODULE(DiffractiveAnalysisEDMProducer);

typedef forwardEDMAnalysis::ForwardEDMProducer<exclusiveDijetsAnalysis::ExclusiveDijetsAnalysis> ExclusiveDijetsAnalysisEDMProducer;
DEFINE_FWK_MODULE(ExclusiveDijetsAnalysisEDMProducer);

typedef forwardEDMAnalysis::ForwardEDMProducer<dijetsTriggerAnalysis::DijetsTriggerAnalysis> DijetsTriggerAnalysisEDMProducer;
DEFINE_FWK_MODULE(DijetsTriggerAnalysisEDMProducer);

typedef forwardEDMAnalysis::ForwardEDMProducer<heavyFlavorAnalysis::HeavyFlavorAnalysis> HeavyFlavorAnalysisEDMProducer;
DEFINE_FWK_MODULE(HeavyFlavorAnalysisEDMProducer);
