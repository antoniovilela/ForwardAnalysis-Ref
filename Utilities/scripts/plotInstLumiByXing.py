#!/usr/bin/env python
import optparse
import csv
import ROOT

if __name__ == '__main__':
    parser = optparse.OptionParser ("Usage: %prog [--options] output.root") 
    # Options
    parser.add_option("-i", dest="inputfile", help="CSV input file")
    parser.add_option("--lumiMax", dest="lumiMax", default=1000, help="maximum lumi section number")
    parser.add_option("-v", dest="verbose", action="store_true", default=False, help="enable verbose")
    (options, args) = parser.parse_args()
    
    if not options.inputfile:  
        parser.error('must set an input file')
    if len(args) != 1:
        parser.error('must set output file as parameter')

    output = args[0]

    # Read csv file
    csvReader = csv.reader(open(options.inputfile))
    # Format: Run | Lumi section | Del. Lumi | Rec. Lumi | Bx 1 | Inst. Lumi 1 | Bx 2 | Inst. Lumi 2 | ... |
    outFile = ROOT.TFile.Open(output, 'recreate')
    if not outFile:
        raise RuntimeError, "Could not open output file '%s'" % output
    histos = {} 
    runNumbers = []
    maxLumiNumber = int(options.lumiMax)
    header = csvReader.next()
    if options.verbose: print header
    for lumiInfo in csvReader:
        runNumber = int(lumiInfo[0])
        lumiSection = int(lumiInfo[1])
        deliveredLumi = float(lumiInfo[2])
        recordedLumi = float(lumiInfo[3])
        if options.verbose: print runNumber,lumiSection,deliveredLumi,recordedLumi
        dirName = "Run%d" % runNumber
        if runNumber not in runNumbers:
            outFile.mkdir(dirName)
            runNumbers.append(runNumber)
            if options.verbose: print "Created new directory",dirName 
        outFile.cd("Run%d" % runNumber)
        if options.verbose: ROOT.gDirectory.pwd()
        for index in range(4,len(lumiInfo),2):
            bunchCrossing = int(lumiInfo[index])
            instLumiBx = float(lumiInfo[index+1])
            if options.verbose: print "   ",bunchCrossing,instLumiBx 
            if (runNumber,bunchCrossing) in histos:
                histos[(runNumber,bunchCrossing)].Fill(lumiSection,instLumiBx)
            else:
                histoName = "instLumi_%d_%d" % (runNumber,bunchCrossing)
                histos[(runNumber,bunchCrossing)] = ROOT.TH1F(histoName,histoName,maxLumiNumber,0,maxLumiNumber) 

    #for key in histos:
    #    histos[key].Write()

    outFile.Write()
    outFile.Close()
