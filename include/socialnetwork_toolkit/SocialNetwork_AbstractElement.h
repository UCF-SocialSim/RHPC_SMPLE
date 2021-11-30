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
/* SocialNetwork_AbstractElement.h */
#ifndef INCLUDE_SOCIALNETWORK_ABSTRACTELEMENT_H
#define INCLUDE_SOCIALNETWORK_ABSTRACTELEMENT_H

namespace rhpc_smple {

// Base class for all elements within social network platforms
// This includes actual nodes (users, content nodes) but also
// other elements like 'replies' that may not have full node status
template<typename ATL, typename SCORETYPE>
class SocialNetwork_AbstractElement{
protected:

	ATL 		          						   type;
	SocialNetwork_AbstractElement<ATL, SCORETYPE>* creator;

public:
	SocialNetwork_AbstractElement(ATL whichType, SocialNetwork_AbstractElement<ATL, SCORETYPE>* contentCreator = 0): type(whichType), creator(contentCreator){}
	virtual ~SocialNetwork_AbstractElement(){}

	SocialNetwork_AbstractElement<ATL, SCORETYPE>* getCreator(){      return creator; }

	virtual ATL getType(){                                            return type; }
	virtual double getScore(SCORETYPE type){                          return 1.0; }

	virtual void updateScore(SCORETYPE type){ }
};

}  // End Namespace rhpc_smple

#endif /* INCLUDE_SOCIALNETWORK_ABSTRACTELEMENT_H */
