// Tell emacs that this is a C++ source
//  -*- C++ -*-.
#ifndef HERWIGJETSPECTRA_H
#define HERWIGJETSPECTRA_H

#include <fun4all/SubsysReco.h>
#include <fun4all/Fun4ALLBase.h>
#include <fun4all/Fun4ALLInputManager.h>
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

#include <g4main/PHG4VtxPoint.h>
#include <g4main/PHG4TruthInfoContainer.h>
#include <g4main/PHG4Particle.h>
#include <phhepmc/PHHepMCGenEvent.h>
#include <phhepmc/PHHepMCGenEventMap.h>
#include <HepMC/GenEvent.h>
#include <phool/phool.h>
#include <phool/getGlass.h>
#include <phool/PHCompositeNode.h>

class PHCompositeNode;

class HerwigJetSpectra : public SubsysReco
{
 public:

  HerwigJetSpectra(const std::string &name = "HerwigJetSpectra", 
			const std::string &fname="HerwigJetSpectra.root")
{
	h_phi=new TH1F("phi", "#varphi distribution of all particles produced in the decay chain of Hepmc record; #varphi; #sum_{particles} E [GeV]", 64, -3.1416, 3.14);
	h_eta=new TH1F("eta", "#eta distribution of all particles produced in decay chain of Hepmc record in sPHENIX acceptance; #eta; #sum_{particles} E [GeV]", 24, -1.12, 1.1); 
	h_pt=new TH1F("pt", "p_{T} distribution of all particles produced in decay chain of Hepmc record; p_{T} [GeV]; N_{part}", 100,-0.5, 99.5);
	h_n_part=new TH1F("n_part", "Number of total particles produced in decay chain of a Hepmc event; n_{part}; N_{event}", 1000, -0,5, 999.5);
	h_mass=new TH1F("mass", "Particle masses of all particle in decay chain of Hepmc record; m [GeV]; N_{part}", 1000, 0, 1000);
	h_phi_orig=new TH1F("phi_orig", "#varphi distribution of primary particles produced in Hepmc record; #varphi; #sum_{particles} E [GeV]", 64, -3.1416, 3.14);
	h_eta_orig=new TH1F("eta_orig", "#eta distribution of primary particles produced in decay chain of Hepmc record in sPHENIX acceptance; #eta; #sum_{particles} E [GeV]", 24, -1.12, 1.1); 
	h_pt_orig=new TH1F("pt_orig", "p_{T} distribution of primary particles produced in decay chain of Hepmc record; p_{T} [GeV]; N_{part}", 100,-0.5, 99.5);
	h_n_part_orig=new TH1F("n_part_orig", "Number of total primary particles produced in decay chain of a Hepmc event; n_{part}; N_{event}", 1000, -0,5, 999.5);
	h_mass_orig=new TH1F("mass_orig", "Particle masses of primary particle in decay chain of Hepmc record; m [GeV]; N_{part}", 1000, 0, 1000);
	h_pt_leading=new TH1F("pt_leading", "p_{T} of leading primary particle in Hepmc record; p_{T}[GeV]; N_{part}", 1000, -0.5, 200);
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

 private:
	TH1F *h_phi, *h_eta, *h_pt, *h_mass, *h_phi_orig, *h_eta_orig, *h_pt_orig, *h_mass_orig, *h_n_part, *h_n_part_orig, *h_pt_leading;
};

#endif // HERWIGJETSPECTRA_H
