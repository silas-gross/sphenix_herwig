// Tell emacs that this is a C++ source
//  -*- C++ -*-.
#ifndef HERWIGJETSPECTRA_H
#define HERWIGJETSPECTRA_H
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

#include <fun4all/SubsysReco.h>
#include <fun4all/Fun4AllBase.h>
#include <fun4all/Fun4AllInputManager.h>
#include <fun4all/Fun4AllReturnCodes.h>
#include <ffaobjects/EventHeaderv1.h>
#include <Event/Event.h>
#include <Event/EventTypes.h>

#include <string>
#include <vector>
#include <map>
#include <math.h>
#include <unordered_set>
#include <unordered_map>
#include <TH1.h>
#include <TFile.h>
#include <TH2.h>
#include <utility> 
#include <iterator> 
#include <algorithm>
#include <thread>

#include <g4main/PHG4VtxPoint.h>
#include <g4main/PHG4TruthInfoContainer.h>
#include <g4main/PHG4Particle.h>
#include <g4main/PHG4Hit.h>
#include <g4main/PHG4HitContainer.h>

#include <phhepmc/PHHepMCGenEvent.h>
#include <phhepmc/PHHepMCGenEventMap.h>
#include <HepMC/GenEvent.h>

#include <phool/phool.h>
#include <phool/getClass.h>
#include <phool/PHCompositeNode.h>
#include <phool/PHObject.h>


#include <jetbase/JetContainer.h>
#include <jetbase/Jet.h>
#include <jetbase/JetAlgo.h>
#include <jetbase/FastJetAlgo.h>
#include <fastjet/JetDefinition.hh>
#include <fastjet/PseudoJet.hh>

#include <phpythia8/PHPythia8.h>
#include <phpythia8/PHPy8JetTrigger.h>

#include "Jet_Obj_Defs.h"
#include "JetKinematicPlots.h"
#include "EventKinematicPlots.h"
//#include "EnergyCorrelatorPlots.h"

class PHCompositeNode;
//class fastjet::PseudoJet;
//class fastjet::GridMedianBackgroundEstimator;
//class fastjet::SelectorPtMax;
//class contrib::ConsitiuentSubtractor

class PHCompositeNode;
class PHPythia8; 
class HerwigJetSpectra : public SubsysReco
{
 public:

  HerwigJetSpectra(bool run_pythia=false, bool pythia_file=false, int verbosity=0, const std::string &name = "HerwigJetSpectra", 
			const std::string &fname="HerwigJetSpectra.root")
{
	std::cout <<"This is running on the "<<name<<" module with jet trigger at " <<trig<<std::endl;
/*<<<<<<< HEAD this is the old way of structureing the plots, need to update things
	h_phi=new TH1F("phi", "Transverse Energy #varphi distribution of all particles produced in the decay chain of Hepmc record; #varphi; #sum_{particles} E_{T} [GeV]", 64, -3.1416, 3.14);
	h_eta=new TH1F("eta", "Transverse Energy #eta distribution of all particles produced in decay chain of Hepmc record in sPHENIX acceptance; #eta; #sum_{particles} E_{T} [GeV]", 24, -1.12, 1.1); 
	h_phi_hit=new TH1F("phi_hit", "hit distribution #varphi distribution of all particles produced in the decay chain of Hepmc record; #varphi", 64, -3.1416, 3.14);
	h_eta_hit=new TH1F("eta_hit", "hit #eta distribution of all particles produced in decay chain of Hepmc record in sPHENIX acceptance; #eta", 24, -1.12, 1.1); 
	h_pt=new TH1F("pt", "p_{T} distribution of final state particles; p_{T} [GeV]; N_{part}", 1000,-0.5, 49.5);
	h_n_part=new TH1F("n_part", "Number of total particles produced in decay chain of a Hepmc event; n_{part}; N_{event}", 1000, -0.5, 999.5);
	h_mass=new TH1F("mass", "Particle masses of final state particles; m [GeV]; N_{part}", 100, 0, 1.5);
	h_E=new TH1F("energy", "Energy of all particles in decay chain; E [GeV]; N_{part}", 200, -0.5, 199.5);
	h_ET=new TH1F("transverse_energy", "Transverse Energy of final state particles; E_{T} [GeV]; N_{part}", 100, -0.5, 49.5);
	h_status=new TH1F("status", "HepMC status of final state particles; status", 30, -0.5, 29.5);
	h_phi_orig=new TH1F("phi_orig", "#varphi distribution of primary particles produced in Hepmc record; #varphi; #sum_{particles} E_{T} [GeV]", 64, -3.1416, 3.14);
	h_eta_orig=new TH1F("eta_orig", "#eta distribution of primary particles produced in decay chain of Hepmc record in sPHENIX acceptance; #eta; #sum_{particles} E_{T} [GeV]", 24, -1.12, 1.1); 
	h_phi_hit_orig=new TH1F("phi_hit_orig", "hit #varphi distribution of primary particles produced in Hepmc record; #varphi; N_{hits}", 64, -3.1416, 3.14);
	h_eta_hit_orig=new TH1F("eta_hit_orig", "#eta distribution of primary partons produced Hepmc record in sPHENIX acceptance; #eta; N_{hits}", 24, -1.12, 1.1); 
	h_pt_orig=new TH1F("pt_orig", "p_{T} distribution of primary particles produced in decay chain of Hepmc record; p_{T} [GeV]; N_{part}", 1000,-0.5, 99.5);
	h_n_part_orig=new TH1F("n_part_orig", "Number of total primary particles produced in decay chain of a Hepmc event; n_{part}; N_{event}", 1000, -0.5, 999.5);
	h_mass_orig=new TH1F("mass_orig", "Particle masses of primary particle in decay chain of Hepmc record; m [GeV]; N_{part}", 100, 0, 1.5);
	h_status_orig=new TH1F("status_orig", "HepMC status of primary partons; status", 30, -0.5, 29.5);
	h_E_orig=new TH1F("energy_orig", "Energy of primary partons; E[GeV]; N_{part}", 200, -0.5, 199.5);
	h_ET_orig=new TH1F("transverse_energy_orig", "Transverse Energy of primary partons; E_{T} [GeV]; N_{part}", 100, -0.5, 49.5);
	h_pt_leading=new TH1F("pt_leading", "p_{T} of leading primary particle in Hepmc record; p_{T}[GeV]; N_{part}", 1000, -0.5, 49.5);
	h_E_total=new TH1F("energy_total", "Total energy of event; E[GeV]; N_{event}", 200, 149.5, 249.5);
	h_vertex=new TH2F("vertex", "Vertex position; r [cm]; z[cm]; N_{event}", 100, -0.1, 0.1, 100, -0.1, 0.1);
	h_ev=new TH1F("event", "Number of HepMC Events per Fun4All event; N_{event}", 10, -0.5, 9.5); 
	h_weight=new TH1F("weights", "HEPMC event weights", 100, -0.5, 1.5); 
	h_hits=new TH2F("hits", "N final state particles; #eta; #phi; N_{particles}", 24, -1.1, 1.1, 64, -3.1416, 3.1414);
	h_hits_orig=new TH2F("hits_orig", "N inital state partons; #eta; #phi; N_{partons}", 24, -1.1, 1.1, 64, -3.1416, 3.1414);
	h_Jet_pt_lead=new TH1F("jet_pt_leading", "p_{T} of identified leading jets in final state; p_{T} [GeV]; Counts", 100, -0.5, 49.5);
	h_pt_R=new TH2F("jet_pt_R", "p_{T} Distribution of particles in jet as a function of R from jet center; R;#frac{p_{T}}{max(p_{T})}; Counts", 200, -0.01, 1.99, 1000, -0.1, 19.9);
	h_Jet_pt=new TH1F("jet_pt", "p_{T} of identified jets in final state; p_{T} [GeV]; Counts", 100, -0.5, 49.5);
	h_Jet_R=new TH1F("jet_R", "R of identified jets in final state, measured from max seperation of originating parton; R; Counts", 30, -0.1, 2.9);
	h_Jet_npart=new TH1F("jet_npart", "Number of particles in final state of jet; N_{particles}; N_{Jets}", 200, -0.5, 199.5); */
	h_e2c=new TH1F("e2c", "2 Point energy correlator averaged over all jets; R_{L}; E2C", 200, -0.01, 1.99);
	h_e3c=new TH1F("e3c", "3 Point energy correlator averaged over all jets; R_{L}; E3C", 200, -0.01, 1.99);
	h_e2c_q=new TH1F("e2c_q", "2 Point energy correlator averaged over alli quark jets; R_{L}; E2C", 200, -0.01, 1.99);
	h_e3c_q=new TH1F("e3c_q", "3 Point energy correlator averaged over all quark jets; R_{L}; E3C", 200, -0.01, 1.99);
	h_e2c_g=new TH1F("e2c_g", "2 Point energy correlator averaged over all gluon jets; R_{L}; E2C", 200, -0.01, 1.99);
	h_e3c_g=new TH1F("e3c_g", "3 Point energy correlator averaged over all gluon jets; R_{L}; E3C", 200, -0.01, 1.99);
	h_e2c_2=new TH1F("e2c_2", "2 Point energy correlator averaged over all jets restricted to size R=0.2; R_{L}; E2C", 200, -0.01, 1.99);
	h_e3c_2=new TH1F("e3c_2", "3 Point energy correlator averaged over all jets restricted to size R=0.2; R_{L}; E3C", 200, -0.01, 1.99);
	h_e2c_4=new TH1F("e2c_4", "2 Point energy correlator averaged over all jets restricted to size R=0.4; R_{L}; E2C", 200, -0.01, 1.99);
	h_e3c_4=new TH1F("e3c_4", "3 Point energy correlator averaged over all jets restriced to size R=0.4; R_{L}; E3C", 200, -0.01, 1.99);
	h_e2c_6=new TH1F("e2c_6", "2 Point energy correlator averaged over all jets restriced to size R=0.6; R_{L}; E2C", 200, -0.01, 1.99);
	h_e3c_6=new TH1F("e3c_6", "3 Point energy correlator averaged over all jets restriced to size R=0.6; R_{L}; E3C", 200, -0.01, 1.99);
	h_e2ct=new TH1F("e2ct", "Integrated 2 Point energy correlator averaged over all jets;#int_{R_{L}} E2C", 50, -0.01, 0.6);
	h_e3ct=new TH1F("e3ct", "Integrated 3 Point energy correlator averaged over all jets; #int_{R_{L}} E3C", 50, -0.01, 0.6);
	h_e2ct_2=new TH1F("e2ct_2", "Integrated 2 Point energy correlator averaged over all jets restricted to size R=0.2; R_{L}; E2C", 2000, -0.01, 199.9);
	h_e3ct_2=new TH1F("e3ct_2", "Integrated 3 Point energy correlator averaged over all jets restricted to size R=0.2; R_{L}; E3C", 2000, -0.01, 1.99);
	h_e2ct_4=new TH1F("e2ct_4", "Integrated 2 Point energy correlator averaged over all jets restricted to size R=0.4; R_{L}; E2C", 2000, -0.01, 199.9);
	h_e3ct_4=new TH1F("e3ct_4", "Integrated 3 Point energy correlator averaged over all jets restriced to size R=0.4; #int{R_{L}} E3C", 2000, -0.1, 199.9);
	h_e2ct_6=new TH1F("e2ct_6", "Integrated 2 Point energy correlator averaged over all jets restriced to size R=0.6;#int_{R_{L}}   E2C", 2000, -0.1, 199.9);
	h_e3ct_6=new TH1F("e3ct_6", "Integrated 3 Point energy correlator averaged over all jets restriced to size R=0.6;#int_{R_{L}} E3C", 2000, -0.01, 199.9);
	for(int i=0; i<11; i++)
	{
		float r=i/10.;
		h_E2C_IC[r]=new TH1F(Form("IC_e2c_%d", i), Form("2 Point Iterative Cone with Progressive Removal R=%f;R_{L}; E2C",r), 200, -0.01, 1.99 );
		h_E3C_IC[r]=new TH1F(Form("IC_e3c_%d", i), Form("3 Point Iterative Cone with Progressive Removal R=%f;R_{L}; E3C", r), 200, -0.01, 1.99 );
		h_E2CT_IC[r]=new TH1F(Form("IC_e2ct_%d", i), Form("Integrated 2 Point Iterative Cone with Progressive Removal R=%f;R_{L}; E2C",r), 50, -0.01, 0.6  );
		h_E3CT_IC[r]=new TH1F(Form("IC_e3ct_%d", i), Form("Intgrated 3 Point Iterative Cone with Progressive Removal R=%f; #int_{R_{L}} E3C", r), 50, -0.01, 0.6 );
	}
	n_evt=0;
	this->do_pythia = run_pythia;
	this->seperate_pythia_file = pythia_file;
	this->verbosity = verbosity;
	if(!seperate_pythia_file) this->HerwigKin=new EventKinematicPlots("Herwig");
	else this->HerwigKin=new EventKinematicPlots("Pythia");
//	HerwigTree=new TTree("Herwig_tree", "Data Tree for Herwig Generated Events");
	if(run_pythia && !seperate_pythia_file){
		this->PythiaKin=new EventKinematicPlots("Pythia");
//		PythiaTree=new TTree("Pythia_tree", "Data Tree for Pythia Generated Events");
//		this->RatioKin=new JetKinematicPlots("Ratio");
		}
	}
	//truth_ec_plots = new EnergyCorrelatorPlots("truth");
	//ICPR_ec_plots =  new EnergyCorrelatorPlots("ICPR" );
  ~HerwigJetSpectra() override;
  //void BuildDataTree(TTree* data_tree){
	//just build the specific trees for the specific generator 
	//should include info that I would want event by event
	//pdg info:
	//	each particle store the 
	//	pdg info 
	//	p_t
	//	energy
	//n particles --integer
	//n jets -- integer
	//jet info:
	//	use the structure from below
	//	pt
	//	mass 
	//	phi 
	//	eta
	//	R
	//	eta width
	//	phi width
	//	originating particle
	//	n final states
  /** Called during initialization.
      Typically this is where you can book histograms, and e.g.
      register them to Fun4AllServer (so they can be output to file
      using Fun4AllServer::dumpHistos() method).
   */
  int Init(PHCompositeNode *topNode) override;

  /** Called for first event when run number is known.
      Typically this is where you may want to fetch data from
      database, because you know the run number. A place
      to book histograms which have to know the run number.
   */
  int InitRun(PHCompositeNode *topNode) override;

  /** Called for each event.
      This is where you do the real work.
   */
  int process_event(PHCompositeNode *topNode) override; //this allow for just checking the number of events in the file as it seems off
  int getKinematics(PHCompositeNode *topNode, EventKinematicPlots*, bool); 
  /// Clean up internals after each event.
  int ResetEvent(PHCompositeNode *topNode) override;

  /// Called at the end of each run.
  int EndRun(const int runnumber) override;

  /// Called at the end of all processing.
  int End(PHCompositeNode *topNode) override;

  /// Reset
  	int Reset(PHCompositeNode * /*topNode*/) override;

  	void Print(const std::string &what = "ALL") const override;
  	float getPt(HepMC::GenParticle*);
  	float getR(HepMC::GenParticle*, HepMC::GenParticle*);
	float getE2C(std::vector<HepMC::GenParticle*>, TH1F*, float=0);
	float getE3C(std::vector<HepMC::GenParticle*>, TH1F*);
  	std::vector<HepMC::GenParticle*> IDJets(PHCompositeNode *topNode, HepMC::GenParticle* ); 
	float GetAnIterativeCone(std::vector<HepMC::GenParticle*> *, float, bool, float, JetCollection*);
	int fastjetID( std::vector<HepMC::GenParticle*>, JetCollection*, int, float, float );
  	PHPythia8* PythiaGenerator(PHCompositeNode *topNode, int trigger);
  	PHCompositeNode* pythiaNode;
  	std::vector<HepMC::GenParticle*> IDJets(PHCompositeNode *topNode, HepMC::GenParticle*, bool); 
  	std::string trig="MB";
  	int trig_val=0;
  	int n_evt=0;
  	bool do_pythia;
	bool seperate_pythia_file;
  	int verbosity;
 private:
//	TTree* HerwigTree, PythiaTree; //this will be a later upgrade
	PHPythia8* pythiagen;
	EventKinematicPlots* HerwigKin, *PythiaKin/*, *RatioKin*/; 
	JetKinematicPlots* JetKin;
	/*struct jetobj{
		std::vector<HepMC::GenParticle*> jet_particles;
		HepMC::GenParticle* originating_parton;
		float pt;
		float mass;
		float ET;
		std::string parton_name;
		int part_id; //=originating_parton->pdg_id();
		float R;
		float phi;
		float eta;
	};*/
	TH1F *h_e2c, *h_e3c, *h_e2c_q, *h_e3c_q, *h_e2c_g, *h_e3c_g;
	TH1F *h_e2c_2, *h_e3c_2, *h_e2c_4, *h_e3c_4, *h_e2c_6, *h_e3c_6;
	TH1F *h_e2ct, *h_e3ct, *h_e2ct_2, *h_e3ct_2, *h_e2ct_4, *h_e3ct_4, *h_e2ct_6, *h_e3ct_6;
	std::map<float, TH1F*> h_E2C_IC, h_E3C_IC, h_E2CT_IC, h_E3CT_IC;

	//EnergyCorrelatorPlot* truth_plots, ICPR_plots;
};

#endif // HERWIGJETSPECTRA_H
