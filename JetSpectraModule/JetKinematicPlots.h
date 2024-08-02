#ifndef __Jet_KINEMATIC_PLOTS_H__
#define __Jet_KINEMATIC_PLOTS_H__

#include <string>
#include <TH1.h>
#include <TH2.h>
#include <map>

struct JetKinematicPlots{
	JetKinematicPlots(std::string type="Truth", std::string generator="Herwig", float r=0.4, bool full=false): TypeLabel(type), Generator(generator), R(r), FullJet(full);
	float R=0.4;
	bool FullJet=false;
	std::string TypeLabel="Truth";
	std::string JetLabel=" "+TypeLabel+" Jets ";
	if(FullJet) JetLabel+="full size jet";
	else JetLabel+="R = "+std::to_string(R);
	std::string r_str=std::to_string( ((int) R*10));
	std::string Generator="Herwig";
	std::string gen_label=generator+JetLabel;
	std::string gen=Generator+TypeLabel;
	TH1F* h_Jet_pt=new TH1F(Form("jet_pt_%s_%s",  gen.c_str(), r_str.c_str()), Form("p_{T} of identified jets in final state %s %d; p_{T} [GeV]; Counts", gen_label.c_str(), R), 100, -0.5, 49.5);
	TH1F* h_Jet_et=new TH1F(Form("jet_et_%s_%s",  gen.c_str(), r_str.c_str()), Form("E_{T} of identified jets in final state %s %d; E_{T} [GeV]; Counts", gen_label.c_str(), R), 100, -0.5, 49.5);
	TH1F* h_Jet_m=new TH1F(Form("jet_m_%s_%s",  gen.c_str(), r_str.c_str()), Form("Mass of identified jets in final state %s %d; |m| [GeV]; Counts", gen_label.c_str(), R), 100, -0.5, 49.5);
	TH1F* h_Jet_npart=new TH1F(Form("jet_npart_%s_%s", gen.c_str(), r_str.c_str()), Form("Number of final state partons at R, averaged over number of jets %s %d; R; N_{Part}", gen_label.c_str(), R), 100, -0.02, 1.8); 
	TH1F* h_Jet_R=new TH1F(Form("jet_R_%s_%s",  gen.c_str(), r_str.c_str()), Form("R of identified jets in final state, measured from max seperation of originating parton %s of size %d; R; Counts", gen_label.c_str(), R), 100, -0.1, 9.9);
	TH1F* h_orig_part=new TH1F(Form("pdg_%s_%s",  gen.c_str(), r_str.c_str()), Form("Particle id of final state particles in %s of size %d ; pdg id; N_{part}", gen_label.c_str(), R), 4600, -2300.5, 2299.5);
 	TH2F* h_pt_R=new TH2F(Form("pt_R_%s_%s", gen.c_str(), r_str.c_str()), Form("p_{T} versus R for particles measured from the highest p_{T} component in %s of size %d; R; p_{T} [GeV]; <N>", gen_label.c_str(), R), 50, -0.05, 1.95, 100, -0.01, 0.99);
	TH2F* h_phi_eta=new TH2F(Form("eta_phi_%s_%s", gen.c_str(), r_str.c_str()), Form("Jet center in %s of size %d; #eta; #varphi; N_{jet}", gen_label.c_str(), R), 96, -1.11, 1.09, 128, -3.1416, 3.14);
	std::map<std::string, TH1F*> OneDPlots {{"pt", h_Jet_pt},{"et", h_Jet_et}, {"m", h_Jet_m}, {"n", h_Jet_npart}, {"R", h_Jet_R}, {"PDGID", h_orig_part}};
	std::map<std::string, TH2F*> TwoDPlots {{"pt R", h_pt_R}, {"phi eta", h_phi_eta}};
	};
#endif		
