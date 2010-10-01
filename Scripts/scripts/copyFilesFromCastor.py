#! /usr/bin/env python

import sys
import optparse
from subprocess import Popen,PIPE

def listFilesInCastor(castor_dir,type,veto,prefix):
    p1 = Popen(['nsls',castor_dir],stdout=PIPE)
    p2 = Popen(['grep',type],stdin=p1.stdout,stdout=PIPE)
    files = None
    if veto:
        p3 = Popen(['grep','-v',veto],stdin=p2.stdout,stdout=PIPE)
        files = [prefix + castor_dir + "/" + item[:-1] for item in p3.stdout]
        p3.stdout.close()
    else:
        files = [prefix + castor_dir + "/" + item[:-1] for item in p2.stdout]
        p2.stdout.close()
    return files

def copyFilesFromCastor(castor_dir,output_dir,type,veto,prefix,suffix):
    from subprocess import call
    files = listFilesInCastor(castor_dir,type,veto,prefix)
    if suffix: files = [item + suffix for item in files]

    print "Copying from %s to %s" % (castor_dir,output_dir) 
    for item in files:
        cmd = ['rfcp',item,output_dir] 
        print "..." + item
        retcode = call(cmd)
        if retcode != 0: raise RuntimeError,'Error in copying file %s to directory %s' % (item,output_dir)

if __name__ == '__main__':
    parser = optparse.OptionParser(usage="usage: %prog [options]")
    parser.add_option("-o","--out", dest="out", metavar="OUT", help="output directory")
    parser.add_option("-d","--dir", dest="dir", metavar="DIR", help="copy files from DIR")
    parser.add_option("-t","--type", dest="type", default="root", metavar="TYPE", help="select only files with substring TYPE (Default: 'root')")
    parser.add_option("-v","--veto", dest="veto", default="", metavar="VETO", help="select only files not matching substring VETO")
    parser.add_option("-p","--prefix", dest="prefix", default="", metavar="PREFIX", help="prepend PREFIX to file path")
    parser.add_option("-s","--suffix", dest="suffix", default="", metavar="SUFFIX", help="append SUFFIX to file path") 

    (input, args) = parser.parse_args()

    if not input.dir: parser.error('must set input directory')
    if not input.out: parser.error('must set output directory')

    copyFilesFromCastor(castor_dir  = input.dir,
                        output_dir  = input.out,
                        type        = input.type,
                        veto        = input.veto,
                        prefix      = input.prefix,
                        suffix      = input.suffix)

    sys.exit(0)
