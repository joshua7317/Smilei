
#ifndef SPECIESFACTORY_H
#define SPECIESFACTORY_H

#include "Species.h"
#include "Species_norm.h"
#include "Species_rrll.h"

#include "PicParams.h"
#include "SmileiMPI.h"

#include "Tools.h"

class SpeciesFactory {
public:
	static Species* create(PicParams& params, int ispec, SmileiMPI* smpi) {
		Species* sp = NULL;
		if (params.species_param[ispec].dynamics_type=="norm") {
			// Species with Boris dynamics
			sp = new Species_norm(&params, ispec, smpi);
		} else if (params.species_param[ispec].dynamics_type=="rrll") {
			// Species with Boris dynamics + Radiation Back-Reaction (using the Landau-Lifshitz formula)
			sp = new Species_rrll(&params, ispec, smpi);
		} // END if

		return sp;
	}

	static std::vector<Species*> createVector(PicParams& params, SmileiMPI* smpi) {
		std::vector<Species*> vecSpecies;
		vecSpecies.resize(params.n_species);
        
        Species *electron_species=NULL;

		for (unsigned int ispec=0 ; ispec<params.n_species ; ispec++) {
			PMESSAGE( 0, smpi->getRank(), "Initializing Species " << ispec);
			vecSpecies[ispec] = SpeciesFactory::create(params, ispec, smpi);
            if (params.species_param[ispec].species_type=="electron") {
                electron_species=vecSpecies[ispec];
                DEBUG(".......................... electron found");
            }
            DEBUG("............................... " << vecSpecies[ispec]->Ionize);
			PMESSAGE( 0, smpi->getRank(), vecSpecies[ispec]->getNbrOfParticles() << " Particles of species " << ispec );

		} // END for ispec

		for (unsigned int ispec=0 ; ispec<params.n_species ; ispec++) {
            DEBUG(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
            if (vecSpecies[ispec]->Ionize)  {
                if (electron_species) {
                    vecSpecies[ispec]->electron_species=electron_species;
                    DEBUG("added electron species to species " << params.species_param[ispec].species_type);
                } else {
                    ERROR("Ionization needs a species of electrons to be defined");
                }
            }
		} // END for ispec

		return vecSpecies;
	}

};

#endif