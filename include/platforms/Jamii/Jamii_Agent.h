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
/* Jamii_Agent.h */
#ifndef INCLUDE_JAMII_AGENT_H
#define INCLUDE_JAMII_AGENT_H

#include "socialnetwork_toolkit/SocialNetwork_Platform.h"

#include "Jamii_Definitions.h"
#include "Jamii_Feed.h"

namespace jamii{

class Jamii; // FORWARD DECLARATION

template<typename ACTIONTYPE, typename SCORETYPE>
class AbstractJamiiAgent: public SocialNetworkNode<JamiiAgentType, ACTIONTYPE, SCORETYPE>{

protected:
	Jamii*                       jamii;
	boost::shared_ptr<JamiiFeed<SCORETYPE>> myFeed;

public:
	AbstractJamiiAgent(repast::AgentId id, JamiiAgentType whichType, ptime dateCreated, SNP* platform, SocialNetwork_AbstractElement<JamiiAgentType, SCORETYPE>* creator = 0);
	virtual ~AbstractJamiiAgent();

    virtual void setCurrentRank(int currentRank);

    virtual std::string showName();
    virtual std::string writeName();

    void placeInFeed(SocialNetwork_AbstractElement<JamiiAgentType, SCORETYPE>* actor,
    		SocialNetwork_AbstractElement<JamiiAgentType, SCORETYPE>* target,
			SocialNetwork_AbstractElement<JamiiAgentType, SCORETYPE>* subtarget,
			JamiiEventType action,
    		JamiiFeedExplanation fe);

    virtual void setPlatform(SNP* platform);

    int getCountOfFeedElements();

    virtual void selectTarget(ACTIONTYPE& action);

};

template<typename ACTIONTYPE, typename SCORETYPE>
AbstractJamiiAgent<ACTIONTYPE, SCORETYPE>::AbstractJamiiAgent(repast::AgentId agentId, JamiiAgentType whichType, ptime dateCreated, SNP* platform, SocialNetwork_AbstractElement<JamiiAgentType, SCORETYPE>* creator):
		SocialNetworkNode<JamiiAgentType, ACTIONTYPE, SCORETYPE>(agentId, whichType, dateCreated, platform, creator),
		myFeed(new JamiiFeed<SCORETYPE>(1000000)) {
	std::stringstream s;
	s << "JM" <<  agentId.hashcode();
	this->setHashTagID(s.str());
}

template<typename ACTIONTYPE, typename SCORETYPE>
AbstractJamiiAgent<ACTIONTYPE, SCORETYPE>::~AbstractJamiiAgent() { }

template<typename ACTIONTYPE, typename SCORETYPE>
void AbstractJamiiAgent<ACTIONTYPE, SCORETYPE>::setCurrentRank(int currentRank){
    this->id.currentRank(currentRank);
}

template<typename ACTIONTYPE, typename SCORETYPE>
std::string AbstractJamiiAgent<ACTIONTYPE, SCORETYPE>::showName(){
	std::ostringstream ss;
	ss << this->getId().hashcode();
	return ss.str();
}

template<typename ACTIONTYPE, typename SCORETYPE>
std::string AbstractJamiiAgent<ACTIONTYPE, SCORETYPE>::writeName(){
	std::ostringstream ss;
	ss << this->getId().hashcode();
	return ss.str();
}

template<typename ACTIONTYPE, typename SCORETYPE>
void AbstractJamiiAgent<ACTIONTYPE, SCORETYPE>::placeInFeed(SocialNetwork_AbstractElement<JamiiAgentType, SCORETYPE>* actor,
		SocialNetwork_AbstractElement<JamiiAgentType, SCORETYPE>* target,
		SocialNetwork_AbstractElement<JamiiAgentType, SCORETYPE>* subtarget,
		JamiiEventType action,
		JamiiFeedExplanation fe){
	  myFeed->push(actor, target, subtarget, action, fe);
}

template<typename ACTIONTYPE, typename SCORETYPE>
void AbstractJamiiAgent<ACTIONTYPE, SCORETYPE>::setPlatform(SNP* platform){
	SocialNetwork_AbstractNode<JamiiAgentType, SCORETYPE>::setPlatform(platform);
	jamii = (Jamii*)(this->containingPlatform);
}

template<typename ACTIONTYPE, typename SCORETYPE>
int AbstractJamiiAgent<ACTIONTYPE, SCORETYPE>::getCountOfFeedElements(){
	return myFeed->size();
}

template<typename ACTIONTYPE, typename SCORETYPE>
void AbstractJamiiAgent<ACTIONTYPE, SCORETYPE>::selectTarget(ACTIONTYPE& action){ }

} // End namespace jamii

#endif /* INCLUDE_JAMII_AGENT_H */
