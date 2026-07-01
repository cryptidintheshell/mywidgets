#pragma once

#include <wx/wx.h>
#include <wx/grid.h>
#include <wx/filedlg.h>
#include <wx/sizer.h>
#include <wx/artprov.h>

#include <string.h>
#include <filesystem>
#include <thread>
#include <iostream>
#include <fstream>

const auto wxDP = wxDefaultPosition;
const auto wxDS = wxDefaultSize;
namespace fs = std::filesystem;

class Window : public wxFrame {

	wxPanel *pnlMain;
	wxBoxSizer* szrPnlMain;

	wxPanel *pnlGallery;
	wxBoxSizer* szrPnlGallery;

	public:
		Window(const wxString &windowTitle, wxPoint windowPoint, wxSize windowSize);
		void GetImages(std::string targetPath);
		void AddImageToGallery(std::string targetPath);
};