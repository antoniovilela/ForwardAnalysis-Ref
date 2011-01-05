import FWCore.ParameterSet.Config as cms

etaMinGenParticleSelector = cms.EDFilter("EtaMinGenParticleSelector",
   src = cms.InputTag("genParticles"),
   maxNumber = cms.uint32(1)
)
