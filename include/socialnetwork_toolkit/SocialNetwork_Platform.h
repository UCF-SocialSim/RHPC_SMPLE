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
/* SocialNetwork_Platform.h */
#ifndef INCLUDE_SOCIALNETWORK_PLATFORM_H
#define INCLUDE_SOCIALNETWORK_PLATFORM_H

#include <iostream>
#include <fstream>
#include <algorithm>

#include "boost/date_time/posix_time/posix_time.hpp"

#include "repast_hpc/AgentId.h"
#include "repast_hpc/SharedContext.h"
#include "repast_hpc/SharedNetwork.h"
#include "repast_hpc/Properties.h"
#include "repast_hpc/logger.h"

#include "utilities/SimpleWeightedSelector.h"
#include "socialnetwork_toolkit/Scenario.h"
#include "socialnetwork_toolkit/SocialNetwork_AbstractElement.h"
#include "socialnetwork_toolkit/Feed.h"
#include "socialnetwork_toolkit/EdgeInformation.h"
#include "socialnetwork_toolkit/InfoID.h"

namespace rhpc_smple {

// Base class for all social network platforms
class SNP{
public:
	SNP(){}
	virtual ~SNP(){}
};


template<typename ATL , typename SCORETYPE>
class SocialNetwork_AbstractNode:public SocialNetwork_AbstractElement<ATL, SCORETYPE>{

public:
	repast::AgentId   id;	              // Repast ID
	std::string       hashTagID;	      // User ID from data
	std::string       canonicalName;
	SNP*              containingPlatform;

    bool              distanceEvent;      // Used when measuring distance in the network
    int               calculatedDistance; // This is the value of distance calculated
    int               passTracker;        // Used to track multiple calls to the DFS traversal algorithm
    ptime             dateCreated;        // Date/time that this node was created.

	SocialNetwork_AbstractNode(repast::AgentId id, ATL whichType, ptime dateElementCreated, SNP* container, SocialNetwork_AbstractElement<ATL, SCORETYPE>* contentCreator = 0);
	virtual ~SocialNetwork_AbstractNode();

    /** Setters **/
    virtual void setCurrentRank(int currentRank);
    virtual void setHashTagID(std::string hashTag);   // Note: also sets canonical name! To set both of these, first do this one, then do canonical name
    virtual void setCanonicalName(std::string cName); // Does not also set hashTag.

    repast::AgentId & getId()             {      return id;    }
    const repast::AgentId & getId() const {      return id;    }

    virtual std::string getHashTagID();
    virtual std::string showName() = 0;
    virtual std::string writeName() = 0;
    //virtual void resyncNetworkTotals(); // Loop through all network connections, which may be newly updated from other processes, and update local variable values

	virtual void setPlatform(SNP* platform);
};

template<typename ATL, typename SCORETYPE>
SocialNetwork_AbstractNode<ATL, SCORETYPE>::SocialNetwork_AbstractNode(repast::AgentId id, ATL whichType, ptime dateElementCreated, SNP* container, SocialNetwork_AbstractElement<ATL, SCORETYPE>* contentCreator):
  SocialNetwork_AbstractElement<ATL, SCORETYPE>(whichType, contentCreator), id(id), distanceEvent(false), calculatedDistance(INT_MAX), passTracker(0), containingPlatform(container), dateCreated(dateElementCreated){ }


template<typename ATL, typename SCORETYPE>
SocialNetwork_AbstractNode<ATL, SCORETYPE>::~SocialNetwork_AbstractNode(){ }

template<typename ATL, typename SCORETYPE>
void SocialNetwork_AbstractNode<ATL, SCORETYPE>::setCurrentRank(int currentRank){
	id.currentRank(currentRank);
}

template<typename ATL, typename SCORETYPE>
void SocialNetwork_AbstractNode<ATL, SCORETYPE>::setHashTagID(std::string hashTag) {
	hashTagID = hashTag;
	canonicalName = hashTagID;
}

template<typename ATL, typename SCORETYPE>
void SocialNetwork_AbstractNode<ATL, SCORETYPE>::setCanonicalName(std::string cName) {
	canonicalName = cName;
}

template<typename ATL, typename SCORETYPE>
std::string SocialNetwork_AbstractNode<ATL, SCORETYPE>::getHashTagID() {
	return hashTagID;
}

template<typename ATL, typename SCORETYPE>
void SocialNetwork_AbstractNode<ATL, SCORETYPE>::setPlatform(SNP* platform){
	containingPlatform = platform;
}





template<typename A, typename ACT, typename FE, typename FQ>
class AbstractSocialNetworkPlatform: public SNP{


public:
	repast::SharedContext<A> context;

	AbstractSocialNetworkPlatform(boost::mpi::communicator* world): SNP(), context(world){}
	virtual ~AbstractSocialNetworkPlatform(){}

	virtual boost::shared_ptr<Feed<A, ACT, FE> > getFeed(A requestor, FQ query){ return 0; }

};



// Templates:
//    ATL =  Agent Type List
//    A   =  Common parent of both user and content nodes
//    E   =  Edge
//    EC  =  EdgeContent
//    ECM =  EdgeContentManager
//    AP  =  AgentPackage
//    ACT =  Action
//    FQ  =  Feed Query
//    FE  =  Feed Explanation
//    FW  =  FileWriter
template<typename ATL, typename A, typename E, typename EC, typename ECM, typename AP, typename ACT, typename FQ, typename FE, typename FW>
class SocialNetwork_Platform: public AbstractSocialNetworkPlatform<A, ACT, FE, FQ>{
public:
	repast::SharedNetwork<A, E, EC, ECM>* agentNetwork;
	ECM                                   edgeContentManager;

	int 	                              nextAgentIndex;
	int                                   rank;
	bool                                  writeJSONEventFile;
	bool                                  writeJSONEventFile_cached;
	bool 								  directPropagation;

	std::ofstream*                        eventJSONOutputFile_ptr;
	std::ofstream*                        eventJSONOutputFile_GLOBAL_ptr;

	SimpleWeightedSelector*               grandTotalEvents;

	ScenarioWithAgents<A>*                enclosingScenario;

	std::string                           outputFilePathAndPrefix;

	std::string                           platformName;
	std::string                           platformAbbreviation;

	std::map<std::string, FW*>            fileWriters;


public:

	SocialNetwork_Platform(boost::mpi::communicator* world, std::string name, std::string abbrev, int countOfEventTypes, ScenarioWithAgents<A>* scenario = 0);
	virtual ~SocialNetwork_Platform();

	// Required- Do not modify
	A*   createAgent(AP package);

	// Agent creation and parallelization; pure virtual
	// MUST be extended by child implementation
	virtual AP   getPackage(A* agent)                                           = 0;
	virtual void updateAgent(AP package)                                        = 0;
	virtual void updateAgent(AP package, A* agent)                              = 0;
	virtual void provideContent(repast::AgentRequest req, std::vector<AP>& out) = 0;
	virtual void providePackage(A* agent, std::vector<AP>& out)                 = 0;

	void init();
	void initSchedule(repast::ScheduleRunner& runner, double offset = 0); // 'Offset' allows setting this as one component of a time step

	virtual void init_schedule(repast::ScheduleRunner& runner, double offset = 0) = 0;
	void initializeOutputFiles(std::string prefix, std::string scenarioName, repast::Properties* props);

	virtual void setDirectPropagation(bool newVal) { directPropagation = newVal; }
	// Methods whereby the model creates new agents when needed;
	// By making this a function of the model, the model can set
	// attributes of the agents easily. Note that in this
	// parent class, the agentType argument is ignored; the
	// SocialSimModel class only creates with USER_AGENT or REPO_AGENT
	// types
	//
	// Note the difference between these two kinds of create methods:
	//    createNewXXXXAgent ONLY returns an instance of the agent, with
	//       minimal construction; the agent is NOT necessarily in a valid
	//       state: it may not have local pointers initialized (call set_local_pointers)
	//       and it may not have all values set the way you will need them to
	//       be set (typically, you call this, set values, and then set the local
	//       pointers based on the values). These are used in the internal RHPC
	//       synchronization calls. The new agent is NOT yet added to the context.
	//  createAndAddNewXXXXAgent creates an instance and may populate it with
	//       values, and calls set_local_pointers, and adds it to the context.
	//       You may wish to change the values and re-set the local pointers,
	//       but implementations are free to establish defaults that make this
	//       unnecessary
	virtual A* createNewAgent(repast::AgentId id, AP agentPackage) = 0;
	virtual A* createAndAddNewAgent(AP agentPackage) = 0;

	/**
	 * This method is mainly driven by parallelization. When an
	 * agent arrives it can have data that indicate how it should
	 * be integrated with the process to which it is arriving, but
	 * it needs this integration to happen. For example, if an agent
	 * has a pointer to a function on Process 1 and moves to Process 2,
	 * it will bring a piece of data that indicates which function it
	 * needs, but it won't bring the actual pointer (because this
	 * could be different on the new process- and likely will). After
	 * it is unpacked, this method will check the agent's data, find
	 * a marker indicating which function, and link it to the corresponding
	 * function on this process. The method, however, can also be used
	 * for newly created agents on this process.
	 */
	void set_local_pointers(A* agent);

	virtual void setLocalPointers(A* agent) = 0;


	// Get Edge: If it does not exist, create one
	boost::shared_ptr<E> getEdge(A* source, A* target);

	// Find Edge: If it does not exist, return null
	boost::shared_ptr<E> findEdge(A* source, A* target);


	// InfoID Retrieval
	virtual InfoID* getPopularInfoID(long weightToAdd){  return enclosingScenario->getPopularInfoID(weightToAdd); }
	virtual InfoID* getShockedInfoID(){                  return enclosingScenario->getShockedInfoID(); }
	virtual std::vector<InfoID*> getEventCountInfoIDs(){ return enclosingScenario->getEventCountInfoIDs(); }
	virtual InfoID* find(std::string newInfoIDName){     return enclosingScenario->find(newInfoIDName); }

	void writeAction(ACT action){
		typename std::map<std::string, FW*>::iterator iter = fileWriters.begin();
		while(iter != fileWriters.end()){
			(*iter).second->write(action.getOutputRepresentation((*iter).second->getType()));
			iter++;
		}
	}

	virtual std::string getPlatformName(){         return platformName; }
	virtual std::string getPlatformAbbreviation(){ return platformAbbreviation; }

	void countEvent(int event);

	void propagate(ACT action, ATL recipientType);

	void setGlobalJSONPtr(std::ofstream* ptr){
		eventJSONOutputFile_GLOBAL_ptr = ptr;
	}


	void pauseJSONLogging(){        writeJSONEventFile         = false; }
	void pauseLogging(){
		pauseJSONLogging();
	}

	void resumeJSONLogging(){       writeJSONEventFile         = writeJSONEventFile_cached; }
	void resumeLogging(){
		resumeJSONLogging();
	}

	void startJSONLogging(){
		writeJSONEventFile = true; // Note: This is used in rare instances where you need to turn on global logging even though individual event logging may be off
	}

};


template<typename ATL, typename A, typename E, typename EC, typename ECM, typename AP, typename ACT, typename FQ, typename FE, typename FW>
SocialNetwork_Platform<ATL, A, E, EC, ECM, AP, ACT, FQ, FE, FW>::SocialNetwork_Platform(boost::mpi::communicator* world, std::string name, std::string abbrev, int countOfEventTypes, ScenarioWithAgents<A>* scenario): AbstractSocialNetworkPlatform<A , ACT, FE, FQ>(world), edgeContentManager(),
        enclosingScenario(scenario), nextAgentIndex(0), rank(repast::RepastProcess::instance()->rank()), writeJSONEventFile(true),
		grandTotalEvents(0), eventJSONOutputFile_ptr(0), eventJSONOutputFile_GLOBAL_ptr(0), directPropagation(false),
		outputFilePathAndPrefix("./"), platformName(name), platformAbbreviation(abbrev){

	grandTotalEvents = new SimpleWeightedSelector(0);
	for(int i = 0; i < countOfEventTypes; i++) grandTotalEvents->addValue(0);


	// Construct the agent network
	agentNetwork = new repast::SharedNetwork<A, E, EC, ECM>("agentNetwork", true, &edgeContentManager);
	AbstractSocialNetworkPlatform<A, ACT, FE, FQ>::context.addProjection(agentNetwork);

	eventJSONOutputFile_ptr        = new std::ofstream();
}




template<typename ATL, typename A, typename E, typename EC, typename ECM, typename AP, typename ACT, typename FQ, typename FE, typename FW>
SocialNetwork_Platform<ATL, A, E, EC, ECM, AP, ACT, FQ, FE, FW>::~SocialNetwork_Platform(){

	std::ofstream& eventJSONOutputFile         = *eventJSONOutputFile_ptr;

	// Note: Close this if it was ever opened, even if it was paused
	if(writeJSONEventFile_cached){
		// Close the output file
        eventJSONOutputFile.flush();
		eventJSONOutputFile.close();
	}


	delete grandTotalEvents;
	delete eventJSONOutputFile_ptr;
}

template<typename ATL, typename A, typename E, typename EC, typename ECM, typename AP, typename ACT, typename FQ, typename FE, typename FW>
void SocialNetwork_Platform<ATL, A, E, EC, ECM, AP, ACT, FQ, FE, FW>::init(){ }

template<typename ATL, typename A, typename E, typename EC, typename ECM, typename AP, typename ACT, typename FQ, typename FE, typename FW>
void SocialNetwork_Platform<ATL, A, E, EC, ECM, AP, ACT, FQ, FE, FW>::initSchedule(repast::ScheduleRunner& runner, double offset){
	init_schedule(runner, offset);
}

template<typename ATL, typename A, typename E, typename EC, typename ECM, typename AP, typename ACT, typename FQ, typename FE, typename FW>
void SocialNetwork_Platform<ATL, A, E, EC, ECM, AP, ACT, FQ, FE, FW>::initializeOutputFiles(std::string prefix, std::string scenarioName, repast::Properties* props){
	LOG_INFO("init", "<SocialNetworkPlatform>: Initializing Output Files for SocialNetwork_Platform");
	std::ofstream& eventJSONOutputFile  = *eventJSONOutputFile_ptr;
	writeJSONEventFile                  = (props->getProperty("write.json.log") != "false");
	writeJSONEventFile_cached           = writeJSONEventFile;


	std::stringstream sst;
	sst << "output/" << prefix << "-" << getPlatformAbbreviation() << "-" << scenarioName;
	outputFilePathAndPrefix = sst.str();

	if(writeJSONEventFile){
		std::stringstream JSONfilename;
		JSONfilename << outputFilePathAndPrefix << ".json";
		eventJSONOutputFile.open(JSONfilename.str(), ios::trunc);  // erase any existing file
		eventJSONOutputFile.flush();
	}
	
}


template<typename ATL, typename A, typename E, typename EC, typename ECM, typename AP, typename ACT, typename FQ, typename FE, typename FW>
A* SocialNetwork_Platform<ATL, A, E, EC, ECM, AP, ACT, FQ, FE, FW>::createAgent(AP package){
	repast::AgentId id(package.id, package.rank, package.type, package.currentRank);
    A* agent = createNewAgent(id, package.type);
    if(agent) updateAgent(package, agent);
    return agent;
}


template<typename ATL, typename A, typename E, typename EC, typename ECM, typename AP, typename ACT, typename FQ, typename FE, typename FW>
void SocialNetwork_Platform<ATL, A, E, EC, ECM, AP, ACT, FQ, FE, FW>::set_local_pointers(A* agent){
	setLocalPointers(agent);
	if(enclosingScenario != 0) enclosingScenario->set_local_pointers(agent);
}


template<typename ATL, typename A, typename E, typename EC, typename ECM, typename AP, typename ACT, typename FQ, typename FE, typename FW>
boost::shared_ptr<E> SocialNetwork_Platform<ATL, A, E, EC, ECM, AP, ACT, FQ, FE, FW>::getEdge(A* source, A* target){
    boost::shared_ptr<E> ret = findEdge(source, target);
    if(ret != 0) return ret;
    boost::shared_ptr<E> newEdge(new E(source, target));
    agentNetwork->addEdge(newEdge);
    return newEdge;
}

template<typename ATL, typename A, typename E, typename EC, typename ECM, typename AP, typename ACT, typename FQ, typename FE, typename FW>
boost::shared_ptr<E> SocialNetwork_Platform<ATL, A, E, EC, ECM, AP, ACT, FQ, FE, FW>::findEdge(A* source, A* target){
	return agentNetwork->findEdge(source, target);
}

template<typename ATL, typename A, typename E, typename EC, typename ECM, typename AP, typename ACT, typename FQ, typename FE, typename FW>
void SocialNetwork_Platform<ATL, A, E, EC, ECM, AP, ACT, FQ, FE, FW>::countEvent(int event){
	grandTotalEvents->adjust(event, 1);
}

template<typename ATL, typename A, typename E, typename EC, typename ECM, typename AP, typename ACT, typename FQ, typename FE, typename FW>
void SocialNetwork_Platform<ATL, A, E, EC, ECM, AP, ACT, FQ, FE, FW>::propagate(ACT action, ATL recipientType){
	vector<A*> recipients;
	agentNetwork->successors(action.actor, recipients);
	typename vector<A*>::iterator iter = recipients.begin();
	while(iter != recipients.end()){
		FE fe;
		if((*iter)->getType() == recipientType) {
            (*iter)->placeInFeed(action.actor, action.target, action.subtarget, action.type, fe);
        }
		iter++;
	}
}


// All agents should descend from this one, regardless of platform or type
// Typename ATL = Agent Type List
// Typename T = Action Type
// Typename SCORETYPE = the kind of scores this node can generate
template<typename ATL, typename T, typename SCORETYPE>
class SocialNetworkNode: public SocialNetwork_AbstractNode<ATL, SCORETYPE>{

public:
	// Things that cannot be ported across processes:
	std::vector<T> actions;

	SocialNetworkNode(repast::AgentId id, ATL whichType, ptime dateCreated, SNP* container, SocialNetwork_AbstractElement<ATL, SCORETYPE>* contentCreator = 0);
	virtual ~SocialNetworkNode();

	virtual void doSelectBehavior(){}
	virtual void selectBehavior(){}
	virtual void receiveAction(T& action){}

};


template<typename ATL, typename T, typename SCORETYPE>
SocialNetworkNode<ATL, T, SCORETYPE>::SocialNetworkNode(repast::AgentId id, ATL whichType, ptime dateCreated, SNP* container, SocialNetwork_AbstractElement<ATL, SCORETYPE>* contentCreator):
  SocialNetwork_AbstractNode<ATL, SCORETYPE>(id, whichType, dateCreated, container, contentCreator), actions(){}

template<typename ATL, typename T, typename SCORETYPE>
SocialNetworkNode<ATL, T, SCORETYPE>::~SocialNetworkNode(){}

}  // End Namespace rhpc_smple

#endif /* INCLUDE_SOCIALNETWORK_PLATFORM_H */
