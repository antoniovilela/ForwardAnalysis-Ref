#! /usr/bin/env python

import os,sys

def listFiles(dir,type,prefix):

    files = os.listdir(dir)
    files = [ "%s%s/%s" % (prefix,dir,item) for item in files if item.find(type) != -1 ]

    return files


if __name__ == '__main__':
    import optparse
    parser = optparse.OptionParser(usage="usage: %prog [options]")
    parser.add_option("-d","--dir", dest="dir", metavar="DIR", help="list files in DIR")
    parser.add_option("-t","--type", dest="type", default="root", metavar="TYPE", help="select only files with substring TYPE (Default: 'root')")
    parser.add_option("-p","--prefix", dest="prefix", default="", metavar="PREFIX", help="prepend PREFIX to file path")

    (input, args) = parser.parse_args()

    if not input.dir: parser.error('must set directory option')

    files = listFiles(dir=input.dir,
                      type=input.type,
                      prefix=input.prefix)

    # Print out
    for item in files: print item

    sys.exit(0)
