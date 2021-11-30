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
/* Twitter_Network.h */
#ifndef INCLUDE_TWITTER_NETWORK_H
#define INCLUDE_TWITTER_NETWORK_H

#include "repast_hpc/SharedContext.h"
#include "repast_hpc/SharedNetwork.h"

#include "socialnetwork_toolkit/SocialNetwork_Platform.h"
#include "socialnetwork_toolkit/EventCounter.h"

#include "platforms/Twitter/Twitter_Definitions.h"

using namespace rhpc_smple;

namespace twitter{

class TwitterFlagCounter: public SocialRelationshipIndicator<TwitterRelationshipType>{
public:
	TwitterFlagCounter();
	virtual ~TwitterFlagCounter();
};

class TwitterEventCounter: public SocialEventCounter<TwitterEventType>{
public:
	TwitterEventCounter();
	virtual ~TwitterEventCounter();
};

class TwitterEdgeInfo:
    public SocialNetwork_FlagEdgeInformation<TwitterRelationshipType, TwitterFlagCounter>,
    public SocialNetwork_CountEdgeInformation<TwitterEventType, TwitterEventCounter>{
	friend class boost::serialization::access;

public:
	TwitterEdgeInfo();;

	virtual ~TwitterEdgeInfo();

	template<class Archive>
	void serialize(Archive& ar, const unsigned int version){
		SocialNetwork_FlagEdgeInformation<TwitterRelationshipType, TwitterFlagCounter>::serialize(ar, version);
		SocialNetwork_CountEdgeInformation<TwitterEventType, TwitterEventCounter>::serialize(ar, version);;
	}
};


/* Custom Network Components */

template<typename AGENTTYPE, typename EI> // EdgeInf
class TwitterEdge : public repast::RepastEdge<AGENTTYPE>{
private:
    EI _edgeInfo;

 public:
    TwitterEdge();
    TwitterEdge(AGENTTYPE* source, AGENTTYPE* target);
    TwitterEdge(AGENTTYPE* source, AGENTTYPE* target, double weight);
    TwitterEdge(AGENTTYPE* source, AGENTTYPE* target, double weight, EI edgeInfo);

    TwitterEdge(boost::shared_ptr<AGENTTYPE> source, boost::shared_ptr<AGENTTYPE> target);
    TwitterEdge(boost::shared_ptr<AGENTTYPE> source, boost::shared_ptr<AGENTTYPE> target, double weight);
    TwitterEdge(boost::shared_ptr<AGENTTYPE> source, boost::shared_ptr<AGENTTYPE> target, double weight, EI edgeInfo);


    EI& getEdgeInfo();
    void setEdgeInfo(EI edgeInfo);

    // Querying
    // Note that this defines an edge in Twitter's vocabulary,
    // translating the flags and counters to meaningful statements for Twitter content and users
    bool follows(){       return _edgeInfo.getFlag(FOLLOWS)     != 0; }
    bool created(){       return _edgeInfo.getFlag(CREATED)     != 0; }

    int  numIdles(){      return _edgeInfo.getCount(IDLE); }
    int  numTweets(){     return _edgeInfo.getCount(TWEET); }
    int  numRetweets(){   return _edgeInfo.getCount(RETWEET); }
    int  numQuotes(){  	  return _edgeInfo.getCount(QUOTE); }
    int  numReplies(){    return _edgeInfo.getCount(REPLY); }

    bool any(){           return !(_edgeInfo.isEmpty()); }

    // Updating
    void logEvent(TwitterEventType event){ _edgeInfo.recordEvent(event); }

    void startFollow(){     				_edgeInfo.setFlag(FOLLOWS,     1); }
    void stopFollow(){      				_edgeInfo.setFlag(FOLLOWS,     0); }

	int  getTotalEvents(){
	    return numTweets() + numRetweets() + numQuotes() + numReplies();
	}

};


template<typename AGENTTYPE, typename EI>
TwitterEdge<AGENTTYPE, EI>::TwitterEdge(): _edgeInfo(){}

template<typename AGENTTYPE, typename EI>
TwitterEdge<AGENTTYPE, EI>::TwitterEdge(AGENTTYPE* source, AGENTTYPE* target) : repast::RepastEdge<AGENTTYPE>(source, target), _edgeInfo() {}

template<typename AGENTTYPE, typename EI>
TwitterEdge<AGENTTYPE, EI>::TwitterEdge(AGENTTYPE* source, AGENTTYPE* target, double weight) : repast::RepastEdge<AGENTTYPE>(source, target, weight), _edgeInfo() {}

template<typename AGENTTYPE, typename EI>
TwitterEdge<AGENTTYPE, EI>::TwitterEdge(AGENTTYPE* source, AGENTTYPE* target, double weight, EI edgeInfo) : repast::RepastEdge<AGENTTYPE>(source, target, weight), _edgeInfo(edgeInfo) {}

template<typename AGENTTYPE, typename EI>
TwitterEdge<AGENTTYPE, EI>::TwitterEdge(boost::shared_ptr<AGENTTYPE> source, boost::shared_ptr<AGENTTYPE> target) : repast::RepastEdge<AGENTTYPE>(source, target), _edgeInfo() {}

template<typename AGENTTYPE, typename EI>
TwitterEdge<AGENTTYPE, EI>::TwitterEdge(boost::shared_ptr<AGENTTYPE> source, boost::shared_ptr<AGENTTYPE> target, double weight) : repast::RepastEdge<AGENTTYPE>(source, target, weight), _edgeInfo() {}

template<typename AGENTTYPE, typename EI>
TwitterEdge<AGENTTYPE, EI>::TwitterEdge(boost::shared_ptr<AGENTTYPE> source, boost::shared_ptr<AGENTTYPE> target, double weight, EI edgeInfo) : repast::RepastEdge<AGENTTYPE>(source, target, weight), _edgeInfo(edgeInfo) {}

template<typename AGENTTYPE, typename EI>
EI& TwitterEdge<AGENTTYPE, EI>::getEdgeInfo(){ return _edgeInfo; }

template<typename AGENTTYPE, typename EI>
void TwitterEdge<AGENTTYPE, EI>::setEdgeInfo(EI edgeInfo){ _edgeInfo = edgeInfo; }


/* Custom Edge Content */
template<typename AGENTTYPE, typename EI> // Edge Info
struct TwitterEdgeContent : public repast::RepastEdgeContent<AGENTTYPE>{

    friend class boost::serialization::access;

public:
    EI edgeInfo;

    TwitterEdgeContent();
    TwitterEdgeContent(TwitterEdge<AGENTTYPE, EI>* edge);

    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) {
        repast::RepastEdgeContent<AGENTTYPE>::serialize(ar, version);
        ar & edgeInfo;
    }

};

template<typename AGENTTYPE, typename EI>
TwitterEdgeContent<AGENTTYPE, EI>::TwitterEdgeContent(){}

template<typename AGENTTYPE, typename EI>
TwitterEdgeContent<AGENTTYPE, EI>::TwitterEdgeContent(TwitterEdge<AGENTTYPE, EI>* edge):
                repast::RepastEdgeContent<AGENTTYPE>(edge),
                edgeInfo(edge->getEdgeInfo()) {  }




/* Custome Edge Content Manager */
template<typename AGENTTYPE, typename EI>
class TwitterEdgeContentManager {

public:
    TwitterEdgeContentManager();
    virtual ~TwitterEdgeContentManager();

    TwitterEdge<AGENTTYPE, EI>*        createEdge(TwitterEdgeContent<AGENTTYPE, EI>& content, repast::Context<AGENTTYPE>* context);
    TwitterEdgeContent<AGENTTYPE, EI>* provideEdgeContent(TwitterEdge<AGENTTYPE, EI>* edge);

};

template<typename AGENTTYPE, typename EI>
TwitterEdgeContentManager<AGENTTYPE, EI>::TwitterEdgeContentManager(){}

template<typename AGENTTYPE, typename EI>
TwitterEdgeContentManager<AGENTTYPE, EI>::~TwitterEdgeContentManager(){}

template<typename AGENTTYPE, typename EI>
TwitterEdge<AGENTTYPE, EI>* TwitterEdgeContentManager<AGENTTYPE, EI>::createEdge(TwitterEdgeContent<AGENTTYPE, EI>& content, repast::Context<AGENTTYPE>* context){
return new TwitterEdge<AGENTTYPE, EI>(
		context->getAgent(content.source),
		context->getAgent(content.target),
		content.weight,
		content.edgeInfo);
}

template<typename AGENTTYPE, typename EI>
TwitterEdgeContent<AGENTTYPE, EI>* TwitterEdgeContentManager<AGENTTYPE, EI>::provideEdgeContent(TwitterEdge<AGENTTYPE, EI>* edge){
	return new TwitterEdgeContent<AGENTTYPE, EI>(edge);
}

} // End namespace twitter

#endif /* INCLUDE_TWITTER_NETWORK_H */
