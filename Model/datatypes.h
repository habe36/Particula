/*

 The file contains basic data types for the simulation.

 $Author: habe $
 $Date: 2017/06/19 07:19:53 $
 $Revision: 1.11 $
 
 Copyrights 2006,2007 All Rights Reserved by HABE, aka ABE Hiroshi, JAPAN.

*/
#ifndef __H_DATATYPES_H__
#define __H_DATATYPES_H__

#include <vector>
//#include <macstl/valarray.h>
//#include <valarray>
#include	"H/Hvec3D.h"
#ifdef _H_ENABLE_EIGEN_
#include <Eigen/Dense>
typedef Eigen::VectorXf vectorType;
#else
#include	"H/Hvector.h"
typedef	H::V::Hvector	vectorType;
#endif
const	float	c_pi = 3.1415926;					//!< ratio of the circumference.

//#define scalarType  float
//#define coordType	float
//#define vectorType  std::vector
//#define vectorType	stdext::valarray
//#define vectorType	std::valarray<float>
//#define vectorType	vectorType
#define		vecontType	std::vector

typedef float			scalarType;
typedef float			coordType;

typedef vectorType*		vectorTypeP;
typedef vectorType&		vectorTypeR;

typedef H::Hvec3D<coordType>		Locus;

struct Locus2 {
	coordType	x,y;
	Locus2() {}
	Locus2( coordType _x, coordType _y ) {
		x = _x;
		y = _y;
	}
};

/*!
  property stores the data concerning the mass body.
  when this indicates the mass body, the id indicates the id number and 
  x specifies the location of it. when this indicates the total 
 */
struct property {
	int				id;
	scalarType		mass;
	Locus			x;
	property();
	property( int _id, float _ms, Locus _x );
};

/*! ------------------------------------------------------------------

  property inplementation.

  --------------------------------------------------------------------
 */
inline
property::property() {
  id = -8;
  mass = 0;
  x = Locus(0,0,0);
}

inline
property::property( int _id, float _ms, Locus _x ) {
  id = _id;
  mass = _ms;
  x = _x;
}


#if defined( __SQRT__ )
inline float fsqrt ( float argument )
{
  float result;
  asm ( "fsqrts %0, %1" : /*OUT*/ "=f" ( result ) : /*IN*/ "f" ( argument ) );
  return result;
}
#endif

#endif