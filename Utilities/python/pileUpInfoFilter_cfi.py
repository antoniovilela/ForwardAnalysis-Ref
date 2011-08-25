import FWCore.ParameterSet.Config as cms

pileUpInfoFilter = cms.EDFilter("PileUpInfoFilter",
    pileUpInfoTag = cms.InputTag("addPileupInfo"),
    numberOfPileUpEvents = cms.uint32(0)
)
