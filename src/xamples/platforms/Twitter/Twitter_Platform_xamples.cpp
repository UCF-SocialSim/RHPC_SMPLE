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
/* Twitter_Platform_xamples.cpp */
#include "xamples/platforms/Twitter/Twitter_Platform_xamples.h"


#include "utilities/Utilities.h"
#include "utilities/ParameterizedWeightedSelector.h"

using namespace twitter;

extern ptime currentEventTime;         // A handle for the global simulation time

Twitter::Twitter(boost::mpi::communicator* world, ScenarioWithAgents<TwitterAgent>* scenario):
    AbstractTwitter<TwitterAgent, TwitterAction, Twitter_Xamples_ScoreTypes, TwitterAgentPackage, Twitter_FileWriter>(world, scenario){
}

Twitter::~Twitter(){}

void Twitter::init_schedule(repast::ScheduleRunner& runner, double offset){ }

TwitterAgentPackage Twitter::getPackage(TwitterAgent* agent){
	repast::AgentId id = agent->getId();
	TwitterAgentPackage package(id.id(), id.startingRank(), agent->getType(), id.currentRank(), agent->getHashTagID());
	return package;
}


void Twitter::updateAgent(TwitterAgentPackage package){
    repast::AgentId id(package.id, package.rank, package.type);
    TwitterAgent* agent = context.getAgent(id);
	if (agent != NULL) updateAgent(package, agent);
}

void Twitter::updateAgent(TwitterAgentPackage package, TwitterAgent* agent){
	agent->setCurrentRank(package.currentRank); // THIS IS REQUIRED
	agent->setHashTagID(package.hashTagID);
	set_local_pointers(agent);
}


void Twitter::provideContent(repast::AgentRequest req, std::vector<TwitterAgentPackage>& out) {
	// Needed for parallelization only
}

void Twitter::providePackage(TwitterAgent * agent, std::vector<TwitterAgentPackage>& out){
    out.push_back(getPackage(agent));
}

TwitterAgent* Twitter::createNewAgent(repast::AgentId id, TwitterAgentPackage agentPackage){
	ptime creationDate = (agentPackage.dateCreated == boost::date_time::not_a_date_time ? currentEventTime : agentPackage.dateCreated);
	if     (agentPackage.type == USER_AGENT){
		return new TwitterUserAgent(id, creationDate, agentPackage, this);
	}
	else if(agentPackage.type == CONVERSATION_AGENT){
		// Conflict resolution
		SocialNetwork_AbstractElement<TwitterAgentType, Twitter_Xamples_ScoreTypes>* creator = context.getAgent(agentPackage.creatorID);
		TwitterConversationAgent* newAgent = new TwitterConversationAgent(id, creationDate, agentPackage, this, creator);
		newAgent->informationIDs.insert(agentPackage.informationIDs.begin(), agentPackage.informationIDs.end());
		return newAgent;
	}
	else return 0;
}

/*
 * Note the difference between this method and 'createAgent(Package)' above.
 * The 'createAgent(Package)' method is mandated by the RHPC parallelization
 * strategy and refers to creating an agent that already exists on another
 * process; the agent package passed will include the full information
 * about the agent, including its ID value. This method creates a NEW agent-
 * one that doesn't exist elsewhere. The method does NOT use the ID information
 * in the agent package; the ID is assigned as if the agent is completely
 * new. However, other information in the agent package is used to construct
 * the initial instance values for the agent. These are then used to set
 * any local pointers that the agent might need.
 */
TwitterAgent* Twitter::createAndAddNewAgent(TwitterAgentPackage agentPackage){
	repast::AgentId id(nextAgentIndex, rank, agentPackage.type, rank); // Note: increment count of agents created on this process
	TwitterAgent* agent = (TwitterAgent*)createNewAgent(id, agentPackage);

	// Call to set pointer values, which may be based on non-pointer values (e.g. cluster)
	set_local_pointers(agent);
	context.addAgent(agent);
	nextAgentIndex++;
	return agent;
}

void Twitter::setLocalPointers(TwitterAgent* agent){
	agent->setPlatform(this);
}

boost::shared_ptr<TwitterFeed<Twitter_Xamples_ScoreTypes>> Twitter::getFeed(TwitterAgent* requestor, TwitterFeedQuery_xamples query){
	boost::shared_ptr<TwitterFeed<Twitter_Xamples_ScoreTypes>> ret(new TwitterFeed<Twitter_Xamples_ScoreTypes>());
	if(query.type == TwitterFeedQueryType_xamples::TW_BASIC){
		std::vector<TwitterAgent*> conversationAgents;
		selectFittestAgents(query.countOfRandomTweetsToRetrieve, conversationAgents, CONVERSATION_AGENT, query.scoreType);
		for(int i = 0; i < conversationAgents.size(); i++){
			TwitterFeedExplanation feedExp;
			feedExp.fitnessValue = conversationAgents.at(i)->getScore(query.scoreType);
			ret->push(0, conversationAgents.at(i), 0, IDLE, feedExp);
		}
		return ret;
	}
	else{
		LOG_DEBUG_CANCEL("twitter", "PASSING TO PARENT... ");
		return AbstractTwitter<TwitterAgent, TwitterAction, Twitter_Xamples_ScoreTypes, TwitterAgentPackage, Twitter_FileWriter>::getFeed(requestor, query);
	}
}


void Twitter::getOutlinkSums(TwitterAgent* source, TwitterEdge<TwitterAgent, TwitterEdgeInfo>& accumulator){
	std::vector<TwitterAgent*> recipients;
	agentNetwork->successors(source, recipients);
	std::vector<TwitterAgent*>::iterator iter = recipients.begin();
	while(iter != recipients.end()){
		boost::shared_ptr<TwitterEdge<TwitterAgent, TwitterEdgeInfo>> edge = findEdge(source, (*iter));
		accumulator.getEdgeInfo().eventCounter.add(edge->getEdgeInfo().eventCounter);
		iter++;
	}
}

long Twitter::getTotalActionCount(TwitterAgent* source){
	long ret = 0;
	std::vector<TwitterAgent*> recipients;
	agentNetwork->adjacent(source, recipients);
	std::vector<TwitterAgent*>::iterator iter = recipients.begin();
	while(iter != recipients.end()){
		boost::shared_ptr<TwitterEdge<TwitterAgent, TwitterEdgeInfo>> edge1 = findEdge(source, (*iter));
		if(edge1 != 0) ret += edge1->getTotalEvents();

		boost::shared_ptr<TwitterEdge<TwitterAgent, TwitterEdgeInfo>> edge2 = findEdge((*iter), source);
		if(edge2 != 0) ret += edge2->getTotalEvents();
		iter++;
	}
	return ret;
}

void Twitter::selectFittestAgents(int n, std::vector<TwitterAgent*>& agents, TwitterAgentType whichType, Twitter_Xamples_ScoreTypes score){

	// Need these in a vector; this calls selectAgents with a set, which is faster
	// (when called with a vector it performs a shuffle for random order)
	std::set<TwitterAgent*>* agentsSetPtr = new std::set<TwitterAgent*>();
	std::set<TwitterAgent*>& agentsSet = *agentsSetPtr;
	context.selectAgents(repast::SharedContext<TwitterAgent>::LOCAL, agentsSet, (int)whichType);
	std::vector<TwitterAgent*> agentsVec;
	agentsVec.insert(agentsVec.begin(), agentsSet.begin(), agentsSet.end());
    delete agentsSetPtr; // don't need this anymore

	SimpleWeightedSelector sws(-1);
	std::vector<TwitterAgent*>::iterator iter = agentsVec.begin();
	iter = agentsVec.begin();
	while(iter != agentsVec.end()){
		sws.addValue((*iter)->getScore(score) * 100);
		iter++;
	}
	LOG_INFO("twitter", "        sws size = " + std::to_string(sws.size()) + " and sum = " + std::to_string(sws.getSum()));

	std::set<TwitterAgent*> countSet;

	// Do a check here; if the sum is zero, it means all the entries are zero,
	// and we want to select fully randomly (no weighting) in that case.
	if(sws.getSum() == 0) for(int i = 0; i < sws.size(); i++) sws.increment(i);

	while(countSet.size() < n && sws.getSum() > 0){
		int indx = sws.select();
		TwitterAgent* toInsert = agentsVec.at(indx);
		sws.clearAt(indx);
		pair<std::set<TwitterAgent*>::iterator, bool> ret = countSet.insert(toInsert);
		if(ret.second) agents.push_back(toInsert);
	}
}

//void Twitter::updateCurrentFittestFeed(int n){
//	LOG_INFO("twitter", " running updateCurrentFittestFeed with n = " + std::to_string(n));
//	currentFittest = new TwitterFeed<Twitter_Xamples_ScoreTypes>();
//	currentFittest->setMaxSize(n);
//	std::vector<TwitterAgent*> agents;
//	selectFittestAgents(n, agents, CONVERSATION_AGENT);
//	LOG_INFO("twitter", " SELECTED FITTEST AGENTS: " + std::to_string(agents.size()));
//	std::vector<TwitterAgent*>::iterator iter = agents.begin();
//	while(iter != agents.end()){
//		TwitterFeedExplanation fe;
//		fe.randomlySelected = true;
////		fe.fitnessValue = (*iter)->getFitness();
//		currentFittest->push(0, *iter, 0, IDLE, fe);
//		iter++;
//	}
//	LOG_INFO("twitter", " After adding, current fittest has " + std::to_string(currentFittest->size()));
//	// This is a hack to accommodate the way some old-style initialization was done.
//	ParameterizedWeightedSelector pws = PWSFactory::getInstance()->getParameterizedWeightedSelector("twitter", "target");
//	currentFittest->initiateWeightedSampling(TWITTER_XAMPLES_DEFAULT_SCORE, pws.getMultCoeff(), pws.getExpPower(), pws.getExpectedUpperBound());
//}

void Twitter::getSelector(TwitterAgent* source, SimpleWeightedSelector& sws){
//	std::vector<TwitterAgent*> recipients;
//	agentNetwork->successors(source, recipients);
//	std::vector<TwitterAgent*>::iterator iter = recipients.begin();
//	while(iter != recipients.end()){
//		boost::shared_ptr<BasicTwitterEdge> edge = findEdge(source, (*iter));
//		sws.adjust(TWEET,   edge->numTweets());
//		sws.adjust(QUOTE,   edge->numQuotes());
//		sws.adjust(RETWEET, edge->numRetweets());
//		sws.adjust(REPLY,   edge->numReplies());
//		iter++;
//	}

	sws.clear();
	for(int i = 0; i < grandTotalEvents->size(); i++){
		sws.addValue(grandTotalEvents->at(i)); // Simple
		//sws.addValue((int)( pow(((double)grandTotalEvents->at(i))/1.0),1.0)); // Allows scaling
	}
}
