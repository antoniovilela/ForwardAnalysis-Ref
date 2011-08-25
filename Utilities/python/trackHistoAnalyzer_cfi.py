import FWCore.ParameterSet.Config as cms

trackHistoAnalyzer = cms.EDAnalyzer('SimpleTrackAnalyzer',
    TrackTag = cms.InputTag('generalTracks'),
    MinPt = cms.double(0.),
    MaxPt = cms.double(20.),
    NBinsPt = cms.uint32(200),
    MinEta = cms.double(-3.0),
    MaxEta = cms.double(3.0),
    NBinsEta = cms.uint32(60),
    MinPtSum = cms.double(0.),
    MaxPtSum = cms.double(50.),
    NBinsPtSum = cms.uint32(100),
    MinNTracks = cms.uint32(0),
    MaxNTracks = cms.uint32(40),
    NBinsNTracks = cms.uint32(40)
)
