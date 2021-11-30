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
/* Xample_Model.h */
#ifndef XAMPLE_MODEL_H
#define XAMPLE_MODEL_H

#include "boost/mpi.hpp"
#include "boost/date_time/posix_time/time_serialize.hpp"

#include "repast_hpc/Schedule.h"
#include "repast_hpc/Properties.h"
#include "repast_hpc/SharedContext.h"
#include "repast_hpc/AgentRequest.h"
#include "repast_hpc/TDataSource.h"
#include "repast_hpc/SVDataSet.h"
#include "repast_hpc/SharedNetwork.h"

#include "socialnetwork_toolkit/Scenario.h"
#include "utilities/Utilities.h"

using namespace rhpc_smple;

class Xample_Model{
private:
	std::vector<time_duration> simulationIntervals;
	ptime                      startTime;
	int                        index;
	int                        heartbeatInterval;

protected:
	Scenario* 			scenario;
	repast::Properties* props; 		    // This is the properties read in from model.props
	int   				timeInterval; 	// Number of seconds represented by one simulation tick
	ptime 				endTime;		// Simulation time at which the sim ends

    /*
     * Advances the 'clock' translating simulation ticks into calendar time
     */
    void incrementTime();

    /**
	 * Sends a message to the std out so you know that the application is working.
	 * Only outputs from Rank 0, and is scheduled at once every 15 ticks
	 */
	void heartBeat();

public:
	int rank;

	Xample_Model(repast::Properties* properties, int argc, char** argv, boost::mpi::communicator* comm);
	virtual ~Xample_Model();

    void init();
	void initSchedule(repast::ScheduleRunner& runner);

	void loadSimulationIntervals(std::string filename);
	int  getSimulationTimeInterval();

};


#endif /* XAMPLE_MODEL_H */
