#include "../headers/ArrayPanel.h"
#include "../headers/App.h"
#include "../utility/Number/NumberUtil.h"

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

using namespace std;

ArrayPanel::ArrayPanel(wxWindow* _parent) : wxPanel(_parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL)
{
	this->SetFont(normalFont);
	this->SetBackgroundColour(wxColour("LightGrey"));
	parent = _parent;

	CreateWidgets();
	BindEventHandlers();
}

void ArrayPanel::CreateWidgets()
{
	//
	wxString titleText = wxString(name);
	title = new wxStaticText(this, wxID_ANY, titleText, wxPoint(350, 15));
	title->SetFont(headerFont);

	// 
	delete[] array_boxes;
	delete[] index_texts;
	array_boxes = new wxStaticText * [MAX_LENGTH];
	index_texts = new wxStaticText * [MAX_LENGTH];

	for (int i = 0; i < MAX_LENGTH; ++i) {
		int posX = START_X + i * BOX_WIDTH;
		int posY = START_Y;

		// New box
		wxStaticText* box = new wxStaticText(this, wxID_ANY, to_string(10 + i), wxPoint(posX, posY), wxSize(BOX_WIDTH, -1), wxALIGN_CENTER | wxBORDER_SIMPLE);
		box->SetFont(headerFont);
		box->SetForegroundColour(wxColour("Black"));
		array_boxes[i] = box;

		// New index text
		wxStaticText* index_text = new wxStaticText(this, wxID_ANY, to_string(i + 1), wxPoint(posX, posY - 30), wxSize(BOX_WIDTH, -1), wxALIGN_CENTER);
		index_texts[i] = index_text;

	}

	// 
	createButton = new wxButton(this, wxID_ANY, "Create", wxPoint(100, 350));
	new wxStaticText(this, wxID_ANY, "Size", wxPoint(75, 400));
	sizeInput = new wxSpinCtrl(this, wxID_ANY, "", wxPoint(135, 400), wxSize(100, -1), wxALIGN_LEFT, MIN_LENGTH, MAX_LENGTH);

	choices.Add("Random");
	choices.Add("Empty");
	radioBox = new wxRadioBox(this, wxID_ANY, "", wxPoint(75, 450), wxDefaultSize, choices, 0, wxRA_SPECIFY_ROWS);
	radioBox->SetFont(smallFont);

	//
	updateButton = new wxButton(this, wxID_ANY, "Update", wxPoint(300, 350));
	new wxStaticText(this, wxID_ANY, "Index", wxPoint(300, 400));
	updateIndex = new wxSpinCtrl(this, wxID_ANY, "", wxPoint(375, 400), wxSize(100, -1), wxALIGN_LEFT, MIN_LENGTH, MAX_LENGTH);
	new wxStaticText(this, wxID_ANY, "Value", wxPoint(300, 450));
	updateValue = new wxSpinCtrl(this, wxID_ANY, "", wxPoint(375, 450), wxSize(100, -1), wxALIGN_LEFT, MIN_VALUE, MAX_VALUE);


	//
	findButton = new wxButton(this, wxID_ANY, "Find", wxPoint(550, 350));
	new wxStaticText(this, wxID_ANY, "Value", wxPoint(550, 400));
	findValue = new wxSpinCtrl(this, wxID_ANY, "", wxPoint(625, 400), wxSize(100, -1), wxALIGN_LEFT, MIN_VALUE, MAX_VALUE);

	// 
	int MENU_ID = 0;
	menuButton = new wxButton(this, MENU_ID, "Menu", wxPoint(10, 10));

	// CODE DISPLAY
	codeDisplay = new wxTextCtrl(this, wxID_ANY, "", wxPoint(800, 100), wxSize(300, 400), wxTE_MULTILINE | wxTE_READONLY);
	codeDisplay->SetFont(smallFont);
	SetCode(GetCreateCode());
	codeInstructor = new wxStaticText(this, wxID_ANY, "=>", wxPoint(15000, 15000));
	codeInstructor->SetFont(smallBoldFont);

}


void ArrayPanel::BindEventHandlers()
{
	createButton->Bind(wxEVT_BUTTON, &ArrayPanel::OnCreateButtonClicked, this);
	updateButton->Bind(wxEVT_BUTTON, &ArrayPanel::OnUpdateButtonClicked, this);
	findButton->Bind(wxEVT_BUTTON, &ArrayPanel::OnFindButtonClicked, this);
}

// Public functions

void ArrayPanel::FreeMemory()
{
	delete[] index_texts;
	delete[] array_boxes;
}

string ArrayPanel::GetCreateCode()
{
	string code =
		"const int MAX_LENGTH = 10;\r\n"
		"int a[MAX_LENGTH];\r\n"
		"// or\r\n"
		"int* a = new int[" + to_string(length) + "];\r\n"
	;
	return code;
}

string ArrayPanel::GetUpdateCode(int index, int value)
{
	string code =
		"a[" + to_string(index) + "] = " + to_string(value) + ";\r\n"
	;
	return code;
}

string ArrayPanel::GetFindCode(int value)
{
	string code =
		"for (int i = 0; i < " + to_string(length) + "; ++i) \r\n"
		"    if (a[i] == " + to_string(value) + ")\r\n" 
		"        cout << i << endl;\r\n"
	;
	return code;
}

void ArrayPanel::SetCode(string code)
{
	wxString codeText(code);
	codeDisplay->SetLabel(codeText);
	codeDisplay->Refresh();
	codeDisplay->Update();
}

void ArrayPanel::DrawInstruction(int lineNumber)
{
	int posx = 780;
	int posy = 102 + (lineNumber - 1) * 20;

	codeInstructor->Show();

	codeInstructor->SetPosition(wxPoint(posx, posy));
	codeInstructor->Refresh();
	codeInstructor->Update();

	Sleep(delay_time);
}

// Event Handlers

void ArrayPanel::OnCreateButtonClicked(wxCommandEvent& event)
{
	/* PREVENT MUTIPLE CALLS */
	static bool isBusy = false;

	if (isBusy) return;

	isBusy = true;
	createButton->Disable();

	int arr_length = sizeInput->GetValue();
	wxString type = radioBox->GetStringSelection();

	length = arr_length;

	for (int i = 0; i < MAX_LENGTH; ++i) {
		if (i >= length) {
			array_boxes[i]->Hide();
			index_texts[i]->Hide();
		}
		else {
			array_boxes[i]->Show();
			index_texts[i]->Show();
			string value = type.IsSameAs("Empty") ? "0" : to_string(NumberUtil::getRandomInt(MIN_VALUE, MAX_VALUE));
			wxString valueText = wxString(value);
			array_boxes[i]->SetLabel(valueText);
		}
	}

	SetCode(GetCreateCode());

	codeInstructor->Hide();


	/* END OF PREVENT MULTIPLE CALLS */
	wxGetApp().Yield();
	createButton->Enable();

	isBusy = false;
}

void ArrayPanel::OnUpdateButtonClicked(wxCommandEvent& event)
{
	int index = updateIndex->GetValue() - 1;

	/* PREVENT MUTIPLE CALLS */
	static bool isBusy = false;

	if (isBusy) return;

	isBusy = true;
	updateButton->Disable();


	if (index >= length) {
		wxLogMessage("The index greater than the length!");
	}
	else {

		int value = updateValue->GetValue();
		wxString valueText(to_string(value));
		array_boxes[index]->SetLabel(valueText);
		array_boxes[index]->SetForegroundColour(wxColour("Red"));
		array_boxes[index]->SetBackgroundColour(wxColour(255, 255, 153));
		array_boxes[index]->Refresh();
		array_boxes[index]->Update();

		SetCode(GetUpdateCode(index + 1, value));

		Sleep(delay_time);

		array_boxes[index]->SetForegroundColour(wxColour("Black"));
		array_boxes[index]->SetBackgroundColour(wxColour("LightGrey"));
		array_boxes[index]->Refresh();
		array_boxes[index]->Update();
	}

	codeInstructor->Hide();


	/* END OF PREVENT MULTIPLE CALLS */
	wxGetApp().Yield();
	updateButton->Enable();

	isBusy = false;
}

void ArrayPanel::OnFindButtonClicked(wxCommandEvent& event)
{
		/* PREVENT MUTIPLE CALLS */
	static bool isBusy = false;

	if (isBusy) return;

	isBusy = true;
	findButton->Disable();

	bool isFound = false;
	int value = findValue->GetValue();
	
	SetCode(GetFindCode(value));

	for (int i = 0; i < length; ++i) {

		array_boxes[i]->SetForegroundColour(wxColour("Red"));
		array_boxes[i]->SetBackgroundColour(wxColour(255, 255, 153));
		array_boxes[i]->Refresh();
		array_boxes[i]->Update();

		DrawInstruction(1);

		Sleep(delay_time);

		DrawInstruction(2); 

		// If we can find the value
		if (array_boxes[i]->GetLabel().IsSameAs(to_string(value))) {
			
			DrawInstruction(3);

			array_boxes[i]->SetForegroundColour(wxColour("Blue"));
			array_boxes[i]->SetBackgroundColour(wxColour(102, 255, 102));
			array_boxes[i]->Refresh();
			array_boxes[i]->Update();
			isFound = true;
		}

		else {

			array_boxes[i]->SetForegroundColour(wxColour("Black"));
			array_boxes[i]->SetBackgroundColour(wxColour("LightGrey"));
			array_boxes[i]->Refresh();
			array_boxes[i]->Update();
		}
	}

	if (!isFound) {
		wxString messageText = wxString("The number " + to_string(value) + " cannot be found!");
		wxFormatString message = wxFormatString(messageText);
		wxLogMessage(message);
	}

	else {
		Sleep(found_delay_time);

		for (int i = 0; i < length; ++i) {
			array_boxes[i]->SetForegroundColour(wxColour("Black"));
			array_boxes[i]->SetBackgroundColour(wxColour("LightGrey"));
			array_boxes[i]->Refresh();
			array_boxes[i]->Update();
		}
	}

	codeInstructor->Hide();


	/* END OF PREVENT MULTIPLE CALLS */
	wxGetApp().Yield();
	findButton->Enable();

	isBusy = false;
}
