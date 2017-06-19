/*

 Class Name --- Preference
 
 Description - Application Preference Dialog.

 $Author: hiroshi $
 $Date: 2008/01/15 06:47:51 $
 $Revision: 1.5 $
 
 Copyrights 2007-2008 All Rights Reserved by HABE, aka ABE Hiroshi, JAPAN.

*/

#include "wx/valtext.h"
#include "wx/valgen.h"
#include <wx/notebook.h>

#include "preferences.h"

IMPLEMENT_CLASS( PrefDialog, wxDialog )
/*!
* PersonalRecordDialog event table definition
*/
BEGIN_EVENT_TABLE( PrefDialog, wxDialog )
    EVT_BUTTON(			wxID_OK,			PrefDialog::OnOK)
END_EVENT_TABLE()

PrefDialog::PrefDialog(wxWindow *parent, wxWindowID id, const wxString &title )
: wxDialog(parent, id, title, wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER ) {

	// let the all children window be validative.
	this->SetExtraStyle( wxWS_EX_VALIDATE_RECURSIVELY );

	// first of all, load config data
	LoadValues();
	
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	// Create top-level sizer
	wxBoxSizer* topLevel = new wxBoxSizer(wxVERTICAL);

	// make an instance of wxNotebook for setting preferences.
	wxNotebook* pref_notes = new wxNotebook( this, wxID_ANY, wxDefaultPosition, wxSize(-1,-1), wxNB_TOP );

	// make wxPanel instance for one page.
	wxPanel* _pref1panel = new wxPanel( pref_notes, wxID_ANY, wxDefaultPosition, wxSize(-1,-1), wxTAB_TRAVERSAL );
	// Sizer for notebook.
	wxBoxSizer* _panel1sizer = new wxBoxSizer(wxVERTICAL);

	// make sizer of _pref
	wxBoxSizer* _interval_sizer = new wxBoxSizer(wxHORIZONTAL);
	
	wxStaticText*	_interval_label	= new wxStaticText(_pref1panel, wxID_ANY, _T("Sampling Interval : "));
	wxSpinCtrl*		_num_interval	= new wxSpinCtrl( _pref1panel, ID_INTERVAL, _T("30"), wxDefaultPosition, wxSize(-1,-1), wxSP_ARROW_KEYS, 1, 100000, 100);
	_interval_sizer->Add( _interval_label, 0, wxALIGN_RIGHT|wxALL, 5 );
	_interval_sizer->Add( _num_interval, 0, wxALIGN_RIGHT|wxALL, 5 );
	_panel1sizer->Add( _interval_sizer, 1, wxEXPAND, 5 );

	wxBoxSizer* _button_sizer = new wxBoxSizer(wxHORIZONTAL);
	wxButton* okBtn = new wxButton( _pref1panel, wxID_OK, _T("&OK"), wxDefaultPosition, wxDefaultSize );
	wxButton* cancelBtn = new wxButton( _pref1panel, wxID_CANCEL, "&Cancel", wxDefaultPosition, wxDefaultSize );		
	_button_sizer->Add(okBtn, 0, wxALIGN_LEFT|wxALL, 5);
	_button_sizer->Add(cancelBtn, 0, wxALIGN_LEFT|wxALL, 5);
	_panel1sizer->Add(_button_sizer, 1, wxALIGN_RIGHT, 5);

	_pref1panel->SetSizer( _panel1sizer );

	pref_notes->AddPage( _pref1panel, wxT("Simulation"), true );

	
	topLevel->Add( pref_notes, 1, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
	
	SetSizer(topLevel);
	topLevel->Fit(this);
	topLevel->SetSizeHints(this);
	Layout();
	
	FindWindow(ID_INTERVAL)->SetValidator( wxGenericValidator( &m_interval ) );
}

void
PrefDialog::OnOK( wxCommandEvent& event ) {
	if ( Validate() && TransferDataFromWindow() )
	{
		if ( IsModal() )
			EndModal(wxID_OK); // If modal
		else
		{
			SetReturnCode(wxID_OK);
			this->Show(false); // If modeless
		}
	}
	SaveValues();
}

void
PrefDialog::LoadValues() {
	wxConfig* cfg = new wxConfig( wxTheApp->GetAppName() );
	if( !cfg->Read("Pref/interval", &m_interval) ) {
		m_interval = 30;
	}
//	std::cout << "LoadValues  : " << m_interval << std::endl;
	delete cfg;
}

int
PrefDialog::GetInterval() { return m_interval; }

void
PrefDialog::SaveValues() {
	wxConfig* cfg = new wxConfig( wxTheApp->GetAppName() );
	cfg->Write("Pref/interval", m_interval);
	delete cfg;
//	std::cout << "SaveValues  : " << m_interval << std::endl;
	
}
