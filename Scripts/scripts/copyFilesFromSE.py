#!/usr/bin/python

import os,sys

if __name__ == '__main__':
    import optparse
    usage = "usage: %prog [options] <remote dir> <local dir>"
    parser = optparse.OptionParser(usage=usage)
    parser.add_option("-s","--site", dest="site", metavar="SITE", help="site name")
    parser.add_option("-p","--port", dest="port", type="int", default=8443, metavar="PORT", help="SRM port (Default: 8443)")
    parser.add_option("-t","--type", dest="type", default="root", metavar="TYPE", help="select only files with substring TYPE (Default: 'root')")
    parser.add_option("--srm_str", dest="srmstr", default="/srm/v2/server?SFN=", metavar="SRM", help="SRM string (Default: '/srm/v2/server?SFN=')")
    parser.add_option("--no_exec", dest="enable", action="store_false", default=True, help="files will not be copied")

    (input, args) = parser.parse_args()

    if not input.site: parser.error('must set site name')
    if len(args) != 2: parser.error('exactly two arguments required')
 
    from subprocess import Popen,PIPE,call
    lscmd = 'lcg-ls -b -D srmv2 --vo cms'
    cpcmd = 'lcg-cp -b -D srmv2 --vo cms'

    #srm_string = '//srm/managerv2?SFN='
    #srm_string = '/srm/v2/server?SFN='
    srm_string = input.srmstr
    storage_name = input.site
    storage_port = input.port
    storage_path = args[0] 
    outdir = args[1] 
    type = input.type
    endpoint = 'srm://' + storage_name + ':' + str(storage_port) + srm_string
    fullpath = endpoint + storage_path
    lscmd = '%s "%s"' % (lscmd,fullpath)
    print lscmd
    p1 = Popen(lscmd,shell=True,stdout=PIPE)
    files = [item.rstrip().split("/")[-1] for item in p1.stdout if item.find(type) != -1]
    for file in files:
        outfile = os.path.abspath(outdir) + "/" + file
        cmd = '%s "%s/%s" "file:%s"' % (cpcmd,fullpath,file,outfile)
        print cmd
        if input.enable: call(cmd,shell=True)
