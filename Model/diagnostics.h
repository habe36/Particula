/*

 Dignostics Functinos for the simulation.

 Helps to make the dignosis time series.
 The time series are store in static member data so that can 
 be accessed/shared easily.

 $Author: habe $
 $Date: 2017/06/19 07:19:53 $
 $Revision: 1.11 $
 
 Copyrights 2006-2008 All Rights Reserved by HABE, aka ABE Hiroshi, JAPAN.

*/

#ifndef __H__DIAGNOSTICS_H___
#define __H__DIAGNOSTICS_H___

#include <vector>
#include <string>
#include <algorithm>

#include "Model/datatypes.h"

class Plotter {

	struct Series3D {
		std::string					title;
		std::vector<float>			t;			// for time
		std::vector<float>			x,y,z;		// coordinates
	};
	
	struct Series {
		std::string					title;
		std::vector<float>			x,y;
	};
	
public:
	Plotter();
	~Plotter();
	void		Add( int n, float time, const Locus& p );
	void		Add( int n, float time, float d );
	void		ResetSeries3D();
	void		ResetSeries();
	int			SizeOfSeries( int n ) const;
	int			SizeOfSeries3D( int n ) const;
	void		RegisterSeries( const std::string& title );
	void		RegisterSeries3D( const std::string& title );

    float		GetSeriesMinX( int n ) { return *std::min_element( m_series[n].x.begin(), m_series[n].x.end() ); }
    float		GetSeriesMaxX( int n ) { return *std::max_element( m_series[n].x.begin(), m_series[n].x.end() ); }
    float		GetSeriesMinY( int n ) { return *std::min_element( m_series[n].y.begin(), m_series[n].y.end() ); }
    float		GetSeriesMaxY( int n ) { return *std::max_element( m_series[n].y.begin(), m_series[n].y.end() ); }

    float		GetSeries3DMinX( int n ) { return *std::min_element( m_3d[n].x.begin(), m_3d[n].x.end() ); }
    float		GetSeries3DMaxX( int n ) { return *std::max_element( m_3d[n].x.begin(), m_3d[n].x.end() ); }
    float		GetSeries3DMinY( int n ) { return *std::min_element( m_3d[n].y.begin(), m_3d[n].y.end() ); }
    float		GetSeries3DMaxY( int n ) { return *std::max_element( m_3d[n].y.begin(), m_3d[n].y.end() ); }
    float		GetSeries3DMinZ( int n ) { return *std::min_element( m_3d[n].z.begin(), m_3d[n].z.end() ); }
    float		GetSeries3DMaxZ( int n ) { return *std::max_element( m_3d[n].z.begin(), m_3d[n].z.end() ); }

	static std::vector<Series>		m_series;
	static std::vector<Series3D>	m_3d;
private:
	static bool						m_series_lock;
	static bool						m_3d_lock;
};

template<class Vec>
double
KineticEnergy( int num, const Vec& vx, const Vec& vy, const Vec& vz, const Vec& m ) {
	double kinetic = 0.0;
//	for( int i = 0; i < num; i++ ) {
//		kinetic += m[i] * ( vx[i]*vx[i] + vy[i]*vy[i] + vz[i]*vz[i] );
//	}
	Vec _kin(num);
#ifdef _H_ENABLE_EIGEN_
    _kin = m * ( vx.dot(vx) + vy.dot(vy) + vz.dot(vz) );
#else
    _kin = m * ( vx*vx + vy*vy + vz*vz );
#endif
	for( int i = 0; i < num; i++ ) kinetic += _kin[i];
	kinetic *= 0.5;
//	std::cout << "KE = " << kinetic << "  ";
	return kinetic;
}

template <class Vec>
double
PotentialEnergy( int num, const Vec& x, const Vec& y, const Vec& z, const Vec& m ) {
	double pot = 0.0;
	for( int i = 0; i < num-1; i++ ) {
		for( int j = i+1; j < num; j++ ) {
			double dx = x[i] - x[j];
			double dy = y[i] - y[j];
			double dz = z[i] - z[j];
			double dist = ( dx*dx + dy*dy + dz*dz );
			pot += m[i]*m[j] / sqrt( dist + 0.0001 );
		}
	}
	pot = - pot;
//	std::cout << "PE = " << pot << std::endl;
	return pot;
}

template <class Vec>
void
FixMassCentre( Vec& m, Vec& vx, Vec& vy, Vec& vz ) {
	int n = m.size();
	double M = 0.0;
	double mvx = 0.0, mvy = 0.0, mvz = 0.0;
	for( int i = 0; i < n; i++ ) {
		M += m[i];
		mvx += m[i]*vx[i];
		mvy += m[i]*vy[i];
		mvz += m[i]*vz[i];
	}
//	std::cout << "MDrift = ( " <<  mvx/M << ", " << mvy/M << ", " << mvz/M << ")" << std::endl;
	for( int i = 0; i < n; i++ ) {
		vx[i] -= mvx/M;
		vy[i] -= mvy/M;
		vz[i] -= mvz/M;
	}
	return;
}

#endif