import FWCore.ParameterSet.Config as cms

castorActivityFilter = cms.EDFilter("CastorActivityFilter",
    CastorRecHitTag = cms.InputTag("rechitcorrector"),
    SumEMaxCastor = cms.double(250.)
)
