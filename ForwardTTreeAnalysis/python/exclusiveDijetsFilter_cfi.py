import FWCore.ParameterSet.Config as cms

exclusiveDijetsFilter = cms.EDFilter("ExclusiveDijetsFilter",
# cms.EDAnalyzer("ExclusiveDijetsFilter",
    # Input tags
    JetTag = cms.InputTag("ak5PFJets"),
    ParticleFlowTag = cms.InputTag("particleFlow"),
    TrackMultiplicityTag = cms.InputTag("trackMultiplicityAssociatedToPV","trackMultiplicity"),
    #HCALTowerSummaryTag = cms.InputTag("hcalActivitySummary"),
    # TrackMultiplicityTag = cms.InputTag("analysisTracks","trackMultiplicity"),
    # Selection cuts
    PtMinJet = cms.double(20.0),
    EtaMaxJet = cms.double(2.0),
    DeltaEtaMax = cms.double(999.),
    DeltaPhiMax = cms.double(999.),
    DeltaPtMax = cms.double(999.),
    ThirdJetPtMax = cms.double(9999.),
    NTracksMax = cms.uint32(9999),
    NHFPlusMax = cms.uint32(9999),
    NHFMinusMax = cms.uint32(9999),
  
    # Misc
    HFThresholdIndex = cms.uint32(15)
  
)
