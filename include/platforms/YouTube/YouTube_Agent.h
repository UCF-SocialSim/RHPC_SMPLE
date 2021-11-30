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
/* YouTube_Agent.h */
#ifndef INCLUDE_YOUTUBE_AGENT_H
#define INCLUDE_YOUTUBE_AGENT_H

#include "utilities/ParameterizedWeightedSelector.h"
#include "socialnetwork_toolkit/SocialNetwork_Platform.h"

#include "YouTube_Definitions.h"
#include "YouTube_Feed.h"
#include "YouTube_Action.h"

using namespace rhpc_smple;

namespace youtube{

template<typename SCORETYPE>
class YouTubeComment: public SocialNetwork_AbstractElement<YouTubeAgentType, SCORETYPE>{
public:

    std::string nodeID;
    SocialNetwork_AbstractElement<YouTubeAgentType, SCORETYPE>* parentPtr;     // Will be either a video or a comment
    SocialNetwork_AbstractElement<YouTubeAgentType, SCORETYPE>* rootPtr;       // Will be a video

    int         countOfDirectComments;
    int         countOfAllComments;
    int         indx;
    bool        partOfSeededCascade;


    YouTubeComment(std::string ID,
    		SocialNetwork_AbstractElement<YouTubeAgentType, SCORETYPE>* creatorPtr,
			SocialNetwork_AbstractElement<YouTubeAgentType, SCORETYPE>* parentPtr,
			SocialNetwork_AbstractElement<YouTubeAgentType, SCORETYPE>* rootPtr,
                    bool partOfCascade);
    virtual ~YouTubeComment();

    void incrementAllParentsCounts();

};

template<typename SCORETYPE>
YouTubeComment<SCORETYPE>::YouTubeComment(std::string ID,
		SocialNetwork_AbstractElement<YouTubeAgentType, SCORETYPE>* creatorPtr,
		SocialNetwork_AbstractElement<YouTubeAgentType, SCORETYPE>* parentPtr,
		SocialNetwork_AbstractElement<YouTubeAgentType, SCORETYPE>* rootPtr,
		  bool partOfCascade):
		SocialNetwork_AbstractElement<YouTubeAgentType, SCORETYPE>(COMMENT_ELEMENT, creatorPtr),
		nodeID(ID),
		parentPtr(parentPtr),
		rootPtr(rootPtr),
		countOfDirectComments(0),
		countOfAllComments(0),
		partOfSeededCascade(partOfCascade){

	// Increment the count of the parent's comments
//	if(parentPtr->getType() == COMMENT_ELEMENT) ((YouTubeComment*)parentPtr)->countOfDirectComments++;
//	else                                        ((YouTubeVideoAgent*)parentPtr)->incrementCountOfDirectComments();

	incrementAllParentsCounts();
}

template<typename SCORETYPE>
YouTubeComment<SCORETYPE>::~YouTubeComment(){ }

template<typename SCORETYPE>
void YouTubeComment<SCORETYPE>::incrementAllParentsCounts(){
	if(parentPtr->getType() == COMMENT_ELEMENT){
		YouTubeComment* parent = ((YouTubeComment*)parentPtr);
		parent->countOfAllComments++;
		parent->incrementAllParentsCounts(); // Will recurse back to root
	}
}


template<typename ACTIONTYPE, typename SCORETYPE>
class AbstractYouTubeAgent: public SocialNetworkNode<YouTubeAgentType, ACTIONTYPE, SCORETYPE>{
public:
	YouTube*                       youtube;
	boost::shared_ptr<YouTubeFeed<SCORETYPE>> myFeed;
	bool                           partOfSeededCascade;

	AbstractYouTubeAgent(repast::AgentId agentId, YouTubeAgentType whichType, ptime dateCreated, SNP* platform, SocialNetwork_AbstractElement<YouTubeAgentType, SCORETYPE>* creator = 0);
	virtual ~AbstractYouTubeAgent();

	virtual void setCurrentRank(int currentRank);

    virtual std::string showName();
    virtual std::string writeName();

    virtual void setPlatform(SNP* platform);

    int getCountOfFeedElements();

    virtual void selectTarget(ACTIONTYPE& action){}

};


template<typename ACTIONTYPE, typename SCORETYPE>
AbstractYouTubeAgent<ACTIONTYPE, SCORETYPE>::AbstractYouTubeAgent(repast::AgentId agentId, YouTubeAgentType whichType, ptime dateCreated, SNP* platform, SocialNetwork_AbstractElement<YouTubeAgentType, SCORETYPE>* creator):
		SocialNetworkNode<YouTubeAgentType, ACTIONTYPE, SCORETYPE>(agentId, whichType, dateCreated, platform, creator),
		myFeed(new YouTubeFeed<SCORETYPE>(1000000)), partOfSeededCascade(false){

}

template<typename ACTIONTYPE, typename SCORETYPE>
AbstractYouTubeAgent<ACTIONTYPE, SCORETYPE>::~AbstractYouTubeAgent(){}


template<typename ACTIONTYPE, typename SCORETYPE>
void AbstractYouTubeAgent<ACTIONTYPE, SCORETYPE>::setCurrentRank(int currentRank){
	this->id.currentRank(currentRank);
}

template<typename ACTIONTYPE, typename SCORETYPE>
void AbstractYouTubeAgent<ACTIONTYPE, SCORETYPE>::setPlatform(SNP* platform){
	SocialNetwork_AbstractNode<YouTubeAgentType, SCORETYPE>::setPlatform(platform);
		youtube = (YouTube*)(this->containingPlatform);
}

template<typename ACTIONTYPE, typename SCORETYPE>
std::string AbstractYouTubeAgent<ACTIONTYPE, SCORETYPE>::showName(){
	std::ostringstream ss;
	ss << this->getId().hashcode();
	return ss.str();
}

template<typename ACTIONTYPE, typename SCORETYPE>
std::string AbstractYouTubeAgent<ACTIONTYPE, SCORETYPE>::writeName(){
	std::ostringstream ss;
	ss << this->getId().hashcode();
	return ss.str();
}

template<typename ACTIONTYPE, typename SCORETYPE>
int AbstractYouTubeAgent<ACTIONTYPE, SCORETYPE>::getCountOfFeedElements(){
	return myFeed->size();
}


} // End namespace youtube

#endif /* INCLUDE_YOUTUBE_AGENT_H */
