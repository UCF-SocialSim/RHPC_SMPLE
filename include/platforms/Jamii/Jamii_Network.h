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
/* Jamii_Network.h */
#ifndef INCLUDE_JAMII_NETWORK_H
#define INCLUDE_JAMII_NETWORK_H

#include "repast_hpc/SharedContext.h"
#include "repast_hpc/SharedNetwork.h"

#include "socialnetwork_toolkit/EventCounter.h"
#include "socialnetwork_toolkit/SocialNetwork_Platform.h"

#include "platforms/Jamii/Jamii_Definitions.h"

using namespace rhpc_smple;

namespace jamii{

class JamiiEventCounter: public SocialEventCounter<JamiiEventType>{
public:
	JamiiEventCounter();
	virtual ~JamiiEventCounter();
};


class JamiiEdgeInfo:
    public SocialNetwork_CountEdgeInformation<JamiiEventType, JamiiEventCounter>{
	friend class boost::serialization::access;

public:
	JamiiEdgeInfo();

	virtual ~JamiiEdgeInfo();

	template<class Archive>
	void serialize(Archive& ar, const unsigned int version){
		SocialNetwork_CountEdgeInformation<JamiiEventType, JamiiEventCounter>::serialize(ar, version);
	}
};






/* Custom Network Components */
template<typename AGENTTYPE, typename EI> // EdgeInfo
class JamiiEdge : public repast::RepastEdge<AGENTTYPE>{
private:
    EI _edgeInfo;

 public:
    JamiiEdge();
    JamiiEdge(AGENTTYPE* source, AGENTTYPE* target);
    JamiiEdge(AGENTTYPE* source, AGENTTYPE* target, double weight);
    JamiiEdge(AGENTTYPE* source, AGENTTYPE* target, double weight, EI edgeInfo);

    JamiiEdge(boost::shared_ptr<AGENTTYPE> source, boost::shared_ptr<AGENTTYPE> target);
    JamiiEdge(boost::shared_ptr<AGENTTYPE> source, boost::shared_ptr<AGENTTYPE> target, double weight);
    JamiiEdge(boost::shared_ptr<AGENTTYPE> source, boost::shared_ptr<AGENTTYPE> target, double weight, EI edgeInfo);


    EI& getEdgeInfo();
    void setEdgeInfo(EI edgeInfo);

    // Querying
    int     numPosts(){          				return _edgeInfo.getCount(COMMENT); }

    bool    any(){                              return !(_edgeInfo.isEmpty()); }

    // Updating
    void	logEvent(JamiiEventType event){ _edgeInfo.recordEvent(event); }


	int     getTotalEvents(){
		  return numPosts();
	}

};


template<typename AGENTTYPE, typename EI>
JamiiEdge<AGENTTYPE, EI>::JamiiEdge() : _edgeInfo(){}
template<typename AGENTTYPE, typename EI>
JamiiEdge<AGENTTYPE, EI>::JamiiEdge(AGENTTYPE* source, AGENTTYPE* target) : repast::RepastEdge<AGENTTYPE>(source, target), _edgeInfo() {}
template<typename AGENTTYPE, typename EI>
JamiiEdge<AGENTTYPE, EI>::JamiiEdge(AGENTTYPE* source, AGENTTYPE* target, double weight) : repast::RepastEdge<AGENTTYPE>(source, target, weight), _edgeInfo() {}
template<typename AGENTTYPE, typename EI>
JamiiEdge<AGENTTYPE, EI>::JamiiEdge(AGENTTYPE* source, AGENTTYPE* target, double weight, EI edgeInfo) : repast::RepastEdge<AGENTTYPE>(source, target, weight), _edgeInfo(edgeInfo) {}
template<typename AGENTTYPE, typename EI>
JamiiEdge<AGENTTYPE, EI>::JamiiEdge(boost::shared_ptr<AGENTTYPE> source, boost::shared_ptr<AGENTTYPE> target) : repast::RepastEdge<AGENTTYPE>(source, target), _edgeInfo() {}
template<typename AGENTTYPE, typename EI>
JamiiEdge<AGENTTYPE, EI>::JamiiEdge(boost::shared_ptr<AGENTTYPE> source, boost::shared_ptr<AGENTTYPE> target, double weight) : repast::RepastEdge<AGENTTYPE>(source, target, weight), _edgeInfo() {}
template<typename AGENTTYPE, typename EI>
JamiiEdge<AGENTTYPE, EI>::JamiiEdge(boost::shared_ptr<AGENTTYPE> source, boost::shared_ptr<AGENTTYPE> target, double weight, EI edgeInfo) : repast::RepastEdge<AGENTTYPE>(source, target, weight), _edgeInfo(edgeInfo) {}

template<typename AGENTTYPE, typename EI>
EI& JamiiEdge<AGENTTYPE, EI>::getEdgeInfo(){ return _edgeInfo; }

template<typename AGENTTYPE, typename EI>
void JamiiEdge<AGENTTYPE, EI>::setEdgeInfo(EI edgeInfo){ _edgeInfo = edgeInfo; }


/* Custom Edge Content */
template<typename AGENTTYPE, typename EI> // Edge Info
struct JamiiEdgeContent : public repast::RepastEdgeContent<AGENTTYPE>{

    friend class boost::serialization::access;

public:
    EI edgeInfo;

    JamiiEdgeContent();
    JamiiEdgeContent(JamiiEdge<AGENTTYPE, EI>* edge);

    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) {
        repast::RepastEdgeContent<AGENTTYPE>::serialize(ar, version);
        ar & edgeInfo;
    }

};

template<typename AGENTTYPE, typename EI>
JamiiEdgeContent<AGENTTYPE, EI>::JamiiEdgeContent(){}

template<typename AGENTTYPE, typename EI>
JamiiEdgeContent<AGENTTYPE, EI>::JamiiEdgeContent(JamiiEdge<AGENTTYPE, EI>* edge):
                repast::RepastEdgeContent<AGENTTYPE>(edge),
                edgeInfo(edge->getEdgeInfo()) {  }

/* Custome Edge Content Manager */
template<typename AGENTTYPE, typename EI> // Edge Info
class JamiiEdgeContentManager{
public:
    JamiiEdgeContentManager();
    virtual ~JamiiEdgeContentManager();
    JamiiEdge<AGENTTYPE, EI>* createEdge(JamiiEdgeContent<AGENTTYPE, EI>& content, repast::Context<AGENTTYPE>* context);
    JamiiEdgeContent<AGENTTYPE, EI>* provideEdgeContent(JamiiEdge<AGENTTYPE, EI>* edge);
};

template<typename AGENTTYPE, typename EI>
JamiiEdgeContentManager<AGENTTYPE, EI>::JamiiEdgeContentManager(){}

template<typename AGENTTYPE, typename EI>
JamiiEdgeContentManager<AGENTTYPE, EI>::~JamiiEdgeContentManager(){}

template<typename AGENTTYPE, typename EI>
JamiiEdge<AGENTTYPE, EI>* JamiiEdgeContentManager<AGENTTYPE, EI>::createEdge(JamiiEdgeContent<AGENTTYPE, EI>& content, repast::Context<AGENTTYPE>* context){
return new JamiiEdge<AGENTTYPE, EI>(
		context->getAgent(content.source),
		context->getAgent(content.target),
		content.weight,
		content.edgeInfo);
}

template<typename AGENTTYPE, typename EI>
JamiiEdgeContent<AGENTTYPE, EI>* JamiiEdgeContentManager<AGENTTYPE, EI>::provideEdgeContent(JamiiEdge<AGENTTYPE, EI>* edge){
	return new JamiiEdgeContent<AGENTTYPE, EI>(edge);
}







} // End namespace jamii

#endif /* INCLUDE_JAMII_NETWORK_H */
