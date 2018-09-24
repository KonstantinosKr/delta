#!/user/bin/python3
# vim: tabstop=8 expandtab shiftwidth=4 softtabstop=4
import argparse
from argparse import RawTextHelpFormatter

import re
import os
import csv

import operator

'''
.. module:: extractlikwidmetrics
  :platform: Unix, Windows, Mac
  :synopsis: Contains routines to extract 
             performance metrics from Peano output files with specific file naming pattern.
             The call of the Peano application must be wrapped within likwid-perfctr via: 
             likwid-perfctr -C 0 -g <METRIC> <application>
   
.. moduleauthor:: Dominic Etienne Charrier <dominic.e.charrier@durham.ac.uk>

:synopsis: Extracts performance metrics from Peano output files with specific file naming pattern.
'''

def extract_likwid_metrics(root_dir,prefix):
    '''
    Extracts performance metrics from Peano output files with specific file naming pattern.
    
    We are currently parsing  "MFLOP/s STAT" (SUM), "Memory bandwidth [MBytes/s]" (SUM),
    "Branch misprediction rate STAT" (Avg),  "L2 miss rate STAT" (Avg).
    
    The output of likwid is different for singlecore and multicore
    runs:
    
    # Singlecore Metrics

    ## FLOPS_DP

    +----------------------+-----------+
    |        Metric        |   Core 1  |
    +----------------------+-----------+
    |  Runtime (RDTSC) [s] |   6.4970  |
    | Runtime unhalted [s] |   7.7641  |
    |      Clock [MHz]     | 2867.1614 |
    |          CPI         |   1.4365  |
    |        MFLOP/s       |  578.7295 |
    |      AVX MFLOP/s     |     0     |
    |    Packed MUOPS/s    |  252.4243 |
    |    Scalar MUOPS/s    |  73.8808  |
    +----------------------+-----------+

    ##  MEM

    +-----------------------------------+------------+
    |               Metric              |   Core 1   |
    +-----------------------------------+------------+
    |        Runtime (RDTSC) [s]        |   6.5468   |
    |        Runtime unhalted [s]       |   7.7608   |
    |            Clock [MHz]            |  2849.4441 |
    |                CPI                |   1.4359   |
    |  Memory read bandwidth [MBytes/s] |  9203.1065 |
    |  Memory read data volume [GBytes] |   60.2505  |
    | Memory write bandwidth [MBytes/s] |  4409.1727 |
    | Memory write data volume [GBytes] |   28.8658  |
    |    Memory bandwidth [MBytes/s]    | 13612.2793 |
    |    Memory data volume [GBytes]    |   89.1163  |
    +-----------------------------------+------------+

    ## BRANCH

    +----------------------------+--------------+
    |           Metric           |    Core 1    |
    +----------------------------+--------------+
    |     Runtime (RDTSC) [s]    |    6.5219    |
    |    Runtime unhalted [s]    |    7.7536    |
    |         Clock [MHz]        |   2858.6786  |
    |             CPI            |    1.4345    |
    |         Branch rate        |    0.1190    |
    |  Branch misprediction rate | 2.575205e-07 |
    | Branch misprediction ratio | 2.163851e-06 |
    |   Instructions per branch  |    8.4026    |
    +----------------------------+--------------+

    ## L2CACHE

    +----------------------+-----------+
    |        Metric        |   Core 1  |
    +----------------------+-----------+
    |  Runtime (RDTSC) [s] |   6.5188  |
    | Runtime unhalted [s] |   7.7477  |
    |      Clock [MHz]     | 2855.7219 |
    |          CPI         |   1.4335  |
    |    L2 request rate   |   0.3787  |
    |     L2 miss rate     |   0.1056  |
    |     L2 miss ratio    |   0.2788  |
    +----------------------+-----------+


    # Multicore Metrics


    ## FLOPS_DP:

    +---------------------------+------------+-----------+-----------+-----------+
    |           Metric          |     Sum    |    Min    |    Max    |    Avg    |
    +---------------------------+------------+-----------+-----------+-----------+
    |  Runtime (RDTSC) [s] STAT |   27.7776  |   1.1574  |   1.1574  |   1.1574  |
    | Runtime unhalted [s] STAT |   17.5755  |   0.7025  |   1.1664  |   0.7323  |
    |      Clock [MHz] STAT     | 53287.5604 | 2194.3481 | 2339.6363 | 2220.3150 |
    |          CPI STAT         |   80.7068  |   0.8296  |   3.5650  |   3.3628  |
    |        MFLOP/s STAT       |  3248.7237 |  118.0829 |  532.8151 |  135.3635 |
    |      AVX MFLOP/s STAT     |      0     |     0     |     0     |     0     |
    |    Packed MUOPS/s STAT    |  1416.9944 |  59.0414  |  59.0415  |  59.0414  |
    |    Scalar MUOPS/s STAT    |  414.7347  |     0     |  414.7321 |  17.2806  |
    +---------------------------+------------+-----------+-----------+-----------+

    ## MEM:

    +----------------------------------------+------------+-----------+------------+-----------+
    |                 Metric                 |     Sum    |    Min    |     Max    |    Avg    |
    +----------------------------------------+------------+-----------+------------+-----------+
    |        Runtime (RDTSC) [s] STAT        |   28.1040  |   1.1710  |   1.1710   |   1.1710  |
    |        Runtime unhalted [s] STAT       |   17.6412  |   0.6944  |   1.1724   |   0.7351  |
    |            Clock [MHz] STAT            | 53428.9064 | 2153.7694 |  2358.9159 | 2226.2044 |
    |                CPI STAT                |   81.0719  |   0.8342  |   3.5820   |   3.3780  |
    |  Memory read bandwidth [MBytes/s] STAT | 52486.4902 |     0     | 26297.3151 | 2186.9371 |
    |  Memory read data volume [GBytes] STAT |   61.4616  |     0     |   30.7941  |   2.5609  |
    | Memory write bandwidth [MBytes/s] STAT | 26486.8194 |     0     | 13702.7744 | 1103.6175 |
    | Memory write data volume [GBytes] STAT |   31.0160  |     0     |   16.0459  |   1.2923  |
    |    Memory bandwidth [MBytes/s] STAT    | 78973.3098 |     0     | 40000.0896 | 3290.5546 |
    |    Memory data volume [GBytes] STAT    |   92.4777  |     0     |   46.8401  |   3.8532  |
    +----------------------------------------+------------+-----------+------------+-----------+

    ## BRANCH:

    +---------------------------------+------------+--------------+--------------+--------------+
    |              Metric             |     Sum    |      Min     |      Max     |      Avg     |
    +---------------------------------+------------+--------------+--------------+--------------+
    |     Runtime (RDTSC) [s] STAT    |   27.2616  |    1.1359    |    1.1359    |    1.1359    |
    |    Runtime unhalted [s] STAT    |   17.7047  |    0.7118    |    1.1711    |    0.7377    |
    |         Clock [MHz] STAT        | 53381.5227 |   2197.7583  |   2378.3713  |   2224.2301  |
    |             CPI STAT            |   81.2767  |    0.8336    |    3.5603    |    3.3865    |
    |         Branch rate STAT        |   3.0690   |    0.0450    |    0.1340    |    0.1279    |
    |  Branch misprediction rate STAT |   0.0001   | 9.050274e-07 | 2.820245e-05 | 5.374324e-06 |
    | Branch misprediction ratio STAT |   0.0010   | 6.977308e-06 |    0.0002    | 4.115248e-05 |
    |   Instructions per branch STAT  |  197.2038  |    7.4615    |    22.2424   |    8.2168    |
    +---------------------------------+------------+--------------+--------------+--------------+

    ## L2CACHE

    +---------------------------+------------+-----------+-----------+-----------+
    |           Metric          |     Sum    |    Min    |    Max    |    Avg    |
    +---------------------------+------------+-----------+-----------+-----------+
    |  Runtime (RDTSC) [s] STAT |   27.4632  |   1.1443  |   1.1443  |   1.1443  |
    | Runtime unhalted [s] STAT |   17.7995  |   0.7182  |   1.1748  |   0.7416  |
    |      Clock [MHz] STAT     | 53545.8000 | 2203.7171 | 2357.4567 | 2231.0750 |
    |          CPI STAT         |   81.9969  |   0.8364  |   3.5848  |   3.4165  |
    |    L2 request rate STAT   |   12.3080  |   0.1178  |   0.5385  |   0.5128  |
    |     L2 miss rate STAT     |   2.9400   |   0.0293  |   0.1286  |   0.1225  |
    |     L2 miss ratio STAT    |   5.7406   |   0.2358  |   0.2489  |   0.2392  |
    +---------------------------+------------+-----------+-----------+-----------+
   
    Args:
      root_dir (str):
         Directory containing the Peano output files.
      prefix (str):
         Prefix of the files - usually the date of the test and an identifier for the test.
    '''
    
    metrics    = [
                  " MFLOP/s",
                  "Memory bandwidth [MBytes/s]",
                  "Memory data volume [GBytes]",
                  "L2 bandwidth [MBytes/s]",
                  "L2 request rate",
                  "L2 miss rate",
                  "Branch misprediction rate"
                  ]
                  
    columns    = [ "Sum","Min","Max","Avg" ]
    
    # collect filenames
    with open(prefix+'.likwid.csv', 'w') as csvfile:
        csvwriter = csv.writer(csvfile, delimiter='&',quotechar='|', quoting=csv.QUOTE_MINIMAL)
        
        # write header
        header = ["Nodes","Tasks (per Node)","Order","Cores (per Task)","CC","Mode","Kernels"]
        for metric in metrics:
            for column in columns:          
                    header.append(metric+"("+column+")")
        csvwriter.writerow(header)

        # write content
        for filename in os.listdir(root_dir):
            if filename.endswith(".out.likwid") and filename.startswith(prefix):
                print(filename)
                match = re.search('-p([0-9]+)-.*-n([0-9]+)-t([0-9]+)-c([0-9]+)-([A-Za-z]+)-([A-Za-z]+)-([A-Za-z]+)',filename)
                order = match.group(1)
                nodes = match.group(2)
                tasks = match.group(3)
                cores = match.group(4)
                mode  = match.group(5)
                cc    = match.group(6)
                opt   = match.group(7)
                    
                measurements = parse_likwid_metrics(root_dir+'/'+filename,metrics,int(cores)==1) 
                
                row = [nodes,tasks,order,cores,cc,mode,opt]
                   
                for metric in metrics:
                    for column in columns:
                        row.append ( str(measurements[metric][column]) )
                csvwriter.writerow(row)

def parse_likwid_metrics(file_path,metrics,singlecore=False):
    """
    Reads a single Peano output file and parses likwid performance metrics.
    
    Args:
       file_path (str):
          Path to the Peano output file.
       metrics (str[]):
          A list of the metrics we want to read out.
       singlecore (bool):
          Specifies if the run was a singlecore run.

    Returns:
       A dict holding for each of the found metrics a nested dict that holds the following key-value pairs:
          * 'Sum' 
          * 'Avg' 
          * 'Min' 
          * 'Max' 
    """
    columns    = [ "Sum","Min","Max","Avg" ]
    
    result  = { }
    for metric in metrics:
        result[metric] =  { }
        for column in columns:
            result[metric][column] = -1.0

    try:
        file_handle=open(file_path)
        
        for line in file_handle:
            for metric in metrics: 
                if singlecore:
                    if metric in line:
                        segments = line.split('|')
                        
                        #    |     Runtime (RDTSC) [s]    |    6.5219    |
                        value = float(segments[2].strip());
                        result[metric]["Sum"] = value
                        result[metric]["Min"] = value
                        result[metric]["Max"] = value
                        result[metric]["Avg"] = value
                        
                else:
                    if metric+" STAT" in line:
                        segments = line.split('|')
                        #   |  Runtime (RDTSC) [s] STAT |   27.4632  |   1.1443  |   1.1443  |   1.1443  |
                        result[metric]["Sum"] = float(segments[2].strip());
                        result[metric]["Min"] = float(segments[3].strip());
                        result[metric]["Max"] = float(segments[4].strip());
                        result[metric]["Avg"] = float(segments[5].strip());
    except:
        print ("Error: Could not process file '%s'!\n" % (file_path))
        raise
    return result


def sort_table(filename):
    '''
    Sorts the rows of the file according to nodes,tasks,cores,
    See: https://stackoverflow.com/a/17109098
    '''
    datafile    = open(filename, 'r')
    header      = next(datafile).strip()
    reader      = csv.reader(datafile,delimiter='&')
    sorted_data = sorted(reader, key=lambda x: (int(x[0]),int(x[1]),int(x[2]),int(x[3])))
    datafile.close() 
 
    with open(filename, 'w') as datafile:
        writer = csv.writer(datafile, delimiter='&',quotechar='|', quoting=csv.QUOTE_MINIMAL)
        writer.writerow(header.split('&'))
        writer.writerows(sorted_data)

########################################################################
# START OF THE PROGRAM
########################################################################
# For the meaning of the command line arguments, see the documentation
# of the function 'extrat_table' above.
help = '''
Extract performance metrics from Peano output files with specific file naming pattern
and write them to a csv file with name
<prefix.csv

\n\n
Sample usage:\n
python extractlikwidmetrics.py -path \'examples/151217_phi1_node/' -prefix "EulerFlow"
'''

parser = argparse.ArgumentParser(description=help,formatter_class=RawTextHelpFormatter)
parser.add_argument('-path',required=True,help="Directory containing the Peano output files.")
parser.add_argument('-prefix',required=True,help="Prefix of the Peano output files.")

args     = parser.parse_args();

root_dir = args.path
prefix   = args.prefix

extract_likwid_metrics(root_dir,prefix)
sort_table(prefix+".likwid.csv")
