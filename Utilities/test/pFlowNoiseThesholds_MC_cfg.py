import FWCore.ParameterSet.Config as cms

from pFlowNoiseThesholds_cfg import process
process.source.fileNames = ['file:/data1/antoniov/MinBias_TuneD6T_7TeV-pythia6_START36_V10_SP10-v1_GEN-SIM-RECODEBUG/MinBias_TuneD6T_7TeV-pythia6_START36_V10_SP10-v1_GEN-SIM-RECODEBUG_F63DF090-6879-DF11-9E7D-0030487CDA68.root']
process.output.fileName = '/data1/antoniov/Test/pFlowNoiseThesholds_MC.root'
process.output.outputCommands.append('keep *_edmNtupleEtaMaxGen_*_*')
process.output.outputCommands.append('keep *_edmNtupleEtaMinGen_*_*')
process.TFileService.fileName = 'analysisHistos_pFlowNoiseThesholds_MC.root'

process.load('Utilities.AnalysisSequences.genStableParticles_cfi')
process.genStableParticles.cut = 'status = 1 & ( ( pdgId != 2212 ) | ( pdgId == 2212 & abs(pz) < %f ) )' % (0.75*7000)
process.etaMaxGenCandViewSelector = process.etaMaxCandViewSelector.clone(src = "genStableParticles")
process.etaMinGenCandViewSelector = process.etaMinCandViewSelector.clone(src = "genStableParticles")

process.etaGenHistoAnalyzer = process.etaHistoAnalyzer.clone(src = "genStableParticles")
process.etaMaxGenHistoAnalyzer = process.etaMaxHistoAnalyzer.clone(src = "etaMaxGenCandViewSelector")
process.etaMinGenHistoAnalyzer = process.etaMinHistoAnalyzer.clone(src = "etaMinGenCandViewSelector")

process.edmNtupleEtaMaxGen = process.edmNtupleEtaMax.clone(src = "etaMaxGenCandViewSelector")
process.edmNtupleEtaMinGen = process.edmNtupleEtaMin.clone(src = "etaMinGenCandViewSelector")

process.recoMC_step = cms.Path(process.genStableParticles*
                               process.etaMaxGenCandViewSelector+
                               process.etaMinGenCandViewSelector)

process.edmDumpMC_step = cms.Path(process.edmNtupleEtaMaxGen+
                                  process.edmNtupleEtaMinGen)

process.analysisMC_step = cms.Path(process.eventSelectionBscMinBiasOR+
                                   process.etaGenHistoAnalyzer+
                                   process.etaMaxGenHistoAnalyzer+process.etaMinGenHistoAnalyzer)

from Utilities.PyConfigTools.removeFromPaths import removeFromPaths
removeFromPaths(process,'bptx')
removeFromPaths(process,'hltBscMinBiasORBptxPlusORMinusFilter')
