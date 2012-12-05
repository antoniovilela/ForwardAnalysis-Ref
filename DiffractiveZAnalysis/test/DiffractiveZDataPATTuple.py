import FWCore.ParameterSet.Config as cms

#
# Program Settings
#

class config: pass
config.verbose = True
config.writeEdmOutput = False
config.outputTTreeFile = 'DiffractiveZDataPATTuple.root'
config.runOnMC = True
config.runPATSequences = True
config.usePAT = False
config.globalTagNameData = 'GR_R_42_V23::All' 
config.instLumiROOTFile=''
config.globalTagNameMC = 'START42_V17D::All'
config.comEnergy = 7000.0
config.trackAnalyzerName = 'trackHistoAnalyzer'
config.trackTagName = 'analysisTracks'
config.NumberOfEvents = 1000
config.TriggerOn = False


#
# Define Triggers and Input Files
#

if config.runOnMC:
    config.l1Paths = ('L1_ZeroBias','L1_BptxMinus_NotBptxPlus','L1_SingleJet30U')
    config.hltPaths =('HLT_Jet30_v1','HLT_Jet30_v2','HLT_Jet30_v3','HLT_Jet30_v4','HLT_Jet30_v5','HLT_Jet30_v6')
    config.inputFileName = '/storage1/dmf/PrivateMCProduction/July2012Prod/Pythia/CMSSW_4_2_8_lowpupatch1/src/QCD_Pt_15to3000_TuneZ2_Flat_7TeV_pythia6_cff_py_RAW2DIGI_L1Reco_RECO_inRECOSIM.root'
    config.runPUMC = False  # MC With PU
    config.runGen = True    # MC With Weight

else:
    config.l1Paths = ('L1_SingleJet36','L1_SingleJet16','L1_DoubleJetC56')
    config.hltPaths = ('HLT_Mu0_L1MuOpen','HLT_Mu3','HLT_Mu5','HLT_DoubleMu0','HLT_Jet15U')
#    config.inputFileName = '/storage2/antoniov/data1/MultiJet_Run2010B_Apr21ReReco-v1_AOD/MultiJet_Run2010B_Apr21ReReco-v1_AOD_7EA7B611-7371-E011-B164-002354EF3BDB.root'
    config.inputFileName = '/storage1/dmf/TestSamples/MuRun2010/MuRunA2010.root'
    config.runPUMC = False
    config.runGen = False

#
# CMSSW Main Code
#


process = cms.Process("Analysis")

process.load('FWCore.MessageService.MessageLogger_cfi')

process.options = cms.untracked.PSet(
	wantSummary = cms.untracked.bool(False),
	SkipEvent = cms.untracked.vstring('ProductNotFound')
	)

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(config.NumberOfEvents) )

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring( 'file:%s' % config.inputFileName )
)


#
# Output
#

process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string(config.outputTTreeFile))

#
# Detector Conditions and Scales
#

process.load('Configuration.StandardSequences.Services_cff')
process.load('Configuration.StandardSequences.GeometryExtended_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.Reconstruction_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

#
# Global Tag Input
#

if config.runOnMC: process.GlobalTag.globaltag = config.globalTagNameMC
else: process.GlobalTag.globaltag = config.globalTagNameData

process.load('JetMETCorrections.Configuration.DefaultJEC_cff')
process.load('RecoJets.Configuration.RecoPFJets_cff')
process.load('RecoJets.Configuration.RecoJets_cff')
process.load('CommonTools/RecoAlgos/HBHENoiseFilterResultProducer_cfi')


#
# PAT Sequences
#

if config.runPATSequences:
    from ForwardAnalysis.Skimming.addPATSequences import addPATSequences
    addPATSequences(process,config.runOnMC)

    if config.runOnMC:
	process.patTrigger.addL1Algos = cms.bool( False )
	process.patJets.addTagInfos   = cms.bool( False )
    else:
	process.patTrigger.addL1Algos = cms.bool( True )
	process.patJets.addTagInfos   = cms.bool( True )   

from ForwardAnalysis.Utilities.addCastorRecHitCorrector import addCastorRecHitCorrector
addCastorRecHitCorrector(process)

#
# Open Common Modules
#

process.load("ForwardAnalysis.AnalysisSequences.CommonModulesSequences_cff")
process.pfCandidateNoiseThresholds.src = "pfNoPileUpPFlow"
process.tracksTransverseRegion.JetTag = "selectedPatJetsPFlow"


#
# Open Analysis Modules
#

#
# Import PSET for each Module
#

from ForwardAnalysis.ForwardTTreeAnalysis.DiffractiveAnalysis_cfi import DiffractiveAnalysis
from ForwardAnalysis.DiffractiveZAnalysis.DiffractiveZAnalysis_cfi import DiffractiveZAnalysis
from ForwardAnalysis.ForwardTTreeAnalysis.PATTriggerInfo_cfi import PATTriggerInfo
from ForwardAnalysis.ForwardTTreeAnalysis.DijetsTriggerAnalysis_cfi import DijetsTriggerAnalysis

# PATInfo Without Wildcard * (FIX ME)
#PATTriggerInfo.L1AlgoBitName =  config.l1Paths 
PATTriggerInfo.HLTAlgoBitName = config.hltPaths
PATTriggerInfo.runALLTriggerPath = True

#
# Define Analyzers
#

process.diffractiveZAnalysisTTree = cms.EDAnalyzer("EventInfoDiffractiveDiffractiveZAnalysisTTree",
        EventInfo = cms.PSet(
                    RunOnData = cms.untracked.bool(not config.runOnMC),
                    RunWithMCPU = cms.untracked.bool(config.runPUMC),
                    RunWithGen = cms.untracked.bool(config.runGen)
        ),
        DiffractiveAnalysis = DiffractiveAnalysis,
        DiffractiveZAnalysis = DiffractiveZAnalysis
        )

process.CommonHLTFilter.HLTPaths = config.hltPaths 

process.diffractiveZAnalysisTTree.DiffractiveAnalysis.hltPath = ''
process.diffractiveZAnalysisTTree.DiffractiveAnalysis.trackTag = 'analysisTracks'
process.diffractiveZAnalysisTTree.DiffractiveAnalysis.vertexTag = "offlinePrimaryVertices"
process.diffractiveZAnalysisTTree.DiffractiveAnalysis.particleFlowTag = "pfCandidateNoiseThresholds"
process.diffractiveZAnalysisTTree.DiffractiveAnalysis.jetTag = "selectedPatJetsPFlow"

process.diffractiveZAnalysisTTree.DiffractiveZAnalysis.hltPaths = config.hltPaths
if config.runOnMC:
     process.diffractiveZAnalysisTTree.DiffractiveZAnalysis.RunMC = True
else:
     process.diffractiveZAnalysisTTree.DiffractiveZAnalysis.RunMC = False 

#
# Define MC Access
#

if config.runOnMC:
     process.diffractiveZAnalysisTTree.DiffractiveAnalysis.accessMCInfo = True
     process.gen_step = cms.Path(process.genChargedParticles+
                              process.genProtonDissociative*process.edmNtupleMxGen+
                              process.genStableParticles*
                              process.etaMaxGen+process.etaMinGen*
                              process.edmNtupleEtaMaxGen+process.edmNtupleEtaMinGen)

else:
     process.diffractiveZAnalysisTTree.DiffractiveAnalysis.accessMCInfo = False 

#
# Run Path. 
# If TriggerOn = True (Run with trigger)
#

process.analysis_reco_step = cms.Path(process.analysisSequences)
process.castor_step = cms.Path(process.castorSequence)


if config.TriggerOn:
    process.analysis_diffractiveDiffractiveZAnalysisPATTriggerInfoTTree_step = cms.Path(
    process.eventSelectionHLT + process.diffractiveZAnalysisTTree)

else:
    process.analysis_diffractiveDiffractiveZAnalysisPATTriggerInfoTTree_step = cms.Path(
    process.eventSelection + process.diffractiveZAnalysisTTree)

