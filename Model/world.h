/*

 Class Name --- World
 
 Desctiption - A container class for numerical simulation data, such as
		the mass, location and velocity of the particles.

 $Author: hiroshi $
 $Date: 2008/01/15 06:47:51 $
 $Revision: 1.5 $
 
 Copyrights 2006-2008 All Rights Reserved by HABE, aka ABE Hiroshi, JAPAN.

*/

#ifndef __H_WORLD_H__
#define __H_WORLD_H__

#include "datatypes.h"
#include "force.h"

#include <pthread.h>
#include <vector>

#include <wx/wx.h>

class WorldController;

class World {

friend class WorldController;

public:
	World()		{}
	~World();
	
	void		SetSize( unsigned int n );
	int			GetSize() const { return m_num_particles; }
	void		Set( int n, scalarType m, scalarType x, scalarType y, scalarType z, scalarType vx, scalarType vy, scalarType vz );
	void		SetLocation( int n, const Locus& loc );
	void		GetLocation( int n, Locus& loc ) const;
	void		SetVelocity( int n, scalarType vx, scalarType vy, scalarType vz );
	void		GetVelocity( int n, scalarType& vx, scalarType& vy, scalarType& vz ) const;
	void		SetMass( int nth, scalarType m );
	scalarType	GetMass( int nth ) const;

protected:
private:
	int						m_num_particles;
	scalarType				m_temperature;
	vectorType				m_mass;

	vectorType				m_vx,m_vy,m_vz;
	vectorType				m_vx2,m_vy2,m_vz2;
	vectorType				m_x,m_y,m_z;
	
};

#endif