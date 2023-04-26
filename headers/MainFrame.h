// MainFrame.h

#pragma once

#include <wx/wx.h>
#include <vector>
#include "../headers/MenuPanel.h"
#include "../headers/ArrayPanel.h"
#include "../headers/SinglyLinkedListPanel.h"
#include "../headers/DoublyLinkedListPanel.h"
#include "../headers/StackPanel.h"
#include "../headers/QueuePanel.h"

using namespace std;

class MainFrame : public wxFrame
{
public:
	MainFrame(const wxString& title);

private:

	// declare panels
	MenuPanel* menuPanel;
	ArrayPanel* arrayPanel;
	SinglyLinkedListPanel* sllPanel;
	DoublyLinkedListPanel* dllPanel;
	StackPanel* stackPanel;
	QueuePanel* queuePanel;

	vector <wxPanel*> panels;

	// declare widgets
	wxBoxSizer* sizer;
	wxButton* arrayButton;
	wxButton* sllButton;
	 
	// declare variables

	// widgets init
	void CreateWidgets();
	void CreateMenu();
	void OpenMenu();

	// binding init
	void BindEventHandlers();

	// declare eventHandler
	void OnClose(wxCloseEvent& event);
	void OnChangePanelButtonClicked(wxCommandEvent& event);
	void OnTimeDelayRadioBoxChanged(wxCommandEvent& event);

	//functions 
	void SetTimeDelay(int delayTime);
};