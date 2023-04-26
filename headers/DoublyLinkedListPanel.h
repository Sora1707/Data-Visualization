#pragma once
#include <wx/wx.h>
#include <wx/spinctrl.h>
#include <string>

using namespace std;

class DoublyLinkedListPanel : public wxPanel
{
public: 
	DoublyLinkedListPanel(wxWindow* parent);

	// ID
	const int panelId = 3;

	// Name
	const string name = "Doubly Linked List";

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
	wxFont normalFont = wxFont(wxFontInfo(wxSize(0, 20)).FaceName("Segoe UI"));
	wxFont smallFont = wxFont(wxFontInfo(wxSize(0, 15)).FaceName("Segoe UI"));
	wxFont smallBoldFont = wxFont(wxFontInfo(wxSize(0, 15)).FaceName("Segoe UI").Bold());

	// Delay time
	int delay_time = 1000;
	int found_delay_time = 3000;

	// Menu
	wxButton* menuButton;

	// functions
	void FreeMemory();
	string GetCreateCode();
	string GetUpdateCode(int index, int value);
	string GetAddCode(int index, int value);
	string GetDeleteCode(int index);
	string GetDeleteCodeByValue(int value);
	string GetFindCode(int value, bool fromBegin);
	void SetCode(string code);
	void DrawInstruction(int lineNumber);

private:
	// Init functions
	void CreateWidgets();
	void BindEventHandlers();

	// Drawing functions
	void paintEvent(wxPaintEvent& evt);
	void paintNow();
	void drawArrowHorizontal(wxDC& dc, int y, int x1, int x2, int thickness);
	void OnPaint(wxDC& dc);

	// Event Handlers
	void OnCreateButtonClicked(wxCommandEvent& event);
	void OnUpdateButtonClicked(wxCommandEvent& event);
	void OnAddButtonClicked(wxCommandEvent& event);
	void OnDeleteButtonClicked(wxCommandEvent& event);
	void OnFindButtonClicked(wxCommandEvent& event);

	// Other function
	void ChangeTexture(wxStaticText*& text, wxColour foregroundColor, wxColour backgroundColor);
	void ChangePosition(wxStaticText*& text, wxPoint pos);
	void AddNewBoxesAt(int index, wxStaticText*& newText);

	// Drawer 
	wxPaintDC* drawTool;

	// Parent
	wxWindow* parent;

	// Titles
	wxStaticText* title;

	// Display Linked List (Using Array)
	const int MIN_LENGTH = 1;
	const int MAX_LENGTH = 10;
	const int MIN_VALUE = 0;
	const int MAX_VALUE = 99;
	const int BOX_WIDTH = 40; // BOX_HEIGHT: auto
	const int ARROW_LENGTH = 35;
	const int ARROW_THICKNESS = 3;
	const int START_X = 50, START_Y = 100;
	int length = MAX_LENGTH;
	wxStaticText** array_boxes;
	wxStaticText** index_texts;
	
	// Create
	wxButton* createButton;
	wxSpinCtrl* sizeInput;
	wxArrayString createChoices; // choose random or empty
	wxRadioBox* createRadioBox;

	// Update
	wxButton* updateButton;
	wxSpinCtrl* updateIndex;
	wxSpinCtrl* updateValue;

	// Add 
	wxButton* addButton;
	wxSpinCtrl* addValue;
	wxSpinCtrl* addIndex;
	wxArrayString addChoices;
	wxRadioBox* addRadioBox;
	wxStaticText* addMovingBox;

	// Delete
	wxButton* deleteButton;
	wxSpinCtrl* deleteValue;
	wxSpinCtrl* deleteIndex;
	wxArrayString deleteChoices;
	wxRadioBox* deleteRadioBox;

	// Find
	wxButton* findButton;
	wxSpinCtrl* findValue;
	wxArrayString findChoices;
	wxRadioBox* findRadioBox;

	// Code
	wxTextCtrl* codeDisplay;
	wxStaticText* codeInstructor;
};