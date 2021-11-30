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
/* Parameterized Weighted Selector.h */
#ifndef PARAMETERIZED_WEIGHTED_SELECTOR
#define PARAMETERIZED_WEIGHTED_SELECTOR

#include <unordered_map>

#include "SimpleWeightedSelector.h"

namespace rhpc_smple {

/*
 * This class provides a way to parameterize selection, by which
 * is meant that the base scores passed to it to be used as weights
 * are adjusted mathematically to 'bend' the weighting system.
 *
 * For example, assume two elements with baseline weights of 1 and 2.
 * In normal selection, the probability of selection would be the weight
 * divided by the simple sum of weights, hence the first element would
 * be selected with probability 1/3 and the second with probability 2/3.
 *
 * Suppose instead we parameterized this by applying an exponent of 2 to
 * every incoming weight. This changes the weighting from 1 and 2 to
 * 1 * 1 and 2 * 2, or 1 and 4. This means that the selection likelihoods
 * are now 1/5 and 4/5.
 *
 * In theory these transformations can all be applied prior to sending
 * the weights to the selector; the selection algorithm is always the
 * same simple weighting mechanism, and the adjustment of scores is
 * performed as the elements upon arrival as elements are added.
 *
 * Some care must be taken to use this class properly, because it
 * permits more than one kind of adjustment. Adjustment is specified
 * via three parameters:
 *
 *    multCoeff = Multiplier Coefficient: all incoming scores are
 *         multiplied by this
 *    expPower  = A power to which all incoming scores are raised
 *    expectedUpperBound = a value by which every incoming score is
 *         divided
 *
 * These operations occur in order:
 *
 *    The incoming value is divided by the upper bound
 *    The result of this is raised to the expPower
 *    The result of this is multiplied by the multCoeff
 *
 * Some notes to keep in mind:
 *
 *     - If any of these values is '1', it is effectively irrelevant
 *     - No check is made to ensure that the upper bound is not zero;
 *          a zero value will cause a division by zero error
 *     - If only the multCoeff is used, all incoming values are
 *          multiplied by this as a scalar. If the incoming values
 *          are integers, this is a meaningless transformation,
 *          as it will impact the absolute values of the weights,
 *          but not their relative weighting. The intent of the
 *          multiplier coefficient when used by itself is for weights
 *          that are decimals, especially weights that are limited
 *          to the range of 0 < x <= 1. In this case, the unmodified
 *          weights would all be transformed to integers by being
 *          truncated, and hence would all have weight zero. Applying
 *          the multiplier converts these to a larger value, e.g.
 *          multiplying by 10000, making them have relative weights
 *          that more closely approximate their weights based on
 *          their full original values.
 *     - If the upper bound is exactly one, it does not transform the
 *          incoming value. However, if the upper bound is some value
 *          greater than one, the incoming value will be re-scaled and
 *          may become a fractional value. If this is done, the incoming
 *          values may be transformed to values in the range of
 *          0 < x <=1, in which case the multCoeff _must_ be used to
 *          transform these values as described in the prevous bullet.
 *     - Use of the exp power should also bear in mind the above bullets
 *          if fractional values for either the weights or the exp are
 *          involved.
 *     - As with the parent class (SimpleWeightedSelector), a call to the
 *          getConvertedValue method will return the value that will be
 *          used as a weight when the weight is added, but the weight
 *          will not be added. This function can be used to inspect
 *          the effect of the transformation before choosing whether to
 *          add the weight.
 */
class ParameterizedWeightedSelector: public SimpleWeightedSelector {
protected:
    long multCoeff;
    long expPower;
    long expectedUpperBound;

public:
    ParameterizedWeightedSelector(int defaultValue = -1, long multCoeff = 1, long expPower = 1, long expectedUpperBound = 1);
    virtual ~ParameterizedWeightedSelector();
    
    /**
     * Function that transforms a double into a weight to be added.
     * Returns the value that represents the weight that will be assigned, given the value passed.
     */
    virtual long getConvertedValue(double value);

    long getMultCoeff(){          return multCoeff;          }
    long getExpPower(){           return expPower;           }
    long getExpectedUpperBound(){ return expectedUpperBound; }

};

struct PWSValues {
    int  defaultValue;
    long multCoeff;
    long expPower;
    long expectedUpperBound;
};

class PWSFactory {
protected:
    std::unordered_map<std::string, std::unordered_map<std::string, PWSValues> > dict;
    static PWSFactory * instance; 

public:
    PWSFactory();
    ~PWSFactory();
    static PWSFactory* getInstance();
    void initValues(std::string platform, std::string type, int defaultValue, long multCoeff, long expPower, long expectedUpperBound);
    ParameterizedWeightedSelector getParameterizedWeightedSelector(std::string platform, std::string type);

};


}  // End Namespace rhpc_smple

#endif
