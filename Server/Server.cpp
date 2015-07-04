/////////////////////////////////////////////////////////////////////////////
// Server.h - Initializes sender and receiver for server                   //
// Ver 1.0                                                                //
// Language:       Visual C++ 2013                                        //
// Application:    CSE687 - Project 3 (Message Passing Communivation),    //
//					Spring 2015	   										  //
// Platform:       HP G62, Windows 7 Home Premium, Visual Studio 2013     //
// Author:         Dhivya Narayanan, Master's- Computer Engineering,      //
//				   Syracuse University,                                   //
//                 dnnaraya@syr.edu                                       //
////////////////////////////////////////////////////////////////////////////
/*
*  Package Operations:
*  -------------------
*  This package uses Sender and Receiver class and initializes it which is used by server
*
*
*  Required Files:
*  ---------------
*  Server.h, Server.cpp, Sender.h, Sender.cpp, Receiver.h, Receiver.cpp
*  Sockets.h, Sockets.cpp, AppHelpers.h, AppHelpers.cpp
*
*  Build Command:
*  --------------
*  devenv DemoProject4.sln /rebuild debug
*
*  Maintenance History:
*  --------------------
*  ver 1.0 - 22 Apr 2015
*  - first release
*/
#include"../SenderReceiver/SenderReceiver.h"
#include"../Base64/Base64.h"
#include"../Sockets/Sockets.h"
#include"../Message/Message.h"
#include"../ApplicationHelpers/AppHelpers.h"
#include"../BlockingQueue/BlockingQueue.h"
#include"../FileSystem/FileSystem.h"
#include"../FileMgr/FileMgr.h"
#include"../FileStorage/FileStorage.h"
#include<iostream>
#include<string>


void InitializeServer1(size_t port)
{
	std::cout << "------------------ start of Server ---------------------\n";
	Sendr sr;
	Recvr rc(port);
	rc.Recv();
	std::string sourceIP = rc.getsrcIP();  //destIp in the message
	size_t sourcePort = rc.getsrcPort();   //destPort in the message - this port
	std::string destIP = rc.getdestIp();	//sourceIp in the message
	size_t destPort = rc.getdestPort();		//sourceport in the message
	sr.sendAck(sourceIP, sourcePort, destIP, destPort);		//send acknowledgement
	std::cout.flush();
	std::cin.get();
	std::cout << "-------------------- End of Server ----------------\n";

}

//----< test stub >------------------------------

#ifdef TEST_SERVER

int main(int argc, char* argv[])
{
	InitializeServer1(std::stoi(argv[1]));
	return 0;
}
#endif