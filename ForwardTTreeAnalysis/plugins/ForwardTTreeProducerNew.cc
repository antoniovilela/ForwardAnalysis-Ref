#include "FWCore/Framework/interface/MakerMacros.h"

#include "ForwardAnalysis/ForwardTTreeAnalysis/interface/DiffractiveAnalysis.h"
#include "ForwardAnalysis/ForwardTTreeAnalysis/interface/ExclusiveDijetsAnalysis.h"
#include "ForwardAnalysis/ForwardTTreeAnalysis/interface/PATTriggerInfo.h"
#include "ForwardAnalysis/ForwardTTreeAnalysis/interface/DijetsTriggerAnalysis.h"
#include "ForwardAnalysis/ForwardTTreeAnalysis/interface/HeavyFlavorAnalysis.h"

#include "ForwardAnalysis/ForwardTTreeAnalysis/interface/DiffractiveEvent.h"
#include "ForwardAnalysis/ForwardTTreeAnalysis/interface/ExclusiveDijetsEvent.h"
#include "ForwardAnalysis/ForwardTTreeAnalysis/interface/PATTriggerInfoEvent.h"
#include "ForwardAnalysis/ForwardTTreeAnalysis/interface/DijetsTriggerEvent.h"
#include "ForwardAnalysis/ForwardTTreeAnalysis/interface/HeavyFlavorEvent.h"

#include "ForwardAnalysis/ForwardTTreeAnalysis/interface/ForwardTTreeProducer.h"

typedef forwardTTreeAnalysis::ForwardTTreeProducer<patTriggerInfo::PATTriggerInfo> PATTriggerInfoTTree;
DEFINE_FWK_MODULE(PATTriggerInfoTTree);

typedef forwardTTreeAnalysis::ForwardTTreeProducer<diffractiveAnalysis::DiffractiveAnalysis> DiffractiveAnalysisTTree;
DEFINE_FWK_MODULE(DiffractiveAnalysisTTree);

typedef forwardTTreeAnalysis::ForwardTTreeProducer<exclusiveDijetsAnalysis::ExclusiveDijetsAnalysis> ExclusiveDijetsAnalysisTTree;
DEFINE_FWK_MODULE(ExclusiveDijetsAnalysisTTree);

typedef forwardTTreeAnalysis::ForwardTTreeProducer<dijetsTriggerAnalysis::DijetsTriggerAnalysis> DijetsTriggerAnalysisTTree;
DEFINE_FWK_MODULE(DijetsTriggerAnalysisTTree);

typedef forwardTTreeAnalysis::ForwardTTreeProducer<dijetsTriggerAnalysis::DijetsTriggerAnalysis,
                                                   patTriggerInfo::PATTriggerInfo> DijetsTriggerAnalysisPATTriggerInfoTTree;
DEFINE_FWK_MODULE(DijetsTriggerAnalysisPATTriggerInfoTTree);

typedef forwardTTreeAnalysis::ForwardTTreeProducer<diffractiveAnalysis::DiffractiveAnalysis,
                                                   patTriggerInfo::PATTriggerInfo> DiffractiveAnalysisPATTriggerInfoTTree;
DEFINE_FWK_MODULE(DiffractiveAnalysisPATTriggerInfoTTree);

typedef forwardTTreeAnalysis::ForwardTTreeProducer<diffractiveAnalysis::DiffractiveAnalysis,
                                                   exclusiveDijetsAnalysis::ExclusiveDijetsAnalysis,
                                                   patTriggerInfo::PATTriggerInfo> DiffractiveExclusiveDijetsAnalysisPATTriggerInfoTTree;
DEFINE_FWK_MODULE(DiffractiveExclusiveDijetsAnalysisPATTriggerInfoTTree);

typedef forwardTTreeAnalysis::ForwardTTreeProducer<diffractiveAnalysis::DiffractiveAnalysis,
                                                   heavyFlavorAnalysis::HeavyFlavorAnalysis> DiffractiveHeavyFlavorAnalysisTTree;
DEFINE_FWK_MODULE(DiffractiveHeavyFlavorAnalysisTTree);

