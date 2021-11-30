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
/* GitHub_Feed.h */
#ifndef INCLUDE_GITHUB_FEED_H
#define INCLUDE_GITHUB_FEED_H

#include <sstream>

#include "utilities/SimpleWeightedSelector.h"

#include "socialnetwork_toolkit/InfoID.h"
#include "socialnetwork_toolkit/Explanation.h"
#include "socialnetwork_toolkit/Feed.h"
#include "socialnetwork_toolkit/SocialNetwork_Platform.h"
#include "socialnetwork_toolkit/Action.h"

#include "GitHub_Definitions.h"

using namespace rhpc_smple;

namespace github{


class GitHubFeedExplanation: public Explanation{
public:
	bool   randomlySelected;
	bool   userIsFollowing;
	bool   userIsContributing;
	bool   proposedBecauseUserLikesSimilar;
	bool   userSearch;
    double fitnessValue;
    double distance;

	GitHubFeedExplanation();
	GitHubFeedExplanation(const GitHubFeedExplanation& orig);

	virtual ~GitHubFeedExplanation();

//	virtual void add(GitHubFeedExplanation& other);

	virtual std::string getRow();

};

template<typename SCORETYPE>
class GitHubFeed: public Feed<GitHubAgentType, GitHubEventType, SCORETYPE, GitHubFeedExplanation>{

protected:
	SimpleWeightedSelector* sws;

public:

	GitHubFeed();
	GitHubFeed(int maxSize);
	virtual ~GitHubFeed();

	void getReposNotContributing(std::set<SocialNetwork_AbstractElement<GitHubAgentType, SCORETYPE>*>& repos);

	void getReposContributing(std::set<SocialNetwork_AbstractElement<GitHubAgentType, SCORETYPE>*>& repos);

};

//typedef std::deque<FeedElement<GitHubAgentType, GitHubEventType, GitHubFeedExplanation>>::iterator FeedIterator;

template<typename SCORETYPE>
GitHubFeed<SCORETYPE>::GitHubFeed():
		Feed<GitHubAgentType, GitHubEventType, SCORETYPE, GitHubFeedExplanation>(30),
		sws(0){ }

template<typename SCORETYPE>
GitHubFeed<SCORETYPE>::GitHubFeed(int maxSize):
		Feed<GitHubAgentType, GitHubEventType, SCORETYPE, GitHubFeedExplanation>(maxSize),
		sws(0){ }

template<typename SCORETYPE>
GitHubFeed<SCORETYPE>::~GitHubFeed(){
  if(sws != 0) delete sws;
}

template<typename SCORETYPE>
void GitHubFeed<SCORETYPE>::getReposNotContributing(std::set<SocialNetwork_AbstractElement<GitHubAgentType, SCORETYPE>*>& repos){}

template<typename SCORETYPE>
void GitHubFeed<SCORETYPE>::getReposContributing(std::set<SocialNetwork_AbstractElement<GitHubAgentType, SCORETYPE>*>& repos){}

} // End namespace github

#endif /* INCLUDE_GITHUB_FEED_H */
