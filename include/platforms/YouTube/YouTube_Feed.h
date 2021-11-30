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
/* YouTube_Feed.h */
#ifndef INCLUDE_YOUTUBE_FEED_H
#define INCLUDE_YOUTUBE_FEED_H

#include "utilities/ParameterizedWeightedSelector.h"

#include "socialnetwork_toolkit/Feed.h"
#include "socialnetwork_toolkit/Explanation.h"

#include "platforms/YouTube/YouTube_Definitions.h"

using namespace rhpc_smple;

namespace youtube{


class YouTubeFeedExplanation: public Explanation{
public:
    bool   randomlySelected;
    bool   userIsFollowing;
    bool   userIsContributing;
    bool   proposedBecauseUserLikesSimilar;
    bool   userSearch;
    double fitnessValue;
    double distance;

    //vector<std::string> headings;

    YouTubeFeedExplanation();

    YouTubeFeedExplanation(const YouTubeFeedExplanation& orig);

    virtual ~YouTubeFeedExplanation();

//    virtual void add(RedditFeedExplanation& other){
//        Explanation::add(&randomlySelected, other.randomlySelected);
//        Explanation::add(&userIsFollowing, other.userIsFollowing);
//        Explanation::add(&userIsContributing, other.userIsContributing);
//        Explanation::add(&proposedBecauseUserLikesSimilar, other.proposedBecauseUserLikesSimilar);
//        Explanation::add(&userSearch, other.userSearch);
//    }

    virtual std::string getRow();
};

template<typename SCORETYPE>
class YouTubeFeed: public Feed<YouTubeAgentType, YouTubeEventType, SCORETYPE, YouTubeFeedExplanation>{

protected:

public:
	YouTubeFeed();
    YouTubeFeed(int maxSize);
	virtual ~YouTubeFeed();
};



template<typename SCORETYPE>
YouTubeFeed<SCORETYPE>::YouTubeFeed():
		Feed<YouTubeAgentType, YouTubeEventType, SCORETYPE, YouTubeFeedExplanation>(30){
}

template<typename SCORETYPE>
YouTubeFeed<SCORETYPE>::YouTubeFeed(int maxSize):
		Feed<YouTubeAgentType, YouTubeEventType, SCORETYPE, YouTubeFeedExplanation>(maxSize){
}

template<typename SCORETYPE>
YouTubeFeed<SCORETYPE>::~YouTubeFeed(){ }




} // End namespace youtube

#endif /* INCLUDE_YOUTUBE_FEED_H */
