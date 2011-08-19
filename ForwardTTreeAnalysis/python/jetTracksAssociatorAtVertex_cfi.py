import FWCore.ParameterSet.Config as cms

jetTracksAssociatorAtVertex = cms.EDFilter("JetTracksAssociatorAtVertex",
    jets = cms.InputTag("ak5PFJets"),
    tracks = cms.InputTag("generalTracks"),
    coneSize = cms.double(0.5)
)
