#include "FWCore/Framework/interface/MakerMacros.h"

#include "ForwardAnalysis/ForwardTTreeAnalysis/interface/DiffractiveAnalysis.h"
#include "ForwardAnalysis/ForwardTTreeAnalysis/interface/ExclusiveDijetsAnalysis.h"
#include "ForwardAnalysis/ForwardTTreeAnalysis/interface/PATInfo.h"
#include "ForwardAnalysis/ForwardTTreeAnalysis/interface/DijetsTriggerAnalysis.h"

#include "ForwardAnalysis/ForwardTTreeAnalysis/interface/DiffractiveEvent.h"
#include "ForwardAnalysis/ForwardTTreeAnalysis/interface/ExclusiveDijetsEvent.h"
#include "ForwardAnalysis/ForwardTTreeAnalysis/interface/PATInfoEvent.h"
#include "ForwardAnalysis/ForwardTTreeAnalysis/interface/DijetsTriggerEvent.h"

#include "ForwardAnalysis/ForwardTTreeAnalysis/interface/ForwardTTreeProducer.h"

typedef forwardTTreeAnalysis::ForwardTTreeProducer<patInfo::PATInfo> PATInfoTTree;
DEFINE_FWK_MODULE(PATInfoTTree);

typedef forwardTTreeAnalysis::ForwardTTreeProducer<diffractiveAnalysis::DiffractiveAnalysis> DiffractiveAnalysisTTree;
DEFINE_FWK_MODULE(DiffractiveAnalysisTTree);

typedef forwardTTreeAnalysis::ForwardTTreeProducer<exclusiveDijetsAnalysis::ExclusiveDijetsAnalysis> ExclusiveDijetsAnalysisTTree;
DEFINE_FWK_MODULE(ExclusiveDijetsAnalysisTTree);

typedef forwardTTreeAnalysis::ForwardTTreeProducer<dijetsTriggerAnalysis::DijetsTriggerAnalysis> DijetsTriggerAnalysisTTree;
DEFINE_FWK_MODULE(DijetsTriggerAnalysisTTree);

typedef forwardTTreeAnalysis::ForwardTTreeProducer<dijetsTriggerAnalysis::DijetsTriggerAnalysis,
                                                   patInfo::PATInfo> DijetsTriggerAnalysisPATInfoTTree;
DEFINE_FWK_MODULE(DijetsTriggerAnalysisPATInfoTTree);

typedef forwardTTreeAnalysis::ForwardTTreeProducer<diffractiveAnalysis::DiffractiveAnalysis,
                                                   patInfo::PATInfo> DiffractiveAnalysisPATInfoTTree;
DEFINE_FWK_MODULE(DiffractiveAnalysisPATInfoTTree);

typedef forwardTTreeAnalysis::ForwardTTreeProducer<diffractiveAnalysis::DiffractiveAnalysis,
                                                   exclusiveDijetsAnalysis::ExclusiveDijetsAnalysis,
                                                   patInfo::PATInfo> DiffractiveExclusiveDijetsAnalysisPATInfoTTree;
DEFINE_FWK_MODULE(DiffractiveExclusiveDijetsAnalysisPATInfoTTree);

