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
/* Utilities.h */
#ifndef INCLUDE_SOCIALSIMUTILITIES_H
#define INCLUDE_SOCIALSIMUTILITIES_H

// PREPROCESSOR DIRECTIVES

// Setup logging

// These allow two bundles of logging, called 'core' and 'platform'
// Core logging is on by default, but can be turned off
// Platform logging is off by default, but can be turned on

#ifdef NOCORELOGGING
#define CORE_LOG_DEBUG(__loggername__, __text__ )
#define CORE_LOG_INFO(__loggername__, __text__ )
#define CORE_LOG_WARN(__loggername__, __text__ )
#else
#define CORE_LOG_DEBUG(__loggername__, __text__ ) repast::Log4CL::instance()->get_logger( __loggername__ ).log(repast::DEBUG , __text__ )
#define CORE_LOG_INFO(__loggername__, __text__ ) repast::Log4CL::instance()->get_logger( __loggername__ ).log(repast::INFO, __text__ )
#define CORE_LOG_WARN(__loggername__, __text__ ) repast::Log4CL::instance()->get_logger( __loggername__ ).log(repast::WARN, __text__ )
#endif

#ifdef PLATFORMLOGGING
#define LOG_DEBUG(__loggername__, __text__ ) repast::Log4CL::instance()->get_logger( __loggername__ ).log(repast::DEBUG , __text__ )
#define LOG_INFO(__loggername__, __text__ ) repast::Log4CL::instance()->get_logger( __loggername__ ).log(repast::INFO, __text__ )
#define LOG_WARN(__loggername__, __text__ ) repast::Log4CL::instance()->get_logger( __loggername__ ).log(repast::WARN, __text__ )
#else
#define LOG_DEBUG(__loggername__, __text__ )
#define LOG_INFO(__loggername__, __text__ )
#define LOG_WARN(__loggername__, __text__ )
#endif

// These are for fatal errors and are always logged
#define LOG_ERROR(__loggername__, __text__ ) repast::Log4CL::instance()->get_logger( __loggername__ ).log(repast::ERROR, __text__ )

// These allow you to turn off specific logging statements by adding "_CANCEL"
// The statements remain in the code but are not compiled, which may occasionally
// be preferable to merely re-setting the log level
#define CORE_LOG_DEBUG_CANCEL(__loggername__, __text__ )
#define CORE_LOG_INFO_CANCEL(__loggername__, __text__ )
#define CORE_LOG_WARN_CANCEL(__loggername__, __text__ )
#define LOG_DEBUG_CANCEL(__loggername__, __text__ )
#define LOG_INFO_CANCEL(__loggername__, __text__ )
#define LOG_WARN_CANCEL(__loggername__, __text__ )

// Begin utilities
#include <iostream>
#include <string>

#include "boost/lexical_cast.hpp"
#include "boost/date_time/posix_time/posix_time.hpp"

#include "repast_hpc/Properties.h"
#include "repast_hpc/logger.h"
#include "repast_hpc/Utilities.h"

using namespace boost::posix_time;

// See below for usage; when 'true' properties can be set using default
// values, but when set to false this capacity is disabled. This
// is typically used to ensure that the recorded properties in the
// metadata file are correct and are not changed in the running simulation
// after having been recorded.
extern bool okToUsePropertyDefault;

namespace rhpc_smple {

/**
 * Allows setting a property if it is not already in the
 * property collection.
 *
 * If the flag 'okToUsePropertyDefault' has been set to false,
 * the property will not be set.
 */
template<typename RETVAL>
void setPropertyDefault(std::string propName, RETVAL defaultVal, repast::Properties *props){
	if(!okToUsePropertyDefault){
		LOG_ERROR("general", "Attempted to set default property after metadata written.");
		exit(1);
	}
	if(props->getProperty(propName) == "")
			props->putProperty(propName, boost::lexical_cast<std::string>(defaultVal));
}

/**
 * Will allow specifying that a property should
 * be read, but if it is not in the properties collection
 * then a default value will be used and the property
 * added to the collection with that default value.
 *
 * If the flag 'okToUsePropertyDefault' has been set to false,
 * the property will not be set.
 */
template<typename RETVAL>
RETVAL getPropertyWithDefault(std::string propName, RETVAL defaultVal, repast::Properties *props){
	if(!okToUsePropertyDefault){
		LOG_ERROR("general", "Attempted to set default property after metadata written.");
		exit(1);
	}
	std::string propVal = props->getProperty(propName);
	if(propVal == ""){
		props->putProperty(propName, boost::lexical_cast<std::string>(defaultVal));
		return defaultVal;
	}
	else{
		return boost::lexical_cast<RETVAL>(propVal);
	}
}

/**
 * Given a URL, extract the domain name.
 */
std::string extractDomain(std::string url);

/**
 * Given a string (in the format"2021-01-02X00:00:00" where X
 * is any of "T", ".", ":", or "-") returns a Boost Posix Time
 * object.
 */
ptime parseTime(std::string timeString);

}  // End Namespace rhpc_smple

#endif /* INCLUDE_SOCIALSIMUTILITIES_H */
