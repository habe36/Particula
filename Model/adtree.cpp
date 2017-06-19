/*

 Class Name --- ADTree
 
 Description - An alternate digital tree which contains the distribution
		of particle location and its mass. The tree is expected less 
		computation than OctTree.

 $Author: habe $
 $Date: 2017/06/19 07:19:53 $
 $Revision: 1.5 $
 
 Copyrights 2007 All Rights Reserved by HABE, aka ABE Hiroshi, JAPAN.

 Ref. Bonet J. and Peraire J., 1991, IJNME, 1, 31.

*/

#include "adtree.h"
bool
ADnode::is_internal() const {
	return ( p.id < 0 );
}

void
ADnode::set_internal() {
	p.id = -9;
}

void
ADTree::gen_child_at( iterator it, int side, ADnode& new_one, const property prop ) {
	Locus&	_min = new_one.min;
	Locus&	_max = new_one.max;
	coordType	centre;

	new_one.p = prop;
	new_one.dir = (it->dir+1) % 3;
	_min = it->min;
	_max = it->max;
	
	switch( it->dir ) {
		case 0:
			centre = 0.5*(_max.x + _min.x);
			if( side == 0 ) {
				_max.x = centre;
			} else {
				_min.x = centre;
			}
				break;
		case 1:
			centre = 0.5*(_max.y + _min.y);
			if( side == 0 ) {
				_max.y = centre;
			} else {
				_min.y = centre;
			}
				break;
		case 2:
			centre = 0.5*(_max.z + _min.z);
			if( side == 0 ) {
				_max.z = centre;
			} else {
				_min.z = centre;
			}
				break;
	}
	return;
}


/*!
ADTree Inplementation.

 */
ADTree::iterator
ADTree::insert( iterator it, ADnode& nd ) {

	ADnode	new_one;
	Locus	_x = nd.p.x;
	Locus	_o = it->p.x;
	Locus	_min,_max;
	iterator	_ret;

/*
	switch( it->dir ) {
		case 0:
			std::cout << it->max.x << ", " << _x.x << " : " << _o.x << ", " << it->min.x << std::endl;
			break;
		case 1:
			std::cout << it->max.y << ", " << _x.y << " : " << _o.y << ", " << it->min.y << std::endl;
			break;
		case 2:
			std::cout << it->max.z << ", " << _x.z << " : " << _o.z << ", " << it->min.z << std::endl;
			break;
	}
*/
	_min = it->min;
	_max = it->max;
	int _x_side = it->which_side(_x);

	if( it->is_internal() ) {
		if( is_child_at(it, _x_side) ) {
			_ret = insert( get_child_at( it, _x_side ), nd );
		} else {
			gen_child_at( it, _x_side, new_one, nd.p );
			_ret = set_child_at( it, _x_side, new_one );
		}
	} else { // the node is leaf node.
		int _o_side = it->which_side(_o);
		if( _x_side == _o_side ) {
			// first, current node data move to the child node.
			gen_child_at( it,_o_side, new_one, it->p );
			// then, the current node changes to internal node.
			it->set_internal();
			// set the child and insert the new node.
			_ret = insert( set_child_at( it, _x_side, new_one ), nd );
		} else {
			// first, gen the it node.
			gen_child_at( it, _o_side, new_one, it->p );
			set_child_at( it, _o_side, new_one );
			// then add the nd node.
			gen_child_at( it, _x_side, new_one, nd.p );
			_ret = set_child_at( it, _x_side, new_one );
			
			it->set_internal();
		}
	}
	return _ret;
}
/*!

*/


int
ADnode::which_side( const Locus& x ) const {
	coordType _centre;
	coordType _loc, _min, _max;
	switch( dir ) {
		case 0:
			_min = min.x;
			_max = max.x;
			_loc = x.x;
			break;
		case 1:
			_min = min.y;
			_max = max.y;
			_loc = x.y;
			break;
		case 2:
			_min = min.z;
			_max = max.z;
			_loc = x.z;
			break;
	}

//	std::cout << "WhichSide: " << _max << ", " << _loc << ", " << _min << std::endl;

	assert( _max >= _loc );
	assert( _min <= _loc );

	_centre = 0.5*(_min + _max);

	if( _loc < _centre ) return 0;
	else return 1;
}



void
ADTree::set_bounding_box( Locus& xmin, Locus& xmax, const vectorType& x, const vectorType& y, const vectorType& z ) {
#ifdef _H_ENABLE_EIGEN_
    xmin.x = xmax.x = x[0];
    xmin.y = xmax.y = y[0];
    xmin.z = xmax.z = z[0];
    for( int i = 0; i < x.size(); i++ ) {
#else
    xmin.x = xmax.x = x[x.begin()];
	xmin.y = xmax.y = y[x.begin()];
	xmin.z = xmax.z = z[x.begin()];
	for( int i = x.begin(); i < x.end(); i++ ) {
#endif
		if( xmin.x > x[i] ) xmin.x = x[i];
		if( xmax.x < x[i] ) xmax.x = x[i];
		if( xmin.y > y[i] ) xmin.y = y[i];
		if( xmax.y < y[i] ) xmax.y = y[i];
		if( xmin.z > z[i] ) xmin.z = z[i];
		if( xmax.z < z[i] ) xmax.z = z[i];
	}
		
	Locus centre = 0.5 * ( xmax + xmin );
	Locus length = 1.1 * ( xmax - xmin );
	if( length.x == 0.0 ) length.x = 1.0;
	if( length.y == 0.0 ) length.y = 1.0;
	if( length.z == 0.0 ) length.z = 1.0;

	xmin = centre - 0.5*length;
	xmax = centre + 0.5*length;

	return;
}

/*!

*/

void
ADTree::calc_node_mass( iterator it ) {

  // if leaf node then return.
  if( no_child(it) ) return;

  // then, this node should be internal node.
  it->p.mass = 0;
  Locus com = Locus(0,0,0);
  if( is_left_child( it ) ) {
      iterator cid = it.node->left_child;
      calc_node_mass( cid );
      it->p.mass += cid->p.mass;
      com += cid->p.mass * cid->p.x;
  }
  if( is_right_child( it ) ) {
	  iterator cid = it.node->right_child;
	  calc_node_mass( cid );
	  it->p.mass += cid->p.mass;
	  com += cid->p.mass * cid->p.x;
  }
  it->p.x = (1.0/it->p.mass) * com ;

  return;
}

template<typename iter>
iter
ADTree::set_child_at( iter it, int side, const ADnode& nd ) {

	switch( side ) {
		case 0:
			return set_left_child( it, nd );
			break;
		case 1:
			return set_right_child( it, nd );
			break;
	}
}

template<typename iter>
bool
ADTree::is_child_at( iter it, int side ) const {
	switch( side ) {
		case 0:
			return is_left_child( it );
			break;
		case 1:
			return is_right_child( it );
			break;
	}
}

void
ADTree::print_tree( iterator it ) const {
//	std::cout << it->p.id << ":" << it->p.x.x << ", " << it->p.x.y << ", " << it->p.x.z << std::endl;
	if( is_left_child(it)) {
//		std::cout << "  lef";
		print_tree( it.node->left_child );
	}
	if( is_right_child(it)) {
//		std::cout << "  rig";
		print_tree( it.node->right_child );
	}
}
