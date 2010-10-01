#! /usr/bin/env python

import sys
import optparse
from subprocess import Popen,PIPE

def listFilesInCastor(castor_dir,type,prefix):
    p1 = Popen(['nsls',castor_dir],stdout=PIPE)
    p2 = Popen(['grep',type],stdin=p1.stdout,stdout=PIPE)
    files = [prefix + castor_dir + "/" + item[:-1] for item in p2.stdout]
    p2.stdout.close()
    return files

def haddFilesInCastor(castor_dir,result_file,type,prefix,suffix,option):
    from subprocess import call
    files = listFilesInCastor(castor_dir,type,prefix)
    if suffix: files = [item + suffix for item in files]
 
    cmd = ['hadd']
    if option: cmd.append(option)
    cmd.append(result_file)
    cmd.extend(files)
    #print cmd
    #return 0
    retcode = call(cmd)
    return retcode

if __name__ == '__main__':
    parser = optparse.OptionParser(usage="usage: %prog [options]")
    parser.add_option("-f","--file", dest="file", metavar="FILE", help="output file")
    parser.add_option("-d","--dir", dest="dir", metavar="DIR", help="add files in DIR")
    parser.add_option("-t","--type", dest="type", default="root", metavar="TYPE", help="select only files with substring TYPE (Default: 'root')")
    parser.add_option("-p","--prefix", dest="prefix", default="rfio:", metavar="PREFIX", help="prepend PREFIX to file path (Default 'rfio:')")
    parser.add_option("-s","--suffix", dest="suffix", default="", metavar="SUFFIX", help="append SUFFIX to file path") 
    parser.add_option("-o","--option", dest="option", default="", metavar="OPTION", help="pass OPTION to hadd command")

    (input, args) = parser.parse_args()

    if not input.dir: parser.error('must set directory option')

    retcode = haddFilesInCastor(castor_dir  = input.dir,
                                result_file = input.file,
                                type        = input.type,
                                prefix      = input.prefix,
                                suffix      = input.suffix,
                                option      = input.option)

    sys.exit(retcode)
