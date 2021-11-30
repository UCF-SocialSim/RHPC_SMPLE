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
/* YouTube_Platform.h */
#ifndef INCLUDE_YOUTUBE_PLATFORM_H
#define INCLUDE_YOUTUBE_PLATFORM_H

#include <vector>

#include "utilities/SimpleWeightedSelector.h"

#include "socialnetwork_toolkit/SocialNetwork_Platform.h"
#include "socialnetwork_toolkit/BehaviorSelection.h"

#include "platforms/YouTube/YouTube_Definitions.h"
#include "platforms/YouTube/YouTube_Agent.h"
#include "platforms/YouTube/YouTube_Network.h"

using namespace rhpc_smple;

namespace youtube{

template<typename AGENTTYPE, typename ACTIONTYPE, typename SCORETYPE, typename PACKAGETYPE, typename FILEWRITERTYPE>
class AbstractYouTube:
		public SocialNetwork_Platform<YouTubeAgentType,
			AGENTTYPE,
			YouTubeEdge<AGENTTYPE, YouTubeEdgeInfo>,
			YouTubeEdgeContent<AGENTTYPE, YouTubeEdgeInfo>,
			YouTubeEdgeContentManager<AGENTTYPE, YouTubeEdgeInfo>,
			PACKAGETYPE,
			ACTIONTYPE,
			YouTubeFeedQuery,
			YouTubeFeedExplanation,
			FILEWRITERTYPE>{

protected:
	YouTubeFeed<SCORETYPE>* currentFittest;

public:
	AbstractYouTube(boost::mpi::communicator* world, ScenarioWithAgents<YouTubeAgent>* scenario);
	~AbstractYouTube();

	virtual void init_schedule(repast::ScheduleRunner& runner, double offset = 0){}


	// Actions:
	void doAction(ACTIONTYPE action);
	void api_post(ACTIONTYPE action);
	void api_comment(ACTIONTYPE action);

//	void selectFittestAgents(int n, std::vector<YouTubeAgent*>& agents, YouTubeAgentType type);
//	void updateCurrentFittestFeed(int n);

	virtual boost::shared_ptr<YouTubeFeed<SCORETYPE>> getFeed(AGENTTYPE* requestor, YouTubeFeedQuery query);
	void addRandomElementsToFeed(YouTubeFeed<SCORETYPE>& feed, int count);

	void getSelector(YouTubeAgent* source, SimpleWeightedSelector& sws);
	long getTotalActionCount(YouTubeAgent* source);

};


template<typename AGENTTYPE, typename ACTIONTYPE, typename SCORETYPE, typename PACKAGETYPE, typename FILEWRITERTYPE>
AbstractYouTube<AGENTTYPE, ACTIONTYPE, SCORETYPE, PACKAGETYPE, FILEWRITERTYPE>::AbstractYouTube(boost::mpi::communicator* world, ScenarioWithAgents<YouTubeAgent>* scenario):
  SocialNetwork_Platform<YouTubeAgentType,
	AGENTTYPE,
	YouTubeEdge<AGENTTYPE, YouTubeEdgeInfo>,
	YouTubeEdgeContent<AGENTTYPE, YouTubeEdgeInfo>,
	YouTubeEdgeContentManager<AGENTTYPE, YouTubeEdgeInfo>,
	PACKAGETYPE,
	ACTIONTYPE,
	YouTubeFeedQuery,
	YouTubeFeedExplanation,
	FILEWRITERTYPE>(world, "youtube", "YT", 3, scenario){

}


template<typename AGENTTYPE, typename ACTIONTYPE, typename SCORETYPE, typename PACKAGETYPE, typename FILEWRITERTYPE>
AbstractYouTube<AGENTTYPE, ACTIONTYPE, SCORETYPE, PACKAGETYPE, FILEWRITERTYPE>::~AbstractYouTube(){}


template<typename AGENTTYPE, typename ACTIONTYPE, typename SCORETYPE, typename PACKAGETYPE, typename FILEWRITERTYPE>
boost::shared_ptr<YouTubeFeed<SCORETYPE>> AbstractYouTube<AGENTTYPE, ACTIONTYPE, SCORETYPE, PACKAGETYPE, FILEWRITERTYPE>::getFeed(AGENTTYPE* requestor, YouTubeFeedQuery query){
	boost::shared_ptr<YouTubeFeed<SCORETYPE>> ret(new YouTubeFeed<SCORETYPE>(query.countOfRandomVideosToRetrieve));
	if(query.type == YouTubeFeedQueryType::RETRIEVE_RANDOM_VIDEOS){
		addRandomElementsToFeed(*ret, query.countOfRandomVideosToRetrieve);
	}
	return ret;
}

template<typename AGENTTYPE, typename ACTIONTYPE, typename SCORETYPE, typename PACKAGETYPE, typename FILEWRITERTYPE>
void AbstractYouTube<AGENTTYPE, ACTIONTYPE, SCORETYPE, PACKAGETYPE, FILEWRITERTYPE>::addRandomElementsToFeed(YouTubeFeed<SCORETYPE>& feed, int count){
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

template<typename AGENTTYPE, typename ACTIONTYPE, typename SCORETYPE, typename PACKAGETYPE, typename FILEWRITERTYPE>
void AbstractYouTube<AGENTTYPE, ACTIONTYPE, SCORETYPE, PACKAGETYPE, FILEWRITERTYPE>::doAction(ACTIONTYPE action){
	YouTubeEventType eventType = action.type;
	switch (eventType) {;
		case POST:       			api_post(action);          		break;
		case COMMENT:      			api_comment(action);				break;
//		case CREATE:				api_createSubreddit(action);		break;
//		case DELETE:			    api_deleteSubreddit(action);		break;
//		case SUBSCRIBE:				api_subscribe(action);			break;
//		case UNSUBSCRIBE:		    api_stopSubscription(action);	break;
		default:{
		}
	}
}


template<typename AGENTTYPE, typename ACTIONTYPE, typename SCORETYPE, typename PACKAGETYPE, typename FILEWRITERTYPE>
void AbstractYouTube<AGENTTYPE, ACTIONTYPE, SCORETYPE, PACKAGETYPE, FILEWRITERTYPE>::api_post(ACTIONTYPE action){
//	YouTubeAgentPackage newPackage;

	// Create the new agent
    AGENTTYPE* newPost = this->createAndAddNewAgent(action.getNewAgentPackage());

    action.target = newPost;
    action.payload.newPostOrCommentID = newPost->showName();

    // if(directPropagation) propagate(action);

    this->getEdge(action.actor, action.target)->logEvent(action.type);
	this->countEvent(POST);
	this->writeAction(action);
}
//
template<typename AGENTTYPE, typename ACTIONTYPE, typename SCORETYPE, typename PACKAGETYPE, typename FILEWRITERTYPE>
void AbstractYouTube<AGENTTYPE, ACTIONTYPE, SCORETYPE, PACKAGETYPE, FILEWRITERTYPE>::api_comment(ACTIONTYPE action){
	action.target->receiveAction(action);

//	//	if(directPropagation) propagate(action);

	this->getEdge(action.actor, action.target)->logEvent(COMMENT);
	this->countEvent(COMMENT);
	this->writeAction(action);
}



template<typename AGENTTYPE, typename ACTIONTYPE, typename SCORETYPE, typename PACKAGETYPE, typename FILEWRITERTYPE>
void AbstractYouTube<AGENTTYPE, ACTIONTYPE, SCORETYPE, PACKAGETYPE, FILEWRITERTYPE>::getSelector(YouTubeAgent* source, SimpleWeightedSelector& sws){
////	std::vector<YouTubeAgent*> recipients;
////	agentNetwork->successors(source, recipients);
////	std::vector<YouTubeAgent*>::iterator iter = recipients.begin();
////	while(iter != recipients.end()){
////		boost::shared_ptr<BasicYouTubeEdge> edge = findEdge(source, (*iter));
////		sws.adjust(POST, edge->numPosts());
////		sws.adjust(COMMENT, edge->numComments());
////		iter++;
////	}
////
//	sws.clear();
//	for(int i = 0; i < grandTotalEvents->size(); i++){
//		sws.addValue(grandTotalEvents->at(i)); // Simple
//		//sws.addValue((int)( pow(((double)grandTotalEvents->at(i))/1.0),1.0)); // Allows scaling
//	}
//
//
}

template<typename AGENTTYPE, typename ACTIONTYPE, typename SCORETYPE, typename PACKAGETYPE, typename FILEWRITERTYPE>
long AbstractYouTube<AGENTTYPE, ACTIONTYPE, SCORETYPE, PACKAGETYPE, FILEWRITERTYPE>::getTotalActionCount(YouTubeAgent* source){
	long ret = 0;
//	std::vector<YouTubeAgent*> recipients;
////	agentNetwork->successors(source, recipients);
//	agentNetwork->adjacent(source, recipients);
//	std::vector<YouTubeAgent*>::iterator iter = recipients.begin();
//	while(iter != recipients.end()){
//		boost::shared_ptr<BasicYouTubeEdge> edge1 = findEdge(source, (*iter));
//		if(edge1 != 0) ret += edge1->getTotalEvents();
//
//		boost::shared_ptr<BasicYouTubeEdge> edge2 = findEdge((*iter), source);
//		if(edge2 != 0) ret += edge2->getTotalEvents();
//		iter++;
//	}
	return ret;
}


} // End namespace youtube

#endif /* INCLUDE_YOUTUBE_PLATFORM_H */
