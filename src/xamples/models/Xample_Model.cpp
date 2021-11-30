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
/* Xample_Model.cpp */
#include "xamples/models/Xample_Model.h"

#include <stdio.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "boost/mpi.hpp"
#include "boost/date_time/posix_time/posix_time.hpp"
#include "boost/date_time/gregorian/gregorian.hpp"
#include "boost/algorithm/string.hpp"

#include "repast_hpc/AgentId.h"
#include "repast_hpc/RepastProcess.h"
#include "repast_hpc/Utilities.h"
#include "repast_hpc/Properties.h"
#include "repast_hpc/logger.h"

#include "rhpc-smple/include/utilities/Utilities.h"

#include "xamples/scenarios/SimpleTwitterScenario.h"
#include "xamples/utilities/Xample_ReturnValues.h"

using namespace boost::posix_time;
using namespace boost::gregorian;

ptime currentEventTime;    // A Global variable to hold the time to output with the 4 col data
ptime nextExogEvent;       // A global variable to hold the next time that exogenous events should be performed
int   worldState;          // Set of flags to indicate the world state; used to direct what the exogenous events will be. Default is zero, reset every time increment.
ptime stampTime;		   // A global variable to hold the real-time stamp


Xample_Model::Xample_Model(repast::Properties* properties, int argc, char** argv, boost::mpi::communicator* world) :
			props(properties),  index(0), heartbeatInterval(1), timeInterval(1) {

	rank = repast::RepastProcess::instance()->rank();
	heartbeatInterval = getPropertyWithDefault<int>("system.heartBeat", 1, props);

	LOG_INFO("init", "Calculating start time...");
	std::string startTime = props->getProperty("start.time");
	if(startTime != "" && startTime != "__NONE__"){
		LOG_INFO("init", "Parsing from start.time property");
		currentEventTime = parseTime(startTime);
	}else{
		LOG_ERROR("init", "No Start Time Provided");
		exit(NO_START_TIME_PROPERTY);
	}
	LOG_INFO("init", "Start time is:" + boost::posix_time::to_simple_string(currentEventTime));

	timeInterval     = repast::strToInt(props->getProperty("timeInterval"));
	LOG_INFO("init", "Time interval:" + std::to_string(timeInterval));

	repast::Logger& logger = repast::Log4CL::instance()->get_logger("init");
	logger.log(repast::INFO, "RHPC MAIN CALLING SCENARIO CONSTRUCTOR");
	std::string scenarioName = props->getProperty("ScenarioName");
	if(scenarioName == "SimpleTwitterScenario"){
		scenario = new SimpleTwitterScenario(world, this, props);
	}
	else {
        logger.log(repast::ERROR, "Invalid ScenarioName '" + scenarioName + "' please check your properties file - Exit!");
		exit(0);
    }

	std::string timestampsFile = props->getProperty("simulation.timestamps.filename");
	if(timestampsFile != "" && timestampsFile != "__NONE__"){
		logger.log(repast::INFO, "The simulation will run according to the real-time intervals, therefore, please make sure to provide the event count per infoid files corresponding to this logic.");
		//loadSimulationIntervals(timestampsFile);
	}
	else {
		logger.log(repast::INFO, "No filename provided for the real-time stamps, therefore, the simulation will run according to the fixed time intervals.");
	}


	nextExogEvent = currentEventTime; // Set this up for the start of the sim
	worldState = 1; // Set up so that all exogenous events are done
	logger.log(repast::INFO, "<SocialSimModel Constructor>: Parameter for ScenarioName '" + scenarioName + "'  loaded " + scenarioName);
	logger.log(repast::INFO, "<SocialSimModel Constructor>: Complete");
}

Xample_Model::~Xample_Model(){
	delete scenario;
}


void Xample_Model::init() {
	repast::Logger& logger = repast::Log4CL::instance()->get_logger("init");
	//std::cout << "SocialSimModel: Running scenario init" << std::endl;
	logger.log(repast::INFO, "SocialSimModel: Running scenario init");
	scenario->init();
	//std::cout << "SocialSimModel: Completed scenario init" << std::endl << std::endl;
	logger.log(repast::INFO, "SocialSimModel: Completed scenario init");
}


void Xample_Model::initSchedule(repast::ScheduleRunner& runner) {
	repast::Logger& logger = repast::Log4CL::instance()->get_logger("init");
	logger.log(repast::INFO, "Heartbeat interval: " + std::to_string(heartbeatInterval));
	if(heartbeatInterval > 0 && rank == 0){
		runner.scheduleEvent(0,  heartbeatInterval, repast::Schedule::FunctorPtr(new repast::MethodFunctor<Xample_Model> (this, &Xample_Model::heartBeat)));
	}

	runner.scheduleEvent(0.1, 1, repast::Schedule::FunctorPtr(new repast::MethodFunctor<Scenario> (scenario, &Scenario::perform_exogenous_events)));
	runner.scheduleEvent(0.3, 1, repast::Schedule::FunctorPtr(new repast::MethodFunctor<Scenario> (scenario, &Scenario::perform_agent_actions)));
	runner.scheduleEvent(0.9, 1, repast::Schedule::FunctorPtr(new repast::MethodFunctor<Xample_Model> (this, &Xample_Model::incrementTime)));

	LOG_INFO("init", "Getting end time");
	std::string endTimeStr = props->getProperty("end.time"); // We know this is there because we add it in RHPCMain
	endTime = parseTime(endTimeStr);
	//runner.scheduleStop((endTime - currentEventTime).total_seconds()/timeInterval);
	//std::cout << "End time will be " << endTime << std::endl;
	logger.log(repast::INFO, "End time will be " + endTimeStr);
}



void Xample_Model::incrementTime(){
	currentEventTime += seconds(timeInterval);  				// Move the event time forward
	worldState = 0;
	if(currentEventTime >= nextExogEvent){
		string nEE1 = boost::posix_time::to_simple_string(nextExogEvent);
		nextExogEvent += seconds(timeInterval);
		string nEE2 = boost::posix_time::to_simple_string(nextExogEvent);
		LOG_INFO("init", "INCREASING NEXT EXOG EVENT FROM " + nEE1 + " to " + nEE2);
		worldState = 1;
	}
	if(currentEventTime >= endTime) repast::RepastProcess::instance()->getScheduleRunner().stop();
}

void Xample_Model::heartBeat() {
	repast::Logger& logger = repast::Log4CL::instance()->get_logger("general");
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
    // for more information about date/time format
    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);
	string b = buf;
	string currTick = to_string(repast::RepastProcess::instance()->getScheduleRunner().currentTick());
	logger.log(repast::INFO, "TICK: " + currTick + ", " + "Sim Time: " +boost::posix_time::to_simple_string(currentEventTime) + ", " + "Wall Clock: " + b);
}

int Xample_Model::getSimulationTimeInterval() {
	 return timeInterval;
}


void Xample_Model::loadSimulationIntervals(std::string filename) {
	LOG_INFO("init", "Loading simulation timestamps from file: " + filename);
	std::ifstream  eventData(filename);
	std::string line;
	if (!eventData.good()){
		LOG_ERROR("init", "EXITING: Problem in simulation timestamps file: " + filename);
		exit(UNSPECIFIED_INIT_FILE_ISSUE_A);
	}
	if (!std::getline(eventData,line)){
		LOG_ERROR("init", "EXITING: First line is blank, filename: "  + filename);
		exit(INIT_FILE_FIRST_LINE_BLANK_A);
	}


	bool fileValid = true;
	ptime prevTimeStamp;
	while (fileValid){
		fileValid = (fileValid && std::getline(eventData,line));
		if(fileValid){
			std::vector<std::string> vals;
			boost::split(vals, line, boost::is_any_of(","));
			if(stampTime.is_not_a_date_time()){
				ptime tempTime(time_from_string(*vals.begin()).date(), time_duration(0,0,0));
				stampTime     = tempTime;
				startTime     = tempTime;
				prevTimeStamp = tempTime;
			}
			simulationIntervals.push_back(time_from_string(*vals.begin()) - prevTimeStamp);
			prevTimeStamp = time_from_string(*vals.begin());
		}
	}

	time_duration td = prevTimeStamp.time_of_day();
	if(td.hours() != 0 && td.minutes() != 0 && td.seconds() != 0){
		boost::gregorian::days dd(1);
		ptime midnightPastLastTimeStamp((prevTimeStamp+dd).date(), time_duration(0,0,0));
		simulationIntervals.push_back(midnightPastLastTimeStamp - prevTimeStamp);
	}

	LOG_INFO("init",  "Simulation timestamps read from '" + filename + "' complete.");
}
