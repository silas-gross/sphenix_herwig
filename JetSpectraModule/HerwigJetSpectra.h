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
//#include <fastjet/SISConePlugin.hh>
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
	h_e2c=new TH1F("e2c", "2 Point energy correlator averaged over all jets; R_{L}; E2C", 200, -0.01, 1.99);
	h_e3c=new TH1F("e3c", "3 Point energy correlator averaged over all jets; R_{L}; E3C", 200, -0.01, 1.99);
	h_pt_q=new TH1F("h_pt_q", "p_{T} of quark initated jets; p_{T} [GeV/c]; N_{jets}", 100, -0.5, 99.5);
	h_pt_g=new TH1F("h_pt_g", "p_{T} of gluon initated jets; p_{T} [GeV/c]; N_{jets}", 100, -0.5, 99.5);
	h_n_q=new TH1F("h_n_q", "Number of quark jets per event ; N_{jets}; N_{event}", 20, -0.5, 19.5);
	h_n_g=new TH1F("h_n_g", "Number of gluon jets per event ; N_{jets}; N_{event}", 20, -0.5, 19.5);
	h_n_qg=new TH1F("h_n_qg", "Ratio of number of quark jets to gluon jets per event ; #frac{N_{q jets}}{N_{g jets}}; N_{event}", 50, -0.05, 4.95);
	h_n_qg_pt=new TH2F("h_n_qg_pt", "Ratio of number of quark jets to gluon jets as a functio of lead p_{T} per event ; p_{T} [GeV/c]; #frac{N_{q jets}}{N_{g jets}}; N_{event}", 100, -0.5, 99.5, 50, -0.05, 4.95);
	h_qg_Aj=new TH1F("h_qg_Aj", "A_{jj} for quark-gluon dijets ; A_{jj}; N_{event}", 50, -0.05, 1);
	h_qg_pt=new TH1F("h_qg_pt", "p_{T} leading for quark-gluon dijets ; p_{T} [GeV/c]; N_{event}", 100, -0.05, 99.5);
	h_qg_xj=new TH1F("h_qg_xj", "x_{j} for quark-gluon dijets ; x_{j}; N_{event}", 50, -0.05, 1);
	h_gg_Aj=new TH1F("h_gg_Aj", "A_{jj} for gluon-gluon dijets ; A_{jj}; N_{event}", 50, -0.05, 1);
	h_gg_pt=new TH1F("h_gg_pt", "p_{T} leading for gluon-gluon dijets ; p_{T} [GeV/c]; N_{event}", 100, -0.05, 99.5);
	h_gg_xj=new TH1F("h_gg_xj", "x_{j} for gluon-gluon dijets ; x_{j}; N_{event}", 50, -0.05, 1);
	h_qq_Aj=new TH1F("h_qq_Aj", "A_{jj} for quark-quark dijets ; A_{jj}; N_{event}", 50, -0.05, 1);
	h_qq_pt=new TH1F("h_qq_pt", "p_{T} leading for quark-quark dijets ; p_{T} [GeV/c]; N_{event}", 100, -0.05, 99.5);
	h_qq_xj=new TH1F("h_qq_xj", "x_{j} for quark-quark dijets ; x_{j}; N_{event}", 50, -0.05, 1);
	h_e2c_q=new TH1F("e2c_q", "2 Point energy correlator averaged over all quark jets; R_{L}; E2C", 200, -0.01, 1.99);
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
		h_E3CT_IC[r]=new TH1F(Form("IC_e3ct_%d", i), Form("Intgrated 3 Point Iterative Cone with Progressive Removal R=%f; #int_{R_{L}} E3C", r), 50, -0.01, 0.6);
/*		h_E2C_SIS[r]=new TH1F(Form("SIS_e2c_%d", i), Form("2 Point SIS Cone R=%f;R_{L}; E2C",r), 200, -0.01, 1.99 );
		h_E3C_SIS[r]=new TH1F(Form("SIS_e3c_%d", i), Form("3 Point SIS Cone R=%f;R_{L}; E3C", r), 200, -0.01, 1.99 );
		h_E2CT_SIS[r]=new TH1F(Form("SIS_e2ct_%d", i), Form("Integrated 2 Point SIS Cone R=%f;R_{L}; E2C",r), 50, -0.01, 0.6  );
		h_E3CT_SIS[r]=new TH1F(Form("SIS_e3ct_%d", i), Form("Intgrated 3 Point SIS Cone R=%f; #int_{R_{L}} E3C", r), 50, -0.01, 0.6 );*/
		h_E2C_kt[r]=new TH1F(Form("kt_e2c_%d", i), Form("2 Point kT  R=%f;R_{L}; E2C",r), 200, -0.01, 1.99 );
		h_E3C_kt[r]=new TH1F(Form("kt_e3c_%d", i), Form("3 Point kT R=%f;R_{L}; E3C", r), 200, -0.01, 1.99 );
		h_E2CT_kt[r]=new TH1F(Form("kt_e2ct_%d", i), Form("Integrated 2 Point kT R=%f;R_{L}; E2C",r), 50, -0.01, 0.6  );
		h_E3CT_kt[r]=new TH1F(Form("kt_e3ct_%d", i), Form("Intgrated 3 Point kT R=%f; #int_{R_{L}} E3C", r), 50, -0.01, 0.6 );
		h_E2C_antikt[r]=new TH1F(Form("antikt_e2c_%d", i), Form("2 Point anti-kT R=%f;R_{L}; E2C",r), 200, -0.01, 1.99 );
		h_E3C_antikt[r]=new TH1F(Form("antikt_e3c_%d", i), Form("3 Point anti-kT R=%f;R_{L}; E3C", r), 200, -0.01, 1.99 );
		h_E2CT_antikt[r]=new TH1F(Form("antikt_e2ct_%d", i), Form("Integrated 2 Point anti-kT R=%f;R_{L}; E2C",r), 50, -0.01, 0.6  );
		h_E3CT_antikt[r]=new TH1F(Form("antikt_e3ct_%d", i), Form("Intgrated 3 Point anti-kT R=%f; #int_{R_{L}} E3C", r), 50, -0.01, 0.6 );
		h_E2C_cambridge[r]=new TH1F(Form("cambridge_e2c_%d", i), Form("2 Point Cambridge/Achen Algo R=%f;R_{L}; E2C",r), 200, -0.01, 1.99 );
		h_E3C_cambridge[r]=new TH1F(Form("cambridge_e3c_%d", i), Form("3 Point Cambridge/Achen Algo R=%f;R_{L}; E3C", r), 200, -0.01, 1.99 );
		h_E2CT_cambridge[r]=new TH1F(Form("cambridge_e2ct_%d", i), Form("Integrated 2 Point Cambridge/Achen Algo R=%f;R_{L}; E2C",r), 50, -0.01, 0.6  );
		h_E3CT_cambridge[r]=new TH1F(Form("cambridge_e3ct_%d", i), Form("Intgrated 3 Point Cambridge/Achen Algo R=%f; #int_{R_{L}} E3C", r), 50, -0.01, 0.6 );
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
	void runFastJet(std::vector<HepMC::GenParticle*>, fastjet::JetDefinition, float, JetCollection*);
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
	TH1F *h_pt_q, *h_pt_g, *h_n_q, *h_n_g, *h_n_qg;
	TH1F *h_qg_Aj, *h_qg_xj, *h_qg_pt;
	TH1F *h_gg_Aj, *h_gg_xj, *h_gg_pt;
	TH1F *h_qq_Aj, *h_qq_xj, *h_qq_pt;
	TH2F *h_n_qg_pt;
	TH1F *h_e2c, *h_e3c, *h_e2c_q, *h_e3c_q, *h_e2c_g, *h_e3c_g;
	TH1F *h_e2c_2, *h_e3c_2, *h_e2c_4, *h_e3c_4, *h_e2c_6, *h_e3c_6;
	TH1F *h_e2ct, *h_e3ct, *h_e2ct_2, *h_e3ct_2, *h_e2ct_4, *h_e3ct_4, *h_e2ct_6, *h_e3ct_6;
	std::map<float, TH1F*> h_E2C_IC, h_E3C_IC, h_E2CT_IC, h_E3CT_IC;
//	std::map<float, TH1F*> h_E2C_SIS, h_E3C_SIS, h_E2CT_SIS, h_E3CT_SIS;
	std::map<float, TH1F*> h_E2C_kt, h_E3C_kt, h_E2CT_kt, h_E3CT_kt;
	std::map<float, TH1F*> h_E2C_antikt, h_E3C_antikt, h_E2CT_antikt, h_E3CT_antikt;
	std::map<float, TH1F*> h_E2C_cambridge, h_E3C_cambridge, h_E2CT_cambridge, h_E3CT_cambridge;

	//EnergyCorrelatorPlot* truth_plots, ICPR_plots;
};

#endif // HERWIGJETSPECTRA_H
