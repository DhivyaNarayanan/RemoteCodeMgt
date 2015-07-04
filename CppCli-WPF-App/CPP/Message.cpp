// NativeMessage.cpp

#include "Message.h"
#include"../ApplicationHelpers/AppHelpers.h"
#include <sstream>
#include <algorithm>

using namespace ApplicationHelpers;

Attribute::Attribute(const std::string& name, const std::string& value) : name_(name), value_(value) {}

std::string Attribute::name() { return name_; }

std::string Attribute::value() { return value_; }

std::string Attribute::toString()
{
	std::ostringstream temp;
	temp << name_ << " : " << value_;
	return temp.str();
}

Attribute Attribute::fromString(const std::string& src)
{
	std::string name, value;
	std::string::const_iterator iter = std::find(src.begin(), src.end(), ':');
	if (iter != std::end(src))
	{
		name = std::string(std::begin(src), --iter);
		value = std::string(iter + 3, std::end(src));
	}
	return Attribute(name, value);
}

Message::Message(const std::string& command) : command_(command) {}

void Message::addAttrib(const Attribute& attr)
{
	attribs_.push_back(attr);
}

void Message::addBody(const std::string& body)
{
	body_ = body;
}

std::string Message::command()
{
	return command_;
}

Attribute Message::operator[](size_t i)
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

std::string Message::makeHeader(size_t blocksize)
{
	numBytes = blocksize;

	addAttrib(Attribute("CONTENTLENGTH", std::to_string(blocksize)));
	for (auto item : attribs_)
	{
		header_ += item.toString();
		header_ += '\n';
	}
	return header_;
}
std::string Message::getHeader()
{
	return header_;
}

std::string Message::buildMsg(size_t blocksize)
{
	message_ = makeHeader(blocksize);
	message_ += '\n';
	message_ += body();

	return message_;
}
std::string Message::getCommand()
{
	std::string cmd;
	int i = 0;
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
std::string Message::getsrcIP()
{
	std::string srcIP;
	int i = 0;
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
size_t Message::getsrcPort()
{
	size_t srcport = 0;
	int i = 0;
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
std::string Message::getdstIP()
{
	std::string dstIP;
	int i = 0;
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
size_t Message::getdstPort()
{
	size_t dstport = 0;
	int i = 0;
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
std::string Message::getFilePath()
{
	std::string path;
	int i = 0;
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

size_t Message::FileSize(std::string filespec)
{
	FileSystem::FileInfo fileinfo(filespec);
	size_t tmp_filesize = fileinfo.size();
	return tmp_filesize;
}

size_t Message::getFileSize()
{
	size_t fsize = 0;
	int i = 0;
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
size_t Message::getBlocksize()
{
	size_t bsize = 0;
	int i = 0;
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

InterpretMsg::InterpretMsg(std::string &msg) : message_(msg)
{
	std::string tmsg = message_;
	//std::string attrib;
	size_t eof_header = tmsg.find("\n\n");
	int i = 0;
	while (i < eof_header)
	{
		header_ += tmsg[i];
		i++;
	}
	header_ += '\n';
	int j = eof_header;
	while (tmsg[j] == '\n')
		j++;
	while (tmsg[j] != '\0')
	{
		body_ += tmsg[j];
		j++;
	}
	size_t pos = 0;
	int k = 0;
	while (header_[k] != '\0')
	{
		std::string attrib;
		size_t tpos = header_.find("\n", pos);
		for (int l = pos; l < tpos; l++)
			attrib += header_[l];
		attribs_.push_back(Attribute::fromString(attrib));
		pos = tpos++;
		k = ++pos;
	}
}

std::string InterpretMsg::getCommand()
{
	std::string cmd;
	int i = 0;
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
std::string InterpretMsg::getsrcIP()
{
	std::string srcIP;
	int i = 0;
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
size_t InterpretMsg::getsrcPort()
{
	size_t srcport = 0;
	int i = 0;
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
std::string InterpretMsg::getdstIP()
{
	std::string dstIP;
	int i = 0;
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
size_t InterpretMsg::getdstPort()
{
	size_t dstport = 0;
	int i = 0;
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
std::string InterpretMsg::getFilePath()
{
	std::string path;
	int i = 0;
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
size_t InterpretMsg::getFileSize()
{
	size_t fsize = 0;
	int i = 0;
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
size_t InterpretMsg::getBlocksize()
{
	size_t bsize = 0;
	int i = 0;
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

#include <iostream>

#ifdef TEST_MESSAGE

int main()
{
	title("testing Message classes", '=');
	putLine();

	title("testing attributes class");
	Attribute test1("jim", "instructor");
	std::cout << "\n  " << test1.toString();

	Attribute test2 = Attribute::fromString("vishal : TA");
	std::cout << "\n  " << test2.toString() << "\n\n";

	title("testing NMessage class");
	Message msg("FILEUPLOAD");
	msg.addAttrib(Attribute("COMMAND", msg.command()));
	msg.addAttrib(Attribute("SOURCEIP", "localhost"));
	msg.addAttrib(Attribute("SOURCEPORT", "8080"));
	msg.addAttrib(Attribute("DESTIP", "localhost"));
	msg.addAttrib(Attribute("DESTPORT", "9080"));
	msg.addAttrib(Attribute("FILEPATH", "Test.txt"));
	msg.addAttrib(Attribute("FILESIZE", "1024"));
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