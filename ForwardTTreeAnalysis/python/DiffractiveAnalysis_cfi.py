import FWCore.ParameterSet.Config as cms

DiffractiveAnalysis = cms.PSet(
    # Input tags
    vertexTag = cms.InputTag("goodOfflinePrimaryVertices"),
    trackTag = cms.InputTag("analysisTracks"),
    jetTag = cms.InputTag("selectedPatJetsPFlow"),
    metTag = cms.InputTag("patMETsPFlow"),
    caloTowerTag = cms.InputTag("towerMaker"), 
    castorRecHitTag = cms.InputTag("castorRecHitCorrector"),
    particleFlowTag = cms.InputTag("pfNoPileUpPFlow"),
    genChargedParticlesTag = cms.InputTag("genChargedParticles"),
    triggerResultsTag = cms.InputTag("TriggerResults","","HLT"),
    hcalTowerSummaryTag = cms.InputTag("hcalActivitySummary"),
    # Misc
    energyThresholdHB = cms.double(1.5),
    energyThresholdHE = cms.double(2.0),
    energyThresholdHF = cms.double(4.0),
    # Options
    comEnergy = cms.double(7000.),
    applyEnergyScaleHCAL = cms.bool(False),
    energyScaleFactorHCAL = cms.double(1.0),
    hltPath = cms.string("HLT_L1_BscMinBiasOR_BptxPlusORMinus"), 
    techTrigBit = cms.int32(34),
    accessMCInfo = cms.untracked.bool(False)
)
