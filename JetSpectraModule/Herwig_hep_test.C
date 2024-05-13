#pragma once
#if ROOT_VERSION_CODE >= ROOT_VERSION(6,00,0)
#include "HerwigJetSpectra.h"
#include "sPhenixStyle.h"
#include "sPhenixStyle.C"
#include <fun4all/Fun4AllInputManager.h>
#include <fun4all/Fun4AllServer.h>
#include <phhepmc/Fun4AllHepMCInputManager.h>
#include <fun4all/Fun4AllDstInputManager.h>
#include <fun4allraw/Fun4AllPrdfInputManager.h>
#include <fun4allraw/Fun4AllPrdfInputPoolManager.h>
#include <fun4all/SubsysReco.h>
#include <phool/PHRandomSeed.h>
#include <sstream>
#include <string.h>
#include <G4_Global.C>
#include <algorithm>
#include <fstream> 

R__LOAD_LIBRARY(libfun4all.so);
R__LOAD_LIBRARY(libfun4allraw.so);
R__LOAD_LIBRARY(libHerwigJetSpectra.so);
R__LOAD_LIBRARY(libffamodules.so);
R__LOAD_LIBRARY(libffarawmodules.so);
R__LOAD_LIBRARY(libphhepmc.so);

int Herwig_hep_test(std::string filename="/sphenix/user/sgross/sphenix_herwig/herwig_files/sphenix_10GeV_jetpt.hepmc")
{
	SetsPhenixStyle();
	Fun4AllServer* se=Fun4AllServer::instance();
	Fun4AllHepMCInputManager *in =new Fun4AllHepMCInputManager("in");
	//std::fstream f;
	//f.open(filename);
	std::string fn, temp, type="MB";
	std::stringstream ss (filename);
	while(std::getline(ss, temp, '_')){
		if(temp.find("GeV") != std::string::npos) type=temp;
	}
	se->registerInputManager(in);
        se->fileopen(in->Name().c_str(), filename);
	HerwigJetSpectra* ts=new HerwigJetSpectra("HerwigJetSpectra");
	ts->trig=type;
	std::cout<<"The spectra analyzer is running over generators with jet trigger set to " <<ts->trig <<std::endl;
	se->registerSubsystem(ts);
	std::cout<<"Is the random seed here?" <<std::endl;	
	//want to load in the random seed ideally
	se->run();
//	ts->Print();
	std::cout<<"Ran over "<<ts->n_evt<<" events" <<std::endl;
	return 0;
}	
#endif
