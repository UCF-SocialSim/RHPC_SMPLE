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
/* Jamii_Feed.h */
#ifndef INCLUDE_JAMII_FEED_H
#define INCLUDE_JAMII_FEED_H

#include <sstream>

#include "utilities/SimpleWeightedSelector.h"

#include "socialnetwork_toolkit/Explanation.h"
#include "socialnetwork_toolkit/Feed.h"

#include "Jamii_Definitions.h"

using namespace rhpc_smple;

namespace jamii{


class JamiiFeedExplanation: public Explanation{
public:
	bool randomlySelected;
	double fitnessValue;

	JamiiFeedExplanation();
	JamiiFeedExplanation(const JamiiFeedExplanation& orig);

	virtual ~JamiiFeedExplanation();

	virtual std::string getRow();
};

template<typename SCORETYPE>
class JamiiFeed: public Feed<JamiiAgentType, JamiiEventType, SCORETYPE, JamiiFeedExplanation>{

public:

	JamiiFeed();
	JamiiFeed(int maxSize);
	virtual ~JamiiFeed();

};


template<typename SCORETYPE>
JamiiFeed<SCORETYPE>::JamiiFeed():
		Feed<JamiiAgentType, JamiiEventType, SCORETYPE, JamiiFeedExplanation>(30){ }

template<typename SCORETYPE>
JamiiFeed<SCORETYPE>::JamiiFeed(int maxSize):
		Feed<JamiiAgentType, JamiiEventType, SCORETYPE, JamiiFeedExplanation>(maxSize){ }

template<typename SCORETYPE>
JamiiFeed<SCORETYPE>::~JamiiFeed(){ }


} // End namespace jamii


#endif /* INCLUDE_JAMII_FEED_H */
