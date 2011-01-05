import FWCore.ParameterSet.Config as cms

etaMaxGenParticleSelector = cms.EDFilter("EtaMaxGenParticleSelector",
   src = cms.InputTag("genParticles"),
   maxNumber = cms.uint32(1)
)
