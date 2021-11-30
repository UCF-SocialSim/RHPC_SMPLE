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
/* FixedOrderEventLog.h */
#ifndef FIXED_ORDER_EVENT_LOG_H
#define FIXED_ORDER_EVENT_LOG_H

namespace rhpc_smple {
    
/**
 * This data structure provides a map of columns needed by
 * the data load functions to the column position in
 * a csv file. The structure contains a set of default values,
 * e.g. the column specifying a user ID will be column zero,
 * the column specifying the activity time stamp will be
 * column 4, etc. To load a file with these columns in
 * different positions, initialize an instance of this
 * structure with different values. For example, suppose
 * in your file the position of the user ID column and the
 * time stamp column were reversed from their default
 * positions. You can create an instance and then modify these
 * two values, e.g.:
 *
 * FixedOrderEventLog customOrder;
 * customOrder.nodeUserIDColumn = 4;
 * customOrder.nodeTimeColumn = 0;
 *
 * The order can also be specified entirely in the constructor,
 * e.g.:
 *
 * FixedOrdeEventLog* customOrder = new FixedOrderEventLog(5, 1, 2, 3, 0);
 *
 * The remaining omitted arguments will use the defaults. However,
 * this construction is cryptic and not recommended.
 *
*/
class FixedOrderEventLog{

public:
   int nodeUserIDColumn;
   int nodeIDColumn;
   int parentIDColumn;
   int rootIDColumn;
   int nodeTimeColumn;
   int actionTypeColumn;
   int platformColumn;
   int communityIDColumn;
   int infoIDColumn;

   FixedOrderEventLog(
                        int nodeUserID = 0,
                        int nodeID     = 1,
                        int parentID   = 2,
                        int rootID     = 3,
                        int nodeTime   = 4,
                        int actionType = 5,
                        int platform   = 6,
                        int communityID= 7,
                        int infoID     = 8) :
         nodeUserIDColumn(nodeUserID),
         nodeIDColumn(nodeID),
         parentIDColumn(parentID),
         rootIDColumn(rootID),
         nodeTimeColumn(nodeTime),
         actionTypeColumn(actionType),
         platformColumn(platform),
         communityIDColumn(communityID),
         infoIDColumn(infoID){}
};

}  // End Namespace rhpc_smple

#endif
