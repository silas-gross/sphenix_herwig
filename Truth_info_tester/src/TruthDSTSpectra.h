// Tell emacs that this is a C++ source
//  -*- C++ -*-.
#ifndef TRUTHDSTSPECTRA_H
#define TRUTHDSTSPECTRA_H
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

#include <fun4all/SubsysReco.h>
#include <math.h>
#include <string>
#include <map>
#include <vector>
#include <TH1.h>
#include <TH2.h>
#include <TH3.h>
#include <TFile.h>
#include <fun4all/Fun4AllBase.h>
#include <fun4all/Fun4AllReturnCodes.h>
#include <fun4all/Fun4AllInputManager.h>
#include <fun4all/SubsysReco.h>
#include <fun4all/Fun4AllBase.h>
#include <fun4all/Fun4AllReturnCodes.h>
#include <ffaobjects/EventHeaderv1.h>
#include <Event/Event.h>
#include <Event/EventTypes.h>

#include <g4main/PHG4VtxPoint.h>
#include <g4main/PHG4TruthInfoContainer.h>
#include <g4main/PHG4Particle.h>
#include <g4main/PHG4HitContainer.h>
#include <g4main/PHG4Hit.h>
#include <phool/PHCompositeNode.h>


class PHCompositeNode;
class TruthDSTSpectra : public SubsysReco
{
 public:


  TruthDSTSpectra(const std::string &name = "TruthDSTSpectra"){
	particle_pt=new TH1F("particle_pt", "p_{T} of the particle, corrected for vertex; p_{T} [GeV]; N_{evts}", 1000, 0, 10);
	vertex_x_pos=new TH1F("xpos", "x position of vertex; x [cm]; N_{evts}", 1000, -10, 10);
	vertex_y_pos=new TH1F("ypos", "y position of vertex; y [cm]; N_{evts}", 1000, -10, 10);
	vertex_z_pos=new TH1F("zpos", "z position of vertex; z [cm]; N_{evts}", 1000, -100, 100);
	hits = new TH2F("hits", "Position of hits; #eta; #phi; #N_{hits}", 24, -1.11, 1.09, 64, -3.15, 3.13);	
	vtx_plane=new TH2F("vtx", "2D distribution of collision vertex; x [cm]; y [cm]; N_{evts}", 10000, -10, 10, 10000, -10, 10);
	vtx_rzplane=new TH2F("rzvtx", "2D distribution of collision vertex; r [cm]; z [cm]; N_{evts}", 10000, -10, 10, 10000, -10, 10);
	full_vtx=new TH3F("fvtx", "3D distributions of collision vertex; x[cm]; y [cm]; z [cm]; N_{evts}", 10000, -10, 10, 10000, -10, 10, 10000, -10, 10);

	particle_comp=new TH2F("part_m_pt", "Particle Composition; p_{T} [GeV]; m [GeV]", 1000, 0, 10, 100, 0, 10);  //need to figure out how I want to store the particle composition
	}
	;

  ~TruthDSTSpectra() override;
  std::string type="EPOS";	
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
	void fillTruth(PHCompositeNode *topNode);
	void fillTruth(PHCompositeNode *topNode);
	bool data;
	KinematicPlots *truthPlots, *recoPlots;
 TH1F* particle_pt, *vertex_x_pos, *vertex_y_pos, *vertex_z_pos;
 TH2F* hits, *vtx_plane, *vtx_rzplane, *particle_comp;
 TH3F* full_vtx;
 std::map<int, std::vector<double>> particle_mass;
};

#endif // TRUTHDSTSPECTRA_H
