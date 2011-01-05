import FWCore.ParameterSet.Config as cms

etaMinCandSelector = cms.EDFilter("EtaMinCandSelector",
   src = cms.InputTag(""),
   maxNumber = cms.uint32(1)
)
