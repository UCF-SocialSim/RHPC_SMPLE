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
/* Twitter_FeedQuery_xamples.h */
#ifndef INCLUDE_TWITTER_FEED_QUERY_XAMPLES_H
#define INCLUDE_TWITTER_FEED_QUERY_XAMPLES_H

#include "platforms/Twitter/Twitter_Feed_Query.h"

#include "xamples/platforms/Twitter/Twitter_Definitions_xamples.h"

using namespace rhpc_smple;

namespace twitter{

// Feed query types
enum TwitterFeedQueryType_xamples{
	TW_BASIC,
	TWITTERFEEDQUERYTYPE_XAMPLE_META
};


class TwitterFeedQuery_xamples: public TwitterFeedQuery{
public:
	TwitterFeedQueryType_xamples type;
	Twitter_Xamples_ScoreTypes   scoreType;

	TwitterFeedQuery_xamples(TwitterFeedQueryType_xamples t = TWITTERFEEDQUERYTYPE_XAMPLE_META, Twitter_Xamples_ScoreTypes score = TWITTER_XAMPLES_DEFAULT_SCORE);
	virtual ~TwitterFeedQuery_xamples();
};

} // End namespace twitter


#endif /* INCLUDE_TWITTER_FEED_QUERY_XAMPLES_H */
