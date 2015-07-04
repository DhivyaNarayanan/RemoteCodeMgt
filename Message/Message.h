#ifndef MESSAGE_H
#define MESSAGE_H
///////////////////////////////////////////////////////////////////////////////////
// Message.h - Constructs messages to send and Interpret the received message    //
// Ver 1.0																		 //
// Language:       Visual C++ 2013                                               //
// Application:    CSE687 - Project 4 (Remote code management facility)          //
//					Spring 2015	   										         //
// Platform:       HP G62, Windows 7 Home Premium, Visual Studio 2013            //
// Author:         Dhivya Narayanan, Master's- Computer Engineering,             //
//				   Syracuse University,                                          //
//                 dnnaraya@syr.edu                                              //
///////////////////////////////////////////////////////////////////////////////////
/*
*  Package Operations:
*  -------------------
*  This package constructs the message header which has to send over socket
*  and provides the class for interpreting the received message
*
*  Required Files:
*  ---------------
*  Message.h, Message.cpp
*
*  Build Command:
*  --------------
*  devenv DemoProject4.sln /rebuild debug
*
*  Maintenance History:
*  --------------------
*  ver 1.0 -22 Apr 2015
*  - first release
*/

#include <string>
#include <vector>
//#include"../FileSystem/FileSystem.h"

class Attr
{
public:
	Attr(const std::string& name, const std::string& value);
	std::string name();
	std::string value();
	std::string toString();
	static Attr fromString(const std::string& src);
private:
	std::string name_;
	std::string value_;
};

//----< Constructs Message  >---------------------------------------

class Message
{
public:
	using iterator = std::vector<Attr>::iterator;

	Message(const std::string& command);
	void addAttrib(const Attr& attr);
	void addBody(const std::string& body);
	std::string makeHeader(size_t blocksize);
	std::string buildMsg(size_t blocksize);
	std::string command();
	Attr operator[](size_t i);
	iterator begin();
	iterator end();
	std::string body();
	std::string toString();
	//size_t FileSize(std::string filespec);
	std::string getHeader();
	std::string getCommand();
	std::string getsrcIP();
	size_t getsrcPort();
	std::vector<std::string> getdstIP();
	std::vector<size_t> getdstPort();
	std::string getFilePath();
	size_t getFileSize();
	size_t getBlocksize();
	std::vector<std::string> getFilePatt();

private:
	std::string command_;
	std::vector<Attr> attribs_;
	size_t numBytes;
	std::string body_;
	std::string header_;
	std::string message_;
};

//----< Interpret message  >---------------------------------------

class InterpretMsg
{
public:
	InterpretMsg(std::string &msg);
	std::string getCommand();
	std::string getsrcIP();
	size_t getsrcPort();
	std::string getdstIP();
	size_t getdstPort();
	std::string getFilePath();
	size_t getFileSize();
	size_t getBlocksize();
	std::string getHeader();
	std::string getMessage();
	std::string getBlock();
	int numThreads();
	std::vector<std::string> getFilePatt();
	std::string getText();
private:
	std::string message_;
	std::string header_;
	std::vector<Attr> attribs_;
	std::string body_;

};

#endif
