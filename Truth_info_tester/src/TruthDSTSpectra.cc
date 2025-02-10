//____________________________________________________________________________..
//
// This is a template for a Fun4All SubsysReco module with all methods from the
// $OFFLINE_MAIN/include/fun4all/SubsysReco.h baseclass
// You do not have to implement all of them, you can just remove unused methods
// here and in TruthDSTSpectra.h.
//
// TruthDSTSpectra(const std::string &name = "TruthDSTSpectra")
// everything is keyed to TruthDSTSpectra, duplicate names do work but it makes
// e.g. finding culprits in logs difficult or getting a pointer to the module
// from the command line
//
// TruthDSTSpectra::~TruthDSTSpectra()
// this is called when the Fun4AllServer is deleted at the end of running. Be
// mindful what you delete - you do loose ownership of object you put on the node tree
//
// int TruthDSTSpectra::Init(PHCompositeNode *topNode)
// This method is called when the module is registered with the Fun4AllServer. You
// can create historgrams here or put objects on the node tree but be aware that
// modules which haven't been registered yet did not put antyhing on the node tree
//
// int TruthDSTSpectra::InitRun(PHCompositeNode *topNode)
// This method is called when the first event is read (or generated). At
// this point the run number is known (which is mainly interesting for raw data
// processing). Also all objects are on the node tree in case your module's action
// depends on what else is around. Last chance to put nodes under the DST Node
// We mix events during readback if branches are added after the first event
//
// int TruthDSTSpectra::process_event(PHCompositeNode *topNode)
// called for every event. Return codes trigger actions, you find them in
// $OFFLINE_MAIN/include/fun4all/Fun4AllReturnCodes.h
//   everything is good:
//     return Fun4AllReturnCodes::EVENT_OK
//   abort event reconstruction, clear everything and process next event:
//     return Fun4AllReturnCodes::ABORT_EVENT; 
//   proceed but do not save this event in output (needs output manager setting):
//     return Fun4AllReturnCodes::DISCARD_EVENT; 
//   abort processing:
//     return Fun4AllReturnCodes::ABORT_RUN
// all other integers will lead to an error and abort of processing
//
// int TruthDSTSpectra::ResetEvent(PHCompositeNode *topNode)
// If you have internal data structures (arrays, stl containers) which needs clearing
// after each event, this is the place to do that. The nodes under the DST node are cleared
// by the framework
//
// int TruthDSTSpectra::EndRun(const int runnumber)
// This method is called at the end of a run when an event from a new run is
// encountered. Useful when analyzing multiple runs (raw data). Also called at
// the end of processing (before the End() method)
//
// int TruthDSTSpectra::End(PHCompositeNode *topNode)
// This is called at the end of processing. It needs to be called by the macro
// by Fun4AllServer::End(), so do not forget this in your macro
//
// int TruthDSTSpectra::Reset(PHCompositeNode *topNode)
// not really used - it is called before the dtor is called
//
// void TruthDSTSpectra::Print(const std::string &what) const
// Called from the command line - useful to print information when you need it
//
//____________________________________________________________________________..

#include "TruthDSTSpectra.h"

#include <fun4all/Fun4AllReturnCodes.h>

#include <phool/PHCompositeNode.h>

//____________________________________________________________________________..
//TruthDSTSpectra::TruthDSTSpectra(const std::string &name):
 //SubsysReco(name)
//{
 // std::cout << "TruthDSTSpectra::TruthDSTSpectra(const std::string &name) Calling ctor" << std::endl;
//}

//____________________________________________________________________________..
TruthDSTSpectra::~TruthDSTSpectra()
{
  std::cout << "TruthDSTSpectra::~TruthDSTSpectra() Calling dtor" << std::endl;
}

//____________________________________________________________________________..
int TruthDSTSpectra::Init(PHCompositeNode *topNode)
{
  std::cout << "TruthDSTSpectra::Init(PHCompositeNode *topNode) Initializing" << std::endl;
  return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
int TruthDSTSpectra::InitRun(PHCompositeNode *topNode)
{
  std::cout << "TruthDSTSpectra::InitRun(PHCompositeNode *topNode) Initializing for Run XXX" << std::endl;
  return Fun4AllReturnCodes::EVENT_OK;
}
void TruthDSTSpectra::fillTruth(PHCompositeNode *topNode);
{
	.//iterate over truth nodes if running on MC for MC to MC and reco to truth comp
	PHHepMCGenEventMap* phg=findNode::getClass<PHHepMCGenEventMap>(topNode, "PHHepMCGenEventMap");
	
}
void TruthDSTSpectra::fillReco(PHCompositeNode *topNode)P
{
	//fills the output from Reco Calo in MC and data
	//Fundamental parts
}
//____________________________________________________________________________..
int TruthDSTSpectra::process_event(PHCompositeNode *topNode)
{
  	std::cout << "TruthDSTSpectra::process_event(PHCompositeNode *topNode) Processing Event" << std::endl;
 	PHHepMCGenEventMap *phg=findNode::getClass<PHHepMCGenEventMap>(topNode, "PHHepMCGenEventMap");
	//PHG4TruthInfoContainer *truthinfo = findNode::getClass<PHG4TruthInfoContainer>(topNode, "G4TruthInfo");
 	//std::map<std::string, std::vector<float>> hep_map;
	for( PHHepMCGenEventMap::ConstIter eventIter=phg->begin(); eventIter != phg->end(); ++eventIter)
	{
		PHHepMCGenEvent* hpev=eventIter->second;
		if(hpev && hpev->get_embedding_id() == 0 ){
			HepMC::GenEvent * truthevent = hpev->getEvent();
			if(!truthevent) return 1;
			PHHepMCGenEvent *ph2=phg->get(0);
			float x=ph2->get_collision_vertex().x(), y=ph2->get_collision_vertex().y(), z=ph2->get_collision_vertex().z();
			vertex_x_pos->Fill(x);
			vertex_y_pos->Fill(y);
			vertex_z_pos->Fill(z);
			vtx_plane->Fill(x, y, 1);
			float r=sqrt(pow(x, 2) + pow(y, 2));
			vtx_rzplane->Fill(r,z,1);
			full_vtx->Fill(x,y,z);
			for(HepMC::GenEvent::particle_const_iterator iter = truthevent->particles_begin(); iter != truthevent ->particles_end(); ++iter){
				if(!(*iter)->end_vertex() && (*iter)->status() ==1){
					double px=(*iter)->momentum().px();
					double py=(*iter)->momentum().py();
					double pz=(*iter)->momentum().pz();
					double phi=atan2(py, px);
					double pt=sqrt(px*px+py*py);
					double eta=asinh(pz/(sqrt(px*px+py*py)));
					hits->Fill(eta, phi, 1);
					particle_pt->Fill(pt, 1);
					particle_mass[(*iter)->pdg_id()].push_back((*iter)->generated_mass());
					particle_comp->Fill(pt, (*iter)->generated_mass(), 1);
				}
			}
		}
	}	
  return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
int TruthDSTSpectra::ResetEvent(PHCompositeNode *topNode)
{
  std::cout << "TruthDSTSpectra::ResetEvent(PHCompositeNode *topNode) Resetting internal structures, prepare for next event" << std::endl;
  return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
int TruthDSTSpectra::EndRun(const int runnumber)
{
  std::cout << "TruthDSTSpectra::EndRun(const int runnumber) Ending Run for Run " << runnumber << std::endl;
  return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
int TruthDSTSpectra::End(PHCompositeNode *topNode)
{
  std::cout << "TruthDSTSpectra::End(PHCompositeNode *topNode) This is the End..." << std::endl;
  return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
int TruthDSTSpectra::Reset(PHCompositeNode *topNode)
{
 std::cout << "TruthDSTSpectra::Reset(PHCompositeNode *topNode) being Reset" << std::endl;
  return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
void TruthDSTSpectra::Print(const std::string &what) const
{
  std::cout << "TruthDSTSpectra::Print(const std::string &what) const Printing info for " << what << std::endl;
  TFile* f =new TFile(Form("Truth_data_%s.root", type.c_str()), "RECREATE");
  particle_pt->Write();
  vertex_x_pos->Write();
  vertex_y_pos->Write();
  vertex_z_pos->Write();
  hits->Write();
  vtx_plane->Write();
  vtx_rzplane->Write();
  full_vtx->Write();
  for(auto a:particle_mass){
	TH1F* par=new TH1F(Form("particle_mass_%d", a.first), Form("HepMC Generated Mass for PDG ID %d generated by %s; Mass [GeV]", a.first, type.c_str()), 1000, -0.5, 100);
	for(auto m:a.second) par->Fill(m);
	par->Write();
	delete par;
	} 
  f->Write();
  f->Close();
}
