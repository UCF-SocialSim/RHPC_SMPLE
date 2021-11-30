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
/* Telegram_Agent.h */
#ifndef INCLUDE_TELEGRAM_AGENT_H
#define INCLUDE_TELEGRAM_AGENT_H

#include "socialnetwork_toolkit/SocialNetwork_Platform.h"

#include "Telegram_Definitions.h"
#include "Telegram_Feed.h"
#include "Telegram_Action.h"

using namespace rhpc_smple;

namespace telegram{


template<typename SCORETYPE>
class TelegramResponse: public SocialNetwork_AbstractElement<TelegramAgentType, SCORETYPE>{
public:

	std::string nodeID;
	SocialNetwork_AbstractElement<TelegramAgentType, SCORETYPE>* parentPtr;     // Will be either a post or a comment; may be same as root
	SocialNetwork_AbstractElement<TelegramAgentType, SCORETYPE>* rootPtr;       // Will be a posty
	int countOfDirectComments;
	int countOfAllComments;
	bool partOfSeededCascade;
	int indx;

	TelegramResponse(std::string ID,
			SocialNetwork_AbstractElement<TelegramAgentType, SCORETYPE>* creatorPtr,
			SocialNetwork_AbstractElement<TelegramAgentType, SCORETYPE>* parentPtr,
			SocialNetwork_AbstractElement<TelegramAgentType, SCORETYPE>* rootPtr,
				  bool partOfCascade);
	virtual ~TelegramResponse();

	void incrementAllParentsCounts();
};


template<typename SCORETYPE>
TelegramResponse<SCORETYPE>::TelegramResponse(std::string ID,
		SocialNetwork_AbstractElement<TelegramAgentType, SCORETYPE>* creatorPtr,
		SocialNetwork_AbstractElement<TelegramAgentType, SCORETYPE>* parentPtr,
		SocialNetwork_AbstractElement<TelegramAgentType, SCORETYPE>* rootPtr,
		  bool partOfCascade):
		SocialNetwork_AbstractElement<TelegramAgentType, SCORETYPE>(RESPONSE_ELEMENT, creatorPtr),
		nodeID(ID),
		parentPtr(parentPtr),
		rootPtr(rootPtr),
		countOfDirectComments(0),
		countOfAllComments(0),
		partOfSeededCascade(partOfCascade),
		indx(0){

//	// Increment the count of the parent's comments
//	if(parentPtr->getType() == RESPONSE_ELEMENT)
//		((TelegramResponse*)parentPtr)->countOfDirectComments++;
//	else
//		((TelegramConversationAgent*)parentPtr)->incrementCountOfDirectComments();

//	incrementAllParentsCounts();
}

template<typename SCORETYPE>
TelegramResponse<SCORETYPE>::~TelegramResponse(){

}

template<typename SCORETYPE>
void TelegramResponse<SCORETYPE>::incrementAllParentsCounts(){
	if(parentPtr->getType() == RESPONSE_ELEMENT){
		TelegramResponse* parent = ((TelegramResponse*)parentPtr);
		parent->countOfAllComments++;
		parent->incrementAllParentsCounts(); // Will recurse back to root
	}
}


template<typename ACTIONTYPE, typename SCORETYPE>
class AbstractTelegramAgent: public SocialNetworkNode<TelegramAgentType, ACTIONTYPE, SCORETYPE>{
public:
	Telegram*     telegram;
	boost::shared_ptr<TelegramFeed<SCORETYPE>> myFeed;

	AbstractTelegramAgent(repast::AgentId id, TelegramAgentType whichType, ptime dateCreated, SNP* platform, SocialNetwork_AbstractElement<TelegramAgentType, SCORETYPE>* creator = 0);
	virtual ~AbstractTelegramAgent();

	virtual void setCurrentRank(int currentRank);


    virtual std::string showName();
    virtual std::string writeName();

	virtual void setPlatform(SNP* platform);

    void placeInFeed(    		SocialNetwork_AbstractElement<TelegramAgentType, SCORETYPE>* actor,
			SocialNetwork_AbstractElement<TelegramAgentType, SCORETYPE>* target,
			SocialNetwork_AbstractElement<TelegramAgentType, SCORETYPE>* subtarget,
			TelegramEventType action,
			TelegramFeedExplanation fe){
    	  myFeed->push(actor, target, subtarget, action, fe);
    }

    int getCountOfFeedElements(){
    	return myFeed->size();
    }

    virtual void selectTarget(ACTIONTYPE& action);


};


template<typename ACTIONTYPE, typename SCORETYPE>
AbstractTelegramAgent<ACTIONTYPE, SCORETYPE>::AbstractTelegramAgent(repast::AgentId agentId, TelegramAgentType whichType, ptime dateCreated, SNP* platform, SocialNetwork_AbstractElement<TelegramAgentType, SCORETYPE>* creator):
		SocialNetworkNode<TelegramAgentType, ACTIONTYPE, SCORETYPE>(agentId, whichType, dateCreated, platform, creator),
		myFeed(new TelegramFeed<SCORETYPE>()) {
	std::stringstream s;
	s << "TG" << agentId.hashcode();
	this->setHashTagID(s.str());
}


template<typename ACTIONTYPE, typename SCORETYPE>
AbstractTelegramAgent<ACTIONTYPE, SCORETYPE>::~AbstractTelegramAgent() {
}

template<typename ACTIONTYPE, typename SCORETYPE>
std::string AbstractTelegramAgent<ACTIONTYPE, SCORETYPE>::showName(){
	std::ostringstream ss;
	ss << this->getId().hashcode();
	return ss.str();
}

template<typename ACTIONTYPE, typename SCORETYPE>
std::string AbstractTelegramAgent<ACTIONTYPE, SCORETYPE>::writeName(){
	std::ostringstream ss;
	ss << this->getId().hashcode();
	return ss.str();
}
template<typename ACTIONTYPE, typename SCORETYPE>
void AbstractTelegramAgent<ACTIONTYPE, SCORETYPE>::setCurrentRank(int currentRank){
	this->id.currentRank(currentRank);
}

template<typename ACTIONTYPE, typename SCORETYPE>
void AbstractTelegramAgent<ACTIONTYPE, SCORETYPE>::setPlatform(SNP* platform){
	SocialNetwork_AbstractNode<TelegramAgentType, SCORETYPE>::setPlatform(platform);
		telegram = (Telegram*)(this->containingPlatform);
}

template<typename ACTIONTYPE, typename SCORETYPE>
void AbstractTelegramAgent<ACTIONTYPE, SCORETYPE>::selectTarget(ACTIONTYPE& action){

}





} // End namespace telegram

#endif /* INCLUDE_TELEGRAM_AGENT_H */
