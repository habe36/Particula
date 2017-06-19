/*

 Class Name --- View2D
 
 Description - A class to show the two dimensional diagnosis of the simulation.
			wxmathplot is adopted for display.

 $Author: hiroshi $
 $Date: 2009/04/16 01:59:44 $
 $Revision: 1.10 $
 
 Copyrights 2007 All Rights Reserved by HABE, aka ABE Hiroshi, JAPAN.

*/
#include <iostream>

#include <wx/wx.h>
#include <wx/intl.h>
#include <wx/spinctrl.h>

#include "wxmathplot/mathplot.h"
#include "View/View2d.h"

enum {
	ID_TRAJECTORY = 2222
};

BEGIN_EVENT_TABLE(View2D,wxPanel)
  EVT_MENU(mpID_FIT, View2D::OnFit)
  EVT_SPINCTRL(ID_TRAJECTORY, View2D::OnTrajectory)
END_EVENT_TABLE()

View2D::View2D( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style )
: wxPanel( parent, id, pos, size, style )
{
	m_left = new mpWindow( this, -1, wxPoint(0,0), wxSize(-1,-1), wxSUNKEN_BORDER );
	m_right = new mpWindow( this, -1, wxPoint(0,0), wxSize(-1,-1), wxSUNKEN_BORDER );

	m_left->AddLayer( new mpScaleX );
	m_left->AddLayer( new mpScaleY );	
	m_left->AddLayer( m_trajectory = new DataPlot(_T("Trajectory"), m_plotter.m_3d[0].x, m_plotter.m_3d[0].y) );

    m_right->AddLayer(     new mpScaleX(wxT("X")) );
    m_right->AddLayer(     new mpScaleY(wxT("Y"), mpALIGN_LEFT, true) );
    m_right->AddLayer( m_series = new DataPlot(_T("Energy"), m_plotter.m_series[0].x, m_plotter.m_series[0].y) );
	m_right->AddLayer( m_series2 = new DataPlot(_T(""), m_plotter.m_series[1].x, m_plotter.m_series[1].y) );
  
    // set a nice pen for the lissajoux
    wxPen tr_pen(*wxRED, 5, wxSOLID);
    m_trajectory->SetPen( tr_pen);

	wxPen sr_pen(*wxBLUE, 5, wxSOLID);
	m_series->SetPen( sr_pen );
	wxPen sr2_pen(*wxRED, 5, wxSOLID);
	m_series2->SetPen( sr2_pen );
	
	// 
    wxBoxSizer *topsizer = new wxBoxSizer( wxHORIZONTAL );
	wxBoxSizer* _leftSizer = new wxBoxSizer( wxVERTICAL );
	wxBoxSizer* _btnSizer = new wxBoxSizer( wxHORIZONTAL );
	
	wxStaticText*	_lbl = new wxStaticText( this, wxID_ANY, wxT("Particle to Track : ") );
	mw_track = new wxSpinCtrl( this, ID_TRAJECTORY, wxEmptyString, wxDefaultPosition, wxSize(96,36), wxSP_ARROW_KEYS, 0, 100, 0);
	mw_track->SetValue(0); // set the initial value.
	_btnSizer->Add( _lbl, 0, wxALIGN_RIGHT, 5 );
	_btnSizer->Add( mw_track, 0, wxALIGN_RIGHT, 5 );
	m_num_track = 0; // as initial value.
	
	_leftSizer->Add( _btnSizer, 0, wxEXPAND, 5 );
    _leftSizer->Add( m_left, 1, wxEXPAND );

	topsizer->Add( _leftSizer, 1, wxEXPAND );
	topsizer->Add( m_right, 1, wxEXPAND );

    SetAutoLayout( TRUE );
    SetSizer( topsizer );
	Layout();
}

void View2D::ResetTrajectory() {
	m_num_track = mw_track->GetValue();
//	std::cout << "Trajectory " << m_num_track << std::endl;
	m_trajectory->Reset();

	m_plotter.ResetSeries3D();
}

void View2D::ResetEnergyPlot() {
	m_series->Reset();
	m_series2->Reset();
	
	m_plotter.ResetSeries();
}

void View2D::OnTrajectory( wxSpinEvent &WXUNUSED(event) ) {
	ResetTrajectory();
}

void View2D::OnFit( wxCommandEvent &WXUNUSED(event) )
{
    m_left->Fit();
    m_right->Fit();	
}
