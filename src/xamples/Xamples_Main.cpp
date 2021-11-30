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
/* Xamples_Main.cpp */

#include <algorithm>
#include <string>

#include <boost/mpi.hpp>

#include "boost/date_time/posix_time/posix_time.hpp"

#include "repast_hpc/RepastProcess.h"
#include "repast_hpc/initialize_random.h"
#include "repast_hpc/logger.h"

#include "xamples/models/Xample_Model.h"
#include "utilities/Utilities.h"

#include "xamples/utilities/Xample_ReturnValues.h"

using namespace boost::posix_time;
using namespace boost::gregorian;
using namespace rhpc_smple;
using namespace repast;

std::string runGUID;
std::string runFileNamePrefix;
std::string runMetadataTime;
std::string fullScenarioName;

extern bool okToUsePropertyDefault;

void usage() {
 	std::cerr << "usage: X  string string" << std::endl
	          << "  first string: string is the path to the Repast HPC configuration properties file" << std::endl
	          << "  second string: string is the path to the model properties file" << std::endl
	          << "example: mpirun -n 4 ./bin/Xamples_Main.exe ./props/config.props ./props/model.props" << std::endl;
}

void addEndTimeProperty(Properties* props){
	if(props->getProperty("end.time") != "") return; // May be invalid, but that's a different kind of problem... here we assume if there is a value we can use it
	std::string startTime = props->getProperty("start.time");
	ptime startPTime;
	if(startTime != ""){
		startPTime = parseTime(startTime);
	}else{
		boost::gregorian::date d(	strToInt(props->getProperty("start.year")),
									strToInt(props->getProperty("start.month")),
									strToInt(props->getProperty("start.day")));
		time_duration td(			strToInt(props->getProperty("start.hour")),
									strToInt(props->getProperty("start.minute")),
									strToInt(props->getProperty("start.second")), 0);
		startPTime = ptime(d, td);
	}


	std::string numDays = props->getProperty("number.of.days");
	if(numDays != ""){
		int dayCount = strToInt(numDays);
		// Need to add the 'end.time' property
		days nDays = days(dayCount);
		ptime endTime = startPTime + nDays;
		std::string etToPlace = to_iso_extended_string(endTime);
		std::replace( etToPlace.begin(), etToPlace.end(), ':', '.');
		std::replace( etToPlace.begin(), etToPlace.end(), 'T', '.');
		std::replace( etToPlace.begin(), etToPlace.end(), '-', '.');
		props->putProperty("end.time", etToPlace); // Needs to be in YYYY.MM.DD[.HH.MM.SS] format
	}
	else{
		std::string endTimeStr = props->getProperty("end.time");
		if(endTimeStr == ""){
			// Calculate how many ticks to run simulation
			boost::gregorian::date d = date(	strToInt(props->getProperty("end.year")),
					                        	strToInt(props->getProperty("end.month")),
												strToInt(props->getProperty("end.day")));
			time_duration td = time_duration(	strToInt(props->getProperty("end.hour")),
					                         	strToInt(props->getProperty("end.minute")),
												strToInt(props->getProperty("end.second")), 0);
			ptime endTime(d, td);
			// Need to add the 'end.time' property
			std::string etToPlace = to_iso_extended_string(endTime);
			std::replace( etToPlace.begin(), etToPlace.end(), ':', '.');
			std::replace( etToPlace.begin(), etToPlace.end(), 'T', '.');
			std::replace( etToPlace.begin(), etToPlace.end(), '-', '.');
			props->putProperty("end.time", etToPlace); // Needs to be in YYYY.MM.DD[.HH.MM.SS] format
		}
	}
}


/**
 * Initializes:
 *   runMetadatTime- a single time stamp that is used to consistently name all output files
 *   runGUID- an identifier that uniquely names output files
 *   runFileNamePrefix- a prefix prepended to the output file names, generally to put them in a path
 */
void initMetadata(Properties* props){
	struct timeval tmnow;
	gettimeofday(&tmnow, NULL);
	struct tm *tm;
	tm = localtime(&tmnow.tv_sec);

	char buf1[80], usec_buf[80], buf2[80];
	strftime(buf1,80,"%m-%d-%Y-%H-%M-%S", tm);
	strftime(buf2,30,"%m/%d/%Y %H:%M:%S", tm); // Note: not the same as buf1
	sprintf(usec_buf,"%d",(int)tmnow.tv_usec);

	// Construct a string representing the time run
	std::stringstream metadataTimeStream;
	metadataTimeStream << buf2 << ":" << usec_buf;
	runMetadataTime = metadataTimeStream.str();

    // Construct a GUID
    std::stringstream sguid;
    sguid << buf1 << "-" << usec_buf;
    sguid << "-" << props->getProperty("run.number");  // A number (integer) for this run, e.g. within a calibration run
    runGUID = sguid.str();

    // Construct a file name, including path (if appropriate)
    std::stringstream prfx;
    prfx << runGUID;
    runFileNamePrefix = prfx.str();
}


// Added comment in order to have something to commit...
int main(int argc, char** argv){
    	// MPI Init
	boost::mpi::environment  env(argc, argv);
	boost::mpi::communicator world;

	// Requires two explicit arguments- terminate immediately if this is not met
	if (argc < 3){
    	std::cerr << "usage: 3 arguments are required" << std::endl;
        if (world.rank() == 0)  usage();
        return TOO_FEW_ARGUMENTS_TO_MAIN;
    }

	// Parse arguments
	std::string configFileName = argv[1]; // The name of the configuration file is Arg 1
	std::string propsFileName  = argv[2]; // The name of the properties file is Arg 2

	// Check improper argument format
	if (configFileName.size() == 0 || propsFileName.size() == 0){
		std::cerr << "usage: strings for config file and props file must not be zero" << std::endl;
		if (world.rank() == 0)  usage();
		return PROPS_CONFIG_INVALID_LENGTH;
	}

   	// Set this flag to ensure that default properties can be properly used
    okToUsePropertyDefault = true;

    // Initialize RHPC
    RepastProcess::init(configFileName);

    // (Re-)initialize the loggers (Note: Any output above this must use std::cerr; loggers are not available)
    Log4CL::instance()->close(); // Just in case
    Properties* props = new Properties(propsFileName, argc, argv, &world);
    Log4CL::configure(getPropertyWithDefault<long>("run.number", 0, props), configFileName);

    // If the property for the sim end time is not present, need to add it for writing Metadata
    addEndTimeProperty(props);

    // Check if other required properties are present
    if(props->getProperty("run.number") == ""){
        LOG_INFO("init", "No run number provided; run number used is 0");
        props->putProperty("run.number","0");
    }
    // ... other required properties as needed

    // Create a time stamp that will be used as a prefix for all output files:
    initMetadata(props);

    // Initialize RHPC's random number generators
    initializeRandom(*props, &world);

    // Create the instance:
    Xample_Model* model = new Xample_Model(props, argc, argv, &world);

    // Initialize the model
    model->init();

    // Initialize the schedule
    model->initSchedule(RepastProcess::instance()->getScheduleRunner());

    // And run the model; this will run to completion
    RepastProcess::instance()->getScheduleRunner().run();

    // Finalize and end
    RepastProcess::instance()->done();

    // Keep a record of this run's properties; note that this is not done before model init in case props are changed
    std::vector<std::string> keysToWrite;
    keysToWrite.push_back("run.number");
    // ... other properties that you want to record in the csv output record go here
    if(world.rank() == 0) props->writeToSVFile("./output/record.csv", keysToWrite);

    // Cleanup
    delete model;
    delete props;

    return __SUCCESS__;
}



