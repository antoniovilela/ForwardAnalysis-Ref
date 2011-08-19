import FWCore.ParameterSet.Config as cms

tracksOutsideJets = cms.EDProducer("TrackOutsideJetsSelector",
   src = cms.InputTag("generalTracks"),
   copyExtras = cms.untracked.bool(False),
   copyTrajectories = cms.untracked.bool(False),
   JetTag = cms.InputTag("leadingJets"),
   JetConeSize = cms.double(0.5)
)
