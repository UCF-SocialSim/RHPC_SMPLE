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
/* utilities/SimpleWeightedSelector.h */
#ifndef SIMPLE_WEIGHTED_SELECTOR_H
#define SIMPLE_WEIGHTED_SELECTOR_H

#include <vector>

namespace rhpc_smple {

/**
 * This class provides simple weighted selection.
 * Given a collection of numbers, will
 * make a random selection from them based
 * on their values as weights. The selector
 * returns the index of the chosen value.
 *
 * Example:
 *   addValue(1)
 *   addValue(2)
 *   addValue(3)
 *
 *   select() - returns 0, 1, or 2, with
 *   probabilities of 1/6, 2/6, or 3/6 respectively.
 */
class SimpleWeightedSelector{
protected:
	long              sum;
	int               defaultVal;
	std::vector<long> components;
	
public:
	SimpleWeightedSelector(int defaultValue);
	virtual ~SimpleWeightedSelector();
	
	/**
	 * Adds a value to the list of values
	 */
	virtual void addValue(double val);
	
    /**
     * Function that transforms a double into a weight to be added.
     * Returns the value truncated to a long
     */
    virtual long getConvertedValue(double value);

	/**
	 * Returns an index to one of the values
	 * using the values as weights.
	 * May return the default value if sum is zero NOTE: default value is generally -1
	 * Otherwise will return 0 to size()
	 */
	virtual int select();

	/**
	 * Gets the value at the specified index position.
	 * May return the default value if the index is invalid 
	 * NOTE: default value is generally -1
	 */
	virtual long at(int indx);

	/**
	 * Gets the default value
	 */
	virtual int getDefaultValue();

	/**
	 * Gets the count of elements in this selector
	 */
	virtual int size();

	/**
	 * Adjust the element at the specified index;
	 * adds the specified value, with a floor of 0.
	 * Returns the new sum of the selector, which may
	 * be 0. If the index is out of bounds, returns
	 * the original sum.
	 */
	long adjust(int indx, long addend);

	/**
	 * Performs an adjust with an addend of -1.
	 */
	virtual long decrement(int indx);

	/**
	 * Performs an adjust with an addend of 1.
	 */
	virtual long increment(int indx);

	/**
	 * Returns the current sum. Adjusting totals
	 * may cause the sum to change, and possible
	 * become zero. A zero sum will return the
	 * default value.
	 */
	virtual long getSum(){ return sum; }

	/**
	 * Clears the entire selector; the list of components
	 * is cleared to length zero and the sum is reset to zero
	 */
	virtual void clear();

	/**
	 * Performs an adjust at the specified position,
	 * such that the weight at that position is zero.
	 */
	virtual long clearAt(int indx);

	/**
	 * Returns the count of elements that currently have a value
	 * of zero.
	 */
	virtual long getCountOfElementsWithZeroValues();

};

}  // End Namespace rhpc_smple

#endif
