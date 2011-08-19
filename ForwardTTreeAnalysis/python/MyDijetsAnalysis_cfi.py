import FWCore.ParameterSet.Config as cms

from ForwardAnalysis.ForwardTTreeAnalysis.pfThresholds_cfi import pfThresholds

MyDijetsAnalysis = cms.PSet(
    # Input tags
    VertexTag = cms.InputTag("offlinePrimaryVertices"),
    METTag = cms.InputTag("met"),
    CaloTowerTag = cms.InputTag("towerMaker"), 
    ParticleFlowTag = cms.InputTag("particleFlow"),
    GenChargedParticlesTag = cms.InputTag("genChargedParticles"),
    TriggerResultsTag = cms.InputTag("TriggerResults::HLT"),
    HCALTowerSummaryTag = cms.InputTag("hcalActivitySummary"),
    # Misc
    EnergyThresholdHB = cms.double(1.5),
    EnergyThresholdHE = cms.double(2.0),
    EnergyThresholdHF = cms.double(4.0),
    # PF thresholds
    PFlowThresholds = pfThresholds,
    # Options
    ApplyEnergyScaleHCAL = cms.bool(False),
    EnergyScaleFactorHCAL = cms.double(1.0),
    #HLTPath1 = cms.string("HLT_Jet30U*"), 
    #HLTPath2 = cms.string("HLT_ExclDiJet30U_HFAND_v*"),
    #HLTPath3 = cms.string("HLT_ExclDiJet30U_HFOR_v*"),
    #HLTPath2 = cms.string("HLT_Jet15U"),
    #HLTPath3 = cms.string("HLT_Jet50U"),
    #HLTPath4 = cms.string("HLT_DiJetAve15U*"),
    #HLTPath5 = cms.string("HLT_DiJetAve30U*"),
    #HLTPath6 = cms.string("HLT_L1Jet6U"),
    HLTPath1 = cms.string("HLT_Jet30_v*"), 
    HLTPath2 = cms.string("HLT_ExclDiJet60_HFAND_v*"),
    HLTPath3 = cms.string("HLT_ExclDiJet60_HFOR_v*"),
    HLTPath4 = cms.string("HLT_Jet80_v*"),
    HLTPath5 = cms.string("HLT_L1SingleJet36_v*"),
    HLTPath6 = cms.string("HLT_Jet60_v*"),
    #HLTPath2 = cms.string("HLT_Jet15U"),
    #HLTPath3 = cms.string("HLT_Jet50U"),
    #HLTPath7 = cms.string("HLT_L1Jet10U"),
    AccessMCInfo = cms.untracked.bool(False),
    POMPYTMCInfo = cms.untracked.bool(False),
    UsePAT = cms.untracked.bool(False),
##)
#########################
###ExclusiveDijetsAnalysis
    # Input tags
    JetTag = cms.InputTag("ak5PFJets"),
    JetNonCorrTag = cms.InputTag("ak5PFJets"),
    TrackMultiplicityTag = cms.InputTag("trackMultiplicityAssociatedToPV","trackMultiplicity"),
    TrackTag = cms.InputTag(""),
    # PF thresholds
    #PFlowThresholds = pfThresholds,
    # Misc
    HFThresholdIndex = cms.uint32(15),
    UseJetCorrection = cms.bool(True),
    JetCorrectionService = cms.string("ak5PFL2L3"),
    #JetCorrectionService = cms.string("ak5PFL2L3"),
    # If accessing the pile-up info
    AccessPileUpInfo = cms.bool(True),
    EBeam = cms.untracked.double(3500.)
    #RunOnData = cms.untracked.bool(True)
)
