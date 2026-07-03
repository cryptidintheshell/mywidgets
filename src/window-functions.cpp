#include "../headers/window.h"

void Window::GetImages(std::string targetPath) {
	fs::path path(targetPath);
	for (const auto & entry : fs::directory_iterator(path)) {
		if (!entry.is_directory()) {
			printf("%s\n", entry.path().c_str());
			AddImageToGallery(entry.path().c_str());
		} else GetImages(entry.path().c_str());
	}
}

void Window::AddImageToGallery(std::string targetPath) {
	wxPanel* pnlBmp = new wxPanel(pnlGallery, wxID_ANY);
	wxBoxSizer* szrPnlBmp = new wxBoxSizer(wxHORIZONTAL);

	wxImage img;
	wxStaticBitmap* bmp;

    if (img.LoadFile(targetPath, wxBITMAP_TYPE_ANY)) {
    	wxSize imgSize = img.GetSize();
    	// bmp = new wxStaticBitmap(pnlBmp, wxID_ANY, wxBitmap(img.Scale(imgSize.GetWidth()/2, imgSize.GetHeight()/2, wxIMAGE_QUALITY_HIGH)));    	
    	bmp = new wxStaticBitmap(pnlBmp, wxID_ANY, wxBitmap(img.Scale(200, 200, wxIMAGE_QUALITY_HIGH)));    	
    } else bmp = new wxStaticBitmap(pnlBmp, wxID_ANY, wxArtProvider::GetBitmap(wxART_HARDDISK, wxART_OTHER, wxSize(100, 100)));

	bmp->Bind(wxEVT_LEFT_UP, [=](wxMouseEvent &ev){
		wxFrame* frame = new wxFrame(this, wxID_ANY, "test", wxDefaultPosition, wxSize(500, 300), wxNO_BORDER | wxFRAME_NO_TASKBAR);
		wxPanel* pnl = new wxPanel(frame, wxID_ANY);
		wxStaticBitmap* bmp2 = new wxStaticBitmap(pnl, wxID_ANY, wxBitmap(img.Scale(500, 300, wxIMAGE_QUALITY_HIGH)));

		wxBoxSizer* szr1 = new wxBoxSizer(wxHORIZONTAL);
		szr1->Add(bmp2, 1, wxEXPAND);
		pnl->SetSizer(szr1);

		wxBoxSizer* frameSizer = new wxBoxSizer(wxHORIZONTAL);
		frameSizer->Add(pnl, 1, wxEXPAND);
		frame->SetSizer(frameSizer);

		frame->Layout();
		frame->Show(true);
	});

    szrPnlBmp->Add(bmp, 0);
    pnlBmp->SetSizer(szrPnlBmp);

    szrPnlGallery->Add(pnlBmp, 0,wxEXPAND | wxALL, 5);
    pnlGallery->Layout();
    pnlGallery->FitInside();
}