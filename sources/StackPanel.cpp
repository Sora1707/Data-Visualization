#include "../headers/StackPanel.h"
#include "../utility/Number/NumberUtil.h"
#include "../headers/App.h"
#include <wx/wx.h>
#include <string>
#include <fstream>

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#define DATA_FILE "data/Stack.txt"

using namespace std;

StackPanel::StackPanel(wxWindow* _parent) : wxPanel(_parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL)
{
	this->SetFont(normalFont);
	this->SetBackgroundColour(wxColour("LightGrey"));
	parent = _parent;

	ReadFile();
	CreateWidgets();
	BindEventHandlers();
}

void StackPanel::FreeMemory()
{
	delete drawTool;
	delete[] array_boxes;
}

string StackPanel::GetCreateCode()
{
	string code = "// Using linked list";
	return code;
}

string StackPanel::GetPopCode()
{
	string code =
		"if (length == 0)\r\n"
		"    return;\r\n"
		"Node* temp = head;\r\n"
		"head = head->next;\r\n"
		"delete temp;\r\n"
		"--length;\r\n"
		;
	return code;
}

string StackPanel::GetPushCode(int value)
{
	string code =
		"Node* newNode = new Node(" + to_string(value) + ")\r\n"
		"if (length == 0) \r\n"
		"    head = newNode;\r\n"
		"else {\r\n"
		"    newNode->next = head;\r\n"
		"    head = newNode;\r\n"
		"}\r\n"
		"++length;\r\n"
	;
	return code;
}

string StackPanel::GetClearCode()
{
	string code =
		"Node* cur = head\r\n"
		"while (cur != nullptr) {\r\n"
		"    Node* temp = cur;\r\n"
		"    cur = cur->next;\r\n"
		"    delete temp;\r\n"
		"};\r\n"
		"length = 0;\r\n"
		;
	return code;
}

void StackPanel::SetCode(string code)
{
	wxString codeText(code);
	codeDisplay->SetLabel(codeText);
	codeDisplay->Refresh();
	codeDisplay->Update();
}

void StackPanel::DrawInstruction(int lineNumber)
{
	int posx = 780;
	int posy = 102 + (lineNumber - 1) * 20;

	codeInstructor->Show();

	codeInstructor->SetPosition(wxPoint(posx, posy));
	codeInstructor->Refresh();
	codeInstructor->Update();

	Sleep(delay_time);
}

void StackPanel::ReadFile()
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

void StackPanel::SaveFile()
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

void StackPanel::CreateWidgets()
{
	// TITLE
	wxString titleText = wxString(name);
	title = new wxStaticText(this, wxID_ANY, titleText, wxPoint(350, 15));
	title->SetFont(headerFont);

	// ARRAY
	delete[] array_boxes;
	array_boxes = new wxStaticText * [MAX_LENGTH];

	for (int i = 0; i < MAX_LENGTH; ++i) {
		int posX = START_X + BOX_WIDTH * i + ARROW_LENGTH * i;
		int posY = START_Y;
		int value = i >= length ? i : temp_values[i];


		// New box
		wxStaticText* box = new wxStaticText(this, wxID_ANY, to_string(value), wxPoint(posX, posY), wxSize(BOX_WIDTH, -1), wxALIGN_CENTER | wxBORDER_SIMPLE);
		box->SetFont(sllFont);
		box->SetForegroundColour(wxColour("Black"));
		array_boxes[i] = box;

		if (i >= length) {
			box->Hide();
		}
	}
	delete[] temp_values;

	topText = new wxStaticText(this, wxID_ANY, "Top", wxPoint(START_X, START_Y - 30), wxSize(BOX_WIDTH, -1), wxALIGN_CENTER);
	topText->SetFont(smallFont);
	if (length == 0) topText->Hide();

	// ADD BOX (WHEN PUSH)
	addBox = new wxStaticText(this, wxID_ANY, to_string(0), wxPoint(START_X, START_Y + 75), wxSize(BOX_WIDTH, -1), wxALIGN_CENTER | wxBORDER_SIMPLE);
	addBox->SetFont(sllFont);
	ChangeTexture(addBox, RED, LIGHT_YELLOW);
	addBox->Hide();

	// RANDOM CREATE
	randomCreateButton = new wxButton(this, wxID_ANY, "Random", wxPoint(200, 475));

	// PUSH
	pushButton = new wxButton(this, wxID_ANY, "Push", wxPoint(200, 350));
	new wxStaticText(this, wxID_ANY, "Value", wxPoint(175, 400));
	pushValue = new wxSpinCtrl(this, wxID_ANY, "", wxPoint(250, 400), wxSize(100, -1), wxALIGN_LEFT, MIN_VALUE, MAX_VALUE);

	// POP
	popButton = new wxButton(this, wxID_ANY, "Pop", wxPoint(500, 350));

	// CLEAR
	clearButton = new wxButton(this, wxID_ANY, "Clear", wxPoint(500, 475));

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

void StackPanel::BindEventHandlers()
{
	this->Bind(wxEVT_PAINT, &StackPanel::paintEvent, this);

	pushButton->Bind(wxEVT_BUTTON, &StackPanel::OnPushButtonClicked, this);
	
	popButton->Bind(wxEVT_BUTTON, &StackPanel::OnPopButtonClicked, this);

	clearButton->Bind(wxEVT_BUTTON, &StackPanel::OnClearButtonClicked, this);

	randomCreateButton->Bind(wxEVT_BUTTON, &StackPanel::OnRandomCreateButtonClicked, this);
}
// Drawing 

void StackPanel::OnPaint(wxDC& dc)
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

void StackPanel::paintEvent(wxPaintEvent& evt)
{
	drawTool = new wxPaintDC(this);
	OnPaint(*drawTool);
}

void StackPanel::paintNow()
{
	wxClientDC dc(this);
	OnPaint(dc);
}

void StackPanel::drawArrowHorizontal(wxDC& dc, int y, int x1, int x2, int thickness)
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

void StackPanel::OnPushButtonClicked(wxCommandEvent& event)
{
	if (length == MAX_LENGTH) {
		wxLogMessage("The length of the stack has reached its limit!");
		return;
	}

	static bool isBusy = false;

	if (isBusy) return;
	
	// Prevent multiple calls
	isBusy = true;
	pushButton->Disable();

	// 
	int value = pushValue->GetValue();
	wxString valueText(to_string(value));

	SetCode(GetPushCode(value));
	DrawInstruction(1);

	addBox->Show();
	addBox->SetLabel(valueText);
	addBox->Refresh();
	addBox->Update();

	Sleep(delay_time);

	if (length == 0) {
		DrawInstruction(2);
		DrawInstruction(3);
	}
	else {
		DrawInstruction(2);
		DrawInstruction(4);
		DrawInstruction(5);
		DrawInstruction(6);
	}

	addBox->Hide();

	for (int i = length; i >= 1; --i) {
		array_boxes[i]->SetLabel(array_boxes[i - 1]->GetLabel());
	}
	array_boxes[0]->SetLabel(valueText);
	array_boxes[length]->Show();

	++length;
	topText->Show();

	codeInstructor->Hide();
	
	// Draw arrows
	Refresh();
	OnPaint(*drawTool);
	if (drawTool) delete drawTool;

	// End of prevent multiple calls;
	wxGetApp().Yield();
	pushButton->Enable();

	isBusy = false;
}

void StackPanel::OnPopButtonClicked(wxCommandEvent& event)
{
	if (length == 0) {
		wxLogMessage("The stack is empty!");
		return;
	}

	static bool isBusy = false;

	if (isBusy) return;

	// Prevent multiple calls
	isBusy = true;
	popButton->Disable();

	// Highlight the top element
	ChangeTexture(array_boxes[0], RED, LIGHT_YELLOW);

	SetCode(GetPopCode());
	DrawInstruction(3);

	Sleep(delay_time);

	// Remove Highlight
	DrawInstruction(4);

	ChangeTexture(array_boxes[0], BLACK, LIGHT_GREY);


	for (int i = 0; i < length - 1; ++i) {
		array_boxes[i]->SetLabel(array_boxes[i + 1]->GetLabel());
	}

	array_boxes[length - 1]->Hide();
	--length;

	if (length == 0) topText->Hide();
	else topText->Show();

	// Draw arrows
	Refresh();
	Update();
	OnPaint(*drawTool);
	if (drawTool) delete drawTool;

	DrawInstruction(5);

	codeInstructor->Hide();

	// End of prevent multiple calls
	wxGetApp().Yield();
	popButton->Enable();

	isBusy = false;

}

void StackPanel::OnClearButtonClicked(wxCommandEvent& event)
{
	if (length == 0) {
		wxLogStatus("The queue is empty!");
		return;
	}

	static bool isBusy = false;

	if (isBusy) return;

	// Prevent multiple calls
	isBusy = true;
	clearButton->Disable();

	SetCode(GetClearCode());
	DrawInstruction(1);

	while (length > 0) {
		ChangeTexture(array_boxes[0], RED, LIGHT_YELLOW);

		DrawInstruction(2);
		DrawInstruction(3);
		DrawInstruction(4);
		DrawInstruction(5);

		ChangeTexture(array_boxes[0], BLACK, LIGHT_GREY);

		for (int i = 0; i < length - 1; ++i) {
			array_boxes[i]->SetLabel(array_boxes[i + 1]->GetLabel());
		}

		array_boxes[length - 1]->Hide();

		--length;
	}

	topText->Hide();
	DrawInstruction(7);

	// Draw arrows
	Refresh();
	Update();
	OnPaint(*drawTool);
	if (drawTool) delete drawTool;

	codeInstructor->Hide();

	// End of prevent multiple calls
	wxGetApp().Yield();
	clearButton->Enable();

	isBusy = false;
}

void StackPanel::OnRandomCreateButtonClicked(wxCommandEvent& event)
{
	length = NumberUtil::getRandomInt(1, 10);
	for (int i = 0; i < MAX_LENGTH; ++i) {
		if (i >= length) {
			array_boxes[i]->Hide();
		}
		else {
			int value = NumberUtil::getRandomInt(1, 99);
			wxString valueText(to_string(value));
			array_boxes[i]->SetLabel(valueText);
			array_boxes[i]->Show();
		}
	}
	topText->Show();

	//// Draw arrows
	Refresh();
	Update();
	OnPaint(*drawTool);
	//if (drawTool) delete drawTool;
}

void StackPanel::ChangeTexture(wxStaticText*& text, wxColour foregroundColour, wxColour backgroundColour)
{
	text->SetForegroundColour(foregroundColour);
	text->SetBackgroundColour(backgroundColour);
	text->Refresh();
	text->Update();
}

void StackPanel::ChangePosition(wxStaticText*& text, wxPoint pos)
{
	text->SetPosition(pos);
	text->Refresh();
	text->Update();
}

void StackPanel::AddNewBoxesAt(int index, wxStaticText*& newText)
{
	if (length == MAX_LENGTH) return;

	for (int i = length; i > index; --i) {
		array_boxes[i]->SetLabel(array_boxes[i - 1]->GetLabel());
	}
	array_boxes[index]->SetLabel(newText->GetLabel());

	array_boxes[length]->Show();
	++length;
}
