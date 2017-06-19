/*

 Dignostics Functinos for the simulation.

 Helps to make the dignosis time series.
 The time series are store in static member data so that can 
 be accessed/shared easily.
 

 $Author: hiroshi $
 $Date: 2008/01/15 06:47:51 $
 $Revision: 1.6 $
 
 Copyrights 2006-2008 All Rights Reserved by HABE, aka ABE Hiroshi, JAPAN.

*/
#include "Model/diagnostics.h"
#include "Model/datatypes.h"

#include <vector>

std::vector<Plotter::Series>		Plotter::m_series;
std::vector<Plotter::Series3D> 		Plotter::m_3d;
bool								Plotter::m_series_lock;
bool								Plotter::m_3d_lock;


Plotter::Plotter() {
	m_3d_lock = false;
	m_series_lock = false;
	m_series.resize(2);
	m_3d.resize(1);
}

Plotter::~Plotter() {}

void
Plotter::Add( int n, float time, const Locus& p ) {
	m_3d[n].t.push_back( time );
	m_3d[n].x.push_back(p.x);
	m_3d[n].y.push_back(p.y);
	m_3d[n].z.push_back(p.z);
}

void
Plotter::Add( int n, float time, float d ) {
	m_series[n].x.push_back(time);
	m_series[n].y.push_back(d);
//	std::cout << "Plotter::Add : " << n << "  " << time << ", " << d << m_series[n].y[m_series[n].y.size()-1] << " / " << m_series[n].y.size() << std::endl;
}

void
Plotter::ResetSeries3D() {
	for( int n = 0; n < m_3d.size(); n++ ) {
		m_3d[n].t.resize(0);
		m_3d[n].x.resize(0);		
		m_3d[n].y.resize(0);				
		m_3d[n].z.resize(0);		
	}
}

void
Plotter::ResetSeries() {
	for( int n = 0; n < m_series.size(); n++ ) {
		m_series[n].x.resize(0);
		m_series[n].y.resize(0);				
	}
}

int
Plotter::SizeOfSeries( int n ) const {
//	int ss = m_series.size();
//	return m_series[n].data.size();
	return 0;
}

int
Plotter::SizeOfSeries3D( int n ) const {
//	int ss = m_3d.size();
//	return m_3d[n].data.size();
	return 0;
}
	
void
Plotter::RegisterSeries( const std::string& title ) {
	m_series.push_back(Series());
}

void
Plotter::RegisterSeries3D( const std::string& title ) {
	m_3d.push_back(Series3D());
}
