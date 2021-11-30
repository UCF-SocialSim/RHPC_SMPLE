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
/* Telegram_Action.h */
#ifndef INCLUDE_TELEGRAM_ACTION_H
#define INCLUDE_TELEGRAM_ACTION_H

#include <vector>

#include "socialnetwork_toolkit/Action.h"

#include "platforms/Telegram/Telegram_Definitions.h"
#include "platforms/Telegram/Telegram_Payload.h"
#include "platforms/Telegram/Telegram_Feed.h"

using namespace rhpc_smple;

namespace telegram{

class TelegramActionExplanation: public ActionExplanation<TelegramFeedExplanation>{
protected:
	bool becauseOtherUsersLiked;

	std::vector<std::string> headings;

public:
	TelegramActionExplanation();
	TelegramActionExplanation(TelegramFeedExplanation feedExp);

	virtual ~TelegramActionExplanation();

	std::string getRow();
};

template<typename AGENTTYPE, typename SCORETYPE, typename OUTPUT_TYPE, typename PACKAGE_TYPE>
class AbstractTelegramAction: public SocialNetwork_Action<TelegramEventType, AGENTTYPE, SocialNetwork_AbstractElement<TelegramAgentType, SCORETYPE>, TelegramPayload, TelegramActionExplanation, OUTPUT_TYPE, PACKAGE_TYPE>{

public:
	//TelegramResponse* responsePtr;

	AbstractTelegramAction(TelegramEventType event);
	AbstractTelegramAction(TelegramEventType event, AGENTTYPE* actingElement);
	AbstractTelegramAction(TelegramEventType event, AGENTTYPE* actingElement, AGENTTYPE* targetElement);
	virtual ~AbstractTelegramAction();

	virtual std::string getOutputRepresentation(OUTPUT_TYPE fileType, bool writeOnlyFiltered = false);

	virtual std::string getDomain();

};

template<typename AGENTTYPE, typename SCORETYPE, typename OUTPUT_TYPE, typename PACKAGE_TYPE>
AbstractTelegramAction<AGENTTYPE, SCORETYPE, OUTPUT_TYPE, PACKAGE_TYPE>::AbstractTelegramAction(TelegramEventType event):
		SocialNetwork_Action<TelegramEventType, AGENTTYPE, SocialNetwork_AbstractElement<TelegramAgentType, SCORETYPE>, TelegramPayload, TelegramActionExplanation, OUTPUT_TYPE, PACKAGE_TYPE>(event){}

template<typename AGENTTYPE, typename SCORETYPE, typename OUTPUT_TYPE, typename PACKAGE_TYPE>
AbstractTelegramAction<AGENTTYPE, SCORETYPE, OUTPUT_TYPE, PACKAGE_TYPE>::AbstractTelegramAction(TelegramEventType event, AGENTTYPE* actingElement):
		SocialNetwork_Action<TelegramEventType, AGENTTYPE, SocialNetwork_AbstractElement<TelegramAgentType, SCORETYPE>, TelegramPayload, TelegramActionExplanation, OUTPUT_TYPE, PACKAGE_TYPE>(event, actingElement){}

template<typename AGENTTYPE, typename SCORETYPE, typename OUTPUT_TYPE, typename PACKAGE_TYPE>
AbstractTelegramAction<AGENTTYPE, SCORETYPE, OUTPUT_TYPE, PACKAGE_TYPE>::AbstractTelegramAction(TelegramEventType event, AGENTTYPE* actingElement, AGENTTYPE* target):
		SocialNetwork_Action<TelegramEventType, AGENTTYPE, SocialNetwork_AbstractElement<TelegramAgentType, SCORETYPE>, TelegramPayload, TelegramActionExplanation, OUTPUT_TYPE, PACKAGE_TYPE>(event, actingElement, target){}

template<typename AGENTTYPE, typename SCORETYPE, typename OUTPUT_TYPE, typename PACKAGE_TYPE>
AbstractTelegramAction<AGENTTYPE, SCORETYPE, OUTPUT_TYPE, PACKAGE_TYPE>::~AbstractTelegramAction(){}

template<typename AGENTTYPE, typename SCORETYPE, typename OUTPUT_TYPE, typename PACKAGE_TYPE>
std::string AbstractTelegramAction<AGENTTYPE, SCORETYPE, OUTPUT_TYPE, PACKAGE_TYPE>::getOutputRepresentation(OUTPUT_TYPE fileType, bool writeOnlyFiltered){
    // Don't return anything if only writing cascades
    // Filter actions that do not get written
//    if(type == IDLE) return "";
//    if((writeOnlyFiltered == true) &&
//        ((target == 0) || (target->partOfSeededCascade == false))) return "";
//    std::string t = to_iso_extended_string(currentEventTime);
//    std::replace(t.begin(), t.end(), 'T', ' ');
    std::stringstream s;
//    std::string parentID = "";
//    std::string rootID = "";
//
//    if(type == MESSAGE){
//        TelegramConversationAgent* tweet = (TelegramConversationAgent*)target;
//        parentID.assign(tweet == 0 ? payload.newPostOrCommentID : tweet->showName());
//        rootID.assign(tweet == 0 ? payload.newPostOrCommentID : tweet->showName());
//    }
//    else{
//        TelegramResponse* comment = payload.commentPtr;
//        parentID.assign(((TelegramAgent*)comment->parentPtr)->showName());
//        rootID.assign(((TelegramAgent*)comment->rootPtr)->showName());
//    }
//
//    TelegramConversationAgent* tweet = (TelegramConversationAgent*)target;
//
//    std::vector<std::string> payloadInfoIDs;
//    //payloadInfoIDs.insert(payloadInfoIDs.begin(), payload.informationIDs.begin(), payload.informationIDs.end());
//    payload.getIDs(payloadInfoIDs);
//    if(payload.informationIDs.size() == 0) payloadInfoIDs.push_back("");
//	std::string allInfoIDs = "";
//	for (int i = 0; i < payloadInfoIDs.size(); i++){
//		if (i == 0) allInfoIDs = payloadInfoIDs.at(i);
//		else allInfoIDs += "|" + payloadInfoIDs.at(i);
//	}
//    for(int i = 0; i < payloadInfoIDs.size(); i++){
//
//		s << "{";
//		s << "\"nodeID\": \"" << payload.newPostOrCommentID                                  << "\", ";
//		s << "\"nodeUserID\": \"" << actor->writeName()                                       << "\", ";
////		s << "\"UCFnodeUserID\": \"" << actor->showName()                                       << "\", ";
//		s << "\"parentID\": \"" << parentID                                                  << "\", ";
//		s << "\"rootID\": \"" << rootID                                                      << "\", ";
//		s << "\"actionType\": \"" << TelegramEventNames[type]                                << "\", ";
//		s << "\"nodeTime\": \"" << t                                                         << "\", ";
//		s << "\"communityID\": \"" << payload.communityID                                    << "\", ";
//        std::string lowerCaseID = payloadInfoIDs.at(i);
//	    std::transform(lowerCaseID.begin(), lowerCaseID.end(), lowerCaseID.begin(), ::tolower);
//		s << "\"informationID\": \"" << lowerCaseID                                          << "\", ";
//        s << "\"platform\": \"" << "telegram"                                                << "\", ";
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
//		s << "\"has_url\": " << ((payload.urls.size() > 0) ? "1" : "0") << ",";
//		s << "\"all_info_ids\": \""<< allInfoIDs << "\",";
//		s << "\"row_number\": " << i;
//		s << "}"                                                                             << std::endl;
//    }

    return s.str();
}

template<typename AGENTTYPE, typename SCORETYPE, typename OUTPUT_TYPE, typename PACKAGE_TYPE>
std::string AbstractTelegramAction<AGENTTYPE, SCORETYPE, OUTPUT_TYPE, PACKAGE_TYPE>::getDomain(){
	return "telegram.com";
}



} // End namespace telegram

#endif /* INCLUDE_TELEGRAM_ACTION_H */
