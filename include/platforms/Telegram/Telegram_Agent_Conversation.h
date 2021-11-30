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
/* Telegram_Agent_Conversation.h */
#ifndef TELEGRAM_CONVERSATION_AGENT_H
#define TELEGRAM_CONVERSATION_AGENT_H

#include "socialnetwork_toolkit/InfoID.h"
#include "platforms/Telegram/Telegram_Agent.h"

using namespace rhpc_smple;

namespace telegram{

/* Agents */
template<typename SCORETYPE>
class AbstractTelegramConversationAgent{
	int countOfDirectComments; // Count of direct comments, not full tree (which will be size of vector)

public:
    std::vector<TelegramResponse<SCORETYPE>*> responses;

    std::string                    channelID;
    std::set<InfoID*>              informationIDs;

    AbstractTelegramConversationAgent(std::string channel);

    virtual ~AbstractTelegramConversationAgent();


    // Add comment with post itself as parent
	TelegramResponse<SCORETYPE>* addResponse(SocialNetwork_AbstractElement<TelegramAgentType, SCORETYPE>* creatorPtr);

	// Add comment with one of the existing comments as parent
	TelegramResponse<SCORETYPE>* addResponse(SocialNetwork_AbstractElement<TelegramAgentType, SCORETYPE>* creatorPtr, TelegramResponse<SCORETYPE>* parentPtr);

	int getCountOfDirectComments(){
    	  return countOfDirectComments;
    }

    void incrementCountOfDirectComments(){
    	  countOfDirectComments++;
    }

};


template<typename SCORETYPE>
AbstractTelegramConversationAgent<SCORETYPE>::AbstractTelegramConversationAgent(std::string channel):
    channelID(channel){
}

template<typename SCORETYPE>
AbstractTelegramConversationAgent<SCORETYPE>::~AbstractTelegramConversationAgent(){
	for(int i = 0; i < responses.size(); i++) delete responses[i];
}


template<typename SCORETYPE>
TelegramResponse<SCORETYPE>* AbstractTelegramConversationAgent<SCORETYPE>::addResponse(SocialNetwork_AbstractElement<TelegramAgentType, SCORETYPE>* creatorPtr){
//	std::stringstream s;
//	s << showName() << "_" << (countOfDirectComments + 1);
//	TelegramResponse* toAdd = new TelegramResponse(s.str(), creatorPtr, this, this, false);
//	responses.push_back(toAdd);
//	return toAdd;
	return 0;
}

template<typename SCORETYPE>
TelegramResponse<SCORETYPE>* AbstractTelegramConversationAgent<SCORETYPE>::addResponse(SocialNetwork_AbstractElement<TelegramAgentType, SCORETYPE>* creatorPtr, TelegramResponse<SCORETYPE>* parentPtr){
//	std::stringstream s;
//	int responseCount = parentPtr->countOfDirectComments + 1;
//	s << parentPtr->nodeID << "_" << responseCount;
//	TelegramResponse* toAdd = new TelegramResponse(s.str(), creatorPtr, parentPtr->rootPtr, this, false);
//	responses.push_back(toAdd);
//	toAdd->indx = responses.size(); // Note: 1- indexed, not 0-indexed
//	return toAdd;
	return 0;

}

} // End namespace telegram

#endif
