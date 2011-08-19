import FWCore.ParameterSet.Config as cms

singleInteractionFilter = cms.EDFilter("SingleInteractionFilter",
    PileUpInfoTag = cms.InputTag("pileUpInfo")
)
