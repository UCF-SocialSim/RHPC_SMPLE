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
/* Twitter_Agent_Package_xamples.h */
#ifndef INCLUDE_TWITTER_AGENT_PACKAGE_XAMPLES_H
#define INCLUDE_TWITTER_AGENT_PACKAGE_XAMPLES_H

#include <set>

#include "boost/date_time/posix_time/posix_time.hpp"

#include "repast_hpc/AgentId.h"

#include "socialnetwork_toolkit/InfoID.h"

#include "platforms/Twitter/Twitter_Definitions.h"

#include "xamples/platforms/Twitter/Twitter_Definitions_xamples.h"

using namespace boost::posix_time;
using namespace rhpc_smple;

namespace twitter{

/**
 *  Serializable Agent Package
 *
 *  Note that this breaks the child-class paradigm,
 *  because we are only creating ONE kind of agent
 *  package that will have to serve for all kinds of
 *  agents. This is not terrible- it will allow us to
 *  have simulations with mixed classes of agents, for
 *  example.
 */
struct TwitterAgentPackage{

public:
    int    		  			id;
    int    		  			rank;
    TwitterAgentType  		type;
    int    		 	 		currentRank;
    std::string 	  		hashTagID;
    repast::AgentId 		parentID;
    repast::AgentId 		creatorID;
    ptime         			dateCreated;
    std::set<InfoID*>       informationIDs;

    /* Constructors */
    TwitterAgentPackage(); // For serialization
    TwitterAgentPackage(TwitterAgentType agentType);
    TwitterAgentPackage(int idNum, int rankNUm, TwitterAgentType typeVal, int currentRankNum, std::string hashtag);

    /**
     *  For archive packaging
     *  This function is required by the Boost serialization library
     */
    template<class Archive>
    void serialize(Archive &ar, const unsigned int version){
        ar & id;
        ar & rank;
        ar & type;
        ar & currentRank;
        ar & hashTagID;
        ar & parentID;
        ar & dateCreated;
    }

};

} // End namespace xample_twitter

#endif /* INCLUDE_TWITTER_AGENT_PACKAGE_XAMPLES_H */
