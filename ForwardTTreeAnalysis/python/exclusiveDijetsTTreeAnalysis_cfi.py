import FWCore.ParameterSet.Config as cms

from ExclusiveDijetsAnalysis_cfi import ExclusiveDijetsAnalysis

exclusiveDijetsTTreeAnalysis = cms.EDAnalyzer("ForwardTTreeProducer",
    ExclusiveDijetsAnalysis
    )
