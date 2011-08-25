import FWCore.ParameterSet.Config as cms

processIdFilter = cms.EDFilter("ProcessIdFilter",
    GeneratorTag = cms.InputTag("generator"),
    ProcessIds = cms.vint32()
)
