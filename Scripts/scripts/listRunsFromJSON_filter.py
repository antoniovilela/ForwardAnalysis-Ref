#! /usr/bin/env python

import json
import sys
import optparse
import subprocess


def runListFromJSON_filter(filename,runMin,runMax,groupRuns=10):
    jsonFile = open(filename)
    jsonOut = json.load(jsonFile)
    runList = [int(key) for key in jsonOut if int(key) >= runMin and int(key) <= runMax]
    runList.sort()

    strOut = ""
   # runstr_Min = ""
   # runstr_Max = ""

    elements = len(runList)
    runList_ = zip(*[iter(runList)]*groupRuns)
    runGroups =  elements/groupRuns
   
    print '#########################################################################'
    print 'Split Runs:',groupRuns,' Runs: ',elements,' Run group:', runGroups
    print '#########################################################################'
    #print runList_
 
 
    for i in range(0,runGroups): 
         #for j in range(0,groupRuns):
             print 'First: ', i  , runList_[i][0],'->>>> ',' Last:',runList_[i][groupRuns-1] 
             runstr_Min = str(runList_[i][0])
             runstr_Max = str(runList_[i][groupRuns-1])
             ##file_ = str(filename)  
             output_str=  filename + " --min " + runstr_Min + " --max " + runstr_Max + " --output " + "Run_" + runstr_Min + "-" + runstr_Max  + ".json"  
        
             #print output_str
            ## print filename
              #cmd_=shlex.split(output_str)      
             subprocess.call("filterJSON.py "+ output_str,shell=True) 
             
#    for run in runList : strOut += str(run) + ","
#    strOut = strOut.rstrip(',')
    #print strOut

if __name__ == '__main__':
    parser = optparse.OptionParser(usage="usage: %prog [options]")
    parser.add_option("-f","--file", dest="filename", metavar="FILE", help="read JSON file FILE")
    parser.add_option("--runMin", dest="runMin", type="int", default=1, metavar="RUN", help="read info from RUN")
    parser.add_option("--runMax", dest="runMax", type="int", default=9999999, metavar="RUN", help="read info up to RUN")
    parser.add_option("-g","--group", dest="group", type="int", default=10, metavar="GROUP", help="split in run groups of GROUP (Default: 10)")
    
    (input, args) = parser.parse_args()

    if not input.filename: parser.error('must set input JSON file')

    runListFromJSON_filter(filename=input.filename,
                      runMin=input.runMin,
                      runMax=input.runMax,
                      groupRuns=input.group)

    sys.exit(0)
