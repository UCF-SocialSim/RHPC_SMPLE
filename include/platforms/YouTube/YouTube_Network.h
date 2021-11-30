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
/* YouTube_Network.h */
#ifndef INCLUDE_YOUTUBE_NETWORK_H
#define INCLUDE_YOUTUBE_NETWORK_H

#include "repast_hpc/SharedContext.h"
#include "repast_hpc/SharedNetwork.h"

#include "socialnetwork_toolkit/SocialNetwork_Platform.h"
#include "socialnetwork_toolkit/EventCounter.h"
#include "platforms/YouTube/YouTube_Definitions.h"

using namespace rhpc_smple;

namespace youtube{

class YouTubeFlagCounter: public SocialRelationshipIndicator<YouTubeRelationshipType>{
public:
	YouTubeFlagCounter();
	virtual ~YouTubeFlagCounter();
};

class YouTubeEventCounter: public SocialEventCounter<YouTubeEventType>{
public:
	YouTubeEventCounter();
	virtual ~YouTubeEventCounter();
};

class YouTubeEdgeInfo:
    public SocialNetwork_FlagEdgeInformation<YouTubeRelationshipType, YouTubeFlagCounter>,
    public SocialNetwork_CountEdgeInformation<YouTubeEventType, YouTubeEventCounter>{
	friend class boost::serialization::access;

public:
	YouTubeEdgeInfo();
	virtual ~YouTubeEdgeInfo();

	template<class Archive>
	void serialize(Archive& ar, const unsigned int version){
		SocialNetwork_FlagEdgeInformation<YouTubeRelationshipType, YouTubeFlagCounter>::serialize(ar, version);
		SocialNetwork_CountEdgeInformation<YouTubeEventType, YouTubeEventCounter>::serialize(ar, version);
	}
};

/* Custom Network Components */
template<typename AGENTTYPE, typename EI> // EdgeInf
class YouTubeEdge : public repast::RepastEdge<AGENTTYPE>{
private:
    EI _edgeInfo;
    
 public:
    YouTubeEdge();
    YouTubeEdge(AGENTTYPE* source, AGENTTYPE* target);
    YouTubeEdge(AGENTTYPE* source, AGENTTYPE* target, double weight);
    YouTubeEdge(AGENTTYPE* source, AGENTTYPE* target, double weight, EI edgeInfo);
    
    YouTubeEdge(boost::shared_ptr<AGENTTYPE> source, boost::shared_ptr<AGENTTYPE> target);
    YouTubeEdge(boost::shared_ptr<AGENTTYPE> source, boost::shared_ptr<AGENTTYPE> target, double weight);
    YouTubeEdge(boost::shared_ptr<AGENTTYPE> source, boost::shared_ptr<AGENTTYPE> target, double weight, EI edgeInfo);
    
    
    EI& getEdgeInfo();
    void setEdgeInfo(EI edgeInfo);
    
    // Querying
    // Note that this defines an edge in YouTube's vocabulary,
    // translating the flags and counters to meaningful statements for YouTube repos and users
    bool  follows(){        return _edgeInfo.getFlag(SUBSCRIBED)  != 0; }

    int   numIdles(){       return _edgeInfo.getCount(IDLE); }
    int   numPosts(){       return _edgeInfo.getCount(POST); }
    int   numComments(){    return _edgeInfo.getCount(COMMENT); }

    bool  any(){            return !(_edgeInfo.isEmpty()); }

    // Updating
    void  logEvent(YouTubeEventType event){ _edgeInfo.recordEvent(event); }

    int   getTotalEvents();

};


template<typename AGENTTYPE, typename EI>
YouTubeEdge<AGENTTYPE, EI>::YouTubeEdge():_edgeInfo(){}
template<typename AGENTTYPE, typename EI>
YouTubeEdge<AGENTTYPE, EI>::YouTubeEdge(AGENTTYPE* source, AGENTTYPE* target) : repast::RepastEdge<AGENTTYPE>(source, target), _edgeInfo() {}
template<typename AGENTTYPE, typename EI>
YouTubeEdge<AGENTTYPE, EI>::YouTubeEdge(AGENTTYPE* source, AGENTTYPE* target, double weight) : repast::RepastEdge<AGENTTYPE>(source, target, weight), _edgeInfo() {}
template<typename AGENTTYPE, typename EI>
YouTubeEdge<AGENTTYPE, EI>::YouTubeEdge(AGENTTYPE* source, AGENTTYPE* target, double weight, EI edgeInfo) : repast::RepastEdge<AGENTTYPE>(source, target, weight), _edgeInfo(edgeInfo) {}
template<typename AGENTTYPE, typename EI>
YouTubeEdge<AGENTTYPE, EI>::YouTubeEdge(boost::shared_ptr<AGENTTYPE> source, boost::shared_ptr<AGENTTYPE> target) : repast::RepastEdge<AGENTTYPE>(source, target), _edgeInfo() {}
template<typename AGENTTYPE, typename EI>
YouTubeEdge<AGENTTYPE, EI>::YouTubeEdge(boost::shared_ptr<AGENTTYPE> source, boost::shared_ptr<AGENTTYPE> target, double weight) : repast::RepastEdge<AGENTTYPE>(source, target, weight), _edgeInfo() {}
template<typename AGENTTYPE, typename EI>
YouTubeEdge<AGENTTYPE, EI>::YouTubeEdge(boost::shared_ptr<AGENTTYPE> source, boost::shared_ptr<AGENTTYPE> target, double weight, EI edgeInfo) : repast::RepastEdge<AGENTTYPE>(source, target, weight), _edgeInfo(edgeInfo) {}

template<typename AGENTTYPE, typename EI>
EI& YouTubeEdge<AGENTTYPE, EI>::getEdgeInfo(){
	return _edgeInfo;
}

template<typename AGENTTYPE, typename EI>
void YouTubeEdge<AGENTTYPE, EI>::setEdgeInfo(EI edgeInfo){
	_edgeInfo = edgeInfo;
}

template<typename AGENTTYPE, typename EI>
int YouTubeEdge<AGENTTYPE, EI>::getTotalEvents(){
	return numPosts() + numComments();
}


/* Custom Edge Content */
template<typename AGENTTYPE, typename EI>
struct YouTubeEdgeContent : public repast::RepastEdgeContent<AGENTTYPE>{
    
    friend class boost::serialization::access;
    
public:
    EI edgeInfo;

    YouTubeEdgeContent();
    YouTubeEdgeContent(YouTubeEdge<AGENTTYPE, EI>* edge);
    
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) {
        repast::RepastEdgeContent<AGENTTYPE>::serialize(ar, version);
        ar & edgeInfo;
    }
    
};

template<typename AGENTTYPE, typename EI>
YouTubeEdgeContent<AGENTTYPE, EI>::YouTubeEdgeContent(){}

template<typename AGENTTYPE, typename EI>
YouTubeEdgeContent<AGENTTYPE, EI>::YouTubeEdgeContent(YouTubeEdge<AGENTTYPE, EI>* edge):
                repast::RepastEdgeContent<AGENTTYPE>(edge),
                edgeInfo(edge->getEdgeInfo()) {  }

/* Custome Edge Content Manager */
template<typename AGENTTYPE, typename EI>
class YouTubeEdgeContentManager
{
public:
    YouTubeEdgeContentManager();
    virtual ~YouTubeEdgeContentManager();
    YouTubeEdge<AGENTTYPE, EI>* createEdge(YouTubeEdgeContent<AGENTTYPE, EI>& content, repast::Context<AGENTTYPE>* context);
    YouTubeEdgeContent<AGENTTYPE, EI>* provideEdgeContent(YouTubeEdge<AGENTTYPE, EI>* edge);
};

template<typename AGENTTYPE, typename EI>
YouTubeEdgeContentManager<AGENTTYPE, EI>::YouTubeEdgeContentManager(){}

template<typename AGENTTYPE, typename EI>
YouTubeEdgeContentManager<AGENTTYPE, EI>::~YouTubeEdgeContentManager(){}

template<typename AGENTTYPE, typename EI>
YouTubeEdge<AGENTTYPE, EI>* YouTubeEdgeContentManager<AGENTTYPE, EI>::createEdge(YouTubeEdgeContent<AGENTTYPE, EI>& content, repast::Context<AGENTTYPE>* context){
return new YouTubeEdge<AGENTTYPE, EI>(
		context->getAgent(content.source),
		context->getAgent(content.target),
		content.weight,
		content.edgeInfo);
}

template<typename AGENTTYPE, typename EI>
YouTubeEdgeContent<AGENTTYPE, EI>* YouTubeEdgeContentManager<AGENTTYPE, EI>::provideEdgeContent(YouTubeEdge<AGENTTYPE, EI>* edge){
	return new YouTubeEdgeContent<AGENTTYPE, EI>(edge);
}

} // End namespace youtube

#endif
