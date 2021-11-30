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
/* Xample_ReturnValues.h*/
#ifndef XAMPLE_RETURNVALUES_H
#define XAMPLE_RETURNVALUES_H

#define __SUCCESS__                                  0

#define PROPS_CONFIG_INVALID_LENGTH                 -1
#define TOO_FEW_ARGUMENTS_TO_MAIN                   -2
#define NO_START_TIME_PROPERTY                      -3

// TwitterScenario.cpp
#define TW_UNSPECIFIED_USER_TO_CONTENT_ISSUE     -1001
#define TW_USER_TO_CONTENT_FIRST_LINE_BLANK      -1002
#define TW_UNSPECIFIED_PRESIM_ISSUE              -1003
#define TW_PRESIM_FIRST_LINE_BLANK               -1004
#define TW_UNSPECIFIED_USER_REL_ISSUE            -1005
#define TW_USER_REL_FIRST_LINE_BLANK             -1006
#define TW_UNSPECIFIED_USER_INTERNALS_ISSUE      -1007
#define TW_USER_INTERNAL_FIRST_LINE_BLANK        -1008
#define TW_UNSPECIFIED_SEED_POST_ISSUE_A         -1009
#define TW_UNSPECIFIED_SEED_POST_ISSUE_B         -1010

#endif /* XAMPLE_RETURNVALUES_H */
