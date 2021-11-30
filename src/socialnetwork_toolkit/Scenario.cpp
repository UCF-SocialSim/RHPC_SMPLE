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
/* Scenario.cpp */
#include "socialnetwork_toolkit/Scenario.h"

using namespace boost::posix_time;
using namespace rhpc_smple;

extern std::string runFileNamePrefix;
extern std::string runMetadataTime;
extern ptime       currentEventTime;
extern ptime       nextExogEvent;
extern bool        okToUsePropertyDefault;

Scenario::Scenario(repast::Properties* properties): props(properties), writeOnlyCascades(false),
		_shocks(0), thisDaysShocks(0), infoIDStore(0), parentScenario(0), communitiesStore(0), shockBasedInfoStore(0),
		infoIDeventsIndex(0), minimizeAgentsForInfoIDs(true){
	_shocks = new std::map<boost::gregorian::date, std::map<std::string, double>>();
}

Scenario::~Scenario(){
	for(int i = 0; i < subscenarios.size(); i++) delete subscenarios[i];
	delete _shocks;
	delete infoIDStore;
	delete communitiesStore;
	delete shockBasedInfoStore;
}

void Scenario::writeMetadataFile(repast::Properties* props){
	if((props->getProperty("write.metadata.file") != "false")){
		// Write the metadata file
		std::ofstream  meta;
		std::stringstream metadataFilename;
		metadataFilename << "output/" << runFileNamePrefix << "-xMetadata.json";
		meta.open(metadataFilename.str(), std::ios::trunc);  // erase any existing file
		meta << "{" << std::endl
			<< "   \"time_executed\": \"" << runMetadataTime << "\"," << std::endl
			<< "   \"variables\": [";
		boost::transform_iterator<repast::KeyGetter, std::map<std::string, std::string>::const_iterator> iter = props->keys_begin();
		bool first = true;
		while(iter != props->keys_end()){
			if(first) first = false;
			else      meta << ",";
			meta << std::endl;
			std::string key = *iter;
			meta
					<< "       {" << std::endl
					<< "           \"name\": \"" << key << "\"," << std::endl
					<< "           \"value\": \"" << props->getProperty(key) << "\"" << std::endl
					<< "       }";
			iter++;
		}
		meta << "]," << std::endl;
		meta << "   \"model_name\": \"" << props->getProperty("model_name") << "\"" << std::endl
			<< "}" << std::endl;
		meta.close();
		okToUsePropertyDefault = false;
	}
}


void Scenario::addSubscenario(std::string prefix, Scenario* subscenario){
	// Copy all properties of the subscenario into the main properties file so that they are included in the metadata
	// Prepend them with the specified prefix
	repast::Properties::key_iterator iter = subscenario->props->keys_begin();
	while(iter != subscenario->props->keys_end()){
		//std::cout << *iter << std::endl;
		std::stringstream ss;
		ss << prefix << "__" << *iter;
		props->putProperty(ss.str(), subscenario->props->getProperty(*iter));
		iter++;
	}
	subscenario->parentScenario = this;
	if(subscenario->infoIDStore == 0) subscenario->infoIDStore = infoIDStore;
	subscenarios.push_back(subscenario);
}


void Scenario::propagatePropertiesDownward(std::string prefix, repast::Properties* subScenarioProps){
	// If you have any properties in your collection that are marked with the
	// prefix, push them down into the subscenario's properties (minus the prefix)
	std::stringstream s;
	s << prefix << "__";
	std::string compare = s.str();
	repast::Properties::key_iterator myPropIter = props->keys_begin();
	while(myPropIter != props->keys_end()){
		std::string propKey = *myPropIter;
		if((boost::starts_with(propKey, compare)) || (boost::starts_with(propKey, "MP__"))){
			std::string newKey = propKey.substr(compare.length()); // Should always be 4, but why hard-code it?
			subScenarioProps->putProperty(newKey, props->getProperty(propKey));
		}
		myPropIter++;
	}
}

void Scenario::init(){
	// Sim initialization
	CORE_LOG_INFO("init", "Running PARENT init:");
	CORE_LOG_INFO("init", "Running PARENT init_scenario");
	init_scenario();
	CORE_LOG_INFO("init", "Running PARENT init_user_agents");
	init_user_agents();
	CORE_LOG_INFO("init", "Running PARENT init_content_agents");
	init_content_agents();
	CORE_LOG_INFO("init", "Running PARENT init_user_to_content_links");
	init_user_to_content_links();
	CORE_LOG_INFO("init", "Running PARENT init_user_to_user_links");
	init_user_to_user_links();
	CORE_LOG_INFO("init", "Running PARENT init_presimulation_actions");
	init_presimulation_actions();
	CORE_LOG_INFO("init", "Running PARENT do_presimulation_actions");
	do_presimulation_actions();

	// Write the metadata file
	CORE_LOG_INFO("init", "Running PARENT writeMetadataFile");
	Scenario::writeMetadataFile(props);

	// Debug and cleanup
	// CORE_LOG_INFO("init", "Running PARENT debugInit");
	//debugInit();
	CORE_LOG_INFO("init", "Running PARENT cleanupInit");
	cleanupInit();
	CORE_LOG_INFO("init", "Done with PARENT init.");
}


void Scenario::init_scenario(){
	for(int i = 0; i < subscenarios.size(); i++) subscenarios[i]->init_scenario();
	initScenario();
}

void Scenario::init_presimulation_actions(){
	for(int i = 0; i < subscenarios.size(); i++) subscenarios[i]->init_presimulation_actions();
	initPresimulationActions();
}

void Scenario::do_presimulation_actions(){
	for(int i = 0; i < subscenarios.size(); i++) subscenarios[i]->do_presimulation_actions();
	doPresimulationActions();
}

void Scenario::init_user_agents(){
	for(int i = 0; i < subscenarios.size(); i++) subscenarios[i]->init_user_agents();
	initUserAgents();
}

void Scenario::init_content_agents(){
	for(int i = 0; i < subscenarios.size(); i++) subscenarios[i]->init_content_agents();
	initContentAgents();
}

void Scenario::init_user_to_content_links(){
	for(int i = 0; i < subscenarios.size(); i++) subscenarios[i]->init_user_to_content_links();
	initUserToContentLinks();
}

void Scenario::init_user_to_user_links(){
	for(int i = 0; i < subscenarios.size(); i++) subscenarios[i]->init_user_to_user_links();
	initUserToUserLinks();
}

void Scenario::perform_exogenous_events(){
	for(int i = 0; i < subscenarios.size(); i++) subscenarios[i]->perform_exogenous_events();
	performExogenousEvents(); // Child class's implementation
}

void Scenario::perform_agent_actions(){
	for(int i = 0; i < subscenarios.size(); i++) subscenarios[i]->perform_agent_actions();
	performAgentActions(); // Child class's implementation
}


void Scenario::loadShocks(std::string initFilename){
	CORE_LOG_INFO("init", "     Running loadShocks() with file: " + initFilename);
    if(initFilename.length() == 0){
		CORE_LOG_WARN("init", "      No file for exogenous specified- omitting.");
		return;
	}
	std::ifstream  initData(initFilename);
	std::string line;
	if (!initData.good()){ 
		LOG_ERROR("init", "EXITING: Problem in initialization file: " + initFilename);
		exit(UNSPECIFIED_SHOCK_FILE_ISSUE); 
	}
	// First line should be blank (with test)
	if (!std::getline(initData,line)){ 
		LOG_ERROR("init", "EXITING: First line is blank, filename: "  + initFilename);
		exit(SHOCK_FILE_FIRST_LINE_BLANK); 
	}
	std::vector<std::string> headersORIG;
	boost::split(headersORIG, line, boost::is_any_of(","));
	
	std::vector<std::string> headers;
	std::vector<std::string>::iterator headerIter = headersORIG.begin();
	while(headerIter != headersORIG.end()){
		int indx = (*headerIter).find("_open");
		if(indx != std::string::npos){
		   headers.push_back((*headerIter).substr(0, indx));		
		}
		else{
			headers.push_back((*headerIter));
		}
		headerIter++;
	}

	// Advance to where the hash tags are for this Process
	bool fileValid = true;
	std::map<boost::gregorian::date, std::map<std::string, double> >& shocks = *_shocks;
	int lineCount = 0;
	int count = 0;
	int skipped = 0;
	std::set<boost::gregorian::date> dates;
	while(fileValid){
		fileValid = (fileValid && std::getline(initData,line)); // Will be 'true' until a blank line marking eof
		if(fileValid){
			std::vector<std::string> vals;
			boost::split(vals, line, boost::is_any_of(","));

			std::string timestr =  vals[0];

			std::replace(timestr.begin(), timestr.end(),'T',' ');
			timestr.erase(std::remove(timestr.begin(), timestr.end(), 'Z'), timestr.end());

			if(timestr.length() <= 10){
				std::stringstream s;
				s << timestr << " 00:00:00";
				timestr = s.str();
			}


			ptime shockTime(time_from_string(timestr));
			boost::gregorian::date d = shockTime.date();

			if(d >= currentEventTime.date()){
				for(int i = 1; i < vals.size(); i++){
					if(vals[i] != "0.0" && vals[i] != "0"){
						double value = atof(vals[i].c_str());
						CORE_LOG_INFO("init", " Adding a shock for " + headers[i] + " of magnitude " + std::to_string(value) + " on " + to_simple_string(d));
						shocks[d][headers[i]] = value;
						addShockToGlobalInventory(headers[i]);
						dates.insert(d);
						count++;
					}
				}
			}
			else{
				skipped++;
			}
			lineCount++;
		}
	}

	CORE_LOG_INFO("init", "DONE WITH FILE READ: ");
	CORE_LOG_INFO("init", "     "  + std::to_string(lineCount) + " lines ");
	CORE_LOG_INFO("init", "     "  + std::to_string(skipped) + " entries skipped (earlier than " + to_simple_string(currentEventTime.date()) + " or not 8pm)");
	CORE_LOG_INFO("init", "     "  + std::to_string(count) + " shocks ");
	CORE_LOG_INFO("init", "     "  + std::to_string(shocksDuringSim.size()) + " shocks in shocksDuringSim() ");
	CORE_LOG_INFO("init", "     "  + std::to_string(dates.size()) + " unique dates");
}

void Scenario::updateShocks(){
	thisDaysShocks = &((*_shocks)[currentEventTime.date()]);
	repast::Logger& logger = repast::Log4CL::instance()->get_logger("init");

	if(infoIDStore != 0 && infoIDStore->size() != 0){
		if(shockBasedInfoStore == 0) shockBasedInfoStore = new InfoStore();

		shockBasedInfoStore->clear();
		std::map<std::string, double>::iterator iter = thisDaysShocks->begin();
		while(iter != thisDaysShocks->end()){
			// If the shock name is in the info ID store, add it to the shock-based
			if(infoIDStore->find(iter->first)) shockBasedInfoStore->add(iter->first);
			iter++;
		}
		CORE_LOG_INFO("init", " UPDATED SHOCK INFO TO SIZE " + std::to_string(shockBasedInfoStore->size()) + " " + std::to_string((long)shockBasedInfoStore));
	}
}

std::map<std::string, double>& Scenario::getShocks(){
	if(parentScenario != 0) return parentScenario->getShocks();
	if(thisDaysShocks == 0) updateShocks(); // Should only happen if failed to init...
	return *thisDaysShocks;
}

void Scenario::loadInfoIDEventCountFixedIntervals(std::string filename, std::vector<std::unique_ptr<std::map<std::string, long> > >& eventsPerInfoIDFixedIntervals, int simulationTimeInterval){
	CORE_LOG_INFO("init", "Loading information ID events count from " + filename);
	std::ifstream  eventData(filename);
	std::string line;
	std::string initTime = props->getProperty("last.init.event.time");
	std::vector<std::string> dateVals;
	boost::split(dateVals, initTime, boost::is_any_of("."));
	boost::gregorian::date d1(std::stoi(dateVals[0]), std::stoi(dateVals[1]), std::stoi(dateVals[2]));
	ptime simStartTime = ptime(d1, time_duration(0,0,0));
	std::string endTime = props->getProperty("end.time");
	boost::split(dateVals, endTime, boost::is_any_of("."));
	boost::gregorian::date d2(std::stoi(dateVals[0]), std::stoi(dateVals[1]), std::stoi(dateVals[2]));
	ptime simEndTime = ptime(d2, time_duration(0,0,0));
	if (!eventData.good()){ 
		LOG_ERROR("init", "EXITING: Problem in information ID events count file: " + filename);
		exit(UNSPECIFIED_INIT_FILE_ISSUE_A); 
	}
	if (!std::getline(eventData,line)){ 
		LOG_ERROR("init", "EXITING: First line is blank, filename: "  + filename);
		exit(INIT_FILE_FIRST_LINE_BLANK_A); 
	}

	bool fileValid = true;

	std::vector<std::string> headerColumns;
	boost::split(headerColumns,line, boost::is_any_of(","));

	ptime fileTimeStamp;

	while (fileValid){
		fileValid = (fileValid && std::getline(eventData,line));
		if(fileValid){
			std::vector<std::string>::iterator headerItr;
			headerItr = ++headerColumns.begin();					// Ignore first column header
			std::vector<std::string> vals;
			std::vector<std::string>::iterator valsItr;
			std::unique_ptr<std::map<std::string, long> >infoCountRow(new std::map<std::string, long>);
			boost::split(vals, line, boost::is_any_of(","));
			ptime eventsCountTime = time_from_string(*vals.begin());
			if(eventsCountTime >= simStartTime && eventsCountTime < simEndTime) {
				if(!fileTimeStamp.is_not_a_date_time()){
					time_duration fileTimeInterval = time_from_string(*vals.begin()) - fileTimeStamp;
					if(fileTimeInterval.total_seconds() != simulationTimeInterval){
						LOG_ERROR("init", "EXITING: Events count file time interval (" +  std::to_string(fileTimeInterval.total_seconds()) + "s) does not match with simulation time interval (" + std::to_string(simulationTimeInterval) + "s), filename: " + filename);
						exit(FILE_TIME_INTERVAL_MISMATCH);
					}
				}
				fileTimeStamp = time_from_string(*vals.begin());
				for(valsItr = ++vals.begin(); valsItr != vals.end(); ++valsItr, ++headerItr){
					int count = stol(*valsItr);
					if(count){
						infoCountRow->insert({*headerItr, count});
					}
				}
				if(infoCountRow)
					eventsPerInfoIDFixedIntervals.push_back(std::move(infoCountRow));
			}
		}
	}
	CORE_LOG_INFO("init", "Information ID events read from '" + filename + "' complete: ");
}

void Scenario::loadInfoIDEventCount(std::string filename, std::map<ptime, std::map<std::string, int> >& eventsPerInfoID, int simulationTimeInterval){
	CORE_LOG_INFO("init", "Loading information ID events count from "  + filename);
	std::ifstream  eventData(filename);
	std::string line;
	repast::Logger& logger = repast::Log4CL::instance()->get_logger("init");

	if (!eventData.good()){ 
		LOG_ERROR("init", "EXITING: Problem in information ID events count file: " + filename);
		exit(UNSPECIFIED_INIT_FILE_ISSUE_A); 
	}
	if (!std::getline(eventData,line)){ 
		LOG_ERROR("init", "EXITING: First line is blank, filename: "  + filename);
		exit(INIT_FILE_FIRST_LINE_BLANK_A); 
	}

	bool fileValid = true;

	std::vector<std::string> headerColumns;
	boost::split(headerColumns,line, boost::is_any_of(","));

	while (fileValid){
		fileValid = (fileValid && std::getline(eventData,line));
		if(fileValid){
			std::vector<std::string>::iterator headerItr;
			headerItr = ++headerColumns.begin();					// Ignore first column header
			std::vector<std::string> vals;
			std::vector<std::string>::iterator valsItr;
			std::map<std::string, int> infoCountRow;
			boost::split(vals, line, boost::is_any_of(","));
			for(valsItr = ++vals.begin(); valsItr != vals.end(); ++valsItr, ++headerItr){
				int count = stoi(*valsItr);
				if(count){
					infoCountRow.insert({*headerItr, count});
				}
			}
			if(!infoCountRow.empty())
				eventsPerInfoID.insert({time_from_string(*vals.begin()), infoCountRow});
		}
	}
	CORE_LOG_INFO("init", "Information ID events read from '" + filename + "' complete: ");
}
