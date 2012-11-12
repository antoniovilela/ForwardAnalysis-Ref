import FWCore.ParameterSet.Config as cms

from FWCore.MessageLogger.MessageLogger_cfi import *

options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring()
)

TFileService = cms.Service("TFileService",
    fileName = cms.string('')
)
