import FWCore.ParameterSet.Config as cms

etaMinCandViewSelector = cms.EDFilter("EtaMinCandViewSelector",
   src = cms.InputTag(""),
   maxNumber = cms.uint32(1)
)
