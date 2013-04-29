#!/usr/bin/env python
import os

print ''
print '@@@@@@@@@@@@'
print 'Run Analysis'
print '@@@@@@@@@@@@'
print ''

os.system("./CastorThreshold \"/storage1/dmf/Samples/PattuplesHF7/March_2013/ZeroBias_HF7_april13_ZeroBias_Plus_Minus/ZeroBias_13april.root\" \"histo_castor_collisionszerobias.root\" \"exclusiveDijetsAnalysisZeroBiasTTree\" \"collisions\"")

os.system("./CastorThreshold \"/storage1/dmf/Samples/PattuplesHF7/March_2013/ZeroBias_HF7_april13_ZeroBias_Plus_Minus/ZeroBias_13april.root\" \"histo_castor_unpairedzerobias.root\" \"exclusiveDijetsAnalysisZeroBiasTTree\" \"unpaired\"")

os.system("./CastorThreshold \"/storage1/eliza/PATTuples_Mar2013/mc_PYTHIA/mc_pythia_nopu_0.root\" \"histo_castor_collisionspythia.root\" \"exclusiveDijetsAnalysisTTree\" \"collisionsmc\"")

os.system("./CastorThreshold \"/storage1/eliza/PATTuples_Mar2013/mc_PYTHIA/mc_pythia_nopu_0.root\" \"histo_castor_unpairedpythia.root\" \"exclusiveDijetsAnalysisTTree\" \"unpairedmc\"")

os.system("./CastorThreshold \"/storage1/eliza/PATTuples_Mar2013/mc_POMPYT/mc_pompyt_minus_0.root\" \"histo_castor_collisionspompytminus.root\" \"exclusiveDijetsAnalysisTTree\" \"collisionsmc\"")

os.system("./CastorThreshold \"/storage1/eliza/PATTuples_Mar2013/mc_POMPYT/mc_pompyt_minus_0.root\" \"histo_castor_unpairedpompytminus.root\" \"exclusiveDijetsAnalysisTTree\" \"unpairedmc\"")

print ''
print '@@@@@@@@@@@@@@@@@@'
print 'Finishing Analysis'
print '@@@@@@@@@@@@@@@@@@'
print ''
