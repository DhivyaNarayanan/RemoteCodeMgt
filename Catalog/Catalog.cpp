//////////////////////////////////////////////////////////////////////////////////////
// Catalog.cpp -   Supports Text Search in the files present in the filestorage and //
//                 provides supporrt to store the file in the filestorage           //
//				                                                                    //
// ver 1.0                                                                          //
// Language:       Visual C++ 2013                                                  //
// Application:    CSE687 - Project 1 (File Catalogue), Spring 2015                 //
// Platform:       HP G62, Windows 7 Home Premium, Visual Studio 2013               //
// Author:         Dhivya Narayanan, Master's- Computer Engineering,                //
//				   Syracuse University,                                             //
//                 dnnaraya@syr.edu                                                 //
//////////////////////////////////////////////////////////////////////////////////////
#include"Catalog.h"
#include<fstream>

//using FQueue = BlockingQueue<std::string>;

//----< Handles String search concurrently using threads  >---------------------------------------

std::vector<std::string>Catalog::ConcurrentStringSearch(std::string& text, int no_of_threads)
{
	std::vector<std::string>filepaths;
	for (auto fs : store)
	{
		FileStorage::iter_list it_list = fs.second.begin();
		while (it_list != fs.second.end())
		{
			std::string path = FileSystem::Path::fileSpec((*(*(it_list))), fs.first);
			filepaths.push_back(path);
			it_list++;
		}	
	}
	for (size_t i = 0; i < filepaths.size(); i++)
		//FileipQ.enQ(filepaths[i]);

	std::vector<std::thread *> threads;
	/*for (int i = 0; i < no_of_threads; i++)
		threads.push_back(std::thread(&Catalog::StringSearch, text));

	for (auto&thread : threads)
		thread.join();*/
				
	/*while (FileOpQ.size() > 0)
		TxtinFiles.push_back(FileOpQ.deQ());*/
		
	return TxtinFiles;
}

//----< method handle by thread for text search  >---------------------------------------

void Catalog::StringSearch(std::string&text)
{
	//static std::mutex mtx;
	std::string file;
	//mtx.lock();
	//std::string file = FileipQ.deQ();
	//mtx.unlock();
	std::string FileText;
	std::string null_val = "";
	try
	{
		std::string ext = FileSystem::Path::getExt(file);
		if (ext != "dll" && ext != "exe" && file != "run.bat" && file != "compile.bat")
		{
			FileSystem::File infile(file);
			infile.open(FileSystem::File::in);
			if (infile.isGood())
			{
				FileText = infile.readAll(false);
				for (int i = 0; FileText[i] != '\0'; i++)
				{
					FileText[i] = tolower(FileText[i]);					//handles case-insensitive text search
					if (FileText[i] == '\t'&& FileText[i + 1] != '\t')  // handles extra space or tab character in between the strings
						FileText[i] = ' ';
					else if ((FileText[i] == '\t'&& FileText[i + 1] == '\t') || (FileText[i] == ' ' && FileText[i + 1] == ' '))
					{
						FileText.erase(i, 1);
						i--;
					}
				}
			}
			else
				std::cout << "Failed to open the file. \n";
		}
	}
	catch (std::exception& ex)
	{
		std::cout << "\n Exception: " << ex.what() << "\n";
	}
	size_t pos;
	for (int j = 0; text[j] != '\0'; j++)
		text[j] = tolower(text[j]);
	pos = FileText.find(text);
	if (pos != std::string::npos)
		std::string filep = file;
		//FileOpQ.enQ(file);	
}

//----< accepts text and file patterns ; search text in the files only with the given patterns  >---------------------------------------

std::vector<std::string> Catalog::FindText_in_File(std::string& text, std::string& filepatt)
{
	std::vector<std::string>TextinFiles;
	std::string filename_pat;
	FileStorage::iter_map it_map = store.begin();
	while (it_map != store.end())
	{
		FileStorage::iter_list it_list = it_map->second.begin();
		while (it_list != it_map->second.end())
		{
			std::string t_path = FileSystem::Path::fileSpec(**it_list, it_map->first);
			if (filepatt[0] != '*')									//If filename is entered in the console as pattern
				filename_pat = FileSystem::Path::getName(t_path);
			else
			{
				std::string ext = FileSystem::Path::getExt(t_path);
				std::string temp_char = "*.";
				filename_pat = temp_char.append(ext);
			}
			if (filename_pat == filepatt || filepatt == "*.*")
			{
				std::string FileContainsText = TextSearch(text, t_path);
				if (FileContainsText != "")
				{
					TextinFiles.push_back(FileContainsText);
				}
			}
			it_list++;
		}
		it_map++;
	}
	return TextinFiles;
}

//----< search for given text (case-insensitive) in the files stored in the filestorage >---------------------------------------

std::string Catalog::TextSearch(std::string& text, std::string & file)
{
	std::string FileText;
	std::string null_val = "";
	try
	{
		std::string ext = FileSystem::Path::getExt(file);
		if (ext != "dll" && ext != "exe" && file != "run.bat" && file != "compile.bat")
		{
			FileSystem::File infile(file);
			infile.open(FileSystem::File::in);
			if (infile.isGood())
			{
				FileText = infile.readAll(false);
				for (int i = 0; FileText[i] != '\0'; i++)
				{
					FileText[i] = tolower(FileText[i]);					//handles case-insensitive text search
					if (FileText[i] == '\t'&& FileText[i + 1] != '\t')  // handles extra space or tab character in between the strings
						FileText[i] = ' ';
					else if ((FileText[i] == '\t'&& FileText[i + 1] == '\t') || (FileText[i] == ' ' && FileText[i + 1] == ' '))
					{
						FileText.erase(i, 1);
						i--;
					}
				}
			}
			else
				std::cout << "Failed to open the file. \n";
		}
	}
	catch (std::exception& ex)
	{
		std::cout << "\n Exception: " << ex.what() << "\n";
	}
	size_t pos;
	for (int j = 0; text[j] != '\0'; j++)
		text[j] = tolower(text[j]);
	pos = FileText.find(text);
	if (pos != std::string::npos)
		return file;
	return null_val;

}

//----< test stub >--------------------------------------------------------

#ifdef TEST_CATALOG

int main()
{
	std::cout << "Testing Catalog....\n";
	std::cout << "-------------------------------\n";

	FileStorage fs;
	Catalog ct(fs);
	std::string testfile_ = "./Catalog.cpp";
	std::string testtext_ = "class";
	std::string patt = "*.cpp";

	std::string file = ct.TextSearch(testtext_, testfile_);
	std::cout << "File Contains the text 'class' :: " << file << "\n";

	return 0;

}

#endif
