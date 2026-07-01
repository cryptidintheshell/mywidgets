#include "../headers/app.h"
#include "../headers/window.h"

bool App::OnInit() {
	wxInitAllImageHandlers();
	Window* mainWindow = new Window("Pohto", wxDP, wxSize(1000, 600));
	mainWindow->Show(true);
	return true;
} wxIMPLEMENT_APP(App);