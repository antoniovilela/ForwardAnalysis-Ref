import FWCore.ParameterSet.Config as cms

ptHistogram = cms.PSet(
    min = cms.untracked.double(0.),
    max = cms.untracked.double(20.),
    nbins = cms.untracked.int32(200),
    name  = cms.untracked.string('candPt'),
    description  = cms.untracked.string('transverse momentum distribution'),
    plotquantity = cms.untracked.string('pt')
)

etaHistogram = cms.PSet(
    min = cms.untracked.double(-5.0),
    max = cms.untracked.double(5.0),
    nbins = cms.untracked.int32(100),
    name  = cms.untracked.string('candEta'),
    description  = cms.untracked.string('pseudo-rapidity distribution'),
    plotquantity = cms.untracked.string('eta')
)
