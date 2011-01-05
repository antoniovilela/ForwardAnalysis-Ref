import FWCore.ParameterSet.Config as cms

process = cms.Process("Analysis")

process.load("FWCore.MessageLogger.MessageLogger_cfi")

process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        'file:/data1/antoniov/MinimumBias_Commissioning10_GOODCOLL-Jun14thSkim_v1_RECO/MinimumBias_Commissioning10_GOODCOLL-Jun14thSkim_v1_RECO_EC45524A-E682-DF11-B8A7-001A92810AAA.root'
    )
)

process.TFileService = cms.Service("TFileService",
    fileName = cms.string('analysisHistos_pFlowNoiseThesholds.root')
)

process.load('Utilities.AnalysisSequences.minimumBiasAnalysisSequences_cff')
process.load('Utilities.AnalysisSequences.outputModule_cfi')
process.output.outputCommands.append('keep *_pfCandidateNoiseThresholds_*_*')
process.output.outputCommands.append('keep *_edmNtupleEtaMax_*_*')
process.output.outputCommands.append('keep *_edmNtupleEtaMin_*_*')
process.output.outputCommands.append('keep *_edmNtupleEtaMaxNoThresholds_*_*')
process.output.outputCommands.append('keep *_edmNtupleEtaMinNoThresholds_*_*')
process.output.SelectEvents.SelectEvents = cms.vstring('selection_step')
process.output.fileName = '/data1/antoniov/Test/pFlowNoiseThesholds.root'

process.load('ForwardAnalysis.Utilities.pfCandidateNoiseThresholds_cfi')
process.load('ForwardAnalysis.Utilities.etaMaxCandViewSelector_cfi')
process.load('ForwardAnalysis.Utilities.etaMinCandViewSelector_cfi')
#process.load('ForwardAnalysis.Utilities.etaMaxCandSelector_cfi')
#process.load('ForwardAnalysis.Utilities.etaMinCandSelector_cfi')
process.etaMaxCandViewSelector.src = "pfCandidateNoiseThresholds"
process.etaMinCandViewSelector.src = "pfCandidateNoiseThresholds"
#process.etaMaxCandSelector.src = "pfCandidateNoiseThresholds"
#process.etaMinCandSelector.src = "pfCandidateNoiseThresholds"
process.etaMaxCandViewSelectorNoThresholds = process.etaMaxCandViewSelector.clone(src = "particleFlow")
process.etaMinCandViewSelectorNoThresholds = process.etaMinCandViewSelector.clone(src = "particleFlow")

###########
etaHistogram = cms.PSet(
    min = cms.untracked.double(-5.0),
    max = cms.untracked.double(5.0),
    nbins = cms.untracked.int32(100),
    name  = cms.untracked.string('Eta'),
    description  = cms.untracked.string('eta'),
    plotquantity = cms.untracked.string('eta')
)
etaMaxHistogram = etaHistogram.clone(name = 'EtaMax', description = 'eta max')
etaMinHistogram = etaHistogram.clone(name = 'EtaMin', description = 'eta min')
process.etaHistoAnalyzer = cms.EDAnalyzer('CandViewHistoAnalyzer',
    src = cms.InputTag('pfCandidateNoiseThresholds'),
    histograms = cms.VPSet(etaHistogram)
)
process.etaMaxHistoAnalyzer = cms.EDAnalyzer('CandViewHistoAnalyzer',
    src = cms.InputTag('etaMaxCandViewSelector'),
    histograms = cms.VPSet(etaMaxHistogram)
)
process.etaMinHistoAnalyzer = cms.EDAnalyzer('CandViewHistoAnalyzer',
    src = cms.InputTag('etaMinCandViewSelector'),
    histograms = cms.VPSet(etaMinHistogram)
)
process.etaHistoAnalyzerNoThresholds = process.etaHistoAnalyzer.clone(src = "particleFlow")
process.etaMaxHistoAnalyzerNoThresholds = process.etaMaxHistoAnalyzer.clone(src = "etaMaxCandViewSelectorNoThresholds")
process.etaMinHistoAnalyzerNoThresholds = process.etaMinHistoAnalyzer.clone(src = "etaMinCandViewSelectorNoThresholds")

###########
process.edmNtupleEtaMax = cms.EDProducer("CandViewNtpProducer", 
    src = cms.InputTag("etaMaxCandViewSelector"),
    lazyParser = cms.untracked.bool(True),
    prefix = cms.untracked.string(""),
    eventInfo = cms.untracked.bool(True),
    variables = cms.VPSet(
        cms.PSet(
            tag = cms.untracked.string("etaMax"),
            quantity = cms.untracked.string("eta")
        )
    )  
)
process.edmNtupleEtaMin = cms.EDProducer("CandViewNtpProducer", 
    src = cms.InputTag("etaMinCandViewSelector"),
    lazyParser = cms.untracked.bool(True),
    prefix = cms.untracked.string(""),
    eventInfo = cms.untracked.bool(True),
    variables = cms.VPSet(
        cms.PSet(
            tag = cms.untracked.string("etaMin"),
            quantity = cms.untracked.string("eta")
        )
    )
)
process.edmNtupleEtaMaxNoThresholds = process.edmNtupleEtaMax.clone(src = "etaMaxCandViewSelectorNoThresholds")
process.edmNtupleEtaMinNoThresholds = process.edmNtupleEtaMin.clone(src = "etaMinCandViewSelectorNoThresholds") 

###########
process.selection_step = cms.Path(process.eventSelectionBscMinBiasOR)
process.reco_step = cms.Path(process.etaMaxCandViewSelectorNoThresholds+
                             process.etaMinCandViewSelectorNoThresholds+
                             process.pfCandidateNoiseThresholds*
                             process.etaMaxCandViewSelector+process.etaMinCandViewSelector)
process.edmDump_step = cms.Path(process.edmNtupleEtaMax+
                                process.edmNtupleEtaMin+
                                process.edmNtupleEtaMaxNoThresholds+
                                process.edmNtupleEtaMinNoThresholds) 
                            
process.analysis_step = cms.Path(process.eventSelectionBscMinBiasOR+
                                 process.etaHistoAnalyzer+
                                 process.etaMaxHistoAnalyzer+process.etaMinHistoAnalyzer+
                                 process.etaHistoAnalyzerNoThresholds+
                                 process.etaMaxHistoAnalyzerNoThresholds+
                                 process.etaMinHistoAnalyzerNoThresholds)
#process.reco_step = cms.Path(process.pfCandidateNoiseThresholds)
#process.reco_step = cms.Path(process.etaMaxCandViewSelector+process.etaMinCandViewSelector)
process.out_step = cms.EndPath(process.output)
