#pragma once
#include <wx/wx.h>
#include <wx/richtext/richtextctrl.h>
#include <wx/spinctrl.h>
#include <string>

using namespace std;

class ArrayPanel : public wxPanel
{
public: 
	ArrayPanel(wxWindow* parent);

	// ID
	const int panelId = 1;

	// Name
	const string name = "Array";

	// Delay time
	int delay_time = 1000;
	int found_delay_time = 3000;

	// Fonts
	wxFont headerFont = wxFont(wxFontInfo(wxSize(0, 35)).FaceName("Segoe UI"));
	wxFont normalFont = wxFont(wxFontInfo(wxSize(0, 20)).FaceName("Segoe UI"));
	wxFont smallFont = wxFont(wxFontInfo(wxSize(0, 15)).FaceName("Segoe UI"));
	wxFont smallBoldFont = wxFont(wxFontInfo(wxSize(0, 15)).FaceName("Segoe UI").Bold());


	// Menu
	wxButton* menuButton;

	// functions
	void FreeMemory();
	string GetCreateCode();
	string GetUpdateCode(int index, int value);
	string GetFindCode(int value);
	void SetCode(string code);
	void DrawInstruction(int lineNumber);

private:
	// functions
	void CreateWidgets();
	void BindEventHandlers();

	// Event Handlers
	void OnCreateButtonClicked(wxCommandEvent& event);
	void OnUpdateButtonClicked(wxCommandEvent& event);
	void OnFindButtonClicked(wxCommandEvent& event);

	// Parent
	wxWindow* parent;

	// Titles
	wxStaticText* title;

	// Display Array
	int length = 10;
	const int MIN_LENGTH = 1;
	const int MAX_LENGTH = 10;
	const int MIN_VALUE = 0;
	const int MAX_VALUE = 99;
	const int BOX_WIDTH = 60; // BOX_HEIGHT: auto
	const int START_X = 100, START_Y = 100;
	wxStaticText** array_boxes;
	wxStaticText** index_texts;
	
	// Create
	wxButton* createButton;
	wxSpinCtrl* sizeInput;
	wxArrayString choices; // choose random or empty
	wxRadioBox* radioBox;

	// Update
	wxButton* updateButton;
	wxSpinCtrl* updateIndex;
	wxSpinCtrl* updateValue;

	// Find
	wxButton* findButton;
	wxSpinCtrl* findValue;

	// Code
	wxTextCtrl* codeDisplay;
	wxStaticText* codeInstructor;
};