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
/* Telegram_Network.h */
#ifndef INCLUDE_TELEGRAM_NETWORK_H
#define INCLUDE_TELEGRAM_NETWORK_H

#include "repast_hpc/SharedContext.h"
#include "repast_hpc/SharedNetwork.h"

#include "socialnetwork_toolkit/SocialNetwork_Platform.h"
#include "socialnetwork_toolkit/EventCounter.h"
#include "platforms/Telegram/Telegram_Definitions.h"

using namespace rhpc_smple;

namespace telegram{

class TelegramFlagCounter: public SocialRelationshipIndicator<TelegramRelationshipType>{
public:
	TelegramFlagCounter();
	virtual ~TelegramFlagCounter();
};

class TelegramEventCounter: public SocialEventCounter<TelegramEventType>{
public:
	TelegramEventCounter();
	virtual ~TelegramEventCounter();
};


class TelegramEdgeInfo:
    public SocialNetwork_FlagEdgeInformation<TelegramRelationshipType, TelegramFlagCounter>,
    public SocialNetwork_CountEdgeInformation<TelegramEventType, TelegramEventCounter>{
   friend class boost::serialization::access;

public:
	TelegramEdgeInfo();
	virtual ~TelegramEdgeInfo();

	template<class Archive>
	void serialize(Archive& ar, const unsigned int version){
		SocialNetwork_FlagEdgeInformation<TelegramRelationshipType, TelegramFlagCounter>::serialize(ar, version);
		SocialNetwork_CountEdgeInformation<TelegramEventType, TelegramEventCounter>::serialize(ar, version);
	}
};

/* Custom Network Components */
template<typename AGENTTYPE, typename EI> // EdgeInf
class TelegramEdge : public repast::RepastEdge<AGENTTYPE>{
private:
    EI _edgeInfo;

 public:
    TelegramEdge();
    TelegramEdge(AGENTTYPE* source, AGENTTYPE* target);
    TelegramEdge(AGENTTYPE* source, AGENTTYPE* target, double weight);
    TelegramEdge(AGENTTYPE* source, AGENTTYPE* target, double weight, EI edgeInfo);

    TelegramEdge(boost::shared_ptr<AGENTTYPE> source, boost::shared_ptr<AGENTTYPE> target);
    TelegramEdge(boost::shared_ptr<AGENTTYPE> source, boost::shared_ptr<AGENTTYPE> target, double weight);
    TelegramEdge(boost::shared_ptr<AGENTTYPE> source, boost::shared_ptr<AGENTTYPE> target, double weight, EI edgeInfo);


    EI& getEdgeInfo();
    void setEdgeInfo(EI edgeInfo);

    // Querying
    // Note that this defines an edge in GitHub's vocabulary,
    // translating the flags and counters to meaningful statements for GitHub repos and users
    bool    follows(){        					return _edgeInfo.getFlag(FOLLOWS)     != 0; }
    bool    created(){        					return _edgeInfo.getFlag(CREATED)     != 0; }

    int     numIdles(){          				return _edgeInfo.getCount(IDLE); }
    int     numMessages(){        				return _edgeInfo.getCount(MESSAGE); }
    bool    any(){                              return !(_edgeInfo.isEmpty()); }

    // Updating
    void		logEvent(TelegramEventType event){ _edgeInfo.recordEvent(event); }

    void 	startFollow(){     				_edgeInfo.setFlag(FOLLOWS,     1); }
    void 	stopFollow(){      				_edgeInfo.setFlag(FOLLOWS,     0); }

	int     getTotalEvents(){
		  return numMessages();
	}

};


template<typename AGENTTYPE, typename EI>
TelegramEdge<AGENTTYPE, EI>::TelegramEdge() : _edgeInfo(){}
template<typename AGENTTYPE, typename EI>
TelegramEdge<AGENTTYPE, EI>::TelegramEdge(AGENTTYPE* source, AGENTTYPE* target) : repast::RepastEdge<AGENTTYPE>(source, target), _edgeInfo() {}
template<typename AGENTTYPE, typename EI>
TelegramEdge<AGENTTYPE, EI>::TelegramEdge(AGENTTYPE* source, AGENTTYPE* target, double weight) : repast::RepastEdge<AGENTTYPE>(source, target, weight), _edgeInfo() {}
template<typename AGENTTYPE, typename EI>
TelegramEdge<AGENTTYPE, EI>::TelegramEdge(AGENTTYPE* source, AGENTTYPE* target, double weight, EI edgeInfo) : repast::RepastEdge<AGENTTYPE>(source, target, weight), _edgeInfo(edgeInfo) {}
template<typename AGENTTYPE, typename EI>
TelegramEdge<AGENTTYPE, EI>::TelegramEdge(boost::shared_ptr<AGENTTYPE> source, boost::shared_ptr<AGENTTYPE> target) : repast::RepastEdge<AGENTTYPE>(source, target), _edgeInfo() {}
template<typename AGENTTYPE, typename EI>
TelegramEdge<AGENTTYPE, EI>::TelegramEdge(boost::shared_ptr<AGENTTYPE> source, boost::shared_ptr<AGENTTYPE> target, double weight) : repast::RepastEdge<AGENTTYPE>(source, target, weight), _edgeInfo() {}
template<typename AGENTTYPE, typename EI>
TelegramEdge<AGENTTYPE, EI>::TelegramEdge(boost::shared_ptr<AGENTTYPE> source, boost::shared_ptr<AGENTTYPE> target, double weight, EI edgeInfo) : repast::RepastEdge<AGENTTYPE>(source, target, weight), _edgeInfo(edgeInfo) {}

template<typename AGENTTYPE, typename EI>
EI& TelegramEdge<AGENTTYPE, EI>::getEdgeInfo(){ return _edgeInfo; }

template<typename AGENTTYPE, typename EI>
void TelegramEdge<AGENTTYPE, EI>::setEdgeInfo(EI edgeInfo){ _edgeInfo = edgeInfo; }



/* Custom Edge Content */
template<typename AGENTTYPE, typename EI> // Edge Info
struct TelegramEdgeContent : public repast::RepastEdgeContent<AGENTTYPE>{

    friend class boost::serialization::access;

public:
    EI edgeInfo;

    TelegramEdgeContent();
    TelegramEdgeContent(TelegramEdge<AGENTTYPE, EI>* edge);

    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) {
        repast::RepastEdgeContent<AGENTTYPE>::serialize(ar, version);
        ar & edgeInfo;
    }

};

template<typename AGENTTYPE, typename EI>
TelegramEdgeContent<AGENTTYPE, EI>::TelegramEdgeContent(){}

template<typename AGENTTYPE, typename EI>
TelegramEdgeContent<AGENTTYPE, EI>::TelegramEdgeContent(TelegramEdge<AGENTTYPE, EI>* edge):
                repast::RepastEdgeContent<AGENTTYPE>(edge),
                edgeInfo(edge->getEdgeInfo()) {  }




/* Custome Edge Content Manager */
template<typename AGENTTYPE, typename EI> // Edge Info
class TelegramEdgeContentManager
{
public:
    TelegramEdgeContentManager();
    virtual ~TelegramEdgeContentManager();
    TelegramEdge<AGENTTYPE, EI>* createEdge(TelegramEdgeContent<AGENTTYPE, EI>& content, repast::Context<AGENTTYPE>* context);
    TelegramEdgeContent<AGENTTYPE, EI>* provideEdgeContent(TelegramEdge<AGENTTYPE, EI>* edge);
};

template<typename AGENTTYPE, typename EI>
TelegramEdgeContentManager<AGENTTYPE, EI>::TelegramEdgeContentManager(){}

template<typename AGENTTYPE, typename EI>
TelegramEdgeContentManager<AGENTTYPE, EI>::~TelegramEdgeContentManager(){}

template<typename AGENTTYPE, typename EI>
TelegramEdge<AGENTTYPE, EI>* TelegramEdgeContentManager<AGENTTYPE, EI>::createEdge(TelegramEdgeContent<AGENTTYPE, EI>& content, repast::Context<AGENTTYPE>* context){
return new TelegramEdge<AGENTTYPE, EI>(
		context->getAgent(content.source),
		context->getAgent(content.target),
		content.weight,
		content.edgeInfo);
}

template<typename AGENTTYPE, typename EI>
TelegramEdgeContent<AGENTTYPE, EI>* TelegramEdgeContentManager<AGENTTYPE, EI>::provideEdgeContent(TelegramEdge<AGENTTYPE, EI>* edge){
	return new TelegramEdgeContent<AGENTTYPE, EI>(edge);
}

} // End namespace telegram

#endif /* INCLUDE_TELEGRAM_NETWORK_H */
