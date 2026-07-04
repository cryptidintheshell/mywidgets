#include <wx/wx.h>
#include <wx/grid.h>
#include <wx/sizer.h>
#include <wx/artprov.h>

#ifdef __WXGTK__
#include <gtk/gtk.h>
#include <gdk/gdkx.h> // Required for X11/Window Manager actions
#endif

#include <string.h>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <algorithm>

wxImage img;
wxSize imgSize;
wxSize popupSize;

wxSize FitWithinBounds(int origW, int origH, int maxW, int maxH) {
    if (origW <= 0 || origH <= 0) return wxSize(maxW, maxH);

    double scale = std::min(
        static_cast<double>(maxW) / origW,
        static_cast<double>(maxH) / origH
    );

    scale = std::min(scale, 1.0);

    int newW = std::max(1, static_cast<int>(origW * scale));
    int newH = std::max(1, static_cast<int>(origH * scale));
    return wxSize(newW, newH);
}

class FrameWindow : public wxFrame {
public:
    FrameWindow(const wxString &title, wxPoint position, wxSize size)
    : wxFrame(NULL, wxID_ANY, title, position, size, wxNO_BORDER | wxFRAME_NO_TASKBAR) {        
        wxPanel* pnl = new wxPanel(this, wxID_ANY);
        wxBoxSizer* szr1 = new wxBoxSizer(wxHORIZONTAL);

        wxStaticBitmap* bmp2 = new wxStaticBitmap(pnl, wxID_ANY, wxBitmap(img.Scale(popupSize.GetWidth(), popupSize.GetHeight(), wxIMAGE_QUALITY_HIGH)));
        szr1->Add(bmp2, 1, wxEXPAND);
        pnl->SetSizer(szr1);

        // Bind the show event to apply native window depth
        Bind(wxEVT_SHOW, &FrameWindow::OnShow, this);

        // Bind right-click on the image/panel to trigger the system menu
        // pnl->Bind(wxEVT_RIGHT_DOWN, &FrameWindow::OnRightClick, this);
        // bmp2->Bind(wxEVT_RIGHT_DOWN, &FrameWindow::OnRightClick, this);
    }

private:
    void OnShow(wxShowEvent& event) {
        if (event.IsShown()) {
			#ifdef __WXGTK__
	            GtkWidget* widget = this->GetHandle();
	            if (widget) {
	                GdkWindow* window = gtk_widget_get_window(widget);
	                if (window) {
	                    gdk_window_lower(window);
	                    gdk_window_set_keep_below(window, TRUE);
	                }
	            }
			#else
	            this->Lower();
			#endif
        } event.Skip();
    }

    // void OnRightClick(wxMouseEvent& event) {
	// 	#ifdef __WXGTK__
	//         GtkWidget* widget = this->GetHandle();
	//         if (widget) {
	//             GdkWindow* window = gtk_widget_get_window(widget);
	//             if (window) {
	//                 // Create a native GDK event structure mimicking the right click
	//                 GdkEvent* gdk_event = gdk_event_new(GDK_BUTTON_PRESS);
	//                 gdk_event->button.window = g_object_ref(window);
	//                 gdk_event->button.send_event = TRUE;
	//                 gdk_event->button.time = GDK_CURRENT_TIME;
	//                 gdk_event->button.x = event.GetX();
	//                 gdk_event->button.y = event.GetY();
	//                 gdk_event->button.button = 3; // 3 represents Right Mouse Button

	//                 // Ask the window manager to open the Alt+Space window settings menu
	//                 gdk_window_show_window_menu(window, gdk_event);

	//                 gdk_event_free(gdk_event);
	//                 return; // Event handled completely
	//             }
	//         }
	// 	#endif

    //     event.Skip();
    // }
};

class App : public wxApp {
    virtual bool OnInit() {
        wxInitAllImageHandlers();

        if (wxAppConsole::argc < 2) {
            std::cerr << "Error: No image path provided." << std::endl;
            return false;
        }

        if (img.LoadFile(wxAppConsole::argv[1], wxBITMAP_TYPE_ANY)) {
            imgSize = img.GetSize();
            popupSize = FitWithinBounds(img.GetWidth(), img.GetHeight(), 500, 300);

            FrameWindow *window = new FrameWindow("Window", wxDefaultPosition, wxSize(popupSize.GetWidth(), popupSize.GetHeight()));
            window->Show(true);
            return true;
        } else {
            return false;
        }
    }
};

wxIMPLEMENT_APP(App);