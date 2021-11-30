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
/* Twitter_Action.h */
#ifndef INCLUDE_TWITTER_ACTION_H
#define INCLUDE_TWITTER_ACTION_H

#include <vector>

#include "socialnetwork_toolkit/SocialNetwork_Platform.h"

#include "platforms/Twitter/Twitter_Definitions.h"
#include "platforms/Twitter/Twitter_Payload.h"
#include "platforms/Twitter/Twitter_Feed.h"

using namespace rhpc_smple;

namespace twitter{


class TwitterActionExplanation: public ActionExplanation<TwitterFeedExplanation>{
protected:
	bool                     becauseOtherUsersLiked;
	std::vector<std::string> headings;

public:
	TwitterActionExplanation();
	TwitterActionExplanation(TwitterFeedExplanation feedExp);

	virtual ~TwitterActionExplanation();

	std::string getRow();
};


template<typename AGENTTYPE, typename SCORETYPE, typename OUTPUT_TYPE, typename PACKAGE_TYPE>
class AbstractTwitterAction: public SocialNetwork_Action<TwitterEventType, AGENTTYPE, SocialNetwork_AbstractElement<TwitterAgentType, SCORETYPE>, TwitterPayload, TwitterActionExplanation, OUTPUT_TYPE, PACKAGE_TYPE>{

public:

	AbstractTwitterAction(TwitterEventType event);
	AbstractTwitterAction(TwitterEventType event, AGENTTYPE* actingElement);
	AbstractTwitterAction(TwitterEventType event, AGENTTYPE* actingElement, AGENTTYPE* targetElement);
	virtual ~AbstractTwitterAction();

	virtual std::string getOutputRepresentation(OUTPUT_TYPE fileType, bool writeOnlyFiltered = false);

	virtual std::string getDomain();

};


template<typename AGENTTYPE, typename SCORETYPE, typename OUTPUT_TYPE, typename PACKAGE_TYPE>
AbstractTwitterAction<AGENTTYPE, SCORETYPE, OUTPUT_TYPE, PACKAGE_TYPE>::AbstractTwitterAction(TwitterEventType event):
		SocialNetwork_Action<TwitterEventType, AGENTTYPE, SocialNetwork_AbstractElement<TwitterAgentType, SCORETYPE>, TwitterPayload, TwitterActionExplanation, OUTPUT_TYPE, PACKAGE_TYPE>(event){}

template<typename AGENTTYPE, typename SCORETYPE, typename OUTPUT_TYPE, typename PACKAGE_TYPE>
AbstractTwitterAction<AGENTTYPE, SCORETYPE, OUTPUT_TYPE, PACKAGE_TYPE>::AbstractTwitterAction(TwitterEventType event, AGENTTYPE* actingElement):
		SocialNetwork_Action<TwitterEventType, AGENTTYPE, SocialNetwork_AbstractElement<TwitterAgentType, SCORETYPE>, TwitterPayload, TwitterActionExplanation, OUTPUT_TYPE, PACKAGE_TYPE>(event, actingElement){}

template<typename AGENTTYPE, typename SCORETYPE, typename OUTPUT_TYPE, typename PACKAGE_TYPE>
AbstractTwitterAction<AGENTTYPE, SCORETYPE, OUTPUT_TYPE, PACKAGE_TYPE>::AbstractTwitterAction(TwitterEventType event, AGENTTYPE* actingElement, AGENTTYPE* target):
		SocialNetwork_Action<TwitterEventType, AGENTTYPE, SocialNetwork_AbstractElement<TwitterAgentType, SCORETYPE>, TwitterPayload, TwitterActionExplanation, OUTPUT_TYPE, PACKAGE_TYPE>(event, actingElement, target){}

template<typename AGENTTYPE, typename SCORETYPE, typename OUTPUT_TYPE, typename PACKAGE_TYPE>
AbstractTwitterAction<AGENTTYPE, SCORETYPE, OUTPUT_TYPE, PACKAGE_TYPE>::~AbstractTwitterAction(){}

template<typename AGENTTYPE, typename SCORETYPE, typename OUTPUT_TYPE, typename PACKAGE_TYPE>
std::string AbstractTwitterAction<AGENTTYPE, SCORETYPE, OUTPUT_TYPE, PACKAGE_TYPE>::getOutputRepresentation(OUTPUT_TYPE fileType, bool writeOnlyFiltered){
    std::stringstream s;
    return s.str();
}

template<typename AGENTTYPE, typename SCORETYPE, typename OUTPUT_TYPE, typename PACKAGE_TYPE>
std::string AbstractTwitterAction<AGENTTYPE, SCORETYPE, OUTPUT_TYPE, PACKAGE_TYPE>::getDomain(){
	return "twitter.com";
}


} // End namespace twitter

#endif /* INCLUDE_TWITTER_ACTION_H */
