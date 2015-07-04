/////////////////////////////////////////////////////////////////////////////
// ClientHandler.cpp - Handles Client Requests and process it              //
// Ver 1.1                                                                //
// Language:       Visual C++ 2013                                        //
// Application:    CSE687 - Project 4 (Remote Code management facility)   //
//					Spring 2015	   										  //
// Platform:       HP G62, Windows 7 Home Premium, Visual Studio 2013     //
// Author:         Dhivya Narayanan, Master's- Computer Engineering,      //
//				   Syracuse University,                                   //
//                 dnnaraya@syr.edu                                       //
////////////////////////////////////////////////////////////////////////////

#include"ClientHandler.h"
#include<chrono>
#include<iostream>
#include<fstream>
#include<string>

using namespace std;
//----< Client Handler - functor >------------------------------

void ClientHandler::operator()(Socket& socket_)
{
	while (true)
	{
		// interpret test command

		std::string msg = socket_.recvString();
		std::cout << "Received message :: " << msg << "\n";
		if (msg.size() == 0)
			break;

		bq.enQ(msg);

		if (msg == "START OF FILE_UPLOAD")
		{
			if (RecvFile(socket_))
				std::cout << "------ Message Received successfully ----\n\n";				
			else
				std::cout << "----Message  not Received successfully----\n";
			continue; // go back and get another command
		}
		if (msg == "FILE_DOWNLOAD REQUEST")
		{
			if (DownloadFile(socket_))
				std::cout << "-----------File Downloaded successfully-------\n\n";
			else
				std::cout << "-------Failed to download file-------\n";
			continue;
		}
		if (msg == "FILE_SEARCH REQUEST")
		{
			if (FileSearch(socket_))
				std::cout << "-----------File search completed successfully-------\n\n";				
			else
				std::cout << "-------Failed to search file-------\n";
			continue;
		}
		if (msg == "TEXT_SEARCH REQUEST")
		{
			if (TextSearch(socket_))
				std::cout << "-----------Text search completed successfully-------\n\n";
			else
				std::cout << "-------Text search failed-------\n";
			continue;
		}

		if (msg == "ACKNOWLEDGE")
		{
			if (RecvAcknowledge(socket_))
				std::cout << "\n----------- Acknowledge received ---------\n";
			else
				std::cout << "-----------Waiting for Acknowledgement ---------\n";
			continue;
		}

	}

	// we get here if command isn't requesting a test, e.g., "TEST_STOP"

	std::cout << "ClientHandler socket connection closing\n";
	socket_.shutDown();
	socket_.close();
	std::cout << "ClientHandler thread terminating\n";

}
//----< run when receives stop command >------------------------------

void ClientHandler::recvStop(Socket&socket_)
{
	std::cout << "\n\nClientHandler socket connection closing\n";
	socket_.shutDown();
	socket_.close();
	std::cout << "ClientHandler thread terminating\n";
}

//----< Receive messages and write the block into the file >------------------------------

bool ClientHandler::RecvFile(Socket& socket_)
{
	std::cout << "\nReceiving messages .......\n------------------------------------ - \n";
	std::ofstream myfile;
	std::string filename;
	if (!FileSystem::Directory::exists("OtherFiles"))
		FileSystem::Directory::create("OtherFiles");
	if (!FileSystem::Directory::exists("CPP"))
		FileSystem::Directory::create("CPP");
	if (!FileSystem::Directory::exists("JAVA"))
		FileSystem::Directory::create("JAVA");
	if (!FileSystem::Directory::exists("C#"))
		FileSystem::Directory::create("C#");
	std::string newfile;
	while (true)
	{
		std::string str = socket_.recvString();			//receives the string message
		bq.enQ(str);
		if (socket_ == INVALID_SOCKET)
			return false;
		if (str.size() > 0)
		{
			if (str == "TEST_STOP")
				recvStop(socket_);
			else
			{
				InterpretMsg IMsg(str);
				std::string filepath = IMsg.getFilePath();
				destport = IMsg.getsrcPort();    //source port in the message 
				destIP = IMsg.getsrcIP();
				sourceIP = IMsg.getdstIP();
				sourcePort = IMsg.getdstPort();	//this port -; dest port in the message
				filename = FileSystem::Path::getName(filepath);
				std::string ext = FileSystem::Path::getExt(filepath);
				if (ext == "cpp" || ext == "h")
					newfile = FileSystem::Path::fileSpec("CPP", filename);
				else if (ext == "java")
					newfile = FileSystem::Path::fileSpec("JAVA", filename);
				else if (ext == "cs")
					newfile = FileSystem::Path::fileSpec("C#", filename);
				else
					newfile = FileSystem::Path::fileSpec("OtherFiles", filename);
				size_t numBytes = IMsg.getBlocksize();
				
				std::ofstream out;
				out.open(newfile.c_str(), std::ios::binary);
				std::string encodedString = socket_.recvString();
				std::vector<char> fdecodedBytes = Base64::decode(encodedString);
				for (unsigned int i = 0; i<fdecodedBytes.size(); ++i)
					out.put(fdecodedBytes[i]);
				out.close();
				std::cout << "\n Received  message....";								
			}
		}
		else
			break;
		if (socket_.bytesWaiting() == 0)
			break;
	}
	
	return true;
}
//----< Send file download request message >------------------------------

bool ClientHandler::DownloadFile(Socket& socket_)
{
	while (true)
	{
		if (socket_.bytesWaiting() == 0)
			break;
		std::string str = socket_.recvString();
		bq.enQ(str);
		if (socket_ == INVALID_SOCKET)
			return false;
		InterpretMsg IMsg(str);
		std::string filepath = IMsg.getFilePath();
		destport = IMsg.getsrcPort();    //source port in the message 
		destIP = IMsg.getsrcIP();
		sourceIP = IMsg.getdstIP();
		sourcePort = IMsg.getdstPort();	//this port -; dest port in the message
		size_t numBytes = IMsg.getBlocksize();
		Message nmsg("FILE_DOWNLOAD");
		FileSystem::FileInfo fileinfo(filepath);
		size_t filesize = fileinfo.size();
		nmsg.addAttrib(Attr("COMMAND", nmsg.command()));
		nmsg.addAttrib(Attr("SOURCEIP", sourceIP));
		nmsg.addAttrib(Attr("SOURCEPORT", std::to_string(sourcePort)));
		nmsg.addAttrib(Attr("DESTIP", destIP));
		nmsg.addAttrib(Attr("DESTPORT", std::to_string(destport)));
		nmsg.addAttrib(Attr("FILEPATH", filepath));
		nmsg.addAttrib(Attr("FILESIZE", std::to_string(filesize)));
		Sendr sr;
		SocketSystem ss;
		SocketConnecter si;
		while (!si.connect(destIP, destport))			//connect to destPort - server
		{
			std::cout << "Client waiting to connect \n";
			::Sleep(100);
		}
		sr.sendFile(si, nmsg);

	}
	return true;
}

//----< Send file search request message >------------------------------


bool ClientHandler::FileSearch(Socket& socket_)
{
	while (true)
	{
		std::string str = socket_.recvString();			//receives the string message
		bq.enQ(str);
		if (socket_ == INVALID_SOCKET)
			return false;
		if (str.size() > 0)
		{
			if (str == "TEST_STOP")
				recvStop(socket_);
			else
			{
				InterpretMsg IMsg(str);
				std::string category = IMsg.getFilePath();
				std::vector<std::string> filepatt = IMsg.getFilePatt();
				destport = IMsg.getsrcPort();    //source port in the message 
				destIP = IMsg.getsrcIP();
				sourceIP = IMsg.getdstIP();
				sourcePort = IMsg.getdstPort();	//this port -; dest port in the message
				FileStorage ds;
				FileMgr fm(category, ds);
				for (size_t i = 0; i < filepatt.size(); i++)
					fm.addPattern(filepatt[i]);
				//std::string path = category;
				std::string path = "../Server/" + category;
				std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
				fm.search(path);
				std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

				std::cout << "\n\n<=== All Files and their paths present in the given path and patterns ====>\n\n";
				std::cout << " File Name\t" << " Paths in which the file is present\n";
				std::cout << "-------------     ------------------------------------\n";
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
				std::cout << "Time taken to complete FileSearch (in millisec) ::  " << chrono::duration_cast<chrono::milliseconds>(end - begin).count() << "\n\n";
				std::cout << "----------------------------------------------------------------------\n";
			}
		}
		else
			break;
		if (socket_.bytesWaiting() == 0)
			break;
	}
	
	return true;
}
bool ClientHandler::ConcStringSearch(Socket& socket_)
{
	while (true)
	{
		if (socket_.bytesWaiting() == 0)
			break;
		std::string str = socket_.recvString();
		bq.enQ(str);
		if (socket_ == INVALID_SOCKET)
			return false;
		InterpretMsg IMsg(str);
		std::string filepath = IMsg.getFilePath();
		std::vector<std::string> filepatt = IMsg.getFilePatt();
		int numThread = IMsg.numThreads();
		if (numThread == 0)
			numThread = 4;				//Default number of threads , in case of client doesn't mention no_of_threads
		destport = IMsg.getsrcPort();    //source port in the message 
		destIP = IMsg.getsrcIP();
		sourceIP = IMsg.getdstIP();
		sourcePort = IMsg.getdstPort();	//this port -; dest port in the message
		FileStorage ds;
		FileMgr fm(filepath, ds);
		for (size_t i = 0; i < filepatt.size(); i++)
			fm.addPattern(filepatt[i]);
		fm.search(filepath);
		Catalog cs(ds);
		std::string text = IMsg.getText();
		std::vector<std::string> TextinFiles;
		std::set<std::string> temp_file;
		std::set<std::string> temp_path;
		TextinFiles = cs.ConcurrentStringSearch(text, numThread);
		std::cout << "\n------< The given text found in following file paths >--------\n\n";
		std::cout << " File Name\t" << " Paths in which the file is present\n";
		std::cout << "-----------     ------------------------------------\n";
		for (size_t i = 0; i < TextinFiles.size(); i++)
		{
			std::cout << FileSystem::Path::getName(TextinFiles[i]) << "\n    \t\t";
			temp_file.insert(FileSystem::Path::getName(TextinFiles[i]));
			std::cout << FileSystem::Path::getPath(TextinFiles[i]) << "\n\n";
			temp_path.insert(FileSystem::Path::getPath(TextinFiles[i]));
		}

		std::cout << "\n\n---------< Summary of files which contains the given text >-------\n\n";
		std::cout << " Total No.of files  :  " << TextinFiles.size() << "\n";
		std::cout << " No.of Unique files :  " << temp_file.size() << "\n";
		std::cout << " No.of Unique paths :  " << temp_path.size() << "\n\n";
		std::cout << ".....................................................................\n\n";
		
	}
	return true;

}
//----< send text search request message>------------------------------

bool ClientHandler::TextSearch(Socket& socket_)
{
	while (true)
	{
		if (socket_.bytesWaiting() == 0)
			break;
		std::string str = socket_.recvString();
		bq.enQ(str);
		if (socket_ == INVALID_SOCKET)
			return false;
		InterpretMsg IMsg(str);
		std::string filepath = IMsg.getFilePath();
		//std::string filepath = "../Server";
		std::vector<std::string> filepatt = IMsg.getFilePatt();
		destport = IMsg.getsrcPort();    //source port in the message 
		destIP = IMsg.getsrcIP();
		sourceIP = IMsg.getdstIP();
		sourcePort = IMsg.getdstPort();	//this port -; dest port in the message
		FileStorage ds;
		FileMgr fm(filepath, ds);
		for (size_t i = 0; i < filepatt.size(); i++)
			fm.addPattern(filepatt[i]);
		fm.search(filepath);
		Catalog cs(ds);
		std::string text = IMsg.getText();
		std::vector<std::string> TextinFiles;
		std::set<std::string> temp_file;
		std::set<std::string> temp_path;
		std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
		for (auto pat : filepatt)
		{
			std::vector<std::string> TFiles = cs.FindText_in_File(text, pat);
			for (size_t i = 0; i < TFiles.size(); i++)
				TextinFiles.push_back(TFiles[i]);
		}
		std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
		std::cout << "\n------< The given text found in following file paths >--------\n\n";
		std::cout << "Machine on which the files reside ::\n----------------------------------- \n IP Addr  :  " << sourceIP << "\n Port  :  " << sourcePort;
		std::cout << "\n\n File Name\t" << " Paths in which the file is present\n";
		std::cout << "-----------     ------------------------------------\n";
		for (size_t i = 0; i < TextinFiles.size(); i++)
		{
			std::cout << FileSystem::Path::getName(TextinFiles[i]) << "\n    \t\t";
			temp_file.insert(FileSystem::Path::getName(TextinFiles[i]));
			std::cout << FileSystem::Path::getPath(TextinFiles[i]) << "\n\n";
			temp_path.insert(FileSystem::Path::getPath(TextinFiles[i]));
		}

		std::cout << "\n\n---------< Summary of files which contains the given text >-------\n\n";
		std::cout << " Total No.of files  :  " << TextinFiles.size() << "\n";
		std::cout << " No.of Unique files :  " << temp_file.size() << "\n";
		std::cout << " No.of Unique paths :  " << temp_path.size() << "\n\n";
		std::cout << ".....................................................................\n\n";
		std::cout << "Time taken to complete FileSearch (in millisec) ::  " << chrono::duration_cast<chrono::milliseconds>(end - begin).count() << "\n\n";
		std::cout << "----------------------------------------------------------------------\n";
	}
	return true;
}

//----< runs when receive acknowledgement >------------------------------

bool ClientHandler::RecvAcknowledge(Socket& socket_)
{
	while (true)
	{
		if (socket_.bytesWaiting() == 0)
			break;
		std::string str = socket_.recvString();
		if (socket_ == INVALID_SOCKET)
			return false;
		if (str.size() > 0)
		{
			std::cout << "\nClient recvd....:   ";
			std::cout << str << "\n";
			bq.enQ(str);
		}
		else
			break;
	}
	return true;

}

//----< returns DestIP - same as sourceIP in the message >------------------------------

std::string ClientHandler::getdestIp()		//gets the sourceIp in the message
{
	return destIP;
}

//----< returns Destport - same as sourcePort in the message >------------------------------

size_t ClientHandler::getdestPort()
{
	return destport;
}

#ifdef TEST_CLIENTHANDLER
int main()
{
	return 0;
}
#endif