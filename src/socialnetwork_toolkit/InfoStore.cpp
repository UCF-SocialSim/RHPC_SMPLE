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
/* InfoStore.cpp */
#include "socialnetwork_toolkit/InfoStore.h"

#include <string>

#include "boost/mpi.hpp"

#include "utilities/Utilities.h"

using namespace rhpc_smple;

InfoID& InfoStore::add(std::string infoID, long weight){
		int i = 0;
		boost::ptr_vector<InfoID>::iterator iter = infoIDs.begin();
		while((iter != infoIDs.end()) && (*iter != infoID)){
			i++;
			iter++;
		}
		if(iter == infoIDs.end()){
			infoIDs.push_back(new InfoID(infoID));
			sws->addValue(weight);
			return infoIDs.back();
		}
		else{
			sws->adjust(i, weight);
			return *iter;
		}
	}

	InfoID* InfoStore::find(std::string name){
		boost::ptr_vector<InfoID>::iterator iter = infoIDs.begin();
		while(iter != infoIDs.end()){
			if(*iter == name) return &*iter;
			iter++;
		}
		return 0;
	}

	void InfoStore::get(std::set<InfoID*>& IDs, int n){
		int targetSize = IDs.size() + n;
		int maxAttempts = n + 100;
		int attempts = 0;
		while(IDs.size() < n && attempts < maxAttempts){
			IDs.insert(&(infoIDs.at(sws->select())));
			attempts++;
		}
	}

	void InfoStore::getAll(std::set<std::string>& IDs){
		boost::ptr_vector<InfoID>::iterator iter = infoIDs.begin();
		while(iter != infoIDs.end()){
			IDs.insert(iter->id);
			iter++;
		}
	}

	void InfoStore::initializeFromFileList(std::string fullFilePathname, long weight){
		CORE_LOG_INFO("init", "Running infoStore.initializeFromFileList with file: " + fullFilePathname);
		if(fullFilePathname.length() == 0){
			CORE_LOG_WARN("init", "        No file specified- omitting.");
			return;
		}
		std::ifstream  initData(fullFilePathname);
		std::string line;
		if (!initData.good()){ 
			LOG_ERROR("init",  "EXITING: Problem (A) in initialization file: " + fullFilePathname);
			exit(INFO_STORE_UNSPECIFIED_FILE_ISSUE); 
			}

		// Advance to where the hash tags are for this Process
		int lineCount = 0;
		int count = infoIDs.size();
		bool fileValid = true;
		fileValid = (fileValid && std::getline(initData,line));
		int skipped   = 0;

		while(fileValid){
			fileValid = (fileValid && std::getline(initData,line)); // Will be 'true' until a blank line marking eof
			if(fileValid){
				lineCount++;
				std::vector<std::string> vals;
				boost::split(vals, line, boost::is_any_of(","));

				std::string id = vals[0];
				if(vals.size() == 1) add(id, weight);
				else {
					add(id,  atoi((vals[1]).c_str()));
					// Some lists will have comma-separated synonyms
					// If this is the case, a numeric column will have to provide a weight, even if that weight is one
					for(int i = 2; i < vals.size(); i++) synonyms[vals[i]] = id;
				}


			}

		}
		count = infoIDs.size() - count; // How many new ones were added

		CORE_LOG_INFO("init", "Info Store load from file '" + fullFilePathname + "' complete:");
		CORE_LOG_INFO("init", "   "  + std::to_string(lineCount) + " lines ( + 1 header)");
		CORE_LOG_INFO("init", "   "  + std::to_string(count)     + " new unique IDs ");
	}

	InfoID* InfoStore::pickAndVote(long weightToAdd){
		if(sws->size() == 0 || infoIDs.size() == 0) return 0;
		int index = sws->select();
		sws->adjust(index, weightToAdd);
		return &(infoIDs.at(index));
	}

	int InfoStore::size(){
		return infoIDs.size();
	}

	void InfoStore::clear(){
		sws->clear();
		infoIDs.clear();
		synonyms.clear();
	}

   void InfoStore::report(std::stringstream& ss){
		int i = 0;
		boost::ptr_vector<InfoID>::iterator iter = infoIDs.begin();
		ss << "infoID,weight\n";
		while(iter != infoIDs.end()){
			InfoID id = *iter;
			ss << id.toString() << "," << sws->at(i) << "\n";
			iter++;
			i++;
		} 
	}
