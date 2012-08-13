import FWCore.ParameterSet.Config as cms

pileUpInfoFilter = cms.EDFilter("PileUpInfoFilter",
    pileUpSummaryInfoTag = cms.InputTag("addPileupInfo"),
    numberOfPileUpEventsBx0 = cms.int32(0)
    # Only select on out-of-time pile-up if defined
    #numberOfPileUpEventsBxm1 = cms.int32(0)
    #numberOfPileUpEventsBxp1 = cms.int32(0)
)
