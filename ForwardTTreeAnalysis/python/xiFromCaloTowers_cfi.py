import FWCore.ParameterSet.Config as cms

xiFromCaloTowers = cms.EDProducer("XiEdmNtupleDumper",
    comEnergy = cms.double(10000.),
    ParticlesTag = cms.InputTag("towerMaker"),
    UseCaloTowers = cms.bool(True),
    HFPlusTowerThreshold = cms.double(1.0),
    HFMinusTowerThreshold = cms.double(1.0),
    HEPlusTowerThreshold = cms.double(1.0),
    HEMinusTowerThreshold = cms.double(1.0),
    HBTowerThreshold = cms.double(1.0),
    UseMuonInfo = cms.bool(True),
    # Muons
    MuonTag = cms.InputTag("muons"),
    UseMETInfo = cms.bool(True),
    # MET
    CaloMETTag = cms.InputTag("met")
)
