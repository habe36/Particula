/*

 Class Name --- World
 
 Desctiption - A container class for numerical simulation data, such as
		the mass, location and velocity of the particles.

 $Author: hiroshi $
 $Date: 2008/01/15 06:47:51 $
 $Revision: 1.5 $
 
 Copyrights 2006-2008 All Rights Reserved by HABE, aka ABE Hiroshi, JAPAN.

*/

#include <wx/wx.h>
#include "world.h"

#ifdef __WXMAC__
 #include <OpenGL/gl.h>
 #include <OpenGL/glu.h>
#else
 #include <gl/gl.h>
 #include <gl/glu.h>
#endif

#include <cstdio>
#include <iostream>
#include <string>
#include <vector>
//#include <valarray>

using namespace std;

#include "datatypes.h"

void
World::SetVelocity( int n, scalarType vx, scalarType vy, scalarType vz ) {
	m_vx[n] = vx;
	m_vy[n] = vy;
	m_vz[n] = vz;
}

void
World::SetSize( unsigned int n ) {

	m_num_particles = n;
	m_mass.resize(n);
	m_x.resize(n);
	m_y.resize(n);
	m_z.resize(n);		
	m_vx.resize(n);
	m_vy.resize(n);
	m_vz.resize(n);
}

void
World::Set( int n, scalarType m, scalarType x, scalarType y, scalarType z, scalarType vx, scalarType vy, scalarType vz ) {
	SetMass( n, m );
	SetLocation( n, Locus( x,y,z ) );
	SetVelocity( n, vx,vy,vz );
}

void
World::GetLocation( int n, Locus& loc ) const {
	loc.x = m_x[n];
	loc.y = m_y[n];
	loc.z = m_z[n];
	return;
}

void
World::SetLocation( int i, const Locus& lc ) {
	m_x[i] = lc.x;
	m_y[i] = lc.y;
	m_z[i] = lc.z;
}

World::~World() {}

void
World::GetVelocity( int n, scalarType& vx, scalarType& vy, scalarType& vz ) const {
	vx = m_vx[n];
	vy = m_vy[n];
	vz = m_vz[n];
}

void
World::SetMass( int nth, scalarType m ) {
	m_mass[nth] = m;
}

scalarType
World::GetMass( int nth ) const {
	return m_mass[nth];
}
