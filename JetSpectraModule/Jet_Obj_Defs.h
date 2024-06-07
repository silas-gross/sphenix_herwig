#include <string>
#include <vector>

	struct jetobj{
		jetobj(std::string method="raw"):
			jet_id_method(method){};
		std::vector<HepMC::GenParticle*> jet_particles;
		std::string jet_id_method="raw";
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
	struct JetCollection{
		JetCollection(std::string method="raw",float R=0.4,float pt_min=0.0):
			jet_id_method(method), jetR(R), jetpt_min(pt_min) {};
		std::string jet_id_method="raw";
		float jetR=0.4;
		float jetpt_min=0.0;
		std::vector<jetobj*> Identified_jets;
		int n_jets=0;
	};

