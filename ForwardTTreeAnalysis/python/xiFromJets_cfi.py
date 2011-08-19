import FWCore.ParameterSet.Config as cms

xiFromJets = cms.EDProducer("XiEdmNtupleDumper",
    comEnergy = cms.double(7000.),
    ParticlesTag = cms.InputTag("ak5PFJets"),
    UseMuonInfo = cms.bool(True),
    MuonTag = cms.InputTag("muons"),
    UseMETInfo = cms.bool(True),
    CaloMETTag = cms.InputTag("met"),
    UseCaloTowers = cms.bool(False) 
)
