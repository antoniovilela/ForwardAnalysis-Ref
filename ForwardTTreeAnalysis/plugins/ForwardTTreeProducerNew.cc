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

#include "ForwardAnalysis/ForwardTTreeAnalysis/interface/ForwardTTreeProducer.h"

typedef forwardTTreeAnalysis::ForwardTTreeProducer<patTriggerInfo::PATTriggerInfo> PATTriggerInfoTTree;
DEFINE_FWK_MODULE(PATTriggerInfoTTree);

typedef forwardTTreeAnalysis::ForwardTTreeProducer<eventInfo::EventInfo> EventInfoTTree;
DEFINE_FWK_MODULE(EventInfoTTree);

typedef forwardTTreeAnalysis::ForwardTTreeProducer<diffractiveAnalysis::DiffractiveAnalysis> DiffractiveAnalysisTTree;
DEFINE_FWK_MODULE(DiffractiveAnalysisTTree);

typedef forwardTTreeAnalysis::ForwardTTreeProducer<exclusiveDijetsAnalysis::ExclusiveDijetsAnalysis> ExclusiveDijetsAnalysisTTree;
DEFINE_FWK_MODULE(ExclusiveDijetsAnalysisTTree);

typedef forwardTTreeAnalysis::ForwardTTreeProducer<dijetsTriggerAnalysis::DijetsTriggerAnalysis> DijetsTriggerAnalysisTTree;
DEFINE_FWK_MODULE(DijetsTriggerAnalysisTTree);

typedef forwardTTreeAnalysis::ForwardTTreeProducer<eventInfo::EventInfo,
                                                   exclusiveDijetsAnalysis::ExclusiveDijetsAnalysis> EventInfoExclusiveDijetsAnalysisTTree;
DEFINE_FWK_MODULE(EventInfoExclusiveDijetsAnalysisTTree);

typedef forwardTTreeAnalysis::ForwardTTreeProducer<dijetsTriggerAnalysis::DijetsTriggerAnalysis,
                                                   patTriggerInfo::PATTriggerInfo> DijetsTriggerAnalysisPATTriggerInfoTTree;
DEFINE_FWK_MODULE(DijetsTriggerAnalysisPATTriggerInfoTTree);

typedef forwardTTreeAnalysis::ForwardTTreeProducer<diffractiveAnalysis::DiffractiveAnalysis,
                                                   patTriggerInfo::PATTriggerInfo> DiffractiveAnalysisPATTriggerInfoTTree;
DEFINE_FWK_MODULE(DiffractiveAnalysisPATTriggerInfoTTree);

typedef forwardTTreeAnalysis::ForwardTTreeProducer<diffractiveAnalysis::DiffractiveAnalysis,
                                                   heavyFlavorAnalysis::HeavyFlavorAnalysis> DiffractiveHeavyFlavorAnalysisTTree;
DEFINE_FWK_MODULE(DiffractiveHeavyFlavorAnalysisTTree);

typedef forwardTTreeAnalysis::ForwardTTreeProducer<eventInfo::EventInfo,
                                                   diffractiveAnalysis::DiffractiveAnalysis,
                                                   exclusiveDijetsAnalysis::ExclusiveDijetsAnalysis> EventInfoDiffractiveExclusiveDijetsAnalysisTTree;
DEFINE_FWK_MODULE(EventInfoDiffractiveExclusiveDijetsAnalysisTTree);

typedef forwardTTreeAnalysis::ForwardTTreeProducer<diffractiveAnalysis::DiffractiveAnalysis,
                                                   exclusiveDijetsAnalysis::ExclusiveDijetsAnalysis,
                                                   patTriggerInfo::PATTriggerInfo> DiffractiveExclusiveDijetsAnalysisPATTriggerInfoTTree;
DEFINE_FWK_MODULE(DiffractiveExclusiveDijetsAnalysisPATTriggerInfoTTree);
typedef forwardTTreeAnalysis::ForwardTTreeProducer<diffractiveAnalysis::DiffractiveAnalysis,
                                                   exclusiveDijetsAnalysis::ExclusiveDijetsAnalysis,
                                                   eventInfo::EventInfo,
                                                   patTriggerInfo::PATTriggerInfo> EventInfoDiffractiveExclusiveDijetsAnalysisPATTriggerInfoTTree;

DEFINE_FWK_MODULE(EventInfoDiffractiveExclusiveDijetsAnalysisPATTriggerInfoTTree);
