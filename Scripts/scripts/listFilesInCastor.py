#! /usr/bin/env python

import sys
import optparse
from subprocess import Popen,PIPE

def listFilesInCastor(castor_dir,type,prefix):
    if not castor_dir: raise ValueError,'Please specify valid castor dir'

    p1 = Popen(['nsls',castor_dir],stdout=PIPE)
    p2 = Popen(['grep',type],stdin=p1.stdout,stdout=PIPE)
    files = [prefix + castor_dir + "/" + item[:-1] for item in p2.stdout]
    p2.stdout.close()
    return files


if __name__ == '__main__':
    parser = optparse.OptionParser(usage="usage: %prog [options]")
    parser.add_option("-d","--dir", dest="dir", metavar="DIR", help="list files in DIR")
    parser.add_option("-t","--type", dest="type", default="root", metavar="TYPE", help="select only files with substring TYPE (Default: 'root')")
    parser.add_option("-p","--prefix", dest="prefix", default="rfio:", metavar="PREFIX", help="prepend PREFIX to file path (Default 'rfio:')")

    (input, args) = parser.parse_args()

    if not input.dir: parser.error('must set directory option')

    files = listFilesInCastor(castor_dir=input.dir,
                                   type=input.type,
                                 prefix=input.prefix)

    # Print out
    for item in files: print item

    sys.exit(0)
