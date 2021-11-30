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
/* Reddit_Feed.h */
#ifndef INCLUDE_REDDIT_FEED_H
#define INCLUDE_REDDIT_FEED_H

#include <sstream>

#include "utilities/SimpleWeightedSelector.h"

#include "socialnetwork_toolkit/InfoID.h"
#include "socialnetwork_toolkit/Explanation.h"
#include "socialnetwork_toolkit/Feed.h"
#include "socialnetwork_toolkit/Action.h"

#include "Reddit_Definitions.h"

using namespace rhpc_smple;

namespace reddit{


class RedditFeedExplanation: public Explanation{
public:
    bool   randomlySelected;
    bool   userIsFollowing;
    bool   userIsContributing;
    bool   proposedBecauseUserLikesSimilar;
    bool   userSearch;
    double fitnessValue;
    double distance;

    // Constructors
    RedditFeedExplanation();
    RedditFeedExplanation(const RedditFeedExplanation& orig);

    // Destructor
    virtual ~RedditFeedExplanation();

    virtual std::string getRow(){
        std::stringstream s;
        s << (randomlySelected ? "1" : "0") << "," <<
                (userIsFollowing ? "1" : "0") << "," <<
                (userIsContributing ? "1" : "0") << "," <<
                (proposedBecauseUserLikesSimilar ? "1" : "0") << "," <<
                (userSearch ? "1" : "0")  << "," <<
				fitnessValue << "," <<
				distance;
        return s.str();
    }
};

template<typename SCORETYPE>
class RedditFeed: public Feed<RedditAgentType, RedditEventType, SCORETYPE, RedditFeedExplanation>{

public:
	RedditFeed();
    RedditFeed(int maxSize);
	virtual ~RedditFeed();

};


template<typename SCORETYPE>
RedditFeed<SCORETYPE>::RedditFeed():
		Feed<RedditAgentType, RedditEventType, SCORETYPE, RedditFeedExplanation>(30){ }

template<typename SCORETYPE>
RedditFeed<SCORETYPE>::RedditFeed(int maxSize):
		Feed<RedditAgentType, RedditEventType, SCORETYPE, RedditFeedExplanation>(maxSize){ }

template<typename SCORETYPE>
RedditFeed<SCORETYPE>::~RedditFeed(){ }


} // End namespace reddit

#endif /* INCLUDE_REDDIT_FEED_H */
