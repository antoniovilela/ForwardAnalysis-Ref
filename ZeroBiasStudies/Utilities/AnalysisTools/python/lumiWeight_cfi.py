import FWCore.ParameterSet.Config as cms

lumiWeight = cms.EDProducer("LuminosityWeightProducer",
    rootFileName = cms.string(""),
    prefix = cms.untracked.string("instLumi")
)
