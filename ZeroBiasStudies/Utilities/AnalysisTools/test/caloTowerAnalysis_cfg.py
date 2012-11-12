import FWCore.ParameterSet.Config as cms

process = cms.Process("Analysis")

process.load("FWCore.MessageLogger.MessageLogger_cfi")
#process.MessageLogger.cerr.FwkReport.reportEvery = 1000 

process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(True)
)

#process.load("Geometry.CMSCommonData.cmsIdealGeometryXML_cfi")
#process.load("Geometry.CaloEventSetup.CaloTowerConstituents_cfi")

#process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
#process.GlobalTag.globaltag = ""
#process.prefer("GlobalTag")

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring("file:/tmp/antoniov/MinimumBias_Commissioning10_GOODCOLL-Jun14thSkim_v1_CA78CDED-2E83-DF11-BDD8-0026189438A7.root")
)

process.TFileService = cms.Service("TFileService",
    fileName = cms.string('analysisHistos_CaloTowerAnalysis.root')
)

process.calotwranalysis = cms.EDAnalyzer("CaloTowerNoiseAnalyzer",
    CaloTowersTag = cms.InputTag("towerMaker"),
    AccessRecHits = cms.untracked.bool(True),
    HFRecHitsLabel = cms.untracked.InputTag("hfreco"),
    NBinsHB = cms.untracked.int32(20),
    NBinsHE = cms.untracked.int32(20),
    NBinsHF = cms.untracked.int32(20),
    NBinsEB = cms.untracked.int32(20),
    NBinsEE = cms.untracked.int32(20),
    NumberOfTresholds = cms.uint32(10),
    TowerEnergyTresholdHBMin = cms.double(0.0),
    TowerEnergyTresholdHBMax = cms.double(10.0),
    TowerEnergyTresholdHEMin = cms.double(0.0),
    TowerEnergyTresholdHEMax = cms.double(10.0),
    TowerEnergyTresholdHFMin = cms.double(0.0),
    TowerEnergyTresholdHFMax = cms.double(10.0),
    TowerEnergyTresholdEBMin = cms.double(0.0),
    TowerEnergyTresholdEBMax = cms.double(10.0),
    TowerEnergyTresholdEEMin = cms.double(0.0),
    TowerEnergyTresholdEEMax = cms.double(10.0),
    ReweightHFTower = cms.bool(False),
    #ReweightHistoName = cms.vstring("",""),
    ApplyEnergyOffset = cms.bool(False) 
)

process.load('MinimumBiasAnalysis.MinimumBiasAnalysis.analysisSequences_cff')

process.generalTracksFilter = cms.EDFilter("TrackCountFilter",
    src = cms.InputTag('generalTracks'),
    minNumber = cms.uint32(1) 
)
#process.pixelLessTracksFilter = cms.EDFilter("TrackCountFilter",
#    src = cms.InputTag('ctfPixelLess'),
#    minNumber = cms.uint32(1) 
#)
process.pixelTracksFilter = cms.EDFilter("TrackCountFilter",
    src = cms.InputTag('pixelTracks'),
    minNumber = cms.uint32(1) 
)

process.vertexVeto = cms.Sequence(~process.primaryVertexFilter)
process.trackVeto = cms.Sequence(~process.generalTracksFilter +
                                 ~process.pixelTracksFilter) 

process.calotwranalysisNoSel = process.calotwranalysis.clone()
process.calotwranalysisColl = process.calotwranalysis.clone()
process.calotwranalysisNoColl = process.calotwranalysis.clone()
process.calotwranalysisNoCollNoVtx = process.calotwranalysis.clone()
process.calotwranalysisNoCollNoTrk = process.calotwranalysis.clone()

process.analysisNoSel = cms.Path(process.calotwranalysisNoSel)
process.analysisColl = cms.Path(process.eventSelectionBscMinBiasOR + process.calotwranalysisColl)
process.analysisNoColl = cms.Path(process.l1NoColl + process.calotwranalysisNoColl)
process.analysisNoCollNoVtx = cms.Path(process.l1NoColl + process.vertexVeto + 
                                       process.calotwranalysisNoCollNoVtx)
process.analysisNoCollNoTrk = cms.Path(process.l1NoColl + process.trackVeto +
                                       process.calotwranalysisNoCollNoTrk)

