import FWCore.ParameterSet.Config as cms

process = cms.Process("Analysis")

process.load('FWCore.MessageService.MessageLogger_cfi')
process.MessageLogger.cerr.threshold = 'INFO'

process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(3000) )

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring('file:/data1/antoniov/MinimumBias_Commissioning10_GOODCOLL-Jun14thSkim_v1_RECO/MinimumBias_Commissioning10_GOODCOLL-Jun14thSkim_v1_RECO_EC45524A-E682-DF11-B8A7-001A92810AAA.root')
)

process.TFileService = cms.Service("TFileService",
    fileName = cms.string("analysisHistos_caloActivityFilter.root")
)

process.load('Utilities.AnalysisSequences.minimumBiasAnalysisSequences_cff')
process.load('Utilities.AnalysisSequences.outputModule_cfi')
process.output.outputCommands.append('keep *_*_*_Analysis')
process.output.SelectEvents.SelectEvents = cms.vstring('selection_hcalVetoSumEMaxHFPlus4_step',
                                                       'selection_hcalVetoSumEMaxHFMinus4_step')
process.output.fileName = '/data1/antoniov/Test/caloActivityFilter.root'

process.load('ForwardAnalysis.Utilities.caloActivityFilter_cfi')
process.caloActivityFilter.EnergyThresholdHB = 1.25
process.caloActivityFilter.EnergyThresholdHE = 1.9
process.caloActivityFilter.EnergyThresholdHF = 4.0
process.caloActivityFilter.EnergyThresholdEB = 0.6
process.caloActivityFilter.EnergyThresholdEE = 2.45
process.hcalVetoSumEMaxHFPlus4 = process.caloActivityFilter.clone(SumEMaxHFPlus = 4.0)
process.hcalVetoSumEMaxHFMinus4 = process.caloActivityFilter.clone(SumEMaxHFMinus = 4.0)
process.hcalVetoSumETMaxHFPlus1 = process.caloActivityFilter.clone(SumETMaxHFPlus = 1.0)
process.hcalVetoSumETMaxHFMinus1 = process.caloActivityFilter.clone(SumETMaxHFMinus = 1.0)
process.hcalVetoSumEMaxHFPlus4SumEMaxHEPlus2 = process.caloActivityFilter.clone(SumEMaxHFPlus = 4.0, SumEMaxHEPlus = 2.0)
process.hcalVetoSumEMaxHFMinus4SumEMaxHEMinus2 = process.caloActivityFilter.clone(SumEMaxHFMinus = 4.0, SumEMaxHEMinus = 2.0)
process.hcalVetoSumETMaxHFPlus1SumETMaxHEPlus1 = process.caloActivityFilter.clone(SumETMaxHFPlus = 1.0, SumETMaxHEPlus = 1.0)
process.hcalVetoSumETMaxHFMinus1SumETMaxHEMinus1 = process.caloActivityFilter.clone(SumETMaxHFMinus = 1.0, SumETMaxHEMinus = 1.0)

process.load('Utilities.AnalysisTools.trackHistoAnalyzer_cfi')
process.trackHistoAnalyzer_hcalVetoSumEMaxHFPlus4 = process.trackHistoAnalyzer.clone()
process.trackHistoAnalyzer_hcalVetoSumEMaxHFMinus4 = process.trackHistoAnalyzer.clone()
process.trackHistoAnalyzer_hcalVetoSumETMaxHFPlus1 = process.trackHistoAnalyzer.clone()
process.trackHistoAnalyzer_hcalVetoSumETMaxHFMinus1 = process.trackHistoAnalyzer.clone()
process.trackHistoAnalyzer_hcalVetoSumEMaxHFPlus4SumEMaxHEPlus2 = process.trackHistoAnalyzer.clone()
process.trackHistoAnalyzer_hcalVetoSumEMaxHFMinus4SumEMaxHEMinus2 = process.trackHistoAnalyzer.clone()
process.trackHistoAnalyzer_hcalVetoSumETMaxHFPlus1SumETMaxHEPlus1 = process.trackHistoAnalyzer.clone()
process.trackHistoAnalyzer_hcalVetoSumETMaxHFMinus1SumETMaxHEMinus1 = process.trackHistoAnalyzer.clone()

process.selection_hcalVetoSumEMaxHFPlus4_step = cms.Path(process.hcalVetoSumEMaxHFPlus4)
process.selection_hcalVetoSumEMaxHFMinus4_step = cms.Path(process.hcalVetoSumEMaxHFMinus4)

process.analysis_hcalVetoSumEMaxHFPlus4_step = cms.Path(process.hcalVetoSumEMaxHFPlus4 + process.trackHistoAnalyzer_hcalVetoSumEMaxHFPlus4)
process.analysis_hcalVetoSumEMaxHFMinus4_step = cms.Path(process.hcalVetoSumEMaxHFMinus4 + process.trackHistoAnalyzer_hcalVetoSumEMaxHFMinus4)
process.analysis_hcalVetoSumETMaxHFPlus1_step = cms.Path(process.hcalVetoSumETMaxHFPlus1 + process.trackHistoAnalyzer_hcalVetoSumETMaxHFPlus1)
process.analysis_hcalVetoSumETMaxHFMinus1_step = cms.Path(process.hcalVetoSumETMaxHFMinus1 + process.trackHistoAnalyzer_hcalVetoSumETMaxHFMinus1)
process.analysis_hcalVetoSumEMaxHFPlus4SumEMaxHEPlus2_step = cms.Path(process.hcalVetoSumEMaxHFPlus4SumEMaxHEPlus2 + process.trackHistoAnalyzer_hcalVetoSumEMaxHFPlus4SumEMaxHEPlus2)
process.analysis_hcalVetoSumEMaxHFMinus4SumEMaxHEMinus2_step = cms.Path(process.hcalVetoSumEMaxHFMinus4SumEMaxHEMinus2 + process.trackHistoAnalyzer_hcalVetoSumEMaxHFMinus4SumEMaxHEMinus2)
process.analysis_hcalVetoSumETMaxHFPlus1SumETMaxHEPlus1_step = cms.Path(process.hcalVetoSumETMaxHFPlus1SumETMaxHEPlus1 + process.trackHistoAnalyzer_hcalVetoSumETMaxHFPlus1SumETMaxHEPlus1)
process.analysis_hcalVetoSumETMaxHFMinus1SumETMaxHEMinus1_step = cms.Path(process.hcalVetoSumETMaxHFMinus1SumETMaxHEMinus1 + process.trackHistoAnalyzer_hcalVetoSumETMaxHFMinus1SumETMaxHEMinus1)

process.out_step = cms.EndPath(process.output)
