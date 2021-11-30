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
/* InfoID.h */
#ifndef INCLUDE_SOCIALNETWORK_TOOLKIT_INFOID_H
#define INCLUDE_SOCIALNETWORK_TOOLKIT_INFOID_H

#include <string>

namespace rhpc_smple {
class InfoID{

private:
	static int uniqueID;

public:
	std::string id;
	int         id_number;

	InfoID(std::string idVal);

	~InfoID();

	std::string toString();

};

// Note: Comparison for ordering assumes that string IDs can be the same
// for more than one InfoID, while comparison for equality tests if
// two IDs have the same value
inline bool operator <(const InfoID& l, const InfoID& r){
	return l.id_number < r.id_number;
}

// Equality
inline bool operator ==(const InfoID& l, const InfoID& r){
	return l.id == r.id;
}

// Direct comparison with a string
inline bool operator ==(const InfoID& id, const std::string str){
	return id.id == str;
}

// Works both ways...
inline bool operator ==(const std::string str, const InfoID& id){
	return id.id == str;
}


// Inequality
inline bool operator !=(const InfoID& l, const InfoID& r){
	return l.id != r.id;
}

// Direct comparison with a string
inline bool operator !=(const InfoID& id, const std::string str){
	return id.id != str;
}

// Works both ways...
inline bool operator !=(const std::string str, const InfoID& id){
	return id.id != str;
}


}  // End Namespace rhpc_smple

#endif /* INCLUDE_SOCIALNETWORK_TOOLKIT_INFOID_H */
