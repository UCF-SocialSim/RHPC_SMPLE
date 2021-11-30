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
/* Twitter_Agent_xamples.cpp */
#include "xamples/platforms/Twitter/Twitter_Agent_xamples.h"

#include <sstream>
#include <iomanip>
#include <algorithm>

#include "boost/date_time/posix_time/posix_time.hpp"

#include "xamples/platforms/Twitter/Twitter_Platform_xamples.h"

extern ptime currentEventTime;

using namespace twitter;

TwitterAgent::TwitterAgent(repast::AgentId agentId, TwitterAgentType whichType, ptime dateCreated, TwitterAgentPackage& package, SNP* platform, SocialNetwork_AbstractElement<TwitterAgentType, Twitter_Xamples_ScoreTypes>* creator):
		AbstractTwitterAgent<TwitterAction, Twitter_Xamples_ScoreTypes>(agentId, whichType, dateCreated, platform, creator),
		currentAgeScore(1){
	std::stringstream s;
	s << "TW" <<  agentId.hashcode();
	setHashTagID(s.str());

	currentAgeScore = 3; // New things are exciting
}

TwitterAgent::~TwitterAgent(){
}


long TwitterAgent::getTotalActionCount(){
	return twitter->getTotalActionCount(this);
}

ptime TwitterAgent::getDateCreated(){
	return dateCreated;
}

void TwitterAgent::updateScore(Twitter_Xamples_ScoreTypes type){
	switch(type){
	  case TWITTER_XAMPLES_DEFAULT_SCORE:{
		  if((currentAgeScore > 1) && ((currentEventTime - dateCreated) > boost::posix_time::hours(10)) ) currentAgeScore = 1;
		  break;
	  }
	  case TWITTER_XAMPLES_SCORETYPES_META: break;
	}
}

double TwitterAgent::getScore(Twitter_Xamples_ScoreTypes type){
	switch(type){
	  case TWITTER_XAMPLES_DEFAULT_SCORE:{
		  return currentAgeScore;
	  }
	  case TWITTER_XAMPLES_SCORETYPES_META:{
		  break;
	  }
	}
	return AbstractTwitterAgent<TwitterAction, Twitter_Xamples_ScoreTypes>::getScore(type);
}


