import FWCore.ParameterSet.Config as cms

etaMinCut = -1.0
etaMinSelector = cms.EDFilter("CandViewSelector",
    src = cms.InputTag("etaMinPFCands"),
    cut = cms.string('eta > %f' % etaMinCut),
    filter = cms.bool(True) 
)
