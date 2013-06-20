import FWCore.ParameterSet.Config as cms

from ForwardAnalysis.DiffractiveZAnalysis.pfThresholds_cfi import pfThresholds

DiffractiveZAnalysis = cms.PSet(
    hltPaths = cms.vstring('HLT_Mu9'),
    TriggerResultsTag = cms.InputTag("TriggerResults::HLT"),
    electronTag = cms.InputTag("gsfElectrons"),
    muonTag = cms.InputTag("muons"),
    genTag = cms.InputTag("genChargedParticles"),
    pfTag = cms.InputTag("pfCandidateNoiseThresholds"), 
    PVtxCollectionTag=cms.InputTag('goodOfflinePrimaryVertices'),
    zdcHitsTag = cms.InputTag("zdcreco"),
    castorHitsTag = cms.InputTag("castorRecHitCorrector"),
    RunMC = cms.untracked.bool(False),
    RunZPat = cms.untracked.bool(True),
    RunZDC = cms.untracked.bool(False),
    RunCastor = cms.untracked.bool(True),
    EachTower = cms.untracked.bool(True),
    fCGeVCastor = cms.double(0.015), # for MC we need use 0.9375 (0.015/0.016)  
    castorThreshold = cms.double(0.364), # 364 MeV for 2010
    pTPFThresholdCharged = cms.double(0.1),
    energyPFThresholdBar = cms.double(1.5),
    energyPFThresholdEnd = cms.double(3.5),
    energyPFThresholdHF = cms.double(7.0),
    energyThresholdHB = cms.double(1.5),
    energyThresholdHE = cms.double(2.0),
    energyThresholdHF = cms.double(7.0),
    energyThresholdEB = cms.double(0.6),
    energyThresholdEE = cms.double(1.5),
    CaloTowerTag = cms.InputTag("towerMaker"),
    )
