/*

 Class Name --- BaseController
 
 Description - A class to controll and interfacing wxWidgets stuff and 
			the main simulation classes.

 $Author: habe $
 $Date: 2017/06/19 07:19:53 $
 $Revision: 1.34 $
 
 Copyrights 2006-2008 All Rights Reserved by HABE, aka ABE Hiroshi, JAPAN.

*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cstdlib>

#include <wx/wx.h>
#include <wx/timer.h>
#include <wx/sizer.h>
#include <wx/notebook.h>
#include <wx/panel.h>
#include <wx/spinctrl.h>
#include <wx/tglbtn.h>
#include <wx/slider.h>
#include <wx/datetime.h>
#include <wx/image.h>
#include <wx/dirdlg.h>
#include <wx/thread.h>
#include <wx/sysopt.h>

#include "Controller/base_controller.h"
#include "Controller/preferences.h"
#include "View/View3D.h"
#include "View/View2D.h"

#include <pthread.h>

const	wxString	BaseController::mv_version = wxT("Particula Numerica 0.32 beta");

BEGIN_EVENT_TABLE(BaseController, wxFrame)
    EVT_MENU(			wxID_EXIT,			BaseController::OnExit)
    EVT_MENU(			wxID_ABOUT,			BaseController::OnAbout)
	EVT_MENU(			wxID_PREFERENCES,	BaseController::OnPreferences)
    EVT_MENU(			ID_EXIT,			BaseController::OnExit)
//	EVT_IDLE(								BaseController::OnIdle)
    EVT_TIMER(			ID_TIMER,			BaseController::OnTimer)
	EVT_TOGGLEBUTTON(	ID_P_TGLBTN,		BaseController::OnTglBtn)
	EVT_BUTTON(			ID_RESET_BTN,		BaseController::OnResetBtn)
	EVT_SLIDER(			ID_TEMPERATURE,		BaseController::OnTempSlider)
	EVT_BUTTON(			ID_SIM_INITBTN,		BaseController::OnSimInitBtn)
	EVT_BUTTON(			ID_PHYS_INITBTN,	BaseController::OnPhysInitBtn)
	EVT_BUTTON(			ID_PHYS_SAVEBTN,	BaseController::OnPhysSaveBtn)
	EVT_BUTTON(			ID_INIT_ADD_BTN,	BaseController::OnAddBtn)
	EVT_BUTTON(			ID_INIT_DEL_BTN,	BaseController::OnDelBtn)
	EVT_BUTTON(			ID_INIT_CHG_BTN,	BaseController::OnChgBtn)
	EVT_LISTBOX(		ID_INIT_ADD_LIST,	BaseController::OnListBox)
//	EVT_BUTTON(			ID_LOADBTN,			BaseController::OnLoadBtn)
	EVT_BUTTON(			ID_RECBTN,			BaseController::OnRecordBtn)
	EVT_CHOICE(			ID_CHOICE,			BaseController::OnChoice)
	EVT_CHOICE(			ID_INIT_CHOICE,		BaseController::OnInitChoice)
END_EVENT_TABLE()

void
BaseController::InitSimulation() {
	// set the interval 
	mv_timer.Start(33);
	m_is_stopped = true;
}

void
BaseController::Start() {
	// check if the total time to simulate is begger than the current time
	if( m_world_ctlr.GetTime() >= m_world_ctlr.GetTotalTime() ) {
		mv_start_btn->SetValue(false);
		return;
	}

	wxDateTime now = wxDateTime::Now();
#ifdef JAPANESE
	*mv_log << _T("シミュレーション開始 (");
#else
	*mv_log << _T("The Simulation start now (");
#endif
	*mv_log << now.Format("%c)\n");

	int nn = mv_2dview->GetNumTrack();
	m_world_ctlr.SetTrackParticle( nn );
	char cbuf[64];
	sprintf( cbuf, "Track Particle : %d\n", nn );
	*mv_log << cbuf;

	m_world_ctlr.Start();
	m_is_stopped = false;

	mv_start_btn->SetLabel( _T("Stop") );
	mv_control_note->Disable();
	mv_reset_btn->Disable();
	
	GetMenuBar()->Enable(wxID_PREFERENCES,false);
	
	ClearPanelBuffer();
}

void
BaseController::Stop() {
	if( m_world_ctlr.IsSaveMovie() ) {
		// OnTimer is exclusively run with Main Thread so OnTimer is never called while "Stop"ping.
		if(!m_world_ctlr.IsDrawn()) SaveFrame();
		m_world_ctlr.SetDrawn(true);
		m_world_ctlr.CondSignal();
	}
	m_world_ctlr.Stop();
	m_is_stopped = true;
	wxDateTime now = wxDateTime::Now();
	*mv_log << _T("The Simulation is stopped (");
	*mv_log << now.Format("%c)\n");

	mv_start_btn->SetLabel( _T("Restart") );
	mv_control_note->Enable();
	mv_start_btn->SetValue(false);
	mv_reset_btn->Enable();
	
	GetMenuBar()->Enable(wxID_PREFERENCES, true);

}

wxMenuBar*
BaseController::NewMenuBar( int id ) {
	wxMenuBar* _menuBar = new wxMenuBar( 0 );
	wxMenu* _fileMenu = new wxMenu();
	wxMenu* _helpMenu = new wxMenu();

    _helpMenu->Append(wxID_ABOUT, _T("&About...\tF2"), _T("Show about dialog"));
	_helpMenu->Append(wxID_PREFERENCES, _T("Preferences..."), _T("Preferences Dialog"));

    _menuBar->Append(_helpMenu, _T("&Help"));


	wxMenuItem* exit = new wxMenuItem( _fileMenu, ID_EXIT, wxString( wxT("e&Xit") ) , wxT(""), wxITEM_NORMAL );
	_fileMenu->Append( exit );
	_menuBar->Append( _fileMenu, wxT("File") );

	wxMenu* _editMenu = new wxMenu();
	wxMenuItem* _cutedit = new wxMenuItem( _editMenu, ID_DEFAULT, wxString( wxT("&Cut") ) + wxT('\t') + wxT("ALT-X"), wxT(""), wxITEM_NORMAL );
	_editMenu->Append( _cutedit );
	wxMenuItem* _copyedit = new wxMenuItem( _editMenu, ID_DEFAULT, wxString( wxT("&Copy") ) + wxT('\t') + wxT("ALT-C"), wxT(""), wxITEM_NORMAL );
	_editMenu->Append( _copyedit );
	_menuBar->Append( _editMenu, wxT("&Edit") );

	wxMenu* _optionMenu = new wxMenu();
	wxMenuItem* _option = new wxMenuItem( _optionMenu, ID_DEFAULT, wxString( wxT("O&ption") ) , wxT(""), wxITEM_NORMAL );
	_optionMenu->Append( _option );
	_menuBar->Append( _optionMenu, wxT("&Option") );

//	_menuBar->Enable( ID_EXIT, false );

	return _menuBar;
}

void
BaseController::OnPhysSaveBtn( wxCommandEvent &event ) {
    wxString path = wxFileSelector(
								   _T("Select the file to save"),
								   wxEmptyString, wxEmptyString,
								   _T(""),
								   wxString::Format
								   (
									_T("NEMO Ascii (*.ascii)|*.ascii|NEMO text (*.txt)|*.txt|All files (%s)|%s"),
									wxFileSelectorDefaultWildcardStr,
									wxFileSelectorDefaultWildcardStr
                                    ),
								   wxFD_SAVE,
								   this
                                   );
    if ( !path )
        return;
	m_world_ctlr.SaveNEMO( path.c_str() );
}

void
BaseController::OnChoice( wxCommandEvent &event ) {
}

void
BaseController::OnAddBtn( wxCommandEvent &event ) {

	
	try {
		m_init_func_selected->GetValues();
	}
	catch(const char* m) {
		wxMessageDialog msg( this, m, "Error has occured.", wxOK );
		msg.ShowModal();
		return;
	}
	int sel = mv_initial_listbox->GetSelection();
	wxString _item = GenListBoxItem();
	wxString _act;

	if( sel == wxNOT_FOUND ) {
		mv_initial_listbox->Append( _item );
		m_init_func_list.push_back( m_init_func_selected );
		_act = _T(" is Appended.\n");
	} else {
		m_init_func_list.insert( m_init_func_list.begin()+sel, m_init_func_selected );
		mv_initial_listbox->Insert( _item, sel );
		_act = _T("is Inserted.\n");
	}
	

	mv_initial_del_btn->Disable();
	mv_initial_chg_btn->Disable();
	
	GenDefaultInitPanel();
	m_world_ctlr.SetTime(0.0);
	
	mv_ctrl_panel->Refresh();

	
}

void
BaseController::OnDelBtn( wxCommandEvent &event ) {
	int sel = mv_initial_listbox->GetSelection();
	if( sel == wxNOT_FOUND ) {
		wxMessageDialog msg( this, "Nothing is done.", "List Item is NOT Selected.", wxOK );
		msg.ShowModal();
	} else {
		mv_initial_listbox->Delete( sel );
		m_init_func_list.erase( m_init_func_list.begin()+sel );
		*mv_log << _T("Item ") << sel << _T(" is deleted.\n");
		m_world_ctlr.SetTime(0.0);
	}
	GenDefaultInitPanel();
	mv_initial_add_btn->Enable();
	mv_initial_del_btn->Disable();
	mv_initial_chg_btn->Disable();
	if( m_init_func_list.size() == 0 ) mv_phys_save_btn->Disable();
	mv_ctrl_panel->Refresh();

	return;
}

void
BaseController::OnChgBtn( wxCommandEvent &event ) {
	int sel = mv_initial_listbox->GetSelection();
	if( sel == wxNOT_FOUND ) {
		wxMessageDialog msg( this, "Nothing is done.", "List Item is NOT Selected.", wxOK );
		msg.ShowModal();
		return;
	} else {
		m_init_func_selected = m_init_func_list[sel];
		*mv_log << _T("Item ") << sel << _T(" is changed.\n");
	}
	//
	// data of the selected func is about to be loaded.
	try {
		m_init_func_selected->GetValues();
	}
	catch(const char* m) {
		wxMessageDialog msg( this, m, "Error has occured.", wxOK );
		msg.ShowModal();
		return;
	}
	
	wxString _item = GenListBoxItem();
	mv_initial_listbox->Delete( sel );
	mv_initial_listbox->Insert( _item, sel );
	mv_initial_listbox->SetSelection( wxNOT_FOUND );
	
	GenDefaultInitPanel();
	m_world_ctlr.SetTime(0.0);
	mv_initial_add_btn->Enable();
	mv_initial_del_btn->Disable();
	mv_initial_chg_btn->Disable();
	
	m_phys_inited = true;
	mv_ctrl_panel->Refresh();
}

void
BaseController::OnSimInitBtn( wxCommandEvent &event ) {
	InitSimParam();
}

void
BaseController::InitSimParam() {

	wxString st = mv_sim_time->GetValue();
	const char* sss = st.c_str();
	scalarType _total_time;
	sscanf( st.c_str(), "%e", &_total_time );
	m_world_ctlr.SetTotalTime( _total_time );
//	m_world_ctlr.SetTime( 0.0 );
	m_world_ctlr.SetNumThread( mv_sim_threads->GetValue() );
	m_world_ctlr.SetDeltaT( 0.01 );
	int sel = mv_sim_method->GetSelection();
	if( sel >= 0 ) {
		m_world_ctlr.SetCalcMethod( sel );
	} else {
		wxMessageDialog msg( this, "Select Calculation Method", "Error Has Occurred", wxOK );
		msg.ShowModal();
		return;
	}
	m_sim_inited = true;
	*mv_log << _T("Total Time to simulate is ") + st + _T("\n");
	*mv_log << _T("Delta T is 0.01\n");
	int nn;
	*mv_log << _T("Numerical Method is "); 
	*mv_log << m_world_ctlr.GetCalcMethods(nn)[sel].c_str();
	*mv_log << _T("\n");
}

void
BaseController::OnPhysInitBtn( wxCommandEvent &event ) {
	int n_list = m_init_func_list.size();
	if( n_list == 0 ) {
		wxMessageDialog msg( this, "Add Distribution Model", "Error Has Occurred", wxOK );
		msg.ShowModal();
		return;
	}
	InitPhysParam();
}

void
BaseController::InitPhysParam() {
	int n_list = m_init_func_list.size();
	int np = 0;
	m_world_ctlr.ResizeId(0);
	for( int i = 0; i < n_list; i++ ) {
		int ns = m_init_func_list[i]->GetNum();
		np += ns;
		m_world_ctlr.PushId(ns);
		m_init_func_list[i]->Generate();
	}

	m_world_ctlr.SetNumParticles( np );

	int it = 0;
	for( int i = 0; i < n_list; i++ ) {
		int ns = m_init_func_list[i]->GetNum();
		for( int n = 0; n < ns; n++ ) {
			m_world_ctlr.SetMass(		it, m_init_func_list[i]->GetMass(n) );
			m_world_ctlr.SetLocation(	it, m_init_func_list[i]->GetLoc(n) );
			Locus v = m_init_func_list[i]->GetVel(n);
			m_world_ctlr.SetVelocity(	it, v  );
//			std::cout << "(Vx,Vy,Vz) = (" << v.x << ", " << v.y << ", " << v.z << ") at " << it << std::endl;

			it++;
		}
	}	

	char cbuf[64];
	double t = m_world_ctlr.GetTime();
	sprintf( cbuf, "Time = %10.4f\n", t );
	mv_sim_time_label->SetLabel( cbuf );
	mv_3dview->ShowTime( cbuf );
	mv_3dview->ReDraw( m_world_ctlr );
	mv_2dview->SetNumParticles( np );
	mv_2dview->ResetTrajectory();
	mv_2dview->ResetEnergyPlot();
	
	m_phys_inited = true;
	
	mv_phys_save_btn->Enable();
	
	sprintf( cbuf, "The Num of Particles is %d\n", np );
	*mv_log << cbuf;
}

void
BaseController::OnRecordBtn( wxCommandEvent& event ) {
	static wxString dirCurr = _T("");

	if( m_world_ctlr.IsSaveMovie() ) {
		m_world_ctlr.EnableGLSaveMovie( false );
		m_world_ctlr.SetDrawn( false );
		m_world_ctlr.ReleaseWaitDrawn();
		mv_rec_btn->SetLabel(_T("Off"));
		*mv_log << "Recording Movie File is stopped\n";
	} else {
		wxDirDialog dialog( this, _T("Select Directory to Save Movie Images"), dirCurr, wxDD_NEW_DIR_BUTTON );
		
		if( dialog.ShowModal() == wxID_OK ) {
			m_world_ctlr.EnableGLSaveMovie( true );
			dirCurr = dialog.GetPath() + wxFILE_SEP_PATH;
			*mv_log << "Recording Directory is set : " + dirCurr + "\n";
		} else {
			*mv_log << "Setting Recording Directory Canceled\n";
			return;
		}
        m_world_ctlr.SetSaveFolder( std::string(dirCurr.mb_str()) );
		mv_rec_btn->SetLabel(_T("On"));
		
//		mv_img_worker = new MyWorkerThread( m_world_ctlr, *this );
//		mv_img_worker->Create();
//		mv_img_worker->Run();
	}
}

void
BaseController::OnTglBtn( wxCommandEvent &event ) {
	if( event.GetInt() == 0 ) {
		Stop();
//		mv_start_btn->SetLabel( _T("Start") );
//		mv_control_note->Enable();
	} else {
		if( !m_sim_inited ) {
			wxMessageDialog msg( this, "Simulation Parameters are not set.\nClick Set Data", "Error Has Occured", wxOK );
			msg.ShowModal();
			
			// Select Sim Ctrl Panel
			mv_control_note->SetSelection(0); 
			mv_start_btn->SetValue(false);
			return;
		}
		if( !m_phys_inited ) {
			wxMessageDialog msg( this, "Physical Parameters are not set.\nClick Set Data", "Error Has Occurred", wxOK );
			msg.ShowModal();

			// Select Physical Panel
			mv_control_note->SetSelection(1);
			mv_start_btn->SetValue(false);
			return;
		}
		mv_reset_btn->Enable();
		Start();
	}
}

void
BaseController::OnResetBtn( wxCommandEvent &event ) {
	m_world_ctlr.SetTime(0.0);
	InitSimParam();
	InitPhysParam();

	mv_start_btn->SetLabel( _T("Start") );
//	mv_reset_btn->Disable();
}

void
BaseController::OnTempSlider(wxCommandEvent& event) {
	std::ostringstream strem;
	strem.setf(std::ios_base::fixed);
	strem.precision(4);
	strem << "Temp.:" << mv_temperature->GetValue()*c_temp_ratio;
	std::string num = strem.str();

	mv_temperature_label->SetLabel(num.c_str());
}

void
BaseController::OnInitChoice(wxCommandEvent& event) {
	int sel = mv_initial_choice->GetSelection();
	mv_initial_panel_sizer->Detach( mv_initial_panel );
	mv_initial_panel->Destroy();
	
	m_init_func_selected = DistPanelSp( GenDistPanel( sel ) );
	mv_initial_panel = m_init_func_selected->CreatePanel( mv_phys_ctrl_panel );
	mv_initial_panel_sizer->Add( mv_initial_panel, 0, wxALL|wxEXPAND, 5);
//	mv_phys_ctrl_panel->SetSizer( mv_phys_ctrl_sizer );
	mv_phys_ctrl_sizer->Layout();

	mv_ctrl_panel->Layout();
	mv_ctrl_panel->Refresh();
	mv_initial_del_btn->Disable();
	mv_initial_chg_btn->Disable();
	mv_initial_add_btn->Enable();
	mv_initial_listbox->Deselect( mv_initial_listbox->GetSelection() );
	
}

void
BaseController::OnListBox(		wxCommandEvent& event ) {
	int sel = event.GetInt();

//	std::cout << "OnListBox" << std::endl;

	if( sel == wxNOT_FOUND ) {
		wxMessageDialog msg( this, "List Item is NOT Selected.", "Error has occured.", wxOK );
		msg.ShowModal();
		mv_initial_del_btn->Disable();
		mv_initial_chg_btn->Disable();
		return;
	}
	m_init_func_selected = m_init_func_list[sel];
//	std::cout << "OnListBox: " << m_init_func_selected->WhoAmI() << std::endl;

	// change the panel.
	mv_initial_panel_sizer->Detach( mv_initial_panel );
	mv_initial_panel->Destroy();
	mv_initial_panel = m_init_func_selected->CreatePanel( mv_phys_ctrl_panel );
	mv_initial_panel_sizer->Add( mv_initial_panel, 0, wxALL|wxEXPAND, 5 );

	mv_initial_choice->SetSelection(GetListNumber(m_init_func_selected->WhoAmI()));

	mv_initial_del_btn->Enable();
	mv_initial_chg_btn->Enable();
	mv_initial_add_btn->Disable();

	mv_phys_ctrl_sizer->Layout();
	mv_phys_ctrl_panel->Update();
	mv_ctrl_panel->Refresh();

}

void
BaseController::OnTimer(wxTimerEvent& event) {

	static bool _is_stopped = true;
	
	char cbuf[64];
	double t = m_world_ctlr.GetTime();
	sprintf( cbuf, "Time = %10.4f\n", t );

	if( m_world_ctlr.IsRunning() ) {

		mv_sim_time_label->SetLabel( cbuf );
		
		mv_3dview->ShowTime( cbuf );
		mv_3dview->ReDraw(m_world_ctlr);
		mv_2dview->UpdateAll();

		if( m_world_ctlr.IsSaveMovie() && !m_world_ctlr.IsDrawn() ) {
			SaveFrame();
			m_world_ctlr.SetDrawn(true);
			m_world_ctlr.CondSignal();
		}

		_is_stopped = false;
				
	} else if( !_is_stopped ) {
		if( !m_is_stopped ) Stop();
		mv_sim_time_label->SetLabel( cbuf );
		
		mv_3dview->ShowTime( cbuf );
		mv_3dview->ReDraw(m_world_ctlr);
		mv_2dview->UpdateAll();

		_is_stopped = true;
	}
}
/*
void
BaseController::OnIdle( wxIdleEvent& event ) {

//	std::cout << "OnIdle is called" << std::endl;

	if( m_world_ctlr.IsWaitDrawn() ) {
		//			std::cout << "OnTime is wait and IsDrawn is " << m_world_ctlr.IsDrawn() << std::endl;
		SaveFrame();
		//			std::cout << "SaveFramed" << std::endl;
		m_world_ctlr.SetDrawn(true);
		//			std::cout << "OnTimer Released" << std::endl;
	}

	//		if( m_world_ctlr.IsSaveMovie() ) SaveFrame();
	
	event.Skip();

}
*/

void
BaseController::OnExit(wxCommandEvent& event) {
    // true is to force the frame to close
	
//	std::cout << "OnExit" << std::endl;

	if( m_world_ctlr.IsRunning() ) {
		wxMessageDialog msg( this, "Stop Simulation ?", "Simulation is still running.", wxYES_NO|wxNO_DEFAULT );
		switch( msg.ShowModal() ) {
			case wxID_YES:
				Stop();
				Close();
				break;
			case wxID_NO:
				break;
			default:
				break;
		}
	} else {
		Close(true);
	}
	return;
}

void
BaseController::OnAbout( wxCommandEvent& WXUNUSED(event) ) {
//	std::cout << "OnAbout is called" << std::endl;
	wxString msg = mv_version;
	msg += _T("\nAuthor: HABE (c) 2006-2008\nThanks for wxWidgets, \n");
#if defined(WIN32)
	msg += _T("pthreads-win32, GLUT for Win32, \n");
#endif
	msg += _T("mathplot and STL");
	(void)wxMessageBox(msg, _T("About Particula Numerica"));
}


void
BaseController::OnPreferences( wxCommandEvent& WXUNUSED(event) ) {
	PrefDialog dialog(this, wxID_DEFAULT, "Preferences");
	if (dialog.ShowModal() == wxID_OK) {
		m_world_ctlr.SetSampleInterval( dialog.GetInterval() );
		*mv_log << "Sample Interval is " << dialog.GetInterval() << _T("\n");
	}
}

void
BaseController::SaveFrame() const {
	static int _id = 0;
	
	// get the wxImage from GL Buffer.
	wxImage* img = mv_3dview->GetGLBuffer();
	
	char _fn[10];
	sprintf(_fn, "%04d", _id );
	std::string _fullpath = m_world_ctlr.GetSaveFolder() + _fn + ".png";
	img->SaveFile(  wxString(_fullpath.c_str(),wxConvUTF8), wxBITMAP_TYPE_PNG );
	
	delete img;
	_id++;
}

wxPanel* 
BaseController::SimControlPanel( wxWindow* parent, int id )	{
	wxPanel* _sim_ctrl_panel;
	_sim_ctrl_panel = new wxPanel( parent, ID_DEFAULT, wxDefaultPosition, wxSize(-1,-1), wxTAB_TRAVERSAL );
	_sim_ctrl_panel->SetMinSize( wxSize(-1,-1) );
	wxBoxSizer* _sim_ctrl_sizer;
	_sim_ctrl_sizer = new wxBoxSizer( wxVERTICAL );
	
	mv_sim_set_btn = new wxButton( _sim_ctrl_panel, ID_SIM_INITBTN, wxT("Set Data"), wxDefaultPosition, wxDefaultSize, 0 );
	_sim_ctrl_sizer->Add( mv_sim_set_btn, 0, wxALL, 5 );
	
	wxStaticText* _sim_time_label = new wxStaticText( _sim_ctrl_panel, ID_DEFAULT, wxT("Time to simulate"), wxDefaultPosition, wxDefaultSize, 0 );
	_sim_ctrl_sizer->Add( _sim_time_label, 0, wxALIGN_RIGHT|wxALL, 5 );
	
	mv_sim_time = new wxTextCtrl( _sim_ctrl_panel, ID_DEFAULT, wxT("100.000"), wxDefaultPosition, wxDefaultSize, 0 );
	_sim_ctrl_sizer->Add( mv_sim_time, 0, wxALIGN_RIGHT|wxALL, 5 );
	
	wxStaticText* _sim_method_label = new wxStaticText( _sim_ctrl_panel, ID_DEFAULT, wxT("Calculation Method"), wxDefaultPosition, wxDefaultSize, 0 );
	_sim_ctrl_sizer->Add( _sim_method_label, 0, wxALIGN_RIGHT|wxALL, 5 );
	
	int num_method;
	strArray _methods = m_world_ctlr.GetCalcMethods(num_method);
	wxString* mv_sim_methodChoices = new wxString[ num_method ];
	for( int i = 0; i < num_method; i++ ) mv_sim_methodChoices[i] = _methods[i].c_str();
	mv_sim_method = new wxChoice( _sim_ctrl_panel, ID_DEFAULT, wxDefaultPosition, wxDefaultSize, num_method, mv_sim_methodChoices, 0 );
	_sim_ctrl_sizer->Add( mv_sim_method, 0, wxALIGN_RIGHT|wxALL, 5 );
	mv_sim_method->Select(0);
	
	wxStaticText* _sim_thread_label = new wxStaticText( _sim_ctrl_panel, ID_DEFAULT, wxT("Num. of Threads"), wxDefaultPosition, wxDefaultSize, 0 );
	_sim_ctrl_sizer->Add( _sim_thread_label, 0, wxALIGN_RIGHT|wxALL, 5 );
	
	mv_sim_threads = new wxSpinCtrl( _sim_ctrl_panel, wxID_ANY, wxT("1"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 10, 2);
	_sim_ctrl_sizer->Add( mv_sim_threads, 0, wxALIGN_RIGHT|wxALL, 5 );
    mv_sim_threads->Enable(false);
	
	wxStaticText* _sim_rec_label = new wxStaticText( _sim_ctrl_panel, ID_DEFAULT, wxT("Record Movie"), wxDefaultPosition, wxDefaultSize, 0 );
	_sim_ctrl_sizer->Add( _sim_rec_label, 0, wxALIGN_RIGHT|wxALL, 5 );

	mv_rec_btn = new wxButton( _sim_ctrl_panel, ID_RECBTN, wxT("Off"), wxDefaultPosition, wxDefaultSize, 0);
	_sim_ctrl_sizer->Add( mv_rec_btn, 0, wxALIGN_RIGHT|wxALL, 5 );
	
	_sim_ctrl_panel->SetSizer( _sim_ctrl_sizer );
	_sim_ctrl_panel->Layout();
	_sim_ctrl_sizer->Fit( _sim_ctrl_panel );
	
	return _sim_ctrl_panel;
}

wxPanel*
BaseController::PhysControlPanel( wxWindow* parent, int id ) {

	wxPanel* _phys_ctrl_panel = new wxPanel( parent, ID_DEFAULT, wxDefaultPosition, wxSize(-1,-1), wxTAB_TRAVERSAL );
	mv_phys_ctrl_sizer = new wxBoxSizer( wxVERTICAL );
//	_phys_ctrl_panel->SetMinSize( wxSize( -1,640 ) );

	wxBoxSizer* _d_sizer = new wxBoxSizer(wxHORIZONTAL);
	_d_sizer->SetMinSize( wxSize(256,-1) );
	mv_phys_set_btn = new wxButton( _phys_ctrl_panel, ID_PHYS_INITBTN, wxT("Set Data"), wxDefaultPosition, wxDefaultSize, 0 );
	_d_sizer->Add( mv_phys_set_btn, 0, wxALIGN_CENTRE );
	_d_sizer->Add( 64,0,0 );
	mv_phys_save_btn = new wxButton( _phys_ctrl_panel, ID_PHYS_SAVEBTN, wxT("Save Data"), wxDefaultPosition, wxDefaultSize, 0 );
	_d_sizer->Add( mv_phys_save_btn, 0, wxALIGN_CENTRE );
	mv_phys_save_btn->Disable();

	mv_phys_ctrl_sizer->Add( _d_sizer );
	mv_phys_ctrl_sizer->Add( new wxStaticLine( _phys_ctrl_panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL ), 0, wxALL|wxEXPAND, 5 );
	
	vecontType<std::string> _slist;
	GenDistFuncList( _slist );
	wxString* _list = new wxString[_slist.size()];
	for( int i = 0; i < _slist.size(); i++ ) { _list[i] = _slist[i].c_str(); }
	mv_initial_choice = new wxChoice( _phys_ctrl_panel, ID_INIT_CHOICE, wxDefaultPosition, wxDefaultSize, _slist.size(), _list, 0 );
	mv_phys_ctrl_sizer->Add( mv_initial_choice, 0, wxALL, 5 );
	mv_initial_choice->Select(1);
	delete [] _list;

	mv_initial_panel_sizer = new wxBoxSizer(wxVERTICAL);
	mv_initial_panel = InitFuncPanel( _phys_ctrl_panel );
	mv_phys_ctrl_sizer->Add( mv_initial_panel_sizer, 0, wxALL|wxEXPAND, 5 );
	mv_initial_panel_sizer->Add( mv_initial_panel, 0, wxALL|wxEXPAND, 5 );

	wxBoxSizer* _btns = new wxBoxSizer( wxHORIZONTAL );
	mv_initial_add_btn = new wxButton( _phys_ctrl_panel, ID_INIT_ADD_BTN, wxT("Add"), wxDefaultPosition, wxDefaultSize, 0 );
	mv_initial_del_btn = new wxButton( _phys_ctrl_panel, ID_INIT_DEL_BTN, wxT("Delete"), wxDefaultPosition, wxDefaultSize, 0 );
	mv_initial_chg_btn = new wxButton( _phys_ctrl_panel, ID_INIT_CHG_BTN, wxT("Change"), wxDefaultPosition, wxDefaultSize, 0 );
	_btns->Add( mv_initial_add_btn,  1, wxEXPAND | wxALL, 5 );
	_btns->Add( mv_initial_del_btn,  1, wxEXPAND | wxALL, 5 );
	_btns->Add( mv_initial_chg_btn,  1, wxEXPAND | wxALL, 5 );
	mv_initial_del_btn->Disable();
	mv_initial_chg_btn->Disable();

	mv_phys_ctrl_sizer->Add( _btns, 0, wxALL, 5 );
	mv_initial_listbox = new wxListBox( _phys_ctrl_panel, ID_INIT_ADD_LIST, wxDefaultPosition, wxSize(-1,-1), 0, 0, wxLB_SINGLE );
	mv_initial_listbox->SetMinSize( wxSize(196,256) );
	mv_phys_ctrl_sizer->Add( mv_initial_listbox, 1, wxALL|wxEXPAND, 5 );
	
	_phys_ctrl_panel->SetSizer( mv_phys_ctrl_sizer );
	_phys_ctrl_panel->Layout();
	mv_phys_ctrl_sizer->Fit( _phys_ctrl_panel );
	
	return _phys_ctrl_panel;
}

wxPanel*
BaseController::InitFuncPanel( wxWindow* parent ) {
	m_init_func_selected = DistPanelSp(GenDistPanel( 2 ));
	mv_initial_choice->Select(2);
	return m_init_func_selected->CreatePanel( parent );
}

void
BaseController::GenDefaultInitPanel() {
	mv_initial_panel_sizer->Detach( mv_initial_panel );
	mv_initial_panel->Destroy();
	mv_initial_panel = InitFuncPanel( mv_phys_ctrl_panel );
	mv_initial_panel_sizer->Add( mv_initial_panel, 0, wxALL|wxEXPAND, 5);
	mv_phys_ctrl_sizer->Layout();
}

void
BaseController::LoadConfig() {

	wxString appname = wxTheApp->GetAppName();
	const char* ss = appname.c_str();
    wxConfig *cfg = new wxConfig (wxTheApp->GetAppName());

	int interval=0;
	if( !cfg->Read(_T("Pref/Interval"), &interval) ) {
		interval = 31; // default value
		cfg->Write(_T("Pref/Interval"), interval );
	}

	m_world_ctlr.SetSampleInterval( interval );

	delete cfg;
	
	return;
}

BaseController::BaseController( wxWindow* parent, int id, wxString title, wxPoint pos, wxSize size, int style )
	: wxFrame( parent, id, title, pos, size, style ), mv_timer(this, ID_TIMER),
	m_sim_inited(false), m_phys_inited(false)
{
	LoadConfig();

#ifdef __MACH__
	wxSystemOptions::SetOption(wxMAC_TEXTCONTROL_USE_MLTE,1);
#endif

	//	this->SetMinSize( wxSize( 800,600 ) );
	
	// for menu bar of the main frame.
	this->SetMenuBar( NewMenuBar( 0 ) );
	
//	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* _sizer;
	_sizer = new wxBoxSizer( wxHORIZONTAL );
	
// control panel 
	mv_ctrl_panel = new wxPanel( this, ID_DEFAULT, wxDefaultPosition, wxSize( -1,-1 ), wxTAB_TRAVERSAL );
	wxBoxSizer* _sim_sizer;
	_sim_sizer = new wxBoxSizer( wxVERTICAL );

	// start/restart and reset button.
	wxBoxSizer* _start_sizer = new wxBoxSizer( wxHORIZONTAL );
	mv_start_btn = new wxToggleButton( mv_ctrl_panel, ID_P_TGLBTN, wxT("Start"), wxDefaultPosition, wxDefaultSize, 0 );
	_start_sizer->Add( mv_start_btn, 0, wxALIGN_LEFT|wxALL, 5 );
	_start_sizer->Add( 64, 0 );
	mv_reset_btn = new wxButton( mv_ctrl_panel, ID_RESET_BTN, wxT("Revert"), wxDefaultPosition, wxDefaultSize, 0 );
	_start_sizer->Add( mv_reset_btn, 0, wxALIGN_LEFT|wxALL, 5 );
	mv_reset_btn->Disable();

	_sim_sizer->Add( _start_sizer, 0, wxALIGN_LEFT|wxALL, 5 );
	
	mv_sim_time_label = new wxStaticText( mv_ctrl_panel, ID_DEFAULT, wxT("Time =     0.0000"), wxDefaultPosition, wxDefaultSize, 0 );
	_sim_sizer->Add( mv_sim_time_label, 0, wxALIGN_RIGHT|wxALL, 5 );
	
	mv_control_note = new wxNotebook( mv_ctrl_panel, ID_DEFAULT, wxDefaultPosition, wxSize(-1,-1), wxNB_TOP );
	mv_sim_ctrl_panel = SimControlPanel( mv_control_note, ID_DEFAULT );
	mv_control_note->AddPage( mv_sim_ctrl_panel, wxT("Sim Ctrl"), true );
	mv_phys_ctrl_panel = PhysControlPanel( mv_control_note, ID_DEFAULT );
	mv_control_note->AddPage( mv_phys_ctrl_panel, wxT("Physical"), false );
	
	_sim_sizer->Add( mv_control_note, 0, wxALL|wxEXPAND, 5 );
	
	mv_ctrl_panel->SetSizer( _sim_sizer );
	mv_ctrl_panel->Layout();
	_sim_sizer->Fit( mv_ctrl_panel );

	_sizer->Add( mv_ctrl_panel, 0, wxALL|wxEXPAND, 5 );
	
	
// view panel.	
	wxBoxSizer* _view_sizer;
	_view_sizer = new wxBoxSizer( wxVERTICAL );
	
	mv_view = new wxNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	mv_3dview = new View3D( mv_view, ID_DEFAULT, wxDefaultPosition, wxSize( 640,640 ), 0), _T("3D View");
	mv_3dview->SetMinSize( wxSize( 640,640 ) );
	mv_3dview->SetWorldController( m_world_ctlr );
	
	mv_view->AddPage( mv_3dview, wxT("3D View"), true );
	mv_2dview = new View2D( mv_view, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
//	mv_2dview = new View2D();
	mv_view->AddPage( mv_2dview, wxT("2D View"), false );
	
	_view_sizer->Add( mv_view, 1, wxEXPAND | wxALL, 5 );
	
	mv_log = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( -1,96 ), wxTE_MULTILINE | wxTE_READONLY );
	mv_log->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_ACTIVEBORDER ) );
	
	_view_sizer->Add( mv_log, 0, wxALL|wxEXPAND, 5 );
	
	_sizer->Add( _view_sizer, 1, wxEXPAND, 5 );
	
	this->SetSizer( _sizer );
	this->Layout();
	_sizer->Fit( this );

// Add PNG Image handler
	wxImage::AddHandler( new wxPNGHandler() );


	InitSimulation();
}
