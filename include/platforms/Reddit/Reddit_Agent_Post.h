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
/* Reddit_Post.h */
#ifndef INCLUDE_REDDIT_AGENT_POST_H
#define INCLUDE_REDDIT_AGENT_POST_H

#include "socialnetwork_toolkit/InfoID.h"
#include "platforms/Reddit/Reddit_Agent.h"

using namespace rhpc_smple;

namespace reddit{

template<typename SCORETYPE>
class AbstractRedditPostAgent{
	int countOfDirectComments; // Count of direct comments, not full tree (which will be size of vector)

public:
    std::vector<RedditComment<SCORETYPE>*> comments;

    std::string                 communityID;
    std::set<InfoID*>           informationIDs;

    AbstractRedditPostAgent(std::string community);

    virtual ~AbstractRedditPostAgent();

    // Add comment with post itself as parent
    RedditComment<SCORETYPE>* addComment(SocialNetwork_AbstractElement<RedditAgentType, SCORETYPE>* creatorPtr);

    // Add comment with one of the existing comments as parent
    RedditComment<SCORETYPE>* addComment(SocialNetwork_AbstractElement<RedditAgentType, SCORETYPE>* creatorPtr, RedditComment<SCORETYPE>* parentPtr);

    int getCountOfDirectComments(){
    	  return countOfDirectComments;
    }

    void incrementCountOfDirectComments(){
    	  countOfDirectComments++;
    }

    virtual std::string getResponseNameRoot();

    virtual SocialNetwork_AbstractElement<RedditAgentType, SCORETYPE>* getSelfPointer();

};



template<typename SCORETYPE>
AbstractRedditPostAgent<SCORETYPE>::AbstractRedditPostAgent(std::string community):communityID(community), countOfDirectComments(0){
//	stringstream s;
//	s << "t3_" << showName();
//	setHashTagID(s.str());
}

template<typename SCORETYPE>
AbstractRedditPostAgent<SCORETYPE>::~AbstractRedditPostAgent(){
	for(int i = 0; i < comments.size(); i++) delete comments[i];
}

//std::string AbstractRedditPostAgent::showName(){
//	std::ostringstream ss;
//	//if(_model->diagnosticMode) ss << (whichCluster + 1) << ",";
//	ss << hashTagID;
//	return ss.str();
//}
//
//std::string AbstractRedditPostAgent::writeName(){
//	std::ostringstream ss;
//	//if(_model->diagnosticMode) ss << (whichCluster + 1) << ",";
//	ss << canonicalName;
//	return ss.str();
//}


template<typename SCORETYPE>
RedditComment<SCORETYPE>* AbstractRedditPostAgent<SCORETYPE>::addComment(SocialNetwork_AbstractElement<RedditAgentType, SCORETYPE>* creatorPtr){
	stringstream s;
	s << "t1_" << this->getResponseNameRoot() << "_" << (countOfDirectComments + 1);
	RedditComment<SCORETYPE>* toAdd = new RedditComment<SCORETYPE>(s.str(), creatorPtr, getSelfPointer(), getSelfPointer(), communityID, false);
	comments.push_back(toAdd);
	toAdd->indx = comments.size(); // Note: 1- indexed, not 0-indexed
	return toAdd;
//	return 0;
}

template<typename SCORETYPE>
RedditComment<SCORETYPE>* AbstractRedditPostAgent<SCORETYPE>::addComment(SocialNetwork_AbstractElement<RedditAgentType, SCORETYPE>* creatorPtr, RedditComment<SCORETYPE>* parentPtr){
	stringstream s;
	int commentCount = parentPtr->countOfDirectComments + 1;
	s << parentPtr->nodeID << "_" << commentCount; // Note: "t1_" prefix will already be on parent...
	RedditComment<SCORETYPE>* toAdd = new RedditComment<SCORETYPE>(s.str(), creatorPtr, parentPtr, parentPtr->rootPtr, communityID, false);
	comments.push_back(toAdd);
	toAdd->indx = comments.size(); // Note: 1- indexed, not 0-indexed
	return toAdd;
//	return 0;
}

template<typename SCORETYPE>
std::string AbstractRedditPostAgent<SCORETYPE>::getResponseNameRoot(){
	stringstream s;
	s << "t1_RD" << (countOfDirectComments + 1);
	return s.str();
}

template<typename SCORETYPE>
SocialNetwork_AbstractElement<RedditAgentType, SCORETYPE>* AbstractRedditPostAgent<SCORETYPE>::getSelfPointer(){
	return 0;
}



} // End namespace reddit

#endif /* INCLUDE_REDDIT_AGENT_POST_H */
