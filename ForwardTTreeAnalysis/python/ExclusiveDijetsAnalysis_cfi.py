import FWCore.ParameterSet.Config as cms

from ForwardAnalysis.AnalysisSequences.pfThresholds_cfi import pfThresholds

ExclusiveDijetsAnalysis = cms.PSet(
    # Input tags
    VertexTag = cms.InputTag("goodOfflinePrimaryVertices"),
    METTag = cms.InputTag("met"),
    CaloTowerTag = cms.InputTag("towerMaker"), 
    CastorRecHitTag = cms.InputTag("castorRecHitCorrector"),
    ParticleFlowTag = cms.InputTag("particleFlow"),
    GenChargedParticlesTag = cms.InputTag("genChargedParticles"),
    TriggerResultsTag = cms.InputTag("TriggerResults::HLT"),
    #HCALTowerSummaryTag = cms.InputTag("hcalActivitySummary"),
    # Misc
    EnergyThresholdHB = cms.double(1.5),
    EnergyThresholdHE = cms.double(2.0),
    EnergyThresholdHF = cms.double(4.0),
    # PF thresholds
    PFlowThresholds = pfThresholds,
    # Options
    ApplyEnergyScaleHCAL = cms.bool(False),
    EnergyScaleFactorHCAL = cms.double(1.0),
    #HLTPath2 = cms.string("HLT_ExclDiJet60_HFAND_v*"),
    #HLTPath3 = cms.string("HLT_ExclDiJet60_HFOR_v*"),
    #HLTPath2 = cms.string("HLT_Jet15U_v*"),
    #HLTPath3 = cms.string("HLT_L1Jet6U_v*"),
    hltPaths = cms.vstring('HLT_ExclDiJet60_HFAND_v*','HLT_Jet60_v*'),
    AccessMCInfo = cms.untracked.bool(True),
    POMPYTMCInfo = cms.untracked.bool(False),
    UsePAT = cms.untracked.bool(False),
    RunOnRECO = cms.untracked.bool(False),
   ## DoBTag = cms.untracked.bool(False),
##)
#########################
###ExclusiveDijetsAnalysis
    # Input tags
    JetTag = cms.InputTag("selectedPatJetsPFlow"),
    JetNonCorrTag = cms.InputTag("ak5PFJets"),
    TrackMultiplicityTag = cms.InputTag("trackMultiplicityTransverseRegion","trackMultiplicity"),
    TrackTag = cms.InputTag(""),
    # PF thresholds
    #PFlowThresholds = pfThresholds,
    # Misc
    HFThresholdIndex = cms.uint32(15),
    UseJetCorrection = cms.bool(False),
    JetCorrectionService = cms.string("ak5PFL2L3"),
    #JetCorrectionService = cms.string("ak5PFL2L3"),
    # If accessing the pile-up info
    AccessPileUpInfo = cms.bool(True),
    EBeam = cms.untracked.double(3500.),
    #RunOnData = cms.untracked.bool(True)
)
