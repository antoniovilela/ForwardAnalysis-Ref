import FWCore.ParameterSet.Config as cms

process = cms.Process("Skim")

process.load('FWCore.MessageService.MessageLogger_cfi')
process.MessageLogger.cerr.threshold = 'INFO'

process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(5000) )

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        'file:/tmp/sheila/JetMETTau_Jul16thReReco-v1_RECO_9C702CC7-3291-DF11-BF3F-0018F3D096C0.root'
    )
)

# Import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('Configuration.StandardSequences.GeometryExtended_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.Reconstruction_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.load("Configuration.EventContent.EventContent_cff")

#process.leadingJets = cms.EDProducer("LargestPtCandViewSelector",
#      src = cms.InputTag("ak5PFJets"),
#      maxNumber = cms.uint32(1)
#)

process.forwardJets = cms.EDFilter("CandViewSelector",
    src = cms.InputTag("ak5PFJets"),
    cut = cms.string("abs(eta) > 2.5 && pt > 20."),
    filter = cms.bool(True)
)

process.GlobalTag.globaltag = 'GR_R_36X_V12A::All'

###################################################################################
# HCAL reflagging
# HF RecHit reflagger -- specify type of HF cleaning to use
from ForwardAnalysis.Utilities.addHcalReflagging import addHFReflagging,addHBHEReflagging
process = addHFReflagging(process,version=10,isMC=False)
# HBHE RecHit reflagger
process = addHBHEReflagging(process)

print "These are the severity levels for the various rechit flags:"
print "(Severity > 10 causes rechit to be ignored by CaloTower maker)"
for i in process.hcalRecAlgos.SeverityLevels: print i

# Use the reflagged HF RecHits to make the CaloTowers
process.towerMaker.hfInput = "hfrecoReflagged"
process.towerMakerWithHO.hfInput = "hfrecoReflagged"
# Use the reflagged HBHE RecHits to make the CaloTowers
process.towerMaker.hbheInput = "hbherecoReflagged"
process.towerMakerWithHO.hbheInput = "hbherecoReflagged"

###################################################################################


process.output = cms.OutputModule("PoolOutputModule",
    process.AODEventContent,
    fileName = cms.untracked.string('fwdJets_skim.root'),
    SelectEvents = cms.untracked.PSet(
        dataTier = cms.untracked.string('SKIM'),
        SelectEvents = cms.vstring('selection_step')
    )
)

# Reflagging and re-reco
process.reflagging_step = cms.Path(process.hfrecoReflagged+process.hbherecoReflagged)
process.rereco_step = cms.Path(process.caloTowersRec
                               *(process.recoJets*process.recoJetIds+process.recoTrackJets)
                               *process.recoJetAssociations
                               *process.metreco
                               ) # re-reco jets and met

#process.selection_step = cms.Path(process.leadingJets*process.forwardJets)
process.selection_step = cms.Path(process.forwardJets)
process.out_step = cms.EndPath(process.output)
