import FWCore.ParameterSet.Config as cms

xiTower = cms.EDProducer("XiTowerDumper",
    CaloTowersTag = cms.InputTag("towerMaker"),
    MuonTag = cms.InputTag("muons"),
    comEnergy = cms.double(10000.),
    UseMETInfo = cms.bool(True),
    CaloMETTag = cms.InputTag("met"),
    HFPlusTowerThreshold = cms.double(1.0),
    HFMinusTowerThreshold = cms.double(1.0),
    HEPlusTowerThreshold = cms.double(1.0),
    HEMinusTowerThreshold = cms.double(1.0),
    HBTowerThreshold = cms.double(1.0)
)
