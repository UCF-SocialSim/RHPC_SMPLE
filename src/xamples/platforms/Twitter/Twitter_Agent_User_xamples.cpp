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
/* Twitter_Agent_User_xamples.cpp */
#include "xamples/platforms/Twitter/Twitter_Agent_User_xamples.h"

#include <assert.h>
#include <cmath>
#include "sstream"

#include "utilities/Utilities.h"

#include "xamples/platforms/Twitter/Twitter_Platform_xamples.h"

using namespace boost::posix_time;
using namespace twitter;

typedef FeedElement<TwitterAgentType, TwitterEventType, TwitterFeedExplanation> TwitterFeedElement;

extern ptime currentEventTime;         // A handle for the global simulation time

TwitterUserAgent::TwitterUserAgent(repast::AgentId id, ptime dateCreated, TwitterAgentPackage& package, SNP* platform):
	TwitterAgent(id, USER_AGENT, dateCreated, package, platform),
	infoIDSelector(0){
	twitter = (Twitter*)containingPlatform; // Downcast the superclass
}

TwitterUserAgent::~TwitterUserAgent(){

}


// Initializers

void TwitterUserAgent::doSelectBehavior(){
	actions.clear();
	selectBehavior();
}

void TwitterUserAgent::selectBehavior(){
    behaviorSelector->getBehavior(this, actions);
}

void TwitterUserAgent::doNextAction(){
	doPendingAction(actions.begin());
}

void TwitterUserAgent::doAllActions(){
	while(actions.size() > 0) doNextAction();
}

void TwitterUserAgent::doPendingAction(std::vector<TwitterAction>::iterator iter){
	if(iter >= actions.begin() && iter < actions.end()){
		doAction(*iter);
		actions.erase(iter);
	}
}

void TwitterUserAgent::doAction(TwitterAction& action){
	twitter->doAction(action);
}

std::string TwitterUserAgent::showName(){
	return hashTagID;
}

std::string TwitterUserAgent::writeName(){
	return canonicalName;
}

void TwitterUserAgent::selectTarget(TwitterAction& action){
	LOG_INFO("twitter", "TS 1");
	TwitterFeedQuery_xamples fq(TwitterFeedQueryType_xamples::TW_BASIC);
	LOG_INFO("twitter", "TS 2");
	fq.countOfRandomTweetsToRetrieve = 1;
	LOG_INFO("twitter", "TS 3");
	boost::shared_ptr<TwitterFeed<Twitter_Xamples_ScoreTypes>> f = twitter->getFeed(this, fq);
	LOG_INFO("twitter", "TS 4");
	action.target = (TwitterAgent*)(f->newest().target); // Always chooses the newest
	LOG_INFO("twitter", "Retrieved target " + std::to_string((long)(action.target)));
//	TwitterConversationAgent* tempTarget = (TwitterConversationAgent*)(action.target);

//	double probOfChoosingSubtarget = ((tempTarget->responses.size() > 0) && (action.type != TwitterEventType::RETWEET) ? 0.25 : 0.00);
//	if(repast::Random::instance()->nextDouble() < probOfChoosingSubtarget){
//		int indx = repast::Random::instance()->nextDouble() * tempTarget->responses.size();
//		action.subtarget = tempTarget->responses.at(indx);
//	}
}

void TwitterUserAgent::selectInfoIDs(SN_ActionPayload& payload){
	infoIDSelector->addInfoIDs(this, *((TwitterPayload*)(&payload)));
}
