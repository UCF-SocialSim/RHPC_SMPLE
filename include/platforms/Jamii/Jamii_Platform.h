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
/* Jamii_Platform.h */
#ifndef INCLUDE_JAMII_PLATFORM_H
#define INCLUDE_JAMII_PLATFORM_H

#include "platforms/Jamii/Jamii_Definitions.h"

#include "socialnetwork_toolkit/SocialNetwork_Platform.h"
#include "socialnetwork_toolkit/BehaviorSelection.h"

#include "platforms/Jamii/Jamii_Definitions.h"
#include "platforms/Jamii/Jamii_Agent.h"
#include "platforms/Jamii/Jamii_Network.h"
#include "platforms/Jamii/Jamii_Feed_Query.h"

namespace jamii{


template<typename AGENTTYPE, typename ACTIONTYPE, typename SCORETYPE, typename PACKAGETYPE, typename FILEWRITERTYPE>
class AbstractJamii:
		public SocialNetwork_Platform<JamiiAgentType,
		                              AGENTTYPE,
									  JamiiEdge<AGENTTYPE, JamiiEdgeInfo>,
									  JamiiEdgeContent<AGENTTYPE, JamiiEdgeInfo>,
									  JamiiEdgeContentManager<AGENTTYPE, JamiiEdgeInfo>,
									  PACKAGETYPE,
									  ACTIONTYPE,
									  JamiiFeedQuery,
									  JamiiFeedExplanation,
									  FILEWRITERTYPE>{
protected:
	JamiiFeed<SCORETYPE>* currentFittest;


public:
	AbstractJamii(boost::mpi::communicator* world, ScenarioWithAgents<AGENTTYPE>* scenario);
	~AbstractJamii();

	virtual void init_schedule(repast::ScheduleRunner& runner, double offset = 0){}

	virtual boost::shared_ptr<JamiiFeed<SCORETYPE>> getFeed(AGENTTYPE* requestor, JamiiFeedQuery query);
	void addRandomPostsToFeed(JamiiFeed<SCORETYPE>& feed, int count);

	// API Functions
	void doAction(ACTIONTYPE action);
	void api_startThread(ACTIONTYPE action);
	void api_comment(ACTIONTYPE action);

	void handleAction(ACTIONTYPE action);

	// Agents' views into the platform:
	long getTotalActionCount(AGENTTYPE* source);

	void getSelector(AGENTTYPE* source, SimpleWeightedSelector& sws);

};

template<typename AGENTTYPE, typename ACTIONTYPE, typename SCORETYPE, typename PACKAGETYPE, typename FILEWRITERTYPE>
AbstractJamii<AGENTTYPE, ACTIONTYPE, SCORETYPE, PACKAGETYPE, FILEWRITERTYPE>::AbstractJamii(boost::mpi::communicator* world, ScenarioWithAgents<AGENTTYPE>* scenario):
      SocialNetwork_Platform<JamiiAgentType,
		                              AGENTTYPE,
									  JamiiEdge<AGENTTYPE, JamiiEdgeInfo>,
									  JamiiEdgeContent<AGENTTYPE, JamiiEdgeInfo>,
									  JamiiEdgeContentManager<AGENTTYPE, JamiiEdgeInfo>,
									  PACKAGETYPE,
									  ACTIONTYPE,
									  JamiiFeedQuery,
									  JamiiFeedExplanation,
									  FILEWRITERTYPE>(world, "jamii", "jm", 3, scenario){

}


template<typename AGENTTYPE, typename ACTIONTYPE, typename SCORETYPE, typename PACKAGETYPE, typename FILEWRITERTYPE>
AbstractJamii<AGENTTYPE, ACTIONTYPE, SCORETYPE, PACKAGETYPE, FILEWRITERTYPE>::~AbstractJamii(){}

template<typename AGENTTYPE, typename ACTIONTYPE, typename SCORETYPE, typename PACKAGETYPE, typename FILEWRITERTYPE>
boost::shared_ptr<JamiiFeed<SCORETYPE>> AbstractJamii<AGENTTYPE, ACTIONTYPE, SCORETYPE, PACKAGETYPE, FILEWRITERTYPE>::getFeed(AGENTTYPE* requestor, JamiiFeedQuery query){
	repast::Logger& logger = repast::Log4CL::instance()->get_logger("jamii");
	boost::shared_ptr<JamiiFeed<SCORETYPE>> ret(new JamiiFeed<SCORETYPE>());
	if(query.type == JamiiFeedQueryType::RETRIEVE_RANDOM_POSTS){
		addRandomPostsToFeed(*ret, query.countOfRandomPostsToRetrieve);
	}
	return ret;
}

template<typename AGENTTYPE, typename ACTIONTYPE, typename SCORETYPE, typename PACKAGETYPE, typename FILEWRITERTYPE>
void AbstractJamii<AGENTTYPE, ACTIONTYPE, SCORETYPE, PACKAGETYPE, FILEWRITERTYPE>::addRandomPostsToFeed(JamiiFeed<SCORETYPE>& feed, int count){
	typename std::vector<AGENTTYPE*> repos;
	this->context.selectAgents(count, repos, POST_AGENT);
	typename std::vector<AGENTTYPE*>::iterator iter = repos.begin();
	while(iter != repos.end()){
		JamiiFeedExplanation feedExplanation;
		feedExplanation.randomlySelected = true;
		feed.push(*iter, 0, 0, IDLE, feedExplanation); // Pushes in slot 1, as 'actor'
		iter++;
	}
}

template<typename AGENTTYPE, typename ACTIONTYPE, typename SCORETYPE, typename PACKAGETYPE, typename FILEWRITERTYPE>
void AbstractJamii<AGENTTYPE, ACTIONTYPE, SCORETYPE, PACKAGETYPE, FILEWRITERTYPE>::doAction(ACTIONTYPE action){
	JamiiEventType eventType = action.type;
	switch (eventType) {
	    case POST:          api_startThread(action);               break;
		case COMMENT:	    api_comment(action);			       break;
		default:{ }
	}
}

template<typename AGENTTYPE, typename ACTIONTYPE, typename SCORETYPE, typename PACKAGETYPE, typename FILEWRITERTYPE>
void AbstractJamii<AGENTTYPE, ACTIONTYPE, SCORETYPE, PACKAGETYPE, FILEWRITERTYPE>::api_startThread(ACTIONTYPE action){
	AGENTTYPE* newAgent = this->createAndAddNewAgent(action.getNewAgentPackage());
	action.target = newAgent;
	handleAction(action);
}

template<typename AGENTTYPE, typename ACTIONTYPE, typename SCORETYPE, typename PACKAGETYPE, typename FILEWRITERTYPE>
void AbstractJamii<AGENTTYPE, ACTIONTYPE, SCORETYPE, PACKAGETYPE, FILEWRITERTYPE>::api_comment(ACTIONTYPE action){
	action.target->receiveAction(action); // Notify the original target that there was a comment
	// Jamii just creates new events
	AGENTTYPE* newAgent = this->createAndAddNewAgent(action.getNewAgentPackage());
	action.target = newAgent;
	handleAction(action);
}


template<typename AGENTTYPE, typename ACTIONTYPE, typename SCORETYPE, typename PACKAGETYPE, typename FILEWRITERTYPE>
void AbstractJamii<AGENTTYPE, ACTIONTYPE, SCORETYPE, PACKAGETYPE, FILEWRITERTYPE>::handleAction(ACTIONTYPE action){
	this->getEdge(action.actor, action.target)->logEvent(action.type);
	this->countEvent(action.type);
////	action.target->touch();
	action.target->receiveAction(action);
//	if(this->directPropagation) this->propagate(action);
	this->writeAction(action);
}

template<typename AGENTTYPE, typename ACTIONTYPE, typename SCORETYPE, typename PACKAGETYPE, typename FILEWRITERTYPE>
long AbstractJamii<AGENTTYPE, ACTIONTYPE, SCORETYPE, PACKAGETYPE, FILEWRITERTYPE>::getTotalActionCount(AGENTTYPE* source){
	long ret = 0;
//	std::vector<JamiiAgent*> recipients;
//	agentNetwork->adjacent(source, recipients);
//	std::vector<JamiiAgent*>::iterator iter = recipients.begin();
//	while(iter != recipients.end()){
//		boost::shared_ptr<BasicJamiiEdge> edge1 = findEdge(source, (*iter));
//		if(edge1 != 0) ret += edge1->getTotalEvents();
//
//		boost::shared_ptr<BasicJamiiEdge> edge2 = findEdge((*iter), source);
//		if(edge2 != 0) ret += edge2->getTotalEvents();
//		iter++;
//	}
	return ret;
}


template<typename AGENTTYPE, typename ACTIONTYPE, typename SCORETYPE, typename PACKAGETYPE, typename FILEWRITERTYPE>
void AbstractJamii<AGENTTYPE, ACTIONTYPE, SCORETYPE, PACKAGETYPE, FILEWRITERTYPE>::getSelector(AGENTTYPE* source, SimpleWeightedSelector& sws){

}

} // End namespace jamii


#endif /* INCLUDE_JAMII_PLATFORM_H */
