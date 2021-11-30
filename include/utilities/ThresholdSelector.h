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
/* utilities/ThresholdSelector.h */
#ifndef THRESHOLDSELECTOR_H
#define THRESHOLDSELECTOR_H

#include <boost/mpi.hpp>

#include "sstream"

#include "repast_hpc/Random.h"

#include "utilities/Utilities.h"

namespace rhpc_smple {

/**
 * A random selector object that maintains a list
 * of selectable objects and their associated probabilities,
 * e.g.:
 *
 *    Object 1: .10
 *    Object 2: .15
 *    Object 3: .25
 *    Object 4: .50
 *
 * This is a variant of weighted selection, but instead of
 * weights, probabilities are passed. The list of
 * probabilities effectively defines a range, i.e.:
 *
 *    Object 1:    [0.00 , 0.10)
 *    Object 2:    [0.10 , 0.25)
 *    Object 3:    [0.25 , 0.50)
 *    Object 4:    [0.50 , 1.00)
 *
 * When one item is selected, a random throw is performed
 * to get a value 0 <= x < 1; the object selected is
 * the object whose range includes the value.
 *
 * For the multi selection, the elements returned are the
 * objects and their associated probabilities, provided the
 * probability is below a specific value that is randomly
 * selected. If the random throw is '0.21' for our example,
 * Objects 1 (prob = 0.1) and 2 (prob = 0.15) would be returned.
 * [Note that this is based on the probabilities, and not on
 * the summed probabilities of the sequence of elements.]
 */
template <typename T>
class ThresholdSelector{

private:
	struct Pair{
		double prob;
		T      item;
		Pair(double probability, T itemToReturn){
			prob = probability;
			item = itemToReturn;
		}

		void report(){
			LOG_INFO("general",  std::to_string(prob) + " = " + std::to_string(item));
		}
	};

protected:

	std::vector<Pair> probs;
	T                 defaultVal;


public:
	double lastSelectedProbability;       // For debugging, mainly; a record of the last 'throw'
	double probabilityOfLastSelectedItem; // The probability value of the pair selected
	T      lastItemSelected;              // The last item selected, for future retrieval


	ThresholdSelector(T defaultValue);
	~ThresholdSelector();

	void add(double prob, T item);

	T getRandom();
	void getRandomMulti(std::vector<T>& items, std::vector<double>& probabilities);

	virtual void report();

};

template<typename T>
ThresholdSelector<T>::ThresholdSelector(T defaultValue): defaultVal(defaultValue),
  lastItemSelected(defaultValue), lastSelectedProbability(0), probabilityOfLastSelectedItem(1){
}

template<typename T>
ThresholdSelector<T>::~ThresholdSelector(){

}

template<typename T>
void ThresholdSelector<T>::add(double prob, T item){
  if(prob <= 0) return; // A zero value is just not added
  Pair pair(prob, item);
  typename std::vector<Pair>::iterator iter = probs.begin();
  while((iter != probs.end()) && (*iter).prob > prob) iter++;
  if(iter == probs.end()) probs.push_back(pair);
  else                    probs.insert(iter, pair);
}


template<typename T>
T ThresholdSelector<T>::getRandom(){
	if(probs.size() == 0){
		lastItemSelected              = defaultVal;
		probabilityOfLastSelectedItem = 1;
		lastSelectedProbability       = 1;
	}
	else{
		double lastSelectedProbability = repast::Random::instance()->nextDouble();
		int count = 0;
		typename std::vector<Pair>::iterator iter = probs.begin();
		while((iter != probs.end()) && (lastSelectedProbability <= (*iter).prob)){
			count++;
			iter++;
		}
		if(count == 0){
			probabilityOfLastSelectedItem = 1;
			lastItemSelected              = defaultVal;
		}
		else{
			int indx = std::floor(repast::Random::instance()->nextDouble()  * count);
			probabilityOfLastSelectedItem = probs[indx].prob;
			lastItemSelected              = probs[indx].item;
		}
	}
	return lastItemSelected;
}

template<typename T>
void ThresholdSelector<T>::getRandomMulti(std::vector<T>& items, std::vector<double>& probabilities){
	items.clear();
	probabilities.clear();
	if(probs.size() == 0){
		lastItemSelected              = defaultVal;
		probabilityOfLastSelectedItem = 1;
		lastSelectedProbability       = 1;
		items.push_back(defaultVal);
		probabilities.push_back(defaultVal);
	}
	else{
		lastSelectedProbability = repast::Random::instance()->nextDouble();
		typename std::vector<Pair>::iterator iter = probs.begin();
		while((iter != probs.end()) && (lastSelectedProbability <= (*iter).prob)){
			items.push_back(iter->item);
			probabilities.push_back(iter->prob);
			iter++;
		}
	}
}

template<typename T>
void ThresholdSelector<T>::report(){
	LOG_INFO("general", "---");
	for(int i = 0; i < probs.size(); i++) probs[i].report();
	LOG_INFO("general", "---");
}


}  // End Namespace rhpc_smple

#endif
