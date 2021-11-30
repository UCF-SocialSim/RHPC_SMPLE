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
/* Telegram_Platform.h */
#ifndef INCLUDE_TELEGRAM_PLATFORM_H
#define INCLUDE_TELEGRAM_PLATFORM_H

#include <vector>

#include "utilities/SimpleWeightedSelector.h"

#include "socialnetwork_toolkit/SocialNetwork_Platform.h"
#include "socialnetwork_toolkit/BehaviorSelection.h"

#include "platforms/GitHub/GitHub_Definitions.h"
#include "platforms/GitHub/GitHub_Agent.h"
#include "platforms/GitHub/GitHub_Network.h"

using namespace rhpc_smple;

namespace telegram{

template<typename AGENTTYPE, typename ACTIONTYPE, typename SCORETYPE, typename PACKAGETYPE, typename FILEWRITERTYPE>
class AbstractTelegram:
		public SocialNetwork_Platform<TelegramAgentType,
		                              AGENTTYPE,
									  TelegramEdge<AGENTTYPE, TelegramEdgeInfo>,
									  TelegramEdgeContent<AGENTTYPE, TelegramEdgeInfo>,
									  TelegramEdgeContentManager<AGENTTYPE, TelegramEdgeInfo>,
									  PACKAGETYPE,
									  ACTIONTYPE,
									  TelegramFeedQuery,
									  TelegramFeedExplanation,
									  FILEWRITERTYPE>{
protected:
	TelegramFeed<SCORETYPE>* currentFittest;

public:
	AbstractTelegram(boost::mpi::communicator* world, ScenarioWithAgents<TelegramAgent>* scenario);
	~AbstractTelegram();


	virtual void init_schedule(repast::ScheduleRunner& runner, double offset = 0){}


	std::set<TelegramAgent*>* existingChannels;


	// Agents' views into the platform:
	TelegramAgent* randomlySelectRepo();


//	// Methods that an agent might call
	virtual boost::shared_ptr<TelegramFeed<SCORETYPE>> getFeed(AGENTTYPE* requestor, TelegramFeedQuery query);

	// Actions:
	void doAction(ACTIONTYPE action);
	void api_message(ACTIONTYPE action);

	void propagate(ACTIONTYPE action);

	void addRandomMessagesToFeed(TelegramFeed<SCORETYPE>& feed, int count);

	void getOutlinkSums(TelegramAgent* source, TelegramEdge<AGENTTYPE, TelegramEdgeInfo>& accumulator);
	long getTotalActionCount(TelegramAgent* source);

	void selectFittestAgents(int n, std::vector<TelegramAgent*>& agents, TelegramAgentType type);
	void updateCurrentFittestFeed(int n);
	void getSelector(TelegramAgent* source, SimpleWeightedSelector& sws);
//
//	std::map<std::string, double>& getShocks(){
//		return enclosingScenario->getShocks();
//	}

};


template<typename AGENTTYPE, typename ACTIONTYPE, typename SCORETYPE, typename PACKAGETYPE, typename FILEWRITERTYPE>
AbstractTelegram<AGENTTYPE, ACTIONTYPE, SCORETYPE, PACKAGETYPE, FILEWRITERTYPE>::AbstractTelegram(boost::mpi::communicator* world, ScenarioWithAgents<TelegramAgent>* scenario):
      SocialNetwork_Platform<TelegramAgentType,
		                              AGENTTYPE,
									  TelegramEdge<AGENTTYPE, TelegramEdgeInfo>,
									  TelegramEdgeContent<AGENTTYPE, TelegramEdgeInfo>,
									  TelegramEdgeContentManager<AGENTTYPE, TelegramEdgeInfo>,
									  PACKAGETYPE,
									  ACTIONTYPE,
									  TelegramFeedQuery,
									  TelegramFeedExplanation,
									  FILEWRITERTYPE>(world, "telegram", "TE", 2, scenario){

}

template<typename AGENTTYPE, typename ACTIONTYPE, typename SCORETYPE, typename PACKAGETYPE, typename FILEWRITERTYPE>
AbstractTelegram<AGENTTYPE, ACTIONTYPE, SCORETYPE, PACKAGETYPE, FILEWRITERTYPE>::~AbstractTelegram(){}

template<typename AGENTTYPE, typename ACTIONTYPE, typename SCORETYPE, typename PACKAGETYPE, typename FILEWRITERTYPE>
boost::shared_ptr<TelegramFeed<SCORETYPE>> AbstractTelegram<AGENTTYPE, ACTIONTYPE, SCORETYPE, PACKAGETYPE, FILEWRITERTYPE>::getFeed(AGENTTYPE* requestor, TelegramFeedQuery query){
	boost::shared_ptr<TelegramFeed<SCORETYPE>> ret(new TelegramFeed<SCORETYPE>());
	if(query.type == TelegramFeedQueryType::RETRIEVE_RANDOM_MESSAGES){
		addRandomMessagesToFeed(*ret, query.countOfRandomMessagesToRetrieve);
		return ret;
	}
	return ret;
}


template<typename AGENTTYPE, typename ACTIONTYPE, typename SCORETYPE, typename PACKAGETYPE, typename FILEWRITERTYPE>
void AbstractTelegram<AGENTTYPE, ACTIONTYPE, SCORETYPE, PACKAGETYPE, FILEWRITERTYPE>::addRandomMessagesToFeed(TelegramFeed<SCORETYPE>& feed, int count){
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

//// Actions
template<typename AGENTTYPE, typename ACTIONTYPE, typename SCORETYPE, typename PACKAGETYPE, typename FILEWRITERTYPE>
void AbstractTelegram<AGENTTYPE, ACTIONTYPE, SCORETYPE, PACKAGETYPE, FILEWRITERTYPE>::doAction(ACTIONTYPE action){
	TelegramEventType eventType = action.type;
	//std::cout << " TG DOING ACTION " << eventType << " WITH ACTOR " << action.actor << " AND TARGET " << action.target << std::endl;
	switch (eventType) {;
		case MESSAGE:  api_message(action);   				break;
		default:{
		}
	}
	// Write the record of what was done, and why
	// Write To Event Record
	// Write To Explanatory Record
}

//// N.B. Writing actions is done in the individual function because
//// in theory a function can do more than one thing
template<typename AGENTTYPE, typename ACTIONTYPE, typename SCORETYPE, typename PACKAGETYPE, typename FILEWRITERTYPE>
void AbstractTelegram<AGENTTYPE, ACTIONTYPE, SCORETYPE, PACKAGETYPE, FILEWRITERTYPE>::api_message(ACTIONTYPE action){
//	//std::cout << " TG DOING A TWEET ACTION IN API" << std::endl;
//	TelegramAgentPackage newAgentDefinition;
//	newAgentDefinition.type = CONVERSATION_AGENT;
//	newAgentDefinition.whichCluster = 0;
//	newAgentDefinition.creatorID = action.actor->getId();
//	//TelegramConversationAgent* newTweet = (TelegramConversationAgent*)createAndAddNewAgent(newAgentDefinition);
//	//newTweet->partOfSeededCascade = action.payload.partOfSeededCascade;
//	TelegramConversationAgent* newTweet = (TelegramConversationAgent*)randomlySelectRepo();
//	newTweet->informationIDs.insert(action.payload.informationIDs.begin(), action.payload.informationIDs.end());
//	action.target = newTweet;
//	//if(action.payload.partOfSeededCascade){
//	//	newTweet->setHashTagID(action.payload.newPostOrCommentID);
//	//}
//	//else{
//		action.payload.newPostOrCommentID = newTweet->showName();
//	//}
//
//	if(directPropagation) propagate(action);
//
//	getEdge(action.actor, action.target)->logEvent(MESSAGE);
//	countEvent(MESSAGE);
//	writeAction(action);
}

template<typename AGENTTYPE, typename ACTIONTYPE, typename SCORETYPE, typename PACKAGETYPE, typename FILEWRITERTYPE>
long AbstractTelegram<AGENTTYPE, ACTIONTYPE, SCORETYPE, PACKAGETYPE, FILEWRITERTYPE>::getTotalActionCount(TelegramAgent* source){
	long ret = 0;
//	std::vector<TelegramAgent*> recipients;
//	agentNetwork->adjacent(source, recipients);
//	std::vector<TelegramAgent*>::iterator iter = recipients.begin();
//	while(iter != recipients.end()){
//		boost::shared_ptr<BasicTelegramEdge> edge1 = findEdge(source, (*iter));
//		if(edge1 != 0) ret += edge1->getTotalEvents();
//
//		boost::shared_ptr<BasicTelegramEdge> edge2 = findEdge((*iter), source);
//		if(edge2 != 0) ret += edge2->getTotalEvents();
//		iter++;
//	}
	return ret;
}


template<typename AGENTTYPE, typename ACTIONTYPE, typename SCORETYPE, typename PACKAGETYPE, typename FILEWRITERTYPE>
void AbstractTelegram<AGENTTYPE, ACTIONTYPE, SCORETYPE, PACKAGETYPE, FILEWRITERTYPE>::getSelector(TelegramAgent* source, SimpleWeightedSelector& sws){
//	std::vector<TelegramAgent*> recipients;
//	agentNetwork->successors(source, recipients);
//	std::vector<TelegramAgent*>::iterator iter = recipients.begin();
//	while(iter != recipients.end()){
//		boost::shared_ptr<BasicTelegramEdge> edge = findEdge(source, (*iter));
//		sws.adjust(TWEET,   edge->numTweets());
//		sws.adjust(QUOTE,   edge->numQuotes());
//		sws.adjust(RETWEET, edge->numRetweets());
//		sws.adjust(REPLY,   edge->numReplies());
//		iter++;
//	}
//	sws.clear();
//	for(int i = 0; i < grandTotalEvents->size(); i++){
//		sws.addValue(grandTotalEvents->at(i)); // Simple
//		//sws.addValue((int)( pow(((double)grandTotalEvents->at(i))/1.0),1.0)); // Allows scaling
//	}
}

} // End namespace telegram

#endif /* INCLUDE_TELEGRAM_PLATFORM_H */
