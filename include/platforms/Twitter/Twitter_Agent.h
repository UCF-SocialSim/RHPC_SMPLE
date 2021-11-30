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
/* Twitter_Agent.h */
#ifndef TWITTER_AGENT_H
#define TWITTER_AGENT_H


#include "Twitter_Definitions.h"
#include "Twitter_Feed.h"
#include "Twitter_Action.h"

namespace twitter{

template<typename SCORETYPE>
class TwitterResponse: public SocialNetwork_AbstractElement<TwitterAgentType, SCORETYPE>{
public:

	std::string nodeID;
	SocialNetwork_AbstractElement<TwitterAgentType, SCORETYPE>* parentPtr;     // Will be either a post or a comment; may be same as root
	SocialNetwork_AbstractElement<TwitterAgentType, SCORETYPE>* rootPtr;       // Will be a post
	int countOfDirectComments;
	int countOfAllComments;
	bool partOfSeededCascade;
	int indx;

	TwitterResponse(std::string ID,
			SocialNetwork_AbstractElement<TwitterAgentType, SCORETYPE>* creatorPtr,
			SocialNetwork_AbstractElement<TwitterAgentType, SCORETYPE>* parentPtr,
			SocialNetwork_AbstractElement<TwitterAgentType, SCORETYPE>* rootPtr,
				  bool partOfCascade);
	virtual ~TwitterResponse();

	void incrementAllParentsCounts();
};

template<typename SCORETYPE>
TwitterResponse<SCORETYPE>::TwitterResponse(std::string ID,
		SocialNetwork_AbstractElement<TwitterAgentType, SCORETYPE>* creatorPtr,
		SocialNetwork_AbstractElement<TwitterAgentType, SCORETYPE>* parentPtr,
		SocialNetwork_AbstractElement<TwitterAgentType, SCORETYPE>* rootPtr,
		  bool partOfCascade):
		SocialNetwork_AbstractElement<TwitterAgentType, SCORETYPE>(RESPONSE_ELEMENT, creatorPtr),
		nodeID(ID),
		parentPtr(parentPtr),
		rootPtr(rootPtr),
		countOfDirectComments(0),
		countOfAllComments(0),
		partOfSeededCascade(partOfCascade),
		indx(0){

//	// Increment the count of the parent's comments
//	if(parentPtr->getType() == RESPONSE_ELEMENT)
//		((TwitterResponse<SCORETYPE>*)parentPtr)->countOfDirectComments++;
//	else
//		((TwitterConversationAgent<SCORETYPE>*)parentPtr)->incrementCountOfDirectComments();
//
//	incrementAllParentsCounts();
}

template<typename SCORETYPE>
TwitterResponse<SCORETYPE>::~TwitterResponse(){

}

template<typename SCORETYPE>
void TwitterResponse<SCORETYPE>::incrementAllParentsCounts(){
//	if(parentPtr->getType() == RESPONSE_ELEMENT){
//		TwitterResponse* parent = ((TwitterResponse*)parentPtr);
//		parent->countOfAllComments++;
//		parent->incrementAllParentsCounts(); // Will recurse back to root
//	}
}

template<typename ACTIONTYPE, typename SCORETYPE>
class AbstractTwitterAgent: public SocialNetworkNode<TwitterAgentType, ACTIONTYPE, SCORETYPE>{
protected:

	Twitter*                                  twitter;
	boost::shared_ptr<TwitterFeed<SCORETYPE>> myFeed;

public:

	// Constructors:
	AbstractTwitterAgent(repast::AgentId id, TwitterAgentType whichType, ptime dateCreated, SNP* platform, SocialNetwork_AbstractElement<TwitterAgentType, SCORETYPE>* creator = 0);
    // Destructor
    virtual ~AbstractTwitterAgent();

    virtual void setCurrentRank(int currentRank);

    virtual std::string showName();
    virtual std::string writeName();

    virtual void setPlatform(SNP* platform);

    void placeInFeed(
    		SocialNetwork_AbstractElement<TwitterAgentType, SCORETYPE>* actor,
			SocialNetwork_AbstractElement<TwitterAgentType, SCORETYPE>* target,
			SocialNetwork_AbstractElement<TwitterAgentType, SCORETYPE>* subtarget,
			TwitterEventType action,
			TwitterFeedExplanation fe){
    	 	 myFeed->push(actor, target, subtarget, action, fe);
    }

    int getCountOfFeedElements(){
    	return myFeed->size();
    }

    virtual void selectTarget(ACTIONTYPE& action){};

};


template<typename ACTIONTYPE, typename SCORETYPE>
AbstractTwitterAgent<ACTIONTYPE, SCORETYPE>::AbstractTwitterAgent(repast::AgentId agentId, TwitterAgentType whichType, ptime dateCreated, SNP* platform, SocialNetwork_AbstractElement<TwitterAgentType, SCORETYPE>* creator):
		SocialNetworkNode<TwitterAgentType, ACTIONTYPE, SCORETYPE>(agentId, whichType, dateCreated, platform, creator),
		myFeed(new TwitterFeed<SCORETYPE>(1000000)){

	std::stringstream s;
	s << "TW" <<  agentId.hashcode();
	this->setHashTagID(s.str());
}

template<typename ACTIONTYPE, typename SCORETYPE>
AbstractTwitterAgent<ACTIONTYPE, SCORETYPE>::~AbstractTwitterAgent() {
}

template<typename ACTIONTYPE, typename SCORETYPE>
void AbstractTwitterAgent<ACTIONTYPE, SCORETYPE>::setCurrentRank(int currentRank){
	this->id.currentRank(currentRank);
}

template<typename ACTIONTYPE, typename SCORETYPE>
void AbstractTwitterAgent<ACTIONTYPE, SCORETYPE>::setPlatform(SNP* platform){
	SocialNetwork_AbstractNode<TwitterAgentType, SCORETYPE>::setPlatform(platform);
		twitter = (Twitter*)(this->containingPlatform);
}

template<typename ACTIONTYPE, typename SCORETYPE>
std::string AbstractTwitterAgent<ACTIONTYPE, SCORETYPE>::showName(){
	std::ostringstream ss;
	ss << this->getId().hashcode();
	return ss.str();
}

template<typename ACTIONTYPE, typename SCORETYPE>
std::string AbstractTwitterAgent<ACTIONTYPE, SCORETYPE>::writeName(){
	std::ostringstream ss;
	ss << this->getId().hashcode();
	return ss.str();
}

} // End namespace twitter

#endif
