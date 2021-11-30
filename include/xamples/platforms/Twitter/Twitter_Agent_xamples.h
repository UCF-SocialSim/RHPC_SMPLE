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
/* Twitter_Agent_xamples.h */
#ifndef INCLUDE_TWITTER_AGENT_XAMPLES_H
#define INCLUDE_TWITTER_AGENT_XAMPLES_H

#include "platforms/Twitter/Twitter_Agent.h"

#include "xamples/platforms/Twitter/Twitter_Definitions_xamples.h"
#include "xamples/platforms/Twitter/Twitter_Agent_Package_xamples.h"
#include "xamples/platforms/Twitter/Twitter_Action_xamples.h"

namespace twitter{

/**
 * An Agent base class that allows multiple classes in the context
 **/
class TwitterAgent: public AbstractTwitterAgent<TwitterAction, Twitter_Xamples_ScoreTypes>{

private:
	double currentAgeScore;

public:

    TwitterAgent(repast::AgentId id, TwitterAgentType whichType,
    		ptime dateCreated, TwitterAgentPackage& package, SNP* platform,
			SocialNetwork_AbstractElement<TwitterAgentType, Twitter_Xamples_ScoreTypes>* creator = 0);
    virtual ~TwitterAgent();

    virtual long  getTotalActionCount();
    virtual ptime getDateCreated();

    virtual void updateScore(Twitter_Xamples_ScoreTypes type);

    virtual double getScore(Twitter_Xamples_ScoreTypes type);

};

} // End namespace twitter

#endif /* INCLUDE_TWITTER_AGENT_XAMPLES_H */
