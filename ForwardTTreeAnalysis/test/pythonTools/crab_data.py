cdata_cfg = """
[CRAB]
jobtype = cmssw
scheduler = glite
@@USESERVER@@use_server =  1
@@USESERVER@@server_name = cern_vocms22 2.7.3- glite

[CMSSW]
datasetpath=@@DATASET@@
pset=pset.py
output_file=@@OUTPUT@@
lumi_mask=@@JSONFILE@@
total_number_of_lumis=@@LUMIS@@
lumis_per_job = @@LUMISJOB@@
runselection = @@RUNSEL@@
@@USEDBSIN@@dbs_url = @@DBSSERVERIN@@ 

[USER]
ui_working_dir=@@UIWORKINGDIR@@

@@USESAVELOCAL@@return_data = 1
@@USESAVELOCAL@@copy_data = 0
@@USESAVELOCAL@@publish_data=0

@@USEDBSOUT@@copy_data = 1
@@USEDBSOUT@@storage_element = @@SEELEMENT@@
@@USEDBSOUT@@storage_path = @@STORAGEPATH@@
@@USEDBSOUT@@user_remote_dir = @@SE_SAVE@@
@@USEDBSOUT@@publish_data=1
@@USEDBSOUT@@publish_data_name = @@PUBLISHNAME@@ 
@@USEDBSOUT@@dbs_url_for_publication = @@DBSSERVEROUT@@ 

@@USESE_SAVE@@return_data = 0
@@USESE_SAVE@@copy_data = 1
@@USESE_SAVE@@check_user_remote_dir = 1
@@USESE_SAVE@@storage_element = @@SEELEMENT@@
@@USESE_SAVE@@storage_path = @@STORAGEPATH@@
@@USESE_SAVE@@storage_port = @@PORT@@
@@USESE_SAVE@@user_remote_dir = @@SE_SAVE@@

@@USEADDITIONALFILE@@additional_input_files = @@ADDITIONALFILE@@

email = @@MAIL@@

[GRID]
proxy_server = myproxy.cern.ch
@@USESEWHITELIST@@se_white_list = @@SEWHITELIST@@
@@USESEBLACKLIST@@se_black_list = @@SEBLACKLIST@@
virtual_organization = cms

"""

