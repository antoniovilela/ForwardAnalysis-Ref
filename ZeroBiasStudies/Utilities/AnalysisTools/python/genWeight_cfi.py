import FWCore.ParameterSet.Config as cms

genWeight = cms.EDProducer("GeneratorWeightProducer",
    generator = cms.InputTag("generator")
)
