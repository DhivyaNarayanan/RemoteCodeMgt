///////////////////////////////////////////////////////////////////////////////////
// Message.cpp - Constructs messages to send and Interpret the received message  //
// Ver 1.0																		 //
// Language:       Visual C++ 2013                                               //
// Application:    CSE687 - Project 4 (Remote code management facility)          //
//					Spring 2015	   										         //
// Platform:       HP G62, Windows 7 Home Premium, Visual Studio 2013            //
// Author:         Dhivya Narayanan, Master's- Computer Engineering,             //
//				   Syracuse University,                                          //
//                 dnnaraya@syr.edu                                              //
///////////////////////////////////////////////////////////////////////////////////

#include "Message.h"
#include <sstream>
#include <iostream>
#include <algorithm>


Attr::Attr(const std::string& name, const std::string& value) : name_(name), value_(value) {}

std::string Attr::name() { return name_; }

std::string Attr::value() { return value_; }

std::string Attr::toString()
{
	std::ostringstream temp;
	temp << name_ << " : " << value_;
	return temp.str();
}

Attr Attr::fromString(const std::string& src)
{
	std::string name, value;
	std::string::const_iterator iter = std::find(src.begin(), src.end(), ':');
	if (iter != std::end(src))
	{
		name = std::string(std::begin(src), --iter);
		value = std::string(iter + 3, std::end(src));
	}
	return Attr(name, value);
}

//----< Message class - constructor  >---------------------------------------

Message::Message(const std::string& command) : command_(command) {}

//----< Add attributes in message >---------------------------------------

void Message::addAttrib(const Attr& attr)
{
	attribs_.push_back(attr);
}

//----< Add body to message >---------------------------------------

void Message::addBody(const std::string& body)
{
	body_ = body;
}

//----< returns command in message >---------------------------------------

std::string Message::command()
{
	return command_;
}

Attr Message::operator[](size_t i)
{
	return attribs_[i];
}

Message::iterator Message::begin() { return attribs_.begin(); }

Message::iterator Message::end() { return attribs_.end(); }

std::string Message::body() { return body_; }

std::string Message::toString()
{
	std::ostringstream out;
	out << command_ << "\n";
	for (auto item : attribs_)
		out << item.toString() << "\n";
	out << "\n" << body_;
	return out.str();
}

//----< constructs header  message >---------------------------------------

std::string Message::makeHeader(size_t blocksize)
{
	numBytes = blocksize;

	addAttrib(Attr("CONTENTLENGTH", std::to_string(blocksize)));
	for (auto item : attribs_)
	{
		header_ += item.toString();
		header_ += '\n';
	}
	return header_;
}

//----<returns header message >---------------------------------------

std::string Message::getHeader()
{
	return header_;
}
//----< constructs  message >---------------------------------------

std::string Message::buildMsg(size_t blocksize)
{
	message_ = makeHeader(blocksize);
	message_ += '\n';
	message_ += body();

	return message_;
}

//----< Get command from message >---------------------------------------

std::string Message::getCommand()
{
	std::string cmd;
	size_t i = 0;
	while (i < attribs_.size())
	{
		if (attribs_[i].name() == "COMMAND")
		{
			cmd = attribs_[i].value();
			break;
		}
		i++;
	}
	return cmd;
}

//----< Get source IP from message >---------------------------------------

std::string Message::getsrcIP()
{
	std::string srcIP;
	size_t i = 0;
	while (i < attribs_.size())
	{
		if (attribs_[i].name() == "SOURCEIP")
		{
			srcIP = attribs_[i].value();
			break;
		}
		i++;
	}
	return srcIP;
}

//----< Get source port from message >---------------------------------------

size_t Message::getsrcPort()
{
	size_t srcport = 0;
	size_t i = 0;
	while (i < attribs_.size())
	{
		if (attribs_[i].name() == "SOURCEPORT")
		{
			srcport = std::stoi(attribs_[i].value());
			break;
		}
		i++;
	}
	return srcport;
}
//----< Get dest IPs from message >---------------------------------------

std::vector<std::string> Message::getdstIP()
{
	std::vector<std::string> dstIPvec;
	std::string dstIP;
	size_t i = 0;
	while (i < attribs_.size())
	{
		if (attribs_[i].name() == "IPADDR")
		{
			dstIP = attribs_[i].value();
			dstIPvec.push_back(dstIP);
		}
		i++;
	}
	return dstIPvec;

}

//----< Get dest ports from message >---------------------------------------

std::vector<size_t> Message::getdstPort()
{
	std::vector<size_t> dstportvec;
	size_t dstport = 0;
	size_t i = 0;
	while (i < attribs_.size())
	{
		if (attribs_[i].name() == "PORT")
		{
			dstport = std::stoi(attribs_[i].value());
			dstportvec.push_back(dstport);
		}
		i++;
	}
	return dstportvec;
}

//----< Get file path from message >---------------------------------------

std::string Message::getFilePath()
{
	std::string path;
	size_t i = 0;
	while (i < attribs_.size())
	{
		if (attribs_[i].name() == "FILEPATH")
		{
			path = attribs_[i].value();
			break;
		}
		i++;
	}
	return path;
}

//----< get Filesize  of the file using filesystem >---------------------------------------

//size_t Message::FileSize(std::string filespec)
//{
//	FileSystem::FileInfo fileinfo(filespec);
//	size_t tmp_filesize = fileinfo.size();
//	return tmp_filesize;
//}

//----< Get filesize from message >---------------------------------------

size_t Message::getFileSize()
{
	size_t fsize = 0;
	size_t i = 0;
	while (i < attribs_.size())
	{
		if (attribs_[i].name() == "FILESIZE")
		{
			fsize = std::stoi(attribs_[i].value());
			break;
		}
		i++;
	}
	return fsize;
}

//----< Get contentlength from message >---------------------------------------

size_t Message::getBlocksize()
{
	size_t bsize = 0;
	size_t i = 0;
	while (i < attribs_.size())
	{
		if (attribs_[i].name() == "CONTENTLENGTH")
		{
			bsize = std::stoi(attribs_[i].value());
			break;
		}
		i++;
	}
	return bsize;
}

//----< Get filepatterns from message >---------------------------------------

std::vector<std::string> Message::getFilePatt()
{
	std::string patt;
	std::vector<std::string> pattvec;
	size_t i = 0;
	while (i < attribs_.size())
	{
		if (attribs_[i].name() == "FILEPATTERN")
		{
			patt = attribs_[i].value();
			pattvec.push_back(patt);
			//break;
		}
		i++;
	}
	return pattvec;
}

//----< Interpret Message class - constructor >---------------------------------------

InterpretMsg::InterpretMsg(std::string &msg) : message_(msg)
{

	header_ = message_;
	size_t pos = 0;
	int k = 0;
	while (header_[k] != '\0')
	{
		std::string attrib;
		size_t tpos = header_.find("\n", pos);
		for (size_t l = pos; l < tpos; l++)
			attrib += header_[l];
		attribs_.push_back(Attr::fromString(attrib));
		pos = tpos++;
		k = ++pos;
	}
}

//----< Interpret command from message >---------------------------------------

std::string InterpretMsg::getCommand()
{
	std::string cmd;
	size_t i = 0;
	while (i < attribs_.size())
	{
		if (attribs_[i].name() == "COMMAND")
		{
			cmd = attribs_[i].value();
			break;
		}
		i++;
	}
	return cmd;
}

//----< Interpret source IP from message >---------------------------------------

std::string InterpretMsg::getsrcIP()
{
	std::string srcIP;
	size_t i = 0;
	while (i < attribs_.size())
	{
		if (attribs_[i].name() == "SOURCEIP")
		{
			srcIP = attribs_[i].value();
			break;
		}
		i++;
	}
	return srcIP;
}
//----< Interpret source port from message >---------------------------------------

size_t InterpretMsg::getsrcPort()
{
	size_t srcport = 0;
	size_t i = 0;
	while (i < attribs_.size())
	{
		if (attribs_[i].name() == "SOURCEPORT")
		{
			srcport = std::stoi(attribs_[i].value());
			break;
		}
		i++;
	}
	return srcport;
}

//----< Interpret Dest IP from message >---------------------------------------

std::string InterpretMsg::getdstIP()
{
	std::vector<std::string> dstIpVec;
	std::string dstIP;
	size_t i = 0;
	while (i < attribs_.size())
	{
		if (attribs_[i].name() == "DESTIP")
		{
			dstIP = attribs_[i].value();
			break;
		}
		i++;
	}
	return dstIP;
}

//----< Interpret dest port from message >---------------------------------------

size_t InterpretMsg::getdstPort()
{
	std::vector<size_t> dstportvec;
	size_t dstport = 0;
	size_t i = 0;
	while (i < attribs_.size())
	{
		if (attribs_[i].name() == "DESTPORT")
		{
			dstport = std::stoi(attribs_[i].value());
			break;
		}
		i++;
	}
	return dstport;
}

//----< Interpret filepath from message >---------------------------------------

std::string InterpretMsg::getFilePath()
{
	std::string path;
	size_t i = 0;
	while (i < attribs_.size())
	{
		if (attribs_[i].name() == "FILEPATH")
		{
			path = attribs_[i].value();
			break;
		}
		i++;
	}
	return path;
}

//----< Interpret Filesize from message >---------------------------------------

size_t InterpretMsg::getFileSize()
{
	size_t fsize = 0;
	size_t i = 0;
	while (i < attribs_.size())
	{
		if (attribs_[i].name() == "FILESIZE")
		{
			fsize = std::stoi(attribs_[i].value());
			break;
		}
		i++;
	}
	return fsize;
}

//----< Interpret contentlength from message >---------------------------------------

size_t InterpretMsg::getBlocksize()
{
	size_t bsize = 0;
	size_t i = 0;
	while (i < attribs_.size())
	{
		if (attribs_[i].name() == "CONTENTLENGTH")
		{
			bsize = std::stoi(attribs_[i].value());
			break;
		}
		i++;
	}
	return bsize;
}

//----< Interpret header from message >---------------------------------------

std::string InterpretMsg::getHeader()
{
	return header_;
}
std::string InterpretMsg::getMessage()
{
	return message_;
}
std::string InterpretMsg::getBlock()
{
	return body_;
}

//----< Interpret filepatterns from message >---------------------------------------

std::vector<std::string> InterpretMsg::getFilePatt()
{
	std::string patt;
	std::vector<std::string> pattvec;
	size_t i = 0;
	while (i < attribs_.size())
	{
		if (attribs_[i].name() == "FILEPATTERN")
		{
			patt = attribs_[i].value();
			pattvec.push_back(patt);
			//break;
		}
		i++;
	}
	return pattvec;
}

//----< Interpret text to be searched from message >---------------------------------------

std::string InterpretMsg::getText()
{
	std::string text;
	size_t i = 0;
	while (i < attribs_.size())
	{
		if (attribs_[i].name() == "TEXT")
		{
			text = attribs_[i].value();
			break;
		}
		i++;
	}
	return text;
}

//----< Interpret num of threads from message for concurrent string search >---------------------------------------

int InterpretMsg::numThreads()
{
	int numthread=0;
	size_t i = 0;
	while (i < attribs_.size())
	{
		if (attribs_[i].name() == "NO_OF_THREADS")
		{
			numthread = std::stoi(attribs_[i].value());
			break;
		}
		i++;
	}
	return numthread;
}

//----< Test stub >---------------------------------------

#ifdef TEST_MESSAGE

int main()
{
	Message msg("FILEUPLOAD");
	msg.addAttrib(Attr("COMMAND", msg.command()));
	msg.addAttrib(Attr("SOURCEIP", "localhost"));
	msg.addAttrib(Attr("SOURCEPORT", "8080"));
	msg.addAttrib(Attr("DESTIP", "localhost"));
	msg.addAttrib(Attr("DESTPORT", "9080"));
	msg.addAttrib(Attr("FILEPATH", "Test.txt"));
	msg.addAttrib(Attr("FILESIZE", "1024"));
	msg.addBody("some body");
	std::cout << "\n" << msg.toString();
	std::cout << "\n\n";
	std::cout << "\n\n Body :: " << msg.body();
	std::string message = msg.buildMsg(512);
	std::cout << "\n\n Message :: \n" << message;
	std::cout << "\n============================\n";
	InterpretMsg im(message);
	std::cout << " IM - Header :: \n" << im.getHeader();
	std::cout << "\n IM - Body ::  " << im.getBlock();
	std::cout << "\n IM - command ::  " << im.getCommand();
	std::cout << "\n IM - srcIP ::  " << im.getsrcIP();
	std::cout << "\n IM - srcport ::  " << im.getsrcPort(); 
	std::cout << "\n IM - dstIP ::  " << im.getdstIP();
	std::cout << "\n IM - dstPort ::  " << im.getdstPort();
	std::cout << "\n IM - Path ::  " << im.getFilePath();
	std::cout << "\n IM - size ::  " << im.getFileSize();
	std::cout << "\n IM - contetlength::  " << im.getBlocksize();
	std::cout << "\n==========================================\n";

	return 0;
}
#endif