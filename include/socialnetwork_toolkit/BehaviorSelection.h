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
/* BehaviorSelection.h */
#ifndef BEHAVIOR_SELECTION_H
#define BEHAVIOR_SELECTION_H

#include "sstream"

#include "utilities/ThresholdSelector.h"

namespace rhpc_smple {

/*
 * Notes:
 *
 * Behavior Selectors are classes that are given an actor
 * and return a list of actions that the actor will undertake.
 *
 * This file contains a definition for an abstract class 'Behavior Selector' and
 * several implementations:
 *
 *   ThresholdBehaviorSelector
 *   ExtendedThresholdBehaviorSelector
 *   ExtendedMultiThresholdBehaviorSelector
 *   RandomUniformBehaviorSeletor
 *
 * The base class is abstract and takes the following templated types:
 *
 * AGENT  Type for agent
 * ACTION Type for actions
 *
 */
template<typename AGENT, typename ACTION>
class BehaviorSelector{
public:
	BehaviorSelector(){}
	virtual ~BehaviorSelector(){}

	virtual void getBehavior(AGENT* actor, std::vector<ACTION>& actions) = 0;
};


/**
 * See BehaviorSelection for AGENT and ACTION; EVENT is the type of event to be considered
 */
template<typename AGENT, typename ACTION, typename EVENT>
class ThresholdBehaviorSelector:
		public ThresholdSelector<EVENT>,
		public BehaviorSelector<AGENT, ACTION>{
public:
	ThresholdBehaviorSelector(EVENT defaultEvent);
	virtual ~ThresholdBehaviorSelector();

	virtual void getBehavior(AGENT* actor, std::vector<ACTION>& actions);

	virtual void report(){
		LOG_INFO("general", "---");
		std::stringstream s;
		for(int i = 0; i < ThresholdSelector<EVENT>::probs.size(); i++) s << ThresholdSelector<EVENT>::probs[i].prob << " = " << ThresholdSelector<EVENT>::probs[i].item << std::endl;
		LOG_INFO("general", s.str());
		LOG_INFO("general", "---");
	}
};


template<typename AGENT, typename ACTION, typename EVENT>
ThresholdBehaviorSelector<AGENT, ACTION, EVENT>::ThresholdBehaviorSelector(EVENT defaultEvent): ThresholdSelector<EVENT>(defaultEvent){}

template<typename AGENT, typename ACTION, typename EVENT>
ThresholdBehaviorSelector<AGENT, ACTION, EVENT>::~ThresholdBehaviorSelector(){}

template<typename AGENT, typename ACTION, typename EVENT>
void ThresholdBehaviorSelector<AGENT, ACTION, EVENT>::getBehavior(AGENT* actor, std::vector<ACTION>& actions){
	actions.emplace_back(ThresholdSelector<EVENT>::getRandom(), actor); // NB: Used to not place 'Git- IDLE', now will place everything; should filter IDLE later
}




template<typename AGENT, typename ACTION, typename EVENT>
class RandomUniformBehaviorSelector: public BehaviorSelector<AGENT, ACTION>{

	EVENT* behaviors;
	int    behaviorCount;

public:
	RandomUniformBehaviorSelector(std::vector<EVENT> events);
	virtual ~RandomUniformBehaviorSelector();

	virtual void getBehavior(AGENT* actor, std::vector<ACTION>& actions);

};


template<typename AGENT, typename ACTION, typename EVENT>
RandomUniformBehaviorSelector<AGENT, ACTION, EVENT>::RandomUniformBehaviorSelector(std::vector<EVENT> events):
        behaviorCount(events.size()){
	behaviors = new EVENT[behaviorCount];
	for(int i = 0; i < behaviorCount; i++) behaviors[i] = events[i];
}

template<typename AGENT, typename ACTION, typename EVENT>
RandomUniformBehaviorSelector<AGENT, ACTION, EVENT>::~RandomUniformBehaviorSelector(){
	delete behaviors;
}

template<typename AGENT, typename ACTION, typename EVENT>
void RandomUniformBehaviorSelector<AGENT, ACTION, EVENT>::getBehavior(AGENT* actor, std::vector<ACTION>& actions){
	actions.emplace_back(behaviors[(int)(floor(repast::Random::instance()->nextDouble() * behaviorCount))], actor);
}






template<typename AGENT, typename ACTION, typename EVENT>
class ExtendedThresholdBehaviorSelector:
		public ThresholdBehaviorSelector<AGENT, ACTION, EVENT>{
public:
	ExtendedThresholdBehaviorSelector(EVENT defaultEvent);
	virtual ~ExtendedThresholdBehaviorSelector();

	virtual void getBehavior(AGENT* actor, std::vector<ACTION>& actions);
};





template<typename AGENT, typename ACTION, typename EVENT>
ExtendedThresholdBehaviorSelector<AGENT, ACTION, EVENT>::ExtendedThresholdBehaviorSelector(EVENT defaultEvent):
	ThresholdBehaviorSelector<AGENT, ACTION, EVENT>(defaultEvent){}

template<typename AGENT, typename ACTION, typename EVENT>
ExtendedThresholdBehaviorSelector<AGENT, ACTION, EVENT>::~ExtendedThresholdBehaviorSelector(){}


template<typename AGENT, typename ACTION, typename EVENT>
void ExtendedThresholdBehaviorSelector<AGENT, ACTION, EVENT>::getBehavior(AGENT* actor, std::vector<ACTION>& actions){
	EVENT toDo = ThresholdSelector<EVENT>::getRandom();
	if(toDo != ThresholdSelector<EVENT>::defaultVal){
		actions.emplace_back(toDo, actor);
		int prob = (int)(ThresholdSelector<EVENT>::probabilityOfLastSelectedItem);
		while(prob > 1){
			actions.emplace_back(toDo, actor);
			prob--;
		}
	}
}






template<typename AGENT, typename ACTION, typename EVENT>
class ExtendedMultiThresholdBehaviorSelector:
		public ThresholdBehaviorSelector<AGENT, ACTION, EVENT>{
public:
	ExtendedMultiThresholdBehaviorSelector(EVENT defaultEvent);
	virtual ~ExtendedMultiThresholdBehaviorSelector();

	virtual void getBehavior(AGENT* actor, std::vector<ACTION>& actions);
};


template<typename AGENT, typename ACTION, typename EVENT>
ExtendedMultiThresholdBehaviorSelector<AGENT, ACTION, EVENT>::ExtendedMultiThresholdBehaviorSelector(EVENT defaultEvent):
	ThresholdBehaviorSelector<AGENT, ACTION, EVENT>(defaultEvent){}

template<typename AGENT, typename ACTION, typename EVENT>
ExtendedMultiThresholdBehaviorSelector<AGENT, ACTION, EVENT>::~ExtendedMultiThresholdBehaviorSelector(){}

template<typename AGENT, typename ACTION, typename EVENT>
void ExtendedMultiThresholdBehaviorSelector<AGENT, ACTION, EVENT>::getBehavior(AGENT* actor, std::vector<ACTION>& actions){
	std::vector<EVENT> items;
	std::vector<double> ps;
	ThresholdSelector<EVENT>::getRandomMulti(items, ps);
	for(int i = 0; i < ps.size(); i++){
		double prob = ps[i];
		while(prob > 1){
			actions.emplace_back(items[i], actor);
			prob -= 1;
		}
		if(ThresholdSelector<EVENT>::lastSelectedProbability < prob) actions.emplace_back(items[i], actor);
	}
}

}  // End Namespace rhpc_smple

#endif
