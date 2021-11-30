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
/* Scenario.h */
#ifndef INCLUDE_SCENARIO_H
#define INCLUDE_SCENARIO_H

#include <string>
#include <sstream>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>

#include <boost/algorithm/string.hpp>
#include "boost/date_time/posix_time/posix_time.hpp"
#include "boost/date_time/posix_time/posix_time_io.hpp"
#include "boost/date_time/gregorian/gregorian.hpp" //include all types plus i/o

#include "repast_hpc/Properties.h"
#include "repast_hpc/logger.h"

#include "utilities/Utilities.h"
#include "utilities/FixedOrderEventLog.h"
#include "utilities/ReturnValues.h"
#include "socialnetwork_toolkit/InfoStore.h"
#include "socialnetwork_toolkit/InfoID.h"

using namespace boost::posix_time;

typedef std::map<boost::gregorian::date, std::map<std::string, double> > shockMap;

namespace rhpc_smple {

class Scenario{

private:
	std::vector<Scenario*>                                      subscenarios;

protected:
	shockMap*                                                   _shocks;
	std::map<std::string, double>*                              thisDaysShocks;
	std::set<std::string>                                       shocksDuringSim;
	repast::Properties*                                         props;
	std::map<std::string, int> 	                                currentTickInfoIDs;
    std::string 				                                infoIDStrategy;
	int 						                                infoIDeventsIndex;


	// For event-driven scheduling
    std::vector<std::unique_ptr<std::map<std::string, long> > > eventsPerInfoIDFixedIntervals; 	// Depending on the real-time or fixed time intervals,
    std::map<ptime, std::map<std::string, int> >                eventCountPerInfoID;            // only one of these containers will be used.
    bool                                                        minimizeAgentsForInfoIDs;


public:
	bool                                                        writeOnlyCascades;
	InfoStore*                                                  infoIDStore;
	InfoStore*                                                  shockBasedInfoStore;
	InfoStore*                                                  communitiesStore;
	Scenario*                                                   parentScenario;

private:
	void init_scenario();
	void init_user_agents();
	void init_content_agents();
	void init_user_to_content_links();
	void init_user_to_user_links();
	void init_presimulation_actions();
	void do_presimulation_actions();


protected:

	void loadInfoIDEventCountFixedIntervals(std::string filename, std::vector<std::unique_ptr<std::map<std::string, long> > >& eventsPerInfoIDFixedIntervals, int simulationTimeInterval);
	void loadInfoIDEventCount(std::string filename, std::map<ptime, std::map<std::string, int> >& eventsPerInfoID, int simulationTimeInterval);

public:

	static void writeMetadataFile(repast::Properties* props);

	Scenario(repast::Properties* properties);
	virtual ~Scenario();

	virtual void addSubscenario(std::string prefix, Scenario* subscenario);
	void propagatePropertiesDownward(std::string prefix, repast::Properties* props);

	void init();

	virtual void initScenario(){}
	virtual void initUserAgents(){}
	virtual void initContentAgents(){}
	virtual void initUserToContentLinks(){}
	virtual void initUserToUserLinks(){}
	virtual void initPresimulationActions(){}
	virtual void cleanupInit(){};               // Clean up items on the heap that are only used during initialization

	//void sync();
	void perform_exogenous_events();
	void perform_agent_actions();

	virtual void doPresimulationActions(){}

	virtual void synchronize(){}
	virtual void performExogenousEvents(){}
	virtual void performAgentActions(){}
	virtual void recordResults(){}

	virtual void resyncNetworkTotals(){}
	virtual void reportNetwork(){}

	void loadShocks(std::string initFilename);

	void updateShocks();
	std::map<std::string, double>& getShocks();

	virtual void debugInit(){};

	virtual InfoStore* getInfoIDStore(int infoStoreType = 0){
		return infoStoreType == 0 ? infoIDStore : 0;
	}

	virtual InfoID* getPopularInfoID(long weightToAdd){
		return getPopularInfoID(0, weightToAdd);
	}


	virtual InfoID* getPopularInfoID(int infoStoreType, long weightToAdd ){
		InfoStore* temp = getInfoIDStore(infoStoreType);
		return temp != 0 ?
				temp->pickAndVote(weightToAdd) :
				parentScenario != 0 ?
						parentScenario->getPopularInfoID(infoStoreType, weightToAdd): 0;
	}

	virtual InfoID* getShockedInfoID(long weightToAdd = 0){
		return shockBasedInfoStore != 0 ?
						shockBasedInfoStore->pickAndVote(weightToAdd) :
						parentScenario != 0 ?
								parentScenario->getShockedInfoID(weightToAdd): 0;
	}

	virtual std::vector<InfoID*> getEventCountInfoIDs(){
		std::vector<InfoID*> eventCountInfoIDs;

		for(auto iter = currentTickInfoIDs.begin(); iter != currentTickInfoIDs.end(); ++iter){
			if(iter->second){
				InfoID* infoIDToAdd = infoIDStore->find(iter->first);
				if(infoIDToAdd){
					eventCountInfoIDs.push_back(infoIDToAdd);
					iter->second--;
					if(!minimizeAgentsForInfoIDs) return eventCountInfoIDs;
				}
			}
		}
	    return eventCountInfoIDs;
	}

	virtual InfoID* find(std::string newInfoIDName){
		return infoIDStore != 0 ?
				infoIDStore->find(newInfoIDName) :
				parentScenario != 0 ?
						parentScenario->find(newInfoIDName): 0;
	}
	
	virtual void addShockToGlobalInventory(std::string shock){
		if(parentScenario != 0) parentScenario->addShockToGlobalInventory(shock);
		else shocksDuringSim.insert(shock);
	}
	
	virtual bool shockIsInGlobalInventory(std::string shock){
		if(parentScenario != 0) return parentScenario->shockIsInGlobalInventory(shock);
		else return shocksDuringSim.find(shock) != shocksDuringSim.end();
	}
	
	virtual int countOfShocksInGlobalInventory(){
		if(parentScenario != 0) return parentScenario->countOfShocksInGlobalInventory();
		else return shocksDuringSim.size();
	}
	
};


template<typename AGENT>
class ScenarioWithAgents: public Scenario{

private:
	std::vector<ScenarioWithAgents<AGENT>*> subscenariosWithAgents;

protected:
	std::map<std::string, std::string>      userNameConcordance;
	std::vector<long>						agentEventCountTimeSeries;
	int 									timeSeriesIndex;
    FixedOrderEventLog                      fixedOrderEventLog;

public:
	int                                     maxAgents;


public:
	ScenarioWithAgents(repast::Properties* properties);
	~ScenarioWithAgents();

	using Scenario::addSubscenario;
	virtual void addSubscenario(std::string prefix, ScenarioWithAgents<AGENT>* subscenarioWithAgents);

	void set_local_pointers(AGENT* agent);

	virtual void setLocalPointers(AGENT* agent){}

	virtual void loadAgentIDsFromCSVFile(std::string file, int col,
			std::map<std::string, AGENT*>& map, std::string prefix = "");

	virtual void loadAgentIDsFromCrossPlatformCSVFile(std::string file, int col,
			std::map<std::string, AGENT*>& map, std::string platformID, boost::posix_time::ptime earliestEventTime, boost::posix_time::ptime lastInitEventTime,
			std::string prefix = "", std::string missingFlag="", int altColumn = 0,
			std::string creationAction = "",
			std::map<std::string, std::string>* mapOfCreatorIDs = 0,
			std::map<std::string, std::set<std::string>>* mapOfInfoIDs = 0,
			std::map<std::string, ptime>* mapOfDatesCreated = 0,
			std::map<std::string, std::string>* mapOfRootIDs = 0);

	virtual void createAgentsFromInputNameMap(std::map<std::string, AGENT*>& map);

	virtual AGENT* createDefaultAgent(){ return 0; };

	virtual void setMaxAgents(int newVal){ maxAgents = newVal; }

	virtual void debugInit(){};

	void populateUserNameConcordance(std::string filename);

	virtual void populateCommunityInfo(std::string filename, int idCol, int communityIDcol,
			std::map<std::string, InfoID*>& nameToInfoIDMap, InfoStore* infoStore,
			std::string platformID, int platformCol);

	virtual void loadTimeSeriesFromPlatformCSVFile(std::string filename, std::vector<long>& eventStore, int simulationTimeInterval);

};

template<typename AGENT>
ScenarioWithAgents<AGENT>::ScenarioWithAgents(repast::Properties* properties): Scenario(properties), maxAgents(-1), timeSeriesIndex(0){}

template<typename AGENT>
ScenarioWithAgents<AGENT>::~ScenarioWithAgents(){}

template<typename AGENT>
void ScenarioWithAgents<AGENT>::addSubscenario(std::string prefix, ScenarioWithAgents<AGENT>* subscenarioWithAgents){
	Scenario::addSubscenario(prefix, subscenarioWithAgents);          // Call the parent class, which this method shadows...
	subscenariosWithAgents.push_back(subscenarioWithAgents);
}

template<typename AGENT>
void ScenarioWithAgents<AGENT>::set_local_pointers(AGENT* agent){
	setLocalPointers(agent);
	for(int i = 0; i < subscenariosWithAgents.size(); i++) subscenariosWithAgents[i]->set_local_pointers(agent);
}


template<typename AGENT>
void ScenarioWithAgents<AGENT>::loadAgentIDsFromCSVFile(std::string initFilename, int col,
		std::map<std::string, AGENT*>& map, std::string prefix){
	CORE_LOG_INFO("init", "<ScenarioWithAgents> Loading agent IDs from " + initFilename + " column " + std::to_string(col));

	int originalSize = map.size();

	std::ifstream  initData(initFilename);
	std::string line;
	if (!initData.good()){ 
		LOG_ERROR("init", "EXITING: Problem in initialization file: " + initFilename);
		exit(UNSPECIFIED_INIT_FILE_ISSUE_A); 
	}
	if (!std::getline(initData,line)){ 
		LOG_ERROR("init", "EXITING: First line is blank, filename: "  + initFilename);
		exit(INIT_FILE_FIRST_LINE_BLANK_A); 
	}

	bool fileValid = true;
	AGENT* temp = 0;
	int lineCount = 0;
	std::string lastNew = "";
	std::string lastSkipped = "";
	int skipped = 0;
	int useMax = maxAgents > 0 ? maxAgents : 20000000;
	while(fileValid && map.size() < maxAgents){
		fileValid = (fileValid && std::getline(initData,line)); // Will be 'true' until a blank line marking eof
		if(fileValid){
			std::vector<std::string> vals;
			boost::split(vals, line, boost::is_any_of(","));
			std::stringstream key;
			key << prefix << vals[col];

			std::string userID = key.str();
			if(userID.length() > 1){ // This is a hack because some of the data are invalid!
				int o = map.size();
				map[userID] = temp;
				if(map.size() != o) lastNew = userID;
			}
			else{
				lastSkipped = userID;
				skipped++;
			}
			lineCount++;
		}
	}

	initData.close();

	CORE_LOG_INFO("init", "<ScenarioWithAgents> Scenario Read from '" + initFilename + "' complete: ");
	CORE_LOG_INFO("init", "   Lines:           " + std::to_string(lineCount));
	CORE_LOG_INFO("init", "   Original Size:   " + std::to_string(originalSize));
	CORE_LOG_INFO("init", "   New:             " + std::to_string(map.size() - originalSize));
	CORE_LOG_INFO("init", "   Unique Agents:   " + std::to_string(map.size()));
	CORE_LOG_INFO("init", "   Last New Added:  '" + lastNew);
	CORE_LOG_INFO("init", "   Last Skipped (of " + std::to_string(skipped) + "):    '" + lastSkipped + "'");
}


template<typename AGENT>
void ScenarioWithAgents<AGENT>::loadAgentIDsFromCrossPlatformCSVFile(std::string initFilename, int col,
		std::map<std::string, AGENT*>& map, std::string platformID, boost::posix_time::ptime earliestEventTime,
		boost::posix_time::ptime lastInitEventTime, std::string prefix, std::string missingFlag, int altColumn,
		std::string creationAction,
		std::map<std::string, std::string>* mapOfCreatorIDs,
		std::map<std::string, std::set<std::string>>* mapOfInfoIDs,
		std::map<std::string, ptime>* mapOfDatesCreated,
		std::map<std::string, std::string>* mapOfRootIDs){
	CORE_LOG_INFO("init", "<ScenarioWithAgents> Loading agent IDs from " + initFilename + " column " + std::to_string(col) + " from lines '" + platformID + "'");

	int originalSize = map.size();

	std::ifstream  initData(initFilename);
	std::string line;
	if (!initData.good()){ 
		LOG_ERROR("init", "EXITING: Problem in initialization file: " + initFilename);
		exit(UNSPECIFIED_INIT_FILE_ISSUE_A); 
	}
	if (!std::getline(initData,line)){ 
		LOG_ERROR("init", "EXITING: First line is blank, filename: "  + initFilename);
		exit(INIT_FILE_FIRST_LINE_BLANK_A); 
	}

	bool fileValid = true;
	AGENT* temp = 0;
	int lineCount = 0;
	int platformLineCount = 0;
	std::string lastNew = "";
	std::string lastSkipped = "";
	int skipped = 0;
	int useMax = maxAgents > 0 ? maxAgents : 20000000;
	int eventsBeforeEarliest = 0;
   int eventsAfterLast = 0;
	int preInformationCollected = 0;
	while(fileValid && map.size() < maxAgents){
		fileValid = (fileValid && std::getline(initData,line)); // Will be 'true' until a blank line marking eof
		if(fileValid){
			std::vector<std::string> vals;
			boost::split(vals, line, boost::is_any_of(","));
			if(vals[fixedOrderEventLog.platformColumn] == platformID){
				platformLineCount++;
				std::stringstream key;
				int colu = vals[col] != missingFlag ? col : altColumn;
				key << prefix << vals[colu]; // Weird, but checks if the original column equals the 'missing' flag and switchs to the alternate column if it does.
				std::string userID = key.str();

				std::string timestr =  vals[fixedOrderEventLog.nodeTimeColumn];
				std::replace(timestr.begin(), timestr.end(),'T',' ');
				timestr.erase(std::remove(timestr.begin(), timestr.end(), 'Z'), timestr.end());
				boost::posix_time::ptime eventTime(time_from_string(timestr));
				boost::gregorian::date eventDate = eventTime.date();


				// If it's too early, we don't record it in full, but we may need to scrape some information
				// Not all of this will be used, but we can collect it all
				if(eventDate < earliestEventTime.date()){
					if(userID.length() > 1){
						if(mapOfInfoIDs != 0){
							if(vals[fixedOrderEventLog.infoIDColumn].length() > 0){
								preInformationCollected++;
								(*mapOfInfoIDs)[userID].insert(vals[fixedOrderEventLog.infoIDColumn]);
							}
						}
						if(mapOfDatesCreated != 0){
							ptime orig = (*mapOfDatesCreated)[userID]; // Note: this creates and adds the default, hence the check for default in the next line.
							if((orig.is_not_a_date_time()) || (eventTime < orig)) (*mapOfDatesCreated)[userID] = eventTime;
						}
					}
					eventsBeforeEarliest++;
				}
				else if(eventDate >= lastInitEventTime.date()){
					eventsAfterLast++;
				}
				else{
					if(userID.length() > 1){ // This is a hack because some of the data are invalid!
						int o = map.size();
						map[userID] = temp;
						if(map.size() != o){
							lastNew = userID;
						}
						if(mapOfCreatorIDs != 0 && ((creationAction == "any") || (vals[fixedOrderEventLog.actionTypeColumn] == creationAction))){
							(*mapOfCreatorIDs)[userID] = vals.at(fixedOrderEventLog.nodeUserIDColumn);
						}
						if(mapOfInfoIDs != 0){
							if(vals[fixedOrderEventLog.infoIDColumn].length() > 0){
								(*mapOfInfoIDs)[userID].insert(vals[fixedOrderEventLog.infoIDColumn]);
							}
						}
						if(mapOfDatesCreated != 0){
							ptime orig = (*mapOfDatesCreated)[userID]; // Note: this creates and adds the default, hence the check for default in the next line.
							if((orig.is_not_a_date_time()) || (eventTime < orig)) (*mapOfDatesCreated)[userID] = eventTime;
						}
						if(mapOfRootIDs != 0){
							(*mapOfRootIDs)[userID] = vals.at(fixedOrderEventLog.rootIDColumn);
						}
					}
					else{
						lastSkipped = userID;
						skipped++;
					}
				}
			}
			lineCount++;
		}
	}

	initData.close();

	CORE_LOG_INFO("init", "<ScenarioWithAgents> Scenario Read from '" + initFilename + "' (Cross-Platform) complete: ");
	CORE_LOG_INFO("init", "   Lines:                   " + std::to_string(lineCount));
	CORE_LOG_INFO("init", "   Original Size:           " + std::to_string(originalSize));
	CORE_LOG_INFO("init", "   New:                     " + std::to_string(map.size() - originalSize));
	CORE_LOG_INFO("init", "   Unique Agents:           " + std::to_string(map.size()));
	CORE_LOG_INFO("init", "   Last New Added:           '"+ lastNew + "'" );
	CORE_LOG_INFO("init", "   Last Skipped (of "         + std::to_string(skipped) + "):    '" + lastSkipped + "'");
	CORE_LOG_INFO("init", "   Events before earliest:  " + std::to_string(eventsBeforeEarliest));
	CORE_LOG_INFO("init", "   Events after last:       " + std::to_string(eventsAfterLast));
	CORE_LOG_INFO("init", "   Preinformation collected " + std::to_string(preInformationCollected));
}



template<typename AGENT>
void ScenarioWithAgents<AGENT>::populateCommunityInfo(std::string filename, int idCol, int communityIDcol,
		std::map<std::string, InfoID*>& nameToInfoIDMap, InfoStore* infoStore,
		std::string platformID, int platformCol){

	repast::Logger& logger = repast::Log4CL::instance()->get_logger("init");		
	std::stringstream msg;
	msg << "<ScenarioWithAgents> Loading community IDs from '" << filename << "'" << std::endl <<
		   "   Content ID column: " << idCol << std::endl <<
		   "   Community ID col:  " << communityIDcol << std::endl <<
		   "   Platform:          " << platformID << std::endl <<
		   "   Platform col:      " << platformCol;
	CORE_LOG_INFO("init", msg.str());

	int originalSize = nameToInfoIDMap.size();

	std::ifstream  initData(filename);
	std::string line;
	if (!initData.good()){ 
		LOG_ERROR("init", "EXITING: Problem in initialization file: " + filename);
		exit(UNSPECIFIED_INIT_FILE_ISSUE_A); 
	}
	if (!std::getline(initData,line)){ 
		LOG_ERROR("init", "EXITING: First line is blank, filename: "  + filename);
		exit(INIT_FILE_FIRST_LINE_BLANK_A); 
	}

	bool fileValid = true;
	int lineCount = 0;
	int platformLines = 0;
	int countOfInfoIDs = infoStore->size();
	while(fileValid){
		fileValid = (fileValid && std::getline(initData,line)); // Will be 'true' until a blank line marking eof
		if(fileValid){
			std::vector<std::string> vals;
			boost::split(vals, line, boost::is_any_of(","));
			if(vals.size() > 1){
				if(vals[platformCol] == platformID){
					platformLines++;

					std::string nodeName      = vals[idCol];
					std::string communityName = vals[communityIDcol];
					InfoID& community = infoStore->add(communityName); // Will add it if not there
					nameToInfoIDMap[nodeName] = &community;
				}
			}
			lineCount++;
		}
	}

	initData.close();


	countOfInfoIDs = infoStore->size() - countOfInfoIDs;
	CORE_LOG_INFO("init",  "<ScenarioWithAgents> Completed populating Community Info:");
	CORE_LOG_INFO("init", "   " + std::to_string(lineCount) + " lines");
	CORE_LOG_INFO("init", "   " + std::to_string(platformLines) + " platform lines");
	CORE_LOG_INFO("init", "   " + std::to_string(countOfInfoIDs) + " new info IDs added");
	CORE_LOG_INFO("init", "   " + std::to_string(nameToInfoIDMap.size()) + " names mapped");
}


template<typename AGENT>
void ScenarioWithAgents<AGENT>::createAgentsFromInputNameMap(std::map<std::string, AGENT*>& map){
	repast::Logger& logger = repast::Log4CL::instance()->get_logger("init");
	CORE_LOG_INFO("init", "<ScenarioWithAgents> Creating agents from name map");
	typename std::map<std::string, AGENT*>::iterator iter = map.begin();
	while(iter != map.end()){
		AGENT* agent = createDefaultAgent();
		agent->setHashTagID((*iter).first);
		(*iter).second = agent;
		iter++;
	}
}

template<typename AGENT>
void ScenarioWithAgents<AGENT>::populateUserNameConcordance(std::string filename){

	repast::Logger& logger = repast::Log4CL::instance()->get_logger("init");
	std::stringstream msg;
	msg << "<ScenarioWithAgents> Loading user concordances from " << filename;
	CORE_LOG_INFO("init", msg.str());

	int originalSize = userNameConcordance.size();

	std::ifstream  initData(filename);
	std::string line;
	if (!initData.good()){ 
		LOG_ERROR("init", "EXITING: Problem in initialization file: " + filename);
		exit(UNSPECIFIED_INIT_FILE_ISSUE_A); 
	}
	if (!std::getline(initData,line)){ 
		LOG_ERROR("init", "EXITING: First line is blank, filename: "  + filename);
		exit(INIT_FILE_FIRST_LINE_BLANK_A); 
	}

	bool fileValid = true;
	int lineCount = 0;
	while(fileValid){
		fileValid = (fileValid && std::getline(initData,line)); // Will be 'true' until a blank line marking eof
		if(fileValid){
			std::vector<std::string> vals;
			boost::split(vals, line, boost::is_any_of(":"));
			if(vals.size() > 1){
				std::vector<std::string> col1Vals;
				boost::split(col1Vals, vals[0], boost::is_any_of("\""));
				std::string orig = col1Vals[1];

				std::vector<std::string> col2Vals;
				boost::split(col2Vals, vals[1], boost::is_any_of("\""));
				std::string canon = col2Vals[1];

				userNameConcordance[orig] = canon;

			}
			lineCount++;
		}
	}

	initData.close();

	CORE_LOG_INFO("init", "<ScenarioWithAgents> Completed loading user concordance: ");
	CORE_LOG_INFO("init", "   " + std::to_string(lineCount) + " lines" );
	CORE_LOG_INFO("init", "   " + std::to_string(userNameConcordance.size() - originalSize) + " new names");
	CORE_LOG_INFO("init", "   " + std::to_string(userNameConcordance.size()) + " concordance size");
}

template<typename AGENT>
void ScenarioWithAgents<AGENT>::loadTimeSeriesFromPlatformCSVFile(std::string filename,std::vector<long>& seriesVector, int simulationTimeInterval){
	CORE_LOG_INFO("init", "<ScenarioWithAgents> Loading time series from " + filename);

	repast::Logger& logger = repast::Log4CL::instance()->get_logger("init");
	std::ifstream  eventData(filename);
	std::string line;
	if (!eventData.good()){ 
		LOG_ERROR("init", "EXITING: Problem in events sequence file: " + filename);
		exit(UNSPECIFIED_INIT_FILE_ISSUE_A); 
	}
	if (!std::getline(eventData,line)){ 
		LOG_ERROR("init", "EXITING: First line is blank, filename: "  + filename);
		exit(INIT_FILE_FIRST_LINE_BLANK_A); 
	}

	bool fileValid = true;

	ptime fileTimeStamp;

	while (fileValid){
		fileValid = (fileValid && std::getline(eventData,line));
		if(fileValid){
			std::vector<std::string> vals;
			boost::split(vals, line, boost::is_any_of(","));
			if(!fileTimeStamp.is_not_a_date_time()){
				time_duration fileTimeInterval = time_from_string(*vals.begin()) - fileTimeStamp;
				if(fileTimeInterval.total_seconds() != simulationTimeInterval){
					std::string fti = std::to_string(fileTimeInterval.total_seconds());
					LOG_ERROR("init", "EXITING: Events count file time interval (" +  fti  +"s) does not match with simulation time interval (" + std::to_string(simulationTimeInterval) + "s), filename: " + filename);
					exit(FILE_TIME_INTERVAL_MISMATCH);
				}
			}
			fileTimeStamp = time_from_string(vals.at(0));
			std::string agentsActing = vals.at(1);
			seriesVector.push_back(std::stod(agentsActing));
		}
	}

	eventData.close();

	CORE_LOG_INFO("init", "<ScenarioWithAgents> Events sequence read from '" + filename + "' complete: ");
}

}  // End Namespace rhpc_smple

#endif /* INCLUDE_SCENARIO_H */
