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
/* Reddit_Platform.h */
#ifndef INCLUDE_REDDIT_PLATFORM_H
#define INCLUDE_REDDIT_PLATFORM_H

#include <vector>

#include "utilities/SimpleWeightedSelector.h"

#include "socialnetwork_toolkit/SocialNetwork_Platform.h"
#include "socialnetwork_toolkit/BehaviorSelection.h"

#include "platforms/Reddit/Reddit_Definitions.h"
#include "platforms/Reddit/Reddit_Agent.h"
#include "platforms/Reddit/Reddit_Network.h"
#include "platforms/Reddit/Reddit_Feed_Query.h"

using namespace rhpc_smple;

namespace reddit{


template<typename AGENTTYPE, typename ACTIONTYPE, typename SCORETYPE, typename PACKAGETYPE, typename FILEWRITERTYPE>
class AbstractReddit:
		public SocialNetwork_Platform<RedditAgentType,
		                              AGENTTYPE,
									  RedditEdge<AGENTTYPE, RedditEdgeInfo>,
									  RedditEdgeContent<AGENTTYPE, RedditEdgeInfo>,
									  RedditEdgeContentManager<AGENTTYPE, RedditEdgeInfo>,
									  PACKAGETYPE,
									  ACTIONTYPE,
									  RedditFeedQuery,
									  RedditFeedExplanation,
									  FILEWRITERTYPE>{
protected:
	std::set<std::string> subreddits;
	std::vector<std::string> subredditsList;
	RedditFeed<SCORETYPE>* currentFittest;


public:
	AbstractReddit(boost::mpi::communicator* world, ScenarioWithAgents<AGENTTYPE>* scenario);
	~AbstractReddit();

	virtual void init_schedule(repast::ScheduleRunner& runner, double offset = 0);


	// API Functions
	void doAction(ACTIONTYPE action);
	void api_post(ACTIONTYPE action);
	void api_comment(ACTIONTYPE action);
	void api_createSubreddit(ACTIONTYPE action);
	void api_deleteSubreddit(ACTIONTYPE action);
	void api_subscribe(ACTIONTYPE action);
	void api_stopSubscription(ACTIONTYPE action);


	// Special for this platform
	void addSubreddit(std::string name);
	std::string getRandomlySelectedCommunity();


	// TO BE MOVED TO HIGHER LEVEL

	//    -- Convert this to take a query and an agent type to update?
//    void updateCurrentFittestFeed(int n);

	//    -- Modify this to fit paradigm: pass query, indicate whether fitness
	//       is to be recalculated...?
//    void selectFittestAgents(int n, std::vector<RedditAgent*>& agents, RedditAgentType type);
    void getSelector(AGENTTYPE* source, SimpleWeightedSelector& sws);


    virtual boost::shared_ptr<RedditFeed<SCORETYPE>> getFeed(AGENTTYPE* requestor, RedditFeedQuery query);


	// This is specifically a Multiplex issue, I think...
//  void getOutlinkSums(RedditAgent* source, BasicRedditEdge& accumulator);
    long getTotalActionCount(AGENTTYPE* source);


//    std::map<std::string, double>& getShocks();

};








template<typename AGENTTYPE, typename ACTIONTYPE, typename SCORETYPE, typename PACKAGETYPE, typename FILEWRITERTYPE>
AbstractReddit<AGENTTYPE, ACTIONTYPE, SCORETYPE, PACKAGETYPE, FILEWRITERTYPE>::AbstractReddit(boost::mpi::communicator* world, ScenarioWithAgents<AGENTTYPE>* scenario):
    SocialNetwork_Platform<RedditAgentType,
      AGENTTYPE,
	  RedditEdge<AGENTTYPE, RedditEdgeInfo>,
	  RedditEdgeContent<AGENTTYPE, RedditEdgeInfo>,
	  RedditEdgeContentManager<AGENTTYPE, RedditEdgeInfo>,
	  PACKAGETYPE,
	  ACTIONTYPE,
	  RedditFeedQuery,
	  RedditFeedExplanation,
	  FILEWRITERTYPE>(world, "reddit", "RD", 7, scenario), currentFittest(0){

//   addSubreddit("Community_1");
//   addSubreddit("Community_2");
//   addSubreddit("Community_3");

}

template<typename AGENTTYPE, typename ACTIONTYPE, typename SCORETYPE, typename PACKAGETYPE, typename FILEWRITERTYPE>
AbstractReddit<AGENTTYPE, ACTIONTYPE, SCORETYPE, PACKAGETYPE, FILEWRITERTYPE>::~AbstractReddit(){}



template<typename AGENTTYPE, typename ACTIONTYPE, typename SCORETYPE, typename PACKAGETYPE, typename FILEWRITERTYPE>
void AbstractReddit<AGENTTYPE, ACTIONTYPE, SCORETYPE, PACKAGETYPE, FILEWRITERTYPE>::init_schedule(repast::ScheduleRunner& runner, double offset){}


template<typename AGENTTYPE, typename ACTIONTYPE, typename SCORETYPE, typename PACKAGETYPE, typename FILEWRITERTYPE>
void AbstractReddit<AGENTTYPE, ACTIONTYPE, SCORETYPE, PACKAGETYPE, FILEWRITERTYPE>::addSubreddit(std::string name){
	subreddits.insert(name); // Will check for dups
	subredditsList.clear();
	subredditsList.insert(subredditsList.begin(), subreddits.begin(), subreddits.end());
}

template<typename AGENTTYPE, typename ACTIONTYPE, typename SCORETYPE, typename PACKAGETYPE, typename FILEWRITERTYPE>
boost::shared_ptr<RedditFeed<SCORETYPE>> AbstractReddit<AGENTTYPE, ACTIONTYPE, SCORETYPE, PACKAGETYPE, FILEWRITERTYPE>::getFeed(AGENTTYPE* requestor, RedditFeedQuery query){
	repast::Logger& logger = repast::Log4CL::instance()->get_logger("reddit");
	boost::shared_ptr<RedditFeed<SCORETYPE>> ret(new RedditFeed<SCORETYPE>());
	if(query.type == RedditFeedQueryType::RETRIEVE_RANDOM_POSTS){
//		addRandomPostsToFeed(*ret, query.countOfRandomPostsToRetrieve);
	}
	return ret;
}




template<typename AGENTTYPE, typename ACTIONTYPE, typename SCORETYPE, typename PACKAGETYPE, typename FILEWRITERTYPE>
void AbstractReddit<AGENTTYPE, ACTIONTYPE, SCORETYPE, PACKAGETYPE, FILEWRITERTYPE>::doAction(ACTIONTYPE action){
	RedditEventType eventType = action.type;
//	//std::cout << " DOING ACTION " << eventType << " WITH ACTOR " << action.actor << " AND TARGET " << action.target << std::endl;
	switch (eventType) {;
		case POST:       			api_post(action);          		break;
		case COMMENT:      			api_comment(action);			break;
		case CREATE:				api_createSubreddit(action);	break;
		case DELETE:				api_deleteSubreddit(action);	break;
		case SUBSCRIBE:				api_subscribe(action);			break;
		case UNSUBSCRIBE:		    api_stopSubscription(action);	break;
		default:{
		}
	}
	// Write the record of what was done, and why
	// Write To Event Record
	// Write To Explanatory Record
}

template<typename AGENTTYPE, typename ACTIONTYPE, typename SCORETYPE, typename PACKAGETYPE, typename FILEWRITERTYPE>
void AbstractReddit<AGENTTYPE, ACTIONTYPE, SCORETYPE, PACKAGETYPE, FILEWRITERTYPE>::api_post(ACTIONTYPE action){
	// Doing a post means creating a new post object
	PACKAGETYPE newPackage;
	newPackage.type = POST_AGENT;
	newPackage.community = action.payload.communityID;
//	newPackage.whichCluster = 0;
	newPackage.creatorID = action.actor->getId();
	// HashTagID is left blank; platform 'create' method will add this
//	// Set any content for the post from the action.payload into the package
	AGENTTYPE* newPost = this->createAndAddNewAgent(newPackage);
	// Potentially this could be passed via the package
//	newPost->informationIDs.insert(action.payload.informationIDs.begin(), action.payload.informationIDs.end());
//	// Add new post to graph
//	// Connect Agent to New Post
//	// Connect New Post to SubReddit
//	newPost->communityID = action.payload.communityID;
//	newPost->partOfSeededCascade = action.payload.partOfSeededCascade;
//	if(action.payload.partOfSeededCascade){
//		newPost->setHashTagID(action.payload.newPostOrCommentID);
//	}
//	else{
//		action.payload.newPostOrCommentID = newPost->showName();
//	}
	action.target = newPost;
	action.payload.newPostOrCommentID = newPost->showName();
//
////    if(this->directPropagation) propagate(action);
//	// if(directPropagation){
//	// 	// Propagate to recipients...
//	// 	vector<RedditAgent*> recipients;
//	// 	agentNetwork->successors(action.actor, recipients);
//	// 	vector<RedditAgent*>::iterator iter = recipients.begin();
//	// 	RedditFeedExplanation fe;
//	// 	fe.userIsFollowing = true;
//	// 	//std::cout << " SOURCE " << action.actor << " acting: " << std::endl;
//	// 	while(iter != recipients.end()){
//	// 		if((*iter)->getType() == USER_AGENT){
//	// 			//std::cout << "      RECIPIENT: " << (*iter) << " receiving (post) " << newPost->showName() << std::endl;
//	// 			(*iter)->placeInFeed(newPost, 0, 0, IDLE, fe);
//	// 		}
//	// 		iter++;
//	// 	}
//	// }
//
//    /////// TO DO: Replace this
////	boost::shared_ptr<BasicRedditEdge> edge = getEdge(action.actor, action.target);
////	edge->logEvent(action.type);
	this->countEvent(POST);
	this->writeAction(action);
}

template<typename AGENTTYPE, typename ACTIONTYPE, typename SCORETYPE, typename PACKAGETYPE, typename FILEWRITERTYPE>
void AbstractReddit<AGENTTYPE, ACTIONTYPE, SCORETYPE, PACKAGETYPE, FILEWRITERTYPE>::api_comment(ACTIONTYPE action){
	action.target->receiveAction(action);
	this->getEdge(action.actor, action.target)->logEvent(COMMENT);
	this->countEvent(COMMENT);
	this->writeAction(action);

	//std::cout << " DOING AN API COMMENT FOR " << action.actor->showName() << " with action type " << action.type << std::endl;
    // Assumes that the action contains:
	//    A source (the actor doing it)
	//    A post that will be the root node of the comment
	//    An index that is either 0 (indicating that the comment is a child of the root post)
	//       or a value > 0 that is the 1 - indexed position in the root's vector of comments
//	AGENTTYPE* post = (AGENTTYPE*)(action.target);
//	((RedditPostAgent*)(action.target))->informationIDs.insert(action.payload.informationIDs.begin(), action.payload.informationIDs.end());
//    // std::cout << "Post is " << post << std::endl;
//	post->touch();
//	int indx = (action.subtarget == 0 ? 0: ((RedditComment*)(action.subtarget))->indx); // Note: These values are 1- indexed, not 0-indexed
//	RedditComment* newComment = 0;
//	if(indx == 0){
//		newComment = post->addComment(action.actor);
//	}
//	else{
//		indx--;
//		RedditComment* parentComment = post->comments.at(indx);
//		newComment = post->addComment(action.actor, parentComment);
//
//	}
//	action.payload.newPostOrCommentID = newComment->nodeID;
//	action.payload.partOfSeededCascade = post->partOfSeededCascade;
//	newComment->partOfSeededCascade = post->partOfSeededCascade;
//	action.payload.communityID = post->communityID;
//	action.newlyCreatedObject = newComment;
//
////	if(this->directPropagation) propagate(action);
//
//	// Record the action in the network
////	boost::shared_ptr<BasicRedditEdge> edge = getEdge(action.actor, action.target);
////	edge->logEvent(action.type);

	// Write the action as output
//	this->countEvent(COMMENT);
//	this->writeAction(action);
}

template<typename AGENTTYPE, typename ACTIONTYPE, typename SCORETYPE, typename PACKAGETYPE, typename FILEWRITERTYPE>
void AbstractReddit<AGENTTYPE, ACTIONTYPE, SCORETYPE, PACKAGETYPE, FILEWRITERTYPE>::api_createSubreddit(ACTIONTYPE action){
//	((RedditPostAgent*)(action.target))->informationIDs.insert(action.payload.informationIDs.begin(), action.payload.informationIDs.end());
//	writeAction(action);
}

template<typename AGENTTYPE, typename ACTIONTYPE, typename SCORETYPE, typename PACKAGETYPE, typename FILEWRITERTYPE>
void AbstractReddit<AGENTTYPE, ACTIONTYPE, SCORETYPE, PACKAGETYPE, FILEWRITERTYPE>::api_deleteSubreddit(ACTIONTYPE action){
//	((RedditPostAgent*)(action.target))->informationIDs.insert(action.payload.informationIDs.begin(), action.payload.informationIDs.end());
	this->writeAction(action);
}

template<typename AGENTTYPE, typename ACTIONTYPE, typename SCORETYPE, typename PACKAGETYPE, typename FILEWRITERTYPE>
void AbstractReddit<AGENTTYPE, ACTIONTYPE, SCORETYPE, PACKAGETYPE, FILEWRITERTYPE>::api_subscribe(ACTIONTYPE action){
//	((RedditPostAgent*)(action.target))->informationIDs.insert(action.payload.informationIDs.begin(), action.payload.informationIDs.end());
//	writeAction(action);
}

template<typename AGENTTYPE, typename ACTIONTYPE, typename SCORETYPE, typename PACKAGETYPE, typename FILEWRITERTYPE>
void AbstractReddit<AGENTTYPE, ACTIONTYPE, SCORETYPE, PACKAGETYPE, FILEWRITERTYPE>::api_stopSubscription(ACTIONTYPE action){
//	((RedditPostAgent*)(action.target))->informationIDs.insert(action.payload.informationIDs.begin(), action.payload.informationIDs.end());
//	writeAction(action);
}




template<typename AGENTTYPE, typename ACTIONTYPE, typename SCORETYPE, typename PACKAGETYPE, typename FILEWRITERTYPE>
std::string AbstractReddit<AGENTTYPE, ACTIONTYPE, SCORETYPE, PACKAGETYPE, FILEWRITERTYPE>::getRandomlySelectedCommunity(){
	return subredditsList.at( (int)((double)subredditsList.size() * repast::Random::instance()->nextDouble()));
}

//template<typename AGENTTYPE, typename ACTIONTYPE, typename SCORETYPE, typename PACKAGETYPE, typename FILEWRITERTYPE>
//void AbstractReddit<AGENTTYPE, ACTIONTYPE, SCORETYPE, PACKAGETYPE, FILEWRITERTYPE>::getOutlinkSums(RedditAgent* source, BasicRedditEdge& accumulator){
////	std::vector<RedditAgent*> recipients;
////	agentNetwork->successors(source, recipients);
////	std::vector<RedditAgent*>::iterator iter = recipients.begin();
////	while(iter != recipients.end()){
////		boost::shared_ptr<BasicRedditEdge> edge = findEdge(source, (*iter));
////		accumulator.getEdgeInfo().eventCounter.add(edge->getEdgeInfo().eventCounter);
////		iter++;
////	}
//}
//
template<typename AGENTTYPE, typename ACTIONTYPE, typename SCORETYPE, typename PACKAGETYPE, typename FILEWRITERTYPE>
long AbstractReddit<AGENTTYPE, ACTIONTYPE, SCORETYPE, PACKAGETYPE, FILEWRITERTYPE>::getTotalActionCount(AGENTTYPE* source){
	long ret = 0;
//	std::vector<RedditAgent*> recipients;
//	agentNetwork->adjacent(source, recipients);
//	std::vector<RedditAgent*>::iterator iter = recipients.begin();
//	while(iter != recipients.end()){
//		boost::shared_ptr<BasicRedditEdge> edge1 = findEdge(source, (*iter));
//		if(edge1 != 0) ret += edge1->getTotalEvents();
//
//		boost::shared_ptr<BasicRedditEdge> edge2 = findEdge((*iter), source);
//		if(edge2 != 0) ret += edge2->getTotalEvents();
//		iter++;
//	}
	return ret;
}
//
//template<typename AGENTTYPE, typename ACTIONTYPE, typename SCORETYPE, typename PACKAGETYPE, typename FILEWRITERTYPE>
//void AbstractReddit<AGENTTYPE, ACTIONTYPE, SCORETYPE, PACKAGETYPE, FILEWRITERTYPE>::selectFittestAgents(int n, std::vector<RedditAgent*>& agents, RedditAgentType whichType){
////	std::set<RedditAgent*>* agentsTempPtr = new std::set<RedditAgent*>();
////	std::set<RedditAgent*>& agentsTemp = *agentsTempPtr;
////	context.selectAgents(repast::SharedContext<RedditAgent>::LOCAL, agentsTemp, (int)whichType);
////	std::vector<RedditAgent*> agentsTempii;
////	agentsTempii.insert(agentsTempii.begin(), agentsTemp.begin(), agentsTemp.end());
////    delete agentsTempPtr; // don't need this anymore
////
////    // double maxFitness = 1;
////	std::vector<RedditAgent*>::iterator iter = agentsTempii.begin();
////	// while(iter != agentsTempii.end()){
////	// 	double f = (*iter)->getFitness();
////	// 	if(f > maxFitness) maxFitness = f;
////	// 	iter++;
////	// }
////
////	ParameterizedWeightedSelector pws = PWSFactory::getInstance()->getParameterizedWeightedSelector("reddit", "target");
////	// SimpleWeightedSelector sws(-1);
////	// iter = agentsTempii.begin();
////	while(iter != agentsTempii.end()){
////		// double f = pow((*iter)->getFitness()/maxFitness, 4);
////		double f = (*iter)->getFitness();
////		//if(f > 1) std::cout << " TOO HIGH F VALUE: " << f << " type: " << whichType << std::endl;
////		// long val = (long)(10000 * f + 1);
////		pws.addValue(f); // get something about the agent...
////		iter++;
////	}
////
////	std::set<RedditAgent*> countSet;
////
////	// Do a check here; if the sum is zero, it means all the entries are zero,
////	// and we want to select fully randomly (no weighting) in that case.
////	if(pws.getSum() == 0) for(int i = 0; i < pws.size(); i++) pws.increment(i);
////
////	while(countSet.size() < n && pws.getSum() > 0){
////		int indx = pws.select();
////		RedditAgent* toInsert = agentsTempii.at(indx);
////		pws.clearAt(indx);
////		pair<std::set<RedditAgent*>::iterator, bool> ret = countSet.insert(toInsert);
////		if(ret.second) agents.push_back(toInsert);
////	}
//}
//
//template<typename AGENTTYPE, typename ACTIONTYPE, typename SCORETYPE, typename PACKAGETYPE, typename FILEWRITERTYPE>
//void AbstractReddit<AGENTTYPE, ACTIONTYPE, SCORETYPE, PACKAGETYPE, FILEWRITERTYPE>::updateCurrentFittestFeed(int n){
////	currentFittest = new RedditFeed();
////	currentFittest->setMaxSize(n);
////	std::vector<RedditAgent*> agents;
////	selectFittestAgents(n, agents, POST_AGENT);
////	std::vector<RedditAgent*>::iterator iter = agents.begin();
////	while(iter != agents.end()){
////		RedditFeedExplanation fe;
////		fe.randomlySelected = true;
////		fe.fitnessValue = (*iter)->getFitness();
////		currentFittest->push(0, *iter, 0, IDLE, fe);
////		iter++;
////	}
////	currentFittest->initiateWeightedSampling();
//}
//
template<typename AGENTTYPE, typename ACTIONTYPE, typename SCORETYPE, typename PACKAGETYPE, typename FILEWRITERTYPE>
void AbstractReddit<AGENTTYPE, ACTIONTYPE, SCORETYPE, PACKAGETYPE, FILEWRITERTYPE>::getSelector(AGENTTYPE* source, SimpleWeightedSelector& sws){
//	std::vector<RedditAgent*> recipients;
//	agentNetwork->successors(source, recipients);
//	std::vector<RedditAgent*>::iterator iter = recipients.begin();
//	while(iter != recipients.end()){
//		boost::shared_ptr<BasicRedditEdge> edge = findEdge(source, (*iter));
//		sws.adjust(POST, edge->numPosts());
//		sws.adjust(COMMENT, edge->numComments());
//		iter++;
//	}
//
//	sws.clear();
//	for(int i = 0; i < grandTotalEvents->size(); i++){
//		sws.addValue(grandTotalEvents->at(i)); // Simple
//		//sws.addValue((int)( pow(((double)grandTotalEvents->at(i))/1.0),1.0)); // Allows scaling
//	}


}
//
//
//template<typename AGENTTYPE, typename ACTIONTYPE, typename SCORETYPE, typename PACKAGETYPE, typename FILEWRITERTYPE>
//std::map<std::string, double>& AbstractReddit<AGENTTYPE, ACTIONTYPE, SCORETYPE, PACKAGETYPE, FILEWRITERTYPE>::getShocks(){
//  return this->enclosingScenario->getShocks();
//}
//
//
//





} // End namespace reddit

#endif /* INCLUDE_REDDIT_PLATFORM_H */
