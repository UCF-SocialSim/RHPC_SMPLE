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
/* Feed.h*/
#ifndef INCLUDE_FEED_H
#define INCLUDE_FEED_H

#include <deque>

#include "utilities/SimpleWeightedSelector.h"
#include "utilities/ParameterizedWeightedSelector.h"

#include "socialnetwork_toolkit/Payload.h"
#include "socialnetwork_toolkit/SocialNetwork_AbstractElement.h"

namespace rhpc_smple {

class Explanation;    // Forward Declaration

// Social network platforms provide a 'feed' (like a newsfeed)
// That is always a list of elements and explanations.
template<typename AgentTypeList, typename ActionTypeList, typename SCORETYPE, typename FE = Explanation>
class FeedElement{

public:
	SocialNetwork_AbstractElement<AgentTypeList, SCORETYPE>* actor;
	SocialNetwork_AbstractElement<AgentTypeList, SCORETYPE>* target;
	SocialNetwork_AbstractElement<AgentTypeList, SCORETYPE>* subtarget; // E.g. Comment, Response
	ActionTypeList                                           action;

	SN_ActionPayload                           		         payload;
	FE                                         			     feedExplanation;

	FeedElement(SocialNetwork_AbstractElement<AgentTypeList, SCORETYPE>* actor,
			SocialNetwork_AbstractElement<AgentTypeList, SCORETYPE>* target,
			SocialNetwork_AbstractElement<AgentTypeList, SCORETYPE>* subtarget,
			ActionTypeList whichAction,
			FE expl,
			SN_ActionPayload payload = SN_ActionPayload::DUMMY_PAYLOAD):
		actor(actor), target(target), subtarget(subtarget), action(whichAction), feedExplanation(expl), payload(payload){}
	virtual ~FeedElement(){}
};


// Templates:
//    AgentTypeList   = list of types of agents, e.g. REPO_AGENT
//    ActionTypeList  = list of event types
//    SCORETYPE       = the types of scores this agent can generate
//    FE              = Feed Explanation
template<typename AgentTypeList, typename ActionTypeList, typename SCORETYPE, typename FE = Explanation>
class Feed{

protected:
	SimpleWeightedSelector*                                                   ws;

public:
	std::deque<FeedElement<AgentTypeList, ActionTypeList,  SCORETYPE, FE> >*  feedElements;
	int                                                                       maxSize;

public:
	Feed(int size = 10000):maxSize(size), ws(0){
		feedElements = new std::deque<FeedElement<AgentTypeList, ActionTypeList, SCORETYPE, FE> >();
	}

	virtual ~Feed(){
		delete ws;
		delete feedElements;
	}

	void push(
			SocialNetwork_AbstractElement<AgentTypeList, SCORETYPE>* actor,
			SocialNetwork_AbstractElement<AgentTypeList, SCORETYPE>* target,
			SocialNetwork_AbstractElement<AgentTypeList, SCORETYPE>* subtarget,
			ActionTypeList action,
			FE feedExplanation,
			SN_ActionPayload payload = SN_ActionPayload::DUMMY_PAYLOAD);

	void push(FeedElement<AgentTypeList, ActionTypeList, SCORETYPE, FE> element);

	int size(){  return feedElements->size(); }

	void trimOldest();

	void setMaxSize(int newSize){
		maxSize = newSize;
		trimOldest();
	}

	FeedElement<AgentTypeList, ActionTypeList, SCORETYPE, FE>& newest(){
		return (*(feedElements->rbegin()));
	}

	void popNewest(){
		feedElements->pop_back();
	}

	FeedElement<AgentTypeList, ActionTypeList, SCORETYPE, FE>& oldest(){
		return (*(feedElements->begin()));
	}

	void popOldest(){
		feedElements->pop_front();
	}

	FeedElement<AgentTypeList, ActionTypeList, SCORETYPE, FE>& at(int indx){
		return (feedElements->at(indx));
	}

	void eraseAt(int indx){
		typename std::deque<FeedElement<AgentTypeList, ActionTypeList, SCORETYPE, FE> >::iterator iter = feedElements->begin();
		for(int i = 0; i < indx; i++) iter++;
		feedElements->erase(iter);
	}

	void clear(){
		feedElements->clear();
	}

	int getMaxSize(){
		return maxSize;
	}

	virtual void initiateWeightedSampling(SCORETYPE type, long multCoeff = -1, long expPower = -1, long expectedUpperBound = -1){
		if(ws != 0) delete ws;
		if(multCoeff == -1 && expPower == -1 && expectedUpperBound == -1){
			ws = new ParameterizedWeightedSelector(-1, multCoeff, expPower, expectedUpperBound);
		}
		else{
			ws = new SimpleWeightedSelector(-1);
		}
		typename std::deque<FeedElement<AgentTypeList, ActionTypeList, SCORETYPE, FE> >::iterator iter = feedElements->begin();
		while(iter != this->feedElements->end()){
			ws->addValue(((*iter).target)->getScore(type));
			iter++;
		}
	}

	virtual FeedElement<AgentTypeList, ActionTypeList, SCORETYPE, FE> getOneSelectedWeighted(){
		return feedElements->at(ws->select());
	}

};

template<typename AgentTypeList, typename ActionTypeList, typename SCORETYPE, typename FE>
void Feed<AgentTypeList, ActionTypeList, SCORETYPE, FE>::trimOldest(){
	while(feedElements->size() > maxSize) feedElements->pop_front();
}

template<typename AgentTypeList, typename ActionTypeList, typename SCORETYPE, typename FE>
void Feed<AgentTypeList, ActionTypeList, SCORETYPE, FE>::push(
			SocialNetwork_AbstractElement<AgentTypeList, SCORETYPE>* actor,
			SocialNetwork_AbstractElement<AgentTypeList, SCORETYPE>* target,
			SocialNetwork_AbstractElement<AgentTypeList, SCORETYPE>* subtarget,
			ActionTypeList action,
			FE feedExplanation,
			SN_ActionPayload payload			){
	FeedElement<AgentTypeList, ActionTypeList, SCORETYPE, FE> newElement(actor, target, subtarget, action, feedExplanation, payload);
	push(newElement);
}

template<typename AgentTypeList, typename ActionTypeList, typename SCORETYPE, typename FE>
void Feed<AgentTypeList, ActionTypeList, SCORETYPE, FE>::push(FeedElement<AgentTypeList, ActionTypeList, SCORETYPE, FE> element){
	feedElements->push_back(element);
	trimOldest();
}

}  // End Namespace rhpc_smple

#endif /* INCLUDE_FEED_H */
