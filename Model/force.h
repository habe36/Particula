/*
 
 Class Name --- HPPForce
 
 Description - An abstructed class for Particle Particle Force calculation.

 $Author: hiroshi $
 $Date: 2007/03/19 13:17:20 $
 $Revision: 1.2 $
 
 Copyrights 2006, 2007 All Rights Reserved by HABE, aka ABE Hiroshi, JAPAN.

*/

#ifndef __H__FORCE__H__
#define __H__FORCE__H__

#include <iostream>
#include <vector>
#include <string>

#include "datatypes.h"

class HPPForce {
public:
	HPPForce() {}
	virtual ~HPPForce() {}
	virtual void	CalcForce( const vectorType& x, const vectorType& y, const vectorType& z, 
							   const vectorType& m,
							   vectorType& ax, vectorType& ay, vectorType& az ) = 0;
	
	virtual void	PreForce( const vectorType& x, const vectorType& y, const vectorType& z, 
							   const vectorType& m ) = 0;
							   
	virtual void	PostForce( const vectorType& x, const vectorType& y, const vectorType& z, 
							   const vectorType& m ) = 0;

	static void			GetMethods( std::vector<std::string>& methods ) ;	
	static HPPForce*	CalcMethod( int method );
	
private:
	HPPForce( const HPPForce& s) {}
	HPPForce& operator = ( const HPPForce& s ) {
		return *this;
	}
};

class HPMForce {
};

#endif