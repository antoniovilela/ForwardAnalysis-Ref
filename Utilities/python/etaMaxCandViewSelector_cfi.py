import FWCore.ParameterSet.Config as cms

etaMaxCandViewSelector = cms.EDFilter("EtaMaxCandViewSelector",
   src = cms.InputTag(""),
   maxNumber = cms.uint32(1)
)
