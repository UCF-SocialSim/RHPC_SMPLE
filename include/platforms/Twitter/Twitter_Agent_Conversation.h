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
/* Twitter_Agent_Conversation.h */
#ifndef INCLUDE_TWITTER_AGENT_CONVERSATION_H
#define INCLUDE_TWITTER_AGENT_CONVERSATION_H

#include <vector>
#include <set>

#include "socialnetwork_toolkit/InfoID.h"
#include "platforms/Twitter/Twitter_Agent.h"

using namespace rhpc_smple;

namespace twitter{

template<typename SCORETYPE>
class AbstractTwitterConversationAgent{

public:
	int countOfDirectComments; // Count of direct comments, not full tree (which will be size of vector)
    std::vector<TwitterResponse<SCORETYPE>*> responses;
    std::set<InfoID*>             informationIDs;

    AbstractTwitterConversationAgent();
    virtual ~AbstractTwitterConversationAgent();

	TwitterResponse<SCORETYPE>* addResponse(SocialNetwork_AbstractElement<TwitterAgentType, SCORETYPE>* creatorPtr, bool store = true);

	// Add comment with one of the existing comments as parent
	TwitterResponse<SCORETYPE>* addResponse(SocialNetwork_AbstractElement<TwitterAgentType, SCORETYPE>* creatorPtr, TwitterResponse<SCORETYPE>* parentPtr);

	int getCountOfDirectComments(){
    	  return countOfDirectComments;
    }

    void incrementCountOfDirectComments(){
    	  countOfDirectComments++;
    }


    virtual std::string getResponseNameRoot();

    virtual SocialNetwork_AbstractElement<TwitterAgentType, SCORETYPE>* getSelfPointer();

};


template<typename SCORETYPE>
AbstractTwitterConversationAgent<SCORETYPE>::AbstractTwitterConversationAgent():
	countOfDirectComments(0){}

template<typename SCORETYPE>
AbstractTwitterConversationAgent<SCORETYPE>::~AbstractTwitterConversationAgent(){
	for(int i = 0; i < responses.size(); i++) delete responses[i];
}

template<typename SCORETYPE>
TwitterResponse<SCORETYPE>* AbstractTwitterConversationAgent<SCORETYPE>::addResponse(SocialNetwork_AbstractElement<TwitterAgentType, SCORETYPE>* creatorPtr, bool store){
	stringstream s;
	s << getResponseNameRoot() << "_" << (countOfDirectComments + 1);
	TwitterResponse<SCORETYPE>* toAdd = new TwitterResponse<SCORETYPE>(s.str(), creatorPtr, getSelfPointer(), getSelfPointer(), false);
	if(store){
		responses.push_back(toAdd);
		toAdd->indx = responses.size(); // Note: 1- indexed, not 0-indexed
	}
	else{
		toAdd->indx = -1;
	}
	return toAdd;

}

// Add comment with one of the existing comments as parent
template<typename SCORETYPE>
TwitterResponse<SCORETYPE>* AbstractTwitterConversationAgent<SCORETYPE>::addResponse(SocialNetwork_AbstractElement<TwitterAgentType, SCORETYPE>* creatorPtr, TwitterResponse<SCORETYPE>* parentPtr){
	stringstream s;
	int responseCount = parentPtr->countOfDirectComments + 1;
	s << parentPtr->nodeID << "_" << responseCount;
	TwitterResponse<SCORETYPE>* toAdd = new TwitterResponse<SCORETYPE>(s.str(), creatorPtr, parentPtr, parentPtr->rootPtr, false);
	responses.push_back(toAdd);
	toAdd->indx = responses.size(); // Note: 1- indexed, not 0-indexed
	return toAdd;
}

template<typename SCORETYPE>
std::string AbstractTwitterConversationAgent<SCORETYPE>::getResponseNameRoot(){
	stringstream s;
	s << "TWEET" << (countOfDirectComments + 1);
	return s.str();
}

template<typename SCORETYPE>
SocialNetwork_AbstractElement<TwitterAgentType, SCORETYPE>* AbstractTwitterConversationAgent<SCORETYPE>::getSelfPointer(){
	return 0;
}

} // End namespace twitter

#endif /* INCLUDE_TWITTER_AGENT_CONVERSATION_H */
