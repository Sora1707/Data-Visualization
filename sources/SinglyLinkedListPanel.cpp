#include "../headers/SinglyLinkedListPanel.h"
#include "../headers/App.h"
#include "../utility/Number/NumberUtil.h"
#include <wx/wx.h>
#include <string>
#include <fstream>

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#define DATA_FILE "data/SinglyLinkedList.txt"

using namespace std;

SinglyLinkedListPanel::SinglyLinkedListPanel(wxWindow* _parent) : wxPanel(_parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL)
{
	this->SetFont(smallFont);
	this->SetBackgroundColour(wxColour("LightGrey"));
	parent = _parent;

	ReadFile();
	CreateWidgets();
	BindEventHandlers();
}

void SinglyLinkedListPanel::FreeMemory()
{
	delete drawTool;
	delete[] index_texts;
	delete[] array_boxes;
}

string SinglyLinkedListPanel::GetCreateCode()
{
	return "// " + name;
}

string SinglyLinkedListPanel::GetUpdateCode(int index, int value)
{
	string code =
	"Node * cur = head;\r\n"
	"int i = 1;\r\n"
	"while (cur != nullptr) {\r\n"
	"    if (i == " + to_string(index) + ") {\r\n"
	"        cur->value = " + to_string(value) + ";\r\n"
	"        break;\r\n"
	"    }\r\n"
	"    ++i;\r\n"
	"    cur = cur->next;\r\n"
	"}\r\n"
	;
	return code;
}

string SinglyLinkedListPanel::GetAddCode(int index, int value)
{
	string code =
		"Node * cur = head;\r\n"
		"int index = 1;\r\n"
		"while (cur != nullptr) {\r\n"
		"    if (i + 1 == " + to_string(index + 1) + ") {\r\n"
		"        Node* newNode = new Node(" + to_string(value) + ");\r\n"
		"        newNode->next = cur->next;\r\n"
		"        cur->next = newNode;\r\n"
		"        break;\r\n"
		"    }\r\n"
		"    ++i;\r\n"
		"    cur = cur->next;\r\n"
		"}\r\n"
		;
	return code;
}

string SinglyLinkedListPanel::GetDeleteCode(int index)
{
	string code =
		"Node * cur = head;\r\n"
		"int index = 1;\r\n"
		"while (cur->next != nullptr) {\r\n"
		"    if (index + 1 == " + to_string(index + 1) + ") {\r\n"
		"        Node* temp = cur->next\r\n"
		"        cur->next = cur->next->next\r\n"
		"        delete temp;\r\n"
		"        break;\r\n"
		"    }\r\n"
		"    ++index;\r\n"
		"    cur = cur->next;\r\n"
		"}\r\n";
	return code;
}

string SinglyLinkedListPanel::GetDeleteCodeByValue(int value)
{
	string code =
		"Node * cur = head;\r\n"
		"while (cur->next != nullptr) {\r\n"
		"    if (cur->next-value == " + to_string(value) + ") {\r\n"
		"        Node* temp = cur->next\r\n"
		"        cur->next = cur->next->next\r\n"
		"        delete temp;\r\n"
		"    }\r\n"
		"    cur = cur->next;\r\n"
		"}\r\n";
	return code;
}

string SinglyLinkedListPanel::GetFindCode(int value)
{
	string code =
		"Node * cur = head;\r\n"
		"while (cur != nullptr) {\r\n"
		"    if (cur->value == " + to_string(value) + ")\r\n"
		"        cout << cur->index;\r\n"
		"    cur = cur->next;\r\n"
		"}\r\n";
	return code;
}

void SinglyLinkedListPanel::SetCode(string code)
{
	wxString codeText(code);
	codeDisplay->SetLabel(codeText);
	codeDisplay->Refresh();
	codeDisplay->Update();
}

void SinglyLinkedListPanel::DrawInstruction(int lineNumber)
{
	int posx = 780;
	int posy = 102 + (lineNumber - 1) * 20;

	codeInstructor->Show();

	codeInstructor->SetPosition(wxPoint(posx, posy));
	codeInstructor->Refresh();
	codeInstructor->Update();

	Sleep(delay_time);
}

void SinglyLinkedListPanel::ReadFile()
{
	temp_values = new int[MAX_LENGTH];

	length = MAX_LENGTH;
	for (int i = 0; i < length; ++i)
		temp_values[i] = i + 1;

	ifstream input;
	input.open(DATA_FILE);
	if (!input) {
		input.close();
		return;
	}

	input >> length;
	for (int i = 0; i < length; ++i)
		input >> temp_values[i];

	input.close();
}

void SinglyLinkedListPanel::SaveFile()
{
	ofstream output;
	output.open(DATA_FILE, ios::out | ios::trunc);
	if (!output) {
		output.close();
		return;
	}
	output << length << "\n";
	for (int i = 0; i < length; ++i)
		output << array_boxes[i]->GetLabel() << " ";

	output.close();
}

// Widgets

void SinglyLinkedListPanel::CreateWidgets()
{
	// TITLE
	wxString titleText = wxString(name);
	title = new wxStaticText(this, wxID_ANY, titleText, wxPoint(275, 15));
	title->SetFont(headerFont);

	// ARRAY
	delete[] array_boxes;
	delete[] index_texts;
	array_boxes = new wxStaticText * [MAX_LENGTH];
	index_texts = new wxStaticText * [MAX_LENGTH];

	for (int i = 0; i < MAX_LENGTH; ++i) {
		int posX = START_X + BOX_WIDTH * i + ARROW_LENGTH * i;
		int posY = START_Y;
		int value = i >= length ? i : temp_values[i];

		// New box
		wxStaticText* box = new wxStaticText(this, wxID_ANY, to_string(value), wxPoint(posX, posY), wxSize(BOX_WIDTH, -1), wxALIGN_CENTER | wxBORDER_SIMPLE);
		box->SetFont(sllFont);
		box->SetForegroundColour(wxColour("Black"));
		array_boxes[i] = box;

		// New index text
		wxStaticText* index_text = new wxStaticText(this, wxID_ANY, to_string(i + 1), wxPoint(posX, posY - 30), wxSize(BOX_WIDTH, -1), wxALIGN_CENTER);
		index_texts[i] = index_text;

		if (i >= length) {
			box->Hide();
			index_text->Hide();
		}
	}
	delete[] temp_values;

	// MOVING BOX (WHEN ADD)
	addMovingBox = new wxStaticText(this, wxID_ANY, to_string(0), wxPoint(START_X, START_Y + 75), wxSize(BOX_WIDTH, -1), wxALIGN_CENTER | wxBORDER_SIMPLE);
	addMovingBox->SetFont(sllFont);
	ChangeTexture(addMovingBox, RED, LIGHT_YELLOW);
	addMovingBox->Hide();

	// CREATE
	createButton = new wxButton(this, wxID_ANY, "Create", wxPoint(50, 300));
	new wxStaticText(this, wxID_ANY, "Size", wxPoint(50, 350));
	sizeInput = new wxSpinCtrl(this, wxID_ANY, "", wxPoint(100, 350), wxSize(50, -1), wxALIGN_LEFT, MIN_LENGTH, MAX_LENGTH);

	createChoices.Add("Random");
	createChoices.Add("Empty");
	createRadioBox = new wxRadioBox(this, wxID_ANY, "", wxPoint(50, 400), wxDefaultSize, createChoices, 0, wxRA_SPECIFY_ROWS);
	createRadioBox->SetFont(smallFont);

	// UPDATE
	updateButton = new wxButton(this, wxID_ANY, "Update", wxPoint(200, 300));
	new wxStaticText(this, wxID_ANY, "Index", wxPoint(200, 350));
	updateIndex = new wxSpinCtrl(this, wxID_ANY, "", wxPoint(250, 350), wxSize(50, -1), wxALIGN_LEFT, MIN_LENGTH, MAX_LENGTH);
	new wxStaticText(this, wxID_ANY, "Value", wxPoint(200, 400));
	updateValue = new wxSpinCtrl(this, wxID_ANY, "", wxPoint(250, 400), wxSize(50, -1), wxALIGN_LEFT, MIN_VALUE, MAX_VALUE);

	// ADD
	addButton = new wxButton(this, wxID_ANY, "Add", wxPoint(350, 300));
	new wxStaticText(this, wxID_ANY, "Index", wxPoint(350, 350));
	addIndex = new wxSpinCtrl(this, wxID_ANY, "", wxPoint(400, 350), wxSize(50, -1), wxALIGN_LEFT, MIN_LENGTH, MAX_LENGTH);
	new wxStaticText(this, wxID_ANY, "Value", wxPoint(350, 400));
	addValue = new wxSpinCtrl(this, wxID_ANY, "", wxPoint(400, 400), wxSize(50, -1), wxALIGN_LEFT, MIN_VALUE, MAX_VALUE);
	
	addChoices.Add("Before");
	addChoices.Add("After");

	addRadioBox = new wxRadioBox(this, wxID_ANY, "", wxPoint(350, 450), wxSize(75, -1), addChoices, 0, wxRA_SPECIFY_ROWS);
	addRadioBox->SetFont(smallFont);

	// DELETE
	deleteButton = new wxButton(this, wxID_ANY, "Delete", wxPoint(500, 300));
	new wxStaticText(this, wxID_ANY, "Index", wxPoint(500, 350));
	deleteIndex = new wxSpinCtrl(this, wxID_ANY, "", wxPoint(550, 350), wxSize(50, -1), wxALIGN_LEFT, MIN_LENGTH, MAX_LENGTH);
	new wxStaticText(this, wxID_ANY, "Value", wxPoint(500, 400));
	deleteValue = new wxSpinCtrl(this, wxID_ANY, "", wxPoint(550, 400), wxSize(50, -1), wxALIGN_LEFT, MIN_VALUE, MAX_VALUE);

	deleteChoices.Add("Index");
	deleteChoices.Add("Value");
	
	deleteRadioBox = new wxRadioBox(this, wxID_ANY, "", wxPoint(500, 450), wxSize(75, -1), deleteChoices, 0, wxRA_SPECIFY_ROWS);

	// FIND
	findButton = new wxButton(this, wxID_ANY, "Find", wxPoint(650, 300));
	new wxStaticText(this, wxID_ANY, "Value", wxPoint(650, 350));
	findValue = new wxSpinCtrl(this, wxID_ANY, "", wxPoint(700, 350), wxSize(50, -1), wxALIGN_LEFT, MIN_VALUE, MAX_VALUE);

	// MENU
	int MENU_ID = 0;
	menuButton = new wxButton(this, MENU_ID, "Menu", wxPoint(10, 10));

	// CODE DISPLAY
	codeDisplay = new wxTextCtrl(this, wxID_ANY, "", wxPoint(800, 100), wxSize(300, 400), wxTE_MULTILINE | wxTE_READONLY);
	codeDisplay->SetFont(smallFont);
	SetCode(GetCreateCode());
	codeInstructor = new wxStaticText(this, wxID_ANY, "=>", wxPoint(15000, 15000));
	codeInstructor->SetFont(smallBoldFont);
}

// Binding Event

void SinglyLinkedListPanel::BindEventHandlers()
{
	this->Bind(wxEVT_PAINT, &SinglyLinkedListPanel::paintEvent, this);

	createButton->Bind(wxEVT_BUTTON, &SinglyLinkedListPanel::OnCreateButtonClicked, this);

	updateButton->Bind(wxEVT_BUTTON, &SinglyLinkedListPanel::OnUpdateButtonClicked, this);
	
	addButton->Bind(wxEVT_BUTTON, &SinglyLinkedListPanel::OnAddButtonClicked, this);
	
	deleteButton->Bind(wxEVT_BUTTON, &SinglyLinkedListPanel::OnDeleteButtonClicked, this);

	findButton->Bind(wxEVT_BUTTON, &SinglyLinkedListPanel::OnFindButtonClicked, this);
}

// Drawing 

void SinglyLinkedListPanel::OnPaint(wxDC& dc)
{
	// Clear all the arrows
	dc.Clear();

	int x1, x2;
	
	for (int i = 0; i < length; ++i) {
		x1 = START_X + BOX_WIDTH * (i + 1) + ARROW_LENGTH * i;
		x2 = x1 + ARROW_LENGTH;
		drawArrowHorizontal(dc, START_Y + BOX_WIDTH / 2, x1, x2, ARROW_THICKNESS);
	}
}

void SinglyLinkedListPanel::paintEvent(wxPaintEvent& evt)
{
	drawTool = new wxPaintDC(this);
	OnPaint(*drawTool);
}

void SinglyLinkedListPanel::paintNow()
{
	wxClientDC dc(this);
	OnPaint(dc);
}

void SinglyLinkedListPanel::drawArrowHorizontal(wxDC& dc, int y, int x1, int x2, int thickness)
{
	// draw an arrow

	dc.SetBrush(*wxBLACK_BRUSH);
	dc.SetPen(wxPen(wxColour("Black"), thickness));
	dc.DrawLine(x1, y, x2, y);;
	wxPoint list[3];

	// To the right
	if (x1 < x2) {
		list[0] = wxPoint(x2 - thickness * 3, y + thickness);
		list[1] = wxPoint(x2 - thickness * 3, y - thickness);
		list[2] = wxPoint(x2, y);
	}
	// To the left
	else {

		list[0] = wxPoint(x2 + thickness * 3, y + thickness);
		list[1] = wxPoint(x2 + thickness * 3, y - thickness);
		list[2] = wxPoint(x2, y);
	}

	dc.DrawPolygon(3, list);
}

// Event Handlers

void SinglyLinkedListPanel::OnCreateButtonClicked(wxCommandEvent& event)
{
	/* PREVENT MUTIPLE CALLS */
	static bool isBusy = false;

	if (isBusy) return;

	isBusy = true;
	createButton->Disable();

	int arr_length = sizeInput->GetValue();
	wxString type = createRadioBox->GetStringSelection();

	length = arr_length;

	for (int i = 0; i < MAX_LENGTH; ++i)  {

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

	// Draw arrows
	Refresh();
	OnPaint(*drawTool);
	delete drawTool;

	/* END OF PREVENT MULTIPLE CALLS */
	wxGetApp().Yield();
	createButton->Enable();

	isBusy = false;
}

void SinglyLinkedListPanel::OnUpdateButtonClicked(wxCommandEvent& event)
{
	/* PREVENT MUTIPLE CALLS */
	static bool isBusy = false;

	if (isBusy) return;

	isBusy = true;
	updateButton->Disable();

	int index = updateIndex->GetValue() - 1;

	if (index >= length) {
		wxLogMessage("The index greater than the length!");
	}
	else {
		int value = updateValue->GetValue();
		wxString valueText = wxString(to_string(value));

		SetCode(GetUpdateCode(index + 1, value));
		DrawInstruction(3);

		for (int i = 0; i <= index; ++i) {
			ChangeTexture(array_boxes[i], RED, LIGHT_YELLOW);

			DrawInstruction(4);

			if (i == index) {
				array_boxes[index]->SetLabel(valueText);
				ChangeTexture(array_boxes[index], BLUE, GREEN);

				DrawInstruction(5);
				DrawInstruction(6);
			}

			if (i != index) {
				DrawInstruction(8);
				DrawInstruction(9);
			}

			ChangeTexture(array_boxes[i], BLACK, LIGHT_GREY);

		}
	}

	codeInstructor->Hide();

	/* END OF PREVENT MULTIPLE CALLS */
	wxGetApp().Yield();
	updateButton->Enable();

	isBusy = false;
}

void SinglyLinkedListPanel::OnAddButtonClicked(wxCommandEvent& event)
{
	int index = addIndex->GetValue() - 1;

	if (length == MAX_LENGTH) {
		wxLogMessage("The length of the linked list has reached limit!");
		return;
	}

	if (index >= length) {
		wxLogMessage("The index is out of range");
		return;
	}

	/* PREVENT MUTIPLE CALLS */
	static bool isBusy = false;

	if (isBusy) return;

	isBusy = true;
	addButton->Disable();

	wxString type = addRadioBox->GetStringSelection();
	int value = addValue->GetValue();
	int posX, posY = START_Y + 75;

	wxString valueText = wxString(to_string(value));

	SetCode(GetAddCode(index + 1, value));

	addMovingBox->Show();
	addMovingBox->SetLabel(valueText);
	addMovingBox->Refresh();
	addMovingBox->Update();
	ChangePosition(addMovingBox, wxPoint(START_X, START_Y + 75));
	

	int ending = index;
	// Animation
	if (type.IsSameAs("Before")) {
		ending = index;
	}
	else if (type.IsSameAs("After")) {
		
		ending = index + 1;
	}

	DrawInstruction(3);

	for (int i = 0; i <= ending; ++i) {

		posX = START_X + BOX_WIDTH * i + ARROW_LENGTH * i;
		ChangePosition(addMovingBox, wxPoint(posX, posY));
		
		if (i != ending) {
			DrawInstruction(4);
			DrawInstruction(10);
			DrawInstruction(11);
		}
		else {
			DrawInstruction(4);
			DrawInstruction(5);
			DrawInstruction(6);
			DrawInstruction(7);
			DrawInstruction(8);
		}

		/*Sleep(delay_time);*/
	}

	addMovingBox->Hide();

	// Change data
	if (type.IsSameAs("Before")) {
		AddNewBoxesAt(index, addMovingBox);
	}
	else {
		AddNewBoxesAt(index + 1, addMovingBox);
	}

	codeInstructor->Hide();

	// Draw arrows
	Refresh();
	OnPaint(*drawTool);
	delete drawTool;

	/* END OF PREVENT MULTIPLE CALLS */
	wxGetApp().Yield();
	addButton->Enable();

	isBusy = false;
}

void SinglyLinkedListPanel::OnDeleteButtonClicked(wxCommandEvent& event)
{
	int index = deleteIndex->GetValue() - 1;
	int value = deleteValue->GetValue();
	int deleteNumber = 0;
	wxString type = deleteRadioBox->GetStringSelection();

	if (index >= length) {
		wxLogMessage("The index is out of range");
		return;
	}

	/* PREVENT MUTIPLE CALLS */
	static bool isBusy = false;

	if (isBusy) return;

	isBusy = true;
	deleteButton->Disable();

	isBusy = false;

	// Animation

	if (type.IsSameAs("Index")) {
		SetCode(GetDeleteCode(index + 1));
		
		DrawInstruction(3);

		for (int i = 0; i <= index; ++i) {
			ChangeTexture(array_boxes[i], RED, LIGHT_YELLOW);

			DrawInstruction(4);

			if (i != index) {
				DrawInstruction(10);
				DrawInstruction(11);
				ChangeTexture(array_boxes[i], BLACK, LIGHT_GREY);
			}
			else {
				ChangeTexture(array_boxes[i], BLUE, GREEN);
				DrawInstruction(5);
				DrawInstruction(6);
				DrawInstruction(7);
				DrawInstruction(8);
			}
		}

		Sleep(delay_time);

		ChangeTexture(array_boxes[index], BLACK, LIGHT_GREY);
	}
	
	else if (type.IsSameAs("Value")) {
		SetCode(GetDeleteCodeByValue(value));

		DrawInstruction(2);

		for (int i = 0; i < length; ++i) {

			ChangeTexture(array_boxes[i], RED, LIGHT_YELLOW);

			DrawInstruction(3);

			if (array_boxes[i]->GetLabel().IsSameAs(to_string(value))) {
				++deleteNumber;
				ChangeTexture(array_boxes[i], BLUE, GREEN);

				DrawInstruction(4);
				DrawInstruction(5);
				DrawInstruction(6);
			}
			else { 
				DrawInstruction(8);
				ChangeTexture(array_boxes[i], BLACK, LIGHT_GREY);
			}
		}

		Sleep(delay_time * 1.5);

		for (int i = 0; i < length; ++i) {
			ChangeTexture(array_boxes[i], BLACK, LIGHT_GREY);
		}
	}

	// Data update
	if (type.IsSameAs("Index")) {
		for (int i = index; i < length - 1; ++i) {
			array_boxes[i]->SetLabel(array_boxes[i + 1]->GetLabel());
		}

		array_boxes[length - 1]->Hide();
		index_texts[length - 1]->Hide();
		--length;
	}
	
	else if (type.IsSameAs("Value")) {
		int* temp = new int[length];

		// Copy a new array
		for (int i = 0; i < length; ++i) {
			temp[i] = stoi(string(array_boxes[i]->GetLabel().mb_str()));
		}

		// Delete elements with same value
		int new_length = 0;
		for (int i = 0; i < length; ++i) {
			if (temp[i] == value) continue;

			array_boxes[new_length++]->SetLabel(to_string(temp[i]));
		}

		for (int i = new_length; i < length; ++i) {
			array_boxes[i]->Hide();
			index_texts[i]->Hide();
		}

		length = new_length;

		delete[] temp;
	}

	codeInstructor->Hide();

	// Draw arrows
	Refresh();
	OnPaint(*drawTool);
	delete drawTool;

	/* END OF PREVENT MULTIPLE CALLS */
	wxGetApp().Yield();
	deleteButton->Enable();
}

void SinglyLinkedListPanel::OnFindButtonClicked(wxCommandEvent& event)
{
	/* PREVENT MUTIPLE CALLS */
	static bool isBusy = false;

	if (isBusy) return;

	isBusy = true;
	findButton->Disable();

	string value = to_string(findValue->GetValue());
	bool isFound = false;

	SetCode(GetFindCode(findValue->GetValue()));
	DrawInstruction(2);

	for (int i = 0; i < length; ++i) {
		ChangeTexture(array_boxes[i], RED, LIGHT_YELLOW);

		DrawInstruction(3);

		// If we can find the value
		if (array_boxes[i]->GetLabel().IsSameAs(value)) {

			ChangeTexture(array_boxes[i], BLUE, GREEN);
			isFound = true;

			DrawInstruction(4);
		}

		else {
			DrawInstruction(5);
			ChangeTexture(array_boxes[i], BLACK, LIGHT_GREY);
		}
	}

	if (!isFound) {
		wxString messageText = wxString("The number " + value + " cannot be found!");
		wxFormatString message = wxFormatString(messageText);
		wxLogMessage(message);
	}

	else {
		Sleep(found_delay_time);

		for (int i = 0; i < length; ++i) {
			ChangeTexture(array_boxes[i], BLACK, LIGHT_GREY);
		}
	}

	codeInstructor->Hide();

	/* END OF PREVENT MULTIPLE CALLS */
	wxGetApp().Yield();
	findButton->Enable();

	isBusy = false;
}

void SinglyLinkedListPanel::ChangeTexture(wxStaticText*& text, wxColour foregroundColour, wxColour backgroundColour)
{
	text->SetForegroundColour(foregroundColour);
	text->SetBackgroundColour(backgroundColour);
	text->Refresh();
	text->Update();
}

void SinglyLinkedListPanel::ChangePosition(wxStaticText*& text, wxPoint pos)
{
	text->SetPosition(pos);
	text->Refresh();
	text->Update();
}

void SinglyLinkedListPanel::AddNewBoxesAt(int index, wxStaticText*& newText)
{
	if (length == MAX_LENGTH) return;

	for (int i = length; i > index; --i) {
		array_boxes[i]->SetLabel(array_boxes[i - 1]->GetLabel());
		array_boxes[i]->Update();
	}
	array_boxes[index]->SetLabel(newText->GetLabel());
	array_boxes[index]->Update();

	array_boxes[length]->Show();
	array_boxes[length]->Update();
	index_texts[length]->Show();
	index_texts[length]->Update();
	++length;
}
