/*

 Functions to generate the initial condition for particle simulation.

 $Author: hiroshi $
 $Date: 2008/05/02 00:38:29 $
 $Revision: 1.4 $
 
 Copyrights 2006-2008 All Rights Reserved by HABE, aka ABE Hiroshi, JAPAN.

*/
#ifndef __H__DISTRIBUTOR__H__
#define __H__DISTRIBUTOR__H__

// this file provides the initial particle distribution functions.

#include "datatypes.h"

extern void homo_sphere( scalarType& x, scalarType& y, scalarType& z );

// gaussian gives gaussian distribution with mean = 0.0, 
// disparsion = 1.0 by Box-Mueller method. 
extern scalarType gaussian( void );

extern Locus rand_spherical( scalarType r );
extern Locus rand_cylindorical( scalarType r, scalarType z );

extern float frand( float mn, float mx );

extern void		dist_plummer( int n, vectorType& x, vectorType&y, vectorType& z, vectorType& vx, vectorType& vy, vectorType& vz );
extern void		dist_hernquist( int n, vectorType& x, vectorType&y, vectorType& z, vectorType& vx, vectorType& vy, vectorType& vz );
extern void		dist_jaffe( int n, vectorType& x, vectorType&y, vectorType& z, vectorType& vx, vectorType& vy, vectorType& vz );
extern void		dist_miyamoto( int n, scalarType beta, vectorType& x, vectorType&y, vectorType& z, vectorType& vx, vectorType& vy, vectorType& vz );

#endif