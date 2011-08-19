'''

import FWCore.ParameterSet.Config as cms

pfThresholds = cms.PSet(
    hadronCharged = cms.PSet(
        ptMin = cms.double(0.5),
        energyMin = cms.double(0.0) 
    ),
    hadronNeutral = cms.PSet(
        ptMin = cms.double(0.0),
        energyMin = cms.double(2.0)
    ),
    electron = cms.PSet(
        ptMin = cms.double(0.5),
        energyMin = cms.double(0.0)
    ),
    gamma = cms.PSet(
        ptMin = cms.double(0.0),
        energyMin = cms.double(2.0) 
    ),
    hadronHF = cms.PSet(
        ptMin = cms.double(0.0),
        energyMin = cms.double(4.0)
    ),
    emHF = cms.PSet(
        ptMin = cms.double(0.0),
        energyMin = cms.double(4.0)
    )
)
'''
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

