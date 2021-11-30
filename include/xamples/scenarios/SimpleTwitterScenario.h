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
/* SimpleTwitterScenario.h */
#ifndef SIMPLETWITTERSCENARIO_H
#define SIMPLETWITTERSCENARIO_H

#include "xamples/models/Xample_Model.h"
#include "repast_hpc/Properties.h"

#include "utilities/SimpleWeightedSelector.h"
#include "socialnetwork_toolkit/Scenario.h"

#include "platforms/Twitter/Twitter_Definitions.h"

#include "xamples/platforms/Twitter/Twitter_Definitions_xamples.h"
#include "xamples/platforms/Twitter/Twitter_Platform_xamples.h"
#include "xamples/platforms/Twitter/Twitter_BehaviorSelection_xamples.h"
#include "xamples/platforms/Twitter/Twitter_InfoIDSelection_xamples.h"

#include "xamples/models/Xample_Model.h"
#include "xamples/scenarios/BaseScenario.h"
#include "xamples/platforms/Twitter/Twitter_Agent_xamples.h"
#include "xamples/platforms/Twitter/Twitter_Agent_User_xamples.h"
#include "xamples/platforms/Twitter/Twitter_Agent_Conversation_xamples.h"

using namespace twitter;

class SimpleTwitterScenario: public BaseScenario<Xample_Model, TwitterAgent, TwitterUserAgent>{

	std::vector<TWBehaviorSelector*>     behaviorSelectors;
	std::vector<TWInfoIDSelector*>       infoIDSelectors;
	std::vector<TwitterAgent*>*          staticAgentList;
	std::map<std::string, std::string>*           mapOfCreatorIDsPerContentNodes;
	std::map<std::string, std::set<std::string>>* mapOfInfoIDsPerContentNodes;
	std::map<std::string, ptime>*                 mapOfDatesCreated;

private:
	TwitterEventType getEventType(std::string actionName);
	bool diagnosticMode;

public:
	Twitter* twitter;

	SimpleTwitterScenario(
			boost::mpi::communicator* world,
			Xample_Model* model,
			repast::Properties* properties);
	virtual ~SimpleTwitterScenario();


	virtual void initScenario();
	virtual void synchronize(){ }
	virtual void performExogenousEvents();
	virtual void performAgentActions();
	virtual void recordResults(){ }

	virtual void resyncNetworkTotals(){ }
	virtual void reportNetwork(){ }

	virtual void initUserAgents();
	virtual void initContentAgents();
	virtual void initUserToContentLinks();
	virtual void initUserToUserLinks();
	virtual void cleanupInit();

	virtual void setLocalPointers(TwitterAgent* agent);

	void selectAllAgents(std::vector<TwitterAgent*>& agents, TwitterAgentType agentType = USER_AGENT);
	void selectFractionOfAgents(double frac, std::vector<TwitterAgent*>& agents);
	void selectNAgents(int n, std::vector<TwitterAgent*>& agents);
	void selectNAgentsWeighted(int n, std::vector<TwitterAgent*>& agents);

};


#endif /* SIMPLETWITTERSCENARIO_H */
