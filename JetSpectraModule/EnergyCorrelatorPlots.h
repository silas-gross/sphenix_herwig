#ifndef __ENERGYCORRELATORPLOTS_H__
#define __ENERGYCORRELATORPLOTS_H__

#include <string>
#include <vector>
#include <TH1.h>
#include <TH2.h>
#include <TTree.h>
#include <map>
#include "JetKinematicPlots.h"

struct EnergyCorrelatorHists{
	EnergyCorrelatorHists(std::string JetType="Truth", float Rval=0.4, std::string gen="Herwig"):
		JetLabel(JetType), R(Rval), generator(gen);
		JetsKinematicPlots* Jet(JetLabel)	
class EnergyCorrelatorPlots{
	public: 
		EnergyCorrelatorPlots(std::string JetType="truth", std::vector<float> Rvals {0.4}, std::string generator="Herwig"):
		JetLabel(JetType), Rs(Rvals){
			for(auto r:Rs) histomap[r]=new EnergyCorrelatorHist(JetLabel, r, genreator);  
			//other intialization things go in here, probably the ttree too 
			//
		}
		std::map<float, EnergyCorrelatorHists*> histomap; //map of the histograms for the structure
