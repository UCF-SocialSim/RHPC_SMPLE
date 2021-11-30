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
/* Explanation.h */
#ifndef INCLUDE_EXPLANATION_H
#define INCLUDE_EXPLANATION_H

namespace rhpc_smple {

class Explanation{
public:
	Explanation(){}
	virtual ~Explanation(){}

	// When two explanations are added together,
	// there is an assumption that the flags are 'or-ed'
	// together. That is, if Explanation 1 includes a
	// flag that indicates "Chosen by random selection",
	// and Explanation 2 has a flag that indicates,
	// "Occurred due to platform strategy change",
	// when these explanations are added/merged,
	// the result should be an Explanation in which
	// both of these flags are true.
	void add(bool* mine, bool other){
		*mine = other || *mine;
	}

	virtual std::string getRow(){ return "INVALID"; }

};

// An ActionExplanation includes a FeedExplanation as one
// component; the end result is an object that contains
// an explanation about why the object was available to the agent
// as a target (the Feed Explanation) and why it was selected
// by the agent for the action.
template<typename FE>
class ActionExplanation: public Explanation{

public:
	FE feedExplanation;

	ActionExplanation(): Explanation(), feedExplanation(){}
	ActionExplanation(FE feedExp): Explanation(), feedExplanation(feedExp){}
	virtual ~ActionExplanation(){}

	virtual std::string getRow(){ return "INVALID"; }

};

}  // End Namespace rhpc_smple

#endif /* INCLUDE_EXPLANATION_H */
