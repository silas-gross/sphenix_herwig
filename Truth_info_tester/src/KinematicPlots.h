#ifndef __KINEMATICPLOTS_H__
#define __KINEMATICPLOTS_H__

#include <string>
#include <vector>

#include <TH1.h>
#include <TH2.h>
#include <TH3.h>
#include <TTree.h>

class KinematicPlots
{
//class for general storing of data and histograms for easier comparison afterwards
  public:
	KinematicPlots(bool truth_or_reco=false, int run_num=0){
		//make the plots and tag them correctly 
		this->run_number=run_num;
		this->truth=truth_or_reco;
		if(truth) this->truth_tag="TRUTH";
		else this->truth_tag="RECO";
		
	}
	~KinematicPlots();	
  private:
	std::string truth_tag, run_number_tag;
	int run_number;
	bool truhth;
	std::vector<TH1F*> OneDHistograms;
	std::vector<TH2F*> TwoDHistograms;
	std::vector<TH3F*> ThreeDHistograms;
	TTree* 
	
}

