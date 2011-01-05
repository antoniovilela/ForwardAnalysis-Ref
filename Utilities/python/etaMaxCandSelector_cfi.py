import FWCore.ParameterSet.Config as cms

etaMaxCandSelector = cms.EDFilter("EtaMaxCandSelector",
   src = cms.InputTag(""),
   maxNumber = cms.uint32(1)
)
