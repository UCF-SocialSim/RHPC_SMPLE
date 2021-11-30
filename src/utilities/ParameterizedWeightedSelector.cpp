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
/* ParameterizedWeightedSelector.cpp */
#include "utilities/ParameterizedWeightedSelector.h"

#include "math.h"

using namespace rhpc_smple;

ParameterizedWeightedSelector::ParameterizedWeightedSelector(int defaultValue, long multCoeff, long expPower, long expectedUpperBound):
    multCoeff(multCoeff), expPower(expPower), expectedUpperBound(expectedUpperBound), SimpleWeightedSelector(defaultValue){ }

ParameterizedWeightedSelector::~ParameterizedWeightedSelector(){ }

long ParameterizedWeightedSelector::getConvertedValue(double value){
   return (long)(pow((value/(double)expectedUpperBound), expPower) * multCoeff);
}

PWSFactory::PWSFactory() {
}

PWSFactory::~PWSFactory() {
    delete instance;
}

PWSFactory* PWSFactory::instance;

void PWSFactory::initValues(std::string platform, std::string type, int defaultValue, long multCoeff, long expPower, long expectedUpperBound) {
    std::unordered_map<std::string, PWSValues> res;
    PWSValues p;
    p.defaultValue = defaultValue;
    p.multCoeff = multCoeff;
    p.expPower = expPower;
    p.expectedUpperBound = expectedUpperBound;
    dict[platform][type] = p;
}

ParameterizedWeightedSelector PWSFactory::getParameterizedWeightedSelector(std::string platform, std::string type) {
    PWSValues vals = dict[platform][type];
    return ParameterizedWeightedSelector(vals.defaultValue, vals.multCoeff, vals.expPower, vals.expectedUpperBound);

}

PWSFactory* PWSFactory::getInstance() {
    if (!instance){
        instance = new PWSFactory;
    }
    return instance;
}
