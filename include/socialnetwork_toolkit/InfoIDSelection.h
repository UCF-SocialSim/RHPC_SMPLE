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
/* InfoIDSelection.h */
#ifndef INCLUDE_INFOIDSELECTION_H
#define INCLUDE_INFOIDSELECTION_H

#include "InfoID.h"

namespace rhpc_smple {

template<typename AGENT, typename ACTIONTYPE, typename PAYLOAD, typename PLATFORM>
class InfoIDSelector{
public:
	PLATFORM* platform;

	InfoIDSelector(PLATFORM* containingPlatform): platform(containingPlatform){}
	virtual ~InfoIDSelector(){}

	virtual void addInfoIDs(AGENT* actor, PAYLOAD& payload) = 0; // Pure virtual
};


// Test class that does nothing- will add no info IDs.
template<typename AGENT, typename ACTIONTYPE, typename PAYLOAD, typename PLATFORM>
class DoNothingInfoIDSelector: public InfoIDSelector<AGENT, ACTIONTYPE, PAYLOAD, PLATFORM>{
public:
	DoNothingInfoIDSelector(PLATFORM* containingPlatform): InfoIDSelector<AGENT, ACTIONTYPE, PAYLOAD, PLATFORM>(containingPlatform){}
	virtual ~DoNothingInfoIDSelector(){}

	virtual void addInfoIDs(AGENT* actor, PAYLOAD& payload){} // Does nothing
};



template<typename AGENT, typename ACTIONTYPE, typename PAYLOAD, typename PLATFORM>
class PopularInfoIDSelector: public InfoIDSelector<AGENT, ACTIONTYPE, PAYLOAD, PLATFORM>{
public:
	PopularInfoIDSelector(PLATFORM* containingPlatform): InfoIDSelector<AGENT, ACTIONTYPE, PAYLOAD, PLATFORM>(containingPlatform){}
	virtual ~PopularInfoIDSelector(){}

	virtual void addInfoIDs(AGENT* actor, PAYLOAD& payload){
		InfoID* id_ptr =  this->platform->getPopularInfoID(0);
		if(id_ptr != 0){
			payload.informationIDs.insert(id_ptr);
			if(repast::Random::instance()->nextDouble() < .2){
				InfoID* id_ptr2 =  this->platform->getPopularInfoID(0);
				if((id_ptr2 != 0) && (id_ptr2 != id_ptr)) payload.informationIDs.insert(id_ptr2);
			}
		}
	}
};



template<typename AGENT, typename ACTIONTYPE, typename PAYLOAD, typename PLATFORM>
class ShockBasedInfoIDSelector: public InfoIDSelector<AGENT, ACTIONTYPE, PAYLOAD, PLATFORM>{
public:
	ShockBasedInfoIDSelector(PLATFORM* containingPlatform): InfoIDSelector<AGENT, ACTIONTYPE, PAYLOAD, PLATFORM>(containingPlatform){}
	virtual ~ShockBasedInfoIDSelector(){}

	virtual void addInfoIDs(AGENT* actor, PAYLOAD& payload){
		InfoID* id_ptr =  this->platform->getShockedInfoID();
		if(id_ptr != 0){
			payload.informationIDs.insert(id_ptr);
			if(repast::Random::instance()->nextDouble() < .2){
				InfoID* id_ptr2 =  this->platform->getShockedInfoID();
				if((id_ptr != 0) && (id_ptr2 != id_ptr)) payload.informationIDs.insert(id_ptr2);
			}
		}
	}
};


template<typename AGENT, typename ACTIONTYPE, typename PAYLOAD, typename PLATFORM>
class EventCountBasedInfoIDSelector: public InfoIDSelector<AGENT, ACTIONTYPE, PAYLOAD, PLATFORM>{
public:
	EventCountBasedInfoIDSelector(PLATFORM* containingPlatform): InfoIDSelector<AGENT, ACTIONTYPE, PAYLOAD, PLATFORM>(containingPlatform){}
	virtual ~EventCountBasedInfoIDSelector(){}

	virtual void addInfoIDs(AGENT* actor, PAYLOAD& payload){
		std::vector<InfoID*> infoIDsToAdd = this->platform->getEventCountInfoIDs();
		if(!infoIDsToAdd.empty()) {
			for(auto itr = infoIDsToAdd.begin(); itr!= infoIDsToAdd.end(); ++itr)
				payload.informationIDs.insert(*itr);
		}
	}
};

}  // End Namespace rhpc_smple

#endif /* INCLUDE_INFOIDSELECTION_H */
