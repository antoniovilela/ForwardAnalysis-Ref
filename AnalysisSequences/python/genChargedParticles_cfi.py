import FWCore.ParameterSet.Config as cms

genChargedParticles = cms.EDFilter("GenParticleSelector",
    filter = cms.bool(False),
    src = cms.InputTag("genParticles"),
    cut = cms.string('charge != 0 & pt > 0.5 & abs(eta) < 2.4 & status = 1')
)
