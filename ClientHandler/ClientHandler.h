#ifndef CLIENTHANDLER_H
#define CLIENTHANDLER_H
/////////////////////////////////////////////////////////////////////////////
// ClientHandler.h - Handles Client Requests and process it               //
// Ver 1.1                                                                //
// Language:       Visual C++ 2013                                        //
// Application:    CSE687 - Project 4 (Remote Code management facility)   //
//					Spring 2015	   										  //
// Platform:       HP G62, Windows 7 Home Premium, Visual Studio 2013     //
// Author:         Dhivya Narayanan, Master's- Computer Engineering,      //
//				   Syracuse University,                                   //
//                 dnnaraya@syr.edu                                       //
////////////////////////////////////////////////////////////////////////////
/*
*  Package Operations:
*  -------------------
*  This package uses Sockets and receive message over the socket
*  - receives string which includes header strings and buffer
*
*  Required Files:
*  ---------------
*   BlockingQueue.h, BlockingQueue.cpp
*  Sockets.h, Sockets.cpp, AppHelpers.h, AppHelpers.cpp
*
*  Build Command:
*  --------------
*  devenv DemoProject4.sln /rebuild debug
*
*  Maintenance History:
*  --------------------
*  ver 1.1  - 22 Apr 2015
*     - Added functionalities to handle file download, file search, text search
*  ver 1.0  - 08 Apr 2015
*  - first release
*/

#include"../Base64/Base64.h"
#include"../Sockets/Sockets.h"
#include"../Message/Message.h"
#include"../ApplicationHelpers/AppHelpers.h"
#include"../BlockingQueue/BlockingQueue.h"
#include"../FileSystem/FileSystem.h"
#include"../FileStorage/FileStorage.h"
#include"../SenderReceiver/SenderReceiver.h"
#include"../FileMgr/FileMgr.h"
#include"../Catalog/Catalog.h"

#include<iostream>
#include<string>

//using BQueue = BlockingQueue<std::string>;

class ClientHandler
{
public:
	using byte = char;
	ClientHandler(BlockingQueue<std::string>&q) :bq(q){}
	void operator()(Socket& socket_);
	bool RecvFile(Socket& socket_);
	bool RecvAcknowledge(Socket& socket_);
	bool DownloadFile(Socket& socket_);
	bool FileSearch(Socket& socket_);
	bool TextSearch(Socket& socket_);
	//bool FindFileLoc(Socket& socket_);
	bool ConcStringSearch(Socket& socket_);
	void recvStop(Socket&socket_);
	std::string getdestIp();			//gets the sourceIp in the message
	size_t getdestPort();				//gets the sourceport in the message

private:
	BlockingQueue<std::string>&bq;
	size_t destport;
	std::string sourceIP;
	std::string destIP;
	size_t sourcePort;   // port no. of this receiver - equivalent to destPort in the message
};

#endif