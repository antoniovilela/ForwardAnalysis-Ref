import FWCore.ParameterSet.Config as cms

pileUpInfo = cms.EDProducer("PileUpEdmNtupleDumper",
    AccessCrossingFramePlayBack = cms.untracked.bool(False),
    BunchCrossings = cms.vint32(0)
)

