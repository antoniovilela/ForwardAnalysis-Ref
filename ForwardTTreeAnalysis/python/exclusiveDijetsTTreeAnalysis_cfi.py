import FWCore.ParameterSet.Config as cms

from DiffractiveAnalysis_cfi import DiffractiveAnalysis
from ExclusiveDijetsAnalysis_cfi import ExclusiveDijetsAnalysis
from PATInfo_cfi import PATInfo

exclusiveDijetsTTreeAnalysis = cms.EDAnalyzer("ForwardTTreeProducer",
    diffractiveAnalysis = DiffractiveAnalysis,
    exclusiveDijetsAnalysis = ExclusiveDijetsAnalysis,
    patInfo = PATInfo

    )
