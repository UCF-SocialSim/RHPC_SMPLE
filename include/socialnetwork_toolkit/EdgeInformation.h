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
/* EdgeInformation.h */
#ifndef INCLUDE_SOCIALNETWORK_TOOLKIT_EDGEINFORMATION_H
#define INCLUDE_SOCIALNETWORK_TOOLKIT_EDGEINFORMATION_H

#include "socialnetwork_toolkit/EventCounter.h"

namespace rhpc_smple {

template<typename R>
class SocialRelationshipIndicator: public EventCounter<R, EventCount<bool>, bool>{
public:
	SocialRelationshipIndicator(int size);
	~SocialRelationshipIndicator();
};

template<typename R>
SocialRelationshipIndicator<R>::SocialRelationshipIndicator(int size):
  EventCounter<R, EventCount<bool>, bool>(size){}

template<typename R>
SocialRelationshipIndicator<R>::~SocialRelationshipIndicator(){}



template<typename E>
class SocialEventCounter: public EventCounter<E, EventCount<int>, int>{
public:
	SocialEventCounter(int size);
	~SocialEventCounter();
};


template<typename E>
SocialEventCounter<E>::SocialEventCounter(int size):
  EventCounter<E, EventCount<int>, int>(size){}

template<typename E>
SocialEventCounter<E>::~SocialEventCounter(){}



template<typename I>
class SocialInfluenceMeasures: public EventCounter<I, EventCount<double>, double>{
public:
	SocialInfluenceMeasures(int size);
	~SocialInfluenceMeasures();
};

template<typename I>
SocialInfluenceMeasures<I>::SocialInfluenceMeasures(int size):
  EventCounter<I, EventCount<double>, double>(size){
}

template<typename I>
SocialInfluenceMeasures<I>::~SocialInfluenceMeasures(){}




template<typename F, typename FC>
class SocialNetwork_FlagEdgeInformation{
	friend class boost::serialization::access;

public:
	FC flags;

	SocialNetwork_FlagEdgeInformation(){};
	virtual ~SocialNetwork_FlagEdgeInformation(){};

	virtual void setFlag(F flag, bool newVal = true){
		flags.set(flag, newVal);
	}
	virtual bool getFlag(F flag){
		return flags.getValue(flag);
	}

	virtual bool isEmpty(){
		return flags.isEmpty();
	}

	template<class Archive>
	void serialize(Archive& ar, const unsigned int version){
		ar& flags;
	}
};


template<typename E, typename EC>
class SocialNetwork_CountEdgeInformation{
	friend class boost::serialization::access;

public:
	EC eventCounter;

	SocialNetwork_CountEdgeInformation(){};
	virtual ~SocialNetwork_CountEdgeInformation(){};

	virtual void recordEvent(E event){
		eventCounter.log(event);
	}
	virtual int getCount(E event){
		return eventCounter.getValue(event);
	}

	virtual bool isEmpty(){
		return eventCounter.isEmpty();
	}

	template<class Archive>
	void serialize(Archive& ar, const unsigned int version){
		ar& eventCounter;
	}
};


template<typename I, typename IC>
class SocialNetwork_InfluenceEdgeInformation{
	friend class boost::serialization::access;

public:
	IC influenceCounter;

	SocialNetwork_InfluenceEdgeInformation(){};
	virtual ~SocialNetwork_InfluenceEdgeInformation(){};

	virtual void setInfluence(I influence, double newVal){
		influenceCounter.set(influence, newVal);
	}

	virtual int getInfluence(I influenceType){
		return influenceCounter.getValue(influenceType);
	}

	virtual bool isEmpty(){
		return true; // This is to keep backwards compatibility- check to be sure it's OK.
	}

	template<class Archive>
	void serialize(Archive& ar, const unsigned int version){
		ar& influenceCounter;
	}
};

}  // End Namespace rhpc_smple

#endif /* INCLUDE_SOCIALNETWORK_TOOLKIT_EDGEINFORMATION_H */
