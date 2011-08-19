import FWCore.ParameterSet.Config as cms

pileUpNumberFilter = cms.EDFilter("PileUpNumberFilter",
    PileUpInfoTag = cms.InputTag("pileUpInfo"),
    NumberOfPileUpEvents = cms.uint32(0)
)
