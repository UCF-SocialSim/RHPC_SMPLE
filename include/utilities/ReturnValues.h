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
/* ReturnValues.h */
#ifndef INCLUDE_RETURNVALUES_H
#define INCLUDE_RETURNVALUES_H

namespace rhpc_smple {

// Scenario.h
#define UNSPECIFIED_INIT_FILE_ISSUE_A            -1001
#define INIT_FILE_FIRST_LINE_BLANK_A             -1002
#define FILE_TIME_INTERVAL_MISMATCH				 -1003

// Scenario.cpp
#define UNSPECIFIED_SHOCK_FILE_ISSUE             -2001
#define SHOCK_FILE_FIRST_LINE_BLANK              -2002

// InfoStore.cpp
#define INFO_STORE_UNSPECIFIED_FILE_ISSUE        -7001

}  // End Namespace rhpc_smple

#endif /* INCLUDE_RETURNVALUES_H */
