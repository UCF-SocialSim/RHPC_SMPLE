#!/bin/bash

mkdir -p ./output

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/opt/sfw/Boost/Boost_1.76/lib:/opt/sfw/NetCDF/lib:/opt/sfw/NetCDF-cxx/lib:/opt/sfw/repast_hpc-2.3.0/lib:/opt/sfw/rhpc_smple/

PROP=$1
shift

SUFFIX=$1
shift

mpiexec -n 1 ./bin/Xample_Model.exe ./props/config.props ./props/$PROP.props $@ >OUT.$SUFFIX.txt $added_props

