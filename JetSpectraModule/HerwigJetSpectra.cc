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
		int last_part=0, n_loop=0;
		bool stuck=false;
		std::map<HepMC::GenVertex*, int> holding_part;
		HepMC::GenVertex* active_vertex=decay;
		HepMC::GenVertex::particles_out_const_iterator par=decay->particles_out_const_begin(); 
		HepMC::GenVertex::particles_out_const_iterator parent_par=decay->particles_out_const_begin();
		std::cout<<"have set up the structure needed, now entering while loop" <<std::endl;
		while(parent_par != decay->particles_out_const_end()){
			//The goal is to go to the deepest level and collect all partilces that are final
			bool jump=false;
			//if( *par )
			//{
				try{
					if((!active_vertex && holding_part.size() == 0) || !decay) break;
					if(par == active_vertex->particles_out_const_end() || parent_par == decay->particles_out_const_end() ) jump=true;
					if(jump) --par;
					if( (*par)->barcode() == last_part ) n_loop++;
					else n_loop=0;
					if (n_loop > 3 ) stuck=true; 
					else stuck=false;
					if(stuck){
						 if( par == active_vertex->particles_out_const_end() || !(*par)->end_vertex()) jump=true;	
						 else par=(*par)->end_vertex()->particles_out_const_begin();
					}
				//	std::cout<<stuck<<std::endl;
					std::cout<<"The active particle has mass " <<(*par)->generated_mass() <<" and status " <<(*par)->status() <<" and  barcode " <<(*par)->barcode() <<std::endl;
					HepMC::GenVertex* testvt=(*par)->end_vertex(); 
					//if(testvt) std::cout<<"Have the end vertex located at z=" <<testvt->position().z() <<std::endl;
					if( !testvt && !jump){
						
						std::cout<<"Trying to find out if there are any particles in here?" <<std::endl;
						if(final_state_barcodes.find((*par)->barcode()) != final_state_barcodes.end()){
							auto pt=par;
							++par; 
							if(par == (*pt)->production_vertex()->particles_out_const_end()){
								bool try_to_escape=true;
								while(try_to_escape)
								{
									holding_part.erase(--holding_part.cend());
									active_vertex=holding_part.rbegin()->first;
									par=active_vertex->particles_out_const_begin();
									while(par != active_vertex->particles_out_const_end())
									{
										if((*par)->barcode() == holding_part[active_vertex]) break;
										else ++par;
									}
									if(par != active_vertex->particles_out_const_end()) ++par;
									if(par != active_vertex->particles_out_const_end()){
										try_to_escape=false;
										continue;
										}
								}
							}
							continue;
						}
						else final_state_barcodes.emplace((*par)->barcode());
						if((*par)->status() == 1 ){
							final_state_jet.push_back((*par));
							std::cout<<"found a final state particle, now have " <<final_state_jet.size() <<std::endl;
						}
					
					//recorded final states, now move to the next
					try{	
						last_part=(*par)->barcode();
						++par;
						}
					catch(std::exception& e){ std::cout<<"Exception " <<e.what() <<std::endl;} 
					//if(*par){
					//	 last_part=(*par)->barcode();
					//}
					continue;
				}
				else  {
					//stores what particle we are at in the vertex to return to when we exhaust the end 
					last_part=(*par)->barcode();
					holding_part[active_vertex]=(*par)->barcode();
					active_vertex=(*par)->end_vertex();
					if(active_vertex && active_vertex->particles_out_size() > 0) par=active_vertex->particles_out_const_begin();
		//			std::cout<<"Going one level deeper" <<std::endl;
					std::cout<<"Have a depth of " <<holding_part.size() <<std::endl;
					continue;
					}
				}
				catch(std::exception& e) { std::cout<<"Caught error " <<e.what() <<std::endl;}
			//}
			//else if ( active_vertex && *par){
					
				try{
				HepMC::GenVertex* parent = holding_part.rbegin()->first;
				bool alldone=false;
				for(auto tp=parent->particles_out_const_begin(); tp != parent->particles_out_const_end(); ++tp){
					if(holding_part.rbegin()->second == (*tp)->barcode()){
						alldone=true;
					}
					else alldone=false;
				}
				delete parent;
				if(holding_part.size()==0 ||  !holding_part.rbegin()->first) break; 
				if(alldone || holding_part.rbegin()->first->particles_out_size() < 1 ){
					if(holding_part.size() <= 1 ) break;
				 	holding_part.erase(--holding_part.cend());
				}
				std::cout<<"Backed out to depth of " <<holding_part.size() <<std::endl;
				active_vertex=holding_part.rbegin()->first;
				if( !active_vertex || active_vertex->particles_out_size() == 0 ) continue;
				std::cout<<"The active vertex has " <<active_vertex->particles_out_size() <<" outgoing particles " <<std::endl;	
				bool out_of_vertexs=false;
				while (active_vertex->particles_out_size() < 1 ){
					holding_part.erase(--holding_part.cend());
					if(holding_part.size() < 1){
						out_of_vertexs=true;
						break;
					}
					active_vertex=holding_part.rbegin()->first;
					std::cout<<"Threw out spent vertex, new one has " <<active_vertex->particles_out_size() <<std::endl;
					if(holding_part.size() < 1 ){
						out_of_vertexs=true;
						 break;
					}
				}
				if(out_of_vertexs) break;
				bool eov=false;
				if( !active_vertex || active_vertex->particles_out_size() < 1 ) continue;
				for(auto bc=active_vertex->particles_out_const_begin(); bc != active_vertex->particles_out_const_end(); ++bc){
					std::cout<<"Barcode of the particle is " <<(*bc)->barcode() <<std::endl;
					std::cout<<"barcode of the last particle in the chute was " <<holding_part.rbegin()->second <<std::endl; 
					if(*bc && holding_part.rbegin()->first && (*bc)->barcode() == holding_part.rbegin()->second){
						last_part=(*par)->barcode();
						par = ++bc;
						if(par == active_vertex->particles_out_const_end()) eov=true;
						break;
						}
					}
				if(eov) continue;
				}
				catch(std::exception& e){ std::cout<<"Exception " <<e.what() <<std::endl;} 
		//		std::cout<<"updating the holding vertex" <<std::endl;
				holding_part[active_vertex]=(*par)->barcode();
				std::cout<<"Backing out by at least one level" <<std::endl;
				continue;
			//}
			//else{
			//	break;
				//par =active_vertex->particles_out_const_begin();
		//	}
			if(active_vertex->barcode() == decay->barcode() ){
				//break;
				try{ ++parent_par;}
				catch(std::exception& e){ 
					std::cout<<"Exception " <<e.what() <<std::endl;
					break;	
				} 
				continue;
			}
		}
		//delete *par;
		//delete *parent_par; 
		std::cout<<"Got rid of the particle pointers"<<std::endl;
		//if (active_vertex)
		//{
		//	 delete active_vertex;
		//}
		//if (decay){
		//	 delete decay;
		//}
		std::cout<<"Got rid of the pointers" <<std::endl;
		holding_part.clear();
		final_state_barcodes.clear();
		return final_state_jet;
	}	
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
