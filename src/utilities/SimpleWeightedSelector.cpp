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
/* SimpleWeightedSelector.cpp */
#include "utilities/SimpleWeightedSelector.h"

#include "repast_hpc/Random.h"

using namespace rhpc_smple;

SimpleWeightedSelector::SimpleWeightedSelector(int defaultValue): sum(0), defaultVal(defaultValue){}

SimpleWeightedSelector::~SimpleWeightedSelector(){}

void SimpleWeightedSelector::addValue(double value){
    long valToAdd = getConvertedValue(value);
    components.push_back(valToAdd);
    sum += valToAdd;
}

long SimpleWeightedSelector::getConvertedValue(double value){
    return (long)value;
}

int SimpleWeightedSelector::select(){
	if(components.size() == 0) return defaultVal;

	double rnd = repast::Random::instance()->nextDouble();

	long choice = (long)(rnd * (double)sum);
    for (int indx = 0; indx < components.size(); indx++){
		choice -= components[indx];
		if (choice < 0)	return indx;
	}

	return (int)(repast::Random::instance()->nextDouble() * ((double)components.size()));
}

long SimpleWeightedSelector::at(int indx){
	return (indx < 0 || indx >= components.size()) ? defaultVal : components.at(indx);
}

int SimpleWeightedSelector::getDefaultValue(){
	return defaultVal;
}

int SimpleWeightedSelector::size(){
	return components.size();
}

long SimpleWeightedSelector::adjust(int indx, long addend){
	if(indx < 0 || indx >= components.size()) return sum;
	long origVal = components[indx];
	long* toChange = &(components.at(indx));
	*toChange = *toChange + addend;
	if(*toChange < 0) *toChange = 0;
	long newVal = components[indx];
	sum += (newVal - origVal);
	return sum;
}

long SimpleWeightedSelector::decrement(int indx){
	return adjust(indx, -1);
}

long SimpleWeightedSelector::increment(int indx){
	return adjust(indx, 1);
}

void SimpleWeightedSelector::clear(){
	components.clear();
	sum = 0;
}

long SimpleWeightedSelector::clearAt(int indx){
	return adjust(indx, -1 * at(indx));
}

long SimpleWeightedSelector::getCountOfElementsWithZeroValues(){
	long ret = 0;
	for(int i = 0; i < size(); i++) if(at(i) == 0) ret++;
	return ret;
}

