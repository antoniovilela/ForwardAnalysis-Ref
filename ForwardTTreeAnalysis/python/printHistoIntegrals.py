#! /usr/bin/env python

import optparse
import ROOT
import sys

if __name__ == "__main__":
    # Setup options parser
    parser = optparse.OptionParser \
             ("usage: %prog [options] file.root")
    parser.add_option ('--mean', dest='mean', action='store_true',
                       help = 'print mean of histograms')
    options, args = parser.parse_args()
    if len (args) != 1:
        parser.print_usage()
        sys.exit()
    file = ROOT.TFile.Open(args[0])
    nextKey = ROOT.TIter( file.GetListOfKeys() )
    key = nextKey()
    while key:
        hist = key.ReadObj()
        try:
            print "%-24s %12.5f" % (hist.GetName(), hist.Integral()),
        except:
            # must not have been a TH1 derivative
            key = nextKey()
            continue
        if options.mean:
            print "  %12.5f" % hist.GetMean(),
        print
        key = nextKey()

