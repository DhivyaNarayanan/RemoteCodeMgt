#ifndef FILEMGR_H
#define FILEMGR_H
//////////////////////////////////////////////////////////////////////////////////
// FileMgr.h -     Collects files of a particular pattern in a Directory,       //
//				   if needed recursively                                        //
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
This package provides a single class: FileMgr.
-This class is intended to Provide support to collect all files of a particular set of patterns
in a Directory, if needed recursively.


Function Prologues - Public Interfaces
======================================
setRecurse();		//sets recursive search value for finding files within sub-directories
setDuplicate();		//sets duplicates display value to get only the duplicate file names and their paths
Isduplicate();		//returns true if duplicate display value is true, else returns false
isTextSearch();		//returns true if text search value is set to true, else returns false
addPattern();		//adds pattern for the file search
search();			//search the given path for files
find();				//get the files in the given path, if needed recursively
setTextSearch();    //sets text search value for searching given text in all the files present in the catalog
text_given();		// returns the text to be searched in the files present in the catalog

Required Files:
================
FileMgr.h, FileMgr.cpp, FileSystem.h, FileSystem.cpp, FileStorage.cpp

Maintenance History:
=====================
Ver 1.0 - 26 Jan 2015
- first release
*/

#include "../FileSystem/FileSystem.h"
#include "../FileStorage/FileStorage.h"
#include<vector>

class FileMgr
{
public:
	using iterator = FileStorage::iter_map;
	using patterns = std::vector<std::string>;

	FileMgr(const std::string& path, FileStorage& fs) : path_(path), store_(fs)		//Promotion Constructor
	{
		patterns_.push_back("*.*");
	}
	void setRecurse(bool rec){ recurse_ = rec; }
	void setDuplicate(bool dup){ duplicates_ = dup; }
	bool Isduplicate(){ return duplicates_; }
	bool IsRecurse(){ return recurse_; }
	bool isTextSearch(){ return textsearch_; }
	std::string text_given(){ return text_; }
	void addPattern(const std::string& patt);
	void search(std::string&path);
	void find(const std::string& path);
	void setTextSearch(bool ts, std::string txt);

private:
	std::string path_;								//stores the path from CmdlineParser
	FileStorage& store_;							//reference to class FileStorage
	patterns patterns_;								//stores patterns
	std::vector<std::string> text_in_files;			//stores files contains the search text
	bool recurse_ = true;							//holds recursive search value
	bool duplicates_ = false;						//holds duplicates display value
	bool textsearch_ = false;						//holds textsearch value
	std::string text_;								//holds the text to be searched in the files

};

#endif
