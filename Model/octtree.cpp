/*

 Class Name --- OctTree
 
 Description - A class deal with octet tree algorithm.
	Octet tree is a extention of binary tree but eight children.
	The eight number is coincide with the eight zone of three dimensional
	space. A node is corresponding to a space cell and can have up to eight
	child-cells.

 $Author: hiroshi $
 $Date: 2007/03/19 13:17:20 $
 $Revision: 1.3 $
 
 Copyrights 2007 All Rights Reserved by HABE, aka ABE Hiroshi, JAPAN.

 Ref.	Barnes J. E., Hut P., 1986, Natur, 325, 446.
		Yelick (online doc) LBNL/UCB
 
*/

#include "octtree.h"

/*!
OctTree Inplementation.

 */
OctTree::iterator
OctTree::insert( iterator it, node& nd ) {

	Locus	x = nd.p.x;
	int oct = it->whichOctant(x);
	
// if no child at oct.
	if( !is_child_at( oct-1, it ) ) {
		it->setOctant( nd.centre, nd.length, oct );
		return set_child_at( oct-1, it, nd );
// else
	} else {
		it = get_child_at( oct-1, it );
		if( no_child(it) ) {
			node nd_org = *it;
			int oct_org = it->whichOctant( nd_org.p.x );
			it->setOctant( nd_org.centre, nd_org.length, oct_org );
			it->p.id = -9; // change the node into internal.
			set_child_at( oct_org-1, it, nd_org );
			insert( it, nd );
		} else {
			insert( it, nd );
		}
	}
}

/*!
  find the leaf node which contains 'x' and return the position.
  if no leaf find return end();
  \param x
  \return 
 */
OctTree::iterator
OctTree::find( Locus& x ) {
  iterator iit,it = begin();
  int oct = it->whichOctant(x)-1;
  while( is_child_at( oct, it ) ) {
	try {
	   iit = get_child_at( oct, it );
	} catch( std::string err ) {
	   std::cerr << err << std::endl;
	   return it;
	} catch(...) {
		exit(0);
	}
	it = iit;
    oct = it->whichOctant(x)-1;
    assert( it->isInside(x) );
  }

  return it; // return the leaf node.
}

/*!
	Calculate the total mass under 'it'.
	The leaf node contains the mass of the particle which is contained
	and all the remain intenal node have the total mass under the node.
	
*/
void
OctTree::calcNodeMass( iterator it ) {

  // if leaf node then return.
  if( no_child(it) ) return;

  // then, this node should be internal node.
  it->p.mass = 0;
  Locus com = Locus(0,0,0);
  for( int i = 0; i < 8; i++ ) {
    if( is_child_at( i, it ) ) {
      iterator cid = get_child_at( i, it );
      calcNodeMass( cid );
      it->p.mass += cid->p.mass;
      com += cid->p.mass * cid->p.x;
    }
  }
  it->p.x = (1.0/it->p.mass) * com ;

  return;
}

