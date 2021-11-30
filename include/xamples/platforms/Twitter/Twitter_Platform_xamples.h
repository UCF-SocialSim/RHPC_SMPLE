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
/* Twitter_Platform_xamples.h */
#ifndef INCLUDE_TWITTER_PLATFORM_XAMPLES_H
#define INCLUDE_TWITTER_PLATFORM_XAMPLES_H

#include "socialnetwork_toolkit/SocialNetwork_Platform.h"
#include "socialnetwork_toolkit/BehaviorSelection.h"

#include "platforms/Twitter/Twitter_Definitions.h"
#include "platforms/Twitter/Twitter_Network.h"
#include "platforms/Twitter/Twitter_Platform.h"

#include "Twitter_Definitions_xamples.h"
#include "Twitter_Agent_xamples.h"
#include "Twitter_Agent_Conversation_xamples.h"
#include "Twitter_Agent_User_xamples.h"
#include "Twitter_Action_xamples.h"

#include "Twitter_Feed_Query_xamples.h"

using namespace rhpc_smple;

namespace twitter{

typedef TypedFileWriter<Twitter_Xamples_FileOutputTypes> Twitter_FileWriter;

//  Templates:
class Twitter: public AbstractTwitter<TwitterAgent, TwitterAction, Twitter_Xamples_ScoreTypes, TwitterAgentPackage, Twitter_FileWriter>{
protected:

	TwitterFeed<Twitter_Xamples_ScoreTypes>* currentFittest;

public:
	Twitter(boost::mpi::communicator* world, ScenarioWithAgents<TwitterAgent>* scenario);
	~Twitter();

	virtual TwitterAgentPackage   	getPackage(TwitterAgent* agent);
	virtual void 				    updateAgent(TwitterAgentPackage package);
	virtual void 				    updateAgent(TwitterAgentPackage package, TwitterAgent* agent);
	virtual void                    provideContent(repast::AgentRequest req, std::vector<TwitterAgentPackage>& out);
	virtual void                    providePackage(TwitterAgent* agent, std::vector<TwitterAgentPackage>& out);
	virtual TwitterAgent*           createNewAgent(repast::AgentId id, TwitterAgentPackage agentPackage);
	virtual TwitterAgent*           createAndAddNewAgent(TwitterAgentPackage agentPackage);
	virtual void                    setLocalPointers(TwitterAgent* agent);

	virtual void init_schedule(repast::ScheduleRunner& runner, double offset = 0);

	// Methods that an agent might call
	virtual boost::shared_ptr<TwitterFeed<Twitter_Xamples_ScoreTypes>> getFeed(TwitterAgent* requestor, TwitterFeedQuery_xamples query);


	void getOutlinkSums(TwitterAgent* source, TwitterEdge<TwitterAgent, TwitterEdgeInfo>& accumulator);
	long getTotalActionCount(TwitterAgent* source);

	void selectFittestAgents(int n, std::vector<TwitterAgent*>& agents, TwitterAgentType type, Twitter_Xamples_ScoreTypes score = TWITTER_XAMPLES_DEFAULT_SCORE);

	void getSelector(TwitterAgent* source, SimpleWeightedSelector& sws);

};

} // End namespace twitter


#endif /* INCLUDE_TWITTER_PLATFORM_XAMPLES_H */
