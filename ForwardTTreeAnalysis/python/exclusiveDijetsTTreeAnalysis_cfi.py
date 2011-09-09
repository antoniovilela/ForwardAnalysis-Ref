import FWCore.ParameterSet.Config as cms

from DiffractiveAnalysis_cfi import DiffractiveAnalysis
from ExclusiveDijetsAnalysis_cfi import ExclusiveDijetsAnalysis

exclusiveDijetsTTreeAnalysis = cms.EDAnalyzer("ForwardTTreeProducer",
    diffractiveAnalysis = DiffractiveAnalysis,
    exclusiveDijetsAnalysis = ExclusiveDijetsAnalysis
    )
