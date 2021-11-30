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
/* FileWriter.cpp */
#include "utilities/FileWriter.h"

using namespace rhpc_smple;

FileWriter::FileWriter(std::ofstream* sharedOutstream, bool startInPausedState):
        stream(sharedOutstream),
        responsibleForStream(false){
	// Constructor should set state to 'invalid' if the output stream cannot
	// be opened properly
	state = (startInPausedState ? FileWriter_State::paused : FileWriter_State::active);
}


FileWriter::FileWriter(std::string outfilepath, bool startInPausedState):
        responsibleForStream(true){
	// Constructor should set state to 'invalid' if the output stream cannot
	// be opened properly
	stream = new std::ofstream();
	stream->open(outfilepath, std::ios::trunc);  // erase any existing file
	stream->flush();
	state = (startInPausedState ? FileWriter_State::paused : FileWriter_State::active);
}

FileWriter::~FileWriter(){
	if(responsibleForStream){
		close();
		delete stream;
	}
}

FileWriter_State FileWriter::getState(){
	return state;
}

void FileWriter::pause(){
	state = FileWriter_State::paused;
}

void FileWriter::resume(){
	state = FileWriter_State::active;
}

bool FileWriter::write(std::string out){
	if(state == FileWriter_State::paused) return true;
	if(state == FileWriter_State::active){
		(*stream) << out;
		stream->flush();
		return true;
	}
	return false; // All other states besides 'active' and 'paused'
}

void FileWriter::close(){
	if(state != FileWriter_State::closed && state != FileWriter_State::invalid){
		stream->flush();
		stream->close();
		state = FileWriter_State::closed;
	} // Will leave in invalid state if that's where it started
}



