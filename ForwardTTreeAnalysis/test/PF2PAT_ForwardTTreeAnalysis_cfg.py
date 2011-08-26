import FWCore.ParameterSet.Config as cms

# Settings
class config: pass
config.verbose = True
config.writeEdmOutput = False
config.runOnMC = False
config.globalTagNameData = 'GR_R_42_V19::All' #JECDATA 

config.globalTagNameMC = ''
#config.outputEdmFile = 'DijetsAnalysis.root'
config.outputTTreeFile = 'DijetsanalysisDATA42X_PATTTree.root'
config.comEnergy = 7000.0
config.trackAnalyzerName = 'trackHistoAnalyzer'
#config.trackTagName = 'selectGoodTracks'
config.trackTagName = 'analysisTracks'
#config.generator = 'Pythia6'
config.varyAttributes = True
config.runOfflineOnly = True
config.runNoColl = False
config.runBPTX = False
config.runHCALFilter = False
#config.UsePAT = True
#------------------------------------------------------------------------------
process = cms.Process("Analysis")

process.load('FWCore.MessageService.MessageLogger_cfi')
process.MessageLogger.cerr.threshold = 'INFO'

process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True),
SkipEvent = cms.untracked.vstring('ProductNotFound') )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
       
        'file:/storage2/sfonseca/CMSSW/Forward/CMSSW_4_2_8/src/ForwardAnalysis/Skimming/test/patTuple_PF2PAT.root'
    )
)
#-------------------------------------------------------------------------------
# Import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('Configuration.StandardSequences.GeometryExtended_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.Reconstruction_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.load('JetMETCorrections.Configuration.DefaultJEC_cff')
#process.load("CondCore.DBCommon.CondDBCommon_cfi")

process.options.wantSummary = True
#--------------------------------------------------------------------------------
if config.runOnMC: process.GlobalTag.globaltag = config.globalTagNameMC
else: process.GlobalTag.globaltag = config.globalTagNameData

#---------------------------------------------------------------------------------
##Jet Correction Service

process.ak5CaloL1Offset.useCondDB = False
process.ak5PFL1Offset.useCondDB = False
process.ak5JPTL1Offset.useCondDB = False

#---------------------------------------------------------------------------------
# Output
process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string(config.outputTTreeFile))

## if config.writeEdmOutput:
##     process.load("ForwardAnalysis.ForwardTTreeAnalysis.outputModule_cfi")
##     from ForwardAnalysis.ForwardTTreeAnalysis.EventContent_cff import ForwardTTreeAnalysisEventContent
##     process.output.outputCommands = ForwardTTreeAnalysisEventContent.outputCommands
##     process.output.fileName = config.outputEdmFile
##     process.output.SelectEvents.SelectEvents = cms.vstring('selection_step')

## process.TFileService = cms.Service("TFileService",
##                                    fileName = cms.string(config.outputTTreeFile)
## )

###################################################################
# Analysis modules
#--------------------------------
from Utilities.AnalysisTools.countsAnalyzer_cfi import countsAnalyzer
 
process.load("ForwardAnalysis.ForwardTTreeAnalysis.exclusiveDijetsAnalysisSequences_cff")
#process.load("ForwardAnalysis.ForwardTTreeAnalysis.singleVertexFilter_cfi")
process.load('ForwardAnalysis.ForwardTTreeAnalysis.exclusiveDijetsTTreeAnalysis_cfi')
#process.exclusiveDijetsTTreeAnalysis.TriggerResultsTag = cms.InputTag("TriggerResults::HLT")
process.exclusiveDijetsTTreeAnalysis.EBeam = config.comEnergy/2.
process.exclusiveDijetsTTreeAnalysis.TrackTag = config.trackTagName
process.exclusiveDijetsTTreeAnalysis.ParticleFlowTag = "pfCandidateNoiseThresholds"
#process.exclusiveDijetsTTreeAnalysisPFNoiseThresholds = process.exclusiveDijetsTTreeAnalysis.clone(ParticleFlowTag = "pfCandidateNoiseThresholds")

process.analysis_reco_step = cms.Path(process.analysisSequences)
process.analysis_exclusiveDijetsAnalysis_step = cms.Path(process.eventSelectionHLT+
                                                         process.exclusiveDijetsTTreeAnalysis)
