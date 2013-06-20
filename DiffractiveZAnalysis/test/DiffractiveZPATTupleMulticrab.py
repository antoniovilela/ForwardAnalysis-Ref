#flake8: noqa

'''
>>-------------------------<<-
Diffractive Z NTuple Producer
>>-------------------------<<

Goal:
Produce your diffractive Z ntuple. 

Usage:
    cmsRun DiffractiveZPATTupleMultiple.py

Example:
    cmsRun DiffractiveZPATTupleMulticrab.py Run=data_MuonA

Optional arguments:
    Run = data_MuonA, data_MuonB, data_ElectronA, data_ElectronB, MC_none, MC_PU, MC_FlatWeight or MC_FlatWeight_and_PU 

Authors: D. Figueiredo, R. Arciadiacono and N. Cartiglia
'''

import FWCore.ParameterSet.Config as cms
import os, sys
import atexit

from FWCore.ParameterSet.VarParsing import VarParsing
options = VarParsing ('analysis')
options.register('Run','MC_PU',VarParsing.multiplicity.singleton, VarParsing.varType.string,"Option to Run: data or MC.")
options.parseArguments()

process = cms.Process("Analysis")

class config: pass
config.verbose = True
config.writeEdmOutput = False
config.outputTTreeFile = 'DiffractiveZDataPATTuple.root'
config.runPATSequences = True
config.comEnergy = 7000.0
config.trackAnalyzerName = 'trackHistoAnalyzer'
config.trackTagName = 'analysisTracks'
config.NumberOfEvents = -1

#
# Define Options to Run
#
######################################################################################

if options.Run == "data_MuonA":
  print("")
  print("###################")
  print("Data Muon 2010 RunA")
  print("###################")
  print("")
  config.globalTagNameData = 'GR_R_42_V23::All'
  config.TriggerOn = True
  triggerlist = 'HLT_Mu9','HLT_L1Mu20'
  l1list = 'L1_ZeroBias','L1_SingleEG5'
  config.runOnMC = False
  config.runPUMC = False
  config.runGen = False

elif options.Run == "data_MuonB":
  print("")
  print("###################")
  print("Data Muon 2010 RunB")
  print("###################")
  print("")
  config.globalTagNameData = 'GR_R_42_V23::All'
  config.TriggerOn = True
  triggerlist = 'HLT_Mu9','HLT_L1Mu20'
  l1list = 'L1_ZeroBias','L1_SingleEG5'
  config.runOnMC = False
  config.runPUMC = False
  config.runGen = False

elif options.Run == "data_ElectronA":
  print("")
  print("#######################")
  print("Data Electron 2010 RunA")
  print("#######################")
  print("")
  config.globalTagNameData = 'GR_R_42_V23::All'
  config.TriggerOn = True
  triggerlist = 'HLT_Photon10_L1R','HLT_Photon15_Cleaned_l1R'
  l1list = 'L1_ZeroBias','L1_SingleEG5'
  config.runOnMC = False
  config.runPUMC = False
  config.runGen = False

elif options.Run == "data_ElectronB":
  print("")
  print("#######################")
  print("Data Electron 2010 RunB")
  print("#######################")
  print("")
  config.globalTagNameData = 'GR_R_42_V23::All'
  config.TriggerOn = True
  triggerlist = 'HLT_Ele15_SW_CaloEleId_L1R','HLT_Ele17_SW_CaloEleId_L1R','HLT_Ele17_SW_TightEleId_L1R' 
  l1list = 'L1_ZeroBias','L1_SingleEG5'
  config.runOnMC = False
  config.runPUMC = False
  config.runGen = False

elif options.Run == "MC_FlatWeight_and_PU":
  print("")
  print("#####################")
  print("MC Flat Weight and PU")
  print("#####################")
  print("")
  config.globalTagNameMC = 'START42_V17D::All'
  config.TriggerOn = False
  triggerlist = 'no_trigger','no_trigger'
  l1list = 'no_trigger','no_trigger'
  config.runOnMC = True
  config.runPUMC = True
  config.runGen = True

elif options.Run == "MC_FlatWeight":
  print("")
  print("##############")
  print("MC Flat Weight")
  print("##############")
  print("")
  config.globalTagNameMC = 'START42_V17D::All'
  config.TriggerOn = False
  triggerlist = 'no_trigger','no_trigger'
  l1list = 'no_trigger','no_trigger'
  config.runOnMC = True
  config.runPUMC = False
  config.runGen = True

elif options.Run == "MC_PU":
  print("")
  print("#####")
  print("MC PU")
  print("#####")
  print("")
  config.globalTagNameMC = 'START42_V17D::All'
  config.TriggerOn = False
  triggerlist = 'no_trigger','no_trigger'
  l1list = 'no_trigger','no_trigger'
  config.runOnMC = True
  config.runPUMC = True
  config.runGen = False

elif options.Run == "MC_none":
  print("")
  print("#######")
  print("MC None")
  print("#######")
  print("")
  config.globalTagNameMC = 'START42_V17D::All'
  config.TriggerOn = False
  triggerlist = 'no_trigger','no_trigger'
  l1list = 'no_trigger','no_trigger'
  config.runOnMC = True
  config.runPUMC = False
  config.runGen = False

else:
  print("")
  print("") 
  raise RuntimeError, "Unknown option. EXIT! YOU NEED TO SETUP WITH ONE OF THE CORRECT OPTIONS."
  print("")


print("")
print(">>> Input Options:")
print("Run with MC: %s" % config.runOnMC)
print("Run MC with Pile Up: %s" % config.runPUMC)
print("Run MC with Flat Weight: %s" % config.runGen)
print("Run with Trigger: %s" % config.TriggerOn)
if not config.runOnMC: 
   print("Data Global Tag: " + config.globalTagNameData)
else:
   print("MC Global Tag: " + config.globalTagNameMC)  
print("")

#
# Define Triggers and Input Files
#
######################################################################################

if config.runOnMC:
    config.l1Paths = (l1list)
    config.hltPaths =(triggerlist)
    config.inputFileName = '/storage1/dmf/TestSamples/PYTHIA6_QCD_15to3000_private_SL_RECO/QCD_Pt_15to3000_TuneZ2_Flat_7TeV_pythia6_cff_py_RAW2DIGI_L1Reco_RECO_233_3_nsm.root'

else:
    config.l1Paths = (l1list)
    config.hltPaths = (triggerlist)
    config.inputFileName = '/storage1/dmf/TestSamples/MuRun2010/MuRunA2010.root'

#
# CMSSW Main Code
#
######################################################################################

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
######################################################################################

process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string(config.outputTTreeFile))

#
# Detector Conditions and Scales
#
######################################################################################

process.load('Configuration.StandardSequences.Services_cff')
process.load('Configuration.StandardSequences.GeometryExtended_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.Reconstruction_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

#
# Global Tag Input
#
######################################################################################

if config.runOnMC: process.GlobalTag.globaltag = config.globalTagNameMC
else: process.GlobalTag.globaltag = config.globalTagNameData

process.load('JetMETCorrections.Configuration.DefaultJEC_cff')
process.load('RecoJets.Configuration.RecoPFJets_cff')
process.load('RecoJets.Configuration.RecoJets_cff')
process.load('CommonTools/RecoAlgos/HBHENoiseFilterResultProducer_cfi')

#
# PAT Sequences
#
######################################################################################

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
# Remove PAT MCMatching for Data
#
######################################################################################

# load the standard PAT config
process.load("PhysicsTools.PatAlgos.patSequences_cff")

# load the coreTools of PAT
from PhysicsTools.PatAlgos.tools.coreTools import *

#if not config.runOnMC: 
#    removeMCMatching(process, ['All'])

#
# PAT Muons and Electrons WorkFlow
#
######################################################################################

from PhysicsTools.PatAlgos.mcMatchLayer0.electronMatch_cfi import *
from TrackingTools.TransientTrack.TransientTrackBuilder_cfi import *
from PhysicsTools.PatAlgos.producersLayer1.electronProducer_cfi import *

from PhysicsTools.PatAlgos.mcMatchLayer0.muonMatch_cfi import *
from TrackingTools.TransientTrack.TransientTrackBuilder_cfi import *
from PhysicsTools.PatAlgos.producersLayer1.muonProducer_cfi import *

#process.patElectrons.pvSrc="goodOfflinePrimaryVertices"
#process.patMuons.pvSrc="goodOfflinePrimaryVertices"

if not config.runOnMC:

    process.makePatElectrons = cms.Sequence(
        patElectrons
    )

    process.makePatMuons = cms.Sequence(
         patMuons
    )

else: 
    process.makePatElectrons = cms.Sequence(
         electronMatch *
         patElectrons
    )
   
    process.makePatMuons = cms.Sequence(
         muonMatch*
         patMuons
    )

#
# PAT Isolation Variables
#
######################################################################################

#from PhysicsTools.PatAlgos.tools.muonTools import *
#addMuonUserIsolation(process)

#from PhysicsTools.PatAlgos.tools.electronTools import *
#addElectronUserIsolation(process)

#
# Open Common Modules
#
######################################################################################

process.load("ForwardAnalysis.AnalysisSequences.CommonModulesSequences_cff")
process.pfCandidateNoiseThresholds.src = "pfNoPileUpPFlow"
process.tracksTransverseRegion.JetTag = "selectedPatJetsPFlow"

#
# Import PSET for each Module
#
######################################################################################

from ForwardAnalysis.ForwardTTreeAnalysis.DiffractiveAnalysis_cfi import DiffractiveAnalysis
from ForwardAnalysis.DiffractiveZAnalysis.DiffractiveZAnalysis_cfi import DiffractiveZAnalysis
from ForwardAnalysis.ForwardTTreeAnalysis.PATTriggerInfo_cfi import PATTriggerInfo

#PATTriggerInfo.L1AlgoBitName =  config.l1Paths 
PATTriggerInfo.HLTAlgoBitName = config.hltPaths
PATTriggerInfo.runALLTriggerPath = True

#
# Define Analyzers
#
######################################################################################

process.diffractiveZAnalysisTTree = cms.EDAnalyzer("EventInfoDiffractiveDiffractiveZAnalysisTTree",
        EventInfo = cms.PSet(
                    RunOnData = cms.untracked.bool(not config.runOnMC),
                    RunWithMCPU = cms.untracked.bool(config.runPUMC),
                    RunWithWeightGen = cms.untracked.bool(config.runGen)
        ),
        DiffractiveAnalysis = DiffractiveAnalysis,
        DiffractiveZAnalysis = DiffractiveZAnalysis
        )

process.CommonHLTFilter.HLTPaths = config.hltPaths 

process.diffractiveZAnalysisTTree.DiffractiveAnalysis.hltPath = ''
process.diffractiveZAnalysisTTree.DiffractiveAnalysis.trackTag = 'analysisTracks'
process.diffractiveZAnalysisTTree.DiffractiveAnalysis.vertexTag = "goodOfflinePrimaryVertices"
process.diffractiveZAnalysisTTree.DiffractiveAnalysis.particleFlowTag = "pfCandidateNoiseThresholds"
process.diffractiveZAnalysisTTree.DiffractiveAnalysis.jetTag = "selectedPatJetsPFlow"

process.diffractiveZAnalysisTTree.DiffractiveZAnalysis.hltPaths = config.hltPaths
if config.runOnMC:
     process.diffractiveZAnalysisTTree.DiffractiveZAnalysis.RunMC = True
     process.diffractiveZAnalysisTTree.DiffractiveZAnalysis.fCGeVCastor = 0.9375
else:
     process.diffractiveZAnalysisTTree.DiffractiveZAnalysis.RunMC = False
     process.diffractiveZAnalysisTTree.DiffractiveZAnalysis.fCGeVCastor = 0.015 

#
# Define MC Access
#
######################################################################################

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
######################################################################################

process.pat_Producer = cms.Path(process.makePatElectrons + process.makePatMuons)
process.analysis_reco_step = cms.Path(process.analysisSequences)
process.castor_step = cms.Path(process.castorSequence)


if config.TriggerOn:
    process.analysis_diffractiveDiffractiveZAnalysisPATTriggerInfoTTree_step = cms.Path(
    process.eventSelectionHLT + process.diffractiveZAnalysisTTree)

else:
    process.analysis_diffractiveDiffractiveZAnalysisPATTriggerInfoTTree_step = cms.Path(
    process.eventSelection + process.diffractiveZAnalysisTTree)

