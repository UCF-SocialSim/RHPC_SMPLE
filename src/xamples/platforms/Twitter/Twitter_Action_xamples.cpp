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
/* Twitter_Action_xamples.cpp */
#include "xamples/platforms/Twitter/Twitter_Action_xamples.h"

#include "xamples/platforms/Twitter/Twitter_Agent_xamples.h"
#include "xamples/platforms/Twitter/Twitter_Agent_Conversation_xamples.h"
#include "xamples/platforms/Twitter/Twitter_Agent_User_xamples.h"

using namespace twitter;

const char* TwitterEventNames[] = {
	"idle",
	"tweet",
	"retweet",
	"quote",
	"reply"
};

extern ptime currentEventTime;

TwitterAction::TwitterAction(TwitterEventType event):
		AbstractTwitterAction<TwitterAgent, Twitter_Xamples_ScoreTypes, Twitter_Xamples_FileOutputTypes, TwitterAgentPackage>(event){}

TwitterAction::TwitterAction(TwitterEventType event, TwitterAgent* actingElement):
		AbstractTwitterAction<TwitterAgent, Twitter_Xamples_ScoreTypes, Twitter_Xamples_FileOutputTypes, TwitterAgentPackage>(event, actingElement){}

TwitterAction::TwitterAction(TwitterEventType event, TwitterAgent* actingElement, TwitterAgent* targetElement):
		AbstractTwitterAction<TwitterAgent, Twitter_Xamples_ScoreTypes, Twitter_Xamples_FileOutputTypes, TwitterAgentPackage>(event, actingElement, targetElement){}

TwitterAction::~TwitterAction(){}

std::string TwitterAction::getOutputRepresentation(Twitter_Xamples_FileOutputTypes type, bool writeOnlyFiltered ){
	if((this->type == IDLE) || ((writeOnlyFiltered == true) && (target == 0))) return "";
	std::string t = to_iso_extended_string(currentEventTime);
	std::replace(t.begin(), t.end(), 'T', ' ');

	std::vector<std::string> payloadInfoIDs;
	payload.getIDs(payloadInfoIDs);
	if(payloadInfoIDs.size() == 0) payloadInfoIDs.push_back("");
	std::string allInfoIDs = "";
	for (int i = 0; i < payloadInfoIDs.size(); i++) allInfoIDs += (i == 0 ? "" : "|") + payloadInfoIDs.at(i);
	std::stringstream s;
	switch(type){
		case TWITTER_XAMPLES_FILEOUTPUT_JSON:{
			s << "{"
			  << "\"nodeUserID\": \""     << actor->writeName()               << "\", "
			  << "\"nodeID\": \""         << payload.newPostOrCommentID       << "\", "
			  << "\"actionType\": \""     << TwitterEventNames[this->type]    << "\", "
			  << "\"nodeTime\": \""       << t                                << "\", "
			  << "\"all_info_ids\": \""   << allInfoIDs                       << "\""
			  << "}" << std::endl;
		}
		case TWITTER_XAMPLES_FILEOUTPUT_CSV:{
			s << actor->writeName()               << ","
			  << payload.newPostOrCommentID       << ","
			  << TwitterEventNames[this->type]    << ","
			  << t                                << ","
			  << allInfoIDs                       << ""
			  << std::endl;
		}
		case TWITTER_XAMPLES_FILEOUTPUTTYPES_META:{}
	}
	return s.str();
}

TwitterAgentPackage TwitterAction::getNewAgentPackage(int agentOrder){
	TwitterAgentPackage newAgentDefinition;
	newAgentDefinition.type = CONVERSATION_AGENT;
	newAgentDefinition.creatorID = this->actor->getId();
	newAgentDefinition.informationIDs.insert(payload.informationIDs.begin(), payload.informationIDs.end());
	return newAgentDefinition;
}



