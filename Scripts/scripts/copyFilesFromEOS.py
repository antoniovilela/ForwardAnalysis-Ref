#! /usr/bin/env python

import sys
import optparse
from subprocess import Popen,PIPE,call
from listFilesEOS import listFilesEOS

def copyFilesFromEOS(dir,output_dir,type,suffix,enable):

    cpcmd = 'xrdcp'
    copy_prefix = 'root://eoscms//eos/cms'

    files = listFilesEOS(dir,type,copy_prefix)
    if suffix: files = [item + suffix for item in files]

    print "Copying from %s to %s" % (dir,output_dir) 
    for item in files:
        cmd = '%s %s %s' % (cpcmd,item,output_dir)
        print cmd
        if enable:
	    retcode = call(cmd,shell=True)
	    if retcode != 0: raise RuntimeError,'Error in copying file %s to directory %s' % (item,output_dir)

if __name__ == '__main__':
    parser = optparse.OptionParser(usage="usage: %prog [options]")
    parser.add_option("-o","--out", dest="out", metavar="OUT", help="output directory")
    parser.add_option("-d","--dir", dest="dir", metavar="DIR", help="copy files from DIR")
    parser.add_option("-t","--type", dest="type", default="root", metavar="TYPE", help="select only files with substring TYPE (Default: 'root')")
    #parser.add_option("-p","--prefix", dest="prefix", default="", metavar="PREFIX", help="prepend PREFIX to file path")
    parser.add_option("-s","--suffix", dest="suffix", default="", metavar="SUFFIX", help="append SUFFIX to file path") 
    parser.add_option("--no_exec", dest="enable", action="store_false", default=True, help="files will not be copied")

    (input, args) = parser.parse_args()

    if not input.dir: parser.error('must set input directory')
    if not input.out: parser.error('must set output directory')

    copyFilesFromEOS(dir  = input.dir,
                     output_dir  = input.out,
                     type        = input.type,
                     #prefix      = input.prefix,
                     suffix      = input.suffix,
                     enable      = input.enable)

    sys.exit(0)
