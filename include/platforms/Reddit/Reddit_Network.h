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
/* Reddit_Network.h */
#ifndef INCLUDE_REDDIT_NETWORK_H
#define INCLUDE_REDDIT_NETWORK_H

#include "repast_hpc/SharedContext.h"
#include "repast_hpc/SharedNetwork.h"

#include "socialnetwork_toolkit/SocialNetwork_Platform.h"
#include "socialnetwork_toolkit/EventCounter.h"
#include "platforms/Reddit/Reddit_Definitions.h"

using namespace rhpc_smple;

namespace reddit{

class RedditFlagCounter: public SocialRelationshipIndicator<RedditRelationshipType>{
public:
	RedditFlagCounter();
	virtual ~RedditFlagCounter();
};

class RedditEventCounter: public SocialEventCounter<RedditEventType>{
public:
	RedditEventCounter();
	virtual ~RedditEventCounter();
};

class RedditEdgeInfo:
    public SocialNetwork_FlagEdgeInformation<RedditRelationshipType, RedditFlagCounter>,
    public SocialNetwork_CountEdgeInformation<RedditEventType, RedditEventCounter>{
	friend class boost::serialization::access;

public:
	RedditEdgeInfo();

	virtual ~RedditEdgeInfo();

	template<class Archive>
	void serialize(Archive& ar, const unsigned int version){
		SocialNetwork_FlagEdgeInformation<RedditRelationshipType, RedditFlagCounter>::serialize(ar, version);
		SocialNetwork_CountEdgeInformation<RedditEventType, RedditEventCounter>::serialize(ar, version);
	}
};


/* Custom Network Components */
template<typename AGENTTYPE, typename EI> // EdgeInf
class RedditEdge : public repast::RepastEdge<AGENTTYPE>{
private:
    EI _edgeInfo;
    
 public:
    RedditEdge();
    RedditEdge(AGENTTYPE* source, AGENTTYPE* target);
    RedditEdge(AGENTTYPE* source, AGENTTYPE* target, double weight);
    RedditEdge(AGENTTYPE* source, AGENTTYPE* target, double weight, EI edgeInfo);
    
    RedditEdge(boost::shared_ptr<AGENTTYPE> source, boost::shared_ptr<AGENTTYPE> target);
    RedditEdge(boost::shared_ptr<AGENTTYPE> source, boost::shared_ptr<AGENTTYPE> target, double weight);
    RedditEdge(boost::shared_ptr<AGENTTYPE> source, boost::shared_ptr<AGENTTYPE> target, double weight, EI edgeInfo);
    
    
    EI& getEdgeInfo();
    void setEdgeInfo(EI edgeInfo);
    
    // Querying
    // Note that this defines an edge in Reddit's vocabulary,
    // translating the flags and counters to meaningful statements for Reddit repos and users
    bool    follows(){        					return _edgeInfo.getFlag(SUBSCRIBED)  != 0; }

    int     numIdles(){          				return _edgeInfo.getCount(IDLE); }
    int     numPosts(){           				return _edgeInfo.getCount(POST); }
    int     numComments(){       				return _edgeInfo.getCount(COMMENT); }

    bool    any(){                               return !(_edgeInfo.isEmpty()); }

    // Updating
    void		logEvent(RedditEventType event){ _edgeInfo.recordEvent(event); }

    int     getTotalEvents(){
    	  return numPosts() + numComments();
    }

};


template<typename AGENTTYPE, typename EI>
RedditEdge<AGENTTYPE, EI>::RedditEdge():_edgeInfo(){}
template<typename AGENTTYPE, typename EI>
RedditEdge<AGENTTYPE, EI>::RedditEdge(AGENTTYPE* source, AGENTTYPE* target) : repast::RepastEdge<AGENTTYPE>(source, target), _edgeInfo() {}
template<typename AGENTTYPE, typename EI>
RedditEdge<AGENTTYPE, EI>::RedditEdge(AGENTTYPE* source, AGENTTYPE* target, double weight) : repast::RepastEdge<AGENTTYPE>(source, target, weight), _edgeInfo() {}
template<typename AGENTTYPE, typename EI>
RedditEdge<AGENTTYPE, EI>::RedditEdge(AGENTTYPE* source, AGENTTYPE* target, double weight, EI edgeInfo) : repast::RepastEdge<AGENTTYPE>(source, target, weight), _edgeInfo(edgeInfo) {}
template<typename AGENTTYPE, typename EI>
RedditEdge<AGENTTYPE, EI>::RedditEdge(boost::shared_ptr<AGENTTYPE> source, boost::shared_ptr<AGENTTYPE> target) : repast::RepastEdge<AGENTTYPE>(source, target), _edgeInfo() {}
template<typename AGENTTYPE, typename EI>
RedditEdge<AGENTTYPE, EI>::RedditEdge(boost::shared_ptr<AGENTTYPE> source, boost::shared_ptr<AGENTTYPE> target, double weight) : repast::RepastEdge<AGENTTYPE>(source, target, weight), _edgeInfo() {}
template<typename AGENTTYPE, typename EI>
RedditEdge<AGENTTYPE, EI>::RedditEdge(boost::shared_ptr<AGENTTYPE> source, boost::shared_ptr<AGENTTYPE> target, double weight, EI edgeInfo) : repast::RepastEdge<AGENTTYPE>(source, target, weight), _edgeInfo(edgeInfo) {}

template<typename AGENTTYPE, typename EI>
EI& RedditEdge<AGENTTYPE, EI>::getEdgeInfo(){
	return _edgeInfo;
}

template<typename AGENTTYPE, typename EI>
void RedditEdge<AGENTTYPE, EI>::setEdgeInfo(EI edgeInfo){ _edgeInfo = edgeInfo; }


/* Custom Edge Content */
template<typename AGENTTYPE, typename EI> // Edge Info
struct RedditEdgeContent : public repast::RepastEdgeContent<AGENTTYPE>{
    
    friend class boost::serialization::access;
    
public:
    EI edgeInfo;

    RedditEdgeContent();
    RedditEdgeContent(RedditEdge<AGENTTYPE, EI>* edge);
    
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) {
        repast::RepastEdgeContent<AGENTTYPE>::serialize(ar, version);
        ar & edgeInfo;
    }
    
};

template<typename AGENTTYPE, typename EI>
RedditEdgeContent<AGENTTYPE, EI>::RedditEdgeContent(){}

template<typename AGENTTYPE, typename EI>
RedditEdgeContent<AGENTTYPE, EI>::RedditEdgeContent(RedditEdge<AGENTTYPE, EI>* edge):
                repast::RepastEdgeContent<AGENTTYPE>(edge),
                edgeInfo(edge->getEdgeInfo()) {  }

/* Custome Edge Content Manager */
template<typename AGENTTYPE, typename EI>// Edge Info
class RedditEdgeContentManager
{
public:
    RedditEdgeContentManager();
    virtual ~RedditEdgeContentManager();
    RedditEdge<AGENTTYPE, EI>* createEdge(RedditEdgeContent<AGENTTYPE, EI>& content, repast::Context<AGENTTYPE>* context);
    RedditEdgeContent<AGENTTYPE, EI>* provideEdgeContent(RedditEdge<AGENTTYPE, EI>* edge);
};

template<typename AGENTTYPE, typename EI>
RedditEdgeContentManager<AGENTTYPE, EI>::RedditEdgeContentManager(){}

template<typename AGENTTYPE, typename EI>
RedditEdgeContentManager<AGENTTYPE, EI>::~RedditEdgeContentManager(){}

template<typename AGENTTYPE, typename EI>
RedditEdge<AGENTTYPE, EI>* RedditEdgeContentManager<AGENTTYPE, EI>::createEdge(RedditEdgeContent<AGENTTYPE, EI>& content, repast::Context<AGENTTYPE>* context){
return new RedditEdge<AGENTTYPE, EI>(
		context->getAgent(content.source),
		context->getAgent(content.target),
		content.weight,
		content.edgeInfo);
}

template<typename AGENTTYPE, typename EI>
RedditEdgeContent<AGENTTYPE, EI>* RedditEdgeContentManager<AGENTTYPE, EI>::provideEdgeContent(RedditEdge<AGENTTYPE, EI>* edge){
	return new RedditEdgeContent<AGENTTYPE, EI>(edge);
}

} // End namespace reddit

#endif
