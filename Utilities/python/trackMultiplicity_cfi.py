import FWCore.ParameterSet.Config as cms

trackMultiplicity = cms.EDProducer("TrackMultiplicityEdmNtupleDumper",
   TracksTag = cms.InputTag("generalTracks")
)
