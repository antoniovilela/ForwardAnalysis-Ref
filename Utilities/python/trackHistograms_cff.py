import FWCore.ParameterSet.Config as cms

trackPtHistogram = cms.PSet(
    min = cms.untracked.double(0.),
    max = cms.untracked.double(20.),
    nbins = cms.untracked.int32(200),
    name  = cms.untracked.string('TrackPt'),
    description  = cms.untracked.string('track transverse momentum'),
    plotquantity = cms.untracked.string('pt')
)

trackEtaHistogram = cms.PSet(
    min = cms.untracked.double(-3.0),
    max = cms.untracked.double(3.0),
    nbins = cms.untracked.int32(60),
    name  = cms.untracked.string('TrackEta'),
    description  = cms.untracked.string('track pseudo-rapidity'),
    plotquantity = cms.untracked.string('eta')
)
