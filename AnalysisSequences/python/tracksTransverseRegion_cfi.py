import FWCore.ParameterSet.Config as cms
from math import pi

tracksTransverseRegion = cms.EDProducer("TrackWithJetSelector",
   src = cms.InputTag("generalTracks"),
   copyExtras = cms.untracked.bool(False),
   copyTrajectories = cms.untracked.bool(False),
   JetTag = cms.InputTag("ak5PFJets"),
   DeltaPhiMin = cms.double(pi/3),
   DeltaPhiMax = cms.double(2*pi/3)
)
