#!/usr/bin/env python
import os

print ''
print '@@@@@@@@@@@@'
print 'Run Analysis'
print '@@@@@@@@@@@@'
print ''

#  root -l -b -q 'ExclDijets.C("inputfile.root","outputfile.root", pT(Jet1) Cut, pT(Jet2) Cut, Number of Vertex Cut, Trigger Option, Turn on(off) PU Reweight, Turn on(off) Luminosity Reweight, Turn on(off) Trigger Efficiency, Turn on(off) event-per-event Weight, Luminosity Weight, Trigger Efficiency)'
#
# TURN ON  = true
# TURN OFF = false
#
# I)   If you turn off PU reweight, the default weight will be 1;
# II)  If you turn off Luminosity reweight, the default weight will be 1;
# III) If you turn off Trigger Efficiency, the default weight will be 1;
# IV)  If you turn off event-per-event weight (some MC sample), the default weight will be 1;
#
# P.S.: Trigger Option. How to use?
#
#       from CMSSW CONFIG FILE:
#
#       config.hltPaths = ('HLT_ExclDiJet60_HFOR_v*','HLT_ExclDiJet60_HFAND_v*','HLT_Jet60_v*' )
#
# so the if you would like to use HLT_ExclDiJet60_HFAND_v*, the Trigger Option = 1.
#        if you would like to use HLT_Jet60_v*, the Trigger Option = 2.
#

os.system("root -l -b -q 'ExclDijets.C(\"/storage2/dmf/phDAnalysis/DPE_Exclusive/CMSSW_4_2_8/src/ForwardAnalysis/ForwardTTreeAnalysis/test/crab_QCD_Pt-15to3000-Z2/res/QCD_Pt-15to3000-Z2_407_1_tP5.root\",\"teste.root\",60,60,1,0,false,true,false,true,2.,3.)'")

print ''
print '@@@@@@@@@@@@@@@@@@'
print 'Finishing Analysis'
print '@@@@@@@@@@@@@@@@@@'
print ''


