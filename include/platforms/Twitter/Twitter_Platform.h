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
/* Twitter_Platform.h */
#ifndef INCLUDE_TWITTER_PLATFORM_H
#define INCLUDE_TWITTER_PLATFORM_H

#include <vector>

#include "utilities/SimpleWeightedSelector.h"

#include "socialnetwork_toolkit/SocialNetwork_Platform.h"
#include "socialnetwork_toolkit/BehaviorSelection.h"

#include "platforms/Twitter/Twitter_Definitions.h"
#include "platforms/Twitter/Twitter_Agent.h"
#include "platforms/Twitter/Twitter_Agent_Conversation.h"
#include "platforms/Twitter/Twitter_Network.h"
#include "platforms/Twitter/Twitter_Feed_Query.h"

using namespace rhpc_smple;

namespace twitter{


template<typename AGENTTYPE, typename ACTIONTYPE, typename SCORETYPE, typename PACKAGETYPE, typename FILEWRITERTYPE>
class AbstractTwitter:
		public SocialNetwork_Platform<TwitterAgentType,
		                              AGENTTYPE,
									  TwitterEdge<AGENTTYPE, TwitterEdgeInfo>,
									  TwitterEdgeContent<AGENTTYPE, TwitterEdgeInfo>,
									  TwitterEdgeContentManager<AGENTTYPE, TwitterEdgeInfo>,
									  PACKAGETYPE,
									  ACTIONTYPE,
									  TwitterFeedQuery,
									  TwitterFeedExplanation,
									  FILEWRITERTYPE>{
protected:

	TwitterFeed<SCORETYPE>* currentFittest;

	void addRandomTweetsToFeed(TwitterFeed<SCORETYPE>& feed, int count);

public:
	AbstractTwitter(boost::mpi::communicator* world, ScenarioWithAgents<AGENTTYPE>* scenario);
	~AbstractTwitter();

	virtual void init_schedule(repast::ScheduleRunner& runner, double offset = 0){}

//	// Methods that an agent might call
	virtual boost::shared_ptr<TwitterFeed<SCORETYPE>> getFeed(AGENTTYPE* requestor, TwitterFeedQuery query);

	// Actions:
	void doAction(ACTIONTYPE action);

	void api_tweet(ACTIONTYPE action);
	void api_retweet(ACTIONTYPE action);
	void api_reply(ACTIONTYPE action);
	void api_quote(ACTIONTYPE action);

	void propagate(ACTIONTYPE action);


	void getOutlinkSums(AGENTTYPE* source, TwitterEdge<AGENTTYPE, TwitterEdgeInfo>& accumulator);
	long getTotalActionCount(AGENTTYPE* source);

	void selectFittestAgents(int n, std::vector<AGENTTYPE*>& agents, TwitterAgentType type);
	void updateCurrentFittestFeed(int n);
	void getSelector(AGENTTYPE* source, SimpleWeightedSelector& sws);

};


template<typename AGENTTYPE, typename ACTIONTYPE, typename SCORETYPE, typename PACKAGETYPE, typename FILEWRITERTYPE>
AbstractTwitter<AGENTTYPE, ACTIONTYPE, SCORETYPE, PACKAGETYPE, FILEWRITERTYPE>::AbstractTwitter(boost::mpi::communicator* world, ScenarioWithAgents<AGENTTYPE>* scenario):
		SocialNetwork_Platform<TwitterAgentType,
		                              AGENTTYPE,
									  TwitterEdge<AGENTTYPE, TwitterEdgeInfo>,
									  TwitterEdgeContent<AGENTTYPE, TwitterEdgeInfo>,
									  TwitterEdgeContentManager<AGENTTYPE, TwitterEdgeInfo>,
									  PACKAGETYPE,
									  ACTIONTYPE,
									  TwitterFeedQuery,
									  TwitterFeedExplanation,
									  FILEWRITERTYPE>(world, "twitter", "TW", 5, scenario){}


template<typename AGENTTYPE, typename ACTIONTYPE, typename SCORETYPE, typename PACKAGETYPE, typename FILEWRITERTYPE>
AbstractTwitter<AGENTTYPE, ACTIONTYPE, SCORETYPE, PACKAGETYPE, FILEWRITERTYPE>::~AbstractTwitter(){}


template<typename AGENTTYPE, typename ACTIONTYPE, typename SCORETYPE, typename PACKAGETYPE, typename FILEWRITERTYPE>
boost::shared_ptr<TwitterFeed<SCORETYPE>> AbstractTwitter<AGENTTYPE, ACTIONTYPE, SCORETYPE, PACKAGETYPE, FILEWRITERTYPE>::getFeed(AGENTTYPE* requestor, TwitterFeedQuery query){
	boost::shared_ptr<TwitterFeed<SCORETYPE>> ret(new TwitterFeed<SCORETYPE>());
	if(query.type == TwitterFeedQueryType::RETRIEVE_RANDOM_TWEETS){
		addRandomTweetsToFeed(*ret, query.countOfRandomTweetsToRetrieve);
	}
	return ret;
}

template<typename AGENTTYPE, typename ACTIONTYPE, typename SCORETYPE, typename PACKAGETYPE, typename FILEWRITERTYPE>
void AbstractTwitter<AGENTTYPE, ACTIONTYPE, SCORETYPE, PACKAGETYPE, FILEWRITERTYPE>::addRandomTweetsToFeed(TwitterFeed<SCORETYPE>& feed, int count){
//	std::vector<AGENTTYPE*> repos;
//	context.selectAgents(count, repos, VIDEO_AGENT);
//	std::vector<YouTubeAgent*>::iterator iter = repos.begin();
//	while(iter != repos.end()){
//		YouTubeFeedExplanation feedExplanation;
//		feedExplanation.randomlySelected = true;
//		feed.push(*iter, 0, 0, IDLE, feedExplanation);
//		iter++;
//	}
}


// Actions
// Actions have explanations attached...
template<typename AGENTTYPE, typename ACTIONTYPE, typename SCORETYPE, typename PACKAGETYPE, typename FILEWRITERTYPE>
void AbstractTwitter<AGENTTYPE, ACTIONTYPE, SCORETYPE, PACKAGETYPE, FILEWRITERTYPE>::doAction(ACTIONTYPE action){
	TwitterEventType eventType = action.type;
	switch (eventType) {;
		case TWEET:       			    api_tweet(action);   				break;
		case RETWEET:                 	api_retweet(action);				break;
		case REPLY:						api_reply(action);					break;
		case QUOTE:						api_quote(action);					break;
		default:{
		}
	}
}


// N.B. Writing actions is done in the individual function because
// in theory a function can do more than one thing
template<typename AGENTTYPE, typename ACTIONTYPE, typename SCORETYPE, typename PACKAGETYPE, typename FILEWRITERTYPE>
void AbstractTwitter<AGENTTYPE, ACTIONTYPE, SCORETYPE, PACKAGETYPE, FILEWRITERTYPE>::api_tweet(ACTIONTYPE action){

	// Create the new agent
    AGENTTYPE* newTweet = this->createAndAddNewAgent(action.getNewAgentPackage());

    // Amend the action with information from its result
	action.target                     = newTweet;
	action.payload.newPostOrCommentID = newTweet->showName();

	// Log the results
	this->getEdge(action.actor, action.target)->logEvent(TWEET);
	this->countEvent(TWEET);
	this->writeAction(action);
}

template<typename AGENTTYPE, typename ACTIONTYPE, typename SCORETYPE, typename PACKAGETYPE, typename FILEWRITERTYPE>
void AbstractTwitter<AGENTTYPE, ACTIONTYPE, SCORETYPE, PACKAGETYPE, FILEWRITERTYPE>::api_retweet(ACTIONTYPE action){
	action.target->receiveAction(action);
	this->getEdge(action.actor, action.target)->logEvent(RETWEET);
	this->countEvent(RETWEET);
	this->writeAction(action);
}

template<typename AGENTTYPE, typename ACTIONTYPE, typename SCORETYPE, typename PACKAGETYPE, typename FILEWRITERTYPE>
void AbstractTwitter<AGENTTYPE, ACTIONTYPE, SCORETYPE, PACKAGETYPE, FILEWRITERTYPE>::api_reply(ACTIONTYPE action){
	action.target->receiveAction(action);

	this->getEdge(action.actor, action.target)->logEvent(REPLY);
	this->countEvent(REPLY);
	this->writeAction(action);
}

template<typename AGENTTYPE, typename ACTIONTYPE, typename SCORETYPE, typename PACKAGETYPE, typename FILEWRITERTYPE>
void AbstractTwitter<AGENTTYPE, ACTIONTYPE, SCORETYPE, PACKAGETYPE, FILEWRITERTYPE>::api_quote(ACTIONTYPE action){
	action.target->receiveAction(action);

	this->getEdge(action.actor, action.target)->logEvent(QUOTE);
	this->countEvent(QUOTE);
	this->writeAction(action);
}


} // End namespace twitter


#endif /* INCLUDE_TWITTER_PLATFORM_H */
