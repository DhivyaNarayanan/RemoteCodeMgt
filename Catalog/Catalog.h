#ifndef CATALOG_H
#define CATALOG_H
//////////////////////////////////////////////////////////////////////////////////////
// Catalog.h -     Supports Text Search in the files present in the filestorage and //
//                 provides supporrt to store the file in the filestorage           //
//				                                                                    //
// ver 1.1                                                                          //
// Language:       Visual C++ 2013                                                  //
// Application:    CSE687 - Project 1 & 4 Spring 2015								//
// Platform:       HP G62, Windows 7 Home Premium, Visual Studio 2013               //
// Author:         Dhivya Narayanan, Master's- Computer Engineering,                //
//				   Syracuse University,                                             //
//                 dnnaraya@syr.edu                                                 //
//////////////////////////////////////////////////////////////////////////////////////
/*
Package Operations
==================
This package provides a single class: Catalog.
-This class is intended to Support Text Search in the files present in the filestorage and
provides supporrt to store the file in the filestorage


Function Prologues - Public Interfaces
======================================
TextSearch();			//search for particular text in the files stored in the filestorage
FindText_in_File();  	//search for given text in the files only with the given patterns stored in the filstorage
ConcurrentStringSearch(); //Do concurrent string using multiple threads
StringSearch();        // method handle by thread for text search

Required Files:
================
Catalog.h, Catalog.cpp, FileSystem.cpp

Build process:
===============
devenv DemoProject4.sln /rebuild debug

Maintenance History:
=====================
ver 1.1 - 24 Apr 2015
	Addded ConcurrentStringSearch functionalities
Ver 1.0 - 02 Feb 2015
- first release
*/

#include<iostream>
#include<fstream>
#include<set>
#include<vector>
#include<string>
#include <thread>
//#include <mutex>
//#include <chrono>
#include <Windows.h>
#include"../FileStorage/FileStorage.h"
#include"../FileMgr/FileMgr.h"
#include"../FileSystem/FileSystem.h"
//#include"../BlockingQueue/BlockingQueue.h"

//using FQueue = BlockingQueue<std::string>;
class Catalog
{
public:
	Catalog(FileStorage &fs) : store(fs){}					//constructor for named instance
	std::string TextSearch(std::string& text, std::string& file);
	std::vector<std::string>FindText_in_File(std::string& text, std::string& filepatt);
	std::vector<std::string>ConcurrentStringSearch(std::string& text,int no_of_threads);
	void StringSearch(std::string&text);


private:
	FileStorage& store;							// reference to class FileStorage 
	std::vector<std::string>TxtinFiles;
	//FQueue FileOpQ;
	//FQueue FileipQ;
};
#endif
