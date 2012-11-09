import FWCore.ParameterSet.Config as cms

process = cms.Process("Analysis")

process.load("FWCore.MessageLogger.MessageLogger_cfi")
#process.MessageLogger.cerr.FwkReport.reportEvery = 1000 

process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(True)
)
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring('file:/storage1/eliza/samples_test/MinimumBias_ZeorBias.root')
)

process.TFileService = cms.Service("TFileService",
    fileName = cms.string('analysisHistos_CaloTowerAnalysis.root')
)

#process.load('Utilities.AnalysisSequences.defaultSequences_cff')
#process.TFileService.fileName = 'analysisHistos_caloTowerNoiseAnalysis.root'

#############
process.load('Utilities.AnalysisSequences.minimumBiasAnalysisSequences_cff')
process.multipleVertexFilter.src = 'primaryVertexFilter'
process.load('Utilities.AnalysisSequences.hltFilter_cfi')
process.hltBPTXPlusOrMinusOnly = process.hltFilter.clone(
    HLTPaths = ['HLT_L1_BPTX_PlusOnly','HLT_L1_BPTX_MinusOnly']
)
process.hltZeroBias = process.hltFilter.clone(
    #andOr = cms.bool(False),
    HLTPaths = ['HLT_ZeroBias']
)
process.load('Utilities.AnalysisSequences.primaryVertexFilter_cfi')
process.load('Utilities.AnalysisSequences.trackCountFilter_cfi')
process.generalTracksFilter = process.trackCountFilter.clone(src = 'generalTracks')
#process.pixelLessTracksFilter = process.trackCountFilter.clone(src = 'ctfPixelLess')
process.pixelTracksFilter = process.trackCountFilter.clone(src = 'pixelTracks')

process.vertexVeto = cms.Sequence(~process.primaryVertexFilter)
process.trackVeto = cms.Sequence(~process.generalTracksFilter + ~process.pixelTracksFilter) 
#############

process.load('Utilities.AnalysisTools.caloTowerNoiseAnalyzer_cfi')
process.caloTowerAnalysisNoSel = process.caloTowerNoiseAnalyzer.clone()
process.caloTowerAnalysisColl = process.caloTowerNoiseAnalyzer.clone()
process.caloTowerAnalysisNoColl = process.caloTowerNoiseAnalyzer.clone()
process.caloTowerAnalysisNoCollNoVtx = process.caloTowerNoiseAnalyzer.clone()
process.caloTowerAnalysisNoCollNoTrk = process.caloTowerNoiseAnalyzer.clone()
process.caloTowerAnalysisBPTXPlusOrMinusOnly = process.caloTowerNoiseAnalyzer.clone()
process.caloTowerAnalysisBPTXPlusOrMinusOnlyNoVtx = process.caloTowerNoiseAnalyzer.clone()
process.caloTowerAnalysisBPTXPlusOrMinusOnlyNoTrk = process.caloTowerNoiseAnalyzer.clone()

process.caloTowerAnalysisZeroBias = process.caloTowerNoiseAnalyzer.clone()
process.caloTowerAnalysisZeroBiasNoVtx = process.caloTowerNoiseAnalyzer.clone()
process.caloTowerAnalysisZeroBiasNoTrk = process.caloTowerNoiseAnalyzer.clone()

process.analysisNoSel = cms.Path(process.caloTowerAnalysisNoSel)
process.analysisColl = cms.Path(process.l1CollBscOr+
                                process.offlineSelection+
                                process.caloTowerAnalysisColl)
process.analysisNoColl = cms.Path(process.l1NoColl+
                                  process.caloTowerAnalysisNoColl)
process.analysisNoCollNoVtx = cms.Path(process.l1NoColl+
                                       process.vertexVeto+ 
                                       process.caloTowerAnalysisNoCollNoVtx)
process.analysisNoCollNoTrk = cms.Path(process.l1NoColl+
                                       process.trackVeto+
                                       process.caloTowerAnalysisNoCollNoTrk)
process.analysisBPTXPlusOrMinusOnly = cms.Path(process.hltBPTXPlusOrMinusOnly+
                                               process.caloTowerAnalysisBPTXPlusOrMinusOnly) 
process.analysisBPTXPlusOrMinusOnlyNoVtx = cms.Path(process.hltBPTXPlusOrMinusOnly+
                                                    process.vertexVeto+
                                                    process.caloTowerAnalysisBPTXPlusOrMinusOnlyNoVtx)
process.analysisBPTXPlusOrMinusOnlyNoTrk = cms.Path(process.hltBPTXPlusOrMinusOnly+
                                                    process.trackVeto+
                                                    process.caloTowerAnalysisBPTXPlusOrMinusOnlyNoTrk)

process.analysisZeroBias = cms.Path(process.hltZeroBias+
                                               process.caloTowerAnalysisZeroBias)
process.analysisZeroBiasNoVtx = cms.Path(process.hltZeroBias+
                                                    process.vertexVeto+
                                                    process.caloTowerAnalysisZeroBiasNoVtx)
process.analysisZeroBiasNoTrk = cms.Path(process.hltZeroBias+
                                                    process.trackVeto+
                                                    process.caloTowerAnalysisZeroBiasNoTrk)
