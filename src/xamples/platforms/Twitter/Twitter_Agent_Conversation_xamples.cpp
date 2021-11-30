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
/* Twitter_Agent_Conversation_xamples.cpp */
#include "xamples/platforms/Twitter/Twitter_Agent_Conversation_xamples.h"

using namespace twitter;

//int TwitterConversationAgent::incrementResponseCount(std::string parentCommentID){
//	int ret = 0;
//	std::vector<TwitterResponse*>::iterator iter = responses.begin();
//	while(iter != responses.end()){
//		if((*iter)->parentID == parentCommentID){
//			ret = (*iter)->countOfComments;
//			(*iter)->countOfComments = ret + 1;
//			break;
//		}
//		iter++;
//	}
//	return ret;
//}

TwitterConversationAgent::TwitterConversationAgent(repast::AgentId id, ptime dateCreated, TwitterAgentPackage& package, SNP* platform, SocialNetwork_AbstractElement<TwitterAgentType, Twitter_Xamples_ScoreTypes>* creator):
    TwitterAgent(id, CONVERSATION_AGENT, dateCreated, package, platform, creator),
	AbstractTwitterConversationAgent<Twitter_Xamples_ScoreTypes>(){

	stringstream s;
	s << "TW_" << showName();
	setHashTagID(s.str());
}

TwitterConversationAgent::~TwitterConversationAgent(){ }

std::string TwitterConversationAgent::showName(){
	return hashTagID;
}

std::string TwitterConversationAgent::writeName(){
	return canonicalName;
}

void TwitterConversationAgent::receiveAction(TwitterAction& action){
	LOG_INFO("twitter", "receiving: 1");

	if(action.type == RETWEET || action.type == REPLY || action.type == QUOTE){
		LOG_INFO("twitter", "receiving: 2");
     	// When receiving a retweet, need to update the information IDs.
        informationIDs.insert(action.payload.informationIDs.begin(), action.payload.informationIDs.end());
        LOG_INFO("twitter", "receiving: 3");

        int indx = (action.subtarget == 0 ? 0 : ((TwitterResponse<Twitter_Xamples_ScoreTypes>*)(action.subtarget))->indx);
        LOG_INFO("twitter", "receiving: 4");

        TwitterResponse<Twitter_Xamples_ScoreTypes>* newResponse = 0;
        LOG_INFO("twitter", "receiving: 5");

        if(indx == 0){
        	LOG_INFO("twitter", "receiving: 6");

			newResponse = addResponse(action.actor);
		}
		else{
			LOG_INFO("twitter", "receiving: 7 " + std::to_string(indx) );

			indx--;
			LOG_INFO("twitter", "receiving: 8 " + std::to_string(indx) );

			TwitterResponse<Twitter_Xamples_ScoreTypes>* parentComment = responses.at(indx);
			LOG_INFO("twitter", "receiving: 9");

			newResponse = addResponse(action.actor, parentComment);
			LOG_INFO("twitter", "receiving: 10");

		}
        LOG_INFO("twitter", "receiving: 11");

        action.payload.newPostOrCommentID = newResponse->nodeID;
        LOG_INFO("twitter", "receiving: 12");

		action.newlyCreatedObject = newResponse;
		LOG_INFO("twitter", "receiving: 13");

	}
}

std::string TwitterConversationAgent::getResponseNameRoot(){
	return showName();
}

SocialNetwork_AbstractElement<TwitterAgentType, Twitter_Xamples_ScoreTypes>* TwitterConversationAgent::getSelfPointer(){
	return this;
}


