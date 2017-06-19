#include <iostream>
#include <string>
#include <vector>

#include <wx/wx.h>
#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/spinctrl.h>
#include <wx/filename.h>

#include "View/DistributePanel.h"
#include "Model/datatypes.h"
#include "Model/distributor.h"

DistributePanel*
GenDistPanel( int n ) {

	assert( n < 6 );
	DistributePanel* _pnl;

	switch( n ) {
		case 0:
			_pnl = new RandomPanel();
			break;
		case 1:
			_pnl = new LoadFilePanel();
			break;
		case 2:
			_pnl = new PlummerPanel();
			break;
		case 3:
			_pnl = new HernquistPanel();
			break;
		case 4:
			_pnl = new JaffePanel();
			break;
		case 5:
			_pnl = new MiyamotoPanel();
			break;
		default:
			break;
	}

	return _pnl;
}

void
GenDistFuncList( vecontType<std::string>& list ) {
	list.push_back( "Random Sphere" );
	list.push_back( "Load File" );
	list.push_back( "Plummer" );
	list.push_back( "Hernquist" );
	list.push_back( "Jaffe" );
	list.push_back( "Miyamoto" );
}

int
GetListNumber( wxString name ) {
//	std::cout << "GetListNumber : " << name << std::endl;
	vecontType<std::string> list;
	GenDistFuncList( list );
	int id = -999;
    std::string nm = std::string(name.mb_str());
	for( int i = 0; i < list.size(); i++ ) {
		if( list[i] == nm ) id = i;
	}
	assert( id != -999 );
	return id;
}

void
DistributePanel::AddToSizer( wxWindow* _panel, wxGridSizer* _sizer ) {
	_sizer->Add( new wxStaticText( _panel, wxID_ANY, wxT("Centre x :"), wxDefaultPosition, wxDefaultSize, 0 ), 0, wxALL, 5 );
	mw_cx = new wxTextCtrl( _panel, wxID_ANY, wxString::Format("%f", m_x.x), wxDefaultPosition, wxDefaultSize, 0 );
	_sizer->Add( mw_cx, 0, wxALL, 5 );

	_sizer->Add( new wxStaticText( _panel, wxID_ANY, wxT("Centre y :"), wxDefaultPosition, wxDefaultSize, 0 ), 0, wxALL, 5 );
	mw_cy = new wxTextCtrl( _panel, wxID_ANY, wxString::Format("%f", m_x.y), wxDefaultPosition, wxDefaultSize, 0 );
	_sizer->Add( mw_cy, 0, wxALL, 5 );

	_sizer->Add( new wxStaticText( _panel, wxID_ANY, wxT("Centre z :"), wxDefaultPosition, wxDefaultSize, 0 ), 0, wxALL, 5 );
	mw_cz = new wxTextCtrl( _panel, wxID_ANY, wxString::Format("%f", m_x.z), wxDefaultPosition, wxDefaultSize, 0 );
	_sizer->Add( mw_cz, 0, wxALL, 5 );
	
	_sizer->Add( new wxStaticText( _panel, wxID_ANY, wxT("Bulk Vel. x :"), wxDefaultPosition, wxDefaultSize, 0 ), 0, wxALL, 5 );
	mw_vx = new wxTextCtrl( _panel, wxID_ANY, wxString::Format("%f", m_v.x), wxDefaultPosition, wxDefaultSize, 0 );
	_sizer->Add( mw_vx, 0, wxALL, 5 );

	_sizer->Add( new wxStaticText( _panel, wxID_ANY, wxT("Bulk Vel. y :"), wxDefaultPosition, wxDefaultSize, 0 ), 0, wxALL, 5 );
	mw_vy = new wxTextCtrl( _panel, wxID_ANY, wxString::Format("%f", m_v.y), wxDefaultPosition, wxDefaultSize, 0 );
	_sizer->Add( mw_vy, 0, wxALL, 5 );

	_sizer->Add( new wxStaticText( _panel, wxID_ANY, wxT("Bulk Vel. z :"), wxDefaultPosition, wxDefaultSize, 0 ), 0, wxALL, 5 );
	mw_vz = new wxTextCtrl( _panel, wxID_ANY, wxString::Format("%f", m_v.z), wxDefaultPosition, wxDefaultSize, 0 );
	_sizer->Add( mw_vz, 0, wxALL, 5 );
}

void
DistributePanel::GetBulkValues() throw( const char* ) {
	double		_tmp;
	long int	_n;
	bool		_flag = true;

	_flag = _flag && mw_cx->GetValue().ToDouble(&_tmp);
	m_x.x = _tmp;
	_flag = _flag && mw_cy->GetValue().ToDouble(&_tmp);
	m_x.y = _tmp;
	_flag = _flag && mw_cz->GetValue().ToDouble(&_tmp);
	m_x.z = _tmp;
	_flag = _flag && mw_vx->GetValue().ToDouble(&_tmp);
	m_v.x = _tmp;
	_flag = _flag && mw_vy->GetValue().ToDouble(&_tmp);
	m_v.y = _tmp;
	_flag = _flag && mw_vz->GetValue().ToDouble(&_tmp);
	m_v.z = _tmp;
	
//	std::cout << mw_num->GetValue() << ", " << _n << ": " << m_m << std::endl;
	
	if ( !_flag )	throw "Data for Bulk is not valid";
}

wxPanel*
RandomPanel::CreatePanel( wxWindow* parent ) {
	wxPanel* _panel = new wxPanel( parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxGridSizer* _sizer = new wxGridSizer( 2, 2, 0, 0 );

	_sizer->Add( new wxStaticText( _panel, wxID_ANY, wxT("Number :"), wxDefaultPosition, wxDefaultSize, 0 ), 0, wxALL, 5 );
	mw_num = new wxTextCtrl( _panel, wxID_ANY, wxString::Format("%d", m_n), wxDefaultPosition, wxDefaultSize, 0 );
	_sizer->Add( mw_num, 0, wxALL, 5 );

	_sizer->Add( new wxStaticText( _panel, wxID_ANY, wxT("Total Mass :"), wxDefaultPosition, wxDefaultSize, 0 ), 0, wxALL, 5 );
	mw_m = new wxTextCtrl( _panel, wxID_ANY, wxString::Format("%f", m_m), wxDefaultPosition, wxDefaultSize, 0 );
	_sizer->Add( mw_m, 0, wxALL, 5 );

	_sizer->Add( new wxStaticText( _panel, wxID_ANY, wxT("Temperature :"), wxDefaultPosition, wxDefaultSize, 0 ), 0, wxALL, 5 );
	mw_temp = new wxTextCtrl( _panel, wxID_ANY, wxString::Format("%f", m_temp), wxDefaultPosition, wxDefaultSize, 0 );
	_sizer->Add( mw_temp, 0, wxALL, 5 );

	AddToSizer( _panel, _sizer );

	_panel->SetSizer( _sizer );
	_panel->Layout();

	_panel->Update();

	return _panel;
}

void
RandomPanel::GetValues() throw( const char* ) {
	double		_tmp;
	bool		_flag = true;
	long int	_n;

	_flag = _flag && mw_m->GetValue().ToDouble(&_tmp);
	m_m = _tmp;

	_flag = _flag && mw_temp->GetValue().ToDouble(&_tmp);
	m_temp = _tmp;

	_flag = _flag && mw_num->GetValue().ToLong(&_n);
	m_n = (int)_n;
	
	if ( !_flag )	throw "RandomPanel data is not valid";
	
	try {
		GetBulkValues();
	}
	catch( const char* err ) {
		throw err;
	}
	
}

void
RandomPanel::Generate() {

	ResizeBuffer();

	scalarType sigma = sqrt(m_temp / (m_m));
	for( int i = 0; i < m_n; i++ ) {
		m_buf_vx[i] = ::gaussian()*sigma + m_v.x;
		m_buf_vy[i] = ::gaussian()*sigma + m_v.y;
		m_buf_vz[i] = ::gaussian()*sigma + m_v.z;
		m_buf_m[i] = m_m/m_n;
		scalarType xx,yy,zz;
		homo_sphere( xx,yy,zz );
		m_buf_x[i] = xx + m_x.x;
		m_buf_y[i] = yy + m_x.y;
		m_buf_z[i] = zz + m_x.z;
	}
}

wxPanel*
LoadFilePanel::CreatePanel( wxWindow* parent ) {
	wxPanel* _panel = new wxPanel( parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxGridSizer* _sizer = new wxGridSizer( 3, 2, 0, 0 );

	if( m_filename.empty() ) {
		m_filename = wxFileSelector(
									_T("Select the file to load"),
									wxEmptyString, wxEmptyString,
									_T(""),
									wxString::Format
									(
									 _T("NEMO Ascii (*.ascii)|*.ascii|NEMO text (*.txt)|*.txt|All files (%s)|%s"),
									 wxFileSelectorDefaultWildcardStr,
									 wxFileSelectorDefaultWildcardStr
									 ),
									wxFD_CHANGE_DIR
									);
	}
	wxString substr;
	if( m_filename.empty() ) {
		substr = _T("XXXX");
	} else {
		wxFileName fname = m_filename;
		substr = fname.GetName();
		LoadNEMO( m_filename );
		
	}

	_sizer->Add(new wxStaticText( _panel, wxID_ANY, _T("File to load :"), wxDefaultPosition, wxDefaultSize, 0 ));
	_sizer->Add(new wxStaticText( _panel, wxID_ANY, substr, wxDefaultPosition, wxDefaultSize, 0 ));
	_sizer->Add( new wxStaticText( _panel, wxID_ANY, wxT("Number :"), wxDefaultPosition, wxDefaultSize, 0 ), 0, wxALL, 5 );
	mw_num = new wxTextCtrl( _panel, wxID_ANY, wxString::Format("%d", m_n), wxDefaultPosition, wxDefaultSize, 0 );
	_sizer->Add( mw_num, 0, wxALL, 5 );

	_sizer->Add( new wxStaticText( _panel, wxID_ANY, wxT("Total Mass :"), wxDefaultPosition, wxDefaultSize, 0 ), 0, wxALL, 5 );
	mw_m = new wxTextCtrl( _panel, wxID_ANY, wxString::Format("%f", m_m), wxDefaultPosition, wxDefaultSize, 0 );
	_sizer->Add( mw_m, 0, wxALL, 5 );

	mw_num->Disable();
	mw_m->Disable();

	AddToSizer( _panel, _sizer );

	_panel->SetSizer( _sizer );
	_panel->Layout();

	_panel->Update();

	return _panel;
}

void
LoadFilePanel::LoadNEMO( const char* path ) {
	FILE *fp;
	int n,dim;
	double t, dum,x,y,z;

	fp = fopen(path,"r");
	fscanf(fp,"%d%d%lf", &n, &dim, &t );
	m_n = n;
	ClearBuffer();
	ResizeBuffer();

//	std::cout << "LoadNEMO " << std::endl;
	m_m = 0.0;
	for( int i = 0; i < n; i++ ) {
		fscanf(fp,"%lf",&dum);
		m_buf_m[i] = dum;
		m_m += dum;
	}
	for( int i = 0; i < n; i++ ) {
		fscanf(fp,"%lf%lf%lf",&x,&y,&z);
		m_buf_x[i] = x;
		m_buf_y[i] = y;
		m_buf_z[i] = z;
//		std::cout << "(x,y,z) = (" << x << ", " << y << ", " << z << ")" << std::endl;
	}
	for( int i = 0; i < n; i++ ) {
		fscanf(fp,"%lf%lf%lf",&x,&y,&z);
		m_buf_vx[i] = x;
		m_buf_vy[i] = y;
		m_buf_vz[i] = z;
//		std::cout << "(Vx,Vy,Vz) = (" << x << ", " << y << ", " << z << ") at " << i << std::endl;
	}
	fclose(fp);
}

void
LoadFilePanel::GetValues() throw( const char* ) {
	if( m_filename.empty() ) throw "File is not selected";
	
//	LoadNEMO( m_filename );
	
	try {
		GetBulkValues();
	}
	catch( const char* err ) {
		throw err;
	}
}

void
LoadFilePanel::Generate() {
	LoadNEMO( m_filename );
	
	for( int i = 0; i < m_n; i++ ) {
		m_buf_x[i] += m_x.x;
		m_buf_y[i] += m_x.y;
		m_buf_z[i] += m_x.z;

		m_buf_vx[i] += m_v.x;
		m_buf_vy[i] += m_v.y;
		m_buf_vz[i] += m_v.z;
	}
}

wxPanel*
PlummerPanel::CreatePanel( wxWindow* parent ) {

	wxPanel* _panel = new wxPanel( parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );

	wxGridSizer* _sizer = new wxGridSizer( 9, 2, 0, 0 );

	_sizer->Add( new wxStaticText( _panel, wxID_ANY, wxT("Number :"), wxDefaultPosition, wxDefaultSize, 0 ), 0, wxALL, 5 );
	mw_num = new wxTextCtrl( _panel, wxID_ANY, wxString::Format("%d", m_n), wxDefaultPosition, wxDefaultSize, 0 );
	_sizer->Add( mw_num, 0, wxALL, 5 );

	_sizer->Add( new wxStaticText( _panel, wxID_ANY, wxT("Total Mass :"), wxDefaultPosition, wxDefaultSize, 0 ), 0, wxALL, 5 );
	mw_m = new wxTextCtrl( _panel, wxID_ANY, wxString::Format("%f", m_m), wxDefaultPosition, wxDefaultSize, 0 );
	_sizer->Add( mw_m, 0, wxALL, 5 );

	_sizer->Add( new wxStaticText( _panel, wxID_ANY, wxT("Param. A :"), wxDefaultPosition, wxDefaultSize, 0 ), 0, wxALL, 5 );
	mw_a = new wxTextCtrl( _panel, wxID_ANY, wxString::Format("%f", m_a), wxDefaultPosition, wxDefaultSize, 0 );
	_sizer->Add( mw_a, 0, wxALL, 5 );

	AddToSizer( _panel, _sizer );

	_panel->SetSizer( _sizer );
	_panel->Layout();

	_panel->Update();

	return _panel;
}

void
PlummerPanel::GetValues() throw(const char*) {
	double		_dum;
	long int	_n;
	bool		_flag = true;

	_flag = _flag && mw_num->GetValue().ToLong(&_n);
	m_n = (int)_n;

	_flag = _flag && mw_m->GetValue().ToDouble(&_dum);
	m_m = _dum;

	_flag = _flag && mw_a->GetValue().ToDouble(&_dum);
	m_a = _dum;

	try {
		GetBulkValues();
	}
	catch( const char* err ) {
		throw err;
	}

	if ( !_flag )	throw "Plummer data is not valid";

	return;
}

void
PlummerPanel::Generate() {

	ResizeBuffer();

	dist_plummer( m_n, m_buf_x, m_buf_y, m_buf_z, m_buf_vx, m_buf_vy, m_buf_vz );
#ifdef _H_ENABLE_EIGEN_
    m_buf_m = Eigen::VectorXf::Constant(m_n, m_m/m_n);
#else
    m_buf_m = H::V::Hvector(m_n, m_m/m_n);
#endif
	m_buf_x *= m_a;
	m_buf_y *= m_a;
	m_buf_z *= m_a;
#ifdef _H_ENABLE_EIGEN_
    m_buf_x += Eigen::VectorXf::Constant(m_n, m_x.x);
    m_buf_y += Eigen::VectorXf::Constant(m_n, m_x.y);
    m_buf_z += Eigen::VectorXf::Constant(m_n, m_x.z);
#else
	m_buf_x += m_x.x;
	m_buf_y += m_x.y;
	m_buf_z += m_x.z;
#endif
	scalarType ia = sqrt(m_m/m_a);
	m_buf_vx *= ia;
	m_buf_vy *= ia;
	m_buf_vz *= ia;
#ifdef _H_ENABLE_EIGEN_
    m_buf_vx += Eigen::VectorXf::Constant(m_n, m_v.x);
    m_buf_vy += Eigen::VectorXf::Constant(m_n, m_v.y);
    m_buf_vz += Eigen::VectorXf::Constant(m_n, m_v.z);
#else
	m_buf_vx += m_v.x;
	m_buf_vy += m_v.y;
	m_buf_vz += m_v.z;
#endif
	
	return;
}


wxPanel*
HernquistPanel::CreatePanel( wxWindow* parent ) {

	wxPanel* _panel = new wxPanel( parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );

	wxGridSizer* _sizer = new wxGridSizer( 10, 2, 0, 0 );

	_sizer->Add( new wxStaticText( _panel, wxID_ANY, wxT("Number :"), wxDefaultPosition, wxDefaultSize, 0 ), 0, wxALL, 5 );
	mw_num = new wxTextCtrl( _panel, wxID_ANY, wxString::Format("%d", m_n), wxDefaultPosition, wxDefaultSize, 0 );
	_sizer->Add( mw_num, 0, wxALL, 5 );

	_sizer->Add( new wxStaticText( _panel, wxID_ANY, wxT("Total Mass :"), wxDefaultPosition, wxDefaultSize, 0 ), 0, wxALL, 5 );
	mw_m = new wxTextCtrl( _panel, wxID_ANY, wxString::Format("%f", m_m), wxDefaultPosition, wxDefaultSize, 0 );
	_sizer->Add( mw_m, 0, wxALL, 5 );

	_sizer->Add( new wxStaticText( _panel, wxID_ANY, wxT("Param. A :"), wxDefaultPosition, wxDefaultSize, 0 ), 0, wxALL, 5 );
	mw_a = new wxTextCtrl( _panel, wxID_ANY, wxString::Format("%f", m_a), wxDefaultPosition, wxDefaultSize, 0 );
	_sizer->Add( mw_a, 0, wxALL, 5 );

	AddToSizer( _panel, _sizer );

	_panel->SetSizer( _sizer );
	_panel->Layout();

	_panel->Update();

	return _panel;
}

void
HernquistPanel::GetValues() throw(const char*) {
	double		_dum;
	long int	_n;
	bool		_flag = true;

	_flag = _flag && mw_num->GetValue().ToLong(&_n);
	m_n = (int)_n;

	_flag = _flag && mw_m->GetValue().ToDouble(&_dum);
	m_m = _dum;

	_flag = _flag && mw_a->GetValue().ToDouble(&_dum);
	m_a = _dum;

	try {
		GetBulkValues();
	}
	catch( const char* err ) {
		throw err;
	}

	if ( !_flag )	throw "Hernquist data is not valid";
	return;
}

void
HernquistPanel::Generate() {

	ResizeBuffer();

	dist_hernquist( m_n, m_buf_x, m_buf_y, m_buf_z, m_buf_vx, m_buf_vy, m_buf_vz );
#ifdef _H_ENABLE_EIGEN_
    m_buf_m = Eigen::VectorXf::Constant(m_n, m_m/m_n);
#else
	m_buf_m = m_m/m_n;
#endif
	m_buf_x *= m_a;
	m_buf_y *= m_a;
	m_buf_z *= m_a;
#ifdef _H_ENABLE_EIGEN_
    m_buf_x += Eigen::VectorXf::Constant(m_n, m_x.x);
    m_buf_y += Eigen::VectorXf::Constant(m_n, m_x.y);
    m_buf_z += Eigen::VectorXf::Constant(m_n, m_x.z);
#else
	m_buf_x += m_x.x;
	m_buf_y += m_x.y;
	m_buf_z += m_x.z;
#endif
	scalarType ia = sqrt(m_m/m_a);
	m_buf_vx *= ia;
	m_buf_vy *= ia;
	m_buf_vz *= ia;
#ifdef _H_ENABLE_EIGEN_
    m_buf_vx += Eigen::VectorXf::Constant(m_n, m_v.x);
    m_buf_vy += Eigen::VectorXf::Constant(m_n, m_v.y);
    m_buf_vz += Eigen::VectorXf::Constant(m_n, m_v.z);
#else
	m_buf_vx += m_v.x;
	m_buf_vy += m_v.y;
	m_buf_vz += m_v.z;
#endif
	return;
}



wxPanel*
JaffePanel::CreatePanel( wxWindow* parent ) {

	wxPanel*		_panel = new wxPanel( parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );

	wxGridSizer*	_sizer = new wxGridSizer( 2, 2, 0, 0 );

	_sizer->Add( new wxStaticText( _panel, wxID_ANY, wxT("Number :"), wxDefaultPosition, wxDefaultSize, 0 ), 0, wxALL, 5 );
	mw_num = new wxTextCtrl( _panel, wxID_ANY, wxString::Format("%d", m_n), wxDefaultPosition, wxDefaultSize, 0 );
	_sizer->Add( mw_num, 0, wxALL, 5 );

	_sizer->Add( new wxStaticText( _panel, wxID_ANY, wxT("Total Mass :"), wxDefaultPosition, wxDefaultSize, 0 ), 0, wxALL, 5 );
	mw_m = new wxTextCtrl( _panel, wxID_ANY, wxString::Format("%f", m_m), wxDefaultPosition, wxDefaultSize, 0 );
	_sizer->Add( mw_m, 0, wxALL, 5 );

	_sizer->Add( new wxStaticText( _panel, wxID_ANY, wxT("Param. A :"), wxDefaultPosition, wxDefaultSize, 0 ), 0, wxALL, 5 );
	mw_a = new wxTextCtrl( _panel, wxID_ANY, wxString::Format("%f", m_a), wxDefaultPosition, wxDefaultSize, 0 );
	_sizer->Add( mw_a, 0, wxALL, 5 );

	AddToSizer( _panel, _sizer );

	_panel->SetSizer( _sizer );
	_panel->Layout();

	_panel->Update();

	return _panel;
}

void
JaffePanel::GetValues() throw(const char*) {
	double		_dum;
	long int	_n;
	bool		_flag = true;

	_flag = _flag && mw_num->GetValue().ToLong(&_n);
	m_n = (int)_n;

	_flag = _flag && mw_m->GetValue().ToDouble(&_dum);
	m_m = _dum;

	_flag = _flag && mw_a->GetValue().ToDouble(&_dum);
	m_a = _dum;

	try {
		GetBulkValues();
	}
	catch( const char* err ) {
		throw err;
	}

	if ( !_flag )	throw "Jaffe data is not valid";
	
	return;
}

void
JaffePanel::Generate() {

	ResizeBuffer();

	dist_jaffe( m_n, m_buf_x, m_buf_y, m_buf_z, m_buf_vx, m_buf_vy, m_buf_vz );
#ifdef _H_ENABLE_EIGEN_
    m_buf_m = Eigen::VectorXf::Constant(m_n, m_m/m_n);
#else
    m_buf_m = m_m/m_n;
#endif
	m_buf_x *= m_a;
	m_buf_y *= m_a;
	m_buf_z *= m_a;
#ifdef _H_ENABLE_EIGEN_
    m_buf_x += Eigen::VectorXf::Constant(m_n, m_x.x);
    m_buf_y += Eigen::VectorXf::Constant(m_n, m_x.y);
    m_buf_z += Eigen::VectorXf::Constant(m_n, m_x.z);
#else
	m_buf_x += m_x.x;
	m_buf_y += m_x.y;
	m_buf_z += m_x.z;
#endif
	scalarType ia = sqrt(m_m/m_a);
	m_buf_vx *= ia;
	m_buf_vy *= ia;
	m_buf_vz *= ia;
#ifdef _H_ENABLE_EIGEN_
    m_buf_vx += Eigen::VectorXf::Constant(m_n, m_v.x);
    m_buf_vy += Eigen::VectorXf::Constant(m_n, m_v.y);
    m_buf_vz += Eigen::VectorXf::Constant(m_n, m_v.z);
#else
	m_buf_vx += m_v.x;
	m_buf_vy += m_v.y;
	m_buf_vz += m_v.z;
#endif
    
	return;
}

wxPanel*
MiyamotoPanel::CreatePanel( wxWindow* parent ) {

	wxPanel* _panel = new wxPanel( parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );

	wxGridSizer* _sizer = new wxGridSizer( 2, 2, 0, 0 );

	_sizer->Add( new wxStaticText( _panel, wxID_ANY, wxT("Number :"), wxDefaultPosition, wxDefaultSize, 0 ), 0, wxALL, 5 );
	mw_num = new wxTextCtrl( _panel, wxID_ANY, wxString::Format("%d", m_n), wxDefaultPosition, wxDefaultSize, 0 );
	_sizer->Add( mw_num, 0, wxALL, 5 );

	_sizer->Add( new wxStaticText( _panel, wxID_ANY, wxT("Total Mass :"), wxDefaultPosition, wxDefaultSize, 0 ), 0, wxALL, 5 );
	mw_m = new wxTextCtrl( _panel, wxID_ANY, wxString::Format("%f", m_m), wxDefaultPosition, wxDefaultSize, 0 );
	_sizer->Add( mw_m, 0, wxALL, 5 );

	_sizer->Add( new wxStaticText( _panel, wxID_ANY, wxT("Param. A :"), wxDefaultPosition, wxDefaultSize, 0 ), 0, wxALL, 5 );
	mw_a = new wxTextCtrl( _panel, wxID_ANY, wxString::Format("%f", m_a), wxDefaultPosition, wxDefaultSize, 0 );
	_sizer->Add( mw_a, 0, wxALL, 5 );

	_sizer->Add( new wxStaticText( _panel, wxID_ANY, wxT("Param. B :"), wxDefaultPosition, wxDefaultSize, 0 ), 0, wxALL, 5 );
	mw_b = new wxTextCtrl( _panel, wxID_ANY, wxString::Format("%f", m_b), wxDefaultPosition, wxDefaultSize, 0 );
	_sizer->Add( mw_b, 0, wxALL, 5 );

	AddToSizer( _panel, _sizer );

	_panel->SetSizer( _sizer );
	_panel->Layout();

	_panel->Update();

	return _panel;
}

void
MiyamotoPanel::GetValues() throw(const char*) {
	double		_dum;
	long int	_n;
	bool		_flag = true;

	_flag = _flag && mw_num->GetValue().ToLong(&_n);
	m_n = (int)_n;

	_flag = _flag && mw_m->GetValue().ToDouble(&_dum);
	m_m = _dum;

	_flag = _flag && mw_a->GetValue().ToDouble(&_dum);
	m_a = _dum;

	_flag = _flag && mw_b->GetValue().ToDouble(&_dum);
	m_b = _dum;

	try {
		GetBulkValues();
	}
	catch( const char* err ) {
		throw err;
	}

	if ( !_flag )	throw "Miyamoto_Nagai data is not valid";

	return;
}

void
MiyamotoPanel::Generate() {

	ResizeBuffer();

	scalarType beta = m_b / m_a;
	dist_miyamoto( m_n, beta, m_buf_x, m_buf_y, m_buf_z, m_buf_vx, m_buf_vy, m_buf_vz );
#ifdef _H_ENABLE_EIGEN_
	m_buf_m = Eigen::VectorXf::Constant(m_n, m_m/m_n);
#else
	m_buf_m = m_m/m_n;
#endif
	m_buf_x *= m_a;
	m_buf_y *= m_a;
	m_buf_z *= m_a;
#ifdef _H_ENABLE_EIGEN_
	m_buf_x += Eigen::VectorXf::Constant(m_n, m_x.x);
	m_buf_y += Eigen::VectorXf::Constant(m_n, m_x.y);
	m_buf_z += Eigen::VectorXf::Constant(m_n, m_x.z);
#else
    m_buf_x += m_x.x;
    m_buf_y += m_x.y;
    m_buf_z += m_x.z;
#endif
	scalarType ia = sqrt(m_m/m_a);
	m_buf_vx *= ia;
	m_buf_vy *= ia;
	m_buf_vz *= ia;
#ifdef _H_ENABLE_EIGEN_
	m_buf_vx += Eigen::VectorXf::Constant(m_n, m_v.x);
	m_buf_vy += Eigen::VectorXf::Constant(m_n, m_v.y);
	m_buf_vz += Eigen::VectorXf::Constant(m_n, m_v.z);
#else
    m_buf_vx += m_v.x;
    m_buf_vy += m_v.y;
    m_buf_vz += m_v.z;
#endif
	
	return;
}



