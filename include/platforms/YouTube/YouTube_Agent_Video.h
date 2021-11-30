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
/* YouTube_Agent_Video.h */
#ifndef YOUTUBE_VIDEO_AGENT_H
#define YOUTUBE_VIDEO_AGENT_H

#include "socialnetwork_toolkit/InfoID.h"
#include "platforms/YouTube/YouTube_Agent.h"

using namespace rhpc_smple;

namespace youtube{

template<typename SCORETYPE>
class AbstractYouTubeVideoAgent{

public:
	int                          countOfDirectComments; // Count of direct comments, not full tree (which will be size of vector)
    std::vector<YouTubeComment<SCORETYPE>*> comments;
    std::set<InfoID*>            informationIDs;

    AbstractYouTubeVideoAgent();

    virtual ~AbstractYouTubeVideoAgent();

    // Add comment with the video itself as parent
    YouTubeComment<SCORETYPE>* addComment(SocialNetwork_AbstractElement<YouTubeAgentType, SCORETYPE>* creatorPtr);

    // Add comment with one of the existing comments as parent
    YouTubeComment<SCORETYPE>* addComment(SocialNetwork_AbstractElement<YouTubeAgentType, SCORETYPE>* creatorPtr, YouTubeComment<SCORETYPE>* parentPtr);

    virtual std::string getResponseNameRoot();

    virtual SocialNetwork_AbstractElement<YouTubeAgentType, SCORETYPE>* getSelfPointer();

    int getCountOfDirectComments();

    void incrementCountOfDirectComments();

};

template<typename SCORETYPE>
AbstractYouTubeVideoAgent<SCORETYPE>::AbstractYouTubeVideoAgent():
	countOfDirectComments(0){
//	stringstream s;
//	s << showName();
//	setHashTagID(s.str());
}

template<typename SCORETYPE>
AbstractYouTubeVideoAgent<SCORETYPE>::~AbstractYouTubeVideoAgent(){
	for(int i = 0; i < comments.size(); i++) delete comments[i];
}

template<typename SCORETYPE>
YouTubeComment<SCORETYPE>* AbstractYouTubeVideoAgent<SCORETYPE>::addComment(SocialNetwork_AbstractElement<YouTubeAgentType, SCORETYPE>* creatorPtr){
	stringstream s;
	s << getResponseNameRoot() << "_" << (countOfDirectComments + 1);
	YouTubeComment<SCORETYPE>* toAdd = new YouTubeComment<SCORETYPE>(s.str(), creatorPtr, getSelfPointer(), getSelfPointer(), false);
	comments.push_back(toAdd);
	toAdd->indx = comments.size(); // Note: 1- indexed, not 0-indexed
	return toAdd;
}

template<typename SCORETYPE>
YouTubeComment<SCORETYPE>* AbstractYouTubeVideoAgent<SCORETYPE>::addComment(SocialNetwork_AbstractElement<YouTubeAgentType, SCORETYPE>* creatorPtr, YouTubeComment<SCORETYPE>* parentPtr){
	stringstream s;
	int commentCount = parentPtr->countOfDirectComments + 1;
	s << parentPtr->nodeID << "_" << commentCount; // Note: "t1_" prefix will already be on parent...
	YouTubeComment<SCORETYPE>* toAdd = new YouTubeComment<SCORETYPE>(s.str(), creatorPtr, parentPtr, parentPtr->rootPtr, false);
	comments.push_back(toAdd);
	toAdd->indx = comments.size(); // Note: 1- indexed, not 0-indexed
	return toAdd;
}

template<typename SCORETYPE>
std::string AbstractYouTubeVideoAgent<SCORETYPE>::getResponseNameRoot(){
	stringstream s;
	s << "VIDEO" << (countOfDirectComments + 1);
	return s.str();
}

template<typename SCORETYPE>
SocialNetwork_AbstractElement<YouTubeAgentType, SCORETYPE>* AbstractYouTubeVideoAgent<SCORETYPE>::getSelfPointer(){
	return 0;
}

template<typename SCORETYPE>
int AbstractYouTubeVideoAgent<SCORETYPE>::getCountOfDirectComments(){
	  return countOfDirectComments;
}

template<typename SCORETYPE>
void AbstractYouTubeVideoAgent<SCORETYPE>::incrementCountOfDirectComments(){
	  countOfDirectComments++;
}

} // End namespace youtube

#endif
