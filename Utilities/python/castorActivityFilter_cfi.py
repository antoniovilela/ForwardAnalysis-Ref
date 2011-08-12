import FWCore.ParameterSet.Config as cms

castorActivityFilter = cms.EDFilter("CastorActivityFilter",
    CastorRecHitTag = cms.InputTag("rechitcorrector"),
    # MC
    #CastorRecHitTag = cms.InputTag("castorreco"),
    SumEMaxCastor = cms.double(250.)
)
