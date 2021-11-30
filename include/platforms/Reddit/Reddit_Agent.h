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
/* Reddit_Agent.h */
#ifndef INCLUDE_REDDIT_AGENT_H
#define INCLUDE_REDDIT_AGENT_H

#include "socialnetwork_toolkit/SocialNetwork_Platform.h"

#include "Reddit_Definitions.h"
#include "Reddit_Feed.h"
#include "Reddit_Action.h"

using namespace rhpc_smple;

namespace reddit{

template<typename SCORETYPE>
class RedditComment: public SocialNetwork_AbstractElement<RedditAgentType, SCORETYPE>{
public:

    std::string nodeID;
    SocialNetwork_AbstractElement<RedditAgentType, SCORETYPE>* parentPtr;     // Will be either a post or a comment; may be same as root
    SocialNetwork_AbstractElement<RedditAgentType, SCORETYPE>* rootPtr;       // Will be a posty
    std::string communityID;
    int         countOfDirectComments;
    int         countOfAllComments;
    int         indx;
    bool        partOfSeededCascade;


    RedditComment(std::string ID,
					SocialNetwork_AbstractElement<RedditAgentType, SCORETYPE>* creatorPtr,
					SocialNetwork_AbstractElement<RedditAgentType, SCORETYPE>* parentPtr,
					SocialNetwork_AbstractElement<RedditAgentType, SCORETYPE>* rootPtr,
                    std::string community,
                    bool partOfCascade);
    virtual ~RedditComment();

    void incrementAllParentsCounts();
};


/**
 * An Agent base class that allows multiple classes in the context
 **/
template<typename ACTIONTYPE, typename SCORETYPE>
class AbstractRedditAgent: public SocialNetworkNode<RedditAgentType, ACTIONTYPE, SCORETYPE>{
protected:

	Reddit*                       reddit;
	boost::shared_ptr<RedditFeed<SCORETYPE>> myFeed;

public:

	bool partOfSeededCascade;

	// Constructors:
	AbstractRedditAgent(repast::AgentId id, RedditAgentType whichType, ptime dateCreated, SNP* platform, SocialNetwork_AbstractElement<RedditAgentType, SCORETYPE>* creator = 0);

    // Destructor
    virtual ~AbstractRedditAgent();

    virtual void setCurrentRank(int currentRank);

    virtual std::string showName();
    virtual std::string writeName();

    virtual void setPlatform(SNP* platform);

    void placeInFeed(SocialNetwork_AbstractElement<RedditAgentType, SCORETYPE>* actor,
    		SocialNetwork_AbstractElement<RedditAgentType, SCORETYPE>* target,
			SocialNetwork_AbstractElement<RedditAgentType, SCORETYPE>* subtarget,
			RedditEventType action,
    		RedditFeedExplanation fe);

    int getCountOfFeedElements();

    virtual void selectTarget(ACTIONTYPE& action);

};

template<typename ACTIONTYPE, typename SCORETYPE>
AbstractRedditAgent<ACTIONTYPE, SCORETYPE>::AbstractRedditAgent(repast::AgentId agentId, RedditAgentType whichType, ptime dateCreated, SNP* platform, SocialNetwork_AbstractElement<RedditAgentType, SCORETYPE>* creator):
		SocialNetworkNode<RedditAgentType, ACTIONTYPE, SCORETYPE>(agentId, whichType, dateCreated, platform, creator),
		myFeed(new RedditFeed<SCORETYPE>(1000000)), partOfSeededCascade(false) {
	std::stringstream s;
	s << "RD" <<  agentId.hashcode();
	this->setHashTagID(s.str());
}

template<typename ACTIONTYPE, typename SCORETYPE>
AbstractRedditAgent<ACTIONTYPE, SCORETYPE>::~AbstractRedditAgent() {
}

template<typename ACTIONTYPE, typename SCORETYPE>
void AbstractRedditAgent<ACTIONTYPE, SCORETYPE>::setCurrentRank(int currentRank){
    this->id.currentRank(currentRank);
}

template<typename ACTIONTYPE, typename SCORETYPE>
std::string AbstractRedditAgent<ACTIONTYPE, SCORETYPE>::showName(){
	std::ostringstream ss;
	ss << this->getId().hashcode();
	return ss.str();
}

template<typename ACTIONTYPE, typename SCORETYPE>
std::string AbstractRedditAgent<ACTIONTYPE, SCORETYPE>::writeName(){
	std::ostringstream ss;
	ss << this->getId().hashcode();
	return ss.str();
}

template<typename ACTIONTYPE, typename SCORETYPE>
void AbstractRedditAgent<ACTIONTYPE, SCORETYPE>::setPlatform(SNP* platform){
	SocialNetwork_AbstractNode<RedditAgentType, SCORETYPE>::setPlatform(platform);
		reddit = (Reddit*)(this->containingPlatform);
}

template<typename ACTIONTYPE, typename SCORETYPE>
void AbstractRedditAgent<ACTIONTYPE, SCORETYPE>::placeInFeed(SocialNetwork_AbstractElement<RedditAgentType, SCORETYPE>* actor,
		SocialNetwork_AbstractElement<RedditAgentType, SCORETYPE>* target,
		SocialNetwork_AbstractElement<RedditAgentType, SCORETYPE>* subtarget,
		RedditEventType action,
		RedditFeedExplanation fe){
	  myFeed->push(actor, target, subtarget, action, fe);
}

template<typename ACTIONTYPE, typename SCORETYPE>
int AbstractRedditAgent<ACTIONTYPE, SCORETYPE>::getCountOfFeedElements(){
	return myFeed->size();
}

template<typename ACTIONTYPE, typename SCORETYPE>
void AbstractRedditAgent<ACTIONTYPE, SCORETYPE>::selectTarget(ACTIONTYPE& action){

}


} // End namespace reddit

#endif /* INCLUDE_REDDIT_AGENT_H */
