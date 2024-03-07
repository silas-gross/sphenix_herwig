//____________________________________________________________________________..
//
// This is a template for a Fun4All SubsysReco module with all methods from the
// $OFFLINE_MAIN/include/fun4all/SubsysReco.h baseclass
// You do not have to implement all of them, you can just remove unused methods
// here and in HerwigJetSpectra.h.
//
// HerwigJetSpectra(const std::string &name = "HerwigJetSpectra")
// everything is keyed to HerwigJetSpectra, duplicate names do work but it makes
// e.g. finding culprits in logs difficult or getting a pointer to the module
// from the command line
//
// HerwigJetSpectra::~HerwigJetSpectra()
// this is called when the Fun4AllServer is deleted at the end of running. Be
// mindful what you delete - you do loose ownership of object you put on the node tree
//
// int HerwigJetSpectra::Init(PHCompositeNode *topNode)
// This method is called when the module is registered with the Fun4AllServer. You
// can create historgrams here or put objects on the node tree but be aware that
// modules which haven't been registered yet did not put antyhing on the node tree
//
// int HerwigJetSpectra::InitRun(PHCompositeNode *topNode)
// This method is called when the first event is read (or generated). At
// this point the run number is known (which is mainly interesting for raw data
// processing). Also all objects are on the node tree in case your module's action
// depends on what else is around. Last chance to put nodes under the DST Node
// We mix events during readback if branches are added after the first event
//
// int HerwigJetSpectra::process_event(PHCompositeNode *topNode)
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
// int HerwigJetSpectra::ResetEvent(PHCompositeNode *topNode)
// If you have internal data structures (arrays, stl containers) which needs clearing
// after each event, this is the place to do that. The nodes under the DST node are cleared
// by the framework
//
// int HerwigJetSpectra::EndRun(const int runnumber)
// This method is called at the end of a run when an event from a new run is
// encountered. Useful when analyzing multiple runs (raw data). Also called at
// the end of processing (before the End() method)
//
// int HerwigJetSpectra::End(PHCompositeNode *topNode)
// This is called at the end of processing. It needs to be called by the macro
// by Fun4AllServer::End(), so do not forget this in your macro
//
// int HerwigJetSpectra::Reset(PHCompositeNode *topNode)
// not really used - it is called before the dtor is called
//
// void HerwigJetSpectra::Print(const std::string &what) const
// Called from the command line - useful to print information when you need it
//
//____________________________________________________________________________..

#include "HerwigJetSpectra.h"

#include <fun4all/Fun4AllReturnCodes.h>

#include <phool/PHCompositeNode.h>

//____________________________________________________________________________..
HerwigJetSpectra::HerwigJetSpectra(const std::string &name):
 SubsysReco(name)
{
  std::cout << "HerwigJetSpectra::HerwigJetSpectra(const std::string &name) Calling ctor" << std::endl;
}

//____________________________________________________________________________..
HerwigJetSpectra::~HerwigJetSpectra()
{
  std::cout << "HerwigJetSpectra::~HerwigJetSpectra() Calling dtor" << std::endl;
}

//____________________________________________________________________________..
int HerwigJetSpectra::Init(PHCompositeNode *topNode)
{
  std::cout << "HerwigJetSpectra::Init(PHCompositeNode *topNode) Initializing" << std::endl;
  return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
int HerwigJetSpectra::InitRun(PHCompositeNode *topNode)
{
  std::cout << "HerwigJetSpectra::InitRun(PHCompositeNode *topNode) Initializing for Run XXX" << std::endl;
  return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
int HerwigJetSpectra::process_event(PHCompositeNode *topNode)
{
  std::cout << "HerwigJetSpectra::process_event(PHCompositeNode *topNode) Processing Event" << std::endl;
  PHHepMCGenEventMap *phg=findNode::getClass<PHHepMCGenEventMap>(topNode, "PHHepMCGenEventMap");
  for ( PHHepMCGenEventMap::ConstIter eventIter=phg->begin(); eventIter != phg->end(); ++eventIter)
  {
	PHHepMCGenEvent* hpev=eventIter->second;
	if(hepev){
		HepMC::GenEvent* ev=hpev->getEvent();
		if(!ev) return 1;
		PHHepMCGenEvent* origvtx=hpev->get(0);
		float x_vtx->origvtx->get_collision_vertex().x(), y_vtx->origvtx->get_collision_vertex.y(), z_vtx=origvtx->get_collision_vertex().z(); //here is the vertex
		//Now need to get the produced particles and differentiate from the end particles
		for(HepMC::GenEvent::particle_const_iter iter=ev->particles_begin(); iter !=truthevent->particles_end(); ++iter){
	if(!(*iter)->end_vertex() && (*iter)->status() == 1 && (*iter)->get_embedding_id() >= 0){
		double px=(*iter)->momentum().px();
		double py=(*iter)->momentum().py();
		double pz=(*iter)->momentum().pz();
		double phi=atan2(py, px);
		double pt=sqrt(px*px+py*py);
		double eta=asinh(pz/pt);
		if(
		
		
  }
  return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
int HerwigJetSpectra::ResetEvent(PHCompositeNode *topNode)
{
  std::cout << "HerwigJetSpectra::ResetEvent(PHCompositeNode *topNode) Resetting internal structures, prepare for next event" << std::endl;
  return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
int HerwigJetSpectra::EndRun(const int runnumber)
{
  std::cout << "HerwigJetSpectra::EndRun(const int runnumber) Ending Run for Run " << runnumber << std::endl;
  return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
int HerwigJetSpectra::End(PHCompositeNode *topNode)
{
  std::cout << "HerwigJetSpectra::End(PHCompositeNode *topNode) This is the End..." << std::endl;
  return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
int HerwigJetSpectra::Reset(PHCompositeNode *topNode)
{
 std::cout << "HerwigJetSpectra::Reset(PHCompositeNode *topNode) being Reset" << std::endl;
  return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
void HerwigJetSpectra::Print(const std::string &what) const
{
  std::cout << "HerwigJetSpectra::Print(const std::string &what) const Printing info for " << what << std::endl;
}
