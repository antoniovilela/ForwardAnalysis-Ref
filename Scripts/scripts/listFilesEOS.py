#! /usr/bin/env python

import sys
import optparse
from subprocess import Popen,PIPE,call

def listFilesEOS(dir,type,prefix):

    #lscmd = 'eoscms ls'
    lscmd = '/afs/cern.ch/project/eos/installation/0.1.0-22d/bin/eos.select ls'
    lscmd = '%s %s' % (lscmd,dir)
    print lscmd
    p1 = Popen(lscmd,shell=True,stdout=PIPE)
    files = [item.rstrip().split("/")[-1] for item in p1.stdout]
    p1.stdout.close()
    
    result = ['%s%s/%s' % (prefix,dir,file) for file in files if file.find(type) != -1] 

    return result


if __name__ == '__main__':
    parser = optparse.OptionParser(usage="usage: %prog [options]")
    parser.add_option("-d","--dir", dest="dir", metavar="DIR", help="list files in DIR")
    parser.add_option("-t","--type", dest="type", default="root", metavar="TYPE", help="select only files with substring TYPE (Default: 'root')")
    parser.add_option("-p","--prefix", dest="prefix", default="", metavar="PREFIX", help="prepend PREFIX to file path")

    (input, args) = parser.parse_args()

    if not input.dir: parser.error('must set directory option')

    files = listFilesEOS(dir=input.dir,
                         type=input.type,
                         prefix=input.prefix)

    # Print out
    for item in files: print item

    sys.exit(0)
