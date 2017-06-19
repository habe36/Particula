/*
 Class Name --- OctTree
 
 Description - A class deal with octet tree algorithm.
	Octet tree is a extention of binary tree but eight children.
	The eight number is coincide with the eight zone of three dimensional
	space. A node is corresponding to a space cell and can have up to eight
	child-cells.

 $Author: hiroshi $
 $Date: 2007/03/19 13:17:20 $
 $Revision: 1.4 $
 
 Copyrights 2007 All Rights Reserved by HABE, aka ABE Hiroshi, JAPAN.

*/

#ifndef __OCTTREE_H___
#define __OCTTREE_H___

#include <H/Htree.h>
#include <valarray>
#include <vector>
#include <iostream>
#include <cmath>

#include <cassert>

#include "datatypes.h"

/*!
  node of the OctTree.
  
  \param centre specifies the centre of the cell when the node is the leaf node.
  if not, specifies the centre of mass.
  \param length specifies the length of the cell of each direction.
 */
struct node {
  property p;
  Locus centre;
  Locus length;
  int whichOctant( Locus x );
  void setOctant( Locus& cent, Locus& leng, int oct );
  bool isInside( Locus& x );
  coordType distance();
};

class OctTree : public H::htree<node,8> {
public:
	OctTree() : H::htree<node,8>(), m_inserting(false) {}
	virtual		~OctTree() {}
	void		lock() { 
		while(m_inserting) {}
		m_inserting = true;
	}
	void		unlock() { m_inserting = false; }
	iterator	insert( iterator it, node& nd );
	iterator	find( Locus& x );
	void		calcNodeMass( iterator it );
private:

	bool		m_inserting;
};

inline
bool
node::isInside( Locus& x ) {
  Locus _len = x - centre;
  if( std::abs(_len.x) <= 0.5*length.x && 
      std::abs(_len.y) <= 0.5*length.y && 
      std::abs(_len.z) <= 0.5*length.z ) return true;
  return false;
}

inline
int
node::whichOctant( Locus x ) {
  assert( length.x != 0.0 || length.y != 0.0 || length.z != 0.0 );
  int n;
  Locus ost = x - centre;
  Locus mx = 0.5*length + centre;
  Locus mn = centre - 0.5 * length;
  /*
  int ss;
  if(!( std::abs(ost.x) <= 0.5*length.x )) {
	  ss = 1;
  }
  if(!( std::abs(ost.y) <= 0.5*length.y )) {
	  ss = 2;
  }
  if(!( std::abs(ost.z) <= 0.5*length.z )) {
	  ss = 3;
  }
  assert( std::abs(ost.x) <= 0.5*length.x );
  assert( std::abs(ost.y) <= 0.5*length.y );
  assert( std::abs(ost.z) <= 0.5*length.z );
  */
  
  if( ost.x >= 0 && ost.y >= 0 && ost.z >= 0 ) { // first octant
    n = 1;
  } else if( ost.x < 0 && ost.y >= 0 && ost.z >= 0 ) { // second octant
    n = 2;
  } else if( ost.x < 0 && ost.y < 0 && ost.z >= 0 ) { // third octant
    n = 3;
  } else if( ost.x >= 0 && ost.y < 0 && ost.z >= 0 ) { // fourth octant
    n = 4;
  } else if( ost.x >= 0 && ost.y >= 0 && ost.z < 0 ) { // fifth octant
    n = 5;
  } else if( ost.x < 0 && ost.y >= 0 && ost.z < 0 ) { // fifth octant
    n = 6;
  } else if( ost.x < 0 && ost.y < 0 && ost.z < 0 ) { // seventh octant
    n = 7;
  } else { // eighth octant
    n = 8;
  }
  return n;
}

inline
void
node::setOctant( Locus& cent, Locus& leng, int oct ) {
  assert( 0 < oct && oct < 9 );
  leng = 0.5 * length;
  switch( oct ) {
  case 1:
    cent = centre + 0.5 * leng;
    break;
  case 2:
    cent = centre + 0.5 * leng;
    cent.x = cent.x - leng.x;
    break;
  case 3:
    cent = centre - 0.5 * leng;
    cent.z = cent.z + leng.z;
    break;
  case 4:
    cent = centre + 0.5 * leng;
    cent.y = cent.y - leng.y;
    break;
  case 5:
    cent = centre + 0.5 * leng;
    cent.z = cent.z - leng.z; 
    break;
  case 6:
    cent = centre - 0.5 * leng;
    cent.y = cent.y + leng.y;
    break;
  case 7:
    cent = centre - 0.5 * leng;
    break;
  case 8:
    cent = centre - 0.5 * leng;
    cent.x = cent.x + leng.x;
    break;
  }
  return;
}

#endif
