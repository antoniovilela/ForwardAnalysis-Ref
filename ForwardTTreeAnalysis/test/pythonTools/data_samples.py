#!/usr/bin/env python
import os



# Creating Configuration file for Data
#----------------------------------------
def ConfigHandlerData(inputfile,outputfile,triggers,addition,uiworkingdir):
    from pset_data import data_py
    data_py = data_py.replace('@@TRIGGERS@@',triggers,-1)
    data_py = data_py.replace('@@INPUT@@',inputfile,-1)
    data_py = data_py.replace('@@OUTPUT@@',outputfile,-1)
    data_py = data_py.replace('@@ADDITIONALFILE@@',addition,-1)
    data_pyFile = open('pset.py','a+')
    data_pyFile.write(data_py)
    data_pyFile.close()
    if os.path.isfile('pset.py'):
       os.system('mkdir -p '+uiworkingdir+'/Backup')
       os.system('cp pset.py '+uiworkingdir+'/Backup/.')
       os.system('cmsRun '+uiworkingdir+'/Backup/pset.py')
       os.system('rm pset.py')
##---------------------------------------


inputfile = '/storage2/antoniov/data1/MultiJet_Run2010B_Apr21ReReco-v1_AOD/MultiJet_Run2010B_Apr21ReReco-v1_AOD_7EA7B611-7371-E011-B164-002354EF3BDB.root'
outputfile = 'teste.root'
additional = '/storage2/eliza/lumibyXing_Cert_160404-176023_7TeV_PromptReco_Collisions11_JSON.root'
triggers = "'HLT_Jet30*','HLT_Jet60*','HLT_Jet80*','HLT_Jet110*','HLT_Jet150*','HLT_Jet190*','HLT_Jet240_v*','HLT_Jet370_v*'"
addition = '/storage2/eliza/lumibyXing_Cert_160404-176023_7TeV_PromptReco_Collisions11_JSON.root'
uiworkingdir = 'DataTest'
ConfigHandlerData(inputfile,outputfile,triggers,addition,uiworkingdir)
