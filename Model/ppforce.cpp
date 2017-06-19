/*

 Class Name --- HPPDirectForce/HPPOctTreeForce/HPPADTreeForce
 
 Description - Class implementation for particle-particle force calculation
			with Direct method, Octet tree method and Alternate digital tree
			method.

 $Author: habe $
 $Date: 2017/06/19 07:19:53 $
 $Revision: 1.6 $
 
 Copyrights 2007 All Rights Reserved by HABE, aka ABE Hiroshi, JAPAN.

*/

#include <wx/wx.h>

#include "datatypes.h"
#include "ppforce.h"
#include "octtree.h"
#include "ppforce.h"

#include <Eigen/Dense>
#include "adtree.h"

/*!
	Calculate the force between particles by direct gravity force calculation.
	
	In this routine summation of the gravity force is partially eliminated by
	summing the force to both particles in the same time.
	
	\param x specifies the vector of particle location x coordinate.
	\param y specifies the vector of particle location y coordinate.
	\param z specifies the vector of particle location z coordinate.
	\param m specifies the vector of particle mass.
	\param ax returns the vector of particle acceleration in x direction.
	\param ay returns the vector of particle acceleration in x direction.
	\param az returns the vector of particle acceleration in x direction.

	\retrun no return.
*/
void
HPPDirectForce::CalcForce( const vectorType& x, const vectorType& y, const vectorType& z, 
						const vectorType& m,
						vectorType& ax, vectorType& ay, vectorType& az ) {
	int np = x.size();
//	std::cout << "CalcForce : " << x.begin() << " to " << x.end() << std::endl;
#ifdef _H_ENABLE_EIGEN_
	for( int i = 0; i < x.size(); i++ ) {
#else
	for( int i = x.begin(); i < x.end(); i++ ) {
#endif
		for( int j = i+1; j < np; j++ ) {
			scalarType rx,ry,rz, r3inv;
			rx = x[j] - x[i];
			ry = y[j] - y[i];
			rz = z[j] - z[i];
			r3inv = sqrt(rx*rx + ry*ry + rz*rz + 0.01);
			r3inv = 1.0/(r3inv*r3inv*r3inv);
			ax[i] += m[j] * rx * r3inv;
			ay[i] += m[j] * ry * r3inv;
			az[i] += m[j] * rz * r3inv;									
			ax[j] += -m[i] * rx * r3inv;
			ay[j] += -m[i] * ry * r3inv;
			az[j] += -m[i] * rz * r3inv;									
		}
	}
//	std::cout << "CalcForce A0 are : " << ax[0] << ", " << ay[0] << ", " << az[0] << std::endl;
//	std::cout << "CalcForce A1 are : " << ax[1] << ", " << ay[1] << ", " << az[1] << std::endl;
	return;
}

void
HPPOTreeForce::MakeTree( const vectorType& x, const vectorType& y, const vectorType& z, 
						   const vectorType& m ) {
	node nd;
	int n = x.size();
	Locus  centre, length;
	Locus xmax,xmin;
#ifdef _H_ENABLE_EIGEN_
    xmin.x = xmax.x = x[0];
    xmin.y = xmax.y = y[0];
    xmin.z = xmax.z = z[0];
#else
	xmin.x = xmax.x = x[x.begin()];
	xmin.y = xmax.y = y[x.begin()];
	xmin.z = xmax.z = z[x.begin()];
#endif
	
//	std::cout << "MakeTree::IN size is " << _tr.size() << std::endl;

#ifdef _H_ENABLE_EIGEN_
    for( int i = 0; i < x.size(); i++ ) {
#else
    for( int i = x.begin(); i < x.end(); i++ ) {
#endif
		if( xmin.x > x[i] ) xmin.x = x[i];
		if( xmax.x < x[i] ) xmax.x = x[i];
		if( xmin.y > y[i] ) xmin.y = y[i];
		if( xmax.y < y[i] ) xmax.y = y[i];
		if( xmin.z > z[i] ) xmin.z = z[i];
		if( xmax.z < z[i] ) xmax.z = z[i];
	}
		
	centre = 0.5 * ( xmax + xmin );
	length = 1.1 * ( xmax - xmin );
	if( length.x == 0.0 ) length.x = 1.0;
	if( length.y == 0.0 ) length.y = 1.0;
	if( length.z == 0.0 ) length.z = 1.0;
	nd.centre = centre;
	nd.length = length;
	_tr.set_root( nd );

#ifdef _H_ENABLE_EIGEN_
        for( int i = 0; i < x.size(); i++ ) {
#else
        for( int i = x.begin(); i < x.end(); i++ ) {
#endif
		OctTree::iterator it = _tr.begin();
		nd.p = property( i, m[i], Locus(x[i], y[i], z[i]) );
//		it = _tr.find( nd.p.x );
		_tr.insert( it, nd );
	}

	_tr.calcNodeMass( _tr.begin() );
	
//	std::cout << "MakeTree::OUT SIZE is " << _tr.size() << std::endl;
	
	return;
}

void
HPPOTreeForce::CalcForce( const vectorType& x, const vectorType& y, const vectorType& z, 
							   const vectorType& m,
								vectorType& ax, vectorType& ay, vectorType& az ) {
								
	int _n = x.size();
	for( int i = 0; i < _n; i++ ) {
		float force = 0.0;
		Locus _dist, _x = Locus( x[i], y[i], z[i] );
		scalarType rx,ry,rz, r3inv;
		std::stack<OctTree::iterator> buff;
		OctTree::iterator it = _tr.begin();
		buff.push(it);
		while( !buff.empty() ) {
			it = buff.top();
			buff.pop();
			if( it->p.id == i ) continue; // avoid self force.
			float l = it->length.x;
			_dist = it->p.x - _x;
			float d = Locus( _dist ).norm();
			if( l/d < _theta0 || _tr.no_child(it) ) {
				r3inv = sqrt(d*d + 0.01);
				r3inv = 1.0/(r3inv*r3inv*r3inv);
				ax[i] += it->p.mass * _dist.x * r3inv;
				ay[i] += it->p.mass * _dist.y * r3inv;			
				az[i] += it->p.mass * _dist.z * r3inv;
//				std::cout << "CalcForce at " << i << " : " << it->p.id << " : " << ax[i] << ", " << ay[i] << ", " << az[i] << std::cout;
			} else {
				for( int j = 0; j < 8; j++ ) {
					if( _tr.is_child_at(j,it) ) buff.push( _tr.get_child_at(j,it) );
				} 
			}
		}
	}	

	return;
}


/*!
	Alternate Digital Tree
*/
void
HPPADTreeForce::MakeTree( const vectorType& x, const vectorType& y, const vectorType& z, 
						   const vectorType& m ) {


	ADnode _top;
	int n = x.size();

	_atr.set_bounding_box( _top.min, _top.max, x,y,z );

#ifdef _H_ENABLE_EIGEN_
    int bgn = 0;
#else
    int bgn = x.begin();
#endif
	_top.dir = 0;
	_top.p = property( bgn, m[bgn], Locus(x[bgn], y[bgn], z[bgn]) );
	_atr.set_root( _top );

	ADnode nd;
#ifdef _H_ENABLE_EIGEN_
    for( int i = 1; i < x.size(); i++ ) {
#else
    for( int i = x.begin()+1; i < x.end(); i++ ) {
#endif
		ADTree::iterator it = _atr.begin();
		nd.p = property( i, m[i], Locus(x[i], y[i], z[i]) );
		_atr.insert( it, nd );
	}

	_atr.calc_node_mass( _atr.begin() );
	
	return;
}


void
HPPADTreeForce::CalcForce( const vectorType& x, const vectorType& y, const vectorType& z, 
							   const vectorType& m,
								vectorType& ax, vectorType& ay, vectorType& az ) {
								
	int _n = x.size();
	for( int i = 0; i < _n; i++ ) {
		float force = 0.0;
		Locus _dist, _x = Locus( x[i], y[i], z[i] );
		scalarType rx,ry,rz, r3inv;
		std::stack<ADTree::iterator> buff;
		ADTree::iterator it = _atr.begin();
		buff.push(it);
		while( !buff.empty() ) {
			it = buff.top();
			buff.pop();
			if( it->p.id == i ) continue; // avoid self force.
			float l = it->max.x-it->min.x;
			_dist = it->p.x - _x;
			float d = Locus( _dist ).norm();
			if( l/d < _theta0 || _atr.no_child(it) ) {
				r3inv = sqrt(d*d + 0.01);
				r3inv = 1.0/(r3inv*r3inv*r3inv);
				ax[i] += it->p.mass * _dist.x * r3inv;
				ay[i] += it->p.mass * _dist.y * r3inv;			
				az[i] += it->p.mass * _dist.z * r3inv;
			} else {
				if( _atr.is_left_child(it) ) buff.push( _atr.get_child_at(it,0) );
				if( _atr.is_right_child(it) ) buff.push( _atr.get_child_at(it,1) );
			}
		}
	}
	
	return;
}
