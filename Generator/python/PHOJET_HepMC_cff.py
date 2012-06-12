import FWCore.ParameterSet.Config as cms

generator = cms.EDProducer("MCFileReader",
	fileNames = cms.untracked.vstring('phojet.hepmc')
)
