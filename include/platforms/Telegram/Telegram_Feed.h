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
/* Telegram_Feed.h */
#ifndef INCLUDE_TELEGRAM_FEED_H
#define INCLUDE_TELEGRAM_FEED_H

#include <sstream>
#include <vector>

#include "utilities/SimpleWeightedSelector.h"

#include "socialnetwork_toolkit/Explanation.h"
#include "socialnetwork_toolkit/Feed.h"

#include "Telegram_Definitions.h"

using namespace rhpc_smple;

namespace telegram{

class TelegramFeedExplanation: public Explanation{
public:
	bool randomlySelected;
	bool userIsFollowing;
	bool userIsContributing;
	bool proposedBecauseUserLikesSimilar;
	bool userSearch;


	TelegramFeedExplanation();
	virtual ~TelegramFeedExplanation();

//	virtual void add(TelegramFeedExplanation& other){
//		Explanation::add(&randomlySelected, other.randomlySelected);
//		Explanation::add(&userIsFollowing, other.userIsFollowing);
//		Explanation::add(&userIsContributing, other.userIsContributing);
//		Explanation::add(&proposedBecauseUserLikesSimilar, other.proposedBecauseUserLikesSimilar);
//		Explanation::add(&userSearch, other.userSearch);
//	}

	virtual std::string getRow();
};


template<typename SCORETYPE>
class TelegramFeed: public Feed<TelegramAgentType, TelegramEventType, SCORETYPE, TelegramFeedExplanation>{

public:
	TelegramFeed(int maxSize = 30);
	virtual ~TelegramFeed();

};

//typedef std::deque<FeedElement<TelegramAgentType, TelegramEventType, TelegramFeedExplanation>>::iterator FeedIterator;

template<typename SCORETYPE>
TelegramFeed<SCORETYPE>::TelegramFeed(int maxSize):
						Feed<TelegramAgentType, TelegramEventType, SCORETYPE, TelegramFeedExplanation>(maxSize){}

template<typename SCORETYPE>
TelegramFeed<SCORETYPE>::~TelegramFeed(){ }

} // End namespace telegram

#endif /* INCLUDE_TELEGRAM_FEED_H */
