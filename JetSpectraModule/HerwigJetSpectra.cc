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
#define PI 3.14159
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
		std::cout<<"looking into the originating partons" <<std::endl;
		float jetptlead=0;
		for(HepMC::GenVertex::particles_out_const_iterator iter=ov->particles_out_const_begin(); iter !=ov->particles_out_const_end(); ++iter)
		{
				std::cout<<"starting the analysis on the parton" <<std::endl;
				jetobj* Jet=new jetobj;
				std::cout<<"successfully created the jet object" <<std::endl;
				Jet->originating_parton=(*iter);
				std::cout<<"have loaded the originating particle into the Jet object"<<std::endl;
		 		try{
					Jet->jet_particles=IDJets(topNode, (*iter)); //ids all the daughter particles coming from the originating partons 	
				}
				catch (std::exception& ex){
					std::cout<<"caught an exception in the jetid stage as " <<ex.what() <<std::endl;
				}
				
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
				h_hits_orig->Fill(eta, phi);
				np_orig++;
				if(pt>pt_lead) pt_lead=pt;
				h_status_orig->Fill((*iter)->status());
				h_ET_orig->Fill(ET);
				float mj=0, R=0, pxj=0, pyj=0, etj=0;
				std::cout<<"Measuring the kinematics of the jet" <<std::endl;
				if(Jet->jet_particles.size() == 0 ) continue;
				for(auto p:Jet->jet_particles){
					mj+=p->momentum().m();
					pxj+=p->momentum().px();
					pyj+=p->momentum().py();
					etj+=p->momentum().e()/p->momentum().eta();
					for(auto n:Jet->jet_particles){
						float phidiff=0;
						if(  (p->momentum().phi() < 0 && n->momentum().phi() > 0 ) ) phidiff=std::min(abs(p->momentum().phi())+ n->momentum().phi(), abs(-PI - p->momentum().phi())+ abs(PI - n->momentum().phi()));
						else if ( p->momentum().phi() > 0 && n->momentum().phi() < 0 ) phidiff=std::min(abs(n->momentum().phi())+ p->momentum().phi(), abs(-PI - n->momentum().phi())+ abs(PI - p->momentum().phi()));

						else phidiff = p->momentum().phi() - n->momentum().phi();
						float rt=sqrt(pow(p->momentum().eta()-n->momentum().eta(), 2) + pow(phidiff, 2));
						if(rt>R) R=rt;
					}
				}	
				Jet->mass=mj;
				Jet->ET=etj;
				Jet->pt=sqrt(pow(pxj,2)+pow(pyj,2));
				Jet->R=R/2;
				Jet->phi=phi;
				Jet->eta=eta;
				h_Jet_pt->Fill(Jet->pt);
				h_Jet_R->Fill(Jet->R);
				h_Jet_npart->Fill(Jet->jet_particles.size());
				if(Jet->pt > jetptlead) jetptlead=Jet->pt;
				delete Jet;
		}
		h_Jet_pt_lead->Fill(jetptlead);
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
		h_hits->Fill(eta, phi);
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
		//int last_part=0, n_loop=0;
		//bool stuck=false, eov=false;
		std::unordered_map<HepMC::GenVertex*, HepMC::GenVertex::particles_out_const_iterator> holding_part;
		HepMC::GenVertex* active_vertex=decay;
		HepMC::GenVertex::particles_out_const_iterator par=decay->particles_out_const_begin(); 
		HepMC::GenVertex::particles_out_const_iterator parent_par=decay->particles_out_const_begin();
		std::cout<<"have set up the structure needed, now entering while loop" <<std::endl;
		while(parent_par != decay->particles_out_const_end()){
			//The goal is to go to the deepest level and collect all partilces that are final
			std::cout<<"Starting a new itteration of the loop" <<std::endl;
			auto last_it=holding_part.begin();
			if(holding_part.size() > 0  && last_it->first == decay ){
				 ++parent_par;
				 last_it->second = parent_par;
				if(parent_par == decay->particles_out_const_end()) break;
				std::cout<<"Moved along the primary particle branch. Now have parton " <<(*parent_par)->barcode() <<std::endl; 
			} 
			if(par == decay->particles_out_const_begin() && holding_part.size() == 0 ){
				 holding_part.emplace(decay, par);
				 continue;
			}
			if( active_vertex && holding_part.size() > 0  && last_it->first && active_vertex != last_it->first){
				std::cout<<"realigning the vertex to the branch tree " <<std::endl; 
				active_vertex=last_it->first;
				par=last_it->second;
				++par; 
				if(par != active_vertex->particles_out_const_end() ) last_it->second=par;
				else{
					auto it=holding_part.cbegin();
					holding_part.erase(it);
					continue;
				}
			} //end of checker for if the active vertex isn't se to what we would hope
			if( active_vertex && active_vertex->particles_out_size() > 0) {
				//make sure that the vertex exists and has outgoing particles 
				if( par != active_vertex->particles_out_const_end() && *par ) {
					//make sure the particle exists and is not an end of vertex
					if (!(*par) || par >= active_vertex->particles_out_const_end() ) std::cout<<"Ok somehow I got here despite seeming to be inconsistent???" <<std::endl;
					bool has_end_vertex=true;
					if ( par >= active_vertex->particles_out_const_end() ) std::cout<<"The active parton is " <<std::distance( active_vertex->particles_out_const_end(), par) <<" steps away from the end iterator" <<std::endl;
					if( par >= active_vertex->particles_out_const_end() || par < active_vertex->particles_out_const_begin() ){
						std::cout<<"I have no clue how this happened??????, somehow we are out of range of the vertex iterator??" <<std::endl;
						auto it=holding_part.cbegin();
						holding_part.erase(it);	
						continue;
					}
					//std::cout<<"The issue below is on a vertex with barcode " <<(*par)->production_vertex()->barcode() <<std::endl;
					if(!(*par)->end_vertex()) has_end_vertex=false; //does the negation make a difference? it shouldn't I would think?
					if( has_end_vertex){
						//this is what we have to do if the particles have an end vertex, so moving deeper
						if( (*par)->end_vertex()->particles_out_size() > 0 ) 
						{
							//there is a well formed vertex
							std::cout<<"The active vertex has barcode " <<active_vertex->barcode() <<" and the new active vertex has barcode " <<(*par)->end_vertex()->barcode() <<std::endl;
							active_vertex=(*par)->end_vertex();
							par=active_vertex->particles_out_const_begin();
							if(active_vertex && *par) holding_part.emplace(active_vertex, par);
							else{
								std::cout<<"Something is off here " <<std::endl;
								break;
							}
							std::cout<<"New depth is " <<holding_part.size() <<std::endl;
							std::cout<<"The active vertex has barcode " <<active_vertex->barcode() <<" and the new holding partition vertex has barcode " <<holding_part.begin()->first->barcode() <<std::endl;
							continue;
						} //End of deepening by one
						else{
							std::cout<<"For some reason the particle has an end vertex, but no apparent particles in the end vertex, will treat as a final state ? Depends on status which is " <<(*par)->status() <<std::endl;
							if(final_state_barcodes.find((*par)->barcode()) == final_state_barcodes.end()){
								if( (*par)->status() == 1 ) final_state_jet.push_back(*par);
								final_state_barcodes.insert((*par)->barcode());
							} //behavior of final state particlse
							++par;
							if( par != active_vertex->particles_out_const_end() && last_it->first==active_vertex ) last_it->second=par;
							else if( last_it->first != active_vertex ) holding_part.emplace(active_vertex, par);
							else{
								auto it=holding_part.cbegin();
								holding_part.erase(it);
							}
							continue;
						}
					} //end of the behavior of parton with an end vertex
					else{
						std::cout<<"Will treat as a final state? Depends on status which is " <<(*par)->status() <<std::endl;
						if(final_state_barcodes.find((*par)->barcode()) == final_state_barcodes.end()){
							if( (*par)->status() == 1 ){
								 final_state_jet.push_back(*par);
								std::cout<<"Added a parton to the final state jet, have  " <<final_state_jet.size() <<std::endl;
							}
							final_state_barcodes.insert((*par)->barcode());
						} //behavior of final state particlse
						else std::cout<<"Have already examined this particle it has barcode " <<(*par)->barcode() <<std::endl;
						++par; 
						if(par == active_vertex->particles_out_const_end()){
							auto it=holding_part.cbegin();
							holding_part.erase(it);
						}	
						last_it->second=par;
						continue;
					}//end of particle having no end state vertex
				} // end of ok parton code
				else if ( par == active_vertex->particles_out_const_end()){
					//handle moving back out of a vertex 
					std::cout<<"the particle itterator is sitting at the end of the itterator chain" <<std::endl;
					auto it = holding_part.cbegin();
					holding_part.erase(it);
					active_vertex=last_it->first;
					par=last_it->second;
					continue;
				} //end of moving back from vertex
				else{
					par=last_it->second;
					if(par != active_vertex->particles_out_const_end() ){
						//get the vertex back in place correctly for the relative placement 
						++par;
						last_it->second=par;
						continue;
					}
					else {
						auto it=holding_part.cbegin();
						holding_part.erase(it);
						continue;
					}
				}
										
			} //end the active vertex > 0 outgoing 
			else if( holding_part.size() > 0 ){
				//active vertex either doesn't exist or it has no outgoing particles
				if(active_vertex){ 
					//if the vertex exists check to see if its the current vertex in the holding part
					if(last_it->first){
						if(active_vertex != last_it->first) active_vertex =last_it->first;
						else{
							auto it=holding_part.cbegin();
							holding_part.erase(it);
							continue;
						} //end of check to see if the active vertex is already the thing in use
						if(last_it->second != active_vertex->particles_out_const_end()) par=last_it->second;
						else{
							//check to see if we would be at the end of the vertex 
							auto it=holding_part.cbegin();
							holding_part.erase(it);
							continue;

						}
						++par; 
						if(par == active_vertex->particles_out_const_end()){
							//check if the new particle itterator is usable
							auto it=holding_part.cbegin();
							holding_part.erase(it);
							continue;
						} //end of check to see if the new particle is actually a good one 
					}//end of check if the holding part has an actual first vertex
					else{
						auto it=holding_part.cbegin();
						holding_part.erase(it);
						continue; //try again to test the vertex, don't assign an untested vertex
					} //end of else to account for dead vertex at end of holding part
					
				} //end of vertex exists if statement
				else{
					if(last_it->first ) active_vertex=last_it->first;
					else{
						auto it=holding_part.cbegin();
						holding_part.erase(it);
						continue; //try again to test the vertex, don't assign an untested vertex
					} //end of else to account for dead vertex at end of holding part
 
				} //end of else corresponding to active vertex if statement
			
			} //end of if there is anything in the holding part
		    else break; //if there is nothing anywhere, just breal
		} //end the while loop on the parent parton 
		std::cout<<"Got rid of the pointers" <<std::endl;
		holding_part.clear();
		final_state_barcodes.clear();
	} //end the else condition for needing to search for daughters
	std::cout<<"The jet has "<<final_state_jet.size() <<" particles in the final state" <<std::endl;
	delete decay;
	std::cout<<"deleted the decay vertex" <<std::endl;
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
