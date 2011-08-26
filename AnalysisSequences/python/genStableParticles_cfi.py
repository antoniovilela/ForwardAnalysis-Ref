import FWCore.ParameterSet.Config as cms

genStableParticles = cms.EDFilter("GenParticleSelector",
    filter = cms.bool(False),
    src = cms.InputTag("genParticles"),
    cut = cms.string('status = 1')
)
