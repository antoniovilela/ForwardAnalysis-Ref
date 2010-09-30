"""
Using on top of CMSSW_3_6_X
JetMETAnalysis/HcalReflagging                    V00-00-16                     
RecoLocalCalo/HcalRecAlgos                       V00-07-11
"""

import FWCore.ParameterSet.Config as cms

def addHFReflagging(process,version,isMC):
    process.load("JetMETAnalysis/HcalReflagging/HFrechitreflaggerJETMET_cff")
    #version = 10
    #isMC = False
    if version==1:
        process.hfrecoReflagged = process.HFrechitreflaggerJETMETv1.clone()
    elif version==2:
        process.hfrecoReflagged = process.HFrechitreflaggerJETMETv2.clone()
    elif version==3:
        process.hfrecoReflagged = process.HFrechitreflaggerJETMETv3.clone()
    elif version==4:
        if (isMC==False):
            process.hfrecoReflagged = process.HFrechitreflaggerJETMETv4.clone()
        else:
            process.hfrecoReflagged = process.HFrechitreflaggerJETMETv2.clone()
    elif version==5:
        if (isMC==False):
            process.hfrecoReflagged = process.HFrechitreflaggerJETMETv5.clone()
        else:
            process.hfrecoReflagged = process.HFrechitreflaggerJETMETv3.clone()
    # CURRENT RECOMMENDATION
    elif version==10:
        process.hfrecoReflagged = process.HFrechitreflaggerJETMETv10.clone()
        # V10 cleaning uses results of prior flags when setting new flags; this is the current recommendation as of 21 July 2010 
        if (isMC==False):
            import string
            process.hfrecoReflagged.PETstat.flagsToSkip =string.atoi('10',2)
            process.hfrecoReflagged.S8S1stat.flagsToSkip=string.atoi('10010',2)
            process.hfrecoReflagged.S9S1stat.flagsToSkip=string.atoi('11010',2)
            # Flag ordering
            # set flag 4 (HFPET -- also sets HFLongShort), then flag 3 (HFS8S1 -- also sets HFLongShort), then flag 0 (HFLongShort -- set directly via S9S1)
            process.hfrecoReflagged.FlagsToSet=(4,3,0) 
    # Add debugging here
    #process.hfrecoReflagged.debug=1
    import JetMETAnalysis.HcalReflagging.RemoveAddSevLevel as RemoveAddSevLevel
    process.hcalRecAlgos=RemoveAddSevLevel.AddFlag(process.hcalRecAlgos,"HFLongShort",11)
    if (isMC==False):  # Don't use HFDigiTime on MC !
        process.hcalRecAlgos=RemoveAddSevLevel.AddFlag(process.hcalRecAlgos,"HFDigiTime",11)

    # Use the reflagged HF RecHits to make the CaloTowers
    process.towerMaker.hfInput = "hfrecoReflagged"
    process.towerMakerWithHO.hfInput = "hfrecoReflagged"

    return process

def addHBHEReflagging(process):
    process.load("JetMETAnalysis/HcalReflagging/isolhbherechitreflaggerJETMET_cfi")
    process.hbherecoReflagged = process.isolhbherechitreflaggerJETMET.clone()

    import JetMETAnalysis.HcalReflagging.RemoveAddSevLevel as RemoveAddSevLevel
    process.hcalRecAlgos=RemoveAddSevLevel.AddFlag(process.hcalRecAlgos,"UserDefinedBit0",10)
 
    # Use the reflagged HBHE RecHits to make the CaloTowers
    process.towerMaker.hbheInput = "hbherecoReflagged"
    process.towerMakerWithHO.hbheInput = "hbherecoReflagged"
 
    return process
