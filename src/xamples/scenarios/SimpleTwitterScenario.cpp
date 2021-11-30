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
/* SimpleTwitterScenario.cpp */
#include "xamples/scenarios/SimpleTwitterScenario.h"

#include "boost/algorithm/string.hpp"

#include "utilities/Utilities.h"
#include "utilities/ParameterizedWeightedSelector.h"

#include "utilities/FixedOrderEventLog.h"
#include "xamples/utilities/Xample_ReturnValues.h"

extern std::string runFileNamePrefix;
extern ptime stampTime;
extern ptime currentEventTime;
extern int   worldState;

using namespace twitter;

SimpleTwitterScenario::SimpleTwitterScenario(boost::mpi::communicator* world,
		Xample_Model* model, repast::Properties* properties): BaseScenario<Xample_Model, TwitterAgent, TwitterUserAgent>(model, properties),
				behaviorSelectors(), staticAgentList(0), mapOfInfoIDsPerContentNodes(0), mapOfDatesCreated(0){

	LOG_INFO("twitter","Constructing Twitter Scenario");

	// Create the platform
	twitter = new Twitter(world, this);

	// Initialize output files
	//twitter->initializeOutputFiles(runFileNamePrefix, "SimpleTwitter", properties);

	std::string tw_outFilename = "output/" + runFileNamePrefix + "-" + twitter->getPlatformAbbreviation() + "-SimpleTwitter";

	Twitter_FileWriter* fileWriter_json = new Twitter_FileWriter(Twitter_Xamples_FileOutputTypes::TWITTER_XAMPLES_FILEOUTPUT_JSON, tw_outFilename + ".json");
	twitter->fileWriters["TWITTER.JSON"] = fileWriter_json;

	Twitter_FileWriter* fileWriter_csv = new Twitter_FileWriter(Twitter_Xamples_FileOutputTypes::TWITTER_XAMPLES_FILEOUTPUT_CSV, tw_outFilename + ".csv");
	twitter->fileWriters["TWITTER.CSV"] = fileWriter_csv;


	LOG_INFO("twitter", "<SimpleTwitterScenario Constructor> Creating Twitter Info Store:");
	infoIDStore = new InfoStore();

	infoIDStrategy = getPropertyWithDefault<std::string>("info.id.strategy", "none", props);
	if(infoIDStrategy == "eventcount"){
		if(actingAgentListStrategy != "infoIDEventDriven" && actingAgentListStrategy != "infoIDEventDrivenWeighted"){
			CORE_LOG_WARN("twitter", " Mismatch detected: InfoID strategy = " + infoIDStrategy + " and acting agent list strategy = " + actingAgentListStrategy);
		}
	}

	limitToInfoIDList = getPropertyWithDefault<bool>("limit.info.ids.to.list", true, props);

	LOG_INFO("twitter", "Twitter Scenario: Completed Constructor");
}


SimpleTwitterScenario::~SimpleTwitterScenario(){
	delete twitter;
}

TwitterEventType SimpleTwitterScenario::getEventType(std::string actionName){
	TwitterEventType whichType = IDLE;
	if      (actionName == "tweet")    whichType = TWEET;
	else if (actionName == "retweet")  whichType = RETWEET;
	else if (actionName == "quote")    whichType = QUOTE;
	else if (actionName == "reply")    whichType = REPLY;

	return whichType;
}

void SimpleTwitterScenario::initScenario(){
	std::string initFilename = getPropertyWithDefault<std::string>("event.data", "" , props);
	LOG_INFO("twitter", "Twitter Scenario: Running initScenario() with event file " + initFilename);
	if(initFilename.length() > 0 && initFilename != "__NONE__"){
		// Load users
		loadAgentIDsFromCrossPlatformCSVFile(initFilename, 0, *userNames,    "twitter", earliestEventTime, lastInitEventTime);

		// Create data structures for capture during pass to load content
		mapOfCreatorIDsPerContentNodes = new std::map<std::string, std::string>();
		mapOfInfoIDsPerContentNodes    = new std::map<std::string, std::set<std::string>>();
		mapOfDatesCreated              = new std::map<std::string, ptime>();

		// Load content
		loadAgentIDsFromCrossPlatformCSVFile(initFilename, 3, *contentNames, "twitter", earliestEventTime, lastInitEventTime, "", "?", 2, "tweet", mapOfCreatorIDsPerContentNodes, mapOfInfoIDsPerContentNodes, mapOfDatesCreated);


		// Scrape the infoIDs for the infIDStore
		std::map<std::string, std::set<std::string>>::iterator infoIDs = mapOfInfoIDsPerContentNodes->begin();
		while(infoIDs != mapOfInfoIDsPerContentNodes->end()){
			std::set<std::string>::iterator infoIDIter = infoIDs->second.begin();
			while(infoIDIter != infoIDs->second.end()){
				getInfoIDStore()->add(*infoIDIter, 1);
				infoIDIter++;
			}
			infoIDs++;
		}

	}
	else{
		LOG_WARN("twitter", "         No filename provided for Cross Platform Event List: '" + initFilename + "'");
	}

	// Create the behavior selector
	behaviorSelectors.push_back(new TwitterDefaultBehaviorSelector());

	// Create the info ID Selector
	infoIDSelectors.push_back(new TW_PopularInfoIDSelector(twitter));

	LOG_INFO("twitter", "Agents Loaded:");
	LOG_INFO("twitter", "   Users:           " + to_string(userNames->size()));
	LOG_INFO("twitter", "   Tweets:          " + to_string(contentNames->size()));
	LOG_INFO("twitter", "Twitter Scenario: Completed initScenario()");
}


void SimpleTwitterScenario::initUserAgents(){
	LOG_INFO("twitter", "Twitter Scenario: Running initUserAgents(): Users = " + to_string(userNames->size()));
	LOG_INFO("twitter", "Twitter Scenario: Creating user instances");

	TwitterAgentPackage dummyPackage(USER_AGENT);
	std::map<std::string, TwitterAgent*>::iterator iter = userNames->begin();
	int count = 0;
	while(iter != userNames->end()){
		TwitterAgent* agent = twitter->createAndAddNewAgent(dummyPackage);
		agent->setHashTagID((*iter).first);
		(*iter).second = agent;
		count++;
		iter++;
	}

	LOG_INFO("twitter", "Twitter Scenario: Completed initUserAgents(): Created " + std::to_string(count));
}

void SimpleTwitterScenario::initContentAgents(){
	LOG_INFO("twitter", "Twitter Scenario: Running initContentAgents(): Content = " + to_string(contentNames->size()));
	std::map<std::string, TwitterAgent*>::iterator iter = contentNames->begin();
	int count = 0;
	while(iter != contentNames->end()){
		TwitterAgentPackage dummyPackage(CONVERSATION_AGENT);

		// Add creator
		std::map<std::string, std::string>::iterator creator = mapOfCreatorIDsPerContentNodes->find((*iter).first);
		if(creator != mapOfCreatorIDsPerContentNodes->end()){
			std::map<std::string, TwitterAgent*>::iterator cr = userNames->find(creator->second);
			if(cr != userNames->end()) dummyPackage.creatorID = cr->second->getId();
		}

		// Add the date/time created
		std::map<std::string, ptime>::iterator datesCreated = mapOfDatesCreated->find((*iter).first);
		if(datesCreated != mapOfDatesCreated->end()) dummyPackage.dateCreated = datesCreated->second;

		// Add information IDs

		TwitterConversationAgent* agent = (TwitterConversationAgent*)(twitter->createAndAddNewAgent(dummyPackage));
		agent->setHashTagID((*iter).first);
		(*iter).second = agent;

		count++;
		iter++;
	}

	LOG_INFO("twitter", "Twitter Scenario: Completed initContentAgents(): created " + to_string(count) + " conversations");
}


void SimpleTwitterScenario::initUserToContentLinks(){
	std::string initFilename    = props->getProperty("event.data");
	std::string agentDataSource = props->getProperty("agent.data.source");

	LOG_INFO("twitter", "Twitter Scenario: Running initUserToContentLinks() with file: '" + initFilename + "'");

	if(initFilename.length() == 0){
		LOG_INFO("twitter", "        No input file specified; omitting.");
		return;
	}

	std::ifstream  initData(initFilename);
	if (!initData.good()){
		LOG_ERROR("twitter", "EXITING: Problem in initialization file: " + initFilename);
		exit(TW_UNSPECIFIED_USER_TO_CONTENT_ISSUE);
	}

	std::string line;
	if (!std::getline(initData, line)){
		LOG_ERROR("twitter", "EXITING: First line is blank, filename: "  + initFilename);
		exit(TW_USER_TO_CONTENT_FIRST_LINE_BLANK);
	}

	int l = 0;
	int platformLines = 0;
	int count = 0;
	int eventsBeforeEarliest = 0;
	int eventsAfterLast = 0;

	int missingUsers   = 0;
	int missingContent = 0;

	long countOfOther = 0;

	bool fileValid = true;
	while(fileValid){
		fileValid = (fileValid && std::getline(initData,line)); // Will be 'true' until a blank line marking eof
		if(fileValid){
			std::vector<std::string> vals;
			boost::split(vals, line, boost::is_any_of(","));
			if(vals[fixedOrderEventLog.platformColumn] == "twitter"){
				boost::gregorian::date eventDate = parseTime(vals[fixedOrderEventLog.nodeTimeColumn]).date();

				if (eventDate >= earliestEventTime.date() && eventDate < lastInitEventTime.date()){
					bool doit = true;

					std::string agentID = vals[fixedOrderEventLog.nodeUserIDColumn];
					std::map<std::string, TwitterAgent*>::iterator userIter = userNames->find(agentID);
					if(userIter  == userNames->end()){
						missingUsers++;
						doit = false;
					}

					std::string contentID = vals[vals[fixedOrderEventLog.rootIDColumn] != "?" ? fixedOrderEventLog.rootIDColumn : fixedOrderEventLog.parentIDColumn];
					std::map<std::string, TwitterAgent*>::iterator contentIter = contentNames->find(contentID);
					if(contentIter  == contentNames->end()){
						LOG_DEBUG("twitter", " Could not find content " + contentID + " in file " + initFilename);
						missingContent++;
						doit = false;
					}

					if(doit){
						TwitterEventType whichType = getEventType(vals[fixedOrderEventLog.actionTypeColumn]);

						twitter->getEdge(userIter->second, contentIter->second)->logEvent(whichType);
						twitter->countEvent(whichType);

						count++;
					}
				}
				else {
					if(eventDate < earliestEventTime.date()) eventsBeforeEarliest++;
					else                                     eventsAfterLast++;
				}
				platformLines++;
			}
			l++;
		}
	}

	LOG_INFO("twitter", "TwitterScenario: Completed initUserToContentLinks():");
	LOG_INFO("twitter", "   " + to_string(l) + " lines ( + 1 header) ");
	LOG_INFO("twitter", "   " + to_string(platformLines) + " platform lines");
	LOG_INFO("twitter", "   " + to_string(count) + " edges ");
	LOG_INFO("twitter", "   " + to_string(missingUsers) + " missing users ");
	LOG_INFO("twitter", "   " + to_string(missingContent) + " missing content");
	LOG_INFO("twitter", "   " + to_string(eventsBeforeEarliest) + " before date range");
	LOG_INFO("twitter", "   " + to_string(eventsAfterLast) + " after date range");
	LOG_INFO("twitter", "   " + to_string(countOfOther) + " 'other'");

	std::stringstream ss;
	infoIDStore->report(ss);
	LOG_INFO("twitter", "INFO ID STORE: \n" + ss.str());
}



void SimpleTwitterScenario::initUserToUserLinks(){
	LOG_INFO("twitter", "TwitterScenario: Running initUserToUserLinks() NOT LOADED-OBSOLETED");

	LOG_INFO("twitter", "TwitterScenario: Completed initUserToUserLinks() NOT LOADED-OBSOLETED");
}


void SimpleTwitterScenario::performExogenousEvents(){
	LOG_INFO("twitter", "TwitterScenario: Running performExogenousEvents()");
	if(twitter->context.size() == 0){
		LOG_WARN("twitter",  "        No agents in context.");
		return;
	}
	std::set<TwitterAgent*> agents;
	std::set<TwitterAgent*> conversationAgents;
	twitter->context.selectAgents(repast::SharedContext<TwitterAgent>::LOCAL, agents);
	std::set<TwitterAgent*>::iterator iter = agents.begin();

	int totalAgents   = 0;
	while(iter != agents.end()){
		if((*iter)->getType() == CONVERSATION_AGENT) conversationAgents.insert(*iter);
		totalAgents++;
		(*iter)->updateScore(TWITTER_XAMPLES_DEFAULT_SCORE); // All agents have their scores updated
		iter++;
	}


	LOG_INFO("twitter", "----- FITNESS UPDATES --- ");
	LOG_INFO("twitter", " Total agents: " + to_string(totalAgents) + " ");
	int nToInclude = agentsPerStep > 30 ? agentsPerStep : 30;

//	// Update the platform's central feed
//	LOG_INFO("twitter", "        Updating current fittest feed to fittest " + to_string(nToInclude) + " agents ");
//	twitter->updateCurrentFittestFeed(nToInclude);

	// Create new agents
	TwitterAgentPackage newPackage(USER_AGENT);

	if(repast::Random::instance()->nextDouble() < probabilityOfAddingNewAgents){
		LOG_INFO("twitter", "        Creating " + to_string(newAgentsPerStep) + " new agents.");
		for(int i = 0; i < newAgentsPerStep; i++){
			TwitterUserAgent* agent = (TwitterUserAgent*) (twitter->createAndAddNewAgent(newPackage));
		}
		LOG_INFO("twitter", "        Done creating " + to_string(newAgentsPerStep) + " new agents." );
	}

	updateShocks();

	if(stampTime.is_not_a_date_time()){
		if(!eventsPerInfoIDFixedIntervals.empty()){
			currentTickInfoIDs = {eventsPerInfoIDFixedIntervals.at(infoIDeventsIndex)->begin(), eventsPerInfoIDFixedIntervals.at(infoIDeventsIndex)->end()};
		if(++infoIDeventsIndex == eventsPerInfoIDFixedIntervals.size())
			infoIDeventsIndex = 0;
		}
	}
	else{
		if(!eventCountPerInfoID.empty()){
			map<ptime, map<string, int>>::iterator mapItr = eventCountPerInfoID.find(stampTime);
			if(mapItr != eventCountPerInfoID.end()){
				currentTickInfoIDs = {mapItr->second.begin(), mapItr->second.end()};
			}
		}
	}

	LOG_INFO("twitter", "TwitterScenario: Completed performExogenousEvents");

}


void SimpleTwitterScenario::performAgentActions(){
	LOG_INFO("twitter", "Twitter Scenario: Running performAgentActions()");

	// The standard model for this is:
	std::vector<TwitterAgent*> agents;
	if(twitter->context.size() == 0){
		LOG_WARN("twitter", "        No agents in context.");
		return; // No agents to do anything.
	}
	else{
		LOG_INFO("twitter", "        Context size is " + to_string(twitter->context.size()) + " and strategy is " + actingAgentListStrategy);
	}

	if(      actingAgentListStrategy == "All"){
		if(staticAgentList == 0){
			selectAllAgents(agents);
			staticAgentList = new std::vector<TwitterAgent*>();
			staticAgentList->insert(staticAgentList->begin(), agents.begin(), agents.end());
		}
		else{
			//selectAllAgents(agents);
		}
	}
	else if( actingAgentListStrategy == "Fraction"){
		selectFractionOfAgents(agentFraction, agents);
	}
	else if( actingAgentListStrategy == "Absolute"){
		double shockCount = this->getShocks().size();
    		double shockEffect = getShockEffect(shockCount);
		//std::cout << " TW Shock Count = " << shockCount << " effect " << shockEffect << std::endl;
		double agentsToAct = (agentsPerStep > 0 ?
				agentsPerStep :
				(shockEffect > 1 ?
						1 :
						agentsPerStep));
		selectNAgents(agentsToAct * shockEffect, agents);
	}
	else if( actingAgentListStrategy == "Weighted"){
		double shockCount = this->getShocks().size();
		double shockEffect = getShockEffect(shockCount);
		double agentsToAct = (agentsPerStep > 0 ?
				agentsPerStep :
				(shockEffect > 1 ?
						1 :
						agentsPerStep));
		selectNAgentsWeighted(agentsToAct * shockEffect, agents);
	}
	else if (actingAgentListStrategy == "Driven"){
		double shockCount = this->getShocks().size();
		double shockEffect = getShockEffect(shockCount);
		long agentsPerStepTemp = (agentEventCountTimeSeries.empty()) ?
					  1 : agentEventCountTimeSeries.at(timeSeriesIndex++);
		if(timeSeriesIndex == agentEventCountTimeSeries.size())
			timeSeriesIndex = 0;
		double agentsToAct = (agentsPerStepTemp > 0 ?
				agentsPerStepTemp :
				(shockEffect > 1 ?
						1 :
						agentsPerStepTemp));
		selectNAgents(agentsToAct * shockEffect, agents);
	}
	else if (actingAgentListStrategy == "drivenWeighted"){
		double shockCount = this->getShocks().size();
		double shockEffect = getShockEffect(shockCount);
		long agentsPerStepTemp = (agentEventCountTimeSeries.empty()) ?
					  1 : agentEventCountTimeSeries.at(timeSeriesIndex++);
		if(timeSeriesIndex == agentEventCountTimeSeries.size())
			timeSeriesIndex = 0;
		double agentsToAct = (agentsPerStepTemp > 0 ?
				agentsPerStepTemp :
				(shockEffect > 1 ?
						1 :
						agentsPerStepTemp));
		selectNAgentsWeighted(agentsToAct * shockEffect, agents);
	}
	else if (actingAgentListStrategy == "infoIDEventDriven"){
		if(infoIDStrategy != "eventcount"){
			CORE_LOG_WARN("twitter", " Mismatch detected: InfoID strategy = " + infoIDStrategy + " and acting agent list strategy = " + actingAgentListStrategy);
		}

		long agentsPerStepTemp = 0;
		for(auto iter = currentTickInfoIDs.begin(); iter != currentTickInfoIDs.end(); ++iter){
			if(Scenario::minimizeAgentsForInfoIDs) agentsPerStepTemp = std::max(agentsPerStepTemp, long(iter->second));
			else                                   agentsPerStepTemp += long(iter->second);
		}

		LOG_INFO("twitter", " NUMBER OF AGENTS infoIDEventDriven: " + to_string(agentsPerStepTemp));
		// The number of agents to act will be equal to the largest number
		selectNAgents(agentsPerStepTemp, agents);
		LOG_INFO("twitter", " NUMBER OF AGENTS SELECTED: " + to_string(agents.size()));
	}
	else if (actingAgentListStrategy == "infoIDEventDrivenWeighted"){
		if(infoIDStrategy != "eventcount"){
			CORE_LOG_WARN("twitter", " Mismatch detected: InfoID strategy = " + infoIDStrategy + " and acting agent list strategy = " + actingAgentListStrategy);
		}

		long agentsPerStepTemp = 0;
		for(auto iter = currentTickInfoIDs.begin(); iter != currentTickInfoIDs.end(); ++iter){
			if(Scenario::minimizeAgentsForInfoIDs) agentsPerStepTemp = std::max(agentsPerStepTemp, long(iter->second));
			else                                   agentsPerStepTemp += long(iter->second);
		}

		LOG_INFO("twitter", " NUMBER OF AGENTS infoIDEventDrivenWeighted: " + to_string(agentsPerStepTemp));
		// The number of agents to act will be equal to the largest number
		selectNAgentsWeighted(agentsPerStepTemp, agents);
		LOG_INFO("twitter", " NUMBER OF AGENTS SELECTED: " + to_string(agents.size()));
	}
	else{
		// Default is just do one agent- should not be used
		twitter->context.selectAgents(repast::SharedContext<TwitterAgent>::LOCAL, 1, agents, USER_AGENT);
	}

	int c = 0;

	std::vector<TwitterAgent*>::iterator it    = agents.begin();
	std::vector<TwitterAgent*>::iterator itEnd = agents.end();

//	if(staticAgentList != 0){
//		LOG_INFO("twitter", " Using static list, which has " + to_string(staticAgentList->size()) + " agents.");
//		it = staticAgentList->begin();
//		itEnd = staticAgentList->end();
//		LOG_INFO("twitter", "        Twitter agents selected to act (static): " + to_string(staticAgentList->size()));
//	}
//	else{
		LOG_INFO("twitter", "        Twitter agents selected to act (listsize): " + to_string(agents.size()));
//	}

	LOG_INFO("twitter", "   Agents selecting behaviors....");
	while(it != itEnd){
		TwitterUserAgent* agent = (TwitterUserAgent*)(*it);
		LOG_INFO("twitter", "   Agents selecting behaviors 1....");
		agent->doSelectBehavior(); // Sets one or more behaviors into the agent's actions list
		LOG_INFO("twitter", "   Agents selecting behaviors 2....");
		InfoID* info = getPopularInfoID(0, 1);
		if (info != 0) {
			agent->actions.begin()->payload.informationIDs.insert(info);
			LOG_INFO("twitter", "Inserted " + info->toString());
		}
		LOG_INFO("twitter", "   Agents selecting behaviors 3....");
		agent->doAllActions();
		LOG_INFO("twitter", "   Agents selecting behaviors 4....");
		it++;
    }
	LOG_INFO("twitter", "Completed performAgentActions");
}


void SimpleTwitterScenario::setLocalPointers(TwitterAgent* agent){
	 if(agent->getType() == USER_AGENT){
    	  TwitterUserAgent* u_agent = (TwitterUserAgent*)agent;
    	  u_agent->behaviorSelector = behaviorSelectors[0];
    	  u_agent->infoIDSelector   = infoIDSelectors[0];
    }
}



void SimpleTwitterScenario::selectAllAgents(std::vector<TwitterAgent*>& agents, TwitterAgentType agentType){
	twitter->context.selectAgents(repast::SharedContext<TwitterAgent>::LOCAL, agents, (int)agentType);
}
void SimpleTwitterScenario::selectFractionOfAgents(double frac, std::vector<TwitterAgent*>& agents){
	int n = (int)(frac * twitter->context.size());
	twitter->context.selectAgents(repast::SharedContext<TwitterAgent>::LOCAL, n, agents, USER_AGENT);
}
void SimpleTwitterScenario::selectNAgents(int n, std::vector<TwitterAgent*>& agents){
	twitter->context.selectAgents(repast::SharedContext<TwitterAgent>::LOCAL, n, agents, USER_AGENT);
}
void SimpleTwitterScenario::selectNAgentsWeighted(int n, std::vector<TwitterAgent*>& agents){
	std::set<TwitterAgent*>* agentsTempPtr = new std::set<TwitterAgent*>();
	std::set<TwitterAgent*>& agentsTemp = *agentsTempPtr;
	twitter->context.selectAgents(repast::SharedContext<TwitterAgent>::LOCAL, agentsTemp, USER_AGENT);

	std::vector<TwitterAgent*> fitAgents;
	std::vector<TwitterAgent*> zeroFitnessAgents;

	ParameterizedWeightedSelector pws = PWSFactory::getInstance()->getParameterizedWeightedSelector("twitter", "user");
	SimpleWeightedSelector sws(-1);
	std::set<TwitterAgent*>::iterator iter = agentsTemp.begin();
	while(iter != agentsTemp.end()){
		double f = 0; //(*iter)->getFitness();
		if(pws.getConvertedValue(f) == 0){
			zeroFitnessAgents.push_back((*iter));
			sws.addValue(1); // Everything is weighted equally
		}
		else{
			fitAgents.push_back((*iter));
			pws.addValue(f); // get something about the agent...
		}
		iter++;
	}
	LOG_DEBUG("twitter", "SELECT_N_WEIGHTED: n = " + to_string(n) + "; sws = "  + to_string(sws.size()) + "; pws = " + to_string(pws.size()) + "; agentsTemp = " + to_string(agentsTemp.size()) + "; pws(0) = "  + to_string(pws.getCountOfElementsWithZeroValues()));
	delete agentsTempPtr; // don't need this anymore

	std::set<TwitterAgent*> countSet;
	LOG_DEBUG("twitter", "About to select " + to_string(n) + " agents; number of elements in weighted selector is " + to_string(pws.size()) + " with " + to_string(pws.getCountOfElementsWithZeroValues()) + " = 0");
	while(countSet.size() < n && pws.getSum() > 0){
		int indx = pws.select();
		TwitterAgent* toInsert = fitAgents.at(indx);
		pws.clearAt(indx);
		pair<std::set<TwitterAgent*>::iterator, bool> ret = countSet.insert(toInsert);
		if(ret.second) agents.push_back(toInsert);
	}

	if(useZeroFitnessAgents){
		int zerosUsed = 0;
		while(countSet.size() < n && sws.getSum() > 0){
			int indx = sws.select();
			TwitterAgent* toInsert = zeroFitnessAgents.at(indx);
			sws.clearAt(indx);
			pair<std::set<TwitterAgent*>::iterator, bool> ret = countSet.insert(toInsert);
			if(ret.second){
				agents.push_back(toInsert);
				zerosUsed++;
			}
		}
		if(zerosUsed > 0) LOG_WARN("twitter", "Used " + to_string(zerosUsed) + " zero-fitness agents (out of sws of " + to_string(sws.size()) + ") to complete set of " + to_string(countSet.size()) + " for request of " + to_string(n));
	}

}

void SimpleTwitterScenario::cleanupInit(){
	LOG_INFO("twitter", "TwitterScenario::CleanupInit");
	delete mapOfInfoIDsPerContentNodes;
	delete mapOfCreatorIDsPerContentNodes;
}




