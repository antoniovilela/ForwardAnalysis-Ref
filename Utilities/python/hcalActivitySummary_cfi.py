import FWCore.ParameterSet.Config as cms

hcalActivitySummary = cms.EDProducer("HCALActivitySummary",
    CaloTowersTag = cms.InputTag("towerMaker"),
    TowerEnergyThresholdHBMin = cms.double(0.0),
    TowerEnergyThresholdHBMax = cms.double(10.0),
    TowerEnergyThresholdHEMin = cms.double(0.0),
    TowerEnergyThresholdHEMax = cms.double(20.0),
    TowerEnergyThresholdHFMin = cms.double(0.0),
    TowerEnergyThresholdHFMax = cms.double(20.0),
    TowerEnergyThresholdEBMin = cms.double(0.0),
    TowerEnergyThresholdEBMax = cms.double(10.0),
    TowerEnergyThresholdEEMin = cms.double(0.0),
    TowerEnergyThresholdEEMax = cms.double(20.0),
    NumberOfThresholds = cms.uint32(100),
    DiscardFlaggedTowers = cms.bool(False),
    ApplyEnergyScale = cms.bool(False),
    EnergyScaleFactorHB = cms.double(1.0),
    EnergyScaleFactorHE = cms.double(1.0),
    EnergyScaleFactorHF = cms.double(1.0),
    EnergyScaleFactorEB = cms.double(1.0),
    EnergyScaleFactorEE = cms.double(1.0)
)
