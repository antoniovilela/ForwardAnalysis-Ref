import FWCore.ParameterSet.Config as cms

# Settings
class config: pass
config.verbose = True
config.writeEdmOutput = False
config.runOnMC = False
config.usePAT = False
config.globalTagNameData = 'GR_R_42_V19::All' 
config.instLumiROOTFile='lumibylsXing_Cert_136033-149442_7TeV_Apr21ReReco_Collisions10_JSON.root'
globalTagNameMC = ''
config.comEnergy = 7000.0
config.trackAnalyzerName = 'trackHistoAnalyzer'
config.trackTagName = 'analysisTracks'
#config.generator = 'Pythia6'

config.inputFileName = '/storage2/eliza/JetMay10RecoRun2011.root'
#config.outputEdmFile = 'DijetsAnalysis.root'
config.outputTTreeFile = 'fwdQCDTTreeAnalysis.root'

process = cms.Process("Analysis")

process.load('FWCore.MessageService.MessageLogger_cfi')
process.MessageLogger.cerr.threshold = 'DEBUG'
process.MessageLogger.debugModules = cms.untracked.vstring('fwdQCDTTreeAnalysis')
process.MessageLogger.destinations = cms.untracked.vstring('cerr')
process.MessageLogger.categories.append('Analysis')
process.MessageLogger.cerr.Analysis = cms.untracked.PSet(limit = cms.untracked.int32(-1))

process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True),
SkipEvent = cms.untracked.vstring('ProductNotFound') )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring( 'file:%s' % config.inputFileName )
    #duplicateCheckMode = cms.untracked.string('noDuplicateCheck')
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

###################################################################################
# CASTOR RecHit Corrector
if not config.runOnMC:
    process.castorRecHitCorrector = cms.EDProducer("RecHitCorrector",
        rechitLabel = cms.InputTag("castorreco","","RECO"),
        revertFactor = cms.double(1),
        doInterCalib = cms.bool(False)
    )

    process.load("CondCore.DBCommon.CondDBSetup_cfi")
    process.CastorDbProducer = cms.ESProducer("CastorDbProducer")
    process.es_castor_pool = cms.ESSource("PoolDBESSource",
        process.CondDBSetup,
        timetype = cms.string('runnumber'),
        connect = cms.string('frontier://cmsfrontier.cern.ch:8000/FrontierProd/CMS_COND_31X_HCAL'),
        authenticationMethod = cms.untracked.uint32(0),
        toGet = cms.VPSet(
            cms.PSet(
                record = cms.string('CastorChannelQualityRcd'),  
                tag = cms.string('CastorChannelQuality_v2.2_offline')
            )
        )
    )
    process.es_prefer_castor = cms.ESPrefer('PoolDBESSource','es_castor_pool')

    process.castorInvalidDataFilter = cms.EDFilter("CastorInvalidDataFilter")
    process.castorSequence = cms.Sequence(process.castorInvalidDataFilter+process.castorRecHitCorrector)

####################################################################################
# Analysis modules
#--------------------------------
from ForwardAnalysis.Utilities.countsAnalyzer_cfi import countsAnalyzer
if not config.runOnMC:
    process.load('ForwardAnalysis.Utilities.lumiWeight_cfi')
    process.lumiWeight.rootFileName = cms.string(config.instLumiROOTFile)
 
#process.load('RecoJets.Configuration.RecoPFJets_cff')
#process.load('RecoJets.Configuration.RecoJets_cff')
#process.load('JetMETCorrections.Configuration.DefaultJEC_cff')

process.load("ForwardAnalysis.ForwardTTreeAnalysis.exclusiveDijetsAnalysisSequences_cff")
process.pfCandidateNoiseThresholds.src = "particleFlow"
#process.load("ForwardAnalysis.ForwardTTreeAnalysis.singleVertexFilter_cfi")

from ForwardAnalysis.ForwardTTreeAnalysis.DiffractiveAnalysis_cfi import DiffractiveAnalysis
process.fwdQCDTTreeAnalysis = cms.EDAnalyzer('ProcessedTreeProducer',
    diffractiveAnalysis = DiffractiveAnalysis,
    ## jet collections ###########################
    pfjets          = cms.InputTag('ak5PFJets'),
    calojets        = cms.InputTag('ak5CaloJets'),
    ## database entry for the uncertainties ######
    PFPayloadName   = cms.string('AK5PF'),
    CaloPayloadName = cms.string('AK5Calo'),
    ## calojet ID and extender for the JTA #######
    calojetID       = cms.InputTag('ak5JetID'),
    calojetExtender = cms.InputTag('ak5JetExtender'),
    ## set the conditions for good Vtx counting ##
    offlineVertices = cms.InputTag('offlinePrimaryVertices'),
    goodVtxNdof     = cms.double(4), 
    goodVtxZ        = cms.double(24),
    ## rho #######################################
    srcCaloRho      = cms.InputTag('kt6CaloJets','rho'),
    srcPFRho        = cms.InputTag('kt6PFJets','rho'),
    ## preselection cuts #########################
    maxY            = cms.double(5.0), 
    minPFPt         = cms.double(20),
    minPFFatPt      = cms.double(10),
    maxPFFatEta     = cms.double(2.5),
    minCaloPt       = cms.double(20),
    minNPFJets      = cms.int32(1),
    minNCaloJets    = cms.int32(1), 
    minJJMass       = cms.double(-1),
    ## trigger ###################################
    printTriggerMenu = cms.untracked.bool(True),
    processName     = cms.string('HLT'),
    triggerName     = cms.vstring('HLT_Jet30_v1','HLT_Jet30_v2','HLT_Jet30_v3','HLT_Jet30_v4','HLT_Jet30_v5','HLT_Jet30_v6',
                                  'HLT_Jet60_v1','HLT_Jet60_v2','HLT_Jet60_v3','HLT_Jet60_v4','HLT_Jet60_v5','HLT_Jet60_v6',
                                  'HLT_Jet80_v1','HLT_Jet80_v2','HLT_Jet80_v3','HLT_Jet80_v4','HLT_Jet80_v5','HLT_Jet80_v6',
                                  'HLT_Jet110_v1','HLT_Jet110_v2','HLT_Jet110_v3','HLT_Jet110_v4','HLT_Jet110_v5','HLT_Jet110_v6',
                                  'HLT_Jet150_v1','HLT_Jet150_v2','HLT_Jet150_v3','HLT_Jet150_v4','HLT_Jet150_v5','HLT_Jet150_v6',
                                  'HLT_Jet190_v1','HLT_Jet190_v2','HLT_Jet190_v3','HLT_Jet190_v4','HLT_Jet190_v5','HLT_Jet190_v6',
                                  'HLT_Jet240_v1','HLT_Jet240_v2','HLT_Jet240_v3','HLT_Jet240_v4','HLT_Jet240_v5','HLT_Jet240_v6',
                                  'HLT_Jet300_v1', 'HLT_Jet300_v2','HLT_Jet300_v3','HLT_Jet300_v4','HLT_Jet300_v5',
                                  'HLT_Jet370_v1','HLT_Jet370_v2','HLT_Jet370_v3','HLT_Jet370_v4','HLT_Jet370_v5','HLT_Jet370_v6'),
    triggerResults  = cms.InputTag("TriggerResults","","HLT"),
    triggerEvent    = cms.InputTag("hltTriggerSummaryAOD","","HLT"),
    ## jec services ##############################
    pfjecService    = cms.string('ak5PFL1FastL2L3Residual'),
    calojecService  = cms.string('ak5CaloL1L2L3Residual')
)
# Diffractive analysis
process.fwdQCDTTreeAnalysis.diffractiveAnalysis.triggerResultsTag = cms.InputTag("TriggerResults::HLT")
process.fwdQCDTTreeAnalysis.diffractiveAnalysis.comEnergy = config.comEnergy
process.fwdQCDTTreeAnalysis.diffractiveAnalysis.trackTag = config.trackTagName
process.fwdQCDTTreeAnalysis.diffractiveAnalysis.vertexTag = "offlinePrimaryVertices"
process.fwdQCDTTreeAnalysis.diffractiveAnalysis.particleFlowTag = "pfCandidateNoiseThresholds"
process.fwdQCDTTreeAnalysis.diffractiveAnalysis.metTag = "met"
process.fwdQCDTTreeAnalysis.diffractiveAnalysis.jetTag = "ak5PFJets"

############# Turn-on the fastjet area calculation needed for the L1Fastjet ##############
############# applied only to PFJets because if CaloJets are re-recoed the JetID map will be lost #####
process.kt6PFJets.doRhoFastjet = True
process.kt6PFJets.Rho_EtaMax = cms.double(5.0)
process.kt6CaloJets.doRhoFastjet = True
process.kt6CaloJets.Rho_EtaMax = cms.double(5.0)
process.ak7PFJets.doAreaFastjet = True
process.ak7PFJets.Rho_EtaMax = cms.double(5.0)
process.ak7PFJets.jetPtMin = cms.double(5.0)
process.ak5PFJets.doAreaFastjet = True
process.ak5PFJets.Rho_EtaMax = cms.double(5.0)
process.ak5PFJets.jetPtMin = cms.double(5.0)

#process.reco_step = cms.Path(process.kt6PFJets * process.kt6CaloJets * process.ak5PFJets * process.ak7PFJets * process.ak5 * process.ak7)
process.reco_step = cms.Path(process.kt6PFJets * process.kt6CaloJets * process.ak5PFJets)

if not config.runOnMC:
    process.eventWeightSequence = cms.Sequence(process.lumiWeight) 
    process.eventWeight_step = cms.Path(process.eventWeightSequence) 

if not config.runOnMC: process.castor_step = cms.Path(process.castorSequence)

process.analysis_reco_step = cms.Path(process.analysisSequences)
process.analysis_exclusiveDijetsAnalysis_step = cms.Path(process.eventSelectionHLT+
                                                         process.fwdQCDTTreeAnalysis)
