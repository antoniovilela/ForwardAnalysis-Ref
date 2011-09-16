import FWCore.ParameterSet.Config as cms

trackMultiplicity = cms.EDProducer("TrackMultiplicityEdmNtupleDumper",
   src = cms.InputTag("generalTracks")
)
