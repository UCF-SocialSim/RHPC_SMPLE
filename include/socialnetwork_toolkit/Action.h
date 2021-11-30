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
/* Action.h */
#ifndef INCLUDE_SOCIALNETWORK_TOOLKIT_ACTION_H
#define INCLUDE_SOCIALNETWORK_TOOLKIT_ACTION_H

#include <string>

namespace rhpc_smple {

/**
 * This is a class that represents a generic action. All actions
 * have:
 *
 *    a kind of action
 *    an actor performing the action
 *    an agent of some kind that is the target
 *    an action explanation
 *    a payload of data that gets carried along
 *
 * The templates to this argument specify:
 *
 *    T            The type of action
 *    A            The type of the actor and target (e.g. 'Reddit_Agent', not user or post)
 *    S            Subtarget, e.g. a 'Comment' on a Reddit 'Post'
 *    P            The type of payload
 *    AE           The type of action explanation
 *    OUTPUT_TYPE  The kind of element that will be written to the output
 *    PACKAGE_TYPE The kind of package that this action can transport
 */
template< typename T, typename A, typename S, typename P, typename AE, typename OUTPUT_TYPE, typename PACKAGE_TYPE>
class SocialNetwork_Action{

public:
	T   type;
	A*  actor;
	A*  target;
	S*  subtarget;
	AE 	ae;
	P   payload;
	S*  newlyCreatedObject;

	SocialNetwork_Action(T eventType, A* actingElement = 0, A* targetElement = 0);
	virtual ~SocialNetwork_Action();

	// Get the JSON representation of this action
	virtual std::string getOutputRepresentation(OUTPUT_TYPE filetype, bool writeOnlyFiltered = false) = 0;

	// Get the domain in which this action applies
	virtual std::string getDomain();

 };


template< typename T, typename A, typename S, typename P, typename AE, typename OUTPUT_TYPE, typename PACKAGE_TYPE>
SocialNetwork_Action<T, A, S, P,AE, OUTPUT_TYPE, PACKAGE_TYPE>::SocialNetwork_Action(T eventType, A* actingElement, A* targetElement):
  type(eventType), actor(actingElement), target(targetElement), subtarget(0), ae(), newlyCreatedObject(0){ }

template< typename T, typename A, typename S, typename P, typename AE, typename OUTPUT_TYPE, typename PACKAGE_TYPE>
SocialNetwork_Action<T, A, S, P,AE, OUTPUT_TYPE, PACKAGE_TYPE>::~SocialNetwork_Action(){}


template< typename T, typename A, typename S, typename P, typename AE, typename OUTPUT_TYPE, typename PACKAGE_TYPE>
std::string SocialNetwork_Action<T, A, S, P,AE, OUTPUT_TYPE, PACKAGE_TYPE>::getDomain(){
	return "";
}

}  // End Namespace rhpc_smple

#endif /* INCLUDE_SOCIALNETWORK_TOOLKIT_ACTION_H */
