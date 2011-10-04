import FWCore.ParameterSet.Config as cms

# Settings
class config: pass
config.verbose = True
config.writeEdmOutput = False
config.runPATSequences = True
config.runOnMC = False
config.usePAT = True
config.globalTagNameData = 'GR_R_42_V19::All' 
config.instLumiROOTFile='lumibylsXing_Cert_136033-149442_7TeV_Apr21ReReco_Collisions10_JSON.root'
config.globalTagNameMC = 'START42_V14A::All'
config.comEnergy = 7000.0
config.trackAnalyzerName = 'trackHistoAnalyzer'
config.trackTagName = 'analysisTracks'
config.outputTTreeFile = 'exclusiveDijetsanalysis_PATTTreeMC.root'
config.data2011 = False

if config.runOnMC:
    config.inputFileName = '/storage2/eliza/samples_test/QCD_Pt-15to30_TuneZ2_7TeV_pythia6AODSIMS_3.root'#MC
else:
    config.inputFileName = '/storage2/antoniov/data1/MultiJet_Run2010B_Apr21ReReco-v1_AOD/MultiJet_Run2010B_Apr21ReReco-v1_AOD_7EA7B611-7371-E011-B164-002354EF3BDB.root'#Data
   #  config.inputFileName = '/storage2/eliza/samples_test/MultiJetPromptReco_v4.root'#data 2011
process = cms.Process("Analysis")

process.load('Configuration.StandardSequences.Services_cff')
process.load('Configuration.StandardSequences.GeometryExtended_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.Reconstruction_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
#process.load('JetMETCorrections.Configuration.DefaultJEC_cff')
#process.load("CondCore.DBCommon.CondDBCommon_cfi")
if config.runPATSequences:
    from ForwardAnalysis.Skimming.addPATSequences import addPATSequences
    addPATSequences(process,config.runOnMC)

if config.runOnMC:
    process.patTrigger.addL1Algos = cms.bool( False )
    process.patJets.addTagInfos   = cms.bool( False )
else:
    process.patTrigger.addL1Algos = cms.bool( True )
    process.patJets.addTagInfos   = cms.bool( True )   


process.load('FWCore.MessageService.MessageLogger_cfi')
if not config.verbose:
    process.MessageLogger.cerr.threshold = 'INFO'
else:
    process.MessageLogger.cerr.threshold = 'DEBUG'
    process.MessageLogger.debugModules = cms.untracked.vstring('exclusiveDijetsTTreeAnalysis')
    process.MessageLogger.destinations = cms.untracked.vstring('cerr')
    process.MessageLogger.categories.append('Analysis')
    process.MessageLogger.cerr.Analysis = cms.untracked.PSet(limit = cms.untracked.int32(-1))

process.options = cms.untracked.PSet( 
    wantSummary = cms.untracked.bool(True),
    SkipEvent = cms.untracked.vstring('ProductNotFound')
    )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring( 'file:%s' % config.inputFileName )
    #duplicateCheckMode = cms.untracked.string('noDuplicateCheck')
)

# Output
process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string(config.outputTTreeFile))
#-------------------------------------------------------------------------------

if config.runOnMC: process.GlobalTag.globaltag = config.globalTagNameMC
else: process.GlobalTag.globaltag = config.globalTagNameData

#---------------------------------------------------------------------------------
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

###################################################################################
# Analysis sequences
process.load("ForwardAnalysis.Utilities.tracksOutsideJets_cfi")
process.tracksOutsideJets.JetTag = "selectedPatJetsPFlow"
process.load("ForwardAnalysis.ForwardTTreeAnalysis.tracksTransverseRegion_cfi")
process.tracksTransverseRegion.JetTag = "selectedPatJetsPFlow"

####################################################################################
# Analysis modules
#--------------------------------
from ForwardAnalysis.Utilities.countsAnalyzer_cfi import countsAnalyzer
#added by eliza
if not config.runOnMC:
    process.load('ForwardAnalysis.Utilities.lumiWeight_cfi')
    process.lumiWeight.rootFileName = cms.string(config.instLumiROOTFile)
#end
 
process.load("ForwardAnalysis.ForwardTTreeAnalysis.exclusiveDijetsAnalysisSequences_cff")

if config.runOnMC:
    process.exclusiveDijetsHLTFilter.HLTPaths = ['HLT_Jet30_v*','HLT_Jet60_v*','HLT_Jet80_v*','HLT_Jet110_v*','HLT_Jet150_v*','HLT_Jet190_v*','HLT_Jet240_v*','HLT_Jet370_v*']

else:
    process.exclusiveDijetsHLTFilter.HLTPaths = ['HLT_ExclDiJet30U_HFAND_v*','HLT_ExclDiJet30U_HFOR_v*','HLT_Jet30U*'] 

if config.data2011: process.exclusiveDijetsHLTFilter.HLTPaths = ['HLT_ExclDiJet60_HFAND_v*','HLT_ExclDiJet60_HFOR_v*','HLT_Jet60_v*'] 


process.pfCandidateNoiseThresholds.src = "pfNoPileUpPFlow"
process.tracksTransverseRegion.JetTag = "selectedPatJetsPFlow"

#process.load("ForwardAnalysis.ForwardTTreeAnalysis.singleVertexFilter_cfi")

process.load('ForwardAnalysis.ForwardTTreeAnalysis.exclusiveDijetsTTreeAnalysis_cfi')
castorTagName = "castorRecHitCorrector"
if config.runOnMC: castorTagName = "castorreco"

# Diffractive analysis
process.exclusiveDijetsTTreeAnalysis.diffractiveAnalysis.triggerResultsTag = cms.InputTag("TriggerResults::HLT")
process.exclusiveDijetsTTreeAnalysis.diffractiveAnalysis.hltPath = 'HLT_L1Tech_BSC_minBias_OR'
process.exclusiveDijetsTTreeAnalysis.diffractiveAnalysis.comEnergy = config.comEnergy
process.exclusiveDijetsTTreeAnalysis.diffractiveAnalysis.trackTag = config.trackTagName
process.exclusiveDijetsTTreeAnalysis.diffractiveAnalysis.vertexTag = "goodOfflinePrimaryVertices"
process.exclusiveDijetsTTreeAnalysis.diffractiveAnalysis.particleFlowTag = "pfCandidateNoiseThresholds"
process.exclusiveDijetsTTreeAnalysis.diffractiveAnalysis.jetTag = "selectedPatJetsPFlow"
process.exclusiveDijetsTTreeAnalysis.diffractiveAnalysis.castorRecHitTag = castorTagName

if config.runOnMC: process.exclusiveDijetsTTreeAnalysis.diffractiveAnalysis.accessMCInfo = True

# Exclusive dijets analysis
process.exclusiveDijetsTTreeAnalysis.exclusiveDijetsAnalysis.EBeam = config.comEnergy/2.
process.exclusiveDijetsTTreeAnalysis.exclusiveDijetsAnalysis.TrackTag = config.trackTagName
process.exclusiveDijetsTTreeAnalysis.exclusiveDijetsAnalysis.VertexTag = "goodOfflinePrimaryVertices"
process.exclusiveDijetsTTreeAnalysis.exclusiveDijetsAnalysis.ParticleFlowTag = "pfCandidateNoiseThresholds"
process.exclusiveDijetsTTreeAnalysis.exclusiveDijetsAnalysis.JetTag = "selectedPatJetsPFlow"
process.exclusiveDijetsTTreeAnalysis.exclusiveDijetsAnalysis.JetNonCorrTag = "ak5PFJets"

process.exclusiveDijetsTTreeAnalysis.exclusiveDijetsAnalysis.TriggerResultsTag = cms.InputTag("TriggerResults::HLT")

if config.runOnMC:
    process.exclusiveDijetsTTreeAnalysis.exclusiveDijetsAnalysis.AccessMCInfo = True
    process.exclusiveDijetsTTreeAnalysis.exclusiveDijetsAnalysis.hltPaths = cms.vstring('HLT_Jet30*','HLT_Jet60_v*','HLT_Jet80_v*','HLT_Jet110_v*','HLT_Jet150_v*','HLT_Jet190_v*','HLT_Jet240_v*','HLT_Jet370_v*')
else: process.exclusiveDijetsTTreeAnalysis.exclusiveDijetsAnalysis.hltPaths = cms.vstring('HLT_ExclDiJet30U_HFAND_v*','HLT_ExclDiJet30U_HFOR_v*','HLT_Jet30U*')
if config.data2011: process.exclusiveDijetsTTreeAnalysis.exclusiveDijetsAnalysis.hltPaths = cms.vstring('HLT_ExclDiJet60_HFAND_v*','HLT_ExclDiJet60_HFOR_v*','HLT_Jet60_v*')

# PAT Trigger

#L1 Trigger
process.exclusiveDijetsTTreeAnalysis.patInfo.L1AlgoBitName = cms.vstring('L1_ZeroBias','L1_BptxMinus_NotBptxPlus','L1_SingleJet30U')
#HLT
process.exclusiveDijetsTTreeAnalysis.patInfo.HLTAlgoBitName = cms.vstring('HLT_ExclDiJet30U_HFOR_v1','HLT_DiJetAve100U_v1')

#added by eliza
if not config.runOnMC:
    process.eventWeightSequence = cms.Sequence(process.lumiWeight) 
    process.eventWeight_step = cms.Path(process.eventWeightSequence) 

if not config.runOnMC: process.castor_step = cms.Path(process.castorSequence)

process.analysis_reco_step = cms.Path(process.analysisSequences)
process.analysis_exclusiveDijetsAnalysis_step = cms.Path(process.eventSelectionHLT+
                                                         process.exclusiveDijetsTTreeAnalysis)
