/*

 Class Name --- View2D
 
 Description - A class to show the two dimensional diagnosis of the simulation.
			wxmathplot is adopted for display.

 $Author: hiroshi $
 $Date: 2008/01/15 06:47:51 $
 $Revision: 1.11 $
 
 Copyrights 2007-2008 All Rights Reserved by HABE, aka ABE Hiroshi, JAPAN.

*/

#ifndef __H__VIEW2D_H__
#define __H__VIEW2D_H__


#include <wx/wx.h>
#include <wx/image.h>
#include <wx/listctrl.h>
#include <wx/sizer.h>
#include <wx/log.h>
#include <wx/intl.h>
#include <wx/spinctrl.h>
#include <mathplot.h>

#include <vector>
#include <algorithm>
#include <cmath>

#include "Model/diagnostics.h"

// derived classes

//class View2D;

class DataPlot : public mpFXY
{
private:
	const std::vector<float>&	m_x;
	const std::vector<float>&	m_y;
	int							m_idx;
public:
    DataPlot( const wxString& label, const std::vector<float>& x, const std::vector<float>& y ) : mpFXY(label), m_x(x), m_y(y) { 
		m_idx=0;
	}
	void Reset() {
		Rewind();
		// caution. the actual instances of m_x and m_y should be also reset to zero size.
		m_idx = 0;
	}
    virtual bool GetNextXY( double & x, double & y )
    {
		bool ret;
        if (m_idx < m_x.size() )
        {
//			std::cout << "GetNextXY  " << m_idx << " : " << x << ", " << y << std::endl;
			x = m_x[m_idx];
			y = m_y[m_idx];
            m_idx++;
            return true;
        }
        else
        {
			return false;
        }
    }
    virtual void Rewind() { m_idx=0; }

    virtual double GetMinX() { 
		if( m_x.empty() ) return 0.0;
		return *std::min_element( m_x.begin(), m_x.end() );
	}
    virtual double GetMaxX() { 
		if( m_x.empty() ) return 0.0;
		return *std::max_element( m_x.begin(), m_x.end() );
	}
    virtual double GetMinY() { 
		if( m_y.empty() ) return 0.0;
		return *std::min_element( m_y.begin(), m_y.end() );
	}
    virtual double GetMaxY() { 
		if( m_y.empty() ) return 0.0;
		return *std::max_element( m_y.begin(), m_y.end() );
	}
};

// View2D

class View2D: public wxPanel
{
public:
    View2D( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxTAB_TRAVERSAL );
	~View2D() {}

	void	SetNumParticles( int n ) { 
		m_num_particle = n;
		mw_track->SetRange(0,n-1);
	}
	int		GetNumTrack() const { return m_num_track; }

    void OnFit( wxCommandEvent &event );

	void OnTrajectory( wxSpinEvent &event );

	void ResetTrajectory();
	void ResetEnergyPlot();

	void UpdateAll() {
		m_left->UpdateAll();
		m_right->UpdateAll();
	}

    mpWindow*		m_left;
	mpWindow*		m_right;
	DataPlot*		m_trajectory;
	DataPlot*		m_series;
	DataPlot*		m_series2;
	long			m_num_particle;
	long			m_num_track;
	Plotter			m_plotter;

	wxSpinCtrl*		mw_track;
private:
//   DECLARE_DYNAMIC_CLASS(View2D)
    DECLARE_EVENT_TABLE()
};

// View2D

#endif
