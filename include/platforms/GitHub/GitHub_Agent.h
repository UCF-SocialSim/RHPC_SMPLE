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
/* GitHub_Agent.h */
#ifndef INCLUDE_GITHUB_AGENT_H
#define INCLUDE_GITHUB_AGENT_H

#include "socialnetwork_toolkit/SocialNetwork_Platform.h"

#include "GitHub_Definitions.h"
#include "GitHub_Feed.h"
#include "GitHub_Action.h"

using namespace rhpc_smple;

namespace github{

template<typename ACTIONTYPE, typename SCORETYPE>
class AbstractGitHubAgent: public SocialNetworkNode<GitHubAgentType, ACTIONTYPE, SCORETYPE>{

protected:
	GitHub*                       gitHub;
	boost::shared_ptr<GitHubFeed<SCORETYPE>> myFeed;

public:
	AbstractGitHubAgent(repast::AgentId id, GitHubAgentType whichType, ptime dateCreated, SNP* platform, SocialNetwork_AbstractElement<GitHubAgentType, SCORETYPE>* creator = 0);
	virtual ~AbstractGitHubAgent();

    virtual void setCurrentRank(int currentRank);

    virtual std::string showName();
    virtual std::string writeName();

    void placeInFeed(SocialNetwork_AbstractElement<GitHubAgentType, SCORETYPE>* actor,
    		SocialNetwork_AbstractElement<GitHubAgentType, SCORETYPE>* target,
			SocialNetwork_AbstractElement<GitHubAgentType, SCORETYPE>* subtarget,
			GitHubEventType action,
    		GitHubFeedExplanation fe);

    virtual void setPlatform(SNP* platform);

    int getCountOfFeedElements();

    virtual void selectTarget(ACTIONTYPE& action);

};

template<typename ACTIONTYPE, typename SCORETYPE>
AbstractGitHubAgent<ACTIONTYPE, SCORETYPE>::AbstractGitHubAgent(repast::AgentId agentId, GitHubAgentType whichType, ptime dateCreated, SNP* platform, SocialNetwork_AbstractElement<GitHubAgentType, SCORETYPE>* creator):
		SocialNetworkNode<GitHubAgentType, ACTIONTYPE, SCORETYPE>(agentId, whichType, dateCreated, platform, creator),
		myFeed(new GitHubFeed<SCORETYPE>(1000000)) {
	std::stringstream s;
	s << "GH" <<  agentId.hashcode();
	this->setHashTagID(s.str());
}

template<typename ACTIONTYPE, typename SCORETYPE>
AbstractGitHubAgent<ACTIONTYPE, SCORETYPE>::~AbstractGitHubAgent() { }

template<typename ACTIONTYPE, typename SCORETYPE>
void AbstractGitHubAgent<ACTIONTYPE, SCORETYPE>::setCurrentRank(int currentRank){
    this->id.currentRank(currentRank);
}

template<typename ACTIONTYPE, typename SCORETYPE>
std::string AbstractGitHubAgent<ACTIONTYPE, SCORETYPE>::showName(){
	std::ostringstream ss;
	ss << this->getId().hashcode();
	return ss.str();
}

template<typename ACTIONTYPE, typename SCORETYPE>
std::string AbstractGitHubAgent<ACTIONTYPE, SCORETYPE>::writeName(){
	std::ostringstream ss;
	ss << this->getId().hashcode();
	return ss.str();
}

template<typename ACTIONTYPE, typename SCORETYPE>
void AbstractGitHubAgent<ACTIONTYPE, SCORETYPE>::placeInFeed(SocialNetwork_AbstractElement<GitHubAgentType, SCORETYPE>* actor,
		SocialNetwork_AbstractElement<GitHubAgentType, SCORETYPE>* target,
		SocialNetwork_AbstractElement<GitHubAgentType, SCORETYPE>* subtarget,
		GitHubEventType action,
		GitHubFeedExplanation fe){
	  myFeed->push(actor, target, subtarget, action, fe);
}

template<typename ACTIONTYPE, typename SCORETYPE>
void AbstractGitHubAgent<ACTIONTYPE, SCORETYPE>::setPlatform(SNP* platform){
	SocialNetwork_AbstractNode<GitHubAgentType, SCORETYPE>::setPlatform(platform);
	gitHub = (GitHub*)(this->containingPlatform);
}

template<typename ACTIONTYPE, typename SCORETYPE>
int AbstractGitHubAgent<ACTIONTYPE, SCORETYPE>::getCountOfFeedElements(){
	return myFeed->size();
}

template<typename ACTIONTYPE, typename SCORETYPE>
void AbstractGitHubAgent<ACTIONTYPE, SCORETYPE>::selectTarget(ACTIONTYPE& action){ }


} // End namespace github

#endif /* INCLUDE_GITHUB_AGENT_H */
