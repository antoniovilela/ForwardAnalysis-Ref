#! /usr/bin/env python

import json
import sys
import optparse

def countLumisFromJSON(filename,runMin,runMax):
    jsonFile = open(filename)
    jsonOut = json.load(jsonFile)
    runList = [int(key) for key in jsonOut if int(key) >= runMin and int(key) <= runMax]
    runList.sort()

    totalNumberLumis = 0
    for run in runList:
        countLumi = 0
        for pair in jsonOut[str(run)]:
            nLumis = pair[-1] - pair[0] + 1
            countLumi += nLumis
        print "Run",run,"-->",countLumi
        totalNumberLumis += countLumi

    print "Total number of lumis =",totalNumberLumis

if __name__ == '__main__':
    parser = optparse.OptionParser(usage="usage: %prog [options]")
    parser.add_option("-f","--file", dest="filename", metavar="FILE", help="read JSON file FILE")
    parser.add_option("--runMin", dest="runMin", type="int", default=1, metavar="RUN", help="read info from RUN")
    parser.add_option("--runMax", dest="runMax", type="int", default=9999999, metavar="RUN", help="read info up to RUN")

    (input, args) = parser.parse_args()

    if not input.filename: parser.error('must set input JSON file')

    countLumisFromJSON(filename=input.filename,
                         runMin=input.runMin,
                         runMax=input.runMax)

    sys.exit(0)
