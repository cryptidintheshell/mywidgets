#include "window-functions.cpp"

Window::Window(const wxString &windowTitle, wxPoint windowPoint, wxSize windowSize) : 
wxFrame(NULL, wxID_ANY, windowTitle, windowPoint, windowSize) {
	pnlMain = new wxPanel(this, wxID_ANY);
	szrPnlMain = new wxBoxSizer(wxVERTICAL);

	// gallery panel
	pnlGallery = new wxPanel(pnlMain, wxID_ANY);
	szrPnlGallery = new wxBoxSizer(wxHORIZONTAL);
	pnlGallery->SetSizer(szrPnlGallery);

	// load the images
	GetImages("/home/movements/Pictures");

	// add the main panels
	szrPnlMain->Add(pnlGallery, 1, wxEXPAND | wxALL, 10);
	pnlMain->SetSizer(szrPnlMain);
}