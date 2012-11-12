import FWCore.ParameterSet.Config as cms

process = cms.Process("Analysis")

process.load('Utilities.AnalysisSequences.defaultSequences_cff')
process.TFileService.fileName = 'analysisHistos_pFlowNoiseAnalysis.root'

#############
process.load('Utilities.AnalysisSequences.minimumBiasAnalysisSequences_cff')

# Applying custom PFlow thresholds
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
#process.pFlowNoiseAnalyzer.applyHFDeadMaterialCorrection = False
process.pFlowNoiseAnalyzer.applyHFDeadMaterialCorrection = True

process.load('ForwardAnalysis.Utilities.pileUpInfoFilter_cfi')
process.pileUpInfoFilter.pileUpSummaryInfoTag = "addPileupInfo"
process.pileUpInfoFilter.numberOfPileUpEventsBx0 = cms.int32(0)

process.pFlowAnalysisNoSel = process.pFlowNoiseAnalyzer.clone()
process.pFlowAnalysisColl = process.pFlowNoiseAnalyzer.clone()
process.pFlowAnalysisNoVtx = process.pFlowNoiseAnalyzer.clone()
process.pFlowAnalysisNoTrk = process.pFlowNoiseAnalyzer.clone()

process.pFlowAnalysisNoiseThresholdsColl = process.pFlowNoiseAnalyzer.clone()
process.pFlowAnalysisNoiseThresholdsColl.particleFlowTag = "pfCandidateNoiseThresholds"

process.pFlowAnalysisCollPU0 = process.pFlowNoiseAnalyzer.clone()
process.pFlowAnalysisNoiseThresholdsCollPU0 = process.pFlowNoiseAnalyzer.clone()
process.pFlowAnalysisNoiseThresholdsCollPU0.particleFlowTag = "pfCandidateNoiseThresholds"
#######
process.analysis_reco_step = cms.Path(process.analysisSequences)

process.analysisNoSel = cms.Path(process.pFlowAnalysisNoSel)
process.analysisColl = cms.Path(process.offlineSelection + process.pFlowAnalysisColl)
process.analysisNoiseThresholdsColl = cms.Path(process.offlineSelection +
                                               process.pFlowAnalysisNoiseThresholdsColl)

process.analysisCollPU0 = cms.Path(process.pileUpInfoFilter + process.offlineSelection +
                                   process.pFlowAnalysisCollPU0)
process.analysisNoiseThresholdsCollPU0 = cms.Path(process.pileUpInfoFilter + process.offlineSelection +
                                                  process.pFlowAnalysisNoiseThresholdsCollPU0)
#######
process.analysisNoVtx = cms.Path(process.vertexVeto + process.pFlowAnalysisNoVtx)
process.analysisNoTrk = cms.Path(process.trackVeto + process.pFlowAnalysisNoTrk)
