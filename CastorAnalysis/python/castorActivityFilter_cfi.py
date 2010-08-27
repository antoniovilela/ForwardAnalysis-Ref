import FWCore.ParameterSet.Config as cms

castorActivity =  cms.EDFilter("CastorActivityFilter",
                       firstTS = cms.untracked.int32(4),
                       lastTS = cms.untracked.int32(6),
                       totalEnergyThreshold = cms.untracked.double(300.)
                       )
