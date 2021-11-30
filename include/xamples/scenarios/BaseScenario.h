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
/* BaseScenario.h */
#ifndef BASESCENARIO_H
#define BASESCENARIO_H

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
#include "socialnetwork_toolkit/Scenario.h"

template<typename MODEL, typename AGENT, typename USERAGENT>
class BaseScenario: public ScenarioWithAgents<AGENT>{

protected:
	MODEL*        worldModel;
	std::string	  modelName;

	std::string   actingAgentListStrategy;
	double        agentFraction;
	std::string   behaviorSelectorType;
	int           agentsPerStep;
	int           newAgentsPerStep;
	double        probabilityOfAddingNewAgents;
    ptime         earliestEventTime;
    ptime         lastInitEventTime;

	long 		  defaultWeight;
	bool 		  limitToInfoIDList;

	int	   		  simulationTimeInterval;
	bool          useZeroFitnessAgents;
	bool          ageAtInit;



public:
	std::map<std::string, AGENT*>* 	  contentNames;
	std::map<std::string, AGENT*>* 	  userNames;
	std::map<std::string, InfoID*>*   agentNamesAndCommunities;
	long                              minShockToHaveEffect;
	long                              maxShockToHaveEffect;
	double                            maxShockEffect;

	BaseScenario(MODEL* model, repast::Properties* properties);
	virtual ~BaseScenario();

	virtual void cleanupInit();
	virtual void debugInit(){};

	double getShockEffect(int countOfShocks);
};

template<typename MODEL, typename AGENT, typename USERAGENT>
BaseScenario<MODEL, AGENT, USERAGENT>::BaseScenario(MODEL* model, repast::Properties* properties):
			ScenarioWithAgents<AGENT>(properties),
			worldModel(model),
			actingAgentListStrategy(""),
			agentFraction(1.0),
			behaviorSelectorType(""),
			agentsPerStep(1),
			newAgentsPerStep(0),
			probabilityOfAddingNewAgents(1.0),
			modelName(""),
			userNames(0),
			contentNames(0),
            earliestEventTime(time_from_string("2000-01-01 00:00:00")),
			lastInitEventTime(time_from_string("2022-01-01 00:00:00")),
			minShockToHaveEffect(0),
			maxShockToHaveEffect(0),
			maxShockEffect(0),
			useZeroFitnessAgents(true),
			ageAtInit(false){
	LOG_INFO("general", "<BaseScenario>: Constructor");
	// Read properties

	// Set this before initializing output files
	if(Scenario::props->getProperty("seed.posts").length() > 0)
		Scenario::writeOnlyCascades = true;
	if(Scenario::props->getProperty("new.agents.per.step").length() > 0)
		newAgentsPerStep = atoi(Scenario::props->getProperty("new.agents.per.step").c_str());
	if(Scenario::props->getProperty("probability.of.adding.new.agents").length() > 0)
		probabilityOfAddingNewAgents = getPropertyWithDefault<double>("probability.of.adding.new.agents",1.0,properties);
    behaviorSelectorType = Scenario::props->getProperty("user.agent.behavior.selection");
    if(Scenario::props->getProperty("earliest.event.time") != "")
    	earliestEventTime = parseTime(Scenario::props->getProperty("earliest.event.time"));
	LOG_INFO("general",  "<BaseScenario>: Earliest event time:  " + to_simple_string(earliestEventTime.date()));
    if(Scenario::props->getProperty("last.init.event.time") != "")
    	lastInitEventTime = parseTime(Scenario::props->getProperty("last.init.event.time"));
    LOG_INFO("general",  "<BaseScenario>: Last init event time: " + to_simple_string(lastInitEventTime.date()));
//	if     (behaviorSelectorType == "Multiplex"){
//		USERAGENT::whichSelection = 1;
//		LOG_INFO("general",  "<BaseScenario>: Agent behavior selection: Multiplex");
//	}
//	else{
//		LOG_WARN("general",  "<BaseScenario>: user.agent.behavior.selection property '" + behaviorSelectorType + "' is not valid and users will not act; use 'Multiplex'");
//	}
//
	actingAgentListStrategy = Scenario::props->getProperty("user.acting.agent.list.strategy");
    if(actingAgentListStrategy == "All"){
		LOG_INFO("general",  "<BaseScenario>: Agents acting per time step: All");
	}
	else if(actingAgentListStrategy == "Absolute" || actingAgentListStrategy == "Weighted"){
		agentsPerStep = atoi(Scenario::props->getProperty("agents.per.step").c_str());
		LOG_INFO("general",  "<BaseScenario>: Agents acting per time step: " + to_string(agentsPerStep));
	}
	else if(actingAgentListStrategy == "Fraction"){
	    agentFraction = atof(Scenario::props->getProperty("agent.fraction.per.step").c_str());
	    LOG_INFO("general",  "<BaseScenario>: Agents acting per time step: " + to_string(agentFraction) + " * total");
	}
	else if(actingAgentListStrategy == "Driven" || actingAgentListStrategy == "drivenWeighted"){
		agentsPerStep = atoi(Scenario::props->getProperty("agents.per.step").c_str());
		LOG_INFO("general",  "<BaseScenario>: Agents acting per time step: " + to_string(agentsPerStep));
	}

	minShockToHaveEffect = getPropertyWithDefault<long>("min.shock.to.have.effect", 0l, properties);
	maxShockToHaveEffect = getPropertyWithDefault<long>("max.shock.to.have.effect", 0l, properties);
	maxShockEffect       = getPropertyWithDefault<double>("max.shock.effect", 1.0, properties);

	useZeroFitnessAgents = getPropertyWithDefault<std::string>("consider.zero.fitness.agents", "true", properties) == "true";

	std::string agentInfoIDDistributionTEMP = getPropertyWithDefault<std::string>("agent.info.id.distribution", "minimize", properties);
	Scenario::minimizeAgentsForInfoIDs = (agentInfoIDDistributionTEMP == "maximize") ? false : true; // Zero is the default, maximize
	LOG_INFO("general",  "Initialized minimize agents to " + boost::lexical_cast<std::string>(Scenario::minimizeAgentsForInfoIDs) + " based on " + agentInfoIDDistributionTEMP);
    modelName = "MBM";

	Scenario::props->putProperty("model_name", modelName);

	ageAtInit = (getPropertyWithDefault<std::string>("age.at.init", "false", Scenario::props) == "true");

    // Data structure for storing lists of user names and (eventually) mapping them with agents as they are created
	userNames                = new std::map<std::string, AGENT*>();
	contentNames             = new std::map<std::string, AGENT*>();
	agentNamesAndCommunities = new std::map<std::string, InfoID*>();

	simulationTimeInterval = model->getSimulationTimeInterval();
	LOG_INFO("general",  "<BaseScenario>: DONE WITH Constructor");
}


template<typename MODEL, typename AGENT, typename USERAGENT>
BaseScenario<MODEL, AGENT, USERAGENT>::~BaseScenario(){}


template<typename MODEL, typename AGENT, typename USERAGENT>
void BaseScenario<MODEL, AGENT, USERAGENT>::cleanupInit(){
	LOG_INFO("general",  "<BaseScenario>: Doing CleanupInit...");
	delete userNames;
	delete contentNames;
	delete agentNamesAndCommunities;
}

template<typename MODEL, typename AGENT, typename USERAGENT>
double BaseScenario<MODEL, AGENT, USERAGENT>::getShockEffect(int countOfShocks){
	if(maxShockToHaveEffect <= minShockToHaveEffect) return 1;
	if(countOfShocks        <= minShockToHaveEffect) return 1;
	if(countOfShocks        >= maxShockToHaveEffect) return maxShockEffect;
	double diff = countOfShocks - minShockToHaveEffect;
	double range = maxShockToHaveEffect - minShockToHaveEffect; // TODO Encapsulate and calculate once
	return 1 + (diff / range) * (maxShockEffect - 1) ;
}

#endif /* BASESCENARIO_H */
