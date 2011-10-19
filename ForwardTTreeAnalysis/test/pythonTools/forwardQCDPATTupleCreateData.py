#!/usr/bin/env python
import os
import random
import time

def crabCall(uiworkingdir):
    print ''
    print '##########################################'
    print 'Creating job...'
    print '##########################################'
    print ''
    os.system('crab -create')
    print ''
    print '##########################################'
    print 'Submiting job...'
    print '##########################################'
    os.system('crab -submit -c '+ uiworkingdir)

def cmsRunCall():
    print ''
    print '##########################################'
    print 'Starting CMSSW...'
    print '##########################################'
    print ''
    os.system('cmsRun pset.py')
    print ''

def ConfigHandler(dataset,uiworkingdir,userremotedir,jsonfile,lumis,lumisjob,runsel,lumixs,sewhitelist,output,triggersmc,triggersdata,mail):
    pset_py = """

import FWCore.ParameterSet.Config as cms

# Settings
class config: pass
config.verbose = True
config.writeEdmOutput = False
config.runOnMC = False
config.runPATSequences = True
config.usePAT = False
config.globalTagNameData = 'GR_R_42_V19::All' 
config.instLumiROOTFile='@@LUMIXS@@'
config.globalTagNameMC = 'START42_V14A::All'
config.comEnergy = 7000.0
config.trackAnalyzerName = 'trackHistoAnalyzer'
config.trackTagName = 'analysisTracks'

if config.runOnMC:
    config.hltPaths =(@@TRIGGERSMC@@)
else:
    #config.hltPaths = (@@TRIGGERSDATA@@')
    config.hltPaths = (@@TRIGGERSDATA@@)

#config.generator = 'Pythia6'

#config.outputEdmFile = 'DijetsAnalysis.root'
config.outputTTreeFile = '@@OUTPUT@@'

if config.runOnMC:
    config.inputFileName = '/storage2/eliza/samples_test/QCD_Pt-15to30_TuneZ2_7TeV_pythia6AODSIMS_3.root'# MC
else:
    #config.inputFileName = '/storage2/eliza/samples_test/MultiJetPromptReco_v4.root'#data 2011
    config.inputFileName = '/storage2/antoniov/data1/MultiJet_Run2010B_Apr21ReReco-v1_AOD/MultiJet_Run2010B_Apr21ReReco-v1_AOD_7EA7B611-7371-E011-B164-002354EF3BDB.root' 


process = cms.Process("Analysis")

process.load('FWCore.MessageService.MessageLogger_cfi')
process.MessageLogger.cerr.threshold = 'DEBUG'
process.MessageLogger.debugModules = cms.untracked.vstring('forwardQCDTTreeAnalysis')
process.MessageLogger.destinations = cms.untracked.vstring('cerr')
process.MessageLogger.categories.append('Analysis')
process.MessageLogger.cerr.Analysis = cms.untracked.PSet(limit = cms.untracked.int32(-1))

process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True),
SkipEvent = cms.untracked.vstring('ProductNotFound') )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(3000) )

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
    from ForwardAnalysis.ForwardTTreeAnalysis.addCastorRecHitCorrector import addCastorRecHitCorrector
    addCastorRecHitCorrector(process)
####################################################################################
# Analysis modules
#--------------------------------
from ForwardAnalysis.Utilities.countsAnalyzer_cfi import countsAnalyzer
if not config.runOnMC:
    process.load('ForwardAnalysis.Utilities.lumiWeight_cfi')
    process.lumiWeight.rootFileName = cms.string(config.instLumiROOTFile)


if config.runOnMC:
     process.forwardQCDTTreeAnalysis.exclusiveDijetsAnalysis.AccessMCInfo = True
else:
     process.forwardQCDTTreeAnalysis.exclusiveDijetsAnalysis.AccessMCInfo = False
 
#process.load('RecoJets.Configuration.RecoPFJets_cff')
#process.load('RecoJets.Configuration.RecoJets_cff')
#process.load('JetMETCorrections.Configuration.DefaultJEC_cff')

process.load("ForwardAnalysis.ForwardTTreeAnalysis.exclusiveDijetsAnalysisSequences_cff")

if config.runOnMC:
    process.exclusiveDijetsHLTFilter.HLTPaths = config.hltPaths 
else:
    process.exclusiveDijetsHLTFilter.HLTPaths = config.hltPaths 

process.pfCandidateNoiseThresholds.src = "pfNoPileUpPFlow"
process.tracksTransverseRegion.JetTag = "selectedPatJetsPFlow"

from ForwardAnalysis.ForwardTTreeAnalysis.DiffractiveAnalysis_cfi import DiffractiveAnalysis
from ForwardAnalysis.ForwardTTreeAnalysis.ExclusiveDijetsAnalysis_cfi import ExclusiveDijetsAnalysis
process.forwardQCDTTreeAnalysis = cms.EDAnalyzer('ProcessedTreeProducer',
    diffractiveAnalysis = DiffractiveAnalysis,
    exclusiveDijetsAnalysis = ExclusiveDijetsAnalysis,
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

config.castorTagName = "castorRecHitCorrector"
if config.runOnMC: config.castorTagName = "castorreco"
# Diffractive analysis
process.forwardQCDTTreeAnalysis.diffractiveAnalysis.triggerResultsTag = cms.InputTag("TriggerResults::HLT")
process.forwardQCDTTreeAnalysis.diffractiveAnalysis.hltPath = ''
process.forwardQCDTTreeAnalysis.diffractiveAnalysis.comEnergy = config.comEnergy
process.forwardQCDTTreeAnalysis.diffractiveAnalysis.trackTag = config.trackTagName
process.forwardQCDTTreeAnalysis.diffractiveAnalysis.vertexTag = "goodOfflinePrimaryVertices"
process.forwardQCDTTreeAnalysis.diffractiveAnalysis.particleFlowTag = "pfCandidateNoiseThresholds"
process.forwardQCDTTreeAnalysis.diffractiveAnalysis.jetTag = "selectedPatJetsPFlow"
process.forwardQCDTTreeAnalysis.diffractiveAnalysis.castorRecHitTag = config.castorTagName
if config.runOnMC:
    process.forwardQCDTTreeAnalysis.diffractiveAnalysis.accessMCInfo = True

# Exclusive dijets analysis
process.forwardQCDTTreeAnalysis.exclusiveDijetsAnalysis.TriggerResultsTag = cms.InputTag("TriggerResults::HLT")
process.forwardQCDTTreeAnalysis.exclusiveDijetsAnalysis.hltPaths = config.hltPaths
process.forwardQCDTTreeAnalysis.exclusiveDijetsAnalysis.EBeam = config.comEnergy/2.
process.forwardQCDTTreeAnalysis.exclusiveDijetsAnalysis.TrackTag = config.trackTagName
process.forwardQCDTTreeAnalysis.exclusiveDijetsAnalysis.VertexTag = "goodOfflinePrimaryVertices"
process.forwardQCDTTreeAnalysis.exclusiveDijetsAnalysis.ParticleFlowTag = "pfCandidateNoiseThresholds"
process.forwardQCDTTreeAnalysis.exclusiveDijetsAnalysis.JetTag = "selectedPatJetsPFlow"
process.forwardQCDTTreeAnalysis.exclusiveDijetsAnalysis.JetNonCorrTag = "ak5PFJets"
if config.runOnMC:
    process.forwardQCDTTreeAnalysis.exclusiveDijetsAnalysis.AccessMCInfo = True

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
process.reco_step = cms.Path(process.kt6PFJets * process.kt6CaloJets * process.ak5PFJets)

if not config.runOnMC: process.castor_step = cms.Path(process.castorSequence)

if config.runPATSequences:
    from ForwardAnalysis.Skimming.addPATSequences import addPATSequences
    addPATSequences(process,config.runOnMC)

    if config.runOnMC:
	process.patTrigger.addL1Algos = cms.bool( False )
	process.patJets.addTagInfos   = cms.bool( False )
    else:
	process.patTrigger.addL1Algos = cms.bool( True )
	process.patJets.addTagInfos   = cms.bool( True )   

if not config.runOnMC:
    process.eventWeightSequence = cms.Sequence(process.lumiWeight) 
    process.eventWeight_step = cms.Path(process.eventWeightSequence) 

if config.runOnMC:
   process.gen_step = cms.Path(process.genChargedParticles+
                               process.genProtonDissociative*process.edmNtupleMxGen+
                               process.genStableParticles*
                               process.etaMaxGen+process.etaMinGen*
                               process.edmNtupleEtaMaxGen+process.edmNtupleEtaMinGen)
process.analysis_reco_step = cms.Path(process.analysisSequences)
process.analysis_forwardQCDAnalysis_step = cms.Path(process.eventSelectionHLT+
                                                    process.forwardQCDTTreeAnalysis)


"""

    crab_cfg = """
[CRAB]
jobtype = cmssw
scheduler = glite
use_server =  1
#server_name = cern_vocms22 2.7.3- glite

[CMSSW]
datasetpath=@@DATASET@@
pset=pset.py
output_file=@@OUTPUT@@
lumi_mask=@@JSONFILE@@
total_number_of_lumis =@@LUMIS@@
lumis_per_job = @@LUMISJOB@@
runselection = @@RUNSEL@@
get_edm_output = 1

[USER]
ui_working_dir=@@UIWORKINGDIR@@
return_data = 1
copy_data = 0
publish_data=0
email = @@MAIL@@
additional_input_files = @@LUMIXS@@

#ui_working_dir=@@UIWORKINGDIR@@
#return_data = 0
#copy_data = 1
#publish_data=0
#email = dmf@cern.ch
#check_user_remote_dir = 1
#storage_element = se-dcache.hepgrid.uerj.br 
#storage_path = /srm/managerv2?SFN=/pnfs/hepgrid.uerj.br/data/cms/store/user/dmf
#storage_port = 8443
#user_remote_dir = @@USEREMOTE@@
#additional_input_files = @@LUMIXS@@


[GRID]
proxy_server = myproxy.cern.ch
@@USESEWHITELIST@@se_white_list = @@SEWHITELIST@@
se_black_list = T3_US_COLORADO, T3_US_FNALLPC
virtual_organization = cms

"""
    print ''
    print '##########################################'
    print 'Removing old stuff...'
    print '##########################################'
    os.system('rm pset.py')
    os.system('rm crab.cfg')
    print ''
    print '##########################################'
    print 'Creating PSet file...'
    print '##########################################'

    pset_py = pset_py.replace('@@TRIGGERSMC@@',triggersmc,-1)
    pset_py = pset_py.replace('@@TRIGGERSDATA@@',triggersdata,-1)
    pset_py = pset_py.replace('@@OUTPUT@@',output,-1)
    pset_py = pset_py.replace('@@LUMIXS@@',lumixs,-1)

    pset_pyFile = open('pset.py','a+')
    pset_pyFile.write(pset_py)
    pset_pyFile.close()

    print ''
    print '##########################################'
    print 'Creating CRAB Config file...'
    print '##########################################'

    crab_cfg = crab_cfg.replace('@@DATASET@@',dataset,-1)
    crab_cfg = crab_cfg.replace('@@OUTPUT@@',output,-1)
    crab_cfg = crab_cfg.replace('@@LUMIXS@@',lumixs,-1)
    crab_cfg = crab_cfg.replace('@@LUMIS@@',lumis,-1)
    crab_cfg = crab_cfg.replace('@@LUMISJOB@@',lumisjob,-1)
    crab_cfg = crab_cfg.replace('@@JSONFILE@@',jsonfile,-1)
    crab_cfg = crab_cfg.replace('@@UIWORKINGDIR@@',uiworkingdir,-1)
    crab_cfg = crab_cfg.replace('@@SEWHITELIST@@',sewhitelist,-1)
    crab_cfg = crab_cfg.replace('@@USEREMOTE@@',userremotedir,-1)
    crab_cfg = crab_cfg.replace('@@RUNSEL@@',runsel,-1)
    crab_cfg = crab_cfg.replace('@@MAIL@@',mail,-1)
    #use or not se_white_list
    if (sewhitelist == ''):
        crab_cfg = crab_cfg.replace('@@USESEWHITELIST@@','#',-1)
    else:
        crab_cfg = crab_cfg.replace('@@USESEWHITELIST@@','',-1)
    crab_cfgFile = open('crab.cfg','a+')
    crab_cfgFile.write(crab_cfg)
    crab_cfgFile.close()

    ##crab or cmsRun
    crabCall(uiworkingdir)

    #cmsRunCall()
    #clear
    print ''
    print '##########################################'
    print 'Backuping crab.cfg and pset.py...'
    print '##########################################'
    os.system('mkdir -p '+uiworkingdir+'/crabCallerBackup')
    os.system('cp crab.cfg '+uiworkingdir+'/crabCallerBackup/.')
    os.system('cp pset.py '+uiworkingdir+'/crabCallerBackup/.')
    print ''
    print 'Backups saved: '+uiworkingdir+'/crabCallerBackup/'
    print ''
    print ''
    print '##########################################'
    print 'Cleaning...'
    print '##########################################'
    os.system('rm pset.py')
    os.system('rm crab.cfg')


print ''
print '@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@'
print '@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@'
print 'Starting submission...'
print '@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@'
print '@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@'

###############
# Samples 2010
###############

## /JetMETTau/Run2010A-Apr21ReReco-v1/AOD 
dataset = '/JetMETTau/Run2010A-Apr21ReReco-v1/AOD '  
uiworkingdir = 'crab_JetMETTau-Run2010A-Apr21ReReco-v1-AOD'
userremotedir = '/Data/crab_JetMETTau-Run2010A-Apr21ReReco-v1-AOD'
jsonfile = 'Intersection_Castor_GOOD_135821-141887.json'
lumis ='1'
lumisjob = '15'
runsel = '135821-141887'
lumixs = 'lumibylsXing_Cert_136033-149442_7TeV_Apr21ReReco_Collisions10_JSON.root'
sewhitelist = ''
output = 'JetMetTauPATTuple.root'
triggersmc = "'HLT_L1Jet6U','HLT_Jet15U','HLT_Jet30U','HLT_Jet50U'"
triggersdata = "'HLT_L1Jet6U','HLT_Jet15U','HLT_Jet30U','HLT_Jet50U'"
mail='dmf@cern.ch'
#ConfigHandler(dataset,uiworkingdir,userremotedir,jsonfile,lumis,lumisjob,runsel,lumixs,sewhitelist,output,triggersmc,triggersdata,mail)

## /JetMET/Run2010A-Apr21ReReco-v1/AOD 
dataset = '/JetMET/Run2010A-Apr21ReReco-v1/AOD'  
uiworkingdir = 'crab_JetMET-Run2010A-Apr21ReReco-v1-AOD'
userremotedir = '/Data/crab_JetMET-Run2010A-Apr21ReReco-v1-AOD'
jsonfile = 'Intersection_Castor_GOOD_141950-144114.json'
lumis ='1'
lumisjob = '15'
runsel = '141950-144114'
lumixs = 'lumibylsXing_Cert_136033-149442_7TeV_Apr21ReReco_Collisions10_JSON.root'
sewhitelist = ''
output = 'JetMetPATTuple.root'
triggersmc = "'HLT_L1Jet6U','HLT_Jet15U','HLT_Jet30U','HLT_Jet50U','HLT_Jet70U','HLT_Jet100U'"
triggersdata = "'HLT_L1Jet6U','HLT_Jet15U','HLT_Jet30U','HLT_Jet50U','HLT_Jet70U','HLT_Jet100U'"
mail = 'dmf@cern.ch'
#ConfigHandler(dataset,uiworkingdir,userremotedir,jsonfile,lumis,lumisjob,runsel,lumixs,sewhitelist,output,triggersmc,triggersdata,mail)

## /Jet/Run2010B-Apr21ReReco-v1/AOD 
dataset = '/Jet/Run2010B-Apr21ReReco-v1/AOD'  
uiworkingdir = 'crab_Jet-Run2010B-Apr21ReReco-v1-AOD'
userremotedir = '/Data/crab_Jet-Run2010B-Apr21ReReco-v1-AOD'
jsonfile = 'Intersection_Castor_GOOD_146240-148068.json'
lumis ='1'
lumisjob = '15'
runsel = '146240-148068'
lumixs = 'lumibylsXing_Cert_136033-149442_7TeV_Apr21ReReco_Collisions10_JSON.root'
sewhitelist = ''
output = 'JetPATTuple.root'
triggersmc = "'HLT_L1Jet6U','HLT_Jet15U'"
triggersdata = "'HLT_L1Jet6U','HLT_Jet15U'"
mail = 'dmf@cern.ch'
#ConfigHandler(dataset,uiworkingdir,userremotedir,jsonfile,lumis,lumisjob,runsel,lumixs,sewhitelist,output,triggersmc,triggersdata,mail)

## /Jet/Run2010B-Apr21ReReco-v1/AOD Other Range
dataset = '/Jet/Run2010B-Apr21ReReco-v1/AOD '  
uiworkingdir = 'crab_Jet-Run2010B-Apr21ReReco-v1-AOD-Other'
userremotedir = '/Data/crab_Jet-Run2010B-Apr21ReReco-v1-AOD-Other'
jsonfile = 'Intersection_Castor_GOOD_148791-149711.json'
lumis ='1'
lumisjob = '15'
runsel = '148791-149711'
lumixs = 'lumibylsXing_Cert_136033-149442_7TeV_Apr21ReReco_Collisions10_JSON.root'
sewhitelist = ''
output = 'JetPATTupleOther.root'
triggersmc = "'HLT_L1Jet6U_v3','HLT_Jet15U_v3','HLT_Jet30U_v3'"
triggersdata = "'HLT_L1Jet6U_v3','HLT_Jet15U_v3','HLT_Jet30U_v3'"
mail = 'dmf@cern.ch'
#ConfigHandler(dataset,uiworkingdir,userremotedir,jsonfile,lumis,lumisjob,runsel,lumixs,sewhitelist,output,triggersmc,triggersdata,mail)

## /MultiJet/Run2010B-Apr21ReReco-v1/AOD
dataset = '/MultiJet/Run2010B-Apr21ReReco-v1/AOD'  
uiworkingdir = 'crab_MultiJet_Run2010A_PATTuple'
userremotedir = '/Data/crab_MultiJet_Run2010A'
jsonfile = 'Intersection_Castor_GOOD_147146-149711.json'
lumis ='-1'
lumisjob = '15'
runsel = '147146-149442'
lumixs = 'lumibylsXing_Cert_136033-149442_7TeV_Apr21ReReco_Collisions10_JSON.root'
sewhitelist = ''
output = 'MultiJetPATTuple.root'
triggersmc = "'HLT_ExclDiJet30U_HFAND_v*','HLT_ExclDiJet30U_HFOR_v*','HLT_Jet30U*'"
triggersdata = "'HLT_ExclDiJet30U_HFAND_v*','HLT_ExclDiJet30U_HFOR_v*','HLT_Jet30U*'"
mail = 'dmf@cern.ch'
#ConfigHandler(dataset,uiworkingdir,userremotedir,jsonfile,lumis,lumisjob,runsel,lumixs,sewhitelist,output,triggersmc,triggersdata,mail)

## TEST
dataset = '/JetMETTau/Run2010A-Apr21ReReco-v1/AOD '  
uiworkingdir = 'crab_Test'
userremotedir = '/Data/Test'
jsonfile = 'Intersection_Castor_GOOD_135821-141887.json'
lumis ='1'
lumisjob = '1'
runsel = '135821-141887'
lumixs = 'lumibylsXing_Cert_136033-149442_7TeV_Apr21ReReco_Collisions10_JSON.root'
sewhitelist = ''
output = 'Test.root'
triggersmc = "'HLT_L1Jet6U','HLT_Jet15U','HLT_Jet30U','HLT_Jet50U'"
triggersdata = "'HLT_L1Jet6U','HLT_Jet15U','HLT_Jet30U','HLT_Jet50U'"
mail='dmf@cern.ch'
#ConfigHandler(dataset,uiworkingdir,userremotedir,jsonfile,lumis,lumisjob,runsel,lumixs,sewhitelist,output,triggersmc,triggersdata,mail)

#
# Samples 2011
#

## /MultiJet/Run2011A-PromptReco-v4/AOD 
dataset = '/MultiJet/Run2011A-PromptReco-v4/AOD'
uiworkingdir = 'crab_MultiJet_Run2011A-v4_PATTuple'
userremotedir = '/Data/crab_MultiJet_Run2011A-v4'
jsonfile = 'Intersection_Castor_GOOD_165071-168437.json'
lumis ='-1'
lumisjob = '15'
runsel = '165071-168437'
lumixs = 'lumibyXing_Cert_160404-176023_7TeV_PromptReco_Collisions11_JSON.root'
sewhitelist = ''
output = 'MultiJetPATTuple-v4.root'
triggersmc = "'HLT_ExclDiJet60U_HFAND_v*','HLT_ExclDiJet60U_HFOR_v*','HLT_Jet60U*'"
triggersdata = "'HLT_ExclDiJet60U_HFAND_v*','HLT_ExclDiJet60U_HFOR_v*','HLT_Jet60U*'"
mail = 'dmf@cern.ch'
#ConfigHandler(dataset,uiworkingdir,userremotedir,jsonfile,lumis,lumisjob,runsel,lumixs,sewhitelist,output,triggersmc,triggersdata,mail)

## /MultiJet/Run2011A-PromptReco-v5/AOD 
dataset = '/MultiJet/Run2011A-PromptReco-v5/AOD'
uiworkingdir = 'crab_MultiJet_Run2011A-v5_PATTuple'
userremotedir = '/Data/crab_MultiJet_Run2011A-v5'
jsonfile = 'Intersection_Castor_GOOD_160404-176023.json'
lumis ='-1'
lumisjob = '15'
runsel = '160404-176023'
lumixs = 'lumibyXing_Cert_160404-176023_7TeV_PromptReco_Collisions11_JSON.root'
sewhitelist = ''
output = 'MultiJetPATTuple-v5.root'
triggersmc = "'HLT_ExclDiJet60U_HFAND_v*','HLT_ExclDiJet60U_HFOR_v*','HLT_Jet60U*'"
triggersdata = "'HLT_ExclDiJet60U_HFAND_v*','HLT_ExclDiJet60U_HFOR_v*','HLT_Jet60U*'"
mail = 'dmf@cern.ch'
#ConfigHandler(dataset,uiworkingdir,userremotedir,jsonfile,lumis,lumisjob,runsel,lumixs,sewhitelist,output,triggersmc,triggersdata,mail)

## /MultiJet/Run2011A-PromptReco-v6/AOD 
dataset = '/MultiJet/Run2011A-PromptReco-v6/AOD'
uiworkingdir = 'crab_MultiJet_Run2011A-v6_PATTuple'
userremotedir = '/Data/crab_MultiJet_Run2011A-v6'
jsonfile = 'Intersection_Castor_GOOD_172620-175770.json'
lumis ='-1'
lumisjob = '15'
runsel = '172620-175770'
lumixs = 'lumibyXing_Cert_160404-176023_7TeV_PromptReco_Collisions11_JSON.root'
sewhitelist = ''
output = 'MultiJetPATTuple-v6.root'
triggersmc = "'HLT_ExclDiJet60U_HFAND_v*','HLT_ExclDiJet60U_HFOR_v*','HLT_Jet60U*'"
triggersdata = "'HLT_ExclDiJet60U_HFAND_v*','HLT_ExclDiJet60U_HFOR_v*','HLT_Jet60U*'"
mail = 'dmf@cern.ch'
#ConfigHandler(dataset,uiworkingdir,userremotedir,jsonfile,lumis,lumisjob,runsel,lumixs,sewhitelist,output,triggersmc,triggersdata,mail)

## TEST 
dataset = '/MultiJet/Run2011A-PromptReco-v6/AOD'
uiworkingdir = 'crab_MultiJet_Run2011A-v6_PATTuple'
userremotedir = '/Data/crab_MultiJet_Run2011A-v6'
jsonfile = 'Intersection_Castor_GOOD_172620-175770.json'
lumis ='1'
lumisjob = '1'
runsel = '172620-175770'
lumixs = 'lumibyXing_Cert_160404-176023_7TeV_PromptReco_Collisions11_JSON.root'
sewhitelist = ''
output = 'MultiJetPATTuple-v6.root'
triggersmc = "'HLT_ExclDiJet60U_HFAND_v*','HLT_ExclDiJet60U_HFOR_v*','HLT_Jet60U*'"
triggersdata = "'HLT_ExclDiJet60U_HFAND_v*','HLT_ExclDiJet60U_HFOR_v*','HLT_Jet60U*'"
mail = 'dmf@cern.ch'
ConfigHandler(dataset,uiworkingdir,userremotedir,jsonfile,lumis,lumisjob,runsel,lumixs,sewhitelist,output,triggersmc,triggersdata,mail)


print ''
print '@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@'
print '@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@'
print 'Finishing submission...'
print '@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@'
print '@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@'



