#ifndef FILESTORAGE_H
#define FILESTORAGE_H

//////////////////////////////////////////////////////////////////////////////////
// FileStorage.h - Support Storage of Filenames and their Paths                 //
// ver 1.0                                                                      //
// Language:       Visual C++ 2013                                              //
// Application:    CSE687 - Project 1 (File Catalogue), Spring 2015             //
// Platform:       HP G62, Windows 7 Home Premium, Visual Studio 2013           //
// Author:         Dhivya Narayanan, Master's- Computer Engineering,            //
//				   Syracuse University,                                         //
//                 dnnaraya@syr.edu                                             //
//////////////////////////////////////////////////////////////////////////////////
/*
Package Operations
==================
This package provides a single class: FileStorage.  The main purpose is to store
all the file names along with their paths.
-STL container std::map and std::set are used to store filenames and Pathnames.
-std::set contains all paths (No duplicates)
-std::map contains all files name as key and list as value which contains iterators to
paths in which the corresponding file are present. (No duplicates)
-Stores each file name only once and each path only once

Function Prologues - Public Interfaces
======================================
begin();		//returns iterator refering to the first element in the map container
end();			//returns iterator refering to element which would follow the last element
in the map container. It does not point to any element, and thus shall not be dereferenced.
find();			//searches the container for the given element and returns iterator to that element if present
size();			//returns size of the map container(no.of elements in the container)
save();			//stores the file name and path into set and map container
set_size();		//returns the size of the set container

Required Files:
================
FileStorage.h, FileStorage.cpp

Maintenance History:
=====================
Ver 1.0 - 26 Jan 2015
- first release
*/

#include<map>
#include<set>
#include<list>
#include<string>
#include<iostream>

class FileStorage
{
public:
	using file = std::string;
	using path = std::string;
	using pathRef = std::set<path>::iterator;
	using list_PRef = std::list<pathRef>;
	using iter_map = std::map<file, list_PRef>::iterator;
	using iter_list = std::list<pathRef>::iterator;

	iter_map begin() { return files_.begin(); }
	iter_map end() { return files_.end(); }
	iter_map find(file filenm) { return files_.find(filenm); }
	size_t size() { return files_.size(); }
	size_t set_size(){ return pathset_.size(); }
	void save(const std::string& filename, const std::string& pathname);

private:
	std::map<file, list_PRef>files_;			// stores filename and the list of references to paths in which the corresponding filename exists
	iter_map it_map;                            // iterator to the map
	std::set<path>pathset_;						// stores all paths
	pathRef iter;								// iterator to the set which contains paths

};


#endif