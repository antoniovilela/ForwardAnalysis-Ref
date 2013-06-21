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

os.system("./DiffractiveZ \"/storage1/dmf/Samples/PattuplesHF7/May_2013/DYtoMuMu/DYtoMuMu_0.root\" \"diffractiveZAnalysisTTree\" \"histo_MC.root\" \"no_trigger\" 0 15.0 5.0 1 \"multiple_pileup\" \"mc_lumi_weight\" 10.1 \"RecoMuon\"")

print ''
print '@@@@@@@@@@@@@@@@@@'
print 'Finishing Analysis'
print '@@@@@@@@@@@@@@@@@@'
print ''
