#!/usr/bin/env python
import os, sys, glob, shutil, threading, multiprocessing

option_list = ['bundle', 'nLi']
data_from = {
    'bundle' : 'data_0',
    'nLi' : 'data_bundle',
}

def main(filename, option):
    if not (option in option_list):
        print option, "option does not exist."
        return

    to_glob = filename[:filename.rfind('_f0')] + '_*.root'
    list_of_files = glob.glob(to_glob)

    file_main, file_extension = os.path.splitext(filename)
    to_replace = file_main[file_main.rfind('_'):]
    # print to_replace

    cmds = []
    for f in list_of_files:
        of = f.replace(to_replace, "_"+option)
        of = of.replace(data_from[option], "data_"+option)
        dirname = os.path.dirname(of)
        if not (os.path.exists(dirname)):
            os.makedirs(dirname)
        cmd = "root -b -q -l loadClasses.C '%s.C(\"%s\", \"%s\")'" % (
            option,
            f,
            of)
        print cmd
        cmds.append(cmd)
    nCores = multiprocessing.cpu_count()
    print 'total cpu: ', nCores
    pool = multiprocessing.Pool(nCores*5)
    for cmd in cmds:
        pool.apply_async(os.system, args=(cmd,))
    pool.close()
    pool.join()


    # print list_of_files; print; print output_files;

def usage():
    print """
    python process.py [filename] [option]
        available options: bundle, nLi
    """

if __name__ == "__main__":
    if (len(sys.argv)<=1):
        usage()
    else:
        main(sys.argv[1], sys.argv[2])