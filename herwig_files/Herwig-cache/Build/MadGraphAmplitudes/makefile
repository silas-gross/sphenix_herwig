include MG5/Source/make_opts  
PROCESS= InterfaceMadGraph.f   MG5/SubProcesses/P452_gg_ggg/matrix.f\
                  

all:  
	 gfortran  -g -O2 -std=legacy -ffixed-line-length-none -w -fbounds-check -ffixed-line-length-none -fPIC -fno-f2c -shared -s -o  InterfaceMadGraph.so -IMG5/SubProcesses/   $(PROCESS) $(LINKLIBS) 