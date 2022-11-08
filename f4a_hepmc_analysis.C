#include <fun4all/Fun4AllInputManager.h>
//#include <stdio>
//#include <iostream>
//#include <fstream>

//Silas--3 November
void f4a_hepmc_analysis()
{
	//The idea here is just to first run ove the hepmc with a macro
	//I think for more advanced analysis there may already be a macro somewhere in mdc2
	//not sure if I am comparable to the pythia file yet, need to check again all calibrations
	//std::ifstream hep_input=open("first_sphenix.hepmc");
	Fun4AllInputManager *in = new Fun4AllHepMCInputManager("DSTIN");
	se->registerInput(in);
	in->fileopen("first_sphenix.hepmc");
}  
