import FWCore.ParameterSet.Config as cms

pfCandidateSelector = cms.EDFilter("PFCandidateSelector",
    src = cms.InputTag("particleFlow"),
    cut = cms.string('')
)
