/**
 * RHPC_SMPLE: Repast HPC Social Media PLatform Emulator
 *
 * Copyright (C) 2021  John T. Murphy
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * This code was authored by John T. Murphy, Ray Giroux, 
 * Christine Grayson, Don Johnson, and Brandon Barnes
 * with contributions from Chathika Gunaratne and Chathura Jayalath
 *
 * If you use this code or the tool in your work, please cite using the following bibtex:
 * @book{murphyAndWadsworth2021,
 *   author =       {Murphy, John T., and Wadsworth, Marin},
 *   title =        {RHPC SMPLE Users Manual},
 *   year =         {2021},
 *   url =          {http://USER_MANUAL_URL}
 * }
 */
/* InfoStore.h */
#ifndef INCLUDE_INFOSTORE_H
#define INCLUDE_INFOSTORE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sstream>
#include <map>
#include <iostream>
#include <fstream>

#include "boost/ptr_container/ptr_vector.hpp"
#include "boost/ptr_container/ptr_set.hpp"
#include "boost/algorithm/string.hpp"

#include "utilities/ReturnValues.h"
#include "utilities/SimpleWeightedSelector.h"
#include "socialnetwork_toolkit/InfoID.h"

namespace rhpc_smple {

class InfoStore{

private:
	boost::ptr_vector<InfoID>          infoIDs;
	SimpleWeightedSelector*            sws;
	std::map<std::string, std::string> synonyms;

public:
	InfoStore(): sws(0){
		sws = new SimpleWeightedSelector(-1);
	}

	virtual ~InfoStore(){
		delete sws;
	}

	// Adds an ID to this store. Weight is optional, default = 10
	// If an InfoID with the equivalent ID string is already present, will
	// adjust the weight of the original by adding the specified weight or the default.
	// If the ID is novel (no existing ID has the same ID string)
	// will add it to the collection with the specified weight or the default.
	InfoID& add(std::string infoID, long weight = 10);

	// Takes a string and returns the InfoID in this store with
	// the same ID name. If none exists, returns null (0)
	InfoID* find(std::string name);

	void get(std::set<InfoID*>& IDs, int n = 1);

	void getAll(std::set<std::string>& IDs);

	void initializeFromFileList(std::string fullFilePathname, long weight = 10);

	// Chooses one of the weighted options
	// And increments the amount of its weighting
	// So that it is more attractive next time.
	// DO NOT CALL IF SIZE IS ZERO!
	InfoID* pickAndVote(long weightToAdd);

	int size();

	void clear();

	void report(std::stringstream& ss);

};


}  // End Namespace rhpc_smple

#endif /* INCLUDE_INFOSTORE_H */
