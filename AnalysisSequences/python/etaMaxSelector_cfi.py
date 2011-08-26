import FWCore.ParameterSet.Config as cms

etaMaxCut = 1.0
etaMaxSelector = cms.EDFilter("CandViewSelector",
    src = cms.InputTag("etaMaxPFCands"),
    cut = cms.string('eta < %f' % etaMaxCut),
    filter = cms.bool(True) 
)
