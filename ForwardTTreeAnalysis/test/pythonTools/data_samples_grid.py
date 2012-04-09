#!/usr/bin/env python
import os



# Creating Configuration file for Data
#----------------------------------------
def ConfigHandlerDataGrid(triggers,inputfile,outputfile,addition,useserver,dataset,jsonfile,lumis,lumisjob,runsel,uiworkingdir,mail,usesavelocal,usedbsin,usedbsout,usese,
useadditional,sewhitelist,seblacklist,dbsin,dbsout,seelement,storagepath,sesave,publishname,port):
    from pset_data import data_py
    data_py = data_py.replace('@@TRIGGERS@@',triggers,-1)
    data_py = data_py.replace('@@INPUT@@',inputfile,-1)
    data_py = data_py.replace('@@OUTPUT@@',outputfile,-1)
    data_py = data_py.replace('@@ADDITIONALFILE@@',addition,-1)
    data_pyFile = open('pset.py','a+')
    data_pyFile.write(data_py)
    data_pyFile.close()
    from crab_data import cdata_cfg
    cdata_cfg = cdata_cfg.replace('@@ADDITIONALFILE@@',addition,-1)
    cdata_cfg = cdata_cfg.replace('@@USESERVER@@',useserver,-1)
    cdata_cfg = cdata_cfg.replace('@@DATASET@@',dataset,-1)
    cdata_cfg = cdata_cfg.replace('@@OUTPUT@@',outputfile,-1)
    cdata_cfg = cdata_cfg.replace('@@JSONFILE@@',jsonfile,-1)
    cdata_cfg = cdata_cfg.replace('@@LUMIS@@',lumis,-1)
    cdata_cfg = cdata_cfg.replace('@@LUMISJOB@@',lumisjob,-1)
    cdata_cfg = cdata_cfg.replace('@@RUNSEL@@',runsel,-1)
    cdata_cfg = cdata_cfg.replace('@@UIWORKINGDIR@@',uiworkingdir,-1)
    cdata_cfg = cdata_cfg.replace('@@USESAVELOCAL@@',usesavelocal,-1)
    cdata_cfg = cdata_cfg.replace('@@USEDBSIN@@',usedbsin,-1)
    cdata_cfg = cdata_cfg.replace('@@DBSSERVERIN@@',dbsin,-1)
    cdata_cfg = cdata_cfg.replace('@@USEDBSOUT@@',usedbsout,-1)
    cdata_cfg = cdata_cfg.replace('@@DBSSERVEROUT@@',dbsout,-1)
    cdata_cfg = cdata_cfg.replace('@@USESE_SAVE@@',usese,-1)
    cdata_cfg = cdata_cfg.replace('@@SEELEMENT@@',seelement,-1)
    cdata_cfg = cdata_cfg.replace('@@STORAGEPATH@@',storagepath,-1)
    cdata_cfg = cdata_cfg.replace('@@SE_SAVE@@',sesave,-1)
    cdata_cfg = cdata_cfg.replace('@@PUBLISHNAME@@',publishname,-1)
    cdata_cfg = cdata_cfg.replace('@@PORT@@',port,-1)
    cdata_cfg = cdata_cfg.replace('@@USEADDITIONALFILE@@',useadditional,-1)
    cdata_cfg = cdata_cfg.replace('@@SEBLACKLIST@@',seblacklist,-1)
    cdata_cfg = cdata_cfg.replace('@@SEWHITELIST@@',sewhitelist,-1)
    if (sewhitelist == ''):
        cdata_cfg = cdata_cfg.replace('@@USESEWHITELIST@@','#',-1)
    else:
        cdata_cfg = cdata_cfg.replace('@@USESEWHITELIST@@','',-1)
    if (seblacklist == ''):
        cdata_cfg = cdata_cfg.replace('@@USESEBLACKLIST@@','#',-1)
    else:
        cdata_cfg = cdata_cfg.replace('@@USESEBLACKLIST@@','',-1)
    cdata_cfg = cdata_cfg.replace('@@MAIL@@',mail,-1)
    cdata_cfgFile = open('crab.cfg','a+')
    cdata_cfgFile.write(cdata_cfg)
    cdata_cfgFile.close()
    if os.path.isfile('crab.cfg'):
       os.system('crab -create')
       os.system('cp crab.cfg '+uiworkingdir+'/.')
       #os.system('crab -submit -c '+ uiworkingdir)
       os.system('rm crab.cfg')
    if os.path.isfile('pset.py'):
       os.system('cp pset.py '+uiworkingdir+'/.')
       os.system('rm pset.py')
##---------------------------------------



## /Jet/Run2010B-Apr21ReReco-v1/AOD 
addition = 'lumibylsXing_Cert_136033-149442_7TeV_Apr21ReReco_Collisions10_JSON.root'
useserver = ''
dataset = '/Jet/Run2010B-Apr21ReReco-v1/AOD'
uiworkingdir = 'crab_Jet-Run2010B-Apr21ReReco-v1-AOD'
jsonfile = 'Intersection_Castor_GOOD_146240-148068.json'
lumis ='-1'
lumisjob = '15'
runsel = '147196-148058'
inputfile = ''
outputfile = 'JetPATTuple.root'
triggers = "'HLT_Jet15U','HLT_Jet30U','HLT_Jet50U', 'HLT_DiJetAve15U', 'HLT_DiJetAve30U', 'HLT_DiJetAve50U', 'HLT_ExclDiJet30U_HFAND_v*','HLT_ExclDiJet30U_HFOR_v*'"
mail = 'dmf@cern.ch'
usesavelocal = ''
usedbsin =  '#'
usedbsout = '#'
usese = '#'
useadditional = '#' 
sewhitelist = ''
seblacklist = 'T3*,T1*'
dbsin = 'https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_02_writer/servlet/DBSServlet'
dbsout = 'https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_02_writer/servlet/DBSServlet'
seelement = 'se-dcache.hepgrid.uerj.br'
storagepath = '/srm/managerv2?SFN=/pnfs/hepgrid.uerj.br/data/cms/store/user/dmf'
sesave = '/MyRemoteDirSE'
publishname = 'datatype_physics_cmssw-version'
port = '8443'
ConfigHandlerDataGrid(triggers,inputfile,outputfile,addition,useserver,dataset,jsonfile,lumis,lumisjob,runsel,uiworkingdir,mail,usesavelocal,usedbsin,usedbsout,usese,
useadditional,sewhitelist,seblacklist,dbsin,dbsout,seelement,storagepath,sesave,publishname,port)



## /MultiJet/Run2010B-Apr21ReReco-v1/AOD
addition = 'lumibylsXing_Cert_136033-149442_7TeV_Apr21ReReco_Collisions10_JSON.root'
useserver = ''
dataset = '/MultiJet/Run2010B-Apr21ReReco-v1/AOD'
uiworkingdir = 'crab_MultiJet_Run2010B_PATTuple'
jsonfile = 'Intersection_Castor_GOOD_146240-148068.json'
lumis ='-1'
lumisjob = '15'
runsel = '147196-148058'
inputfile = ''
outputfile = 'MultiJetPATTuple.root'
triggers = "'HLT_Jet15U','HLT_Jet30U','HLT_Jet50U', 'HLT_DiJetAve15U', 'HLT_DiJetAve30U', 'HLT_DiJetAve50U', 'HLT_ExclDiJet30U_HFAND_v*','HLT_ExclDiJet30U_HFOR_v*'"
mail = 'dmf@cern.ch'
usesavelocal = ''
usedbsin =  '#'
usedbsout = '#'
usese = '#'
useadditional = '#' 
sewhitelist = ''
seblacklist = 'T3*,T1*'
dbsin = 'https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_02_writer/servlet/DBSServlet'
dbsout = 'https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_02_writer/servlet/DBSServlet'
seelement = 'se-dcache.hepgrid.uerj.br'
storagepath = '/srm/managerv2?SFN=/pnfs/hepgrid.uerj.br/data/cms/store/user/dmf'
sesave = '/MyRemoteDirSE'
publishname = 'datatype_physics_cmssw-version'
port = '8443'
ConfigHandlerDataGrid(triggers,inputfile,outputfile,addition,useserver,dataset,jsonfile,lumis,lumisjob,runsel,uiworkingdir,mail,usesavelocal,usedbsin,usedbsout,usese,
useadditional,sewhitelist,seblacklist,dbsin,dbsout,seelement,storagepath,sesave,publishname,port)




