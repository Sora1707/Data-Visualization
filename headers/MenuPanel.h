#pragma once
#include <wx/wx.h>
#include <wx/spinctrl.h>
#include <vector>

using namespace std;

class MenuPanel : public wxPanel
{
public: 
	MenuPanel(wxWindow* parent);

	// ID
	const int panelId = 0;

	// Name
	const string name = "MENU PANEL";

	// Colors
	wxColour RED = wxColour("Red");
	wxColour LIGHT_YELLOW = wxColour(255, 255, 153);
	wxColour BLUE = wxColour("Blue");
	wxColour GREEN = wxColour(102, 255, 102);
	wxColour BLACK = wxColour("Black");
	wxColour LIGHT_GREY = wxColour("LightGrey");
	
	// Fonts
	wxFont headerFont = wxFont(wxFontInfo(wxSize(0, 35)).FaceName("Segoe UI"));
	wxFont sllFont = wxFont(wxFontInfo(wxSize(0, 30)).FaceName("Segoe UI"));
	wxFont normalFont = wxFont(wxFontInfo(wxSize(0, 25)).FaceName("Segoe UI"));
	wxFont smallFont = wxFont(wxFontInfo(wxSize(0, 18)).FaceName("Segoe UI"));

	// Buttons
	wxButton* arrayButton;
	wxButton* sllButton;
	wxButton* dllButton;
	wxButton* stackButton;
	wxButton* queueButton;
	wxButton* cllButton;

	// Time set radio box
	wxArrayString timeDelayChoices;
	wxRadioBox* timeDelayRadioBox;

private:
	// Init functions
	void CreateWidgets();
	void BindEventHandlers();

	// Event Handlers

	// Parent
	wxWindow* parent;

	// Titles
	wxStaticText* title;
};