/*

 Functions to generate the initial condition for particle simulation.

 $Author: hiroshi $
 $Date: 2008/05/08 01:54:41 $
 $Revision: 1.7 $
 
 Copyrights 2006-2008 All Rights Reserved by HABE, aka ABE Hiroshi, JAPAN.

*/


#include "distributor.h"
#include "datatypes.h"
#include <cstdlib>
#include <cmath>

const scalarType one = 0.99f;

void
homo_sphere( scalarType& x, scalarType& y, scalarType& z ) {
	do {
		x =2*scalarType( rand() ) / RAND_MAX - 1;
		y =2*scalarType( rand() ) / RAND_MAX - 1;
		z =2*scalarType( rand() ) / RAND_MAX - 1;
	} while( (x*x + y*y + z*z) > 1.0 );

	return;
}

scalarType
gaussian(void) {
	scalarType x,y,r2;
	scalarType z;
	
	do {
		x = 2.0*( scalarType(rand())/RAND_MAX ) - 1.0;
		y = 2.0*( scalarType(rand())/RAND_MAX ) - 1.0;
		r2 = x*x + y*y;
	} while(r2 >= 1.0 || r2 == 0.0 );
	
	z = sqrt(-2.0*log(r2)/r2)*x;

	return z;
}

Locus
rand_spherical( scalarType r ) {
	Locus v;
	float theta = acos( frand(-1,1) );
	float phi = frand( 0, 2*c_pi);
	v.x = r * sin(theta)*cos(phi);
	v.y = r * sin(theta)*sin(phi);
	v.z = r * cos(theta);
	return v;
}

Locus
rand_cylindrical( scalarType r ) {
	Locus v;
	float phi = frand(0,2*c_pi);
	v.x = r * cos(phi);
	v.y = r * sin(phi);
	v.z = 0.0;
	return v;
}

scalarType
frand( scalarType mn, scalarType mx ) {
	assert(mn<mx);
	return mn + (mx-mn)*(scalarType(rand())/RAND_MAX);
}

void
dist_plummer( int n, vectorType& x, vectorType&y, vectorType& z, vectorType& vx, vectorType& vy, vectorType& vz ) {

	for( int i = 0; i < n; i++ ) {
		scalarType r = 1.0 / sqrt( pow( frand(0,one), -2.0f/3.0f ) - 1.0f );
		Locus pos = rand_spherical( r );
		scalarType r1 = 0.0;
		scalarType r2 = 0.1;
		while( r2 > r1*r1*pow(1.0f-r1*r1,3.5f) ) {
			r1 = frand(0,1);
			r2 = frand(0,0.1);
		}
		// obtain the escape velocity at "r".
		scalarType ve = sqrt(2.0) * pow(1.0+r*r,-0.25);
		scalarType f = r1 * ve;
		Locus vec = rand_spherical( f );
		
		x[i]	= pos.x;
		y[i]	= pos.y;
		z[i]	= pos.z;
		vx[i]	= vec.x;
		vy[i]	= vec.y;
		vz[i]	= vec.z;
	}
}

void
dist_hernquist( int n, vectorType& x, vectorType&y, vectorType& z, vectorType& vx, vectorType& vy, vectorType& vz ) {
	for( int i = 0; i < n; i++ ) {
		scalarType rd = sqrt( frand(0,one) );
		scalarType r = rd / ( 1.0 - rd );
		Locus pos = rand_spherical( r );
		scalarType r1 = 0.0;
		scalarType r2 = 0.1;
		while( r2 > r1*r1*pow(1.0f-r1*r1,3.5f) ) {
			r1 = frand(0,1);
			r2 = frand(0,0.1);
		}
		// obtain the escape velocity at "r".
		scalarType ve = sqrt(2.0/(1+r));
		scalarType f = r1 * ve;
		Locus vec = rand_spherical( f );
		
		x[i]	= pos.x;
		y[i]	= pos.y;
		z[i]	= pos.z;
		vx[i]	= vec.x;
		vy[i]	= vec.y;
		vz[i]	= vec.z;
	}
}

void
dist_jaffe( int n, vectorType& x, vectorType&y, vectorType& z, vectorType& vx, vectorType& vy, vectorType& vz ) {
	for( int i = 0; i < n; i++ ) {
		scalarType rd = frand(0,one);
		scalarType r = rd / ( 1.0 - rd );
		Locus pos = rand_spherical( r );
		scalarType r1 = 0.0;
		scalarType r2 = 0.1;
		while( r2 > r1*r1*pow(1.0f-r1*r1,3.5f) ) {
			r1 = frand(0,1);
			r2 = frand(0,0.1);
		}
		// obtain the escape velocity at "r".
		scalarType ve = sqrt(-2.0*log(r/(1+r)));
//		std::cout << "Jaffe Escape Velocity : " << ve << " at " << r << std::endl;
		scalarType f = r1 * ve;
		Locus vec = rand_spherical( f );
		
		x[i]	= pos.x;
		y[i]	= pos.y;
		z[i]	= pos.z;
		vx[i]	= vec.x;
		vy[i]	= vec.y;
		vz[i]	= vec.z;
	}
}

void
dist_miyamoto( int n, scalarType beta, vectorType& x, vectorType&y, vectorType& z, vectorType& vx, vectorType& vy, vectorType& vz ) {
	for( int i = 0; i < n; i++ ) {
		scalarType R,Z, rd,p;
		scalarType z1,zs1,zb1;
		R = 0.0;
		Z = 0.0;
		p = 1.0;
		z1 = 1;
		zs1 = 1;
		zb1 = 1+beta;
		while( p > (R*R + (3.0f*zb1-2.0f)*zb1*zb1)/(4.0f*c_pi*pow( R*R+zb1*zb1, 2.5f )*pow(z1,1.5f)) ) {
			R = 1.0f / sqrt( pow( frand(0.0f,one), -2.0f/3.0f ) - 1.0f );
			rd = frand(-one,one);
			Z = rd / ( 1.0f - rd*rd );
			z1 = 1.0f + Z*Z;
			zs1 = sqrt(z1);
			zb1 = 1.0f+beta*zs1;
			p = frand(0.0f,1.0f);
		}
		Locus pos = rand_cylindrical( R );
		pos.z = Z;

		// next, velocity.
		scalarType r1 = 0.0f;
		scalarType r2 = 0.1f;
		while( r2 > r1*r1*pow(1.0f-r1*r1,3.5f) ) {
			r1 = frand(0,1);
			r2 = frand(0,0.1);
		}
		// obtain the escape velocity at "R".
		scalarType ve = sqrt(2.0f) * pow(1.0f+R*R,-0.25f);
		scalarType f = r1 * ve;
		Locus vec = rand_cylindrical( f );
		
		x[i]	= pos.x;
		y[i]	= pos.y;
		z[i]	= pos.z;
		vx[i]	= vec.x;
		vy[i]	= vec.y;
		vz[i]	= vec.z;
//		std::cout << "Miyamoto : " << i << ", " << pos << " end" << std::endl;
	}
}

