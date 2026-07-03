#include "../headers/window.h"

// #ifdef __WXGTK__
// #include <gtk/gtk.h>
// #include <gdk/gdkx.h>
// #include <X11/Xlib.h>
// #include <X11/Xatom.h>

// void SendToBottom(wxFrame* frame) {
//     GtkWidget* widget = frame->GetHandle(); // GtkWidget*
//     GdkWindow* gdkWin = gtk_widget_get_window(widget);
//     if (gdkWin) {
//         gdk_window_set_keep_below(gdkWin, TRUE);
//     }
// }
// #endif


void Window::GetImages(std::string targetPath) {
	fs::path path(targetPath);
	for (const auto & entry : fs::directory_iterator(path)) {
		if (!entry.is_directory()) {
			printf("%s\n", entry.path().c_str());
			AddImageToGallery(entry.path().c_str());
		} else GetImages(entry.path().c_str());
	}
}

// Computes a size that fits within maxW x maxH while preserving aspect ratio
wxSize FitWithinBounds(int origW, int origH, int maxW, int maxH) {
    if (origW <= 0 || origH <= 0) return wxSize(maxW, maxH);

    double scale = std::min(
        static_cast<double>(maxW) / origW,
        static_cast<double>(maxH) / origH
    );

    // don't upscale small images past their original size
    scale = std::min(scale, 1.0);

    int newW = std::max(1, static_cast<int>(origW * scale));
    int newH = std::max(1, static_cast<int>(origH * scale));
    return wxSize(newW, newH);
}

void Window::AddImageToGallery(std::string targetPath) {
	wxPanel* pnlBmp = new wxPanel(pnlGallery, wxID_ANY);
	wxBoxSizer* szrPnlBmp = new wxBoxSizer(wxHORIZONTAL);

	wxImage img;
	wxStaticBitmap* bmp;
    wxSize imgSize;

	if (img.LoadFile(targetPath, wxBITMAP_TYPE_ANY)) {
	    imgSize = img.GetSize();

	    const int THUMB_MAX_W = 150;
	    const int THUMB_MAX_H = 150;

	    wxSize thumbSize = FitWithinBounds(imgSize.GetWidth(), imgSize.GetHeight(), THUMB_MAX_W, THUMB_MAX_H);

	    std::cout << "\nfile: " << targetPath << "\n";
	    std::cout << "thumb size: " << thumbSize.GetWidth() << "x" << thumbSize.GetHeight() << "\n";
	    bmp = new wxStaticBitmap(pnlBmp, wxID_ANY, wxBitmap(img.Scale(thumbSize.GetWidth(), thumbSize.GetHeight(), wxIMAGE_QUALITY_HIGH)));

	} else {
	    bmp = new wxStaticBitmap(pnlBmp, wxID_ANY, wxArtProvider::GetBitmap(wxART_HARDDISK, wxART_OTHER, wxSize(100, 100)));
	}

	bmp->Bind(wxEVT_LEFT_UP, [=](wxMouseEvent &ev){
		wxSize popupSize = FitWithinBounds(imgSize.GetWidth(), imgSize.GetHeight(), 500, 300);

		wxFrame* frame = new wxFrame(this, wxID_ANY, "test", wxDefaultPosition, wxSize(popupSize.GetWidth(), popupSize.GetHeight()), wxNO_BORDER | wxFRAME_NO_TASKBAR);
		wxPanel* pnl = new wxPanel(frame, wxID_ANY);

		wxStaticBitmap* bmp2 = new wxStaticBitmap(pnl, wxID_ANY, wxBitmap(img.Scale(popupSize.GetWidth(), popupSize.GetHeight(), wxIMAGE_QUALITY_HIGH)));

    	wxBoxSizer* szr1 = new wxBoxSizer(wxHORIZONTAL);
		szr1->Add(bmp2, 1, wxEXPAND);
		pnl->SetSizer(szr1);

		wxBoxSizer* frameSizer = new wxBoxSizer(wxHORIZONTAL);
		frameSizer->Add(pnl, 1, wxEXPAND);
		frame->SetSizer(frameSizer);

		frame->Layout();
		frame->Show(true);
		// SendToBottom(frame);
	});

    szrPnlBmp->Add(bmp, 0);
    pnlBmp->SetSizer(szrPnlBmp);

    szrPnlGallery->Add(pnlBmp, 0,wxEXPAND | wxALL, 5);
    pnlGallery->Layout();
    pnlGallery->FitInside();
}