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
/* Twitter_Action_xamples.h */
#ifndef INCLUDE_TWITTER_ACTION_XAMPLES_H
#define INCLUDE_TWITTER_ACTION_XAMPLES_H

#include "platforms/Twitter/Twitter_Action.h"

#include "xamples/platforms/Twitter/Twitter_Definitions_xamples.h"
#include "xamples/platforms/Twitter/Twitter_Agent_Package_xamples.h"

namespace twitter{

class TwitterAgent; // Forward declaration

class TwitterAction: public AbstractTwitterAction<TwitterAgent, Twitter_Xamples_ScoreTypes, Twitter_Xamples_FileOutputTypes, TwitterAgentPackage>{

public:
	TwitterAction(TwitterEventType event);
	TwitterAction(TwitterEventType event, TwitterAgent* actingElement);
	TwitterAction(TwitterEventType event, TwitterAgent* actingElement, TwitterAgent* targetElement);
	virtual ~TwitterAction();


	virtual std::string getOutputRepresentation(Twitter_Xamples_FileOutputTypes type, bool writeOnlyFiltered = false);

	virtual TwitterAgentPackage getNewAgentPackage(int agentOrder = 0);
};

} // End namespace twitter

#endif /* INCLUDE_TWITTER_ACTION_XAMPLES_H */
