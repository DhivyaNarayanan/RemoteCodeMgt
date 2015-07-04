**************READ ME**************
Remote Code Management Facility - Project4
======================

Client GUI - WPF
=================
Totally 4 Tabs
* FILE UPLOAD - Get the input for transfering files to the server (File upload)
* FILE DOWNLOAD - Get the input for transfering files from the server(FileDownload)
* FILE SEARCH - Get input for file search commands
		- List all files in the server path (../Server) providing the 				  Category (C#, CPP, JAVA , OTHERFILES)
* TEXT SEARCH - Get input for text search commands 
		- List all files in the server path (../Server) which has the given 			  text

Sample User Inputs in GUI :
=============================

For FILE UPLOAD command
........................
- click "FILE UPLOAD" tab
- Enter Dest IP :   localhost
- Enter Dest Port :  9080
- Click the "Select File" button - You will be provided with browse file panel
 - Select any file to upload
- Click "UPLOAD" button

- Click "CLEAR" to clear the inputs and also to repeat the File Upload task

Output :
--------
- While File is uploading , you can see the "sending message.." in Client console and "Received message.." in server console
- Once the file is uploaded - you will get the message "File uploaded Successfully" in client console
- The uploaded file will be stored in the server path "../Server"
if it is  ".cpp" file - will get stored in "../Server/CPP"
if it is  ".h" file - will get stored in "../Server/CPP"
if it is  ".java" file - will get stored in "../Server/JAVA"
if it is  ".cs" file - will get stored in "../Server/C#"
Other files like ".txt",".bin" will be stored in "../Server/OTHERFILES"


For FILE DOWNLOAD command
..........................

- click "FILE DOWNLOAD" tab
- Enter Dest IP :   localhost
- Enter Dest Port :  9080
- Click the "Select File" button - You will be provided with browse file panel
 - Select any file to download
 - To download the file in the server - select the file in Server directory
- Click "DOWNLOAD" button

- Click "CLEAR" to clear the inputs and also to repeat the File download task

Output :
--------
- While File is downloading , you can see the "sending message.." in server console and "Received message.." in client console
- Once the file is downloaded - you will get the message "File downloaded Successfully" in client console
- The downloaded file will be stored in the path "../CppCli-WPF-App"
if it is  ".cpp" file - will get stored in "../CppCli-WPF-App/CPP"
if it is  ".h" file - will get stored in "../CppCli-WPF-App/CPP"
if it is  ".java" file - will get stored in "../CppCli-WPF-App/JAVA"
if it is  ".cs" file - will get stored in "../CppCli-WPF-App/C#"
Other files like ".txt",".bin" will be stored in "../CppCli-WPF-App/OTHERFILES"


For FILE SEARCH command
..........................

- click "FILE SEARCH" tab
- Enter Dest IP :   localhost
- Enter Dest Port :  9080
- Enter Category : CPP 
Enter either "CPP" or "C#" or "JAVA" or "OTHERFILES"
-Enter FilePatterns : *.h *.cpp

- For multiple file patterns, Enter by giving space between each of them

- Click "CLEAR" to clear the inputs and also to repeat the File Search task

Output :
--------
- List all the files present in the given category in the server path which matches the given patterns


For TEXT SEARCH command
..........................

- click "TEXT SEARCH" tab
- Enter Dest IP :   localhost localhost
	For multiple IP, Enter by giving space between each of them
- Enter Dest Port :  9080 9090
	For multiple file patterns, Enter by giving space between each of them
- Server Path : ../Server (no need to enter - hardcoded)
- Enter FilePatterns : *.h *.cpp
	For multiple file patterns, Enter by giving space between each of them
- Enter Text :  AbstractXmlElement

- Click "CLEAR" to clear the inputs and also to repeat the File Search task

Output :
--------
- List all the files with the given pattern present in the given category in the server path which contains the specified text













