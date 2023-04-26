#include "../headers/MenuPanel.h"
#include "../headers/ArrayPanel.h"
#include "../headers/SinglyLinkedListPanel.h"

#include "../utility/Number/NumberUtil.h"
#include <wx/wx.h>
#include <string>

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

using namespace std;

MenuPanel::MenuPanel(wxWindow* _parent) : wxPanel(_parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL)
{
	this->SetFont(normalFont);
	this->SetBackgroundColour(wxColour("LightGrey"));
	parent = _parent;

	CreateWidgets();
	BindEventHandlers();
}

// Widgets

void MenuPanel::CreateWidgets()
{
	// TITLE
	wxString titleText = wxString(name);
	title = new wxStaticText(this, wxID_ANY, titleText, wxPoint(275, 15));
	title->SetFont(headerFont);

	// TIME DELAY RADIO BOX
	timeDelayChoices.Add("0");
	timeDelayChoices.Add("0.125");
	timeDelayChoices.Add("0.25");
	timeDelayChoices.Add("0.5");
	timeDelayChoices.Add("1");

	timeDelayRadioBox = new wxRadioBox(this, wxID_ANY, "Set Time Delay (s)", wxPoint(100, 400), wxSize(450, -1), timeDelayChoices, 0, wxRA_SPECIFY_COLS | wxTE_CENTER);
	timeDelayRadioBox->SetStringSelection("0.25");
	timeDelayRadioBox->SetFont(smallFont);
}
	

// Binding Event

void MenuPanel::BindEventHandlers()
{
	
}
