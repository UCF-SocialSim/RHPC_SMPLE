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
/* Utilities.cpp */
#include "utilities/Utilities.h"
#include <boost/algorithm/string.hpp>

#include "boost/date_time/gregorian/gregorian.hpp"

using namespace boost::posix_time;
using namespace boost::gregorian;

std::string rhpc_smple::extractDomain(std::string url){
	std::vector<std::string> result;
	boost::split(result, url, boost::is_any_of("/"));
	if(result.size() < 3) return "";
	return result[2];
}

ptime rhpc_smple::parseTime(std::string timeString){
	std::vector<std::string> vals;
	boost::split(vals, timeString, boost::is_any_of(".:T- "));
	boost::gregorian::date d(repast::strToInt(vals[0]), repast::strToInt(vals[1]), repast::strToInt(vals[2]));
	if(vals.size() > 3){
		time_duration td(repast::strToInt(vals[3]), repast::strToInt(vals[4]), repast::strToInt(vals[5]), 0);
		return ptime(d, td);
	}
	time_duration td(0, 0, 0, 0);
	return ptime(d, td);
}
