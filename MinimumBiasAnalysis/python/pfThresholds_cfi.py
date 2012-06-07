import FWCore.ParameterSet.Config as cms

pfThresholds = cms.PSet(
    Barrel = cms.PSet(
        hadronCharged = cms.PSet( pt = cms.double(0.0) ),
        hadronNeutral = cms.PSet( energy = cms.double(1.4) ),
        electron = cms.PSet( pt = cms.double(0.0) ),
        gamma = cms.PSet( energy = cms.double(0.9) )
    ),
    Endcap = cms.PSet(
        hadronCharged = cms.PSet( pt = cms.double(0.0) ),
        hadronNeutral = cms.PSet( energy = cms.double(2.7) ),
        electron = cms.PSet( pt = cms.double(0.0) ),
        gamma = cms.PSet( energy = cms.double(2.5) )
    ),
    Transition = cms.PSet(
        hadronCharged = cms.PSet( pt = cms.double(0.0) ),
        hadronNeutral = cms.PSet( energy = cms.double(3.8) ),
        electron = cms.PSet( pt = cms.double(0.0) ),
        gamma = cms.PSet( energy = cms.double(2.5) ),
        hadronHF = cms.PSet( energy = cms.double(4.0) ),
        emHF = cms.PSet( energy = cms.double(3.5) )
    ),
    Forward = cms.PSet(
        hadronHF = cms.PSet( energy = cms.double(4.0) ),
        emHF = cms.PSet( energy = cms.double(3.5) )
    )
)
