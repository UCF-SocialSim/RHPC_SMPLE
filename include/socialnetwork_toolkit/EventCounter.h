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
/* EventCounter.h */
#ifndef EVENT_COUNTER_H
#define EVENT_COUNTER_H

#include <iostream>
#include <vector>

#include <boost/serialization/utility.hpp>

namespace rhpc_smple {

template<typename R>
class EventCount{
	friend class boost::serialization::access;

private:
	R count;

public:
	EventCount(R initialVal = 0): count(initialVal){}
	virtual ~EventCount(){}

	// Setter
	virtual void setCount(R newCount){	count = newCount; }

	// Direct access
	virtual R* getPtr(){   return &count; }

	// Getter
	virtual R getValue(){  return count; }

	// Increment
	virtual R increment(){ return ++count; }

	// Clear
	virtual void clear(){ count = 0; }

	// Add
	virtual R add(EventCount other){ return count += other.count; }

	template<class Archive>
	void serialize(Archive& ar, const unsigned int version) {
		ar & count;
	}

};


template<>
class EventCount<bool>{
	friend class boost::serialization::access;

private:
	bool count;

public:
	EventCount(bool initialVal = false): count(initialVal){}
	virtual ~EventCount(){}

	// Setter
	virtual void setCount(bool newCount){	count = newCount; }

	// Direct access
	virtual bool* getPtr(){     return &count; }

	// Getter
	virtual int getValue(){		return count; }

	// Increment
	virtual int increment(){    return count = !count; }

	// Clear
	virtual void clear(){ count = false; }

	// Add
	virtual int add(EventCount other){ return count || other.count; }

	template<class Archive>
	void serialize(Archive& ar, const unsigned int version) {
		ar & count;
	}

};


// Most social networks will need to record how many
// times a Source performed an event on a Target,
// as well as certain other information (flags), and will
// need a data structure in their links to do this.
//
//  T = type of events or flags
//  EC = Type of the Event Count to be used
//
// Example: EventCounter<GitHubEvent, TimeStampedEventCount>
//
// Construction must provide number of different events to be recorded,
// e.g. "EventCounter<GitHubEvent, EventCount>(14)" because GitHub
// has 14 events.
template<typename T, typename EC, typename ECDatumType>
class EventCounter{
	friend class boost::serialization::access;
	
private:
	std::vector<EC> logArray;
	bool            _isEmpty;

public:
	EventCounter(int size);
	EventCounter(int size, T event);
	virtual ~EventCounter();

	// Getters
	ECDatumType* getPtr(T event);
	ECDatumType  getValue(T event);
	bool isEmpty();
	ECDatumType getInfluence(T event);

	// Setters
	void set(T event, ECDatumType newVal);
	void clear();
	void add(EventCounter<T, EC, ECDatumType> other);

	// Actions
	void log(T event);


	template<class Archive>
	void serialize(Archive& ar, const unsigned int version) {
		ar & logArray;
		ar & _isEmpty;
	}
	

};

template<typename T, typename EC, typename ECDatumType>
EventCounter<T, EC, ECDatumType>::EventCounter(int size) : logArray(size), _isEmpty(true){ }

template<typename T, typename EC, typename ECDatumType>
EventCounter<T, EC, ECDatumType>::EventCounter(int size, T event) : logArray(size), _isEmpty(true){
	log(event);
}

template<typename T, typename EC, typename ECDatumType>
EventCounter<T, EC, ECDatumType>::~EventCounter(){}


template<typename T, typename EC, typename ECDatumType>
ECDatumType* EventCounter<T, EC, ECDatumType>::getPtr(T event){
	return logArray[(int)event].getPtr();
}

template<typename T, typename EC, typename ECDatumType>
ECDatumType EventCounter<T, EC, ECDatumType>::getValue(T event){
	return logArray[(int)event].getValue();
}

template<typename T, typename EC, typename ECDatumType>
bool EventCounter<T, EC, ECDatumType>::isEmpty(){
	return _isEmpty;
}

template<typename T, typename EC, typename ECDatumType>
void EventCounter<T, EC, ECDatumType>::set(T event, ECDatumType newVal){
	logArray[(int)event].setCount(newVal);
}

template<typename T, typename EC, typename ECDatumType>
ECDatumType EventCounter<T, EC, ECDatumType>::getInfluence(T event){
	return logArray[(int)event].getValue();
}

template<typename T, typename EC, typename ECDatumType>
void EventCounter<T, EC, ECDatumType>::clear(){
	for(int i = 0; i < logArray.size(); i++) logArray[i].clear();
	_isEmpty = true;
}

template<typename T, typename EC, typename ECDatumType>
void EventCounter<T, EC, ECDatumType>::add(EventCounter<T, EC, ECDatumType> other){
	for(int i = 0; i < logArray.size(); i++){
		if(logArray[i].add(other.logArray[i]) != 0) _isEmpty = false ;
	}
}

template<typename T, typename EC, typename ECDatumType>
void EventCounter<T, EC, ECDatumType>::log(T event){
	_isEmpty = false;
	logArray[(int)event].increment();
}

}  // End Namespace rhpc_smple

#endif

