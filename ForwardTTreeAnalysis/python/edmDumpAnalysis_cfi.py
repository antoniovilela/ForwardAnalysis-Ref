import FWCore.ParameterSet.Config as cms

edmDumpAnalysis = cms.EDAnalyzer("DijetsEdmDumpAnalyzer",
    # Input tags
    JetTag = cms.InputTag("ak5PFJetsL2L3"),
    JetNonCorrTag = cms.InputTag("ak5PFJets"),
    ParticleFlowTag = cms.InputTag("particleFlow"),
    # Selection cuts
    PtMinJet = cms.double(20.0),
    EtaMaxJet = cms.double(2.5),
    DeltaEtaMax = cms.double(2.0),
    DeltaPhiMax = cms.double(0.4),
    DeltaPtMax = cms.double(99999.),
    ThirdJetPtMax = cms.double(99999.),
    NTracksMax = cms.uint32(3),
    NHFPlusMax = cms.uint32(0),
    NHFMinusMax = cms.uint32(0),
     
    # Misc
    HFThresholdIndex = cms.uint32(15),
    UseJetCorrection = cms.bool(False),
    JetCorrectionService = cms.string("ak5PFJetsL2L3"),
    EBeam = cms.untracked.double(3500.)
)
