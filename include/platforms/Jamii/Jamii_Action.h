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
/* Jamii_Action.h */
#ifndef INCLUDE_JAMII_ACTION_H
#define INCLUDE_JAMII_ACTION_H

#include <vector>

#include "socialnetwork_toolkit/Action.h"

#include "platforms/Jamii/Jamii_Definitions.h"
#include "platforms/Jamii/Jamii_Payload.h"
#include "platforms/Jamii/Jamii_Feed.h"

namespace jamii{

class JamiiActionExplanation: public ActionExplanation<JamiiFeedExplanation>{
protected:
	bool becauseOtherUsersLiked;

	std::vector<std::string> headings;

public:
	JamiiActionExplanation();
	JamiiActionExplanation(JamiiFeedExplanation feedExp);

	virtual ~JamiiActionExplanation();

	std::string getRow();
};


template<typename AGENTTYPE, typename SCORETYPE, typename OUTPUT_TYPE, typename PACKAGE_TYPE>
class AbstractJamiiAction:
		public SocialNetwork_Action<JamiiEventType, AGENTTYPE, SocialNetwork_AbstractElement<JamiiAgentType, SCORETYPE>, JamiiPayload, JamiiActionExplanation, OUTPUT_TYPE, PACKAGE_TYPE>{

public:

	AbstractJamiiAction(JamiiEventType event, AGENTTYPE* actingElement = NULL, AGENTTYPE* targetElement = NULL);
	virtual ~AbstractJamiiAction();

	virtual std::string getOutputRepresentation(OUTPUT_TYPE filetype, bool writeOnlyFiltered = false);

	virtual std::string getDomain();

};

template<typename AGENTTYPE, typename SCORETYPE, typename OUTPUT_TYPE, typename PACKAGE_TYPE>
AbstractJamiiAction<AGENTTYPE, SCORETYPE, OUTPUT_TYPE, PACKAGE_TYPE>::AbstractJamiiAction(JamiiEventType event, AGENTTYPE* actingElement, AGENTTYPE* target):
		SocialNetwork_Action<JamiiEventType, AGENTTYPE, SocialNetwork_AbstractElement<JamiiAgentType, SCORETYPE>, JamiiPayload, JamiiActionExplanation, OUTPUT_TYPE, PACKAGE_TYPE>(event, actingElement, target)
		{}

template<typename AGENTTYPE, typename SCORETYPE, typename OUTPUT_TYPE, typename PACKAGE_TYPE>
AbstractJamiiAction<AGENTTYPE, SCORETYPE, OUTPUT_TYPE, PACKAGE_TYPE>::~AbstractJamiiAction(){}

template<typename AGENTTYPE, typename SCORETYPE, typename OUTPUT_TYPE, typename PACKAGE_TYPE>
std::string AbstractJamiiAction<AGENTTYPE, SCORETYPE, OUTPUT_TYPE, PACKAGE_TYPE>::getOutputRepresentation(OUTPUT_TYPE filetype, bool writeOnlyFiltered){
    std::stringstream s;
    return s.str();
}

template<typename AGENTTYPE, typename SCORETYPE, typename OUTPUT_TYPE, typename PACKAGE_TYPE>
std::string AbstractJamiiAction<AGENTTYPE, SCORETYPE, OUTPUT_TYPE, PACKAGE_TYPE>::getDomain(){
	return "jamii.com";
}


} // End namespace jamii


#endif /* INCLUDE_JAMII_ACTION_H */
