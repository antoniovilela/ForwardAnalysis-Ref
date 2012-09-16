#! /usr/bin/env python

import sys
import optparse
from subprocess import Popen,PIPE

def listFilesInCastor(castor_dir,type,veto,prefix):
    if not castor_dir: raise ValueError,'Please specify valid castor dir'

    p1 = Popen(['nsls',castor_dir],stdout=PIPE)
    #p2 = Popen(['grep',type],stdin=p1.stdout,stdout=PIPE)
    #files = [prefix + castor_dir + "/" + item[:-1] for item in p2.stdout]
    #p2.stdout.close()
    files = [item[:-1] for item in p1.stdout]
    p1.stdout.close()

    #result = ['%s%s/%s' % (prefix,castor_dir,file) for file in files if file.find(type) != -1] 
    result = [] 
    for item in files:
        if item.find(type) == -1: continue
        if veto and item.find(veto) != -1: continue
        result.append( '%s%s/%s' % (prefix,castor_dir,item) )

    return result


if __name__ == '__main__':
    parser = optparse.OptionParser(usage="usage: %prog [options]")
    parser.add_option("-d","--dir", dest="dir", metavar="DIR", help="list files in DIR")
    parser.add_option("-t","--type", dest="type", default="root", metavar="TYPE", help="select only files with substring TYPE (Default: 'root')")
    parser.add_option("-v","--veto", dest="veto", default="", metavar="VETO", help="select only files without substring VETO")
    parser.add_option("-p","--prefix", dest="prefix", default="rfio:", metavar="PREFIX", help="prepend PREFIX to file path (Default 'rfio:')")

    (input, args) = parser.parse_args()

    if not input.dir: parser.error('must set directory option')

    files = listFilesInCastor(castor_dir = input.dir,
                                    type = input.type,
                                    veto = input.veto,
                                  prefix = input.prefix)

    # Print out
    for item in files: print item

    sys.exit(0)
