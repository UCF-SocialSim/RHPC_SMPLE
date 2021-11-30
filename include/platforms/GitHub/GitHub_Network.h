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
/* Git_Network.h */
#ifndef INCLUDE_GITHUB_NETWORK_H
#define INCLUDE_GITHUB_NETWORK_H

#include "repast_hpc/SharedContext.h"
#include "repast_hpc/SharedNetwork.h"

#include "socialnetwork_toolkit/EventCounter.h"
#include "socialnetwork_toolkit/SocialNetwork_Platform.h"

#include "platforms/GitHub/GitHub_Definitions.h"

using namespace rhpc_smple;

namespace github{


class GitHubFlagCounter: public SocialRelationshipIndicator<GitHubRelationshipType>{
public:
	GitHubFlagCounter();
	virtual ~GitHubFlagCounter();
};

class GitHubEventCounter: public SocialEventCounter<GitHubEventType>{
public:
	GitHubEventCounter();
	virtual ~GitHubEventCounter();
};


class GitHubEdgeInfo:
    public SocialNetwork_FlagEdgeInformation<GitHubRelationshipType, GitHubFlagCounter>,
    public SocialNetwork_CountEdgeInformation<GitHubEventType, GitHubEventCounter>{
	friend class boost::serialization::access;

public:
	GitHubEdgeInfo();

	virtual ~GitHubEdgeInfo();

	template<class Archive>
	void serialize(Archive& ar, const unsigned int version){
		SocialNetwork_FlagEdgeInformation<GitHubRelationshipType, GitHubFlagCounter>::serialize(ar, version);
		SocialNetwork_CountEdgeInformation<GitHubEventType, GitHubEventCounter>::serialize(ar, version);
	}
};


/* Custom Network Components */
template<typename AGENTTYPE, typename EI> // EdgeInfo
class GitHubEdge : public repast::RepastEdge<AGENTTYPE>{
private:
    EI _edgeInfo;
    
 public:
    GitHubEdge();
    GitHubEdge(AGENTTYPE* source, AGENTTYPE* target);
    GitHubEdge(AGENTTYPE* source, AGENTTYPE* target, double weight);
    GitHubEdge(AGENTTYPE* source, AGENTTYPE* target, double weight, EI edgeInfo);
    
    GitHubEdge(boost::shared_ptr<AGENTTYPE> source, boost::shared_ptr<AGENTTYPE> target);
    GitHubEdge(boost::shared_ptr<AGENTTYPE> source, boost::shared_ptr<AGENTTYPE> target, double weight);
    GitHubEdge(boost::shared_ptr<AGENTTYPE> source, boost::shared_ptr<AGENTTYPE> target, double weight, EI edgeInfo);
    
    
    EI& getEdgeInfo();
    void setEdgeInfo(EI edgeInfo);
    
    // Querying
    // Note that this defines an edge in GitHub's vocabulary,
    // translating the flags and counters to meaningful statements for GitHub repos and users
    bool    follows(){        					return _edgeInfo.getFlag(FOLLOWS)     != 0; }
    bool    created(){        					return _edgeInfo.getFlag(CREATED)     != 0; }
    bool    owns(){           					return _edgeInfo.getFlag(OWNS)        != 0; }
    bool    contrib(){        					return _edgeInfo.getFlag(CONTRIBUTES) != 0; }

    int     numIdles(){          				return _edgeInfo.getCount(IDLE); }
    int     numSignups(){        				return _edgeInfo.getCount(USERSIGNUP); }
    int     numCreateRepos(){    				return _edgeInfo.getCount(CREATE_REPO); }
    int     numFollows(){        				return _edgeInfo.getCount(FOLLOW_USER); }
    int     numWatches(){        				return _edgeInfo.getCount(WATCH_REPO); }
    int     numForks(){          				return _edgeInfo.getCount(FORK); }
    int     numGollums(){        				return _edgeInfo.getCount(GOLLUM); }
    int     numCommitComments(){ 				return _edgeInfo.getCount(COMMIT_COMMENT); }
    int     numPullRequests(){   				return _edgeInfo.getCount(PULLREQUEST); }
    int     numIssueComments(){  				return _edgeInfo.getCount(ISSUE_COMMENT); }
    int     numIssues(){         				return _edgeInfo.getCount(ISSUE); }
    int     numPullRequestReviewComments(){   	return _edgeInfo.getCount(PULLREQUESTREVIEWCOMMENT); }
    int     numPushes(){         				return _edgeInfo.getCount(PUSH); }
    int     numReleases(){         				return _edgeInfo.getCount(RELEASE); }
    int     numMemberEvents(){         			return _edgeInfo.getCount(MEMBER); }
    int     numPublicEvents(){         			return _edgeInfo.getCount(PUBLIC); }
    int     numDeleteEvents(){         			return _edgeInfo.getCount(DELETE); }

    bool    any(){                              return !(_edgeInfo.isEmpty()); }

    // Updating
    void		logEvent(GitHubEventType event){ _edgeInfo.recordEvent(event); }

    void 	startFollow(){     				_edgeInfo.setFlag(FOLLOWS,     1); }
	void 	stopFollow(){      				_edgeInfo.setFlag(FOLLOWS,     0); }
	void 	startOwn(){        				_edgeInfo.setFlag(OWNS,        1); }
	void 	stopOwn(){         				_edgeInfo.setFlag(OWNS,        0); }
	void 	startContrib(){    				_edgeInfo.setFlag(CONTRIBUTES, 1); }
	void 	stopContrib(){    				_edgeInfo.setFlag(CONTRIBUTES, 0); }


	int     getTotalEvents(){
		  return numSignups() + numCreateRepos() + numFollows() + numWatches() +
				 numForks() + numGollums() + numCommitComments() + numPullRequests() +
				 numIssueComments() + numIssues() + numPullRequestReviewComments() +
				 numPushes() + numReleases() + numMemberEvents() + numPublicEvents() +
				 numDeleteEvents();
	}

};


template<typename AGENTTYPE, typename EI>
GitHubEdge<AGENTTYPE, EI>::GitHubEdge() : _edgeInfo(){}
template<typename AGENTTYPE, typename EI>
GitHubEdge<AGENTTYPE, EI>::GitHubEdge(AGENTTYPE* source, AGENTTYPE* target) : repast::RepastEdge<AGENTTYPE>(source, target), _edgeInfo() {}
template<typename AGENTTYPE, typename EI>
GitHubEdge<AGENTTYPE, EI>::GitHubEdge(AGENTTYPE* source, AGENTTYPE* target, double weight) : repast::RepastEdge<AGENTTYPE>(source, target, weight), _edgeInfo() {}
template<typename AGENTTYPE, typename EI>
GitHubEdge<AGENTTYPE, EI>::GitHubEdge(AGENTTYPE* source, AGENTTYPE* target, double weight, EI edgeInfo) : repast::RepastEdge<AGENTTYPE>(source, target, weight), _edgeInfo(edgeInfo) {}
template<typename AGENTTYPE, typename EI>
GitHubEdge<AGENTTYPE, EI>::GitHubEdge(boost::shared_ptr<AGENTTYPE> source, boost::shared_ptr<AGENTTYPE> target) : repast::RepastEdge<AGENTTYPE>(source, target), _edgeInfo() {}
template<typename AGENTTYPE, typename EI>
GitHubEdge<AGENTTYPE, EI>::GitHubEdge(boost::shared_ptr<AGENTTYPE> source, boost::shared_ptr<AGENTTYPE> target, double weight) : repast::RepastEdge<AGENTTYPE>(source, target, weight), _edgeInfo() {}
template<typename AGENTTYPE, typename EI>
GitHubEdge<AGENTTYPE, EI>::GitHubEdge(boost::shared_ptr<AGENTTYPE> source, boost::shared_ptr<AGENTTYPE> target, double weight, EI edgeInfo) : repast::RepastEdge<AGENTTYPE>(source, target, weight), _edgeInfo(edgeInfo) {}

template<typename AGENTTYPE, typename EI>
EI& GitHubEdge<AGENTTYPE, EI>::getEdgeInfo(){ return _edgeInfo; }

template<typename AGENTTYPE, typename EI>
void GitHubEdge<AGENTTYPE, EI>::setEdgeInfo(EI edgeInfo){ _edgeInfo = edgeInfo; }


/* Custom Edge Content */
template<typename AGENTTYPE, typename EI> // Edge Info
struct GitHubEdgeContent : public repast::RepastEdgeContent<AGENTTYPE>{
    
    friend class boost::serialization::access;
    
public:
    EI edgeInfo;

    GitHubEdgeContent();
    GitHubEdgeContent(GitHubEdge<AGENTTYPE, EI>* edge);
    
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) {
        repast::RepastEdgeContent<AGENTTYPE>::serialize(ar, version);
        ar & edgeInfo;
    }
    
};

template<typename AGENTTYPE, typename EI>
GitHubEdgeContent<AGENTTYPE, EI>::GitHubEdgeContent(){}

template<typename AGENTTYPE, typename EI>
GitHubEdgeContent<AGENTTYPE, EI>::GitHubEdgeContent(GitHubEdge<AGENTTYPE, EI>* edge):
                repast::RepastEdgeContent<AGENTTYPE>(edge),
                edgeInfo(edge->getEdgeInfo()) {  }

/* Custome Edge Content Manager */
template<typename AGENTTYPE, typename EI> // Edge Info
class GitHubEdgeContentManager{
public:
    GitHubEdgeContentManager();
    virtual ~GitHubEdgeContentManager();
    GitHubEdge<AGENTTYPE, EI>* createEdge(GitHubEdgeContent<AGENTTYPE, EI>& content, repast::Context<AGENTTYPE>* context);
    GitHubEdgeContent<AGENTTYPE, EI>* provideEdgeContent(GitHubEdge<AGENTTYPE, EI>* edge);
};

template<typename AGENTTYPE, typename EI>
GitHubEdgeContentManager<AGENTTYPE, EI>::GitHubEdgeContentManager(){}

template<typename AGENTTYPE, typename EI>
GitHubEdgeContentManager<AGENTTYPE, EI>::~GitHubEdgeContentManager(){}

template<typename AGENTTYPE, typename EI>
GitHubEdge<AGENTTYPE, EI>* GitHubEdgeContentManager<AGENTTYPE, EI>::createEdge(GitHubEdgeContent<AGENTTYPE, EI>& content, repast::Context<AGENTTYPE>* context){
return new GitHubEdge<AGENTTYPE, EI>(
		context->getAgent(content.source),
		context->getAgent(content.target),
		content.weight,
		content.edgeInfo);
}

template<typename AGENTTYPE, typename EI>
GitHubEdgeContent<AGENTTYPE, EI>* GitHubEdgeContentManager<AGENTTYPE, EI>::provideEdgeContent(GitHubEdge<AGENTTYPE, EI>* edge){
	return new GitHubEdgeContent<AGENTTYPE, EI>(edge);
}

} // End namespace github

#endif
