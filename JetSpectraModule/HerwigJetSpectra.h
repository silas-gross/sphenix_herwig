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

#include <TH1.h>
#include <TFile.h>
#include <TH2.h>

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

class PHCompositeNode;

class HerwigJetSpectra : public SubsysReco
{
 public:

  HerwigJetSpectra(const std::string &name = "HerwigJetSpectra", 
			const std::string &fname="HerwigJetSpectra.root")
{
	h_phi=new TH1F("phi", "Energy #varphi distribution of all particles produced in the decay chain of Hepmc record; #varphi; #sum_{particles} E [GeV]", 64, -3.1416, 3.14);
	h_eta=new TH1F("eta", "Energy #eta distribution of all particles produced in decay chain of Hepmc record in sPHENIX acceptance; #eta; #sum_{particles} E [GeV]", 24, -1.12, 1.1); 
	h_phi_hit=new TH1F("phi_hit", "hit distribution #varphi distribution of all particles produced in the decay chain of Hepmc record; #varphi; #sum_{particles} E [GeV]", 64, -3.1416, 3.14);
	h_eta=new TH1F("eta_hit", "hit #eta distribution of all particles produced in decay chain of Hepmc record in sPHENIX acceptance; #eta; #sum_{particles} E [GeV]", 24, -1.12, 1.1); 
	h_pt=new TH1F("pt", "p_{T} distribution of all particles produced in decay chain of Hepmc record; p_{T} [GeV]; N_{part}", 100,-0.5, 99.5);
	h_n_part=new TH1F("n_part", "Number of total particles produced in decay chain of a Hepmc event; n_{part}; N_{event}", 1000, -0.5, 999.5);
	h_mass=new TH1F("mass", "Particle masses of all particle in decay chain of Hepmc record; m [GeV]; N_{part}", 100, 0, 1.5);
	h_E=new TH1F("energy", "Energy of all particles in decay chain, E[GeV], N_{part}", 100, -0.5, 99.5);
	h_status=new TH1F("status", "HepMC status of primary partons; status", 30, -0.5, 29.5);
	h_phi_orig=new TH1F("phi_orig", "#varphi distribution of primary particles produced in Hepmc record; #varphi; #sum_{particles} E [GeV]", 64, -3.1416, 3.14);
	h_eta_orig=new TH1F("eta_orig", "#eta distribution of primary particles produced in decay chain of Hepmc record in sPHENIX acceptance; #eta; #sum_{particles} E [GeV]", 24, -1.12, 1.1); 
	h_phi_hit_orig=new TH1F("phi_hit_orig", "hit #varphi distribution of primary particles produced in Hepmc record; #varphi; N_{hits}", 64, -3.1416, 3.14);
	h_eta_orig=new TH1F("eta_hit_orig", "#eta distribution of primary particles produced in decay chain of Hepmc record in sPHENIX acceptance; #eta; N_{hits}", 24, -1.12, 1.1); 
	h_pt_orig=new TH1F("pt_orig", "p_{T} distribution of primary particles produced in decay chain of Hepmc record; p_{T} [GeV]; N_{part}", 100,-0.5, 99.5);
	h_n_part_orig=new TH1F("n_part_orig", "Number of total primary particles produced in decay chain of a Hepmc event; n_{part}; N_{event}", 1000, -0.5, 999.5);
	h_mass_orig=new TH1F("mass_orig", "Particle masses of primary particle in decay chain of Hepmc record; m [GeV]; N_{part}", 100, 0, 1.5);
	h_status_orig=new TH1F("status_orig", "HepMC status of primary partons; status", 30, -0.5, 29.5);
	h_E_orig=new TH1F("energy_orig", "Energy of primary partons, E[GeV], N_{part}", 100, -0.5, 99.5);
	h_pt_leading=new TH1F("pt_leading", "p_{T} of leading primary particle in Hepmc record; p_{T}[GeV]; N_{part}", 100, -0.5, 99.5);
	h_E_total=new TH1F("energy_total", "Total energy of event, E[GeV], N_{event}", 200, -0.5, 199.5);
	h_vertex=new TH2F("vertex", "Vertex position, r [cm], z[cm], N_{event}", 100, -10, 10, 100, -10, 10);
	}
  ~HerwigJetSpectra() override;

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
  int process_event(PHCompositeNode *topNode) override;

  /// Clean up internals after each event.
  int ResetEvent(PHCompositeNode *topNode) override;

  /// Called at the end of each run.
  int EndRun(const int runnumber) override;

  /// Called at the end of all processing.
  int End(PHCompositeNode *topNode) override;

  /// Reset
  int Reset(PHCompositeNode * /*topNode*/) override;

  void Print(const std::string &what = "ALL") const override;
  std::string trig="MB";
 private:
	TH1F *h_phi, *h_eta, *h_eta_hit, *h_phi_hit, *h_pt, *h_mass, *h_E, *h_status;
	TH1F *h_phi_orig, *h_eta_orig, *h_eta_hit_orig, *h_phi_hit_orig, *h_pt_orig, *h_mass_orig, *h_E_orig, *h_status_orig;
	TH1F *h_n_part, *h_n_part_orig, *h_pt_leading, *h_E_total;
	TH2F *h_vertex;
};

#endif // HERWIGJETSPECTRA_H
