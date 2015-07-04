/////////////////////////////////////////////////////////////////////////////
// SenderReceiver.cpp -  Provides Sendr and Recvr class to send & receive //
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

#define IN_DLL
#include"SenderReceiver.h"
#include"../ClientHandler/ClientHandler.h"
#include<iostream>
#include<fstream>
#include<string>

using namespace std;

//----< enqueue message to sender queue - get msg from GUI >--------------------------------------------------

void Sendr::postMessage(const Message& msg)
{
	sendQ_.enQ(msg);
}

void Sendr::EnqMessage(const std::string &msg)
{
	ipQ_.enQ(msg);
}

BQueue& Sendr::queue() { return sendQ_; }

//----< method to build the message and send - file upload >------------------------------

void Sendr::sendFile(Socket&si, Message&msg_)
{
	std::string filepath = msg_.getFilePath();
	std::string ext = FileSystem::Path::getExt(filepath);
	FileSystem::FileInfo fileinfo(filepath);
	size_t filesize = fileinfo.size();
	std::string str, startmsg = "START OF FILE_UPLOAD";
	int bytesRead, bytesRemaining = filesize;
	EnqMessage(startmsg);
	
	std::ifstream in;
	in.open(filepath.c_str(), std::ios::binary);
	std::vector<char> fBytes;
	while (in.good())
	{
		int i = 0;
		while (i < 1024 && in.good())
		{
			fBytes.push_back(in.get());
			i++;
			bytesRead = i;
		}
		if (bytesRead < 1024)
			fBytes.pop_back();
		msg_.addBody(Base64::encode(fBytes));
		EnqMessage(msg_.makeHeader(filesize));
		EnqMessage(Base64::encode(fBytes));
		std::cout << " \n Sending message .........";
	}
	in.close();
	while (ipQ_.size()>0)
		si.sendString(ipQ_.deQ());
	
	std::cout << "\n------------------------------------------\nFile uploaded successfully ..\n=================================\n";
	//sendStop(si);

}

//----< creates socket and send the message which in turn calls sendMsg function >------------------------------

void Sendr::Send()
{
	SocketSystem ss;
	SocketConnecter si;					//create socketConnector
	
	Message msg_ = sendQ_.deQ();

	std::string MsgHeader = msg_.getHeader();
	std::string cmd = msg_.getCommand();
	std::cout << "Command :: " << cmd << "\n";
	std::string sourceIP = msg_.getsrcIP();
	size_t sourcePort = msg_.getsrcPort();
	std::string filepath = msg_.getFilePath();
	for (size_t i = 0; i < msg_.getdstPort().size(); i++)
	{
		std::string destIP = msg_.getdstIP()[i];
		size_t destPort = msg_.getdstPort()[i];
		msg_.addAttrib(Attr("DESTIP", destIP));
		msg_.addAttrib(Attr("DESTPORT", std::to_string(destPort)));
		while (!si.connect(destIP, destPort))			//connect to destPort - server
		{
			std::cout << "Client waiting to connect \n";
			::Sleep(100);
		}
		std::cout << "\nClient is connected to the server - Port : " << destPort << "\n";
		if (cmd.compare("FILE_UPLOAD") == 0)
			sendFile(si, msg_);							//build and send message over socket
		else if (cmd.compare("FILE_DOWNLOAD") == 0)
			sendDownloadMsg(si, msg_);
		else if (cmd.compare("FILE_SEARCH") == 0)
			sendFileSearchMsg(si, msg_);
		else if (cmd.compare("TEXT_SEARCH") == 0)
			sendTxtSearchMsg(si, msg_);
	}
}

//----< Send FileSearch request message to server >--------------------------------------------------

void Sendr::sendFileSearchMsg(Socket&si, Message&msg_)
{
	std::string startmsg = "FILE_SEARCH REQUEST";
	EnqMessage(startmsg);
	EnqMessage(msg_.makeHeader(0));
	while (ipQ_.size()>0)
		si.sendString(ipQ_.deQ());
	std::cout << "\n File search request sent...\n";
}

//----< Send text search request message to server >--------------------------------------------------

void Sendr::sendTxtSearchMsg(Socket&si, Message&msg_)
{
	std::string startmsg = "TEXT_SEARCH REQUEST";
	EnqMessage(startmsg);
	EnqMessage(msg_.makeHeader(0));
	while (ipQ_.size()>0)
		si.sendString(ipQ_.deQ());
	std::cout << "\n Text search request sent...\n";
}

//----< Send file download request message to server >--------------------------------------------------

void Sendr::sendDownloadMsg(Socket&si, Message& msg_)
{
	std::string startmsg = "FILE_DOWNLOAD REQUEST";
	EnqMessage(startmsg);
	std::cout << "Send message to server :: " << startmsg << "\n";
	EnqMessage(msg_.makeHeader(0));
	while (ipQ_.size()>0)
		si.sendString(ipQ_.deQ());
	std::cout << "\n File download request sent...\n";
}
//----< send stop message >------------------------------

void Sendr::sendStop(Socket&si)
{
	si.sendString("TEST_STOP");

	std::cout << "\n ----- client calling send shutdown -----\n";
	si.shutDownSend();
}

//----< send acknowledgement >------------------------------

void Sendr::sendAck(std::string sourceIp, size_t sourcePort, std::string destIp, size_t destPort)
{
	SocketSystem ss;
	SocketConnecter si;

	while (!si.connect(destIp, destPort))
	{
		std::cout << "Client waiting to connect \n";
		::Sleep(100);
	}
	si.sendString("ACKNOWLEDGE");
	std::string ack_str = "Acknowledgement from server..";
	si.sendString(ack_str);
	std::cout << "\n---------------------------------";
	std::cout << "\ndestIP   :: " << destIp;
	std::cout << "\ndestPort :: " << destPort << "\n";
	std::cout << "Sending Acknowledgement.....\n";
	std::cout << "-------------------------------\n";
}

///////////////////////////////////////////////////////////////////////////////////////

//----< Recvr class - receiver >--------------------------------------------------
//----< create socket listener and starts the clientHandler which receive strings >------------------------------

void Recvr::Recv()
{
	try
	{
		SocketSystem ss;
		SocketListener sl(sourcePort, Socket::IP6);
		ClientHandler cp(opQ_);
		sl.start(cp);
		destIP = cp.getdestIp();
		destport = cp.getdestPort();
		sourceIP = "localhost";

	}
	catch (std::exception& ex)
	{
		std::cout << "  Exception caught:" << ex.what() << "\n\n";
	}
}

//----< get the message from the receiver queue >--------------------------------------------------

std::string Recvr::getMessage()
{
	recvQ_.enQ(opQ_.deQ());
	return recvQ_.deQ();
}

bq& Recvr::queue()
{
	return recvQ_;
}

std::string Recvr::DeqMessage()
{
	return opQ_.deQ();
}

//----< return source IP address - this IP >------------------------------

std::string Recvr::getsrcIP()
{
	return sourceIP;
}

//----<return source Port address - this port >------------------------------

size_t Recvr::getsrcPort()
{
	return sourcePort;
}

//----< returns Dest IP -- srcaddress in the message >------------------------------

std::string Recvr::getdestIp()
{
	return destIP;
}

//----< returns dest port - srcport in the message >------------------------------

size_t Recvr::getdestPort()
{
	return destport;
}

///////////////////////////////////////////////////////////////////////////////////
//----< pass pointers to Sender and Receiver >-------------------------------

MockChannel::MockChannel(ISendr* pSendr, IRecvr* pRecvr) : pISendr_(pSendr), pIRecvr_(pRecvr) {}

//----< creates thread to read from sendQ and echo back to the recvQ >-------

void MockChannel::start()
{
	std::cout << "\n  MockChannel starting up";
	thread_ = std::thread(
		[this] {
		Sendr* pSendr = dynamic_cast<Sendr*>(pISendr_);
		Recvr* pRecvr = dynamic_cast<Recvr*>(pIRecvr_);
		if (pSendr == nullptr || pRecvr == nullptr)
		{
			std::cout << "\n  failed to start Mock Channel\n\n";
			return;
		}
		BQueue& sendQ = pSendr->queue();
		bq& recvQ = pRecvr->queue();
		while (!stop_)
		{
			pSendr->Send();
			pRecvr->Recv();
		}
		std::cout << "\n  Server stopping\n\n";
	});
}
//----< signal server thread to stop >---------------------------------------

void MockChannel::stop() { stop_ = true; }


//----< factory functions >--------------------------------------------------

ISendr* ObjectFactory::createSendr() { return new Sendr; }

IRecvr* ObjectFactory::createRecvr() { return new Recvr(8080); }

IMockChannel* ObjectFactory::createMockChannel(ISendr* pISendr, IRecvr* pIRecvr)
{
	return new MockChannel(pISendr, pIRecvr);
}

//----< test stub >------------------------------


#ifdef TEST_SENDERRECEIVER

int main()
{
	Sendr sr;
	Recvr rc(9080);
	rc.Recv();
	std::string path = "../Message.h";
	Message msg("FILE_UPLOAD");
	msg.addAttrib(Attr("COMMAND", msg.command()));
	msg.addAttrib(Attr("SOURCEIP", "localhost"));
	msg.addAttrib(Attr("SOURCEPORT", "8080"));
	msg.addAttrib(Attr("DESTIP", "localhost"));
	msg.addAttrib(Attr("DESTPORT", "9080"));
	msg.addAttrib(Attr("FILEPATH", path));
	//msg.addAttrib(Attribute("FILESIZE", "1024"));
	sr.postMessage(msg);
	sr.Send();
	return 0;

}
#endif

