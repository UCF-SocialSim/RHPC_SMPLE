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
/* GitHub_Platform.h */
#ifndef INCLUDE_GITHUB_PLATFORM_H
#define INCLUDE_GITHUB_PLATFORM_H

#include <vector>

#include "utilities/SimpleWeightedSelector.h"

#include "socialnetwork_toolkit/SocialNetwork_Platform.h"
#include "socialnetwork_toolkit/BehaviorSelection.h"

#include "platforms/GitHub/GitHub_Definitions.h"
#include "platforms/GitHub/GitHub_Agent.h"
#include "platforms/GitHub/GitHub_Network.h"

using namespace rhpc_smple;

namespace github{


template<typename AGENTTYPE, typename ACTIONTYPE, typename SCORETYPE, typename PACKAGETYPE, typename FILEWRITERTYPE>
class AbstractGitHub:
		public SocialNetwork_Platform<GitHubAgentType,
		                              AGENTTYPE,
									  GitHubEdge<AGENTTYPE, GitHubEdgeInfo>,
									  GitHubEdgeContent<AGENTTYPE, GitHubEdgeInfo>,
									  GitHubEdgeContentManager<AGENTTYPE, GitHubEdgeInfo>,
									  PACKAGETYPE,
									  ACTIONTYPE,
									  GitHubFeedQuery,
									  GitHubFeedExplanation,
									  FILEWRITERTYPE>{
protected:
	GitHubFeed<SCORETYPE>* currentFittest;
	void addRandomReposToFeed(GitHubFeed<SCORETYPE>& feed, int count);

public:
	AbstractGitHub(boost::mpi::communicator* world, ScenarioWithAgents<AGENTTYPE>* scenario);
	~AbstractGitHub();

	virtual void init_schedule(repast::ScheduleRunner& runner, double offset = 0){}

	virtual boost::shared_ptr<GitHubFeed<SCORETYPE>> getFeed(AGENTTYPE* requestor, GitHubFeedQuery query);

	// API Functions
	void doAction(ACTIONTYPE action);
	void api_createRepo(ACTIONTYPE action);
	void api_watchRepo(ACTIONTYPE action);
	void api_followUser(ACTIONTYPE action);
	void api_fork(ACTIONTYPE action);
	void api_gollum(ACTIONTYPE action);
	void api_commit(ACTIONTYPE action);
	void api_pullRequest(ACTIONTYPE action);
	void api_issueComment(ACTIONTYPE action);
	void api_issue(ACTIONTYPE action);
	void api_pullRequestReviewComment(ACTIONTYPE action);
	void api_push(ACTIONTYPE action);
	void api_release(ACTIONTYPE action);
	void api_member(ACTIONTYPE action);
	void api_public(ACTIONTYPE action);
	void api_delete(ACTIONTYPE action);


	void handleAction(ACTIONTYPE action);

	long getTotalActionCount(AGENTTYPE* source);

//	void selectFittestAgents(int n, std::vector<GitAgent*>& agents, GitHubAgentType type);
//	void updateCurrentFittestFeed(int n);
	void getSelector(AGENTTYPE* source, SimpleWeightedSelector& sws);

//	std::map<std::string, double>& getShocks(){
//		return enclosingScenario->getShocks();
//	}

};


template<typename AGENTTYPE, typename ACTIONTYPE, typename SCORETYPE, typename PACKAGETYPE, typename FILEWRITERTYPE>
AbstractGitHub<AGENTTYPE, ACTIONTYPE, SCORETYPE, PACKAGETYPE, FILEWRITERTYPE>::AbstractGitHub(boost::mpi::communicator* world, ScenarioWithAgents<AGENTTYPE>* scenario):
      SocialNetwork_Platform<GitHubAgentType,
		                              AGENTTYPE,
									  GitHubEdge<AGENTTYPE, GitHubEdgeInfo>,
									  GitHubEdgeContent<AGENTTYPE, GitHubEdgeInfo>,
									  GitHubEdgeContentManager<AGENTTYPE, GitHubEdgeInfo>,
									  PACKAGETYPE,
									  ACTIONTYPE,
									  GitHubFeedQuery,
									  GitHubFeedExplanation,
									  FILEWRITERTYPE>(world, "github", "GH", 20, scenario){

}


template<typename AGENTTYPE, typename ACTIONTYPE, typename SCORETYPE, typename PACKAGETYPE, typename FILEWRITERTYPE>
AbstractGitHub<AGENTTYPE, ACTIONTYPE, SCORETYPE, PACKAGETYPE, FILEWRITERTYPE>::~AbstractGitHub(){}






template<typename AGENTTYPE, typename ACTIONTYPE, typename SCORETYPE, typename PACKAGETYPE, typename FILEWRITERTYPE>
boost::shared_ptr<GitHubFeed<SCORETYPE>> AbstractGitHub<AGENTTYPE, ACTIONTYPE, SCORETYPE, PACKAGETYPE, FILEWRITERTYPE>::getFeed(AGENTTYPE* requestor, GitHubFeedQuery query){
	repast::Logger& logger = repast::Log4CL::instance()->get_logger("github");
	boost::shared_ptr<GitHubFeed<SCORETYPE>> ret(new GitHubFeed<SCORETYPE>());
	if(query.type == GitHubFeedQueryType::RETRIEVE_RANDOM_REPOS){
		addRandomReposToFeed(*ret, query.countOfRandomReposToRetrieve);
	}
	return ret;
}

template<typename AGENTTYPE, typename ACTIONTYPE, typename SCORETYPE, typename PACKAGETYPE, typename FILEWRITERTYPE>
void AbstractGitHub<AGENTTYPE, ACTIONTYPE, SCORETYPE, PACKAGETYPE, FILEWRITERTYPE>::addRandomReposToFeed(GitHubFeed<SCORETYPE>& feed, int count){
	typename std::vector<AGENTTYPE*> repos;
	this->context.selectAgents(count, repos, REPO_AGENT);
	typename std::vector<AGENTTYPE*>::iterator iter = repos.begin();
	while(iter != repos.end()){
		GitHubFeedExplanation feedExplanation;
		feedExplanation.randomlySelected = true;
		feed.push(*iter, 0, 0, IDLE, feedExplanation); // Pushes in slot 1, as 'actor'
		iter++;
	}
}


template<typename AGENTTYPE, typename ACTIONTYPE, typename SCORETYPE, typename PACKAGETYPE, typename FILEWRITERTYPE>
void AbstractGitHub<AGENTTYPE, ACTIONTYPE, SCORETYPE, PACKAGETYPE, FILEWRITERTYPE>::doAction(ACTIONTYPE action){
	GitHubEventType eventType = action.type;
	switch (eventType) {
	    case DELETE:		            api_delete(action);                   break;
		case CREATE_REPO: 	            api_createRepo(action);               break;
		case WATCH_REPO:                api_watchRepo(action);	              break;
		case FORK:		                api_fork(action);			          break;
		case COMMIT_COMMENT:            api_commit(action);			          break;
		case PULLREQUEST:	            api_pullRequest(action);		      break;
		case ISSUE_COMMENT:		        api_issueComment(action);       	  break;
		case ISSUE:			            api_issue(action);			          break;
		case GOLLUM:			        api_gollum(action);			          break;
		case PULLREQUESTREVIEWCOMMENT:	api_pullRequestReviewComment(action); break;
		case PUSH:			            api_push(action);			          break;
		case RELEASE:			        api_release(action);	        	  break;
		case MEMBER:			        api_member(action);		              break;
		case PUBLIC:			        api_public(action);			          break;
		default:{ }
	}
}

template<typename AGENTTYPE, typename ACTIONTYPE, typename SCORETYPE, typename PACKAGETYPE, typename FILEWRITERTYPE>
void AbstractGitHub<AGENTTYPE, ACTIONTYPE, SCORETYPE, PACKAGETYPE, FILEWRITERTYPE>::api_delete(ACTIONTYPE action){
//	((GitRepoAgent*)(action.target))->informationIDs.insert(action.payload.informationIDs.begin(), action.payload.informationIDs.end());
//	getEdge(action.actor, action.target)->logEvent(DELETE); // Note: Deletion is not actually deleting the repo, it's deleting a branch or tag
//	countEvent(DELETE);
//	action.target->touch();
//	if(directPropagation) propagate(action);
//	writeAction(action);
}

template<typename AGENTTYPE, typename ACTIONTYPE, typename SCORETYPE, typename PACKAGETYPE, typename FILEWRITERTYPE>
void AbstractGitHub<AGENTTYPE, ACTIONTYPE, SCORETYPE, PACKAGETYPE, FILEWRITERTYPE>::api_createRepo(ACTIONTYPE action){
//	PACKAGETYPE newPackage(action);
	AGENTTYPE* newAgent = this->createAndAddNewAgent(action.getNewAgentPackage());
	action.target = newAgent;
	handleAction(action);
//	GitAgentPackage newPackage;
//	newPackage.type = REPO_AGENT;
//	newPackage.creatorID = action.actor->getId();
//	GitRepoAgent* newRepo = (GitRepoAgent*)createAndAddNewAgent(newPackage);
//	newRepo->informationIDs.insert(action.payload.informationIDs.begin(), action.payload.informationIDs.end());
//	action.target = newRepo;
//	getEdge(action.actor, action.target)->startOwn();
//	countEvent(CREATE_REPO);
//
//	if(directPropagation) propagate(action);
//	writeAction(action);
}

template<typename AGENTTYPE, typename ACTIONTYPE, typename SCORETYPE, typename PACKAGETYPE, typename FILEWRITERTYPE>
void AbstractGitHub<AGENTTYPE, ACTIONTYPE, SCORETYPE, PACKAGETYPE, FILEWRITERTYPE>::api_watchRepo(ACTIONTYPE action){
	action.target->receiveAction(action);
//	((GitRepoAgent*)(action.target))->informationIDs.insert(action.payload.informationIDs.begin(), action.payload.informationIDs.end());
//	getEdge(action.actor, action.target)->logEvent(WATCH_REPO);
//	countEvent(WATCH_REPO);
//	action.target->touch();
//
//	if(directPropagation) propagate(action);
//	writeAction(action);
}

template<typename AGENTTYPE, typename ACTIONTYPE, typename SCORETYPE, typename PACKAGETYPE, typename FILEWRITERTYPE>
void AbstractGitHub<AGENTTYPE, ACTIONTYPE, SCORETYPE, PACKAGETYPE, FILEWRITERTYPE>::api_followUser(ACTIONTYPE action){
//	getEdge(action.actor, action.target)->logEvent(FOLLOW_USER);
//	countEvent(FOLLOW_USER);
//	action.target->touch();
//
//	if(directPropagation) propagate(action);
//
//	writeAction(action);
}

template<typename AGENTTYPE, typename ACTIONTYPE, typename SCORETYPE, typename PACKAGETYPE, typename FILEWRITERTYPE>
void AbstractGitHub<AGENTTYPE, ACTIONTYPE, SCORETYPE, PACKAGETYPE, FILEWRITERTYPE>::api_fork(ACTIONTYPE action){
//	PACKAGETYPE newAgentDefinition(action);
	AGENTTYPE* newRepo = this->createAndAddNewAgent(action.getNewAgentPackage());
//	((GitRepoAgent*)(action.target))->informationIDs.insert(action.payload.informationIDs.begin(), action.payload.informationIDs.end());
//	// Register that the user 'forked' the original
//	getEdge(action.actor, action.target)->logEvent(FORK);
//	countEvent(FORK);
//	action.target->touch();
//
//	// Create a new node
//	GitAgentPackage newAgentDefinition;
//	newAgentDefinition.type = REPO_AGENT;
//	newAgentDefinition.whichCluster = 0;
//	newAgentDefinition.creatorID = action.actor->getId();
//	GitRepoAgent* newRepo = (GitRepoAgent*)createAndAddNewAgent(newAgentDefinition);
//	newRepo->informationIDs.insert(action.payload.informationIDs.begin(), action.payload.informationIDs.end());
//
//	// Designate a parent-child relationship between the newly created
//	// repo and the original:
//	getEdge(action.actor, newRepo)->logEvent(CREATE_REPO);
//	if(directPropagation) propagate(action);
//
//
//	// write all actions required by the output format
//
//	writeAction(action);
}

template<typename AGENTTYPE, typename ACTIONTYPE, typename SCORETYPE, typename PACKAGETYPE, typename FILEWRITERTYPE>
void AbstractGitHub<AGENTTYPE, ACTIONTYPE, SCORETYPE, PACKAGETYPE, FILEWRITERTYPE>::api_gollum(ACTIONTYPE action){
//	((GitRepoAgent*)(action.target))->informationIDs.insert(action.payload.informationIDs.begin(), action.payload.informationIDs.end());
//	getEdge(action.actor, action.target)->logEvent(GOLLUM);
//	countEvent(GOLLUM);
//	action.target->touch();
//
//	if(directPropagation) propagate(action);
//
//	writeAction(action);
}


template<typename AGENTTYPE, typename ACTIONTYPE, typename SCORETYPE, typename PACKAGETYPE, typename FILEWRITERTYPE>
void AbstractGitHub<AGENTTYPE, ACTIONTYPE, SCORETYPE, PACKAGETYPE, FILEWRITERTYPE>::api_commit(ACTIONTYPE action){
//	((GitRepoAgent*)(action.target))->informationIDs.insert(action.payload.informationIDs.begin(), action.payload.informationIDs.end());
//	getEdge(action.actor, action.target)->logEvent(COMMIT_COMMENT);
//	countEvent(COMMIT_COMMENT);
//	action.target->touch();
//
//	if(directPropagation) propagate(action);
//
//	writeAction(action);
}

template<typename AGENTTYPE, typename ACTIONTYPE, typename SCORETYPE, typename PACKAGETYPE, typename FILEWRITERTYPE>
void AbstractGitHub<AGENTTYPE, ACTIONTYPE, SCORETYPE, PACKAGETYPE, FILEWRITERTYPE>::api_pullRequest(ACTIONTYPE action){
//	((GitRepoAgent*)(action.target))->informationIDs.insert(action.payload.informationIDs.begin(), action.payload.informationIDs.end());
//	getEdge(action.actor, action.target)->logEvent(PULLREQUEST);
//	countEvent(PULLREQUEST);
//	action.target->touch();
//
//	if(directPropagation) propagate(action);
//
//	writeAction(action);
}

template<typename AGENTTYPE, typename ACTIONTYPE, typename SCORETYPE, typename PACKAGETYPE, typename FILEWRITERTYPE>
void AbstractGitHub<AGENTTYPE, ACTIONTYPE, SCORETYPE, PACKAGETYPE, FILEWRITERTYPE>::api_issueComment(ACTIONTYPE action){
//	((GitRepoAgent*)(action.target))->informationIDs.insert(action.payload.informationIDs.begin(), action.payload.informationIDs.end());
//	getEdge(action.actor, action.target)->logEvent(ISSUE_COMMENT);
//	countEvent(ISSUE_COMMENT);
//	action.target->touch();
//	if(directPropagation) propagate(action);
//
//	writeAction(action);
}

template<typename AGENTTYPE, typename ACTIONTYPE, typename SCORETYPE, typename PACKAGETYPE, typename FILEWRITERTYPE>
void AbstractGitHub<AGENTTYPE, ACTIONTYPE, SCORETYPE, PACKAGETYPE, FILEWRITERTYPE>::api_issue(ACTIONTYPE action){
//	((GitRepoAgent*)(action.target))->informationIDs.insert(action.payload.informationIDs.begin(), action.payload.informationIDs.end());
//	getEdge(action.actor, action.target)->logEvent(ISSUE);
//	countEvent(ISSUE);
//	action.target->touch();
//
//	if(directPropagation) propagate(action);
//
//	writeAction(action);
}

template<typename AGENTTYPE, typename ACTIONTYPE, typename SCORETYPE, typename PACKAGETYPE, typename FILEWRITERTYPE>
void AbstractGitHub<AGENTTYPE, ACTIONTYPE, SCORETYPE, PACKAGETYPE, FILEWRITERTYPE>::api_pullRequestReviewComment(ACTIONTYPE action){
//	((GitRepoAgent*)(action.target))->informationIDs.insert(action.payload.informationIDs.begin(), action.payload.informationIDs.end());
//	getEdge(action.actor, action.target)->logEvent(PULLREQUESTREVIEWCOMMENT);
//	countEvent(PULLREQUESTREVIEWCOMMENT);
//	action.target->touch();
//
//	if(directPropagation) propagate(action);
//
//	writeAction(action);
}

template<typename AGENTTYPE, typename ACTIONTYPE, typename SCORETYPE, typename PACKAGETYPE, typename FILEWRITERTYPE>
void AbstractGitHub<AGENTTYPE, ACTIONTYPE, SCORETYPE, PACKAGETYPE, FILEWRITERTYPE>::api_push(ACTIONTYPE action){
//	((GitRepoAgent*)(action.target))->informationIDs.insert(action.payload.informationIDs.begin(), action.payload.informationIDs.end());
//	getEdge(action.actor, action.target)->logEvent(PUSH);
//	countEvent(PUSH);
//	action.target->touch();
//
//	if(directPropagation) propagate(action);
//
//	writeAction(action);
}

template<typename AGENTTYPE, typename ACTIONTYPE, typename SCORETYPE, typename PACKAGETYPE, typename FILEWRITERTYPE>
void AbstractGitHub<AGENTTYPE, ACTIONTYPE, SCORETYPE, PACKAGETYPE, FILEWRITERTYPE>::api_release(ACTIONTYPE action){
//	((GitRepoAgent*)(action.target))->informationIDs.insert(action.payload.informationIDs.begin(), action.payload.informationIDs.end());
//	getEdge(action.actor, action.target)->logEvent(RELEASE);
//	countEvent(RELEASE);
//	action.target->touch();
//	if(directPropagation) propagate(action);
//
//	writeAction(action);
}

template<typename AGENTTYPE, typename ACTIONTYPE, typename SCORETYPE, typename PACKAGETYPE, typename FILEWRITERTYPE>
void AbstractGitHub<AGENTTYPE, ACTIONTYPE, SCORETYPE, PACKAGETYPE, FILEWRITERTYPE>::api_member(ACTIONTYPE action){
//	((GitRepoAgent*)(action.target))->informationIDs.insert(action.payload.informationIDs.begin(), action.payload.informationIDs.end());
//	getEdge(action.actor, action.target)->logEvent(MEMBER);
//	countEvent(MEMBER);
//	action.target->touch();
//	if(directPropagation) propagate(action);
//
//	writeAction(action);
}

template<typename AGENTTYPE, typename ACTIONTYPE, typename SCORETYPE, typename PACKAGETYPE, typename FILEWRITERTYPE>
void AbstractGitHub<AGENTTYPE, ACTIONTYPE, SCORETYPE, PACKAGETYPE, FILEWRITERTYPE>::api_public(ACTIONTYPE action){
//	((GitRepoAgent*)(action.target))->informationIDs.insert(action.payload.informationIDs.begin(), action.payload.informationIDs.end());
//	getEdge(action.actor, action.target)->logEvent(PUBLIC);
//	countEvent(PUBLIC);
//	action.target->touch();
//
//	if(directPropagation) propagate(action);
//
//	writeAction(action);
}



template<typename AGENTTYPE, typename ACTIONTYPE, typename SCORETYPE, typename PACKAGETYPE, typename FILEWRITERTYPE>
void AbstractGitHub<AGENTTYPE, ACTIONTYPE, SCORETYPE, PACKAGETYPE, FILEWRITERTYPE>::handleAction(ACTIONTYPE action){
//	this->getEdge(action.actor, action.target)->logEvent(action.type);
//	this->countEvent(action.type);
////	action.target->touch();
//	action.target->receiveAction(action);
//	if(this->directPropagation) this->propagate(action);
//	this->writeAction(action);
}





template<typename AGENTTYPE, typename ACTIONTYPE, typename SCORETYPE, typename PACKAGETYPE, typename FILEWRITERTYPE>
long AbstractGitHub<AGENTTYPE, ACTIONTYPE, SCORETYPE, PACKAGETYPE, FILEWRITERTYPE>::getTotalActionCount(AGENTTYPE* source){
	long ret = 0;
//	std::vector<GitAgent*> recipients;
//	agentNetwork->adjacent(source, recipients);
//	std::vector<GitAgent*>::iterator iter = recipients.begin();
//	while(iter != recipients.end()){
//		boost::shared_ptr<BasicGitHubEdge> edge1 = findEdge(source, (*iter));
//		if(edge1 != 0) ret += edge1->getTotalEvents();
//
//		boost::shared_ptr<BasicGitHubEdge> edge2 = findEdge((*iter), source);
//		if(edge2 != 0) ret += edge2->getTotalEvents();
//		iter++;
//	}
	return ret;
}


template<typename AGENTTYPE, typename ACTIONTYPE, typename SCORETYPE, typename PACKAGETYPE, typename FILEWRITERTYPE>
void AbstractGitHub<AGENTTYPE, ACTIONTYPE, SCORETYPE, PACKAGETYPE, FILEWRITERTYPE>::getSelector(AGENTTYPE* source, SimpleWeightedSelector& sws){
////	std::vector<GitAgent*> recipients;
////	agentNetwork->successors(source, recipients);
////	std::vector<GitAgent*>::iterator iter = recipients.begin();
////	// Make sure there is at least something to choose...
////	while(iter != recipients.end()){
////		boost::shared_ptr<BasicGitHubEdge> edge = findEdge(source, (*iter));
////		sws.adjust((int)CREATE_REPO, edge->numCreateRepos());
////		sws.adjust((int)WATCH_REPO, edge->numWatches());
////		sws.adjust((int)FORK, edge->numForks());
////		sws.adjust((int)GOLLUM, edge->numGollums());
////		sws.adjust((int)COMMIT_COMMENT, edge->numCommitComments());
////		sws.adjust((int)PULLREQUEST, edge->numPullRequests());
////		sws.adjust((int)ISSUE_COMMENT, edge->numIssueComments());
////		sws.adjust((int)ISSUE, edge->numIssues());
////		sws.adjust((int)PULLREQUESTREVIEWCOMMENT, edge->numPullRequestReviewComments());
////		sws.adjust((int)PUSH, edge->numPushes());
////		sws.adjust((int)RELEASE, edge->numReleases());
////		sws.adjust((int)MEMBER, edge->numMemberEvents());
////		sws.adjust((int)PUBLIC, edge->numPublicEvents());
////		sws.adjust((int)DELETE, edge->numDeleteEvents());
////		iter++;
////	}
////	// Bail if there are not links
////	if(sws.getSum() == 0){
////		sws.adjust((int)CREATE_REPO,  1);
////		sws.adjust((int)WATCH_REPO,  1);
////		sws.adjust((int)FORK,  1);
////		sws.adjust((int)GOLLUM,  1);
////		sws.adjust((int)COMMIT_COMMENT,  1);
////		sws.adjust((int)PULLREQUEST,  1);
////		sws.adjust((int)ISSUE_COMMENT,  1);
////		sws.adjust((int)ISSUE,  1);
////		sws.adjust((int)PULLREQUESTREVIEWCOMMENT,  1);
////		sws.adjust((int)PUSH,  1);
////		sws.adjust((int)RELEASE,  1);
////		sws.adjust((int)MEMBER,  1);
////		sws.adjust((int)PUBLIC,  1);
////		sws.adjust((int)DELETE,  1);
////	}
//
//	sws.clear();
//	for(int i = 0; i < grandTotalEvents->size(); i++){
//		sws.addValue(grandTotalEvents->at(i)); // Simple
//		//sws.addValue((int)( pow(((double)grandTotalEvents->at(i))/1.0),1.0)); // Allows scaling
//	}
}



} // End namespace github

#endif /* INCLUDE_GITHUB_PLATFORM_H */
