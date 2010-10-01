#! /usr/bin/env python

import json
import sys
import optparse

def runListFromJSON(filename,runMin,runMax):
    jsonFile = open(filename)
    jsonOut = json.load(jsonFile)
    runList = [int(key) for key in jsonOut if int(key) >= runMin and int(key) <= runMax]
    runList.sort()

    strOut = ""
    for run in runList: strOut += str(run) + ","
    strOut = strOut.rstrip(',')
    print strOut

if __name__ == '__main__':
    parser = optparse.OptionParser(usage="usage: %prog [options]")
    parser.add_option("-f","--file", dest="filename", metavar="FILE", help="read JSON file FILE")
    parser.add_option("--runMin", dest="runMin", type="int", default=1, metavar="RUN", help="read info from RUN")
    parser.add_option("--runMax", dest="runMax", type="int", default=9999999, metavar="RUN", help="read info up to RUN")

    (input, args) = parser.parse_args()

    if not input.filename: parser.error('must set input JSON file')

    runListFromJSON(filename=input.filename,
                      runMin=input.runMin,
                      runMax=input.runMax)

    sys.exit(0)
