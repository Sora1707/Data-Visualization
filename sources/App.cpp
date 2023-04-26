// App.cpp

#include "../headers/App.h"
#include "../headers/MainFrame.h"

bool App::OnInit() {
	MainFrame* mainFrame = new MainFrame("Data Structure");
	mainFrame->SetClientSize(1200, 600);
	mainFrame->Center();
	mainFrame->Show();
	return true;
}

wxIMPLEMENT_APP(App);