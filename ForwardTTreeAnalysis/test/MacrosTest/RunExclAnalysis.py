#!/usr/bin/env python
import os

print ''
print '@@@@@@@@@@@@'
print 'Run Analysis'
print '@@@@@@@@@@@@'
print ''

#  root -l -b -q 'ExclDijets.C("inputfile.root","outputfile.root", pT(Jet1) Cut, pT(Jet2) Cut, Number of Vertex Cut, Trigger Option, Turn on(off) PU Reweight, Turn on(off) Luminosity Reweight, Turn on(off) Trigger Efficiency, Turn on(off) event-per-event Weight, Luminosity Weight Factor, Trigger Efficiency Factor)'
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

############
# MonteCarlo
############

# < EXUME > ------------------------------------------------>>>

os.system("root -l -b -q 'ExclDijets.C(\"/storage2/dmf/Common/OutputFiles/MCExume/signal_exume.root\",\"histo_exume_HLT_excl_30Uand.root\",60,60,1,0,false,true,false,false,0.074335749235474,1.)'")
os.system("root -l -b -q 'ExclDijets.C(\"/storage2/dmf/Common/OutputFiles/MCExume/signal_exume.root\",\"histo_exume_HLT_excl_30Uor.root\",60,60,1,1,false,true,false,false,0.031881325178389,1.)'")
os.system("root -l -b -q 'ExclDijets.C(\"/storage2/dmf/Common/OutputFiles/MCExume/signal_exume.root\",\"histo_exume_HLT_jet30U.root\",60,60,1,2,false,true,false,false,0.000061668063201,1.)'")
#----------------------------------------------------------->>>

# < POMPTY > ------------------------------------------------>>>

# -- PLUS --
os.system("root -l -b -q 'ExclDijets.C(\"/storage2/dmf/Common/OutputFiles/MCPompty/pomptyPlusAll.root\",\"histo_pompty_plus_HLT_excl_30Uand.root\",60,60,1,0,false,true,false,false,104.22112155,1.)'")
os.system("root -l -b -q 'ExclDijets.C(\"/storage2/dmf/Common/OutputFiles/MCPompty/pomptyPlusAll.root\",\"histo_pompty_plus_HLT_excl_30Uor.root\",60,60,1,1,false,true,false,false,44.6986477,1.)'")
os.system("root -l -b -q 'ExclDijets.C(\"/storage2/dmf/Common/OutputFiles/MCPompty/pomptyPlusAll.root\",\"histo_pompty_plus_HLT_jet30U.root\",60,60,1,2,false,true,false,false,0.08646061655,1.)'")

# -- MINUS --
os.system("root -l -b -q 'ExclDijets.C(\"/storage2/dmf/Common/OutputFiles/MCPompty/pomptyMinusAll.root\",\"histo_pompty_minus_HLT_excl_30Uand.root\",60,60,1,0,false,true,false,false,104.22112155,1.)'")
os.system("root -l -b -q 'ExclDijets.C(\"/storage2/dmf/Common/OutputFiles/MCPompty/pomptyMinusAll.root\",\"histo_pompty_minus_HLT_excl_30Uor.root\",60,60,1,1,false,true,false,false,44.6986477,1.)'")
os.system("root -l -b -q 'ExclDijets.C(\"/storage2/dmf/Common/OutputFiles/MCPompty/pomptyMinusAll.root\",\"histo_pompty_minus_HLT_jet30U.root\",60,60,1,2,false,true,false,false,0.08646061655,1.)'")
#----------------------------------------------------------->>>

# < PYTHIA > ------------------------------------------------>>>

#
# event-per-event Weight = TRUE
# PU Reweight = TRUE
#

os.system("root -l -b -q 'ExclDijets.C(\"/storage2/dmf/Common/OutputFiles/MCPythia6_third/QCD_Pt-15to3000-Z2_PUS3.root\",\"histo_pythia_HLT_excl_30Uand.root\",60,60,1,0,true,true,false,true,57656.8187919143,1.)'")
os.system("root -l -b -q 'ExclDijets.C(\"/storage2/dmf/Common/OutputFiles/MCPythia6_third/QCD_Pt-15to3000-Z2_PUS3.root\",\"histo_pythia_HLT_excl_30Uor.root\",60,60,1,1,true,true,false,true,24728.0185854277,1.)'")
os.system("root -l -b -q 'ExclDijets.C(\"/storage2/dmf/Common/OutputFiles/MCPythia6_third/QCD_Pt-15to3000-Z2_PUS3.root\",\"histo_pythia_HLT_jet30U.root\",60,60,1,2,true,true,false,true,47.8314186886673,1.)'")
#----------------------------------------------------------->>>

print ''
print '@@@@@@@@@@@@@@@@@@'
print 'Finishing Analysis'
print '@@@@@@@@@@@@@@@@@@'
print ''


