#!/usr/bin/env python
import os

print ''
print '@@@@@@@@@@@@'
print 'Run Analysis'
print '@@@@@@@@@@@@'
print ''

os.system("./PurityCom \"/storage1/eliza/PATTuples_Mar2013/data_Multijets/data_total/data_MultiJetRB2010_0.root\" \"histo_purity_nopresel_jet2_pT10.root\" \"exclusiveDijetsAnalysisTTree/ProcessedTree\" 1 4 0 1 1")

#----------------------------------------------------------->>>

print ''
print '@@@@@@@@@@@@@@@@@@'
print 'Finishing Analysis'
print '@@@@@@@@@@@@@@@@@@'
print ''
