#! /usr/bin/env python

import sys
from DBSAPI.dbsApi import DbsApi
from DBSAPI.dbsException import *
from DBSAPI.dbsApiException import *
from DBSAPI.dbsOptions import DbsOptionParser

def listLFN(api,dataset):
    fileNames = None
    try :
        #for afile in api.listDatasetFiles(datasetPath=dataset):
        #    print "PATH  %s" % afile['LogicalFileName']
        filesDBS = api.listDatasetFiles(datasetPath=dataset)
        fileNames = [file['LogicalFileName'] for file in filesDBS]

    except DbsApiException, ex:
        print "Caught API Exception %s: %s "  % (ex.getClassName(), ex.getErrorMessage() )
        if ex.getErrorCode() not in (None, ""):
            print "DBS Exception Error Code: ", ex.getErrorCode()
    
    return fileNames

if __name__ == '__main__':
    """
    import optparse
    parser = optparse.OptionParser(usage="usage: %prog [options]")
    parser.add_option("-d","--dataset", dest="dataset", metavar="DATASET", help="list LFN's from DATASET")

    (input, args) = parser.parse_args()

    if not input.dataset: parser.error('must set dataset option')
    """

    import optparse
    optManager  = DbsOptionParser()
    group_local = optparse.OptionGroup(optManager.parser,"Script options")
    group_local.add_option("-d","--dataset", dest="dataset", metavar="DATASET", help="list LFN's from DATASET")
    optManager.parser.add_option_group(group_local)
    (opts,args) = optManager.getOpt()
    api = DbsApi(opts.__dict__)
    #api = DbsApi()

    if not opts.dataset: optManager.parser.error('must set dataset option')

    print "Finding LFN's from",opts.dataset
     
    files = listLFN(api=api,dataset=opts.dataset)

    """
    dataset = '/ZeroBias/Commissioning10-Sep17ReReco_v2/RECO'
    try :
        for afile in api.listDatasetFiles(datasetPath=dataset):
            print "PATH  %s" % afile['LogicalFileName']

    except DbsApiException, ex:
        print "Caught API Exception %s: %s "  % (ex.getClassName(), ex.getErrorMessage() )
        if ex.getErrorCode() not in (None, ""):
            print "DBS Exception Error Code: ", ex.getErrorCode()
    """

    # Print out
    for item in files: print item

    sys.exit(0)
