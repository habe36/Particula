#ifndef __H_BASECONTROLLER_H__
#define __H_BASECONTROLLER_H__
/*

 Class Name --- BaseController
 
 Description - A class to controll and interfacing wxWidgets stuff and 
			the main simulation classes.

 $Author: hiroshi $
 $Date: 2008/07/05 00:32:40 $
 $Revision: 1.15 $
 
 Copyrights 2006-2008 All Rights Reserved by HABE, aka ABE Hiroshi, JAPAN.

*/
#include <pthread.h>
#include <wx/wx.h>
#include <wx/menu.h>
#include <wx/notebook.h> 
#include <wx/statline.h>
#include <wx/glcanvas.h>
#include <wx/spinctrl.h>
#include <wx/tglbtn.h>

#include <iostream>
#include <vector>

#include "H/HSmart_ptr.h"

#include "Model/ppforce.h"
#include "Model/barrier.h"
#include "Model/diagnostics.h"
#include "Controller/world_controller.h"
#include "View/View3D.h"
#include "View/View2D.h"
#include "View/DistributePanel.h"

class MyWorkerThread;

#define ID_DEFAULT wxID_ANY // Default
enum {
	ID_TIMER	= 299,
	ID_CTRL_PAGE,
	ID_VIEW_PAGE,
	ID_P_TGLBTN,
	ID_RESET_BTN,
	ID_PART_NUM,
	ID_TEMPERATURE,
	ID_TH_NUM,
	ID_SIM_INITBTN,
	ID_SIM_TIME,
	ID_PHYS_INITBTN,
	ID_PHYS_SAVEBTN,
	ID_RECBTN,
	ID_INIT_ADD_BTN,
	ID_INIT_DEL_BTN,
	ID_INIT_CHG_BTN,
	ID_INIT_ADD_LIST,
	ID_LOADBTN,
	ID_SAVEBTN,
	ID_TIMELABEL,
	ID_CHOICE,
	ID_INIT_CHOICE,
	ID_ABOUT,
	ID_EXIT
};

const float c_temp_ratio = 0.01;

class BaseController: public wxFrame
{
public:
	BaseController( wxWindow* parent, int id = -1, wxString title = mv_version,
		wxPoint pos = wxDefaultPosition, wxSize size = wxSize( -1,-1 ),
		int style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL);

	wxMenuBar*		NewMenuBar( int id );
	wxBoxSizer*		ControlSizer();
	wxPanel*		SimControlPanel( wxWindow* parent, int id );
	wxPanel*		PhysControlPanel( wxWindow* parent, int id );

	wxPanel*		InitFuncPanel( wxWindow* parent );
	void			GenDefaultInitPanel();
	wxString		GenListBoxItem() {
		wxString _item;
		_item = _item + m_init_func_selected->WhoAmI() + wxT(" / ");
		_item = _item + wxString::Format( "%6d", m_init_func_selected->GetNum() );
		_item = _item + wxT(" / ");
		_item = _item + wxString::Format( "%2.2f", m_init_func_selected->GetMass() );
		return _item;
	}

	void			SaveFrame() const;
	void			LoadConfig();
	
	// Event Handlers.
	void			OnTglBtn(		wxCommandEvent& event );
	void			OnResetBtn(		wxCommandEvent& event );
	void			OnSimInitBtn(	wxCommandEvent& event );
	void			OnPhysInitBtn(	wxCommandEvent& event );
	void			OnPhysSaveBtn(	wxCommandEvent& event );
	void			OnRecordBtn(	wxCommandEvent& event );
//	void			OnLoadBtn(		wxCommandEvent& event );
	void			OnAddBtn(		wxCommandEvent& event );
	void			OnDelBtn(		wxCommandEvent& event );
	void			OnChgBtn(		wxCommandEvent& event );
	void			OnChoice(		wxCommandEvent& event );
	void			OnListBox(		wxCommandEvent& event );
	void			OnExit(			wxCommandEvent& event );
	void			OnTempSlider(	wxCommandEvent& event );
	void			OnTimer(		wxTimerEvent&	event );
    void			OnAbout(		wxCommandEvent& event );
	void			OnPreferences(	wxCommandEvent& event );
	void			OnInitChoice(	wxCommandEvent& event );

	//! Stopping 
	void			Stop();
	void			Start();
	void			InitSimulation();
	void			InitSimParam();
	void			InitPhysParam();
	void			ClearPanelBuffer() {
		int n = m_init_func_list.size();
		for( int i = 0; i < n; i++ ) {
			m_init_func_list[i]->ClearBuffer();
		}
	}
private:
	static const	wxString	mv_version;

	// GUI-wise member data.
	wxTimer				mv_timer;

	wxStaticText*		mv_temperature_label;
	wxSlider*			mv_temperature;

	wxPanel*			mv_ctrl_panel;
	wxToggleButton*		mv_start_btn;
	wxButton*			mv_reset_btn;
	wxStaticText*		mv_sim_time_label;
	wxNotebook*			mv_control_note;
	wxPanel*			mv_sim_ctrl_panel;
	wxButton*			mv_sim_set_btn;
	wxTextCtrl*			mv_sim_time;
	wxChoice*			mv_sim_method;
	wxSpinCtrl*			mv_sim_threads;
	
	wxPanel*			mv_phys_ctrl_panel;
	wxBoxSizer*			mv_phys_ctrl_sizer;
	wxChoice*			mv_initial_choice;
	wxBoxSizer*			mv_initial_panel_sizer;
	wxPanel*			mv_initial_panel;
	wxListBox*			mv_initial_listbox;
	wxButton*			mv_initial_add_btn;
	wxButton*			mv_initial_del_btn;
	wxButton*			mv_initial_chg_btn;

	DistPanelSp			m_init_func_selected;
	vecontType<DistPanelSp>
						m_init_func_list;

	wxNotebook*			mv_phys_note;
	wxPanel*			mv_phys_int_panel;
	wxButton*			mv_phys_set_btn;
	wxButton*			mv_phys_save_btn;
	wxSpinCtrl*			mv_phys_int_num_particles;
	wxPanel*			mv_phys_ext_panel;
	wxButton*			mv_phys_ext_load_btn;
	wxButton*			mv_rec_btn;
	wxStaticText*		mv_phys_ext_label;
	wxButton*			mv_phys_ext_save_btn;
	wxNotebook*			mv_view;
	View3D*				mv_3dview;
	View2D*				mv_2dview;
	wxTextCtrl*			mv_log;
		
    WorldController		m_world_ctlr;

// for plotting
	Plotter				m_plotter;

// for controlling simulation
	bool				m_sim_inited, m_phys_inited, m_is_stopped;

    DECLARE_EVENT_TABLE()
};

#endif