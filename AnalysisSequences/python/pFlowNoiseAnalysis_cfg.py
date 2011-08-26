import FWCore.ParameterSet.Config as cms

process = cms.Process("Analysis")

process.load('Utilities.AnalysisSequences.defaultSequences_cff')
process.TFileService.fileName = 'analysisHistos_pFlowNoiseAnalysis.root'

#############
process.load('Utilities.AnalysisSequences.minimumBiasAnalysisSequences_cff')
process.load('Utilities.AnalysisSequences.hltFilter_cfi')
process.hltBPTXPlusOrMinusOnly = process.hltFilter.clone(
    HLTPaths = ['HLT_L1_BPTX_PlusOnly','HLT_L1_BPTX_MinusOnly']
)
process.load('Utilities.AnalysisSequences.primaryVertexFilter_cfi')
process.load('Utilities.AnalysisSequences.trackCountFilter_cfi')
process.generalTracksFilter = process.trackCountFilter.clone(src = 'generalTracks')
#process.pixelLessTracksFilter = process.trackCountFilter.clone(src = 'ctfPixelLess')
process.pixelTracksFilter = process.trackCountFilter.clone(src = 'pixelTracks')

process.vertexVeto = cms.Sequence(~process.primaryVertexFilter)
process.trackVeto = cms.Sequence(~process.generalTracksFilter + ~process.pixelTracksFilter) 
#############

process.load('Utilities.AnalysisTools.pFlowNoiseAnalyzer_cfi')
process.pFlowAnalysisNoSel = process.pFlowNoiseAnalyzer.clone()
process.pFlowAnalysisColl = process.pFlowNoiseAnalyzer.clone()
process.pFlowAnalysisNoColl = process.pFlowNoiseAnalyzer.clone()
process.pFlowAnalysisNoCollNoVtx = process.pFlowNoiseAnalyzer.clone()
process.pFlowAnalysisNoCollNoTrk = process.pFlowNoiseAnalyzer.clone()
process.pFlowAnalysisBPTXPlusOrMinusOnly = process.pFlowNoiseAnalyzer.clone()
process.pFlowAnalysisBPTXPlusOrMinusOnlyNoVtx = process.pFlowNoiseAnalyzer.clone()
process.pFlowAnalysisBPTXPlusOrMinusOnlyNoTrk = process.pFlowNoiseAnalyzer.clone()

process.analysisNoSel = cms.Path(process.pFlowAnalysisNoSel)
process.analysisColl = cms.Path(process.l1CollBscOr+
                                process.offlineSelection+
                                process.pFlowAnalysisColl)
process.analysisNoColl = cms.Path(process.l1NoColl+
                                  process.pFlowAnalysisNoColl)
process.analysisNoCollNoVtx = cms.Path(process.l1NoColl+
                                       process.vertexVeto+ 
                                       process.pFlowAnalysisNoCollNoVtx)
process.analysisNoCollNoTrk = cms.Path(process.l1NoColl+
                                       process.trackVeto+
                                       process.pFlowAnalysisNoCollNoTrk)
process.analysisBPTXPlusOrMinusOnly = cms.Path(process.hltBPTXPlusOrMinusOnly+
                                               process.pFlowAnalysisBPTXPlusOrMinusOnly) 
process.analysisBPTXPlusOrMinusOnlyNoVtx = cms.Path(process.hltBPTXPlusOrMinusOnly+
                                                    process.vertexVeto+
                                                    process.pFlowAnalysisBPTXPlusOrMinusOnlyNoVtx)
process.analysisBPTXPlusOrMinusOnlyNoTrk = cms.Path(process.hltBPTXPlusOrMinusOnly+
                                                    process.trackVeto+
                                                    process.pFlowAnalysisBPTXPlusOrMinusOnlyNoTrk)
