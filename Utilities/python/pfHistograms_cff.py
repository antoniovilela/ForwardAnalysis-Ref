import FWCore.ParameterSet.Config as cms

pfPtHistogram = cms.PSet(
    min = cms.untracked.double(0.),
    max = cms.untracked.double(20.),
    nbins = cms.untracked.int32(200),
    name  = cms.untracked.string('pfPt'),
    description  = cms.untracked.string('transverse momentum distribution'),
    plotquantity = cms.untracked.string('pt')
)

pfEtaHistogram = cms.PSet(
    min = cms.untracked.double(-5.0),
    max = cms.untracked.double(5.0),
    nbins = cms.untracked.int32(100),
    name  = cms.untracked.string('pfEta'),
    description  = cms.untracked.string('pseudo-rapidity distribution'),
    plotquantity = cms.untracked.string('eta')
)
