import FWCore.ParameterSet.Config as cms

hfTower = cms.EDProducer("HFTowerEdmNtupleDumperAOD",
    CaloTowersTag = cms.InputTag("towerMaker"),
    TowerEnergyThresholdMin = cms.double(0.0),
    TowerEnergyThresholdMax = cms.double(10.0),
    NumberOfThresholds = cms.uint32(50),
    DiscardFlaggedTowers = cms.bool(False),
    ApplyEnergyScale = cms.bool(False),
    EnergyScaleFactor = cms.double(1.0)
)
