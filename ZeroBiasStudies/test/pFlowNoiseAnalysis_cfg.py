import FWCore.ParameterSet.Config as cms

process = cms.Process("Analysis")
##


process.load("FWCore.MessageLogger.MessageLogger_cfi")

process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(100) )

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
         'file:/storage1/eliza/samples_test/MinimumBias_ZeorBias.root'
#        'file:/data1/antoniov/MinimumBias_Commissioning10_GOODCOLL-Jun14thSkim_v1_RECO/MinimumBias_Commissioning10_GOODCOLL-Jun14thSkim_v1_RECO_EC45524A-E682-DF11-B8A7-001A92810AAA.root'
    )
)

process.TFileService = cms.Service("TFileService",
    fileName = cms.string('analysisHistos_pFlowNoiseThesholds.root')
)
##
#process.load('Utilities.AnalysisSequences.defaultSequences_cff')
#process.TFileService.fileName = 'analysisHistos_pFlowNoiseAnalysis.root'

#############
process.load('Utilities.AnalysisSequences.minimumBiasAnalysisSequences_cff')
process.multipleVertexFilter.src = 'primaryVertexFilter'

process.load('Utilities.AnalysisSequences.hltFilter_cfi')
process.hltBPTXPlusOrMinusOnly = process.hltFilter.clone(
    HLTPaths = ['HLT_L1_BPTX_PlusOnly','HLT_L1_BPTX_MinusOnly']
)
process.hltBPTXPlusAndMinusOnly = process.hltFilter.clone(
    #andOr = cms.bool(False),
    HLTPaths = ['HLT_ZeroBias']
)



# Applying custom PFlow thresholds
from ForwardAnalysis.Utilities.pfCandidateSelector_cfi import pfCandidateSelector
process.pfCandidateNoiseThresholds = pfCandidateSelector.clone()
from ForwardAnalysis.Utilities.ExcludeHFEdgesStringCut import ExcludeHFEdgesStringCut
from ForwardAnalysis.Utilities.PFCandidateNoiseStringCut import PFCandidateNoiseStringCut
# Change thresholds here if needed
from ForwardAnalysis.AnalysisSequences.pfThresholds_cfi import pfThresholds
pfThresholdsHF0 = pfThresholds.clone()
pfThresholdsHF0.Transition.hadronHF.energy = 0.0
pfThresholdsHF0.Transition.emHF.energy = 0.0
pfThresholdsHF0.Forward.hadronHF.energy = 0.0
pfThresholdsHF0.Forward.emHF.energy = 0.0
pfStrCut1 = ExcludeHFEdgesStringCut().cut()
pfStrCut2 = PFCandidateNoiseStringCut(pfThresholdsHF0).cut()
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

process.pFlowAnalysisNoSel = process.pFlowNoiseAnalyzer.clone()
process.pFlowAnalysisColl = process.pFlowNoiseAnalyzer.clone()
process.pFlowAnalysisCollOffline = process.pFlowNoiseAnalyzer.clone()
process.pFlowAnalysisNoColl = process.pFlowNoiseAnalyzer.clone()
process.pFlowAnalysisNoCollNoVtx = process.pFlowNoiseAnalyzer.clone()
process.pFlowAnalysisNoCollNoTrk = process.pFlowNoiseAnalyzer.clone()
process.pFlowAnalysisBPTXPlusOrMinusOnly = process.pFlowNoiseAnalyzer.clone()
process.pFlowAnalysisBPTXPlusOrMinusOnlyNoVtx = process.pFlowNoiseAnalyzer.clone()
process.pFlowAnalysisBPTXPlusOrMinusOnlyNoTrk = process.pFlowNoiseAnalyzer.clone()
####and
process.pFlowAnalysisBPTXPlusAndMinusOnly = process.pFlowNoiseAnalyzer.clone()
process.pFlowAnalysisBPTXPlusAndMinusOnlyNoVtx = process.pFlowNoiseAnalyzer.clone()
process.pFlowAnalysisBPTXPlusAndMinusOnlyNoTrk = process.pFlowNoiseAnalyzer.clone()


process.pFlowAnalysisNoiseThresholdsColl = process.pFlowNoiseAnalyzer.clone()
process.pFlowAnalysisNoiseThresholdsColl.particleFlowTag = "pfCandidateNoiseThresholds"
process.pFlowAnalysisNoiseThresholdsCollOffline = process.pFlowNoiseAnalyzer.clone()
process.pFlowAnalysisNoiseThresholdsCollOffline.particleFlowTag = "pfCandidateNoiseThresholds"

#######
process.analysis_reco_step = cms.Path(process.analysisSequences)
process.analysisNoSel = cms.Path(process.pFlowAnalysisNoSel)
process.analysisColl = cms.Path(process.l1CollBscOr+
                                process.offlineSelection+
                                process.pFlowAnalysisColl)
process.analysisCollZeroBias = cms.Path(process.l1CollBscOr + process.hltBPTXPlusAndMinusOnly +
                                process.offlineSelection+
                                process.pFlowAnalysisColl)
process.analysisCollOffline = cms.Path(process.offlineSelection+
                                       process.pFlowAnalysisCollOffline)
process.analysisNoiseThresholdsColl = cms.Path(process.l1CollBscOr+
                                               process.offlineSelection+
                                               process.pFlowAnalysisNoiseThresholdsColl)
process.analysisNoiseThresholdsZeroBiasColl = cms.Path(process.l1CollBscOr+process.hltBPTXPlusAndMinusOnly+
                                               process.offlineSelection+
                                               process.pFlowAnalysisNoiseThresholdsColl)
process.analysisNoiseThresholdsCollOffline = cms.Path(process.offlineSelection+
                                                     process.pFlowAnalysisNoiseThresholdsCollOffline)

process.analysisNoiseThresholdsCollZeroBiasOffline = cms.Path(process.hltBPTXPlusAndMinusOnly + process.offlineSelection+
                                                     process.pFlowAnalysisNoiseThresholdsCollOffline)
#######
process.analysisNoColl = cms.Path(process.l1NoColl+
                                  process.pFlowAnalysisNoColl)
process.analysisNoCollNoVtx = cms.Path(process.l1NoColl+
                                       process.vertexVeto+ 
                                       process.pFlowAnalysisNoCollNoVtx)
process.analysisNoCollNoTrk = cms.Path(process.l1NoColl+
                                       process.trackVeto+
                                       process.pFlowAnalysisNoCollNoTrk)
process.analysisNoCollZeroBiasNoTrk = cms.Path(process.l1NoColl + process.hltBPTXPlusAndMinusOnly + 
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
##and
process.analysisBPTXPlusAndMinusOnly = cms.Path(process.hltBPTXPlusAndMinusOnly+
                                               process.pFlowAnalysisBPTXPlusAndMinusOnly) 
process.analysisBPTXPlusAndMinusOnlyNoVtx = cms.Path(process.hltBPTXPlusAndMinusOnly+
                                                    process.vertexVeto+
                                                    process.pFlowAnalysisBPTXPlusAndMinusOnlyNoVtx)
process.analysisBPTXPlusAndMinusOnlyNoTrk = cms.Path(process.hltBPTXPlusAndMinusOnly+
                                                    process.trackVeto+
                                                    process.pFlowAnalysisBPTXPlusAndMinusOnlyNoTrk)
