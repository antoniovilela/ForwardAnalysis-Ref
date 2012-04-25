import FWCore.ParameterSet.Config as cms

DiffractiveAnalysis = cms.PSet(
    # Input tags
    vertexTag = cms.InputTag("offlinePrimaryVertices"),
    trackTag = cms.InputTag(""),
    jetTag = cms.InputTag(""),
    metTag = cms.InputTag("met"),
    caloTowerTag = cms.InputTag("towerMaker"), 
    castorRecHitTag = cms.InputTag("castorRecHitCorrector"),
    particleFlowTag = cms.InputTag("pfCandidateNoiseThresholds"),
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
    hltPath = cms.string("HLT_L1Tech_BSC_minBias_OR"),
    #hltPath = cms.string(""), 
    techTrigBit = cms.int32(34),
    accessMCInfo = cms.untracked.bool(False)
)
