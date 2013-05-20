#!/usr/bin/env python
import os

print ''
print '@@@@@@@@@@@@'
print 'Run Analysis'
print '@@@@@@@@@@@@'
print ''


#
# PowHeg Z
#

#os.system("./DiffZ \"/storage1/dmf/Samples/PattuplesHF7/January_2013/PATTuplePowHegZ.root\" \"diffractiveZAnalysisTTree/ProcessedTree\" \"histo_powhegZ.root\" 0 0 1 30 15 1 0 1.0 1.0")

#
# Pythia6 DYtoEE
#

#os.system("./DiffZ \"/storage1/dmf/Samples/PattuplesHF7/January_2013/PATTupleDyToEE.root\" \"diffractiveZAnalysisTTree/ProcessedTree\" \"histo_DYToEE.root\" 0 1 1 30 15 1 0 1.0 1.0")


#
# PowHeg Z
#

os.system("./DiffZ \"/storage1/dmf/Samples/PattuplesHF7/May_2013/PowHegZ/PowHegZ_0.root\" \"diffractiveZAnalysisTTree/ProcessedTree\" \"histo_powhegZ.root\" 0 0 1 30 15 1 0 1.0 1.0")

#
# Pythia6 DYtoEE
#

os.system("./DiffZ \"/storage1/dmf/Samples/PattuplesHF7/May_2013/DYtoMuMu/DYtoMuMu_0.root\" \"diffractiveZAnalysisTTree/ProcessedTree\" \"histo_DYToMuMu.root\" 0 1 1 30 15 1 0 1.0 1.0")


print ''
print '@@@@@@@@@@@@@@@@@@'
print 'Finishing Analysis'
print '@@@@@@@@@@@@@@@@@@'
print ''
