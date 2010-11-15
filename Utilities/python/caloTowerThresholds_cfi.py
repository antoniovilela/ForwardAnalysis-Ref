import FWCore.ParameterSet.Config as cms

caloTowerThresholds = cms.PSet(
    EnergyThresholdHB = cms.double(1.25),
    EnergyThresholdHE = cms.double(1.9),
    EnergyThresholdHF = cms.double(4.0),
    EnergyThresholdEB = cms.double(0.6),
    EnergyThresholdEE = cms.double(2.45)
)
