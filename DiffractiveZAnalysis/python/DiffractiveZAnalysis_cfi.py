import FWCore.ParameterSet.Config as cms

from ForwardAnalysis.DiffractiveZAnalysis.pfThresholds_cfi import pfThresholds

DiffractiveZAnalysis = cms.PSet(
    hltPaths = cms.vstring('HLT_Mu3'),
    TriggerResultsTag = cms.InputTag("TriggerResults::HLT"),
    electronTag = cms.InputTag("gsfElectrons"),
    muonTag = cms.InputTag("muons"),
    genTag = cms.InputTag("genChargedParticles"),
    pfTag = cms.InputTag("pfCandidateNoiseThresholds"), 
    PVtxCollectionTag=cms.InputTag('goodOfflinePrimaryVertices'),
    RunMC = cms.untracked.bool(False),
    RunZPat = cms.untracked.bool(True),
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
