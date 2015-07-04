//////////////////////////////////////////////////////////////////////////////////
// FileStorage.cpp - Implemented to Support Storage of Filenames and their Path //
// ver 1.0                                                                      //
// Language:       Visual C++ 2013                                              //
// Application:    CSE687 - Project 1 (File Catalogue), Spring 2015             //
// Platform:       HP G62, Windows 7 Home Premium, Visual Studio 2013           //
// Author:         Dhivya Narayanan, Master's- Computer Engineering,            //
//				   Syracuse University,                                         //
//                 dnnaraya@syr.edu                                             //
//////////////////////////////////////////////////////////////////////////////////

#include"FileStorage.h"
#include<iostream>

//----< stores the file name and path into set and map container >--------------------------------------------------------

void FileStorage::save(const std::string& filename, const std::string& pathname)
{
	std::pair<pathRef, bool> ret;
	ret = pathset_.insert(pathname);				//inserts pathname into set
	iter_map tmp_iter = files_.find(filename);
	if (tmp_iter != files_.end())
	{
		list_PRef tmp_list = tmp_iter->second;
		iter_list t_iter = tmp_list.begin();
		while (t_iter != tmp_list.end())
		{
			if (**t_iter == *ret.first)			  //if the file and corresponding path is already stored in the map, skip it.
				return;
			t_iter++;
		}
		tmp_list.push_back(ret.first);
		files_[filename] = tmp_list;			  //inserts file name and the list of path iterators into map
	}
	else
	{
		list_PRef tmp_list{ ret.first };
		files_[filename] = tmp_list;
	}
}

//----< test stub >--------------------------------------------------------

#ifdef TEST_FILESTORAGE

int main()
{
	std::cout << "Testing FileStorage\n";
	std::cout << "----------------------\n";
	FileStorage fs;
	fs.save("one", "onepath.h");
	fs.save("two", "twopath.h");
	fs.save("three", "threepath.h");
	fs.save("two", "firstpath.h");

	FileStorage::iter_map it = fs.begin();
	std::cout << "-------------------------\n";
	for (it = fs.begin(); it != fs.end(); ++it)
	{
		std::cout << it->first << "\t";
		FileStorage::iter_list it_list = it->second.begin();
		while (it_list != it->second.end())
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