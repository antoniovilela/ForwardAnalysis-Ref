import FWCore.ParameterSet.Config as cms

pfCandidateCandViewSelector = cms.EDFilter("CandViewSelector",
    src = cms.InputTag("particleFlow"),
    cut = cms.string('')
)
