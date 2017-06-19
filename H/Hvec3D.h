#ifndef __H_VEC3D_H_
#define __H_VEC3D_H_

/*! @file
 @brief the file contains a class Hvec3D, which handle a three dimensional vector.

 Class Name --- Hvec3D
 
 Three dimensional vector class with (x,y,z) components of coordinates
 in three dimension.

 Basic operation of vector analysis are implemented.
 Be care of the operator priority, especially outer and innner
 products. These operators are lower priority than addition.

 $Author: habe $
 $Date: 2017/03/16 00:32:44 $
 $Revision: 1.13 $
 
 Copyrights 2007 All Rights Reserved by HABE, aka ABE Hiroshi, JAPAN.

*/

#include <iostream>
#include <cmath>
#include <complex>

namespace H {

using namespace std;

/*! \brief The Hvec3D class deal with the 3D vector.

	The Hvec3D class deal with the three dimensional vector data and its
	operations such as arithmetics, inner product etc.
	
	The Hvec3D class provides the interfaces to the vectors in Cartesian, 
	Cylindorical and Spherical Coordinates but cannot translates the vector 
	
*/
template <class TYPE> // TYPE should be one of int, float, double and complex
class Hvec3D
{
public:
// ---------------------------------------------
// * Constructors
//
//   (x,y,z) for Cartesian Coord.
//   (r,\theta, z) for Cylindrical Coord. (r,t,z)
//   (r,\theta, \phi) for Spherical Coord. (r,t,p)
// ---------------------------------------------
/// default constructor and assign zero to each components.
	Hvec3D() : x(m_el[0]), y(m_el[1]), z(m_el[2]),
	r(x), t(y), p(z) {
		x = 0; y = 0; z = 0;
	} 

	Hvec3D( TYPE vx, TYPE vy, TYPE vz = 0 ) : x(m_el[0]), y(m_el[1]), z(m_el[2]),
	r(x), t(y), p(z){ 
		x = vx;
		y = vy;
		z = vz;
	}

	Hvec3D( const Hvec3D& v ) : x(m_el[0]), y(m_el[1]), z(m_el[2]),
	r(x), t(y), p(z) {
		x = v.x;
		y = v.y;
		z = v.z;		
	}
  
// ---------------------------------------------
// * Destructors
// ---------------------------------------------
	virtual		~Hvec3D() {}

/*! \brief calculate the norm of the vector.
	\return returns the norm of the vector.
*/
  double norm( void ) const {
    return sqrt( x * x + y * y + z * z );
  }

/*! \brief generate a normalized vector.
	\return returns the normalized vector.
*/
  Hvec3D normalized( void ) const {
    return *this / norm();
  }
  
  Hvec3D& operator = ( const Hvec3D& s ) {
    x = s.x;
    y = s.y;
    z = s.z;
    return *this;
  }

  TYPE& operator [] ( size_t i ) {
	  return m_el[i];
  }
  const TYPE& operator [] ( size_t i ) const {
	  return m_el[i];
  }
// ---------------------------------------------
// * Public Data
// ---------------------------------------------
//
// for Descartesian Coordinates
  TYPE &x ; ///< x comp. of vector in Descartesian Coordinates.
  TYPE &y ; ///< y comp. of vector in Descartesian Coordinates.
  TYPE &z ; ///< z comp. of vector in Descartesian Coordinates.

  // for Cylindrical Coordinates
  TYPE &r; ///< radial component of vector in Cylindrical Coordinates.
  TYPE &t; ///< tangential component of vector in Cylindrical Coordinates.

// for Spherical Coordinates, but maybe useless so far.
  TYPE &p; ///< 

/*!	\brief operator add for vector.

	\return returns the result of addtion.
	\param vec specifies the vector to be added.
*/
Hvec3D operator + ( const Hvec3D& vec ) const {
  Hvec3D temp;
  temp.x = x + vec.x;
  temp.y = y + vec.y;
  temp.z = z + vec.z;
  return temp;
}

/*! \brief operator substraction for vector.

	\return returns the result of substraction.
	\param vec specifies a vector to be subtracted.
*/
Hvec3D operator - ( const Hvec3D& vec ) const {
  Hvec3D temp;
  temp.x = x - vec.x;
  temp.y = y - vec.y;
  temp.z = z - vec.z;
  return temp;
}

/*! \brief operator multiplication for vector with scalar.

	each components is multiplied with the scalar value.
	\return returns the result of multiplication.
	\param mul scalar data to multiply each comp. of the vector.
*/
Hvec3D operator * ( TYPE mul ) const {
	Hvec3D temp;
	temp.x = x * mul;
	temp.y = y * mul;
	temp.z = z * mul;
	return temp;
}

/*! \brief operator division by scalar.
	each component of the vector is divided by the inputed scalar.
	\return returns the divided vector by div.
	\param div specifies the divisor.
*/
Hvec3D operator / ( TYPE div ) const {
	Hvec3D temp;
	temp.x = x / div;
	temp.y = y / div;
	temp.z = z / div;
	return temp;
}

/*! \brief outer product oeprator for vector.

	caution!. the operator ^ is less priority operator so
	it might be safe to be braced the operation.

	\return returns a resultant vector by outer product.
	\param vec specifies the vector for outer product.
*/
Hvec3D operator ^ ( const Hvec3D& vec ) const {
	Hvec3D temp;
	temp.x = y * vec.z - z * vec.y;
	temp.y = z * vec.x - x * vec.z;
	temp.z = x * vec.y - y * vec.x;
	return temp;
}

/*!	\brief inner product operator for vector.

	caution!. the operator % is less priority operator so
	it might be safe to be braced the operation.

	\return returns a resultant vector of inner product.
	\param vec specifies the vector to be inner producted.
	
*/
TYPE operator % ( const Hvec3D& vec ) const {
	TYPE temp;
	temp = x * vec.x + y * vec.y + z * vec.z;
	return temp;
}

inline 
Hvec3D& operator += ( const Hvec3D& s ) {
  this->x += s.x;
  this->y += s.y;
  this->z += s.z;
  return *this;
}

inline 
Hvec3D& operator -= ( const Hvec3D& s ) {
  this->x -= s.x;
  this->y -= s.y;
  this->z -= s.z;
  return *this;
}

inline 
Hvec3D& operator *= ( TYPE mul ) {
  this->x *= mul;
  this->y *= mul;
  this->z *= mul;
  return *this;
}

/*! \brief output stream interface.

	output x,y and z component of the vector to the stream.

	\return returns ostream to have been outputed.
	\param os specifies the ostream instance to be outputed.

*/
ostream& put_stream ( ostream& os ) const
  {
    os << x << " " << y << " " << z;
    return os;
  }

/*! \brief input stream interface.

	input x,y and z component of the vector from the stream.

	\return returns istream to have been outputed.
	\param is specifies the istream instance to be inputed.

*/
istream& get_stream ( istream& is )
  {
    is >> x;
    is >> y;
    is >> z;
    return is;
  }
  
/*!	\brief equality operator for vector.

	\retval true if each component of the vector is equal.
	\retval false if one or more of the component is not equal.

	\param v specifies a vector to be compared.
*/
bool operator == ( const Hvec3D& v ) const {
  return ( (x == v.x) && (y == v.y) && (z == v.z) );
}

bool operator != ( const Hvec3D& v ) const {
    return !operator==(v);
}
/*
friend	bool	operator == ( const Hvec3D& v1,	const	Hvec3D& v2 ) {
	return ( (v1.x == v2.x) && (v1.y == v2.y) && (v1.z == v2.z) );
}
*/
protected:
	TYPE	m_el[3];
};

/*! \brief multiplication operator overload for Vec3D
	\param f specifies the value to multiply.
	\param s specifes the vector to be multiplied.
	\return returns vector to be multiplied.
*/
template<class TYPE, class DATA>
inline
Hvec3D<TYPE> operator * ( DATA f, const Hvec3D<TYPE>& s ) {
  Hvec3D<TYPE>	_x;
  _x.x = f * s.x;
  _x.y = f * s.y;
  _x.z = f * s.z;
  return _x;
}

/*! \brief unary minus operator.

	\return returns the vector of which components change its sign.
	\param s specifies the vector to be change the sign of componentts.
*/
template <class TYPE>
Hvec3D<TYPE>
operator - ( const Hvec3D<TYPE>& s ) {
	return Hvec3D<TYPE>( -s.x, -s.y, -s.z );
}

/*! \brief ostream operator.

	\return \returns the ostream instance to which the data has printed.
	\param os specifes an ostream instance to output.
	\param v specifies a vector to be output.
*/
template <class TYPE>
ostream& operator << ( ostream& os, const Hvec3D<TYPE>& v ) {
	return v.put_stream( os );
}

/*! \brief istream operator.

	\return \returns the istream instance from which the data has inputed.
	\param is specifes an istream instance to input.
	\param v specifies a vector to be input.
*/
template <class TYPE>
istream& operator >> ( istream& is, Hvec3D<TYPE>& v ) {
	return v.get_stream( is );
}
/*
 Specializations. In case of TYPE is complex, the norm calc. should
 be treat defferently with real number.
 */
    
template <>
inline double
Hvec3D<complex<double> >::norm( void ) const {
    return sqrt( std::norm(x) + std::norm(y) + std::norm(z) );
}
    
template <>
inline double
Hvec3D<complex<float> >::norm( void ) const {
    return sqrt( std::norm(x) + std::norm(y) + std::norm(z) );
}

} // end of namespace.

#endif
