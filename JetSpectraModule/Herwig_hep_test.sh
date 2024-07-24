#! /bin/bash
export MYINSTALL=/sphenix/user/sgross/install_dir
source /cvmfs/sphenix.sdcc.bnl.gov/gcc-12.1.0/opt/sphenix/core/bin/sphenix_setup.sh -n ana
source /opt/sphenix/core/bin/setup_local.sh $MYINSTALL

root.exe -x -b  /sphenix/user/sgross/sphenix_herwig/JetSpectraModule/Herwig_hep_test.C\(\"$1\"\) 
