import FWCore.ParameterSet.Config as cms

process = cms.Process("Analysis")

process.load('Utilities.AnalysisSequences.defaultSequences_cff')
process.TFileService.fileName = 'analysisHistos_pFlowNoiseAnalysis.root'

#############
process.load('Utilities.AnalysisSequences.jetAnalysisSequences_cff')

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

#############
process.load('Utilities.AnalysisTools.pFlowNoiseAnalyzer_cfi')
process.pFlowNoiseAnalyzer.particleFlowTag = "particleFlow"
process.pFlowNoiseAnalyzer.applyHFEnergyCorrection = False

process.pFlowAnalysisNoSel = process.pFlowNoiseAnalyzer.clone()
process.pFlowAnalysisJetPt30 = process.pFlowNoiseAnalyzer.clone()
process.pFlowAnalysisJetPt60 = process.pFlowNoiseAnalyzer.clone()

process.pFlowAnalysisNoiseThresholdsJetPt30 = process.pFlowNoiseAnalyzer.clone()
process.pFlowAnalysisNoiseThresholdsJetPt30.particleFlowTag = "pfCandidateNoiseThresholds"
process.pFlowAnalysisNoiseThresholdsJetPt60 = process.pFlowNoiseAnalyzer.clone()
process.pFlowAnalysisNoiseThresholdsJetPt60.particleFlowTag = "pfCandidateNoiseThresholds"

#######
process.analysis_reco_step = cms.Path(process.analysisSequences)
process.analysisNoSel = cms.Path(process.pFlowAnalysisNoSel)
process.analysisJetPt30 = cms.Path(process.eventSelectionHLTJet15UPt30+process.pFlowAnalysisJetPt30)
process.analysisJetPt60 = cms.Path(process.eventSelectionHLTJet30UPt60+process.pFlowAnalysisJetPt60)

process.analysisNoiseThresholdsJetPt30 = cms.Path(process.eventSelectionHLTJet15UPt30+
                                                  process.pFlowAnalysisNoiseThresholdsJetPt30)
process.analysisNoiseThresholdsJetPt60 = cms.Path(process.eventSelectionHLTJet30UPt60+
                                                  process.pFlowAnalysisNoiseThresholdsJetPt60)
