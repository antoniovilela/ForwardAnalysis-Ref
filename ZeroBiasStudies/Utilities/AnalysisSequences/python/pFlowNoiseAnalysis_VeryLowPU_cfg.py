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
process.hltL1Tech54ZeroBias = process.hltFilter.clone(
    HLTPaths = ['HLT_L1Tech54_ZeroBias_v*']
)

from ForwardAnalysis.Utilities.pfCandidateSelector_cfi import pfCandidateSelector
process.pfCandidateNoiseThresholds = pfCandidateSelector.clone()
from ForwardAnalysis.Utilities.ExcludeHFEdgesStringCut import ExcludeHFEdgesStringCut
from ForwardAnalysis.Utilities.PFCandidateNoiseStringCut import PFCandidateNoiseStringCut
# Change thresholds here if needed
from ForwardAnalysis.AnalysisSequences.pfThresholds_cfi import pfThresholds
pfStrCut1 = ExcludeHFEdgesStringCut().cut()
pfStrCut2 = PFCandidateNoiseStringCut(pfThresholds).cut()
# Only noise thresholds
pfStrCut = pfStrCut2
#pfStrCut = '%s & %s' % (pfStrCut1,pfStrCut2)
process.pfCandidateNoiseThresholds.cut = pfStrCut
# Set particle-flow collection
process.pfCandidateNoiseThresholds.src = "particleFlow" 
process.pfCandidates = cms.Sequence( process.pfCandidateNoiseThresholds ) 
process.analysisSequences = cms.Sequence( process.pfCandidates )

process.load('Utilities.AnalysisSequences.primaryVertexFilter_cfi')
process.load('Utilities.AnalysisSequences.trackCountFilter_cfi')
process.generalTracksFilter = process.trackCountFilter.clone(src = 'generalTracks')
#process.pixelLessTracksFilter = process.trackCountFilter.clone(src = 'ctfPixelLess')
process.pixelTracksFilter = process.trackCountFilter.clone(src = 'pixelTracks')

process.vertexVeto = cms.Sequence(~process.primaryVertexFilter)
process.trackVeto = cms.Sequence(~process.generalTracksFilter + ~process.pixelTracksFilter) 
#############

process.load('Utilities.AnalysisTools.pFlowNoiseAnalyzer_cfi')
process.pFlowNoiseAnalyzer.particleFlowTag = "particleFlow"
process.pFlowNoiseAnalyzer.applyHFEnergyCorrection = False
#process.pFlowNoiseAnalyzer.applyHFEnergyCorrection = True

process.pFlowAnalysisNoSel = process.pFlowNoiseAnalyzer.clone()
process.pFlowAnalysisColl = process.pFlowNoiseAnalyzer.clone()
process.pFlowAnalysisNoColl = process.pFlowNoiseAnalyzer.clone()
process.pFlowAnalysisNoCollNoVtx = process.pFlowNoiseAnalyzer.clone()
process.pFlowAnalysisNoCollNoTrk = process.pFlowNoiseAnalyzer.clone()
#process.pFlowAnalysisBPTXPlusOrMinusOnly = process.pFlowNoiseAnalyzer.clone()
#process.pFlowAnalysisBPTXPlusOrMinusOnlyNoVtx = process.pFlowNoiseAnalyzer.clone()
#process.pFlowAnalysisBPTXPlusOrMinusOnlyNoTrk = process.pFlowNoiseAnalyzer.clone()
process.pFlowAnalysisL1Tech54ZeroBiasColl = process.pFlowNoiseAnalyzer.clone()
process.pFlowAnalysisL1Tech54ZeroBias = process.pFlowNoiseAnalyzer.clone()
process.pFlowAnalysisL1Tech54ZeroBiasNoVtx = process.pFlowNoiseAnalyzer.clone()
process.pFlowAnalysisL1Tech54ZeroBiasNoTrk = process.pFlowNoiseAnalyzer.clone()

process.pFlowAnalysisNoiseThresholdsColl = process.pFlowNoiseAnalyzer.clone()
process.pFlowAnalysisNoiseThresholdsColl.particleFlowTag = "pfCandidateNoiseThresholds"
process.pFlowAnalysisNoiseThresholdsL1Tech54ZeroBiasColl = process.pFlowNoiseAnalyzer.clone()
process.pFlowAnalysisNoiseThresholdsL1Tech54ZeroBiasColl.particleFlowTag = "pfCandidateNoiseThresholds"

#############
process.analysis_reco_step = cms.Path(process.analysisSequences)
process.analysisNoSel = cms.Path(process.pFlowAnalysisNoSel)
process.analysisColl = cms.Path(process.l1CollBscOr+
                                process.offlineSelection+
                                process.pFlowAnalysisColl)
process.analysisNoiseThresholdsColl = cms.Path(process.l1CollBscOr+
                                               process.offlineSelection+
                                               process.pFlowAnalysisNoiseThresholdsColl)
process.analysisNoColl = cms.Path(process.l1NoColl+
                                  process.pFlowAnalysisNoColl)
process.analysisNoCollNoVtx = cms.Path(process.l1NoColl+
                                       process.vertexVeto+ 
                                       process.pFlowAnalysisNoCollNoVtx)
process.analysisNoCollNoTrk = cms.Path(process.l1NoColl+
                                       process.trackVeto+
                                       process.pFlowAnalysisNoCollNoTrk)
#process.analysisBPTXPlusOrMinusOnly = cms.Path(process.hltBPTXPlusOrMinusOnly+
#                                               process.pFlowAnalysisBPTXPlusOrMinusOnly) 
#process.analysisBPTXPlusOrMinusOnlyNoVtx = cms.Path(process.hltBPTXPlusOrMinusOnly+
#                                                    process.vertexVeto+
#                                                    process.pFlowAnalysisBPTXPlusOrMinusOnlyNoVtx)
#process.analysisBPTXPlusOrMinusOnlyNoTrk = cms.Path(process.hltBPTXPlusOrMinusOnly+
#                                                    process.trackVeto+
#                                                    process.pFlowAnalysisBPTXPlusOrMinusOnlyNoTrk)
process.analysisL1Tech54ZeroBiasColl = cms.Path(process.hltL1Tech54ZeroBias+
                                                process.offlineSelection+
                                                process.pFlowAnalysisL1Tech54ZeroBiasColl)
process.analysisNoiseThresholdsL1Tech54ZeroBiasColl = cms.Path(process.hltL1Tech54ZeroBias+
                                                               process.offlineSelection+
                                                               process.pFlowAnalysisNoiseThresholdsL1Tech54ZeroBiasColl)
process.analysisL1Tech54ZeroBias = cms.Path(process.hltL1Tech54ZeroBias+
                                            process.pFlowAnalysisL1Tech54ZeroBias) 
process.analysisL1Tech54ZeroBiasNoVtx = cms.Path(process.hltL1Tech54ZeroBias+
                                                 process.vertexVeto+
                                                 process.pFlowAnalysisL1Tech54ZeroBiasNoVtx)
process.analysisL1Tech54ZeroBiasNoTrk = cms.Path(process.hltL1Tech54ZeroBias+
                                                 process.trackVeto+
                                                 process.pFlowAnalysisL1Tech54ZeroBiasNoTrk)
