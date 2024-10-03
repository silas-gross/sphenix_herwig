#include <Pythia8/Pythia.h>
#include <Pythia8Plugins/HepMC2.h>
#include <string>
R__LOAD_LIBRARY(libHepMC.so);
R__LOAD_LIBRARY(libpythia8.so);
int GetPythiaHepMC(std::string config_file="pythia_10GeV.cfg", std::string trigger="10GeV")
{
	std::cout<<"Setting up initial stages of the run" <<std::endl;
	std::string filename="sphenix_sigma_pythia_"+trigger+".hepmc";
	Pythia8::Pythia8ToHepMC hepmcoutput(filename);
	Pythia8::Pythia pythia;
	config_file="/sphenix/user/sgross/sphenix_herwig/herwig_files/"+config_file;
	pythia.readFile(config_file);
	pythia.init();
	std::cout<<"Have initialized the pythia setup" <<std::endl;
	std::string strippeddown="none";
	if(trigger.find("MB") == std::string::npos){
		auto p=trigger.find("GeV"); 
		strippeddown=trigger.erase(p);
	}
    	float ptmin=0.;
	if(strippeddown.find("none") == std::string::npos) ptmin=std::stof(strippeddown);
	for(int iEvent=0; iEvent < 50000; ++iEvent){
		std::cout<<"Generating on event " <<iEvent <<std::endl;
		if(!pythia.next()) continue;
		bool oneabove=false;
    		for(int i=0; i<pythia.event.size(); ++i){
            		int status=pythia.event[i].status();
            //cut on getting at least one parton above threshold i.e. Herwig cut
            		if(pythia.event[i].isParton() && status == -23 && pythia.event[i].pT() >=ptmin)
                    		oneabove=true;
            }
            //                                        //only keep events with the cut and generate a new event
                  if(!oneabove){
                         iEvent--;
                         continue;
                        }	
		hepmcoutput.writeNextEvent(pythia);
	}
	std::cout<<"Cross section is " <<pythia.info.sigmaGen() <<std::endl;
	std::cout<<"Generated 500,000 pythia events for the config file : " <<config_file <<std::endl;
	return 0;
}
