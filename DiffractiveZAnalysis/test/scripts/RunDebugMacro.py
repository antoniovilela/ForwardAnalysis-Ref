#!/usr/bin/env python
import os

print ''
print '@@@@@@@@@@@@'
print 'Run Analysis'
print '@@@@@@@@@@@@'
print ''

#
# Electron2010B
#

#'HLT_Mu0_L1MuOpen','HLT_Mu3','HLT_Mu5','HLT_DoubleMu0'

os.system("./DiffractiveZ \"/storage1/dmf/Samples/PattuplesHF7/May_2013/MuRunA2010_DQC/MuRun2010_DQC_complete.root\" \"diffractiveZAnalysisTTree\" \"histo_muon2010A_HLT_Mu0_L1MuOpen.root\" \"trigger\" 0 15.0 5.0 1 \"no_multiple_pileup\" \"no_mc_lumi_weight\" 1.0 \"RecoMuon\"")

print ''
print '@@@@@@@@@@@@@@@@@@'
print 'Finishing Analysis'
print '@@@@@@@@@@@@@@@@@@'
print ''
