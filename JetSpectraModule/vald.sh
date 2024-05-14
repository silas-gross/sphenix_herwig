#! /bin/bash 
export MYINSTALL=/sphenix/user/sgross/install_dir
source /cvmfs/sphenix.sdcc.bnl.gov/gcc-12.1.0/opt/sphenix/core/bin/sphenix_setup.sh -n ana
source /opt/sphenix/core/bin/setup_local.sh $MYINSTALL

/cvmfs/sphenix.sdcc.bnl.gov/gcc-12.1.0/opt/sphenix/utils/bin/valgrind  --track-origins=yes --log-file=/gpfs/mnt/gpfs02/sphenix/user/sgross/sphenix_herwig/JetSpectraModule/valgrind-out.txt /cvmfs/sphenix.sdcc.bnl.gov/gcc-12.1.0/opt/sphenix/core/root-6.26.06.p01/bin/root.exe -x /gpfs/mnt/gpfs02/sphenix/user/sgross/sphenix_herwig/JetSpectraModule/Herwig_hep_test.C\(\"/gpfs/mnt/gpfs02/sphenix/user/sgross/sphenix_herwig/herwig_files/sphenix_10GeV_jetpt.hepmc\"\)
