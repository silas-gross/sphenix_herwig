#ifndef __HERWIGJET_H__
#define __HERWIGJET_H__

#include <ThePEG/Repository/EventGenerator.h>
#include <ThePEG/Repository/Repository.h>
#include <ThePEG/Persistency/PersistendIStream.h>
#include <ThePEG/Utilities/DynamicLoader.h>
#include <ThePEG/EventRecord/Event.h>
#include <ThePEG/EventRecord/Subprocess.h>
#include <ThePEG/Handlers/XComb.h>
#include <ThePEG/Handlers/EventHandler.h>
#include <ThePEG/PDF/PartonExtractor.h>
#include <ThePEG/PDF/PDF.h>
#include <ThePEG/Vectors/HepMCConverter.h>
#include <ThePEG/Cuts/JetFinder.h>
#include <ThePEG/Cuts/JetRegion.h>
#include <ThePEG/Cuts/Cuts.h>

#include <HepMC/GenEvent.h>
#include <HepMC/IO_GenEvent.h>
#include <HepMC/IO_AsciiParticles.h>
#include <HepMC/SimpleVector.h>

#include <iostream>
#include <string>
#include <stdio.h>
//This is a class to allow for the generation of Jet samples from herwig, producing hepmc file records
//ideally this will be modified to allow for integration with Fun4ALL directly

class HerwigJet{
	public:
		HerwigJet(std::string trigger_type="MB", int events=10000, float pt_min=0){
			//Constructor to set the trigger type (jet, minimum bias or photon-jet)
			//sets number of events, default 10k
			//if it is a jet inclusive, sets minimum jet pt to trigger on 
			
			//Default to minimum bias
			this->trigger_type=trigger_type;
			this->events=events;
			this->pt_min=pt_min;
		}
		~HerwigJet(){std::cout<<"Destructing HerwigJet instance"<<std::endl;}
		std::string trigger_type="MB";
		int events=10000;
		float pt_min=0.0;
		float smear=0.0; //to allow for later smearing of vertex dist
};
#endif
