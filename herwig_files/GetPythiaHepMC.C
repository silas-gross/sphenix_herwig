#include <Pythia8/Pythia.h>
#include <Pythia8Plugins/HepMC2.h>
#include <string>
R__LOAD_LIBRARY(libHepMC.so);
R__LOAD_LIBRARY(libpythia8.so);
int GetPythiaHepMC(std::string config_file="pythia_10GeV.cfg", std::string trigger="10GeV")
{
	std::cout<<"Setting up initial stages of the run" <<std::endl;
	std::string filename="sphenix_pythia_"+trigger+".hepmc";
	Pythia8::Pythia8ToHepMC hepmcoutput(filename);
	Pythia8::Pythia pythia;
	config_file="/sphenix/user/sgross/sphenix_herwig/herwig_files/"+config_file;
	pythia.readFile(config_file);
	pythia.init();
	std::cout<<"Have initialized the pythia setup" <<std::endl;
	for(int iEvent=0; iEvent < 500000; ++iEvent){
		std::cout<<"Generating on event " <<iEvent <<std::endl;
		if(!pythia.next()) continue;
		hepmcoutput.writeNextEvent(pythia);
	}
	std::cout<<"Generated 500,000 pythia events for the config file : " <<config_file <<std::endl;
	return 0;
}
