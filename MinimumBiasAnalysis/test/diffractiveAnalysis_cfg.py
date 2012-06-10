import FWCore.ParameterSet.Config as cms

from Utilities.PyConfigTools.parseInput import parseInput
inputFields = ('runOnMC','generator','hltProcessNameMC')
requiredFields = ('runOnMC',)
inputOptions = parseInput(inputFields,requiredFields)
if not hasattr(inputOptions,'generator'): inputOptions.generator = 'Pythia8'
if not hasattr(inputOptions,'hltProcessNameMC'): inputOptions.hltProcessNameMC = 'HLT'

# Settings
class config: pass
config.runOnMC = inputOptions.runOnMC
config.generator = inputOptions.generator
#config.runOnMC = False
#config.generator = 'Pythia8'
config.comEnergy = 8000.0
config.maxEvents = 3000
config.verbose = True
#---
config.runPATSequences = False
config.usePAT = False
#---
config.globalTagNameData = 'GR_R_52_V8::All'
config.globalTagNameMC = 'START52_V10::All'
#config.instLumiROOTFile = 'lumibylsXing_132440-144114_7TeV_Sep17ReReco_Collisions10_JSON_v2_sub_132440.root'
config.trackAnalyzerName = 'trackHistoAnalyzer'
config.trackTagName = 'analysisTracks'
config.vertexTagName = 'analysisVertices'
config.triggerResultsProcessNameMC = inputOptions.hltProcessNameMC
#config.triggerResultsProcessNameMC = 'HLT'
#---
config.switchPVFilter = True #primaryVertexFilterLooseNDOF0
config.varyAttributes = False
config.runPFlowThresholdAnalysis = False
config.runOfflineOnly = False
config.runNoColl = False
config.runZeroBias = False
config.runEtaMaxFilter = True
#config.runHCALFilter = False
#config.runCastorFilter = True

if config.runOnMC:
    config.l1Paths = ()
    config.hltPaths =()

    config.inputFileName = '/storage2/antoniov/data1/MinBias_TuneZ2star_8TeV-pythia6_NoPileUp_START50_V15-v1_GEN-SIM-RECO/MinBias_TuneZ2star_8TeV-pythia6_NoPileUp_START50_V15-v1_GEN-SIM-RECO_0000_00FD0D3D-AB6F-E111-ACA5-0030487F1657.root'
    config.globalTagName = config.globalTagNameMC
    config.outputTTreeFile = 'diffractiveAnalysisTTree_MC.root'
else:
    config.l1Paths = ()
    config.hltPaths = ()

    config.inputFileName = '/storage1/antoniov/data1/LP_MinBias2_Run2012A_PromptReco-v1_RECO_Run193092/LP_MinBias2_Run2012A_PromptReco-v1_RECO_Run193092_00A2F6F1-8895-E111-9ADD-0025901D5DB8.root'
    config.globalTagName = config.globalTagNameData
    config.outputTTreeFile = 'diffractiveAnalysisTTree.root'


############################################################
############################################################
process = cms.Process("Analysis")

process.load('FWCore.MessageService.MessageLogger_cfi')

process.options = cms.untracked.PSet(
	wantSummary = cms.untracked.bool(True),
	SkipEvent = cms.untracked.vstring('ProductNotFound')
	)

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(config.maxEvents) )

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring( 'file:%s' % config.inputFileName )
    #duplicateCheckMode = cms.untracked.string('noDuplicateCheck')
)

#---------------------------------------------------------------------------------
# Output
process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string(config.outputTTreeFile))


process.load('Configuration.StandardSequences.Services_cff')
process.load('Configuration.StandardSequences.GeometryExtended_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.Reconstruction_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.GlobalTag.globaltag = config.globalTagName

if config.runPATSequences:
    from ForwardAnalysis.Skimming.addPATSequences import addPATSequences
    addPATSequences(process,config.runOnMC)

    if config.runOnMC:
	process.patTrigger.addL1Algos = cms.bool( False )
	process.patJets.addTagInfos   = cms.bool( False )
    else:
	process.patTrigger.addL1Algos = cms.bool( True )
	process.patJets.addTagInfos   = cms.bool( True )   

#from ForwardAnalysis.ForwardTTreeAnalysis.addCastorRecHitCorrector import addCastorRecHitCorrector
#addCastorRecHitCorrector(process)

process.load("ForwardAnalysis.MinimumBiasAnalysis.diffractiveAnalysisSequences_cff")
#######################################################################################################################
# Analysis modules
#--------------------------------
## if not config.runOnMC:
##     process.load('ForwardAnalysis.Utilities.lumiWeight_cfi')
##     process.lumiWeight.rootFileName = cms.string(config.instLumiROOTFile)
##     process.eventWeightSequence = cms.Sequence(process.lumiWeight) 
##     process.eventWeight_step = cms.Path(process.eventWeightSequence)
############################################################################################################## 
#process.pfCandidateNoiseThresholds.src = "pfNoPileUpPFlow"
#process.tracksTransverseRegion.JetTag = "selectedPatJetsPFlow"

########################################################################
from ForwardAnalysis.ForwardTTreeAnalysis.DiffractiveAnalysis_cfi import DiffractiveAnalysis
#from ForwardAnalysis.ForwardTTreeAnalysis.PATInfo_cfi import PATInfo
print DiffractiveAnalysis

#PATInfo.L1AlgoBitName =  config.l1Paths 
#PATInfo.HLTAlgoBitName = config.hltPaths 
#PATInfo.runALLTriggerPath = True

process.diffractiveAnalysisTTree = cms.EDAnalyzer("DiffractiveAnalysisTTree",
	DiffractiveAnalysis = DiffractiveAnalysis
	)
process.diffractiveAnalysisTTree.DiffractiveAnalysis.hltPath = 'HLT_L1Tech53_MB_2_v1'
process.diffractiveAnalysisTTree.DiffractiveAnalysis.trackTag = 'analysisTracks'
process.diffractiveAnalysisTTree.DiffractiveAnalysis.vertexTag = "analysisVertices"
process.diffractiveAnalysisTTree.DiffractiveAnalysis.particleFlowTag = "pfCandidateNoiseThresholds"
#process.diffractiveAnalysisTTree.DiffractiveAnalysis.jetTag = "selectedPatJetsPFlow"
process.diffractiveAnalysisTTree.DiffractiveAnalysis.comEnergy = config.comEnergy
process.diffractiveAnalysisTTree.DiffractiveAnalysis.techTrigBit = 53

process.load('Utilities.AnalysisTools.trackHistos_cfi')
process.trackHistos.src = config.trackTagName
process.load('Utilities.AnalysisTools.trackHistoAnalyzer_cfi')
process.trackHistoAnalyzer.TrackTag = config.trackTagName
process.load('Utilities.AnalysisTools.pfHistos_cfi')
process.pFlowHistos = process.pfHistos.clone( src = 'particleFlow' )
process.pFlowWithThresholdsHistos = process.pfHistos.clone( src = 'pfCandidateNoiseThresholds' )
process.load('Utilities.AnalysisTools.pFlowNoiseAnalyzer_cfi')
process.pFlowNoiseAnalyzer.particleFlowTag = 'particleFlow'

attributesEnergyScale = [{'ApplyEnergyScaleHCAL':True,'EnergyScaleFactorHCAL':0.90,'HCALTowerSummaryTag':'hcalActivitySummaryScale090'},
                         {'ApplyEnergyScaleHCAL':True,'EnergyScaleFactorHCAL':0.92,'HCALTowerSummaryTag':'hcalActivitySummaryScale092'},
                         {'ApplyEnergyScaleHCAL':True,'EnergyScaleFactorHCAL':0.95,'HCALTowerSummaryTag':'hcalActivitySummaryScale095'},
                         {'ApplyEnergyScaleHCAL':True,'EnergyScaleFactorHCAL':0.98,'HCALTowerSummaryTag':'hcalActivitySummaryScale098'},  
                         {'ApplyEnergyScaleHCAL':True,'EnergyScaleFactorHCAL':1.02,'HCALTowerSummaryTag':'hcalActivitySummaryScale102'},
                         {'ApplyEnergyScaleHCAL':True,'EnergyScaleFactorHCAL':1.05,'HCALTowerSummaryTag':'hcalActivitySummaryScale105'},
                         {'ApplyEnergyScaleHCAL':True,'EnergyScaleFactorHCAL':1.08,'HCALTowerSummaryTag':'hcalActivitySummaryScale108'}, 
                         {'ApplyEnergyScaleHCAL':True,'EnergyScaleFactorHCAL':1.10,'HCALTowerSummaryTag':'hcalActivitySummaryScale110'}]

attributesThresholds = [{'EnergyThresholdHF':3.6},
                        {'EnergyThresholdHF':6.0},
                        {'EnergyThresholdHF':8.0},
                        {'EnergyThresholdHB':1.2,'EnergyThresholdHE':1.6},
                        {'EnergyThresholdHB':1.8,'EnergyThresholdHE':2.4}]

attributes = attributesEnergyScale
attributes.extend(attributesThresholds)

processIdFilters = None
if config.runOnMC:
    from Utilities.AnalysisSequences.addProcessIdFilters import addProcessIdFilters
    if config.verbose:
        print 'Using generator type',config.generator
    processIdFilters = addProcessIdFilters(process,config.generator)

##########################################################################
if config.runOnMC:
  process.gen_step = cms.Path(process.genChargedParticles+
                              process.genProtonDissociative*process.edmNtupleMxGen+
                              process.genStableParticles*
                              process.etaMaxGen+process.etaMinGen*
                              process.edmNtupleEtaMaxGen+process.edmNtupleEtaMinGen)

process.analysis_reco_step = cms.Path(process.analysisSequences)
#process.castor_step = cms.Path(process.castorSequence)

# Path for event counting
from Utilities.AnalysisTools.countsAnalyzer_cfi import countsAnalyzer
process.countsAll = countsAnalyzer.clone()
process.countshltMinBias = countsAnalyzer.clone()
process.countsBPTX = countsAnalyzer.clone()
process.countsVertexFilter = countsAnalyzer.clone()
process.countsFilterScraping = countsAnalyzer.clone()
process.countsHcalNoiseFilter = countsAnalyzer.clone()
process.countsMultipleVertexVeto = countsAnalyzer.clone()
process.countsEtaMinFilter = countsAnalyzer.clone()
process.countEvents_step = cms.Path(process.countsAll +
                                    process.hltMinBias +
                                    process.countshltMinBias +
                                    process.bptx +
                                    process.countsBPTX + 
                                    process.primaryVertexFilter +
                                    process.countsVertexFilter +
                                    process.filterScraping +
                                    process.countsFilterScraping +
                                    process.HBHENoiseFilter+process.hcalNoiseFilter +
                                    process.countsHcalNoiseFilter +
                                    process.multipleVertexVeto + 
                                    process.countsMultipleVertexVeto +  
                                    process.etaMinFilter +
                                    process.countsEtaMinFilter)

from Utilities.PyConfigTools.analysisTools import *

makeAnalysis(process,'diffractiveAnalysisTTree','eventSelectionMinBias')
makeAnalysis(process,'diffractiveAnalysisTTree','eventSelectionMinBiasNoVertex')
makeAnalysis(process,config.trackAnalyzerName,'eventSelectionMinBias')
makeAnalysis(process,'pFlowHistos','eventSelectionMinBias')
makeAnalysis(process,'pFlowWithThresholdsHistos','eventSelectionMinBias')

if config.varyAttributes:
    makeAnalysis(process,'diffractiveAnalysisTTree','eventSelectionMinBias',attributes)

if config.runPFlowThresholdAnalysis:
    process.pfCandidateCollNew = cms.Sequence(process.pfCandidateHFEdges+process.pfCandidateNoiseThresholds)
    process.analysisSequences.replace(process.pfCandidateNoiseThresholds,process.pfCandidateCollNew)
    makeAnalysis(process,'pFlowNoiseAnalyzer','eventSelectionMinBias')

    process.pFlowNoiseAnalyzerHFEdges = process.pFlowNoiseAnalyzer.clone( particleFlowTag = 'pfCandidateHFEdges' )
    makeAnalysis(process,'pFlowNoiseAnalyzerHFEdges','eventSelectionMinBias')

if config.runOfflineOnly:
    makeAnalysis(process,'diffractiveAnalysisTTree','eventSelection')
    makeAnalysis(process,'diffractiveAnalysisTTree','eventSelectionNoVertex')

if config.runZeroBias:
    makeAnalysis(process,'diffractiveAnalysisTTree','eventSelectionZeroBias')
    makeAnalysis(process,'diffractiveAnalysisTTree','eventSelectionZeroBiasNoVertex')

if config.runNoColl:
    makeAnalysis(process,'diffractiveAnalysisTTree','eventSelectionMinBiasNoColl')
    makeAnalysis(process,'diffractiveAnalysisTTree','eventSelectionMinBiasNoCollNoVertex')

if config.runEtaMaxFilter:
    makeAnalysis(process,'diffractiveAnalysisTTree','eventSelectionMinBiasEtaMaxFilter')
    makeAnalysis(process,'diffractiveAnalysisTTree','eventSelectionMinBiasEtaMinFilter')
    makeAnalysis(process,'diffractiveAnalysisTTree','eventSelectionMinBiasEtaMaxFilterNoVertex')
    makeAnalysis(process,'diffractiveAnalysisTTree','eventSelectionMinBiasEtaMinFilterNoVertex')

if config.switchPVFilter:
    from Utilities.PyConfigTools.replaceInPaths import replaceInPaths
    replaceInPaths(process,'primaryVertexFilter','primaryVertexFilterLooseNDOF0')

if config.runOnMC:
    from Utilities.PyConfigTools.removeFromPaths import removeFromPaths
    removeFromPaths(process,'bptx')
    removeFromPaths(process,'hltMinBias')
    #removeFromPaths(process,'castorInvalidDataFilter')

    DiffractiveAnalysisMC = DiffractiveAnalysis.clone(
                                  accessMCInfo = True,
                                  hltPath = 'HLT_Physics_v*',
                                  castorRecHitTag = 'castorreco', 
                                  triggerResultsTag = cms.InputTag("TriggerResults","",config.triggerResultsProcessNameMC)
    )
    from Utilities.PyConfigTools.setAnalyzerAttributes import setAnalyzerAttributes
    setAnalyzerAttributes(process,'diffractiveAnalysisTTree',
                                  DiffractiveAnalysis = DiffractiveAnalysisMC)
