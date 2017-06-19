/*
 Class Name --- ADTree
 
 Description - An alternate digital tree which contains the distribution
		of particle location and its mass. The tree is expected less 
		computation than OctTree.

 $Author: habe $
 $Date: 2017/06/19 07:19:53 $
 $Revision: 1.3 $
 
 Copyrights 2007 All Rights Reserved by HABE, aka ABE Hiroshi, JAPAN.

 Ref. Bonet J. and Peraire J., 1991, IJNME, 1, 31.

*/

#ifndef __ALTERNATEDIGITALTREE_H___
#define __ALTERNATEDIGITALTREE_H___

#include <H/Hbtree.h>
#include <valarray>
#include <vector>
#include <iostream>
#include <cmath>

#include <cassert>

#include "datatypes.h"

/*!
  node of the ADTree.
  
  \param centre specifies the centre of the cell when the node is the leaf node.
  if not, specifies the centre of mass.
  \param length specifies the length of the cell of each direction.
 */
struct ADnode {
	property	p;
	int			dir; // 0: x, 1: y, 2: z.
	Locus		min,max;
	int			side; // which child this node is. left:0 or right:1.

// methods
	int			which_side( const Locus& x ) const; // return 0: left, 1:right.
	bool		is_internal() const;
	void		set_internal();
};

class ADTree : public H::btree<ADnode>  {
public:
	ADTree() : H::btree<ADnode>() {}
	virtual		~ADTree() {}

	ADnode		new_child( int half );
	iterator	insert( iterator it, ADnode& nd );
	void		calc_node_mass( iterator it );
	void		gen_child_at( iterator it, int side, ADnode& newOne, const property prop );
	void		set_bounding_box( Locus& xmin, Locus& xmax, const vectorType& x, const vectorType& y, const vectorType& z );
	Locus		GetBoundingMin() const { return m_min; }
	Locus		GetBoundingMax() const { return m_max; }

	template<typename iter>		iter	get_child_at( iter it, int side );
	template<typename iter>		iter	set_child_at( iter it, int side, const ADnode& nd );
	template<typename iter>		bool	is_child_at( iter it, int side ) const;
	void	print_tree( iterator it ) const;	
	
private:
	Locus		m_min, m_max;
};

template <typename iter>
iter
ADTree::get_child_at( iter it, int side ) {
    iter its;
    switch( side ) {
        case 0:
            assert( is_left_child(it) );
            its = it.node->left_child;
            break;
        case 1:
            assert( is_right_child(it) );
            its = it.node->right_child;
            break;
    }
    return its;
}

#endif
