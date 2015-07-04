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
*  To run as a Windows Application:
*  - Set Project Properties > Linker > System > Subsystem to Windows
*  - Comment out int main(...) at bottom
*  - Uncomment int _stdcall WinMain() at bottom
*  To run as a Console Application:
*  - Set Project Properties > Linker > System > Subsytem to Console
*  - Uncomment int main(...) at bottom
*  - Comment out int _stdcall WinMain() at bottom
*/
#include "Window.h"
using namespace CppCliWindows;

//----< Constructor  >---------------------------------------

WPFCppCliDemo::WPFCppCliDemo()
{
	// set up channel
	ObjectFactory* pObjFact1 = new ObjectFactory;
	pSendr1_ = pObjFact1->createSendr();
	pRecvr1_ = pObjFact1->createRecvr();
	pChann1_ = pObjFact1->createMockChannel(pSendr1_, pRecvr1_);
	pChann1_->start();
	delete pObjFact1;

	ObjectFactory* pObjFact2 = new ObjectFactory;
	pSendr2_ = pObjFact2->createSendr();
	pRecvr2_ = pObjFact2->createRecvr();
	pChann2_ = pObjFact2->createMockChannel(pSendr2_, pRecvr2_);
	pChann2_->start();
	delete pObjFact2;

	ObjectFactory* pObjFact3 = new ObjectFactory;
	pSendr3_ = pObjFact3->createSendr();
	pRecvr3_ = pObjFact3->createRecvr();
	pChann3_ = pObjFact2->createMockChannel(pSendr3_, pRecvr3_);
	pChann3_->start();
	delete pObjFact3;

	ObjectFactory* pObjFact4 = new ObjectFactory;
	pSendr4_ = pObjFact4->createSendr();
	pRecvr4_ = pObjFact4->createRecvr();
	pChann4_ = pObjFact4->createMockChannel(pSendr4_, pRecvr4_);
	pChann4_->start();
	delete pObjFact4;


	// client's receive thread
	recvThread = gcnew Thread(gcnew ThreadStart(this, &WPFCppCliDemo::getMessage));
	recvThread->Start();
	// set event handlers
	this->Loaded += gcnew System::Windows::RoutedEventHandler(this, &WPFCppCliDemo::OnLoaded);
	this->Closing += gcnew CancelEventHandler(this, &WPFCppCliDemo::Unloading);
	hFileUploadButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::FileUpload);
	hFileDownloadButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::FileDownload);
	hFileSearchButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::FileSearch);
	hTextSearchButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::TextSearch);
	hClearButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::clear);
	ClearButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::clear);
	fClearButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::clear);
	tClearButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::clear);
	vClearButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::clearview);
	//hFolderBrowseButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::browseForFolder);
	//tFolderBrowseButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::browseForFolder_txt);
	hFileBrowseButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::browseForFile);
	FileBrowseButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::browseForFileDownload);
	// set Window properties
	this->Title = "CLIENT - GUI";
	this->Width = 800;
	this->Height = 600;
	// attach dock panel to Window
	this->Content = hDockPanel;
	hDockPanel->Children->Add(hStatusBar);
	hDockPanel->SetDock(hStatusBar, Dock::Bottom);
	hDockPanel->Children->Add(hGrid);
	// setup Window controls and views
	setUpTabControl();
	setUpStatusBar();
	setUpFileUploadView();
	setUpFileDownloadView();
	setUpFileSearchView();
	setUpTextSearchView();
	
}

//----< Destructor  >---------------------------------------

WPFCppCliDemo::~WPFCppCliDemo()
{
	delete pChann1_;
	delete pSendr1_;
	delete pRecvr1_;
	delete pChann2_;
	delete pSendr2_;
	delete pRecvr2_;
	delete pChann3_;
	delete pSendr3_;
	delete pRecvr3_;
	delete pChann4_;
	delete pSendr4_;
	delete pRecvr4_;
}

void WPFCppCliDemo::setUpStatusBar()
{
	hStatusBar->Items->Add(hStatusBarItem);
	hStatus->Text = "very important messages will appear here";
	hStatus->FontWeight = FontWeights::Bold;
	hStatusBarItem->Content = hStatus;
	hStatusBar->Padding = Thickness(10, 2, 10, 2);
}

void WPFCppCliDemo::setUpTabControl()
{
	hGrid->Children->Add(hTabControl);
	hFileUploadTab->Header = "FILE UPLOAD";
	hFileDownloadTab->Header = "FILE DOWNLOAD";
	hFileSearchTab->Header = "FILE SEARCH";
	hTextSearchTab->Header = "TEXT SEARCH";
	//hViewMessageTab->Header = "VIEW MESSAGE";
	hTabControl->Items->Add(hFileUploadTab);
	hTabControl->Items->Add(hFileDownloadTab);
	hTabControl->Items->Add(hFileSearchTab);
	hTabControl->Items->Add(hTextSearchTab);
	//hTabControl->Items->Add(hViewMessageTab);
}

//----< Method to set view message view properties -TextBlock >---------------------------------------

void WPFCppCliDemo::set_vm_TextBlock()
{
	RowDefinition^ hRow1Def = gcnew RowDefinition();
	hViewMessageGrid->RowDefinitions->Add(hRow1Def);
	Border^ hBorder1 = gcnew Border();
	hBorder1->BorderThickness = Thickness(1);
	hBorder1->BorderBrush = Brushes::Black;
	hBorder1->Child = vTextBlock1;
	vTextBlock1->Padding = Thickness(15);
	vTextBlock1->Text = "";
	vTextBlock1->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	vTextBlock1->FontWeight = FontWeights::Bold;
	vTextBlock1->FontSize = 16;
	vScrollViewer1->VerticalScrollBarVisibility = ScrollBarVisibility::Auto;
	vScrollViewer1->Content = hBorder1;
	hViewMessageGrid->SetRow(vScrollViewer1, 0);
	hViewMessageGrid->Children->Add(vScrollViewer1);
}

//----< Method to set view message view properties - Buttons  >---------------------------------------

void WPFCppCliDemo::set_vm_Buttons()
{
	RowDefinition^ hRow2Def = gcnew RowDefinition();
	hRow2Def->Height = GridLength(75);
	hViewMessageGrid->RowDefinitions->Add(hRow2Def);
	vClearButton->Content = "Clear";
	Border^ hBorder3 = gcnew Border();
	hBorder3->Width = 120;
	hBorder3->Height = 30;
	hBorder3->BorderThickness = Thickness(1);
	hBorder3->BorderBrush = Brushes::Black;
	hBorder3->Child = vClearButton;
	vStackPanel1->Children->Add(hBorder3);

	vStackPanel1->Orientation = Orientation::Horizontal;
	vStackPanel1->HorizontalAlignment = System::Windows::HorizontalAlignment::Center;
	hViewMessageGrid->SetRow(vStackPanel1, 1);
	hViewMessageGrid->Children->Add(vStackPanel1);
}

//----< Method to set FileUpload view properties - TextBlock  >---------------------------------------

void WPFCppCliDemo::setTextBlockProperties()
{
	RowDefinition^ hRow1Def = gcnew RowDefinition();
	hFileUploadGrid->RowDefinitions->Add(hRow1Def);
	Border^ hBorder1 = gcnew Border();
	hBorder1->BorderThickness = Thickness(1);
	hBorder1->BorderBrush = Brushes::Black;
	hBorder1->Child = hgrid2;
	hgrid2->SetRow(hfirstPanel, 1);
	hgrid2->Children->Add(hfirstPanel);
	hfirstPanel->Children->Add(mStackPanel);
	TextBlock^ htSpacer = gcnew TextBlock();
	htSpacer->Width = 15;
	hfirstPanel->Children->Add(htSpacer);
	Border^ hBorder12 = gcnew Border();
	hBorder12->BorderThickness = Thickness(1);
	hBorder12->BorderBrush = Brushes::Black;
	hBorder12->Child = hTxtBlock;
	hTxtBlock->Padding = Thickness(15);
	hTxtBlock->Width = 240;
	hTxtBlock->Text = "";
	hTxtBlock->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	hTxtBlock->FontSize = 14;
	ScrollViewer^ htScrollViewer = gcnew ScrollViewer();
	htScrollViewer->VerticalScrollBarVisibility = ScrollBarVisibility::Auto;
	htScrollViewer->Content = hTxtBlock;
	hfirstPanel->Children->Add(hBorder12);
	hfirstPanel->Orientation = Orientation::Horizontal;
	hfirstPanel->HorizontalAlignment = System::Windows::HorizontalAlignment::Center;
	hScrollViewer1->VerticalScrollBarVisibility = ScrollBarVisibility::Auto;
	hScrollViewer1->Content = hBorder1;
	hFileUploadGrid->SetRow(hScrollViewer1, 0);
	hFileUploadGrid->Children->Add(hScrollViewer1);
	TextBlock^ hSpacer = gcnew TextBlock();
	hSpacer->Width = 20;
	hStackPanel2->Children->Add(hDstIp);
	hStackPanel2->Children->Add(hSpacer);
	hStackPanel2->Children->Add(hDstIpTxtBox);
	hStackPanel2->Orientation = Orientation::Horizontal;
	hStackPanel2->HorizontalAlignment = System::Windows::HorizontalAlignment::Center;
	mStackPanel->Children->Add(hStackPanel2);
	TextBlock^ hSpacer3 = gcnew TextBlock();
	hSpacer3->Width = 20;
	hStackPanel3->Children->Add(hDstPort);
	hStackPanel2->Children->Add(hSpacer3);
	hStackPanel3->Children->Add(hDstPortTxtBox);
	hStackPanel3->Orientation = Orientation::Horizontal;
	hStackPanel3->HorizontalAlignment = System::Windows::HorizontalAlignment::Center;
	TextBlock^ spacer = gcnew TextBlock();
	spacer->Height = 50;
	mStackPanel->Children->Add(spacer);
	mStackPanel->Children->Add(hStackPanel3);
	mStackPanel->Orientation = Orientation::Vertical;
	mStackPanel->VerticalAlignment = System::Windows::VerticalAlignment::Center;
}

//----< Method to set FileUpload view properties - TextBox >---------------------------------------

void WPFCppCliDemo::setTextBoxProperties()
{
	hDstIpTxtBox->Padding = Thickness(5);
	hDstIpTxtBox->Text = "";
	hDstIpTxtBox->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	// hDstIpTxtBox->FontWeight = FontWeights::Bold;
	hDstIpTxtBox->FontSize = 16;
	hDstIpTxtBox->Width = 240;
	hDstIpTxtBox->Height = 50;
	hDstPortTxtBox->Padding = Thickness(10);
	hDstPortTxtBox->Text = "";
	hDstPortTxtBox->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	//hDstPortTxtBox->FontWeight = FontWeights::Bold;
	hDstPortTxtBox->FontSize = 16;
	hDstPortTxtBox->Width = 240;
	hDstPortTxtBox->Height = 50;
}

//----< Method to set FileUpload view properties - Label >---------------------------------------

void WPFCppCliDemo::setLabelProperties()
{
	hDstIp->Content = "Enter Dest IP";
	hDstIp->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	hDstIp->FontWeight = FontWeights::Bold;
	hDstIp->FontSize = 16;
	hDstIp->Width = 200;
	hDstIp->Height = 30;
	hDstPort->Content = "Enter Dest Port";
	hDstPort->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	hDstPort->FontWeight = FontWeights::Bold;
	hDstPort->FontSize = 16;
	hDstPort->Width = 200;
	hDstPort->Height = 30;
}

//----< Method to set FileUpload view properties - Buttons >---------------------------------------

void WPFCppCliDemo::setButtonsProperties()
{
	RowDefinition^ hRow2Def = gcnew RowDefinition();
	hRow2Def->Height = GridLength(75);
	hFileUploadGrid->RowDefinitions->Add(hRow2Def);
	hFileUploadButton->Content = "UPLOAD";
	Border^ hBorder2 = gcnew Border();
	hBorder2->Width = 120;
	hBorder2->Height = 30;
	hBorder2->BorderThickness = Thickness(1);
	hBorder2->BorderBrush = Brushes::Black;
	hClearButton->Content = "Clear";
	hBorder2->Child = hFileUploadButton;
	Border^ hBorder3 = gcnew Border();
	hBorder3->Width = 120;
	hBorder3->Height = 30;
	hBorder3->BorderThickness = Thickness(1);
	hBorder3->BorderBrush = Brushes::Black;
	hBorder3->Child = hClearButton;
	hStackPanel1->Children->Add(hBorder2);
	TextBlock^ hSpacer = gcnew TextBlock();
	hSpacer->Width = 10;
	hStackPanel1->Children->Add(hSpacer);
	hStackPanel1->Children->Add(hBorder3);
	hStackPanel1->Orientation = Orientation::Horizontal;
	hStackPanel1->HorizontalAlignment = System::Windows::HorizontalAlignment::Center;
	hFileUploadGrid->SetRow(hStackPanel1, 1);
	hFileUploadGrid->Children->Add(hStackPanel1);
}

//----< Method to set FileUpload view properties - BrowseFile >---------------------------------------

void WPFCppCliDemo::setSelectPathProperties()
{

	TextBlock^ space = gcnew TextBlock();
	space->Width = 20;
	hFileBrowseButton->Content = "Select File";
	hFileBrowseButton->Height = 30;
	hFileBrowseButton->Width = 200;
	hFileBrowseButton->FontSize = 16;
	hFileBrowseButton->BorderThickness = Thickness(2);
	hFileBrowseButton->BorderBrush = Brushes::Black;
	hListBox->Height = 50;
	hListBox->Width = 240;
	hstackpanel4->Children->Add(hFileBrowseButton);
	hstackpanel4->Children->Add(space);
	hstackpanel4->Children->Add(hListBox);
	TextBlock^ spacer1 = gcnew TextBlock();
	spacer1->Height = 50;
	mStackPanel->Children->Add(spacer1);
	mStackPanel->Children->Add(hstackpanel4);
	hstackpanel4->Orientation = Orientation::Horizontal;
	hstackpanel4->HorizontalAlignment = System::Windows::HorizontalAlignment::Center;

}

//----< Method to set FileDownload view properties - Button >---------------------------------------

void WPFCppCliDemo::setDownloadButton()
{
	RowDefinition^ Row2Def = gcnew RowDefinition();
	Row2Def->Height = GridLength(75);
	hFileDownloadGrid->RowDefinitions->Add(Row2Def);
	hFileDownloadButton->Content = "DOWNLOAD";
	Border^ Border2 = gcnew Border();
	Border2->Width = 120;
	Border2->Height = 30;
	Border2->BorderThickness = Thickness(1);
	Border2->BorderBrush = Brushes::Black;
	ClearButton->Content = "Clear";
	Border2->Child = hFileDownloadButton;
	Border^ Border3 = gcnew Border();
	Border3->Width = 120;
	Border3->Height = 30;
	Border3->BorderThickness = Thickness(1);
	Border3->BorderBrush = Brushes::Black;
	Border3->Child = ClearButton;
	StackPanel1->Children->Add(Border2);
	TextBlock^ hSpacer = gcnew TextBlock();
	hSpacer->Width = 10;
	StackPanel1->Children->Add(hSpacer);
	StackPanel1->Children->Add(Border3);
	StackPanel1->Orientation = Orientation::Horizontal;
	StackPanel1->HorizontalAlignment = System::Windows::HorizontalAlignment::Center;
	hFileDownloadGrid->SetRow(StackPanel1, 1);
	hFileDownloadGrid->Children->Add(StackPanel1);
}

//----< Method to set FileDownload view properties - TextBlock >---------------------------------------

void WPFCppCliDemo::setDownloadTxtBlock()
{
	RowDefinition^ hRow1Def = gcnew RowDefinition();
	hFileDownloadGrid->RowDefinitions->Add(hRow1Def);
	Border^ Border1 = gcnew Border();
	Border1->BorderThickness = Thickness(1);
	Border1->BorderBrush = Brushes::Black;
	Border1->Child = grid2;
	grid2->SetRow(firstPanel, 1);
	grid2->Children->Add(firstPanel);
	firstPanel->Children->Add(tStackPanel);
	TextBlock^ htSpacer = gcnew TextBlock();
	htSpacer->Width = 15;
	firstPanel->Children->Add(htSpacer);
	Border^ hBorder12 = gcnew Border();
	hBorder12->BorderThickness = Thickness(1);
	hBorder12->BorderBrush = Brushes::Black;
	hBorder12->Child = TxtBlock;
	TxtBlock->Padding = Thickness(15);
	TxtBlock->Width = 240;
	TxtBlock->Text = "";
	TxtBlock->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	TxtBlock->FontSize = 14;
	ScrollViewer^ htScrollViewer = gcnew ScrollViewer();
	htScrollViewer->VerticalScrollBarVisibility = ScrollBarVisibility::Auto;
	htScrollViewer->Content = TxtBlock;
	firstPanel->Children->Add(hBorder12);
	firstPanel->Orientation = Orientation::Horizontal;
	firstPanel->HorizontalAlignment = System::Windows::HorizontalAlignment::Center;
	ScrollViewer1->VerticalScrollBarVisibility = ScrollBarVisibility::Auto;
	ScrollViewer1->Content = Border1;
	hFileDownloadGrid->SetRow(ScrollViewer1, 0);
	hFileDownloadGrid->Children->Add(ScrollViewer1);
	TextBlock^ hSpacer = gcnew TextBlock();
	hSpacer->Width = 10;
	StackPanel2->Children->Add(DstIp);
	StackPanel2->Children->Add(hSpacer);
	StackPanel2->Children->Add(DstIpTxtBox);
	StackPanel2->Orientation = Orientation::Horizontal;
	StackPanel2->HorizontalAlignment = System::Windows::HorizontalAlignment::Center;
	tStackPanel->Children->Add(StackPanel2);
	TextBlock^ hSpacer3 = gcnew TextBlock();
	hSpacer3->Width = 10;
	StackPanel3->Children->Add(DstPort);
	StackPanel2->Children->Add(hSpacer3);
	StackPanel3->Children->Add(DstPortTxtBox);
	StackPanel3->Orientation = Orientation::Horizontal;
	StackPanel3->HorizontalAlignment = System::Windows::HorizontalAlignment::Center;
	TextBlock^ spacer = gcnew TextBlock();
	spacer->Height = 50;
	tStackPanel->Children->Add(spacer);
	tStackPanel->Children->Add(StackPanel3);
	tStackPanel->Orientation = Orientation::Vertical;
	tStackPanel->VerticalAlignment = System::Windows::VerticalAlignment::Center;
}

//----< Method to set FileDownload view properties - TextBox >---------------------------------------

void WPFCppCliDemo::setDownloadTxtBox()
{
	DstIpTxtBox->Padding = Thickness(5);
	DstIpTxtBox->Text = "";
	DstIpTxtBox->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	// DstIpTxtBox->FontWeight = FontWeights::Bold;
	DstIpTxtBox->FontSize = 16;
	DstIpTxtBox->Width = 240;
	DstIpTxtBox->Height = 50;
	DstPortTxtBox->Padding = Thickness(10);
	DstPortTxtBox->Text = "";
	DstPortTxtBox->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	//DstPortTxtBox->FontWeight = FontWeights::Bold;
	DstPortTxtBox->FontSize = 16;
	DstPortTxtBox->Width = 240;
	DstPortTxtBox->Height = 50;
}

//----< Method to set FileDownload view properties - Browse File >---------------------------------------

void WPFCppCliDemo::setDownloadPath()
{
	TextBlock^ space = gcnew TextBlock();
	space->Width = 20;
	FileBrowseButton->Content = "Select File";
	FileBrowseButton->FontSize = 16;
	FileBrowseButton->Height = 50;
	FileBrowseButton->Width = 200;
	FileBrowseButton->BorderThickness = Thickness(2);
	FileBrowseButton->BorderBrush = Brushes::Black;
	listBox->Height = 50;
	listBox->Width = 240;
	stackpanel4->Children->Add(FileBrowseButton);
	stackpanel4->Children->Add(space);
	stackpanel4->Children->Add(listBox);
	TextBlock^ spacer1 = gcnew TextBlock();
	spacer1->Height = 50;
	tStackPanel->Children->Add(spacer1);
	tStackPanel->Children->Add(stackpanel4);
	stackpanel4->Orientation = Orientation::Horizontal;
	stackpanel4->HorizontalAlignment = System::Windows::HorizontalAlignment::Center;
}

//----< Method to set FileDownload view properties - Label >---------------------------------------

void WPFCppCliDemo::setDownloadLabel()
{
	DstIp->Content = "Enter Dest IP";
	DstIp->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	DstIp->FontWeight = FontWeights::Bold;
	DstIp->FontSize = 16;
	DstIp->Width = 200;
	DstIp->Height = 30;
	DstPort->Content = "Enter Dest Port";
	DstPort->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	DstPort->FontWeight = FontWeights::Bold;
	DstPort->FontSize = 16;
	DstPort->Width = 200;
	DstPort->Height = 30;
}

//----< Method to Display messages >---------------------------------------

void WPFCppCliDemo::setDisplayMsgTxtBlock()
{
	Border^ hBorder1 = gcnew Border();
	hBorder1->BorderThickness = Thickness(1);
	hBorder1->BorderBrush = Brushes::Black;
	hBorder1->Child = displaymsg;
	displaymsg->Padding = Thickness(15);
	displaymsg->Text = "";
	displaymsg->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	displaymsg->FontWeight = FontWeights::Bold;
	displaymsg->FontSize = 14;
	hFileDownloadGrid->SetRow(hBorder1, 2);
	hFileDownloadGrid->Children->Add(hBorder1);
	
}

//----< Method to set FileSearch view properties - TextBlock >---------------------------------------

void WPFCppCliDemo::set_fs_TextBlock()
{
	RowDefinition^ hRow1Def = gcnew RowDefinition();
	hFileSearchGrid->RowDefinitions->Add(hRow1Def);
	Border^ hBorder1 = gcnew Border();
	hBorder1->BorderThickness = Thickness(1);
	hBorder1->BorderBrush = Brushes::Black;
	hBorder1->Child = fgrid2;
	fgrid2->SetRow(ffirstPanel, 1);
	fgrid2->Children->Add(ffirstPanel);
	ffirstPanel->Children->Add(fStackPanel);
	TextBlock^ htSpacer = gcnew TextBlock();
	htSpacer->Width = 15;
	ffirstPanel->Children->Add(htSpacer);
	Border^ hBorder12 = gcnew Border();
	hBorder12->BorderThickness = Thickness(1);
	hBorder12->BorderBrush = Brushes::Black;
	hBorder12->Child = fTxtBlock;
	fTxtBlock->Padding = Thickness(15);
	fTxtBlock->Width = 240;
	fTxtBlock->Text = "";
	fTxtBlock->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	fTxtBlock->FontSize = 14;
	ScrollViewer^ htScrollViewer = gcnew ScrollViewer();
	htScrollViewer->VerticalScrollBarVisibility = ScrollBarVisibility::Auto;
	htScrollViewer->Content = fTxtBlock;
	ffirstPanel->Children->Add(hBorder12);
	ffirstPanel->Orientation = Orientation::Horizontal;
	ffirstPanel->HorizontalAlignment = System::Windows::HorizontalAlignment::Center;
	fScrollViewer1->VerticalScrollBarVisibility = ScrollBarVisibility::Auto;
	fScrollViewer1->Content = hBorder1;
	hFileSearchGrid->SetRow(fScrollViewer1, 0);
	hFileSearchGrid->Children->Add(fScrollViewer1);
	TextBlock^ hSpacer = gcnew TextBlock();
	hSpacer->Width = 10;
	fStackPanel2->Children->Add(fDstIp);
	fStackPanel2->Children->Add(hSpacer);
	fStackPanel2->Children->Add(fDstIpTxtBox);
	fStackPanel2->Orientation = Orientation::Horizontal;
	fStackPanel2->HorizontalAlignment = System::Windows::HorizontalAlignment::Center;
	fStackPanel->Children->Add(fStackPanel2);
	TextBlock^ hSpacer3 = gcnew TextBlock();
	hSpacer3->Width = 10;
	fStackPanel3->Children->Add(fDstPort);
	fStackPanel2->Children->Add(hSpacer3);
	fStackPanel3->Children->Add(fDstPortTxtBox);
	fStackPanel3->Orientation = Orientation::Horizontal;
	fStackPanel3->HorizontalAlignment = System::Windows::HorizontalAlignment::Center;
	TextBlock^ spacer = gcnew TextBlock();
	spacer->Height = 50;
	fStackPanel->Children->Add(spacer);
	fStackPanel->Children->Add(fStackPanel3);
	fStackPanel->Orientation = Orientation::Vertical;
	fStackPanel->VerticalAlignment = System::Windows::VerticalAlignment::Center;
}

//----< Method to set FileSearch view properties - Button >---------------------------------------

void WPFCppCliDemo::set_fs_Buttons()
{
	RowDefinition^ hRow2Def = gcnew RowDefinition();
	hRow2Def->Height = GridLength(75);
	hFileSearchGrid->RowDefinitions->Add(hRow2Def);
	hFileSearchButton->Content = "FILE_SEARCH";
	Border^ hBorder2 = gcnew Border();
	hBorder2->Width = 120;
	hBorder2->Height = 30;
	hBorder2->BorderThickness = Thickness(1);
	hBorder2->BorderBrush = Brushes::Black;
	fClearButton->Content = "Clear";
	hBorder2->Child = hFileSearchButton;
	Border^ hBorder3 = gcnew Border();
	hBorder3->Width = 120;
	hBorder3->Height = 30;
	hBorder3->BorderThickness = Thickness(1);
	hBorder3->BorderBrush = Brushes::Black;
	hBorder3->Child = fClearButton;
	fStackPanel1->Children->Add(hBorder2);
	TextBlock^ hSpacer = gcnew TextBlock();
	hSpacer->Width = 10;
	fStackPanel1->Children->Add(hSpacer);
	fStackPanel1->Children->Add(hBorder3);
	fStackPanel1->Orientation = Orientation::Horizontal;
	fStackPanel1->HorizontalAlignment = System::Windows::HorizontalAlignment::Center;
	hFileSearchGrid->SetRow(fStackPanel1, 1);
	hFileSearchGrid->Children->Add(fStackPanel1);
}

//----< Method to set FileSearch view properties - Browse for path selection >---------------------------------------

void WPFCppCliDemo::set_fs_SelectPath()
{
	TextBlock^ space = gcnew TextBlock();
	space->Width = 20;
	hFolderBrowseButton->Content = "Enter Category";
	hFolderBrowseButton->Height = 50;
	hFolderBrowseButton->Width = 200;
	hFolderBrowseButton->BorderThickness = Thickness(2);
	hFolderBrowseButton->BorderBrush = Brushes::Black;
	flistBox->Height = 50;
	flistBox->Width = 240;
	hFolderBrowserDialog->ShowNewFolderButton = false;
	hFolderBrowserDialog->SelectedPath = System::IO::Directory::GetCurrentDirectory();
	fstackpanel4->Children->Add(hFolderBrowseButton);
	fstackpanel4->Children->Add(space);
	fstackpanel4->Children->Add(flistBox);
	TextBlock^ spacer1 = gcnew TextBlock();
	spacer1->Height = 50;
	fStackPanel->Children->Add(spacer1);
	fStackPanel->Children->Add(fstackpanel4);
	fstackpanel4->Orientation = Orientation::Horizontal;
	fstackpanel4->HorizontalAlignment = System::Windows::HorizontalAlignment::Center;
	TextBlock^ spacer2 = gcnew TextBlock();
	spacer2->Width = 20;
	fStackPanel5->Children->Add(fPatt);
	fStackPanel5->Children->Add(spacer2);
	fStackPanel5->Children->Add(fPattTxtBox);
	fStackPanel5->Orientation = Orientation::Horizontal;
	fStackPanel5->HorizontalAlignment = System::Windows::HorizontalAlignment::Center;
	TextBlock^ spacer12 = gcnew TextBlock();
	spacer12->Height = 50;
	fStackPanel->Children->Add(spacer12);
	fStackPanel->Children->Add(fStackPanel5);
}

//----< Method to set FileSearch view properties - TextBox >---------------------------------------

void WPFCppCliDemo::set_fs_TextBox()
{
	fDstIpTxtBox->Padding = Thickness(5);
	fDstIpTxtBox->Text = "";
	fDstIpTxtBox->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	// fDstIpTxtBox->FontWeight = FontWeights::Bold;
	fDstIpTxtBox->FontSize = 16;
	fDstIpTxtBox->Width = 240;
	fDstIpTxtBox->Height = 30;
	fDstPortTxtBox->Padding = Thickness(10);
	fDstPortTxtBox->Text = "";
	fDstPortTxtBox->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	//fDstPortTxtBox->FontWeight = FontWeights::Bold;
	fDstPortTxtBox->FontSize = 16;
	fDstPortTxtBox->Width = 240;
	fDstPortTxtBox->Height = 50;
	fPattTxtBox->Padding = Thickness(10);
	fPattTxtBox->Text = "";
	fPattTxtBox->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	//fDstPortTxtBox->FontWeight = FontWeights::Bold;
	fPattTxtBox->FontSize = 16;
	fPattTxtBox->Width = 240;
	fPattTxtBox->Height = 50;
}

//----< Method to set FileSearch view properties - Label >---------------------------------------

void WPFCppCliDemo::set_fs_Label()
{
	fDstIp->Content = "Enter Dest IP";
	fDstIp->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	fDstIp->FontWeight = FontWeights::Bold;
	fDstIp->FontSize = 16;
	fDstIp->Width = 200;
	fDstIp->Height = 30;
	fDstPort->Content = "Enter Dest Port";
	fDstPort->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	fDstPort->FontWeight = FontWeights::Bold;
	fDstPort->FontSize = 16;
	fDstPort->Width = 200;
	fDstPort->Height = 30;
	fPatt->Content = "Enter FilePatterns";
	fPatt->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	fPatt->FontWeight = FontWeights::Bold;
	fPatt->FontSize = 16;
	fPatt->Width = 200;
	fPatt->Height = 30;
}

//----< Method to set TextSearch view properties - TextBlock >---------------------------------------

void WPFCppCliDemo::set_txts_TextBlock()
{
	RowDefinition^ hRow1Def = gcnew RowDefinition();
	hTextSearchGrid->RowDefinitions->Add(hRow1Def);
	Border^ hBorder1 = gcnew Border();
	hBorder1->BorderThickness = Thickness(1);
	hBorder1->BorderBrush = Brushes::Black;
	hBorder1->Child = tgrid2;
	tgrid2->SetRow(tfirstPanel, 1);
	tgrid2->Children->Add(tfirstPanel);
	tfirstPanel->Children->Add(txtStackPanel);
	TextBlock^ htSpacer = gcnew TextBlock();
	htSpacer->Width = 15;
	tfirstPanel->Children->Add(htSpacer);
	Border^ hBorder12 = gcnew Border();
	hBorder12->BorderThickness = Thickness(1);
	hBorder12->BorderBrush = Brushes::Black;
	hBorder12->Child = tTxtBlock;
	tTxtBlock->Padding = Thickness(15);
	tTxtBlock->Width = 240;
	tTxtBlock->Text = "";
	tTxtBlock->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	tTxtBlock->FontSize = 14;
	ScrollViewer^ htScrollViewer = gcnew ScrollViewer();
	htScrollViewer->VerticalScrollBarVisibility = ScrollBarVisibility::Auto;
	htScrollViewer->Content = tTxtBlock;
	tfirstPanel->Children->Add(hBorder12);
	tfirstPanel->Orientation = Orientation::Horizontal;
	tfirstPanel->HorizontalAlignment = System::Windows::HorizontalAlignment::Center;
	tScrollViewer1->VerticalScrollBarVisibility = ScrollBarVisibility::Auto;
	tScrollViewer1->Content = hBorder1;
	hTextSearchGrid->SetRow(tScrollViewer1, 0);
	hTextSearchGrid->Children->Add(tScrollViewer1);
	TextBlock^ hSpacer = gcnew TextBlock();
	hSpacer->Width = 10;
	tStackPanel2->Children->Add(tDstIp);
	tStackPanel2->Children->Add(hSpacer);
	tStackPanel2->Children->Add(tDstIpTxtBox);
	tStackPanel2->Orientation = Orientation::Horizontal;
	tStackPanel2->HorizontalAlignment = System::Windows::HorizontalAlignment::Center;
	txtStackPanel->Children->Add(tStackPanel2);
	TextBlock^ hSpacer3 = gcnew TextBlock();
	hSpacer3->Width = 10;
	tStackPanel3->Children->Add(tDstPort);
	tStackPanel2->Children->Add(hSpacer3);
	tStackPanel3->Children->Add(tDstPortTxtBox);
	tStackPanel3->Orientation = Orientation::Horizontal;
	tStackPanel3->HorizontalAlignment = System::Windows::HorizontalAlignment::Center;
	TextBlock^ spacer = gcnew TextBlock();
	spacer->Height = 30;
	txtStackPanel->Children->Add(spacer);
	txtStackPanel->Children->Add(tStackPanel3);
	txtStackPanel->Orientation = Orientation::Vertical;
	txtStackPanel->VerticalAlignment = System::Windows::VerticalAlignment::Center;
}

//----< Method to set TextSearch view properties - Buttons >---------------------------------------

void WPFCppCliDemo::set_txts_Buttons()
{
	RowDefinition^ hRow2Def = gcnew RowDefinition();
	hRow2Def->Height = GridLength(75);
	hTextSearchGrid->RowDefinitions->Add(hRow2Def);
	hTextSearchButton->Content = "TEXT_SEARCH";
	Border^ hBorder2 = gcnew Border();
	hBorder2->Width = 120;
	hBorder2->Height = 30;
	hBorder2->BorderThickness = Thickness(1);
	hBorder2->BorderBrush = Brushes::Black;
	tClearButton->Content = "Clear";
	hBorder2->Child = hTextSearchButton;
	Border^ hBorder3 = gcnew Border();
	hBorder3->Width = 120;
	hBorder3->Height = 30;
	hBorder3->BorderThickness = Thickness(1);
	hBorder3->BorderBrush = Brushes::Black;
	hBorder3->Child = tClearButton;
	tStackPanel1->Children->Add(hBorder2);
	TextBlock^ hSpacer = gcnew TextBlock();
	hSpacer->Width = 10;
	tStackPanel1->Children->Add(hSpacer);
	tStackPanel1->Children->Add(hBorder3);
	tStackPanel1->Orientation = Orientation::Horizontal;
	tStackPanel1->HorizontalAlignment = System::Windows::HorizontalAlignment::Center;
	hTextSearchGrid->SetRow(tStackPanel1, 1);
	hTextSearchGrid->Children->Add(tStackPanel1);
}

//----< Method to set TextSearch view properties - selecting path >---------------------------------------

void WPFCppCliDemo::set_txts_SelectPath()
{
	TextBlock^ space = gcnew TextBlock();
	space->Width = 20;
	tFolderBrowseButton->Content = "Server Path";
	tFolderBrowseButton->Height = 50;
	tFolderBrowseButton->Width = 200;
	tFolderBrowseButton->BorderThickness = Thickness(2);
	tFolderBrowseButton->BorderBrush = Brushes::Black;
	tlistBox->Height = 50;
	tlistBox->Width = 240;
	tlistBox->Text = "../Server";
	tFolderBrowserDialog->ShowNewFolderButton = false;
	tFolderBrowserDialog->SelectedPath = System::IO::Directory::GetCurrentDirectory();
	tstackpanel4->Children->Add(tFolderBrowseButton);
	tstackpanel4->Children->Add(space);
	tstackpanel4->Children->Add(tlistBox);
	TextBlock^ spacer1 = gcnew TextBlock();
	spacer1->Height = 30;
	txtStackPanel->Children->Add(spacer1);
	txtStackPanel->Children->Add(tstackpanel4);
	tstackpanel4->Orientation = Orientation::Horizontal;
	tstackpanel4->HorizontalAlignment = System::Windows::HorizontalAlignment::Center;
	TextBlock^ spacer2 = gcnew TextBlock();
	spacer2->Width = 20;
	tStackPanel5->Children->Add(tPatt);
	tStackPanel5->Children->Add(spacer2);
	tStackPanel5->Children->Add(tPattTxtBox);
	tStackPanel5->Orientation = Orientation::Horizontal;
	tStackPanel5->HorizontalAlignment = System::Windows::HorizontalAlignment::Center;
	TextBlock^ spacer12 = gcnew TextBlock();
	spacer12->Height = 30;
	txtStackPanel->Children->Add(spacer12);
	txtStackPanel->Children->Add(tStackPanel5);
	TextBlock^ spacer3 = gcnew TextBlock();
	spacer3->Width = 20;
	tStackPanel6->Children->Add(tText);
	tStackPanel6->Children->Add(spacer3);
	tStackPanel6->Children->Add(tTextBox);
	tStackPanel6->Orientation = Orientation::Horizontal;
	tStackPanel6->HorizontalAlignment = System::Windows::HorizontalAlignment::Center;
	TextBlock^ spacer13 = gcnew TextBlock();
	spacer12->Height = 30;
	txtStackPanel->Children->Add(spacer13);
	txtStackPanel->Children->Add(tStackPanel6);
}

//----< Method to set TextSearch view properties - TextBox >---------------------------------------

void WPFCppCliDemo::set_txts_TextBox()
{
	tDstIpTxtBox->Padding = Thickness(5);
	tDstIpTxtBox->Text = "";
	tDstIpTxtBox->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	tDstIpTxtBox->FontSize = 16;
	tDstIpTxtBox->Width = 240;
	tDstIpTxtBox->Height = 30;
	tDstPortTxtBox->Padding = Thickness(10);
	tDstPortTxtBox->Text = "";
	tDstPortTxtBox->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	tDstPortTxtBox->FontSize = 16;
	tDstPortTxtBox->Width = 240;
	tDstPortTxtBox->Height = 50;
	tPattTxtBox->Padding = Thickness(10);
	tPattTxtBox->Text = "";
	tPattTxtBox->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	tPattTxtBox->FontSize = 16;
	tPattTxtBox->Width = 240;
	tPattTxtBox->Height = 50;
	tTextBox->Padding = Thickness(10);
	tTextBox->Text = "";
	tTextBox->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	tTextBox->FontSize = 16;
	tTextBox->Width = 240;
	tTextBox->Height = 50;
}

//----< Method to set TextSearch view properties - Label >---------------------------------------

void WPFCppCliDemo::set_txts_Label()
{
	tDstIp->Content = "Enter Dest IP(s)";
	tDstIp->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	tDstIp->FontWeight = FontWeights::Bold;
	tDstIp->FontSize = 16;
	tDstIp->Width = 200;
	tDstIp->Height = 30;
	tDstPort->Content = "Enter Dest Port(s)";
	tDstPort->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	tDstPort->FontWeight = FontWeights::Bold;
	tDstPort->FontSize = 16;
	tDstPort->Width = 200;
	tDstPort->Height = 30;
	tPatt->Content = "Enter FilePatterns";
	tPatt->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	tPatt->FontWeight = FontWeights::Bold;
	tPatt->FontSize = 16;
	tPatt->Width = 200;
	tPatt->Height = 30;
	tText->Content = "Enter Text";
	tText->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	tText->FontWeight = FontWeights::Bold;
	tText->FontSize = 16;
	tText->Width = 200;
	tText->Height = 30;
}

//----< Method to set FileUpload view  >---------------------------------------

void WPFCppCliDemo::setUpFileUploadView()
{
	Console::Write("\n  setting up FileUpload view");
	hFileUploadGrid->Margin = Thickness(20);
	hFileUploadTab->Content = hFileUploadGrid;
	// setUploadTabProperties();
	setTextBoxProperties();
	setLabelProperties();
	setTextBlockProperties();
	setButtonsProperties();
	setSelectPathProperties();
}

//----< Method to set FileDownload view >---------------------------------------

void WPFCppCliDemo::setUpFileDownloadView()
{
	Console::Write("\n  setting up Filedownload view");
	hFileDownloadGrid->Margin = Thickness(20);
	hFileDownloadTab->Content = hFileDownloadGrid;
	setDownloadTxtBox();
	setDownloadLabel();
	setDownloadTxtBlock();
	setDownloadButton();
	setDownloadPath();
	//setDisplayMsgTxtBlock();
	//setDownloadTabProperties();
}

//----< Method to set FileSearch view  >---------------------------------------

void WPFCppCliDemo::setUpFileSearchView()
{
	Console::Write("\n  setting up File search view");
	hFileSearchGrid->Margin = Thickness(20);
	hFileSearchTab->Content = hFileSearchGrid;
	set_fs_TextBox();
	set_fs_Label();
	set_fs_TextBlock();
	set_fs_Buttons();
	set_fs_SelectPath();
}

//----< Method to set TextSearch view  >---------------------------------------

void WPFCppCliDemo::setUpTextSearchView()
{
	Console::Write("\n  setting up Text search view");
	hTextSearchGrid->Margin = Thickness(20);
	hTextSearchTab->Content = hTextSearchGrid;
	set_txts_TextBox();
	set_txts_Label();
	set_txts_TextBlock();
	set_txts_Buttons();
	set_txts_SelectPath();
}

//----< Method to set ViewMessage view  >---------------------------------------

void WPFCppCliDemo::setUpViewMessage()
{
	Console::Write("\n  setting up ViewMessage view");
	hViewMessageGrid->Margin = Thickness(20);
	hViewMessageTab->Content = hViewMessageGrid;
	set_vm_TextBlock();
	set_vm_Buttons();
}

std::string WPFCppCliDemo::toStdString(String^ pStr)
{
	std::string dst;
	for (int i = 0; i < pStr->Length; ++i)
		dst += (char)pStr[i];
	return dst;
}
String^ WPFCppCliDemo::toSystemString(std::string& str)
{
	StringBuilder^ pStr = gcnew StringBuilder();
	for (size_t i = 0; i < str.size(); ++i)
		pStr->Append((Char)str[i]);
	return pStr->ToString();
}

void WPFCppCliDemo::addText(String^ msg)
{
	vTextBlock1->Text = "Received Message : \n";
	vTextBlock1->Text += msg + "\n\n";
}

void WPFCppCliDemo::getMessage()
{
	// recvThread runs this function

	while (true)
	{
		std::cout << "\n  receive thread calling getMessage()";
		std::string msg1 = pRecvr1_->getMessage();
		std::string msg2 = pRecvr2_->getMessage();
		std::string msg3 = pRecvr3_->getMessage();
		std::string msg4 = pRecvr4_->getMessage();
		String^ sMsg1 = toSystemString(msg1);
		String^ sMsg2 = toSystemString(msg2);
		String^ sMsg3 = toSystemString(msg3);
		String^ sMsg4 = toSystemString(msg4);

		array<String^>^ args = gcnew array<String^>(1);
		args[0] = sMsg1;
		args[1] = sMsg2;
		args[2] = sMsg3;
		args[3] = sMsg4;

		Action<String^>^ act = gcnew Action<String^>(this, &WPFCppCliDemo::addText);
		Dispatcher->Invoke(act, args);  // must call addText on main UI thread
	}
}

//----< Event Handler for FileUpload  >---------------------------------------

void WPFCppCliDemo::FileUpload(Object^ obj, RoutedEventArgs^ args)
{
	Console::Write("\n Sending File Upload Message.. \n");
	hStatus->Text = "File uploading..";
	Message msg("FILE_UPLOAD");
	msg.addAttrib(Attr("COMMAND", msg.command()));
	msg.addAttrib(Attr("SOURCEIP", "localhost"));
	msg.addAttrib(Attr("SOURCEPORT", "8080"));
	String^ Ip = hDstIpTxtBox->Text;
	String^ Port = hDstPortTxtBox->Text;
	String^ Path = hListBox->Text;
	msg.addAttrib(Attr("IPADDR", toStdString(Ip)));
	msg.addAttrib(Attr("PORT", toStdString(Port)));
	msg.addAttrib(Attr("FILEPATH", toStdString(Path)));
	pSendr1_->postMessage(msg);
}

//----< Event Handler for FileDownload  >---------------------------------------

void WPFCppCliDemo::FileDownload(Object^ obj, RoutedEventArgs^ args)
{
	Console::Write("\n Sending File Download Message..\n");
	hStatus->Text = "File downloading..";
	Message msg("FILE_DOWNLOAD");
	msg.addAttrib(Attr("COMMAND", msg.command()));
	msg.addAttrib(Attr("SOURCEIP", "localhost"));
	msg.addAttrib(Attr("SOURCEPORT", "8080"));
	String^ Ip = DstIpTxtBox->Text;
	String^ Port = DstPortTxtBox->Text;
	String^ Path = listBox->Text;
	msg.addAttrib(Attr("IPADDR", toStdString(Ip)));
	msg.addAttrib(Attr("PORT", toStdString(Port)));
	msg.addAttrib(Attr("FILEPATH", toStdString(Path)));
	pSendr2_->postMessage(msg);
	Console::Write("\n Msg ::  Download\n");
}

//----< Event Handler for FileSearch  >---------------------------------------

void WPFCppCliDemo::FileSearch(Object^obj, RoutedEventArgs^args)
{
	Console::Write("\n Sending File Search Message..\n");
	hStatus->Text = "Searching for files..";
	Message msg("FILE_SEARCH");
	msg.addAttrib(Attr("COMMAND", msg.command()));
	msg.addAttrib(Attr("SOURCEIP", "localhost"));
	msg.addAttrib(Attr("SOURCEPORT", "8080"));
	String^ Ip = fDstIpTxtBox->Text;
	String^ Port = fDstPortTxtBox->Text;
	String^ Path = flistBox->Text;
	String^ patts = fPattTxtBox->Text;
	std::string pat = toStdString(patts);
	std::vector<std::string> filepatt;
	size_t pos = pat.find(' ');
	int i = 0;
	if (pos > pat.size())
		filepatt.push_back(pat);
	else
	{
	    int initialPos = 0;
		while (pos != std::string::npos) {
			filepatt.push_back(pat.substr(initialPos, pos - initialPos + 1));
			initialPos = pos + 1;
			pos = pat.find(' ', initialPos);
		}
		filepatt.push_back(pat.substr(initialPos, std::min(pos, pat.size()) - initialPos + 1));
	}
	
	msg.addAttrib(Attr("IPADDR", toStdString(Ip)));
	msg.addAttrib(Attr("PORT", toStdString(Port)));
	msg.addAttrib(Attr("FILEPATH", toStdString(Path)));
	for (size_t k = 0; k < filepatt.size(); k++)
		msg.addAttrib(Attr("FILEPATTERN", filepatt[k]));
	pSendr3_->postMessage(msg);
}

//----< Event Handler for TextSearch  >---------------------------------------

void WPFCppCliDemo::TextSearch(Object^obj, RoutedEventArgs^args)
{
	Console::Write("\n Sending Text Search Message..\n");
	hStatus->Text = "Searching for given string in files..";
	Message msg("TEXT_SEARCH");
	msg.addAttrib(Attr("COMMAND", msg.command()));
	msg.addAttrib(Attr("SOURCEIP", "localhost"));
	msg.addAttrib(Attr("SOURCEPORT", "8080"));
	String^ Ip = tDstIpTxtBox->Text;
	String^ Port = tDstPortTxtBox->Text;
	String^ Path = tlistBox->Text;
	String^ patts = tPattTxtBox->Text;
	String^ text = tTextBox->Text;
	std::string pat = toStdString(patts);
	std::string ip = toStdString(Ip);
	std::string port = toStdString(Port);
	std::vector<std::string> filepatt;
	std::vector<std::string> dstIpvec;
	std::vector<std::string>dstportvec;
	
	
	size_t pos = pat.find(' ');
	if (pos > pat.size())
		filepatt.push_back(pat);
	else
	{
		int initialPos = 0;
		while (pos != std::string::npos) {
			filepatt.push_back(pat.substr(initialPos, pos - initialPos + 1));
			initialPos = pos + 1;
			pos = pat.find(' ', initialPos);
		}
		filepatt.push_back(pat.substr(initialPos, std::min(pos, pat.size()) - initialPos + 1));
	}
	size_t ipos = ip.find(' ');
	if (ipos > ip.size())
		dstIpvec.push_back(ip);
	else
	{
		int initialPos = 0;
		while (ipos != std::string::npos) 
		{
			dstIpvec.push_back(ip.substr(initialPos, ipos - initialPos + 1));
			initialPos = ipos + 1;
			ipos = ip.find(' ', initialPos);
		}
		dstIpvec.push_back(ip.substr(initialPos, std::min(ipos, ip.size()) - initialPos + 1));
	}
	size_t ppos = port.find(' ');
	if (ppos > port.size())
		dstportvec.push_back(port);
	else
	{
		int initialPos = 0;
		while (ppos != std::string::npos)
		{
			dstportvec.push_back(port.substr(initialPos, ppos - initialPos + 1));
			initialPos = ppos + 1;
			ppos = port.find(' ', initialPos);
		}
		dstportvec.push_back(port.substr(initialPos, std::min(ppos, port.size()) - initialPos + 1));
	}
	
	for (size_t p = 0; p < dstIpvec.size(); p++)
		msg.addAttrib(Attr("IPADDR", dstIpvec[p]));
	for (size_t q = 0; q < dstportvec.size(); q++)
		msg.addAttrib(Attr("PORT", dstportvec[q]));
	msg.addAttrib(Attr("FILEPATH", toStdString(Path)));
	for (size_t s = 0; s < filepatt.size(); s++)
		msg.addAttrib(Attr("FILEPATTERN", filepatt[s]));
	msg.addAttrib(Attr("TEXT", toStdString(text)));
	pSendr4_->postMessage(msg);
}

//----< Event Handler for clear  >---------------------------------------

void WPFCppCliDemo::clear(Object^ sender, RoutedEventArgs^ args)
{
	Console::Write("\n  cleared message text");
	hStatus->Text = "Cleared message";
	hDstIpTxtBox->Text = "";
	hDstPortTxtBox->Text = "";
	hListBox->Text = "";
	DstIpTxtBox->Text = "";
	DstPortTxtBox->Text = "";
	listBox->Text = "";
	flistBox->Text = "";
	fDstIpTxtBox->Text = "";
	fDstPortTxtBox->Text = "";
	fPattTxtBox->Text = "";
	//tlistBox->Text = "";
	tDstIpTxtBox->Text = "";
	tDstPortTxtBox->Text = "";
	tPattTxtBox->Text = "";
	tTextBox->Text = "";
	delete pChann1_;
	delete pSendr1_;
	delete pRecvr1_;
	delete pChann2_;
	delete pSendr2_;
	delete pRecvr2_;
	delete pChann3_;
	delete pSendr3_;
	delete pRecvr3_;
	delete pChann4_;
	delete pSendr4_;
	delete pRecvr4_;
	ObjectFactory* pObjFact1 = new ObjectFactory;
	pSendr1_ = pObjFact1->createSendr();
	pRecvr1_ = pObjFact1->createRecvr();
	pChann1_ = pObjFact1->createMockChannel(pSendr1_, pRecvr1_);
	pChann1_->start();
	delete pObjFact1;

	ObjectFactory* pObjFact2 = new ObjectFactory;
	pSendr2_ = pObjFact2->createSendr();
	pRecvr2_ = pObjFact2->createRecvr();
	pChann2_ = pObjFact2->createMockChannel(pSendr2_, pRecvr2_);
	pChann2_->start();
	delete pObjFact2;

	ObjectFactory* pObjFact3 = new ObjectFactory;
	pSendr3_ = pObjFact3->createSendr();
	pRecvr3_ = pObjFact3->createRecvr();
	pChann3_ = pObjFact2->createMockChannel(pSendr3_, pRecvr3_);
	pChann3_->start();
	delete pObjFact3;

	ObjectFactory* pObjFact4 = new ObjectFactory;
	pSendr4_ = pObjFact4->createSendr();
	pRecvr4_ = pObjFact4->createRecvr();
	pChann4_ = pObjFact4->createMockChannel(pSendr4_, pRecvr4_);
	pChann4_->start();
	delete pObjFact4;

}

//----< Event Handler for clearview - textblock  >---------------------------------------

void WPFCppCliDemo::clearview(Object^ sender, RoutedEventArgs^ args)
{
	Console::Write("\n  cleared message text");
	hStatus->Text = "Cleared message";
	vTextBlock1->Text = "";
}

//----< Event Handler for Browse folder  >---------------------------------------

void WPFCppCliDemo::browseForFolder(Object^ sender, RoutedEventArgs^ args)
{
	std::cout << "\n  Browsing for folder";
	flistBox->Text = "";
	System::Windows::Forms::DialogResult result;
	result = hFolderBrowserDialog->ShowDialog();
	if (result == System::Windows::Forms::DialogResult::OK)
	{
		String^ path = hFolderBrowserDialog->SelectedPath;
		flistBox->Text = path;
	}
}

//----< Event Handler for Browse folder for text search  >---------------------------------------

void WPFCppCliDemo::browseForFolder_txt(Object^ sender, RoutedEventArgs^ args)
{
	std::cout << "\n  Browsing for folder";
	tlistBox->Text = "";
	System::Windows::Forms::DialogResult result;
	result = tFolderBrowserDialog->ShowDialog();
	if (result == System::Windows::Forms::DialogResult::OK)
	{
		String^ path = tFolderBrowserDialog->SelectedPath;
		tlistBox->Text = path;
	}
}

//----< Event Handler for Browse file  >---------------------------------------

void WPFCppCliDemo::browseForFile(Object^sender, RoutedEventArgs^args)
{
	std::cout << "\n  Browsing for file";
	hListBox->Text = "";
	System::Windows::Forms::DialogResult result;
	result = hFileBrowserDialog->ShowDialog();
	if (result == System::Windows::Forms::DialogResult::OK)
	{
		String^ file = hFileBrowserDialog->FileName;
		hListBox->Text = file;
	}
}

//----< Event Handler for Browse file for file download  >---------------------------------------

void WPFCppCliDemo::browseForFileDownload(Object^sender, RoutedEventArgs^args)
{
	std::cout << "\n  Browsing for file to download";
	listBox->Text = "";
	System::Windows::Forms::DialogResult result;
	result = hFileBrowserDialog->ShowDialog();
	if (result == System::Windows::Forms::DialogResult::OK)
	{
		String^ file = hFileBrowserDialog->FileName;
		listBox->Text = file;
	}
}


void WPFCppCliDemo::OnLoaded(Object^ sender, RoutedEventArgs^ args)
{
	Console::Write("\n  Window loaded");
}
void WPFCppCliDemo::Unloading(Object^ sender, System::ComponentModel::CancelEventArgs^ args)
{
	Console::Write("\n  Window closing");
}

[STAThread]
//int _stdcall WinMain()
int main(array<System::String^>^ args)
{
	Console::WriteLine(L"\n Starting WPFCppCliDemo");

	Application^ app = gcnew Application();
	app->Run(gcnew WPFCppCliDemo());
	Console::WriteLine(L"\n\n");
}