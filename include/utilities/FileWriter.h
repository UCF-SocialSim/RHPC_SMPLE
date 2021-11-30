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
/* FileWriter.h */
#ifndef INCLUDE_FILEWRITER_H
#define INCLUDE_FILEWRITER_H

#include <iostream>
#include <fstream>

namespace rhpc_smple {

/*
 * Possible states for the FileWriter object
 */
enum FileWriter_State{
	active,
	paused,
	closed,
	invalid,
	FILEWRITER_STATE_META
};



/**
 * FileWriter class. Output writting can be
 * paused and resumed. Output file stream can
 * be possessed and managed by this object or
 * can be passed in constructor so that this
 * object shares it with others.
 */
class FileWriter{
private:
	std::ofstream*    stream;
	FileWriter_State  state;
	bool              responsibleForStream;

public:
	FileWriter(std::ofstream* sharedOutstream = 0, bool startInPausedState = false);
	FileWriter(std::string outfilepath, bool startInPausedState = false);
	virtual ~FileWriter();

	FileWriter_State getState();
	std::ofstream*   getStream();

	void pause(); 	// Suspends logging; when called for a write operation, does nothing
	void resume();  // Ends suspended logging; write calls produce output again

	// Write should only occur if state is 'active'
	// If state is 'paused', should return 'true',
	// but if state is other (closed, invalid), should
	// return 'false'
	// If state is active should return true if the write
	// was successful; false otherwise
	// if not successful, state should be set to
	// 'invalid'
	bool write(std::string out);

	// Closes the stream (if owned)
	void close();
};



/*
 * A generic object to write output (of a templated kind)
 * to a file.
 */
template<typename OUTPUT_TYPE>
class TypedFileWriter: public FileWriter{
	OUTPUT_TYPE       type;

public:
	TypedFileWriter(OUTPUT_TYPE t, std::ofstream* sharedOutstream = 0, bool startInPausedState = false);
	TypedFileWriter(OUTPUT_TYPE t, std::string outfilepath, bool startInPausedState = false);
	virtual ~TypedFileWriter();

	OUTPUT_TYPE getType();
};

template<typename OUTPUT_TYPE>
TypedFileWriter<OUTPUT_TYPE>::TypedFileWriter(OUTPUT_TYPE t, std::ofstream* sharedOutstream, bool startInPausedState):
        type(t),
        FileWriter(sharedOutstream, startInPausedState){
}


template<typename OUTPUT_TYPE>
TypedFileWriter<OUTPUT_TYPE>::TypedFileWriter(OUTPUT_TYPE t, std::string outfilepath, bool startInPausedState):
        type(t),
        FileWriter(outfilepath, startInPausedState){
}

template<typename OUTPUT_TYPE>
TypedFileWriter<OUTPUT_TYPE>::~TypedFileWriter(){
}

template<typename OUTPUT_TYPE>
OUTPUT_TYPE TypedFileWriter<OUTPUT_TYPE>::getType(){
	return type;
}

}  // End Namespace rhpc_smple

#endif /* INCLUDE_FILEWRITER_H */
