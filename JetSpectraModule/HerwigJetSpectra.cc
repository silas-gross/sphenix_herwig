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
/*HerwigJetSpectra::HerwigJetSpectra(const std::string &name):
 SubsysReco(name)
{
  std::cout << "HerwigJetSpectra::HerwigJetSpectra(const std::string &name) Calling ctor" << std::endl;
}
*/
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
  n_evt++;
  std::cout << "HerwigJetSpectra::process_event(PHCompositeNode *topNode) Processing Event" << n_evt << std::endl;
  PHHepMCGenEventMap *phg=findNode::getClass<PHHepMCGenEventMap>(topNode, "PHHepMCGenEventMap");
  if(!phg){
	std::cout<<"Did not find event map"<<std::endl;
	return 1;
	}
  int np=0, np_orig=0, hep_ev=0;
  double pt_lead=0, E_total=0;
  for ( PHHepMCGenEventMap::ConstIter eventIter=phg->begin(); eventIter != phg->end(); ++eventIter)
  {
	hep_ev++;
	PHHepMCGenEvent* hpev=eventIter->second;
	if(hpev){
		HepMC::GenEvent* ev=hpev->getEvent();
		if(!ev){
			std::cout<<"Did not find any event" <<std::endl;
			continue;
		}
		std::cout<<"Begin processing event"<<std::endl;
		PHHepMCGenEvent* origvtx=phg->get(0);
		for(auto w:ev->weights()) h_weight->Fill(w);
		float x_vtx=origvtx->get_collision_vertex().x(), y_vtx=origvtx->get_collision_vertex().y(), z_vtx=origvtx->get_collision_vertex().z(); //here is the vertex
		float r=sqrt(x_vtx*x_vtx+y_vtx*y_vtx);
		h_vertex->Fill(r, z_vtx);
		HepMC::GenParticle* pb=ev->beam_particles().first;
		HepMC::GenVertex* ov=pb->end_vertex();
		for(HepMC::GenVertex::particles_out_const_iterator iter=ov->particles_out_const_begin(); iter !=ov->particles_out_const_end(); ++iter)
		{
				jetobj* Jet=new jetobj;
				Jet->originating_parton=(*iter);
				std::cout<<"have loaded the originating particle into the Jet object"<<std::endl;
		 		Jet->jet_particles=IDJets(topNode, (*iter)); //ids all the daughter particles coming from the originating partons 
				std::cout<<"Identified Jet"<<std::endl;
				double px=(*iter)->momentum().px();
				double py=(*iter)->momentum().py();
				double pz=(*iter)->momentum().pz();
				double phi=atan2(py, px);
				double pt=sqrt(px*px+py*py);
				double eta=asinh(pz/pt);
				double mass=(*iter)->generated_mass(); 
				double E=(*iter)->momentum().e();
				double ET=sqrt(mass*mass + pt*pt); 
				h_phi_orig->Fill(phi, ET);
				h_eta_orig->Fill(eta, ET);
				h_pt_orig->Fill(pt);
				h_eta_hit_orig->Fill(eta);
				h_phi_hit_orig->Fill(phi);
				h_mass_orig->Fill(mass);
				h_E_orig->Fill(E);
				np_orig++;
				if(pt>pt_lead) pt_lead=pt;
				h_status_orig->Fill((*iter)->status());
				h_ET_orig->Fill(ET);
				float mj=0, R=0, pxj=0, pyj=0, etj=0;
				for(auto p:Jet->jet_particles){
					mj+=p->momentum().m();
					pxj+=p->momentum().px();
					pyj+=p->momentum().py();
					etj+=p->momentum().e()/p->momentum().eta();
					float rt=sqrt(pow(p->momentum().eta()-eta, 2) + pow(p->momentum().phi()-phi, 2));
					if(rt>R) R=rt;
				}	
				Jet->mass=mj;
				Jet->ET=etj;
				Jet->pt=sqrt(pow(pxj,2)+pow(pyj,2));
				Jet->R=R;
				Jet->phi=phi;
				Jet->eta=eta;
				h_Jet_pt->Fill(Jet->pt);
				h_Jet_R->Fill(Jet->R);
				h_Jet_npart->Fill(Jet->jet_particles.size());
		}	
		//Now need to get the produced particles and differentiate from the end particles
		for(HepMC::GenEvent::particle_const_iterator iter=ev->particles_begin(); iter !=ev->particles_end(); ++iter){
	if(!(*iter)->end_vertex() && (*iter)->status() == 1){ //only pick up final state particles
		double px=(*iter)->momentum().px();
		double py=(*iter)->momentum().py();
		double pz=(*iter)->momentum().pz();
		double phi=atan2(py, px);
		double pt=sqrt(px*px+py*py);
		double eta=asinh(pz/pt);
		double mass=(*iter)->generated_mass(); 
		double E=(*iter)->momentum().e();
		double ET=sqrt(mass*mass + pt*pt); 
		//px=px+x_vtx+y_vtx+z_vtx; //temporary holding in order to avoid an unused variable error
		np++;
		if((*iter)->status()== 1) E_total+=E;
		h_phi->Fill(phi, E);
		h_eta->Fill(eta, E);
		h_eta_hit->Fill(eta);
		h_phi_hit->Fill(phi);
		h_pt->Fill(pt);
		h_mass->Fill(mass);
		h_E->Fill(E);
		h_ET->Fill(ET);
		h_status->Fill((*iter)->status());
	}
	
 	}
 		}
	}
	h_n_part->Fill(np);
	h_n_part_orig->Fill(np_orig);
	h_pt_leading->Fill(pt_lead);
	h_E_total->Fill(E_total);
	h_ev->Fill(hep_ev);
  return Fun4AllReturnCodes::EVENT_OK;
}
//__________________________________________________________________________
std::vector<HepMC::GenParticle*> HerwigJetSpectra::IDJets(PHCompositeNode *topNode, HepMC::GenParticle* originating_parton)
{
	//this is intended to identify all final state particles coming from each originating parton 
	std::vector<HepMC::GenParticle*> final_state_jet;
	HepMC::GenVertex* decay=originating_parton->end_vertex();
	if(!decay){
		final_state_jet.push_back(originating_parton); //if the original state does not decay
	}
	else{
		std::cout<<"Have the decay vertex of the originating parton, now searching for all daughters"<<std::endl;
		std::unordered_set<int> final_state_barcodes;
		std::unordered_set<int> branches;
		HepMC::GenVertex* active_vertex=decay;
		HepMC::GenVertex* parent_vertex=decay;
		HepMC::GenVertex::particles_out_const_iterator par=decay->particles_out_const_begin(); 
		HepMC::GenVertex::particles_out_const_iterator parent_par=par;
		while(par != decay->particles_out_const_end()){ //loops over all particles in the decay chain and preserves the final state particle to get the jet cones
			++par;
			if((*par)->status() == 1 && !(*par)->end_vertex()){
				int bc=(*par)->barcode();
				if(final_state_barcodes.find(bc)== final_state_barcodes.end())
				{ 
					final_state_jet.push_back((*par));
					final_state_barcodes.insert(bc);
				}
			}
			else if ((*par)->end_vertex()){
				parent_vertex=(*par)->production_vertex();
				active_vertex=(*par)->end_vertex();
				parent_par=par;
				par=active_vertex->particles_out_const_begin();
			}
			auto next_par=par;
			next_par++;
			if(next_par == active_vertex->particles_out_const_end()){ //checks to see if we reach the end of a vertes
				par=parent_par; 
				branches.insert(active_vertex->barcode());
				++parent_par;
				//move onto the next particle in the previous vertex
				if(parent_par==parent_vertex->particles_out_const_end()){
					//checks if the parent vertex is used up and then propagates backwards to find the next vertex that hasn't been searched
					branches.insert(parent_vertex->barcode());
					bool still_good=false;
					while(!still_good){
						auto gp=parent_vertex->particles_in_const_begin();
						++gp;
//						auto gparent_vertex=(*gp)->production_vertex();
						while (gp != parent_vertex->particles_in_const_end()){
							if((*gp)->end_vertex()){
								if(branches.find((*gp)->end_vertex()->barcode()) != branches.end()){
									parent_par=gp;
									still_good=true;
								}
								else ++gp;
							}
						}
						if( gp == parent_vertex->particles_in_const_end()) parent_vertex=(*gp)->production_vertex();
					}
				
					par=parent_par;
					if((*par)->barcode() == originating_parton->barcode()) break;
					active_vertex=parent_vertex;
					parent_vertex=(*par)->production_vertex();
				}
			}
		}
	}		
	return final_state_jet;		 
				 
}
//____________________________________________________________________________..
int HerwigJetSpectra::ResetEvent(PHCompositeNode *topNode)
{
//  std::cout << "HerwigJetSpectra::ResetEvent(PHCompositeNode *topNode) Resetting internal structures, prepare for next event" << std::endl;
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
 std::cout<<"Ran over " <<n_evt<<" events" <<std::endl;
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
  TFile* f=new TFile(Form("herwig_output_%s_jetpt_all_particles.root", trig.c_str()), "RECREATE");
  h_pt->Write();
  h_phi->Write();
  h_eta->Write();
  h_mass->Write();
  h_E->Write();
  h_n_part->Write();
  h_status->Write();
  h_phi_hit->Write();
  h_eta_hit->Write();
  h_pt_orig->Write();
  h_phi_orig->Write();
  h_eta_orig->Write();
  h_phi_hit_orig->Write();
  h_eta_hit_orig->Write();
  h_mass_orig->Write();
  h_E_orig->Write();
  h_n_part_orig->Write();
  h_status_orig->Write();
  h_pt_leading->Write();
  h_ev->Write();
  h_E_total->Write();
  h_vertex->Write(); 
  h_weight->Write();
  h_ET->Write();
  h_ET_orig->Write();
  h_Jet_pt->Write();
  h_Jet_R->Write();
  h_Jet_npart->Write();
  f->Write();
}
