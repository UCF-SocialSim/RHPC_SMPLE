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
/* Twitter_Agent_User_xamples.h */
#ifndef INCLUDE_TWITTER_AGENT_USER_XAMPLES_H
#define INCLUDE_TWITTER_AGENT_USER_XAMPLES_H

#include "repast_hpc/AgentId.h"
#include "repast_hpc/SharedContext.h"
#include "repast_hpc/SharedNetwork.h"
#include "repast_hpc/Random.h"

#include "utilities/SimpleWeightedSelector.h"
#include "socialnetwork_toolkit/BehaviorSelection.h"

#include "Twitter_Agent_xamples.h"
#include "Twitter_InfoIDSelection_xamples.h"

using namespace rhpc_smple;

namespace twitter{

/** Twitter User Agents **/
class TwitterUserAgent : public TwitterAgent{
public:
	BehaviorSelector<TwitterAgent, TwitterAction>* behaviorSelector;
	TWInfoIDSelector*                              infoIDSelector;

    TwitterUserAgent(repast::AgentId id, ptime dateCreated, TwitterAgentPackage& package, SNP* platform);

    virtual ~TwitterUserAgent();

	virtual void doNextAction();
	virtual void doAllActions();
	virtual void doPendingAction(std::vector<TwitterAction>::iterator action);

	virtual void doAction(TwitterAction& action);

    virtual std::string showName();
    virtual std::string writeName();

    virtual void doSelectBehavior();
    virtual void selectBehavior();
	virtual void selectTarget(TwitterAction& action);
	virtual void selectInfoIDs(SN_ActionPayload& payload);

};


} // End namespace twitter

#endif /* INCLUDE_TWITTER_AGENT_USER_XAMPLES_H */
