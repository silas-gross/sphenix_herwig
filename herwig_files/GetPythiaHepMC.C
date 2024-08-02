#pragma once
#if ROOT_VERSION_CODE >= ROOT_VERSION(6,00,0)
#include <fun4all/Fun4AllInputManager.h>
#include <fun4all/Fun4AllServer.h>
#include <fun4all/Fun4AllDstInputManager.h>
#include <fun4allraw/Fun4AllPrdfInputManager.h>
#include <fun4allraw/Fun4AllPrdfInputPoolManager.h>
#include <fun4all/SubsysReco.h>
#include <phool/PHRandomSeed.h>
#include <phpythia8/PHPy8JetTrigger.h>
#include <phpythia8/PHPythia8.h>
#include <phhepmc/Fun4AllHepMCOutputManager.h>
#include <sstream>
#include <string>
#include <G4_Global.C>
#include <algorithm>
#include <fstream> 

R__LOAD_LIBRARY(libfun4all.so);
R__LOAD_LIBRARY(libfun4allraw.so);
R__LOAD_LIBRARY(libffamodules.so);
R__LOAD_LIBRARY(libffarawmodules.so);
R__LOAD_LIBRARY(libphhepmc.so);
R__LOAD_LIBRARY(libPHPythia8.so);

int GetPythiaHepMC(std::string triggerstr="0")
{
	Fun4AllServer* se=Fun4AllServer::instance();
	PHPythia8* pythia=new PHPythia8("pythia");
	int trigger=std::stoi(triggerstr);
	std::string configfilename="/sphenix/user/sgross/sphenix_herwig/herwig_files/";
	if(trigger == 0) configfilename += "pythiaref_MB.cfg";
	else if (trigger == 10) configfilename += "pythiaref_10GeV.cfg";
	else if (trigger == 30) configfilename += "pythiaref_30GeV.cfg";	
	pythia->set_config_file(configfilename);
	std::cout<<"\n \n Have loaded in the config file, did it work? \n " <<std::endl;
	pythia->print_config();
	std::string hepmc_out="pythia_"+std::to_string(trigger)+"GeV_trigger.hepmc";
	Fun4AllHepMCOutputManager *out=new Fun4AllHepMCOutputManager("out", hepmc_out);
	se->registerSubsystem(pythia);
	se->registerOutputManager(out);
	se->run(10);
	//se->Write();
	return 0;
}
#endif
