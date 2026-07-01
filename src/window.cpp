#include "../headers/window.h"

Window::Window(const wxString &windowTitle, wxPoint windowPoint, wxSize windowSize) : 
wxFrame(NULL, wxID_ANY, windowTitle, windowPoint, windowSize) {
	pnlMain = new wxPanel(this, wxID_ANY);
	szrPnlMain = new wxBoxSizer(wxVERTICAL);

	// gallery panel
	pnlGallery = new wxPanel(pnlMain, wxID_ANY);
	pnlGallery->SetBackgroundColour(wxColour(45, 45, 48));
	szrPnlGallery = new wxBoxSizer(wxHORIZONTAL);
	pnlGallery->SetSizer(szrPnlGallery);

	// load the images
	GetImages("/home/movements/Pictures");

	// add the main panels
	szrPnlMain->Add(pnlGallery, 1, wxEXPAND | wxALL, 10);
	pnlMain->SetSizer(szrPnlMain);
}

// move this to another file
void Window::GetImages(std::string targetPath) {
	fs::path path(targetPath);
	for (const auto & entry : fs::directory_iterator(path)) {
		if (!entry.is_directory()) {
			printf("%s\n", entry.path().c_str());
			AddImageToGallery(entry.path().c_str());
		} else GetImages(entry.path().c_str());
	}
}

// move this to another file
void Window::AddImageToGallery(std::string targetPath) {
	wxPanel* pnlBmp = new wxPanel(pnlGallery, wxID_ANY);
	wxBoxSizer* szrPnlBmp = new wxBoxSizer(wxHORIZONTAL);

	wxImage img;
	wxStaticBitmap* bmp;

    if (img.LoadFile(targetPath, wxBITMAP_TYPE_ANY)) 
    	bmp = new wxStaticBitmap(pnlBmp, wxID_ANY, wxBitmap(img.Scale(100, 100, wxIMAGE_QUALITY_HIGH)));
    else bmp = new wxStaticBitmap(pnlBmp, wxID_ANY, wxArtProvider::GetBitmap(wxART_HARDDISK, wxART_OTHER, wxSize(100, 100)));

    szrPnlBmp->Add(bmp, 0);
    pnlBmp->SetSizer(szrPnlBmp);

    szrPnlGallery->Add(pnlBmp, 0,wxEXPAND | wxALL, 5);
    pnlGallery->Layout();
    pnlGallery->FitInside();
}