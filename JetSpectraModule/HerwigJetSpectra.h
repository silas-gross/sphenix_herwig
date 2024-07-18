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

#include <phpythia8/PHPythia8.h>
#include <phpythia8/PHPy8JetTrigger.h>

#include "JetKinematicPlots.h"

class PHCompositeNode;
class PHPythia8; 
class HerwigJetSpectra : public SubsysReco
{
 public:

  HerwigJetSpectra(bool run_pythia=false, int verbosity=0, const std::string &name = "HerwigJetSpectra", 
			const std::string &fname="HerwigJetSpectra.root")
{
	std::cout <<"This is running on the "<<name<<" module with jet trigger at " <<trig<<std::endl;
	n_evt=0;
	this->do_pythia = run_pythia;
	this->verbosity = verbosity;
	this->HerwigKin=new JetKinematicPlots("Herwig");
//	HerwigTree=new TTree("Herwig_tree", "Data Tree for Herwig Generated Events");
	if(run_pythia){
		this->PythiaKin=new JetKinematicPlots("Pythia");
//		PythiaTree=new TTree("Pythia_tree", "Data Tree for Pythia Generated Events");
//		this->RatioKin=new JetKinematicPlots("Ratio");
		}
	}

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
  int getKinematics(PHCompositeNode *topNode, JetKinematicPlots*, bool); 
  /// Clean up internals after each event.
  int ResetEvent(PHCompositeNode *topNode) override;

  /// Called at the end of each run.
  int EndRun(const int runnumber) override;

  /// Called at the end of all processing.
  int End(PHCompositeNode *topNode) override;

  /// Reset
  int Reset(PHCompositeNode * /*topNode*/) override;
  PHPythia8* PythiaGenerator(PHCompositeNode *topNode, int trigger);
  PHCompositeNode* pythiaNode;
  void Print(const std::string &what = "ALL") const override;
  std::vector<HepMC::GenParticle*> IDJets(PHCompositeNode *topNode, HepMC::GenParticle*, bool); 
  std::string trig="MB";
  int trig_val=0;
  int n_evt;
  bool do_pythia;
  int verbosity;
 private:
//	TTree* HerwigTree, PythiaTree; //this will be a later upgrade
	PHPythia8* pythiagen;
	JetKinematicPlots* HerwigKin, *PythiaKin/*, *RatioKin*/; 
	struct jetobj{
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
	};
};

#endif // HERWIGJETSPECTRA_H
