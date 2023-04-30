// MainFrame.cpp

#include "../headers/MainFrame.h"
#include <wx/wx.h>
#include <cstdlib>
#include <vector>
#include <fstream>

MainFrame::MainFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title) {

	CreateStatusBar(); // like cout for checking
	CreateWidgets();
	BindEventHandlers();
}

void MainFrame::CreateWidgets()
{
	// Define time for random number
	srand(time(0));

	// Initialize all panels
	menuPanel = new MenuPanel(this);
	 
	arrayPanel = new ArrayPanel(this);

	sllPanel = new SinglyLinkedListPanel(this);

	dllPanel = new DoublyLinkedListPanel(this);

	stackPanel = new StackPanel(this);

	queuePanel = new QueuePanel(this);

	cllPanel = new CircularLinkedListPanel(this);

	// Add all panels
	panels.push_back(menuPanel);
	panels.push_back(arrayPanel);
	panels.push_back(sllPanel);
	panels.push_back(dllPanel);
	panels.push_back(stackPanel);
	panels.push_back(queuePanel);
	panels.push_back(cllPanel);

	// Sizer init
	sizer = new wxBoxSizer(wxVERTICAL);
	this->SetSizer(sizer);

	CreateMenu();

	OpenMenu();
}

void MainFrame::CreateMenu()
{

	// Array
	menuPanel->arrayButton = new wxButton(menuPanel, arrayPanel->panelId, arrayPanel->name, wxPoint(100, 300), wxSize(-1, -1));
	menuPanel->arrayButton->Bind(wxEVT_BUTTON, &MainFrame::OnChangePanelButtonClicked, this);
	arrayPanel->menuButton->Bind(wxEVT_BUTTON, &MainFrame::OnChangePanelButtonClicked, this);

	// Singly Linked List
	menuPanel->sllButton = new wxButton(menuPanel, sllPanel->panelId, sllPanel->name, wxPoint(400, 100), wxSize(-1, -1));
	menuPanel->sllButton->Bind(wxEVT_BUTTON, &MainFrame::OnChangePanelButtonClicked, this);
	sllPanel->menuButton->Bind(wxEVT_BUTTON, &MainFrame::OnChangePanelButtonClicked, this);

	// Doubly Linked List
	menuPanel->dllButton = new wxButton(menuPanel, dllPanel->panelId, dllPanel->name, wxPoint(400, 200), wxSize(-1, -1));
	menuPanel->dllButton->Bind(wxEVT_BUTTON, &MainFrame::OnChangePanelButtonClicked, this);
	dllPanel->menuButton->Bind(wxEVT_BUTTON, &MainFrame::OnChangePanelButtonClicked, this);

	// Stack
	menuPanel->stackButton = new wxButton(menuPanel, stackPanel->panelId, stackPanel->name, wxPoint(100, 100), wxSize(-1, -1));
	menuPanel->stackButton->Bind(wxEVT_BUTTON, &MainFrame::OnChangePanelButtonClicked, this);
	stackPanel->menuButton->Bind(wxEVT_BUTTON, &MainFrame::OnChangePanelButtonClicked, this);

	// Queue
	menuPanel->queueButton = new wxButton(menuPanel, queuePanel->panelId, queuePanel->name, wxPoint(100, 200), wxSize(-1, -1));
	menuPanel->queueButton->Bind(wxEVT_BUTTON, &MainFrame::OnChangePanelButtonClicked, this);
	queuePanel->menuButton->Bind(wxEVT_BUTTON, &MainFrame::OnChangePanelButtonClicked, this);

	// Circular Linked List
	menuPanel->cllButton = new wxButton(menuPanel, cllPanel->panelId, cllPanel->name, wxPoint(400, 300), wxSize(-1, -1));
	menuPanel->cllButton->Bind(wxEVT_BUTTON, &MainFrame::OnChangePanelButtonClicked, this);
	cllPanel->menuButton->Bind(wxEVT_BUTTON, &MainFrame::OnChangePanelButtonClicked, this);

	// Set time delay RadioBox
	SetTimeDelay(250);
	menuPanel->timeDelayRadioBox->Bind(wxEVT_RADIOBOX, &MainFrame::OnTimeDelayRadioBoxChanged, this);
}

void MainFrame::OpenMenu()
{
	// Hide all panels 
	for (auto panel : panels) {
		panel->Hide();
	}

	// Only open the menu at first
	panels[0]->Show();
	sizer->Add(panels[0], 1, wxGROW);
}

void MainFrame::BindEventHandlers()
{
	this->Bind(wxEVT_CLOSE_WINDOW, &MainFrame::OnClose, this);
}

void MainFrame::OnChangePanelButtonClicked(wxCommandEvent& event)
{
	auto* button = (wxButton*)event.GetEventObject();

	auto id = button->GetId();

	sizer->Detach(0);
	for (auto panel : panels) {
		panel->Hide();
	}

	sizer->Prepend(panels[id], 1, wxGROW);
	panels[id]->Show();
	sizer->Layout();

}

void MainFrame::OnTimeDelayRadioBoxChanged(wxCommandEvent& event)
{
	wxString selection = menuPanel->timeDelayRadioBox->GetStringSelection();
	int delayTime = stof(string(selection.mb_str())) * 1000;

	SetTimeDelay(delayTime);
}

void MainFrame::SetTimeDelay(int delayTime)
{
	arrayPanel->delay_time = delayTime;
	sllPanel->delay_time = delayTime;
	dllPanel->delay_time = delayTime;
	stackPanel->delay_time = delayTime;
	queuePanel->delay_time = delayTime;
	cllPanel->delay_time = delayTime;
}

void MainFrame::OnClose(wxCloseEvent& event) {

	arrayPanel->SaveFile();
	sllPanel->SaveFile();
	dllPanel->SaveFile();
	queuePanel->SaveFile();
	stackPanel->SaveFile();
	cllPanel->SaveFile();

	arrayPanel->FreeMemory();
	sllPanel->FreeMemory();
	dllPanel->FreeMemory();
	stackPanel->FreeMemory();
	queuePanel->FreeMemory();
	cllPanel->FreeMemory();

	for (auto panel : panels) {
		delete panel;
	}

	event.Skip();
}
