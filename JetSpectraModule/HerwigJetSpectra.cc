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
  if(verbosity>0) std::cout << "HerwigJetSpectra::HerwigJetSpectra(const std::string &name) Calling ctor" << std::endl;
}
*/
//____________________________________________________________________________..
HerwigJetSpectra::~HerwigJetSpectra()
{
  if(verbosity>5) std::cout << "HerwigJetSpectra::~HerwigJetSpectra() Calling dtor" << std::endl;
}

//____________________________________________________________________________..
int HerwigJetSpectra::Init(PHCompositeNode *topNode)
{
  	if(verbosity>5) std::cout << "HerwigJetSpectra::Init(PHCompositeNode *topNode) Initializing" << std::endl;
  	if(this->do_pythia){
		pythiaNode=new PHCompositeNode("pythiaNode");	
		pythiaNode->makePersistent();
		pythiaNode->setParent(topNode); 
		this->pythiagen=PythiaGenerator(topNode, this->trig_val);
		if(verbosity > 0 ) std::cout<<"Running a pythia generator as well as the Herwig analysis"<<std::endl;
	}
	
		
	return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
int HerwigJetSpectra::InitRun(PHCompositeNode *topNode)
{
  if(verbosity>5) std::cout << "HerwigJetSpectra::InitRun(PHCompositeNode *topNode) Initializing for Run XXX" << std::endl;
  return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
int HerwigJetSpectra::process_event(PHCompositeNode *topNode)
{
  	n_evt++;
 	 if(verbosity>5) std::cout << "HerwigJetSpectra::process_event(PHCompositeNode *topNode) Processing Event" << n_evt << std::endl;
  	if(!seperate_pythia_file) getKinematics(topNode, this->HerwigKin, false); 
	else getKinematics(topNode, this->PythiaKin, false);
  	if(this->do_pythia){
		pythiagen->process_event(topNode);
		if(verbosity > 0 ) std::cout<<"Generated partner pythia event for event "<<n_evt <<std::endl; 
		getKinematics(topNode, this->PythiaKin, true);
	}
	
  	return Fun4AllReturnCodes::EVENT_OK;
}
int HerwigJetSpectra::getKinematics(PHCompositeNode *topNode, JetKinematicPlots* Kinemats, bool pythia_run) 
{
  PHHepMCGenEventMap *phg=findNode::getClass<PHHepMCGenEventMap>(topNode, "PHHepMCGenEventMap");
  if(!phg){
	if(verbosity>0) std::cout<<"Did not find event map"<<std::endl;
	return 1;
	}
  int np=0, np_orig=0, hep_ev=0;
  double pt_lead=0, E_total=0;
  for ( PHHepMCGenEventMap::ConstIter eventIter=phg->begin(); eventIter != phg->end(); ++eventIter)
  {
	hep_ev++;
	if(pythia_run  && hep_ev == 5 )continue;
	PHHepMCGenEvent* hpev=eventIter->second;
	if(hpev){
		HepMC::GenEvent* ev=hpev->getEvent();
		if(!ev){
			if(verbosity>0) std::cout<<"Did not find any event" <<std::endl;
			continue;
		}
		if(verbosity>0) std::cout<<"Begin processing event"<<std::endl;
		PHHepMCGenEvent* origvtx=phg->get(0);
		for(auto w:ev->weights()) Kinemats->h_weight->Fill(w);
		float x_vtx=origvtx->get_collision_vertex().x(), y_vtx=origvtx->get_collision_vertex().y(), z_vtx=origvtx->get_collision_vertex().z(); //here is the vertex
		float r=sqrt(x_vtx*x_vtx+y_vtx*y_vtx);
		Kinemats->h_vertex->Fill(r, z_vtx);
		HepMC::GenParticle* pb=ev->beam_particles().first;
		HepMC::GenVertex* ov=pb->end_vertex();
		if(verbosity>1) std::cout<<"looking into the originating partons" <<std::endl;
		float jetptlead=0;
		for(HepMC::GenVertex::particles_out_const_iterator iter=ov->particles_out_const_begin(); iter !=ov->particles_out_const_end(); ++iter)
		{
				if(verbosity>2) std::cout<<"starting the analysis on the parton" <<std::endl;
				jetobj* Jet=new jetobj;
				if(verbosity>2) std::cout<<"successfully created the jet object" <<std::endl;
				Jet->originating_parton=(*iter);
				if(verbosity>2) std::cout<<"have loaded the originating particle into the Jet object"<<std::endl;
		 		try{
					Jet->jet_particles=IDJets(topNode, (*iter), pythia_run); //ids all the daughter particles coming from the originating partons 	
				}
				catch (std::exception& ex){
					if(verbosity>0) std::cout<<"caught an exception in the jetid stage as " <<ex.what() <<std::endl;
				}
				
				if(verbosity>1) std::cout<<"Identified Jet"<<std::endl;
				double px=(*iter)->momentum().px();
				double py=(*iter)->momentum().py();
				double pz=(*iter)->momentum().pz();
				double phi=atan2(py, px);
				double pt=sqrt(px*px+py*py);
				double eta=asinh(pz/pt);
				double mass=(*iter)->generated_mass(); 
				double E=(*iter)->momentum().e();
				double ET=sqrt(mass*mass + pt*pt); 
				Kinemats->h_phi_orig->Fill(phi, ET);
				Kinemats->h_eta_orig->Fill(eta, ET);
				Kinemats->h_pt_orig->Fill(pt);
				Kinemats->h_eta_hit_orig->Fill(eta);
				Kinemats->h_phi_hit_orig->Fill(phi);
				Kinemats->h_mass_orig->Fill(mass);
				Kinemats->h_E_orig->Fill(E);
				Kinemats->h_hits_orig->Fill(eta, phi);
				np_orig++;
				//if(pt>pt_lead) pt_lead=pt;
				Kinemats->h_status_orig->Fill((*iter)->status());
				Kinemats->h_ET_orig->Fill(ET);
				float mj=0, R=0, pxj=0, pyj=0, etj=0;
				if(verbosity>1) std::cout<<"Measuring the kinematics of the jet" <<std::endl;
				if(Jet->jet_particles.size() == 0 ) continue;
				HepMC::GenParticle* seed=NULL;
				float pt_seed=0;
				std::vector<HepMC::GenParticle*> r02, r04, r06;
				for(auto p:Jet->jet_particles){ 
					if(getPt(p) > pt_seed){
						 seed=p;
						 pt_seed=getPt(p);
					}
				}
				for(auto p:Jet->jet_particles){
					mj+=p->momentum().m();
					//pxj+=p->momentum().px();
					//pyj+=p->momentum().py();
					ptj=getPt(p);
					etj+=p->momentum().e()/cosh(p->momentum().eta());
					ej+=p->momentum().e();
					for(auto n:Jet->jet_particles){
						float rt=getR(p,n);
						if(rt>R) R=rt;
					}
					float r1=getR(p, seed);
					if(r1 <= 0.2) r02.push_back(p);
					if(r1 <= 0.4) r04.push_back(p);
					if(r1 <= 0.6) r06.push_back(p);
					h_pt_R->Fill(r1, getPt(p)/(float)pt_seed);
				}	
				std::cout<<"Done with calc on the jet, recording it now" <<std::endl;
				Jet->mass=mj;
				//calculate energy correlators, divide out at end
				float et2=getE2C(Jet->jet_particles, h_e2c, ej);
				float et2_2=getE2C(r02, h_e2c_2, ej);
				float et2_4=getE2C(r04, h_e2c_4, ej);
				float et2_6=getE2C(r06, h_e2c_6, ej);
				h_e2ct->Fill(et2);
				h_e2ct_2->Fill(et2_2);
				h_e2ct_4->Fill(et2_4);
				h_e2ct_6->Fill(et2_6);
				if(abs(Jet->originating_parton->pdg_id()) == 21) getE2C(Jet->jet_particles, h_e2c_g);
				else if (abs(Jet->originating_parton->pdg_id()) < 9 ) getE2C(Jet->jet_particles, h_e2c_q);
				float et3=getE3C(Jet->jet_particles, h_e3c);
				float et3_2=getE3C(r02, h_e3c_2);
				float et3_4=getE3C(r04, h_e3c_4);
				float et3_6=getE3C(r06, h_e3c_6);
				h_e3ct->Fill(et3);
				h_e3ct_2->Fill(et3_2);
				h_e3ct_4->Fill(et3_4);
				h_e3ct_6->Fill(et3_6);
				if(abs(Jet->originating_parton->pdg_id()) == 21) getE3C(Jet->jet_particles, h_e3c_g);
				else if (abs(Jet->originating_parton->pdg_id()) < 9 ) getE3C(Jet->jet_particles, h_e3c_q);
				std::cout<<"Have now calculated the 2pt and 3pt energy correlators" <<std::endl;
				Jet->ET=etj;
				Jet->pt=ptj;
				Jet->R=R/2;
				Jet->phi=phi;
				Jet->eta=eta;
				Kinemats->h_Jet_pt->Fill(Jet->pt);
				Kinemats->h_Jet_R->Fill(Jet->R);
				Kinemats->h_Jet_npart->Fill(Jet->jet_particles.size());
				if(Jet->pt > jetptlead) jetptlead=Jet->pt;
				delete Jet;
		}
		if(jetptlead > 0 ) Kinemats->h_Jet_pt_lead->Fill(jetptlead);
		//Now need to get the produced particles and differentiate from the end particles
		std::vector<HepMC::GenParticle*> final_state_particles;
		for(HepMC::GenEvent::particle_const_iterator iter=ev->particles_begin(); iter !=ev->particles_end(); ++iter){
	if(!(*iter)->end_vertex() && ((*iter)->status() == 1 || (pythia_run && (*iter)->status() > 0 ))){ //only pick up final state particles
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
		if((*iter)->status()== 1 || (pythia_run && (*iter)->status() > 0 )) E_total+=E;
		Kinemats->h_phi->Fill(phi, E);
		Kinemats->h_eta->Fill(eta, E);
		Kinemats->h_eta_hit->Fill(eta);
		Kinemats->h_phi_hit->Fill(phi);
		Kinemats->h_hits->Fill(eta, phi);
		Kinemats->h_pt->Fill(pt);
		Kinemats->h_mass->Fill(mass);
		Kinemats->h_pdg_id->Fill((*iter)->pdg_id());
		Kinemats->h_E->Fill(E);
		Kinemats->h_ET->Fill(ET);
		Kinemats->h_status->Fill((*iter)->status());
	}
	
	//	JetCollection* ICPRjets1=new JetCollection("Itterative Cone with Progressive Removal", 0.1, 0.1);
		JetCollection* ICPRjets2=new JetCollection("Itterative Cone with Progressive Removal", 0.2, 0.1);
	//	JetCollection* ICPRjets3=new JetCollection("Itterative Cone with Progressive Removal", 0.3, 0.1);
		JetCollection* ICPRjets4=new JetCollection("Itterative Cone with Progressive Removal", 0.4, 0.1);
	//	JetCollection* ICPRjets5=new JetCollection("Itterative Cone with Progressive Removal", 0.5, 0.1);
		JetCollection* ICPRjets6=new JetCollection("Itterative Cone with Progressive Removal", 0.6, 0.1);
	//	JetCollection* ICPRjets7=new JetCollection("Itterative Cone with Progressive Removal", 0.7, 0.1);
	//	JetCollection* ICPRjets8=new JetCollection("Itterative Cone with Progressive Removal", 0.8, 0.1);
	//	JetCollection* ICPRjets9=new JetCollection("Itterative Cone with Progressive Removal", 0.9, 0.1);
		JetCollection* ICPRjetsfull=new JetCollection("Itterative Cone with Progressive Removal", 1.0, 0.1);
		std::vector<JetCollection*> ICPRjets {ICPRjets2, ICPRjets4, ICPRjets6, ICPRjetsfull};//{ ICPRjets1,ICPRjets2, ICPRjets3, ICPRjets4, ICPRjets5,  ICPRjets6, ICPRjets7, ICPRjets8, ICPRjets9, ICPRjetsfull};
		
		std::vector<std::thread> thread_vector;
		for(auto j: ICPRjets){
			//if(j->jetR != 0.4) continue;
			thread_vector.push_back(std::thread(&HerwigJetSpectra::fastjetID, this, final_state_particles, j, 1, j->jetR, 0.1)); //thread vector to do the jet search 
	//		fastjetID(final_state_particles, j, 1, j->jetR, 0.1);
		  //will do a new threads to include all the jets
		}
		for(int i=0; i<(int) thread_vector.size(); i++) thread_vector[i].join();
 	
		for(auto j:ICPRjets){
			for(auto i:j->Identified_jets){ 
			float e2c=getE2C(i->jet_particles, h_E2C_IC[j->jetR]);
			float e3c=getE3C(i->jet_particles, h_E3C_IC[j->jetR]);
			h_E2CT_IC[j->jetR]->Fill(e2c);
			h_E3CT_IC[j->jetR]->Fill(e3c);
		}
		}
	
	
	}
	Kinemats->h_n_part->Fill(np);
	Kinemats->h_n_part_orig->Fill(np_orig);
	Kinemats->h_pt_leading->Fill(pt_lead);
	Kinemats->h_E_total->Fill(E_total);
	Kinemats->h_ev->Fill(hep_ev);
	
  return Fun4AllReturnCodes::EVENT_OK;
}
//__________________________________________________________________________
std::vector<HepMC::GenParticle*> HerwigJetSpectra::IDJets(PHCompositeNode *topNode, HepMC::GenParticle* originating_parton, bool pythia_run)
{
	//this is intended to identify all final state particles coming from each originating parton 
	std::vector<HepMC::GenParticle*> final_state_jet;
	HepMC::GenVertex* decay=originating_parton->end_vertex();
	if(!decay){
		final_state_jet.push_back(originating_parton); //if the original state does not decay
	}
	else{
		if(verbosity>1) std::cout<<"Have the decay vertex of the originating parton, now searching for all daughters"<<std::endl;
		std::unordered_set<int> final_state_barcodes;
		//int last_part=0, n_loop=0;
		//bool stuck=false, eov=false;
		std::unordered_map<HepMC::GenVertex*, HepMC::GenVertex::particles_out_const_iterator> holding_part;
		HepMC::GenVertex* active_vertex=decay;
		HepMC::GenVertex::particles_out_const_iterator par=decay->particles_out_const_begin(); 
		HepMC::GenVertex::particles_out_const_iterator parent_par=decay->particles_out_const_begin();
			//The goal is to go to the deepest level and collect all partilces that are final
	//		std::cout<<"Starting a new itteration of the loop" <<std::endl;
		if(verbosity>2) std::cout<<"have set up the structure needed, now entering while loop" <<std::endl;
		while(parent_par != decay->particles_out_const_end()){
			//The goal is to go to the deepest level and collect all partilces that are final
			if(verbosity>3) std::cout<<"Starting a new itteration of the loop" <<std::endl;
			auto last_it=holding_part.begin();
			if(holding_part.size() > 0  && last_it->first == decay ){
				 ++parent_par;
				 last_it->second = parent_par;
				if(parent_par == decay->particles_out_const_end()) break;
				if(verbosity>2) std::cout<<"Moved along the primary particle branch. Now have parton " <<(*parent_par)->barcode() <<std::endl; 
			} 
			if(par == decay->particles_out_const_begin() && holding_part.size() == 0 ){
				 holding_part.emplace(decay, par);
				 continue;
			}
			if( active_vertex && holding_part.size() > 0  && last_it->first && active_vertex != last_it->first){
				if(verbosity>2) std::cout<<"realigning the vertex to the branch tree " <<std::endl; 
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
					if (!(*par) || par >= active_vertex->particles_out_const_end() ) if(verbosity>4) std::cout<<"Ok somehow I got here despite seeming to be inconsistent???" <<std::endl;
					bool has_end_vertex=true;
					if ( par >= active_vertex->particles_out_const_end() ) if(verbosity>4) std::cout<<"The active parton is " <<std::distance( active_vertex->particles_out_const_end(), par) <<" steps away from the end iterator" <<std::endl;
					if( par >= active_vertex->particles_out_const_end() || par < active_vertex->particles_out_const_begin() ){
						if(verbosity>3) std::cout<<"I have no clue how this happened??????, somehow we are out of range of the vertex iterator??" <<std::endl;
						auto it=holding_part.cbegin();
						holding_part.erase(it);	
						continue;
					}
					//if(verbosity>0) std::cout<<"The issue below is on a vertex with barcode " <<(*par)->production_vertex()->barcode() <<std::endl;
					if(!(*par)->end_vertex()) has_end_vertex=false; //does the negation make a difference? it shouldn't I would think?
					if( has_end_vertex){
						//this is what we have to do if the particles have an end vertex, so moving deeper
						if( (*par)->end_vertex()->particles_out_size() > 0 ) 
						{
							//there is a well formed vertex
							if(verbosity>2) std::cout<<"The active vertex has barcode " <<active_vertex->barcode() <<" and the new active vertex has barcode " <<(*par)->end_vertex()->barcode() <<std::endl;
							active_vertex=(*par)->end_vertex();
							par=active_vertex->particles_out_const_begin();
							if(active_vertex && *par) holding_part.emplace(active_vertex, par);
							else{
								if(verbosity>1) std::cout<<"Something is off here " <<std::endl;
								break;
							}
							if(verbosity>3) std::cout<<"New depth is " <<holding_part.size() <<std::endl;
							if(verbosity>3) std::cout<<"The active vertex has barcode " <<active_vertex->barcode() <<" and the new holding partition vertex has barcode " <<holding_part.begin()->first->barcode() <<std::endl;
							continue;
						} //End of deepening by one
						else{
							if(verbosity>4) std::cout<<"For some reason the particle has an end vertex, but no apparent particles in the end vertex, will treat as a final state ? Depends on status which is " <<(*par)->status() <<std::endl;
							if(final_state_barcodes.find((*par)->barcode()) == final_state_barcodes.end()){
								if( (*par)->status() == 1 || (pythia_run && (*par)->status() > 0 )) final_state_jet.push_back(*par);
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
						if(verbosity>4) std::cout<<"Will treat as a final state? Depends on status which is " <<(*par)->status() <<std::endl;
						if(final_state_barcodes.find((*par)->barcode()) == final_state_barcodes.end()){
							if( (*par)->status() == 1 || (pythia_run && (*par)->status() > 0 )){
								 final_state_jet.push_back(*par);
								if(verbosity>2) std::cout<<"Added a parton to the final state jet, have  " <<final_state_jet.size() <<std::endl;
							}
							final_state_barcodes.insert((*par)->barcode());
						} //behavior of final state particlse
						else if(verbosity>3) std::cout<<"Have already examined this particle it has barcode " <<(*par)->barcode() <<std::endl;
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
					if(verbosity>4) std::cout<<"the particle itterator is sitting at the end of the itterator chain" <<std::endl;
					if(holding_part.size() > 0 ){
						auto it = holding_part.cbegin();
						holding_part.erase(it);
					}
					else break;
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
		if(verbosity>4) std::cout<<"Got rid of the pointers" <<std::endl;
		holding_part.clear();
		final_state_barcodes.clear();
	} //end the else condition for needing to search for daughters
	if(verbosity>1) std::cout<<"The jet has "<<final_state_jet.size() <<" particles in the final state" <<std::endl;
	delete decay;
	if(verbosity>3) std::cout<<"deleted the decay vertex" <<std::endl;
	return final_state_jet;		 
				 
}
float HerwigJetSpectra::getE2C(std::vector<HepMC::GenParticle*> jet, TH1F* h_e2, float energy /*=0*/)
{
	float jet_total_e=0, eec=0;
	for(auto j:jet) jet_total_e+=j->momentum().e();
	if(energy != 0 ) jet_total_e=energy;
	for(int i=0; i < (int) jet.size() - 1; i++){
		for(auto k=i+1; k < (int) jet.size(); k++){
			float erc=(jet.at(i)->momentum().e()*jet.at(k)->momentum().e())/pow(jet_total_e, 2);
			h_e2->Fill(getR(jet.at(i), jet.at(k)),erc);
			eec+=erc;
		}
	}
	return eec;
} 
float HerwigJetSpectra::getE3C(std::vector<HepMC::GenParticle*> jet, TH1F* h_e3)
{
	float jet_total_e=0, eec=0;
	for(auto j:jet) jet_total_e+=j->momentum().e();
	for(int j=0; j < (int) jet.size()-2; j++){
		for(int k=j+1; k < (int) jet.size()-1; k++){
			for(int l=k+1; l < (int)jet.size(); l++){
				float erc=(jet.at(j)->momentum().e()*jet.at(k)->momentum().e()*jet.at(l)->momentum().e())/pow(jet_total_e, 3);
				h_e3->Fill(std::max({getR(jet.at(j),jet.at(k)), getR(jet.at(j), jet.at(l)), getR(jet.at(l), jet.at(k))}),erc);
			eec+=erc;
			}
		}
	}
	return eec;
} 
int HerwigJetSpectra::fastjetID( std::vector<HepMC::GenParticle*> final_states, JetCollection* jet_plot, int method=5, float R=0.4, float pt_min=0.0 )
{
	//Runs the fastJet identifer over the dataset using the passed method
	// method:
	// 1 : IC-Progressive Removal
	// 2 : IC-Split Merge
	// 3 : Seedless Infrared Safe Cone
	// 4 : kt
	// 5 : anti-kt
	// 6 : Cambridge / Aachen
	// 7 : ARCLUS
	// 8 : Optimal
	// The itterative cone methods are implemented by hand, as they are not part of the fast jet package, but are easy enough to implement using standard methods	
	// Methods 4-6 are native to the fast jet code
	// Methods 7 & 8 are not implemented in fast jet and are a pain, so are left as an exercise to the reader 
	//
	// Takes in as a required parameter the collection of final state particles of the jet, fills in the histograms associated with the jet struct object as defined in the module 
	//
	// Optionally takes in method (default kt), R (default 0.4) and minimum p_t (default 0 GeV) 
	int n_jets=0;
	float max_prev=0;
	switch (method)
	{
		case 1:
		{
			bool find_more=true;
			while(find_more){
			float max_jet_pt=0;
			if(final_states.size() == 0 ){
				find_more=false;
				continue;
			}
			for(auto p:final_states){
				float pt=getPt(p);
				if(pt > max_jet_pt) max_jet_pt=pt;
			}
			if(max_jet_pt > pt_min) max_jet_pt=GetAnIterativeCone(&final_states, R, true, pt_min, jet_plot);
			else{
				find_more=false;	
				continue;
			}
			if(max_jet_pt == max_prev){
				find_more =false;
				continue;
			}
			std::cout<<"There are jets found how many? " <<n_jets <<std::endl;
			n_jets++;
			max_prev=max_jet_pt;
			}
		}
		break;
		case 2:
		{
			bool find_more=true;
			while(find_more){
			float max_jet_pt=0;
			if(final_states.size() == 0 ){
				find_more=false;
				continue;
			}
			for(auto p:final_states){
				float pt=getPt(p);
				if(pt > max_jet_pt) max_jet_pt=pt;
			}
			if(max_jet_pt > pt_min) max_jet_pt=GetAnIterativeCone(&final_states, R, false, pt_min, jet_plot);
			else{
				find_more=false;	
				continue;
			}
			}
			n_jets++;
			}
		break;
		case 3:
		{	//the seedless is in fastjet I think	
				
		}
			break;
		case 4:
		{	//kt method
			n_jets++;
		}
			break;
	}
	return n_jets;
		
}
float HerwigJetSpectra::getPt(HepMC::GenParticle* p)
{
	float pt=0;
	pt=pow(p->momentum().px(), 2) + pow(p->momentum().py(), 2);
	pt=sqrt(pt);
	return pt;
}
float HerwigJetSpectra::getR(HepMC::GenParticle* p1, HepMC::GenParticle* p2){
	float eta_dist=p1->momentum().pseudoRapidity() - p2->momentum().pseudoRapidity();
	float phi_dist=p1->momentum().phi() - p2->momentum().phi();
	phi_dist=abs(phi_dist);
	if(phi_dist > PI) phi_dist=2*PI-phi_dist;
	float R=sqrt(pow(eta_dist,2) + pow(phi_dist, 2));
	return R;
}
float HerwigJetSpectra::GetAnIterativeCone(std::vector<HepMC::GenParticle*> *final_states, float R_cone=0.4, bool progressive_removal=true, float pt_min=0, JetCollection* jets=NULL)
{
//find the cone with the highest pt  seed and returns the seed pt of the next highest cone condidtatei
	HepMC::GenParticle* seed=final_states->at(0);
	if(!seed) return -1;
	float seed_pt=getPt(seed);
	std::cout<<"Looking for a jet centered around a seed pt of " <<seed_pt<<std::endl;
	std::unordered_set<int> jet_barcodes;
	for(auto p:*final_states){
		float p_pt=getPt(p);
		if(seed_pt < p_pt){
		       	seed=p;
			seed_pt=getPt(seed);
		}
	}
	//noew I have the seed particle, the next step is to get the conde associate iwth it 
	auto seed4=seed->momentum(), jetmom=seed->momentum();
	jetmom.setPx(0);
	jetmom.setPy(0);
	jetmom.setPz(0);
	jetmom.setE(0);
	std::vector<HepMC::GenParticle*> cone;
	int itterations=0;
	std::cout<<"Preparing to do the IC cone indentification proceedure" <<std::endl;
	while( seed4 != jetmom) {
		
		float seedl=sqrt(abs(pow(seed4.e(), 2) - pow(seed4.px(), 2) - pow(seed4.py(), 2) - pow(seed4.pz(), 2)));
		seed4.setPx(seed4.px()/seedl); //this is the value that must stay fixed for the jet to be id-ed
		seed4.setPy(seed4.py()/seedl); //this is the value that must stay fixed for the jet to be id-ed
		seed4.setPz(seed4.pz()/seedl); //this is the value that must stay fixed for the jet to be id-ed
		seed4.setE(seed4.e()/seedl); //this is the value that must stay fixed for the jet to be id-ed
		for(auto p:*final_states){
			//get the cone candidates
			float R=getR(p, seed);
			if (R <=R_cone) cone.push_back(p);
			//just take the geometric cone for right now
		}
		for(auto j:cone){
			jet_barcodes.insert(j->barcode());
			if(getPt(j) < pt_min) continue;
			if(j->barcode() == seed->barcode()) continue;
			jetmom.setPx(jetmom.px() + j->momentum().px());
			jetmom.setPy(jetmom.py() + j->momentum().py());
			jetmom.setPz(jetmom.pz() + j->momentum().pz());
			jetmom.setE(jetmom.e() + j->momentum().e());
		}
		float jetl=sqrt(abs(pow(jetmom.e(), 2) - pow(jetmom.px(), 2) - pow(jetmom.py(), 2) - pow(jetmom.pz(), 2)));
		jetmom.setPx(jetmom.px()/jetl); //this is the value that must stay fixed for the jet to be id-ed
		jetmom.setPy(jetmom.py()/jetl); //this is the value that must stay fixed for the jet to be id-ed
		jetmom.setPz(jetmom.pz()/jetl); //this is the value that must stay fixed for the jet to be id-ed
		jetmom.setE(jetmom.e()/jetl); //this is the value that must stay fixed for the jet to be id-ed
		if(itterations > 10 ) break;	
		if(jetmom != seed4)
		{
			seed4=jetmom;
			jetmom.setPx(0);
			jetmom.setPy(0);
			jetmom.setPz(0);
			jetmom.setE(0);
			jet_barcodes.clear();
			jet_barcodes.insert(seed->barcode());
			cone.clear();
			itterations++;
		}
	}
	if(cone.size() == 0 || jet_barcodes.size() == 0 || final_states->size() == 0 ) return -1;
	float next_pt=0;
	jetobj* Jet=new jetobj("ICPR");
	Jet->jet_particles=cone;
	jets->Identified_jets.push_back(Jet);
	jets->n_jets++;
	/*for(auto f=final_states->begin(); f != final_states->end(); ++f){
		if(!(*f)) continue;
	       	if(jet_barcodes.find((*f)->barcode()) != jet_barcodes.end() ){
	       		if(progressive_removal) final_states->erase(f);
		}
		else{
			if(getPt(*f) > next_pt) next_pt=getPt(*f);
			}
		}*/

	std::vector<HepMC::GenParticle*> temp_states;
	for(auto f:*final_states){
		if(jet_barcodes.find(f->barcode()) != jet_barcodes.end()) {
			if(!progressive_removal) temp_states.push_back(f);
		}
		else{
			if(getPt(f) > next_pt) next_pt = getPt(f);
			temp_states.push_back(f);
		}
	}
	final_states=&temp_states;
	return next_pt;
	
}	
		
//____________________________________________________________________________..
PHPythia8* HerwigJetSpectra::PythiaGenerator(PHCompositeNode *topNode, int trigger)
{
	//This is method to generate a pythia event in a 1-to-1 rate to the hepmc events
	//Need to generate with the trigger 
	PHPy8JetTrigger *jetTrig = new PHPy8JetTrigger();
//	std::string jetname="MB";
	if(trigger > 0 )
	{
		jetTrig->SetMinJetPt(trigger);
		///jetname=std::to_string(trigger)+"GeV";
	}
	PHPythia8* pythiagen=new PHPythia8();
	std::string config_file_name="../herwig_files/pythiaref_";
	config_file_name+=trig;
	config_file_name+=".cfg";
	pythiagen->set_config_file(config_file_name);
	pythiagen->beam_vertex_parameters(0,0,0,0,0,5);
	if(trigger > 0 ) pythiagen->register_trigger(jetTrig);
	pythiagen->Init(topNode);
	if(verbosity > 1 ) std::cout<<"Running pythia with triger of " <<trig <<std::endl;
	return pythiagen;
}	
//___________________________________________________________________________..
int HerwigJetSpectra::ResetEvent(PHCompositeNode *topNode)
{
//  if(verbosity>0) std::cout << "HerwigJetSpectra::ResetEvent(PHCompositeNode *topNode) Resetting internal structures, prepare for next event" << std::endl;
  return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
int HerwigJetSpectra::EndRun(const int runnumber)
{
 
	if(verbosity>5) std::cout << "HerwigJetSpectra::EndRun(const int runnumber) Ending Run for Run " << runnumber << std::endl;
  return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
int HerwigJetSpectra::End(PHCompositeNode *topNode)
{ 
 if(verbosity>5) std::cout << "HerwigJetSpectra::End(PHCompositeNode *topNode) This is the End..." << std::endl;
 if(verbosity>0) std::cout<<"Ran over " <<n_evt<<" events" <<std::endl;
  return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
int HerwigJetSpectra::Reset(PHCompositeNode *topNode)
{
 if(verbosity>5) std::cout << "HerwigJetSpectra::Reset(PHCompositeNode *topNode) being Reset" << std::endl;
  return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
void HerwigJetSpectra::Print(const std::string &what) const
{
  //average over number of jets, need to put into the new form
  h_pt_R->Scale(1/(float) h_Jet_pt->GetEntries());
  h_e2c->Scale(1/(float) h_Jet_pt->GetEntries());
  h_e3c->Scale(1/(float) h_Jet_pt->GetEntries());
  h_e2c_2->Scale(1/(float) h_Jet_pt->GetEntries());
  h_e3c_2->Scale(1/(float) h_Jet_pt->GetEntries());
  h_e2c_4->Scale(1/(float) h_Jet_pt->GetEntries());
  h_e3c_4->Scale(1/(float) h_Jet_pt->GetEntries());
  h_e2c_6->Scale(1/(float) h_Jet_pt->GetEntries());
  h_e3c_6->Scale(1/(float) h_Jet_pt->GetEntries());
  h_e2c_q->Scale(1/(float) h_Jet_pt->GetEntries());
  h_e3c_q->Scale(1/(float) h_Jet_pt->GetEntries());
  h_e2c_g->Scale(1/(float) h_Jet_pt->GetEntries());
  h_e3c_g->Scale(1/(float) h_Jet_pt->GetEntries());
  h_pt_R->Write();
  h_e2c->Write();
  h_e3c->Write();
  h_e2c_q->Write();
  h_e3c_q->Write();
  h_e2c_g->Write();
  h_e3c_g->Write();
  h_e2c_2->Write();
  h_e3c_2->Write();
  h_e2c_4->Write();
  h_e3c_4->Write();
  h_e2c_6->Write();
  h_e3c_6->Write();
  h_e2ct->Write();
  h_e3ct->Write();
  h_e2ct_2->Write();
  h_e3ct_2->Write();
  h_e2ct_4->Write();
  h_e3ct_4->Write();
  h_e2ct_6->Write();
  h_e3ct_6->Write();
  for(int i=0; i<11; i++){
	float r=i/10.;
	if(h_E2CT_IC.at(r)->GetEntries() > 0 ){
		h_E2C_IC.at(r)->Scale(1/((float)h_E2CT_IC.at(r)->GetEntries()));
		h_E2C_IC.at(r)->Write();
		h_E2CT_IC.at(r)->Write();
	}
	if((h_E3CT_IC.at(r))->GetEntries() > 0 ){
		h_E3C_IC.at(r)->Scale(1/((float) h_E3CT_IC.at(r)->GetEntries()));
		h_E3C_IC.at(r)->Write();
		h_E3CT_IC.at(r)->Write();
	}
  }
  if(verbosity>5) std::cout << "HerwigJetSpectra::Print(const std::string &what) const Printing info for " << what << std::endl;
  TFile* f=new TFile(Form("herwig_with_pythia_output_%s_jetpt_with_jetalgo.root", trig.c_str()), "RECREATE");
  HerwigKin->h_pt->Write();
  HerwigKin->h_phi->Write();
  HerwigKin->h_eta->Write();
  HerwigKin->h_mass->Write();
  HerwigKin->h_pdg_id->Write();
  HerwigKin->h_E->Write();
  HerwigKin->h_n_part->Write();
  HerwigKin->h_status->Write();
  HerwigKin->h_phi_hit->Write();
  HerwigKin->h_eta_hit->Write();
  HerwigKin->h_pt_orig->Write();
  HerwigKin->h_phi_orig->Write();
  HerwigKin->h_eta_orig->Write();
  HerwigKin->h_phi_hit_orig->Write();
  HerwigKin->h_eta_hit_orig->Write();
  HerwigKin->h_mass_orig->Write();
  HerwigKin->h_E_orig->Write();
  HerwigKin->h_n_part_orig->Write();
  HerwigKin->h_status_orig->Write();
  HerwigKin->h_pt_leading->Write();
  HerwigKin->h_ev->Write();
  HerwigKin->h_E_total->Write();
  HerwigKin->h_vertex->Write(); 
  HerwigKin->h_weight->Write();
  HerwigKin->h_ET->Write();
  HerwigKin->h_ET_orig->Write();
  HerwigKin->h_Jet_pt->Write();
  HerwigKin->h_Jet_R->Write();
  HerwigKin->h_Jet_npart->Write();
  HerwigKin->h_Jet_pt_lead->Write();
  HerwigKin->h_hits->Write();
  HerwigKin->h_hits_orig->Write();
  if(do_pythia){
	  PythiaKin->h_pt->Write();
	  PythiaKin->h_phi->Write();
	  PythiaKin->h_eta->Write();
	  PythiaKin->h_mass->Write();
	  PythiaKin->h_pdg_id->Write();
	  PythiaKin->h_E->Write();
	  PythiaKin->h_n_part->Write();
	  PythiaKin->h_status->Write();
	  PythiaKin->h_phi_hit->Write();
	  PythiaKin->h_eta_hit->Write();
	  PythiaKin->h_pt_orig->Write();
	  PythiaKin->h_phi_orig->Write();
	  PythiaKin->h_eta_orig->Write();
	  PythiaKin->h_phi_hit_orig->Write();
	  PythiaKin->h_eta_hit_orig->Write();
	  PythiaKin->h_mass_orig->Write();
	  PythiaKin->h_E_orig->Write();
	  PythiaKin->h_n_part_orig->Write();
	  PythiaKin->h_status_orig->Write();
	  PythiaKin->h_pt_leading->Write();
	  PythiaKin->h_ev->Write();
	  PythiaKin->h_E_total->Write();
	  PythiaKin->h_E_total->Write();
	  PythiaKin->h_vertex->Write(); 
	  PythiaKin->h_weight->Write();
	  PythiaKin->h_ET->Write();
	  PythiaKin->h_ET_orig->Write();
	  PythiaKin->h_Jet_pt->Write();
	  PythiaKin->h_Jet_R->Write();
	  PythiaKin->h_Jet_npart->Write();
	  PythiaKin->h_Jet_pt_lead->Write();
	  PythiaKin->h_hits->Write();
	}	
f->Write();
}
