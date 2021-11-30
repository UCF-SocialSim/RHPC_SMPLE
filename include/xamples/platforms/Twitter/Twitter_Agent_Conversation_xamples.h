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
/* Twitter_Agent_Conversation_xamples.h */
#ifndef INCLUDE_TWITTER_AGENT_CONVERSATION_XAMPLES_H
#define INCLUDE_TWITTER_AGENT_CONVERSATION_XAMPLES_H


#include "repast_hpc/AgentId.h"
#include "repast_hpc/SharedContext.h"
#include "repast_hpc/SharedNetwork.h"

#include "platforms/Twitter/Twitter_Network.h"
#include "platforms/Twitter/Twitter_Agent_Conversation.h"

#include "Twitter_Definitions_xamples.h"
#include "Twitter_Agent_xamples.h"

namespace twitter{

/* Agents */
class TwitterConversationAgent : public TwitterAgent, public AbstractTwitterConversationAgent<Twitter_Xamples_ScoreTypes>{

public:
    TwitterConversationAgent(repast::AgentId id, ptime dateCreated, TwitterAgentPackage& package, SNP* platform, SocialNetwork_AbstractElement<TwitterAgentType, Twitter_Xamples_ScoreTypes>* creator = 0);
    virtual ~TwitterConversationAgent();

    virtual std::string showName();
    virtual std::string writeName();

    virtual void receiveAction(TwitterAction& action);

    virtual std::string getResponseNameRoot();

    virtual SocialNetwork_AbstractElement<TwitterAgentType, Twitter_Xamples_ScoreTypes>* getSelfPointer();

};

} // End namespace twitter



#endif /* INCLUDE_TWITTER_AGENT_CONVERSATION_XAMPLES_H */
