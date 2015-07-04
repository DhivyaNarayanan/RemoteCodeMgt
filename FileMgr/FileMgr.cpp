//////////////////////////////////////////////////////////////////////////////////
// FileMgr.cpp -   Defines FileMgr class and Implemented to Collect files       //
//					of a particular pattern in a Directory,                     //
//				   if needed recursively                                        //
// ver 1.0                                                                      //
// Language:       Visual C++ 2013                                              //
// Application:    CSE687 - Project 1 (File Catalogue), Spring 2015             //
// Platform:       HP G62, Windows 7 Home Premium, Visual Studio 2013           //
// Author:         Dhivya Narayanan, Master's- Computer Engineering,            //
//				   Syracuse University,                                         //
//                 dnnaraya@syr.edu                                             //
//////////////////////////////////////////////////////////////////////////////////
#include"FileMgr.h"
#include<iostream>

//------------------< adds pattern for the file search >--------------------------------------------------------

void FileMgr::addPattern(const std::string& patt)
{
	if (patterns_.size() == 1 && patterns_[0] == "*.*")
		patterns_.pop_back();
	patterns_.push_back(patt);
}

//------------------< search the given path for files>--------------------------------------------------------

void FileMgr::search(std::string&path)
{
	path_ = path;
	//std::string tmppath = "../Server";
	//std::string path = FileSystem::Path::fileSpec(tmppath, path_);
	if (recurse_ == true)
		find(path_);
	else                               //search for files only in the current directory
	{
		for (auto patt : patterns_)
		{
			
			std::vector<std::string> files = FileSystem::Directory::getFiles(path_, patt);
			for (auto f : files)
			{
				store_.save(f, path_);
			}
		}
	}
}

//------------------< get the files in the given path, if needed recursively>--------------------------------------------------------

void FileMgr::find(const std::string& path)
{
	//search for files recursively
	std::vector<std::string> dirs = FileSystem::Directory::getDirectories(path);
	for (auto dir : dirs)
	{
		if (dir != "." && dir != ".." && dirs.size()>2)
		{
			for (auto patt : patterns_)
			{
				std::vector<std::string> files = FileSystem::Directory::getFiles(path, patt);
				for (auto f : files)
				{
					store_.save(f, path);
				}
			}
			std::string t_path = FileSystem::Path::fileSpec(path, dir);
			find(t_path);
		}
		else if (dir == "." && dirs.size() == 2)    //checks for file in the directory which has no sub-directories
		{
			for (auto patt : patterns_)
			{
				std::vector<std::string> files = FileSystem::Directory::getFiles(path, patt);
				for (auto f : files)
				{
					store_.save(f, path);
				}
			}

		}
	}
}

//------------------< sets text search value for searching given text in all the files present in the catalog >--------------------------------------------------------


void FileMgr::setTextSearch(bool ts, std::string txt)
{
	textsearch_ = ts;
	text_ = txt;
}

//----< test stub >--------------------------------------------------------

#ifdef TEST_FILEMGR

int main()
{
	std::cout << "Testing FileMgr..\n";
	std::cout << "------------------------\n";
	FileStorage ds;
	FileMgr fm("G:/SPRING'15/OOD/Projects/FileCatalogue/FileMgr", ds);
	fm.addPattern("*.h");
	fm.addPattern("*.cpp");
	std::string path = "G:/SPRING'15/OOD/Projects/FileCatalogue/FileMgr";
	fm.search(path);

	for (auto fs : ds)
	{
		std::cout << "\n " << fs.first << "\t";
		FileStorage::iter_list it_list = fs.second.begin();
		while (it_list != fs.second.end())
		{
			std::cout << *(*(it_list)) << "\t";
			it_list++;
		}
		std::cout << "\n";
	}
	std::cout << "\n\n";

	return 0;
}

#endif