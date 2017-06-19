#ifndef __H__HPPFORCE_H__
#define __H__HPPFORCE_H__
/*

 Class Name --- HPPDirectForce/HPPOctTreeForce/HPPADTreeForce
 
 Description - Class implementation for particle-particle force calculation
			with Direct method, Octet tree method and Alternate digital tree
			method.

 $Author: habe $
 $Date: 2017/06/19 07:19:53 $
 $Revision: 1.4 $
 
 Copyrights 2007 All Rights Reserved by HABE, aka ABE Hiroshi, JAPAN.

*/

/*
 *  ppforce.h
 *  NBody
 *
 *  Created by HABE on 06/11/27.
 *  Copyright 2006 HABE. All rights reserved.
 *
 */

#include "Model/force.h"
#include "Model/datatypes.h"
#include "Model/octtree.h"
#include "Model/adtree.h"

#include <Eigen/Dense>

class HPPDirectForce : public HPPForce {
public:
	HPPDirectForce() : HPPForce() {}
	virtual ~HPPDirectForce() {}
	virtual void	CalcForce( const vectorType& x, const vectorType& y, const vectorType& z, 
							   const vectorType& m,
								vectorType& ax, vectorType& ay, vectorType& az );
	virtual void	PreForce( const vectorType& x, const vectorType& y, const vectorType& z, 
							   const vectorType& m ) {}
	virtual void	PostForce( const vectorType& x, const vectorType& y, const vectorType& z, 
							   const vectorType& m ) {}

protected:
private:
	HPPDirectForce( const HPPDirectForce& s );
	HPPDirectForce& operator = ( const HPPDirectForce& s );


};

class HPPOTreeForce : public HPPForce {
public:
	HPPOTreeForce() : HPPForce(), _theta0(0.6) {}
	virtual ~HPPOTreeForce() {}
	virtual void	CalcForce( const vectorType& x, const vectorType& y, const vectorType& z, 
							   const vectorType& m,
								vectorType& ax, vectorType& ay, vectorType& az );
	virtual void	PreForce( const vectorType& x, const vectorType& y, const vectorType& z, 
							   const vectorType& m ) {
		MakeTree(x,y,z,m);
//		for( OctTree::pre_order_iterator it = _tr.begin(); it != _tr.end(); it++ ) {
//			std::cout << it->p.id << ":" << it->p.x.x << ", "  << it->p.x.y << ", "  << it->p.x.z << std::endl;
//		}
	}
	virtual void	PostForce( const vectorType& x, const vectorType& y, const vectorType& z, 
							   const vectorType& m ) {
		_tr.clear();
	}
protected:
private:
	OctTree		_tr;
	float		_theta0;

	HPPOTreeForce( const HPPOTreeForce& s );
	HPPOTreeForce& operator = ( const HPPOTreeForce& s );
	virtual void	MakeTree( const vectorType& x, const vectorType& y, const vectorType& z, 
							   const vectorType& m );
};

class HPPADTreeForce : public HPPForce {
public:
	HPPADTreeForce() : HPPForce(), _theta0(0.6) {}
	virtual ~HPPADTreeForce() {}
	virtual void	CalcForce( const vectorType& x, const vectorType& y, const vectorType& z, 
							   const vectorType& m,
								vectorType& ax, vectorType& ay, vectorType& az );
	virtual void	PreForce( const vectorType& x, const vectorType& y, const vectorType& z, 
							   const vectorType& m ) {
		MakeTree(x,y,z,m);
		
		ADTree::pre_order_iterator it = _atr.begin();
//		_atr.print_tree(it);
		
//		for( ADTree::pre_order_iterator it = _atr.begin(); it != _atr.end(); it++ ) {
//			std::cout << it->p.id << ":" << it->p.x.x << ", "  << it->p.x.y << ", "  << it->p.x.z << std::endl;
//		}
	}
							   
	virtual void	PostForce( const vectorType& x, const vectorType& y, const vectorType& z, 
							   const vectorType& m ) {
		_atr.clear();
	}

protected:
private:
	ADTree		_atr;
	float		_theta0;

	HPPADTreeForce( const HPPADTreeForce& s );
	HPPADTreeForce& operator = ( const HPPADTreeForce& s );
	virtual void	MakeTree( const vectorType& x, const vectorType& y, const vectorType& z, 
							   const vectorType& m );
};

#endif