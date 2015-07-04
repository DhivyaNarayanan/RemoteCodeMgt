#ifndef WINDOW_H
#define WINDOW_H
//////////////////////////////////////////////////////////////////////////////////////
// Window.h -     C++\CLI implementation of WPF Application							//
//				                                                                    //
// ver 1.0                                                                          //
// Language:       Visual C++ 2013                                                  //
// Application:    CSE687 - Project 4 (Remote code management facility),Spring 2015 //
// Platform:       HP G62, Windows 7 Home Premium, Visual Studio 2013               //
// Author:         Dhivya Narayanan, Master's- Computer Engineering,                //
//				   Syracuse University,                                             //
//                 dnnaraya@syr.edu													//
// Source:         Window.h - Demo for CSE 687 Project #4 ,ver 1.0					//
//                 Jim Fawcett, CSE687 - Object Oriented Design, Spring 2015		//
//////////////////////////////////////////////////////////////////////////////////////
/*
*  Package Operations:
*  -------------------
*  This package demonstrates how to build a C++\CLI WPF application.  It
*  provides one class, WPFCppCliDemo, derived from System::Windows::Window
*  that is compiled with the /clr option to run in the Common Language
*  Runtime, and another class MockChannel written in native C++ and compiled
*  as a DLL with no Common Language Runtime support.
*
*  The window class hosts, in its window, a tab control with three views, two
*  of which are provided with functionality that you may need for Project #4.
*  It loads the DLL holding MockChannel.  MockChannel hosts a send queue, a
*  receive queue, and a C++11 thread that reads from the send queue and writes
*  the deQ'd message to the receive queue.
*
*  The Client can post a message to the MockChannel's send queue.  It hosts
*  a receive thread that reads the receive queue and dispatches any message
*  read to a ListBox in the Client's FileList tab.  So this Demo simulates
*  operations you will need to execute for Project #4.
*
*  Required Files:
*  ---------------
*  Window.h, Window.cpp, MochChannel.h, MochChannel.cpp,
*  Cpp11-BlockingQueue.h, Cpp11-BlockingQueue.cpp
*
*  Build Command:
*  --------------
*  devenv CppCli-WPF-App.sln
*  - this builds C++\CLI client application and native mock channel DLL
*
*  Maintenance History:
*  --------------------
*  ver 1.0 - 24 Apr 2015
*/
/*
* Create C++/CLI Console Application
* Right-click project > Properties > Common Language Runtime Support > /clr
* Right-click project > Add > References
*   add references to :
*     System
*     System.Windows.Presentation
*     WindowsBase
*     PresentatioCore
*     PresentationFramework
*/

#include"../MockChannel/MockChannel.h"
#include"../Message/Message.h"
#include <iostream>
#include<algorithm>
#include<string>

using namespace System;
using namespace System::Text;
using namespace System::Windows;
using namespace System::Windows::Input;
using namespace System::Windows::Markup;
using namespace System::Windows::Media;                   // TextBlock formatting
using namespace System::Windows::Controls;                // TabControl
using namespace System::Windows::Controls::Primitives;    // StatusBar
using namespace System::Threading;
using namespace System::Threading::Tasks;
using namespace System::Windows::Threading;
using namespace System::ComponentModel;

namespace CppCliWindows
{
	ref class WPFCppCliDemo : Window
	{
		// MockChannel references
		ISendr* pSendr1_;
		IRecvr* pRecvr1_;
		IMockChannel* pChann1_;

		ISendr* pSendr2_;
		IRecvr* pRecvr2_;
		IMockChannel* pChann2_;

		ISendr* pSendr3_;
		IRecvr* pRecvr3_;
		IMockChannel* pChann3_;

		ISendr* pSendr4_;
		IRecvr* pRecvr4_;
		IMockChannel* pChann4_;


		// Controls for Window
		DockPanel^ hDockPanel = gcnew DockPanel();      // support docking statusbar at bottom
		Grid^ hGrid = gcnew Grid();
		TabControl^ hTabControl = gcnew TabControl();
		TabItem^ hFileUploadTab = gcnew TabItem();      // Fileupload tab
		TabItem^ hFileDownloadTab = gcnew TabItem();    // FileDownload tab
		TabItem^ hFileSearchTab = gcnew TabItem();		// FileSearch tab
		TabItem^ hTextSearchTab = gcnew TabItem();      // TextSearch tab
		TabItem^ hViewMessageTab = gcnew TabItem();     // View Message tab
		StatusBar^ hStatusBar = gcnew StatusBar();
		StatusBarItem^ hStatusBarItem = gcnew StatusBarItem();
		TextBlock^ hStatus = gcnew TextBlock();

		//Controls for view message tab view
		Grid^ hViewMessageGrid = gcnew Grid();
		Button^ vClearButton = gcnew Button();
		TextBlock^ vTextBlock1 = gcnew TextBlock();
		ScrollViewer^ vScrollViewer1 = gcnew ScrollViewer();
		StackPanel^ vStackPanel1 = gcnew StackPanel();

		// Controls for File upload View
		Grid^ hFileUploadGrid = gcnew Grid();		//Fileupload grid
		Button^ hFileUploadButton = gcnew Button();
		Button^ hClearButton = gcnew Button();
		TextBox^ hDstIpTxtBox = gcnew TextBox();
		TextBox^ hDstPortTxtBox = gcnew TextBox();
		StackPanel^ mStackPanel = gcnew StackPanel();
		StackPanel^ hfirstPanel = gcnew StackPanel();
		TextBlock^ hTxtBlock = gcnew TextBlock();
		Label^ hDstIp = gcnew Label();
		Label^ hDstPort = gcnew Label();
		Label^ hpath = gcnew Label();
		Forms::OpenFileDialog^ hFileBrowserDialog = gcnew Forms::OpenFileDialog();
		TextBox^ hListBox = gcnew TextBox();
		Button^ hFileBrowseButton = gcnew Button();
		Grid^ hGrid2 = gcnew Grid();
		Grid^ hgrid2 = gcnew Grid();
		ScrollViewer^ hScrollViewer1 = gcnew ScrollViewer();
		StackPanel^ hStackPanel1 = gcnew StackPanel();
		StackPanel^ hStackPanel2 = gcnew StackPanel();
		StackPanel^ hStackPanel3 = gcnew StackPanel();
		StackPanel^ hstackpanel4 = gcnew StackPanel();

		// Controls for File download view
		Grid^ hFileDownloadGrid = gcnew Grid();		//Filedownload grid
		Button^ hFileDownloadButton = gcnew Button();
		Button^ ClearButton = gcnew Button();
		TextBlock^ displaymsg = gcnew TextBlock();
		TextBox^ DstIpTxtBox = gcnew TextBox();
		TextBox^ DstPortTxtBox = gcnew TextBox();
		StackPanel^ tStackPanel = gcnew StackPanel();
		StackPanel^ firstPanel = gcnew StackPanel();
		TextBlock^ TxtBlock = gcnew TextBlock();
		Label^ DstIp = gcnew Label();
		Label^ DstPort = gcnew Label();
		Label^ path = gcnew Label();
		Forms::OpenFileDialog^ FileBrowserDialog = gcnew Forms::OpenFileDialog();
		TextBox^ listBox = gcnew TextBox();
		Button^ FileBrowseButton = gcnew Button();
		Grid^ Grid2 = gcnew Grid();
		Grid^ grid2 = gcnew Grid();
		ScrollViewer^ ScrollViewer1 = gcnew ScrollViewer();
		StackPanel^ StackPanel1 = gcnew StackPanel();
		StackPanel^ StackPanel2 = gcnew StackPanel();
		StackPanel^ StackPanel3 = gcnew StackPanel();
		StackPanel^ stackpanel4 = gcnew StackPanel();

		// Controls for File Search view
		Forms::FolderBrowserDialog^ hFolderBrowserDialog = gcnew Forms::FolderBrowserDialog();
		Button^ hFolderBrowseButton = gcnew Button();
		Grid^ hFileSearchGrid = gcnew Grid();		//File search grid
		Button^ hFileSearchButton = gcnew Button();
		Button^ fClearButton = gcnew Button();
		TextBox^ fDstIpTxtBox = gcnew TextBox();
		TextBox^ fDstPortTxtBox = gcnew TextBox();
		TextBox^ fPattTxtBox = gcnew TextBox();
		StackPanel^ fStackPanel = gcnew StackPanel();
		StackPanel^ ffirstPanel = gcnew StackPanel();
		TextBlock^ fTxtBlock = gcnew TextBlock();
		Label^ fDstIp = gcnew Label();
		Label^ fDstPort = gcnew Label();
		Label^ fPatt = gcnew Label();
		TextBox^ flistBox = gcnew TextBox();
		Grid^ fGrid2 = gcnew Grid();
		Grid^ fgrid2 = gcnew Grid();
		ScrollViewer^ fScrollViewer1 = gcnew ScrollViewer();
		StackPanel^ fStackPanel1 = gcnew StackPanel();
		StackPanel^ fStackPanel2 = gcnew StackPanel();
		StackPanel^ fStackPanel3 = gcnew StackPanel();
		StackPanel^ fstackpanel4 = gcnew StackPanel();
		StackPanel^ fStackPanel5 = gcnew StackPanel();

		// Controls for Text Search view
		Forms::FolderBrowserDialog^ tFolderBrowserDialog = gcnew Forms::FolderBrowserDialog();
		Button^ tFolderBrowseButton = gcnew Button();
		Grid^ hTextSearchGrid = gcnew Grid();		//Text search grid
		Button^ hTextSearchButton = gcnew Button();
		Button^ tClearButton = gcnew Button();
		TextBox^ tDstIpTxtBox = gcnew TextBox();
		TextBox^ tDstPortTxtBox = gcnew TextBox();
		TextBox^ tPattTxtBox = gcnew TextBox();
		TextBox^ tTextBox = gcnew TextBox();
		StackPanel^ txtStackPanel = gcnew StackPanel();
		StackPanel^ tfirstPanel = gcnew StackPanel();
		TextBlock^ tTxtBlock = gcnew TextBlock();
		Label^ tDstIp = gcnew Label();
		Label^ tDstPort = gcnew Label();
		Label^ tPatt = gcnew Label();
		Label^ tText = gcnew Label();
		TextBox^ tlistBox = gcnew TextBox();
		Grid^ tGrid2 = gcnew Grid();
		Grid^ tgrid2 = gcnew Grid();
		ScrollViewer^ tScrollViewer1 = gcnew ScrollViewer();
		StackPanel^ tStackPanel1 = gcnew StackPanel();
		StackPanel^ tStackPanel2 = gcnew StackPanel();
		StackPanel^ tStackPanel3 = gcnew StackPanel();
		StackPanel^ tstackpanel4 = gcnew StackPanel();
		StackPanel^ tStackPanel5 = gcnew StackPanel();
		StackPanel^ tStackPanel6 = gcnew StackPanel();

		// receive thread
		Thread^ recvThread;

	public:
		WPFCppCliDemo();
		~WPFCppCliDemo();
		void setUpStatusBar();
		void setUpTabControl();
		void setUpFileUploadView();
		void setUpFileDownloadView();
		void setUpFileSearchView();
		void setUpTextSearchView();
		void setUpViewMessage();
		//---------------event handlers----------------------
		void FileUpload(Object^ obj, RoutedEventArgs^ args);
		void FileDownload(Object^obj, RoutedEventArgs^args);
		void FileSearch(Object^obj, RoutedEventArgs^args);
		void TextSearch(Object^obj, RoutedEventArgs^args);
		void clear(Object^ sender, RoutedEventArgs^ args);
		void clearview(Object^ sender, RoutedEventArgs^ args);
		void browseForFolder(Object^ sender, RoutedEventArgs^ args);
		void browseForFolder_txt(Object^ sender, RoutedEventArgs^ args);
		void browseForFile(Object^sender, RoutedEventArgs^args);
		void browseForFileDownload(Object^sender, RoutedEventArgs^args);
		void OnLoaded(Object^ sender, RoutedEventArgs^ args);
		void Unloading(Object^ sender, System::ComponentModel::CancelEventArgs^ args);
		void addText(String^ msg);
		void getMessage();

	private:
		std::string toStdString(String^ pStr);
		String^ toSystemString(std::string& str);
		void setTextBlockProperties();			//For Fileupload view
		void setButtonsProperties();
		void setSelectPathProperties();
		void setTextBoxProperties();
		void setLabelProperties();
		void setDownloadButton();				//For FileDownload view
		void setDownloadTxtBlock();
		void setDownloadTxtBox();
		void setDownloadPath();
		void setDownloadLabel();
		void setDisplayMsgTxtBlock();
		void set_fs_TextBlock();				//For FileSearch view
		void set_fs_Buttons();
		void set_fs_SelectPath();
		void set_fs_TextBox();
		void set_fs_Label();
		void set_txts_TextBlock();				//For TextSearch view
		void set_txts_Buttons();
		void set_txts_SelectPath();
		void set_txts_TextBox();
		void set_txts_Label();
		void set_vm_TextBlock();				//For View message tab
		void set_vm_Buttons();
	};
}


#endif
