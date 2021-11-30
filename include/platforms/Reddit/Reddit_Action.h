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
/* Reddit_Action.h */
#ifndef INCLUDE_REDDIT_ACTION_H
#define INCLUDE_REDDIT_ACTION_H

#include "platforms/Reddit/Reddit_Definitions.h"
#include "platforms/Reddit/Reddit_Payload.h"
#include "platforms/Reddit/Reddit_Feed.h"

namespace reddit{


class RedditActionExplanation: public ActionExplanation<RedditFeedExplanation>{
protected:
	bool becauseOtherUsersLiked;

public:
	RedditActionExplanation();
	RedditActionExplanation(RedditFeedExplanation feedExp);

	virtual ~RedditActionExplanation();

	std::string getRow();
};

// AbstractRedditAgent
// Reddit_SocialSim_FileOutputTypes
template<typename AGENTTYPE, typename SCORETYPE, typename OUTPUT_TYPE, typename PACKAGE_TYPE>
class AbstractRedditAction: public SocialNetwork_Action<RedditEventType, AGENTTYPE, SocialNetwork_AbstractElement<RedditAgentType, SCORETYPE>, RedditPayload, RedditActionExplanation, OUTPUT_TYPE, PACKAGE_TYPE>{

public:
	AbstractRedditAction(RedditEventType event);
	AbstractRedditAction(RedditEventType event, AGENTTYPE* actingElement);
	AbstractRedditAction(RedditEventType event, AGENTTYPE* actingElement, AGENTTYPE* targetElement);
	virtual ~AbstractRedditAction();

	virtual std::string getOutputRepresentation(OUTPUT_TYPE fileType, bool writeOnlyFiltered = false);

	virtual std::string getDomain();
};

template<typename AGENTTYPE, typename SCORETYPE, typename OUTPUT_TYPE, typename PACKAGE_TYPE>
AbstractRedditAction<AGENTTYPE, SCORETYPE, OUTPUT_TYPE, PACKAGE_TYPE>::AbstractRedditAction(RedditEventType event):
		SocialNetwork_Action<RedditEventType, AGENTTYPE, SocialNetwork_AbstractElement<RedditAgentType, SCORETYPE>, RedditPayload, RedditActionExplanation, OUTPUT_TYPE, PACKAGE_TYPE>(event){}

template<typename AGENTTYPE, typename SCORETYPE, typename OUTPUT_TYPE, typename PACKAGE_TYPE>
AbstractRedditAction<AGENTTYPE, SCORETYPE, OUTPUT_TYPE, PACKAGE_TYPE>::AbstractRedditAction(RedditEventType event, AGENTTYPE* actingElement):
		SocialNetwork_Action<RedditEventType, AGENTTYPE, SocialNetwork_AbstractElement<RedditAgentType, SCORETYPE>, RedditPayload, RedditActionExplanation, OUTPUT_TYPE, PACKAGE_TYPE>(event, actingElement){}

template<typename AGENTTYPE, typename SCORETYPE, typename OUTPUT_TYPE, typename PACKAGE_TYPE>
AbstractRedditAction<AGENTTYPE, SCORETYPE, OUTPUT_TYPE, PACKAGE_TYPE>::AbstractRedditAction(RedditEventType event, AGENTTYPE* actingElement, AGENTTYPE* target):
		SocialNetwork_Action<RedditEventType, AGENTTYPE, SocialNetwork_AbstractElement<RedditAgentType, SCORETYPE>, RedditPayload, RedditActionExplanation, OUTPUT_TYPE, PACKAGE_TYPE>(event, actingElement, target){}

template<typename AGENTTYPE, typename SCORETYPE, typename OUTPUT_TYPE, typename PACKAGE_TYPE>
AbstractRedditAction<AGENTTYPE, SCORETYPE, OUTPUT_TYPE, PACKAGE_TYPE>::~AbstractRedditAction(){}

template<typename AGENTTYPE, typename SCORETYPE, typename OUTPUT_TYPE, typename PACKAGE_TYPE>
std::string AbstractRedditAction<AGENTTYPE, SCORETYPE, OUTPUT_TYPE, PACKAGE_TYPE>::getOutputRepresentation(OUTPUT_TYPE fileType, bool writeOnlyFiltered){
//    // Don't return anything if only writing cascades
//    // Filter actions that do not get written
//    if(!(type == POST || type == COMMENT)) return "";
//    if((writeOnlyFiltered == true) &&
//        ((target == 0) || (target->partOfSeededCascade == false))) return "";
//    std::string t = to_iso_extended_string(currentEventTime);
//    std::replace(t.begin(), t.end(), 'T', ' ');
    std::stringstream s;
//    std::string parentID = "";
//    std::string rootID = "";
//
//    if(type == POST){
//        RedditPostAgent* post = (RedditPostAgent*)target;
//        parentID.assign(post == 0 ? payload.newPostOrCommentID : post->showName());
//        rootID.assign(post == 0 ? payload.newPostOrCommentID : post->showName());
//    }
//    else{
//		RedditComment* comment = (RedditComment*)(newlyCreatedObject);
//		SocialNetwork_AbstractElement<RedditAgentType>* tempPtr = comment->parentPtr;
//		if (tempPtr->getType() == POST_AGENT)
//		{
//			parentID.assign(((RedditAgent*)(comment->parentPtr))->showName());
//		}
//		else
//		{
//			parentID.assign(((RedditComment*)(comment->parentPtr))->nodeID);
//		}
//        rootID.assign(((RedditAgent*)comment->rootPtr)->showName());
//    }
//
//    std::vector<std::string> payloadInfoIDs;
//    //payloadInfoIDs.insert(payloadInfoIDs.begin(), payload.informationIDs.begin(), payload.informationIDs.end());
//	payload.getAllIDs(payloadInfoIDs);
//
//    if(payloadInfoIDs.size() == 0) payloadInfoIDs.push_back("");
//	std::string allInfoIDs = "";
//	for (int i = 0; i < payloadInfoIDs.size(); i++){
//		if (i == 0) allInfoIDs = payloadInfoIDs.at(i);
//		else allInfoIDs += "|" + payloadInfoIDs.at(i);
//	}
//    for(int i = 0; i < payloadInfoIDs.size(); i++){
//		s << "{";
//		s << "\"nodeID\": \"" << payload.newPostOrCommentID                                  << "\", ";
//		s << "\"nodeUserID\": \"" << actor->writeName()                                       << "\", ";
////		s << "\"UCFnodeUserID\": \"" << actor->showName()                                       << "\", ";
//		s << "\"parentID\": \"" << parentID       << "\", ";
//		s << "\"rootID\": \"" << rootID         << "\", ";
//		s << "\"actionType\": \"" << RedditEventNames[type]                                  << "\", ";
//		s << "\"nodeTime\": \"" << t                                                         << "\", ";
//		s << "\"communityID\": \"" << payload.communityID                                    << "\", ";
//		std::string lowerCaseID = payloadInfoIDs.at(i);
//	    std::transform(lowerCaseID.begin(), lowerCaseID.end(), lowerCaseID.begin(), ::tolower);
//        s << "\"informationID\": \"" << lowerCaseID                                          << "\", ";
//		s << "\"platform\": \"" << "reddit"                                                  << "\", ";
//		bool linksExternal = false;
//		s << "\"domain_linked\": [";
//		for(auto url = payload.urlDomains.begin(); url != payload.urlDomains.end(); ++url ) {
//			linksExternal = linksExternal || (*url).find(getDomain());
//			if (url == payload.urlDomains.begin()) {
//				s << "\"" << *url                                           << "\"";
//			}
//			else {
//				s << ", \"" << *url                                         << "\"";
//			}
//		}
//		s << "],";
//
//		s << "\"links_to_external\": " << (linksExternal ? "1" : "0") << ",";
//		s << "\"has_URL\": " << ((payload.urls.size() > 0) ? "1" : "0") << ",";
//		s << "\"all_info_ids\": \""<< allInfoIDs << "\",";
//		s << "\"row_number\": " << i;
//		s << "}"                                                                             << std::endl;
//    }
    return s.str();
}

template<typename AGENTTYPE, typename SCORETYPE, typename OUTPUT_TYPE, typename PACKAGE_TYPE>
std::string AbstractRedditAction<AGENTTYPE, SCORETYPE, OUTPUT_TYPE, PACKAGE_TYPE>::getDomain(){
	return "reddit.com";
}





} // End namespace reddit


#endif /* INCLUDE_REDDIT_ACTION_H */
