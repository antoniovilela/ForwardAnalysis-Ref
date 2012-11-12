import FWCore.ParameterSet.Config as cms

pFlowNoiseAnalyzer = cms.EDAnalyzer("PFlowNoiseAnalyzer",
    particleFlowTag = cms.InputTag("particleFlow"),
    weight = cms.InputTag("genWeight"),
    #energyMin = cms.double(-1.),
    #energyMax = cms.double(20.),
    #nBins = cms.int32(400),
    energyMin = cms.double(-10.),
    energyMax = cms.double(190.),
    nBins = cms.int32(4000),
    applyHFEnergyCorrection = cms.bool(False),
    applyHFDeadMaterialCorrection = cms.bool(False)
)
