#!/usr/bin/env python

import os,sys
from subprocess import call
import optparse

def mergeTTree(inputdir,groupFiles=100,filename='merge.root',outputdir='.',type='root',execute=True):
    files = os.listdir(inputdir)
    files = [ item for item in files if item.find(type) != -1 ] 
    files = [ item.rstrip() for item in files ]
    # Divide in groups of files
    nFilesPerSection = groupFiles
    sections = []
    while len(files):
        sections.append(files[:nFilesPerSection])
        files = files[nFilesPerSection:]

    outfiletype = filename
    index = 0 
    for sec in sections:
        outfile = '%s/%s_%s.%s' % (os.path.abspath(outputdir),outfiletype.split('.')[0],index,outfiletype.split('.')[1])
        cmd = 'hadd ' + outfile
        for file in sec: cmd += ' %s/%s' % (inputdir,file)
        print cmd
        if execute: call(cmd,shell=True)
        index += 1

if __name__ == '__main__':
    parser = optparse.OptionParser(usage="usage: %prog [options]")
    parser.add_option("-i","--inputdir", dest="inputdir", metavar="DIR", help="add files in DIR")
    parser.add_option("-o","--outputdir", dest="outputdir", default='.', metavar="DIR", help="copy output to DIR (Default: local dir)")
    parser.add_option("-t","--type", dest="type", default="root", metavar="TYPE", help="select only files with substring TYPE (Default: 'root')")
    parser.add_option("-f","--filename", dest="filename", default="merge.root", metavar="FILE", help="output file name (Default: 'merge.root')") 
    parser.add_option("-g","--group", dest="group", type="int", default=100, metavar="GROUP", help="merge files in groups of GROUP (Default: 100)")
    parser.add_option("--no_exec", action="store_true", dest="no_exec", default=False, help="script will not execute")

    (input, args) = parser.parse_args()

    if not input.inputdir: parser.error('must set input directory')

    execute = True
    if input.no_exec: execute = False

    mergeTTree(inputdir=input.inputdir,
               groupFiles=input.group,
               filename=input.filename,
               outputdir=input.outputdir,
               type=input.type,
               execute=execute) 

    sys.exit(0)
