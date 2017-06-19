#ifndef _H__DISTRIBUTE_PANEL_H___
#define _H__DISTRIBUTE_PANEL_H___

#include <wx/wx.h>
#include <wx/panel.h>
#include <wx/spinctrl.h>

#include "Model/datatypes.h"
#include "H/HSmart_ptr.h"

enum {
	ID_LOAD_BTN = 299
};

class DistributePanel;

typedef H::smart_ptr<DistributePanel> DistPanelSp;

class DistributePanel {
public:
	DistributePanel() : m_n(100), m_m(1), m_v(0,0,0), m_x(0,0,0) {}
	virtual ~DistributePanel() {	/*cout << "VelocityPanel deleted" << endl;*/ }
	/*! create a wxWidgets panel to store the velocity distribution information
	@param parent specifies the parent wxWindow. 
	@return returns the wxPanel pointer to the GUI for the velocity distribution.
	*/
	virtual	wxPanel*	CreatePanel( wxWindow* parent ) = 0;
	//! to get the values on the window panel.
	virtual	void		GetValues() throw( const char*) = 0;
	/*! to generate the translational velocity values from the specified data. the data should be stored in the class by GetValues.
	*/
	virtual	void		Generate() = 0;
	/*!
		the member function gives the ID of the distribution.
	 @return returns wxString value which contains the ID of the distribution.
	 */
	virtual wxString	WhoAmI() const = 0;
	int					GetNum() const { return m_n; }
	inline	Locus		GetLoc( int n ) {
		Locus loc( m_buf_x[n], m_buf_y[n], m_buf_z[n] );
		return loc;
	}
	inline	Locus		GetVel( int n ) {
		Locus loc( m_buf_vx[n], m_buf_vy[n], m_buf_vz[n] );
		return loc;
	}
	inline	scalarType	GetMass() const {	return m_m;	}
	inline	scalarType	GetMass( int n ) const {	return m_buf_m[n];	}
	void			ClearBuffer() {
        m_buf_m = vectorType(0);
        m_buf_x = vectorType(0);
        m_buf_y = vectorType(0);
		m_buf_z = vectorType(0);
		m_buf_vx = vectorType(0);
		m_buf_vy = vectorType(0);
		m_buf_vz = vectorType(0);
	}
protected:
	int					m_n;
	scalarType			m_m;
	Locus				m_v;
	Locus				m_x;
	wxTextCtrl*			mw_num;
	wxTextCtrl*			mw_m;		//!< Total Mass of the system.
	wxTextCtrl*			mw_cx;
	wxTextCtrl*			mw_cy;
	wxTextCtrl*			mw_cz;
	wxTextCtrl*			mw_vx;
	wxTextCtrl*			mw_vy;
	wxTextCtrl*			mw_vz;

	vectorType			m_buf_m;
	vectorType			m_buf_x, m_buf_y, m_buf_z;
	vectorType			m_buf_vx, m_buf_vy, m_buf_vz;

	void			AddToSizer( wxWindow* panel, wxGridSizer* szr );
	void			GetBulkValues() throw( const char* );
	void			ResizeBuffer() {
		int n = m_n;
		m_buf_m.resize(n);
		m_buf_x.resize(n);
		m_buf_y.resize(n);
		m_buf_z.resize(n);
		m_buf_vx.resize(n);
		m_buf_vy.resize(n);
		m_buf_vz.resize(n);
	}
private:
};

class RandomPanel : public DistributePanel {
public:
	RandomPanel() : DistributePanel() {}
	~RandomPanel() {}
	wxPanel*	CreatePanel( wxWindow* parent );
	void		GetValues() throw( const char* );
	void		Generate();
	wxString	WhoAmI() const { return "Random Sphere"; }
private:
	wxTextCtrl*			mw_temp;		//!< 
	wxTextCtrl*			mw_a;		//!< 
	scalarType			m_temp;
	scalarType			m_a;
};

class LoadFilePanel : public DistributePanel, public wxEvtHandler {
public:
	LoadFilePanel() : DistributePanel(), wxEvtHandler() {
		m_filename = _T("");
	}
	~LoadFilePanel() {}
	wxPanel*	CreatePanel( wxWindow* parent );
	void		GetValues() throw( const char* );
	void		Generate();
	void		LoadNEMO( const char* filename );
	wxString	WhoAmI() const { return "Load File"; }
private:
	wxString	m_filename;
};

class PlummerPanel : public DistributePanel {
public:
	PlummerPanel() : DistributePanel(), m_a(1.0) {}
	~PlummerPanel() {}
	wxPanel*	CreatePanel( wxWindow* parent );
	void		GetValues() throw( const char* );
	void		Generate();
	wxString	WhoAmI() const { return "Plummer"; }
private:
	wxTextCtrl*			mw_a;		//!< 
	scalarType			m_a;
};

class HernquistPanel : public DistributePanel {
public:
	HernquistPanel() : DistributePanel(), m_a(1.0) {}
	~HernquistPanel() {}
	wxPanel*	CreatePanel( wxWindow* parent );
	void		GetValues() throw( const char* );
	void		Generate();
	wxString	WhoAmI() const { return "Hernquist"; }
private:
	wxTextCtrl*			mw_a;		//!< 
	scalarType			m_a;
};

class JaffePanel : public DistributePanel {
public:
	JaffePanel() : DistributePanel(), m_a(1.0) {}
	~JaffePanel() {}
	wxPanel*	CreatePanel( wxWindow* parent );
	void		GetValues() throw( const char* );
	void		Generate();
	wxString	WhoAmI() const { return "Jaffe"; }
private:
	wxTextCtrl*			mw_a;		//!< 
	scalarType			m_a;
};

class MiyamotoPanel : public DistributePanel {
public:
	MiyamotoPanel() : DistributePanel(), m_a(1.0), m_b(1.0) {}
	~MiyamotoPanel() {}
	wxPanel*	CreatePanel( wxWindow* parent );
	void		GetValues() throw( const char* );
	void		Generate();
	wxString	WhoAmI() const { return "Miyamoto"; }
private:
	wxTextCtrl*			mw_a;		//!< 
	wxTextCtrl*			mw_b;
	scalarType			m_a, m_b;
};


DistributePanel*	GenDistPanel( int n );
void				GenDistFuncList( vecontType<std::string>& list );
int					GetListNumber( wxString name );

#endif