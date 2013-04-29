#!/usr/bin/env python
import os

print ''
print '@@@@@@@@@@@@'
print 'Run Analysis'
print '@@@@@@@@@@@@'
print ''

os.system("./EffMacroCom \"/storage1/dmf/Samples/PattuplesHF7/March_2013/ZeroBias_HF7_SL/zerobias_pf_hf7_march2013.root\" \"histo_effCutsMinBias2010RunB_castor.root\" \"exclusiveDijetsAnalysisZeroBiasTTree/ProcessedTree\" 1 0 1 1 1 1")

#----------------------------------------------------------->>>

print ''
print '@@@@@@@@@@@@@@@@@@'
print 'Finishing Analysis'
print '@@@@@@@@@@@@@@@@@@'
print ''
