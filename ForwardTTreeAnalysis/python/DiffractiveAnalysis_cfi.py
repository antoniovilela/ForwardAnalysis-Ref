import FWCore.ParameterSet.Config as cms

DiffractiveAnalysis = cms.PSet(
    # Input tags
    accessCastorInfo = cms.bool(True),
    accessZDCInfo = cms.bool(True),
    vertexTag = cms.InputTag("offlinePrimaryVertices"),
    trackTag = cms.InputTag("generalTracks"),
    jetTag = cms.InputTag("ak5PFJets"),
    metTag = cms.InputTag("met"),
    caloTowerTag = cms.InputTag("towerMaker"), 
    castorRecHitTag = cms.InputTag("castorRecHitCorrector"),
    zdcHitsTag = cms.InputTag("zdcreco"),
    fCGeVCastor = cms.double(0.015), # for MC we need use 0.9375 (0.015/0.016)  
    castorThreshold = cms.double(0.364), # 364 MeV for 2010
    particleFlowTag = cms.InputTag("pfCandidateNoiseThresholds"),
    edmNtupleEtaMaxTag = cms.InputTag("edmNtupleEtaMax"),
    edmNtupleEtaMinTag = cms.InputTag("edmNtupleEtaMin"),
    genChargedParticlesTag = cms.InputTag("genChargedParticles"),
    triggerResultsTag = cms.InputTag("TriggerResults","","HLT"),
    hcalTowerSummaryTag = cms.InputTag("hcalActivitySummary"),
    # Misc
    energyThresholdHB = cms.double(1.5),
    energyThresholdHE = cms.double(2.0),
    energyThresholdHF = cms.double(7.0),
    # Options
    comEnergy = cms.double(7000.),
    applyEnergyScaleHCAL = cms.bool(False),
    energyScaleFactorHCAL = cms.double(1.0),
    hltPath = cms.string("HLT_L1Tech_BSC_minBias_OR"),
    #hltPath = cms.string(""), 
    techTrigBit = cms.int32(34),
    accessMCInfo = cms.untracked.bool(False)
)
