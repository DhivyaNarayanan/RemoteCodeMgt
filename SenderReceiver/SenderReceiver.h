#ifndef SENDERRECEIVER_H
#define SENDERRECEIVER_H

/////////////////////////////////////////////////////////////////////////////
// SenderReceiver.h -  Provides Sendr and Recvr class to send & receive   //
//                     message over socket                                //
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
*  This package uses Sockets and send and receive message over the socket
*  - pass string which includes header strings and buffer
*
*  Sendr class inherit from base class ISendr in MockChannel.h
*  Recvr class inherit from base class IRecvr in MockChannel.h
*  Required Files:
*  ---------------
*  BlockingQueue.h, BlockingQueue.cpp
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

#include"../Sockets/Sockets.h"
#include"../MockChannel/MockChannel.h"
#include"../Base64/Base64.h"

#include"../Message/Message.h"
#include"../ApplicationHelpers/AppHelpers.h"
#include"../BlockingQueue/BlockingQueue.h"
#include"../FileSystem/FileSystem.h"
#include"../FileMgr/FileMgr.h"
#include"../FileStorage/FileStorage.h"
#include<iostream>
#include<string>


using BQueue = BlockingQueue < Message >;
using bq = BlockingQueue<std::string>;

//----< Sendr class - sender >--------------------------------------------------

class Sendr : public ISendr
{
public:

	void postMessage(const Message& msg);     //GUI to sender
	void EnqMessage(const std::string& msg);   // sender to socket
	BQueue& queue();
	using byte = char;
	Sendr(){}									// constructor
	Sendr(BlockingQueue<std::string>&bq){}
	void sendFile(Socket& si, Message&msg);
	//void Send(Message&msg);						// method to send the message
	void Send();
	void sendAck(std::string sourceIp, size_t sourcePort, std::string destIp, size_t destPort);  //method to send acknwledgement
	void sendStop(Socket&si);					//method to send stop message
	void sendDownloadMsg(Socket&si, Message&msg);
	void sendFileSearchMsg(Socket&si, Message&msg);
	void sendTxtSearchMsg(Socket& si, Message&msg);

private:
	BQueue sendQ_;     //interface to GUI
	bq ipQ_;
};

//----< Recvr class - Receiver >--------------------------------------------------

class Recvr : public IRecvr
{
public:
	using byte = char;
	
	Recvr(size_t port) : sourcePort(port){}
	Recvr(size_t port, BlockingQueue<std::string>&bq){}
	std::string getMessage();
	std::string DeqMessage();
	bq& queue();
	void Recv();
	std::string getsrcIP();
	size_t getsrcPort();
	std::string getdestIp();
	size_t getdestPort();

private:
	bq recvQ_;
	bq  opQ_;
	size_t destport;
	std::string sourceIP;
	std::string destIP;
	size_t sourcePort;     // port no. of this receiver - equivalent to destPort in the message
};

////////////////////////////////////////////////////////////////////

//----< MockChannel >--------------------------------------------------

class MockChannel : public IMockChannel
{
public:
	MockChannel(ISendr* pSendr, IRecvr* pRecvr);
	void start();
	void stop();
private:
	std::thread thread_;
	ISendr* pISendr_;
	IRecvr* pIRecvr_;
	bool stop_ = false;
};


#endif

