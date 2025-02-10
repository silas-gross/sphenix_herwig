#ifndef __HERWIGSPECTRAEVALUATOR_H__
#define __HERWIGSPECTRAEVALUATOR_H__


//C++ libraries
#include <string>
#include <vector>
#include <array>
#include <iostream>
#include <fstream>
#include <utility>
#include <algorithim>
#include <map>
#include <math.h>

//root 
#include <TH1.h>
#include <TH2.h>
#include <TH3.h>
#include <TFile.h>
#include <TTree.h>
#include <TInterpreter.h>
#include <TF1.h>

//fun4all
#include <fun4all/SubsysReco.h>
#include <fun4all/Fun4AllBase.h>
#include <fun4all/Fun4AllReturnCodes.h>

class HerwigSpectraEvaluator : public SubsysReco
{
	public:
		enum SampleType{ //useful to tag the analysis
			Any,
			Jets, 
			Photons,
			Dijet,
			PhotonJet
		};
				
		HerwigSpectraEvaluator(); //
		~HerwigSpectraEvaluator() override;
		/* Stuff needed for any fun4All analysis module */
		void process_event() override; 
		
		//Unique to this type of analysis
		int setSampleType(int sampletype)
		{
			this->sample=sampletype;
			return this->sample;
		}
		
		int getSampleType(){ return this->sample; }
		void setInputFile(std::string input_file=""){ this->_input_file=input_file;}
		
	private:
		
		int sample=SampleType::Any;
		TH1F* h_E, *h_pt, *h_phi, *h_eta, *h_R, *h_nJet;
		TH1F* h_status, *h_pdg_final, *h_pdg_all, *h_pdg_initial; 
		TH1F* h_n_finalstate, *h_n_initialstate;
		TH1F* h_lead_E, *h_lead_pt, *h_lead_phi, *h_lead_eta	
		TH2F* h_eta_phi_N, *h_Energy_eta_phi, *h_p_eta_phi;
		TH3F* h_vertex, *h_hits;
		std::string _input_file=""; 
			
};
#endif
