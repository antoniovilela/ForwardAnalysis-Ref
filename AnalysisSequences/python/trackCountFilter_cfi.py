import FWCore.ParameterSet.Config as cms

trackCountFilter = cms.EDFilter("TrackCountFilter",
    src = cms.InputTag(''),
    minNumber = cms.uint32(1) 
)
