/*
 Class Name --- btree
 
 $Author: hiroshi $
 $Date: 2013/05/07 06:19:59 $
 $Revision: 1.11 $
 
 The class "btree" is a binary tree class.
 The node data is templated so that many of binary tree could be applicable.
 The order is implemented with iterator method. 
 
 Copyrights 2005-2007 All Rights Reserved by HABE, aka ABE Hiroshi, JAPAN.
	
 */
#ifndef _H_BTREE_H__
#define _H_BTREE_H__

#include <iostream>
#include <iterator>
#include <memory>
#include <string>
#include <stack>

#include <cassert>

// habe standard library

namespace H {
	
	/*!
	class for binary tree node
	 */
	template<class T>
	struct btree_node {
		btree_node<T>	*father; //!< pointer to the upper node linked from.
		btree_node<T>	*left_child,  //!< pointer to the left side child. possibly zero.
			*right_child; //!< pointer to the right side child. possibly zero.
		T		data; //! stores the data of the node.
	};
	
	
	/*!
	class for binary tree.
	 */
	template <class T, class btree_node_allocator = std::allocator<btree_node<T> > >
		class btree {
protected:
			typedef btree_node<T>	node_type;
public:
			class iterator_base;
			class pre_order_iterator;
			class post_order_iterator;
			
			btree();
			btree( const T& );
			btree( const iterator_base& );
			btree( const btree<T, btree_node_allocator>& s );
			virtual ~btree();
			void operator=( const btree<T, btree_node_allocator>& );
			
			
			/*!
				abstracted base class for iterator.
			 */
			class iterator_base {
public:
				typedef T                               value_type;
				typedef T*                              pointer;
				typedef T&                              reference;
				typedef size_t                          size_type;
				typedef std::ptrdiff_t                  difference_type;
				typedef std::bidirectional_iterator_tag iterator_category;
				
				iterator_base();
				iterator_base( node_type* );
				
				bool                no_child() const;
				bool                is_left_child() const;
				bool                is_right_child() const;
				
				T&			operator*() const;
				T*			operator->() const;
				
				node_type*		node;
protected:
					bool                skip_current_children_;
private:
			};
			
			/*!
				preorder iterator
			 */
			class pre_order_iterator : public iterator_base {
public:
				pre_order_iterator();
				pre_order_iterator( node_type * );
				pre_order_iterator( const iterator_base& );
				
				bool    operator==(const pre_order_iterator&) const;
				bool    operator!=(const pre_order_iterator&) const;
				pre_order_iterator&  operator++();
				pre_order_iterator&  operator--();
				pre_order_iterator   operator++(int);
				pre_order_iterator   operator--(int);
				pre_order_iterator&  operator+=(unsigned int);
				pre_order_iterator&  operator-=(unsigned int);
				pre_order_iterator	 next_sibling();
				
				using iterator_base::node;
			};
			
			/*!
				postorder iterator
			 */
			class post_order_iterator : public iterator_base {
public:
				post_order_iterator();
				post_order_iterator(node_type *);
				post_order_iterator(const iterator_base&);
				
				bool    operator==(const post_order_iterator&) const;
				bool    operator!=(const post_order_iterator&) const;
				
				post_order_iterator&  operator++();
				post_order_iterator&  operator--();
				post_order_iterator   operator++(int);
				post_order_iterator   operator--(int);
				post_order_iterator&  operator+=(unsigned int);
				post_order_iterator&  operator-=(unsigned int);
				
				void descend_all();
				
				using iterator_base::node;
			};
			
			/*!
				for convenience
			 */
			typedef pre_order_iterator iterator;
			
			// btree methods
			
public:
				// begin and end of iterator.
				pre_order_iterator	begin() const;
			pre_order_iterator	end() const;
			post_order_iterator	begin_post() const;
			post_order_iterator	end_post() const;
			
			// clear all the nodes in the btree.
			void                         clear(); 
			template<typename iter> iter erase(iter); 
			
			template<typename iter> iter set_left_child( iter pos, const T& x ); 
			template<typename iter> iter set_right_child( iter pos, const T& x ); 
			
			template<typename iter> bool no_child( iter pos ) const {
				return ( (pos.node->left_child == 0) && (pos.node->right_child == 0) );
			}
			template<typename iter> bool is_left_child( iter pos ) const { return pos.node->left_child != 0; }
			template<typename iter> bool is_right_child( iter pos ) const { return pos.node->right_child != 0; }
			
			// set the root node of the tree.
			virtual pre_order_iterator	       set_root( const T& top );
			
			//! replace node at 'position' with other node (keeping same
			//! children). 'position' becomes invalid.
			template<typename iter> iter replace(iter position, const T& x);
			
			/*!
			*/
			int						size() const;
			/*! @brief returns the depth of the node corresponding to x.
			 @param x specifies the iterator to be calculated.
			 @return the depth of the x. the top(begin) node returns 1.
			 
			*/
			int						level( iterator_base& x ) const; //! returns the depth to the root.
			bool					is_empty() const { return( begin() == end() ); }
protected:
private:
			btree_node_allocator	alloc_;
			node_type*				T_base_;
			node_type*				construct_node_( const T& nd = T() );
			void					destruct_node_( node_type* nd );
			void					base_init_();
			void					copy_( const btree<T, btree_node_allocator>& );
			void					rcopy_( btree_node<T>* id, btree_node<T>* is );
		};
	
	
	/*
	 member methods of btree class.
	 
	 */
	/*!
		btree default constructor with initializaion of base node by
	 base_init_ private member method.
	 */
	template <class T, class btree_node_allocator>
		btree<T, btree_node_allocator>::btree() {
//			std::cout << "btree default constructed: " << std::endl;		
			base_init_();
		}
	
	/*!
		btree constructor with root node which data is x.
	 
	 \param x stores node data which will be the root(top) node. the x
	 value is copied to the btree internal storage.
	 */
	template <class T, class btree_node_allocator>
		btree<T, btree_node_allocator>::btree( const T& x ) {
			base_init_();
			set_root(x);
		}
	
	/*!
		btree constructor with the node which is indicated by x.
	 
	 \param x indicates the iterator which is pointed the btree node to
	 be set to the root of the constructing btree instance.
	 */
	template <class T, class btree_node_allocator>
		btree<T, btree_node_allocator>::btree( const iterator_base& x ) {
			base_init_();
			set_root((*x));
			replace( begin(), x );
		}
	/*!
		btree constructor with the node which is indicated by x.
	 
	 \param x indicates the iterator which is pointed the btree node to
	 be set to the root of the constructing btree instance.
	 */
	template <class T, class btree_node_allocator>
		btree<T, btree_node_allocator>::btree( const btree<T, btree_node_allocator>& s ) {
//			std::cout << "btree copy constructed: " << std::endl;		
//			clear();
//			std::cout << "btree cleared: " << std::endl;		
			base_init_();
			copy_(s);
//			std::cout << "btree copied: " << std::endl;		
		}
	/*!
		destructor of btree class. 
	 
	 */
	template <class T, class btree_node_allocator>
		btree<T, btree_node_allocator>::~btree() {
//			std::cout << "btree destructed: " << std::endl;
			clear();
			destruct_node_( T_base_ );
		}
	
	/*!
		construct_node_ private member method constructs the btree node with
	 initial value da. the memory is allocated with the allocator and
	 initialize the alloced memory with the node type.
	 
	 \param da stores the btree node data to be set to the constructing
	 node data.
	 */
	template <class T, class btree_node_allocator>
		typename btree<T, btree_node_allocator>::node_type* btree<T, btree_node_allocator>::construct_node_( const T& da ) {
			node_type *newOne, nd;
			newOne = alloc_.allocate(1,0);
			alloc_.construct(newOne,nd);
			newOne->left_child = 0;
			newOne->right_child = 0;
			newOne->data = da;
			
			return newOne;
		}
	
	/*!
		dectruct_node_ destructs the node "nd" and free the memory area.
	 
	 \param nd 
	 \return 
	 */
	template <class T, class btree_node_allocator>
		void btree<T, btree_node_allocator>::destruct_node_( node_type* nd ) {
			nd->data.~T();
			alloc_.deallocate( nd, 1 );
			return;
		}
	
	/*!
		the base (dummy) node initialization.
	 the left child node is the root (top) node of the actual tree.
	 the father is pointed to itself. this is the only case in base node.
	 
	 \return no return.
	 */
	template <class T, class btree_node_allocator>
		void btree<T, btree_node_allocator>::base_init_() {
			T_base_ = construct_node_();
			T_base_->father = T_base_;
			T_base_->left_child = T_base_;
			T_base_->right_child = 0;
		}
	
	/*!
		copy all the node int the "x" to this instance. all the data is
	 stored to different newly allocated memory.
	 
	 */
	template <class T, class btree_node_allocator>
		void btree<T, btree_node_allocator>::copy_( const btree<T, btree_node_allocator>& x ) {
			clear();
			iterator it = x.begin();
			
			if( it == x.end() ) { // x is empty
				base_init_();
				return;
			}
			
			set_root( *it );
			rcopy_(T_base_->left_child, it.node );
		}
	/*!
		copy the child(ren) of the node specified.
		
	 */
	template <class T, class btree_node_allocator>
		void btree<T, btree_node_allocator>::rcopy_( btree_node<T>* id, btree_node<T>* is ) {
			if( is->left_child != 0 ) {
				id->left_child = construct_node_( is->left_child->data );
				id->left_child->father = id;
				rcopy_( id->left_child, is->left_child );
			}
			if( is->right_child != 0 ) {
				id->right_child = construct_node_( is->right_child->data );
				id->right_child->father = id;
				rcopy_( id->right_child, is->right_child );
			}
		}
	
	template <class T, class btree_node_allocator>
		void btree<T, btree_node_allocator>::operator=( const btree<T, btree_node_allocator>& x ) {
//			std::cout << "btree copy operator " << std::endl;
			copy_(x);
		}
	
	/*!
		set left child at the node indicated by position.    
	 
	 \param x a constant character pointer.
	 \return The test results
	 */
	template <class T, class btree_node_allocator>
		template<typename iter> iter btree<T, btree_node_allocator>::set_left_child( iter pos, const T& x ) {
			if( pos.node->left_child == 0 ) {
				pos.node->left_child = construct_node_(x);
				pos.node->left_child->father = pos.node;
			} else {
				destruct_node_( pos.node->left_child );
				pos.node->left_child = construct_node_(x);
				pos.node->left_child->father = pos.node;
			}
			return pos.node->left_child;
		}
	
	template <class T, class btree_node_allocator>
		template<typename iter> iter btree<T, btree_node_allocator>::set_right_child( iter pos, const T& x ) {
			if( pos.node->right_child == 0 ) {
				pos.node->right_child = construct_node_(x);
				pos.node->right_child->father = pos.node;
			} else {
				destruct_node_( pos.node->right_child );
				pos.node->right_child = construct_node_(x);
				pos.node->right_child->father = pos.node;
			}
			return pos.node->right_child;
		}
	
	/*!
		delete all the node in the btree instance.
	 
	 */
	template <class T, class btree_node_allocator>
		void btree<T, btree_node_allocator>::clear() {
			if( begin() == end() ) return; // totaly empty
			erase(begin());
			return;
		}
	
	/*!
		deletes the btree node at the iter n and its children.
	 returns the next position in the sense of "iter".
	 
	 \param it indicates the position to delete.
	 \return the next position in the "iter" order.
	 */
	template <class T, class btree_node_allocator>
		template<class iter> iter btree<T, btree_node_allocator>::erase(iter it) {
			if( it == 0 ) return end();
			node_type *prev=0, *cur = it.node;
			iter ret( it.node->father );
			bool is_left = false;
			if( it == ret.node->left_child ) is_left = true;
			
			std::stack<node_type*> stk;
			
			while( cur != 0 || stk.size() != 0 ) {
				prev = cur;
				if( cur->right_child != 0 ) stk.push(cur->right_child);
				if( cur->left_child != 0 ) {
					cur = cur->left_child;
					destruct_node_(prev);
				} else {
					if( stk.size() == 0 ) {
						destruct_node_(prev);
						cur=0;
					} else {
						cur = stk.top();
						stk.pop();
						destruct_node_(prev);
					}
				}
			}
			if( is_left ) {
				if( ret.node == T_base_ ) {
					ret.node->left_child = T_base_;
				} else {
					ret.node->left_child = 0;
				}
			}
			else          ret.node->right_child = 0;
			return ret++;
		}
	
	template <class T, class btree_node_allocator>
		int btree<T, btree_node_allocator>::size() const {
			int num = 0;
			for( iterator it = begin(); it != end(); it++ ) {
				num++;
			}
			return num;
		}
	
	template <class T, class btree_node_allocator>
		int btree<T, btree_node_allocator>::level( iterator_base& x ) const {
			pre_order_iterator it = x;
			int lev = 0;
			while( it.node->father != it.node ) {
				it = it.node->father;
				lev++;
			}
			return lev;
		}
	
	
	/*********************************************************************************
		member methods of iterator_base class.
		
		*********************************************************************************/
	
	template <class T, class btree_node_allocator>
		btree<T, btree_node_allocator>::iterator_base::iterator_base() 
		: node(0) {}
	
	template <class T, class btree_node_allocator>
		btree<T, btree_node_allocator>::iterator_base::iterator_base( node_type* tn ) 
		: node(tn) {}
	
	template <class T, class btree_node_allocator>
		T& btree<T, btree_node_allocator>::iterator_base::operator*() const
	{
		return node->data;
	}
	
	template <class T, class btree_node_allocator>
		T* btree<T, btree_node_allocator>::iterator_base::operator->() const
	{
		return &(node->data);
	}
	
	template <class T, class btree_node_allocator>
		bool btree<T, btree_node_allocator>::iterator_base::no_child() const
	{
		return ( !(is_left_child() || is_right_child()) );
	}
	
	template <class T, class btree_node_allocator>
		bool btree<T, btree_node_allocator>::iterator_base::is_left_child() const
	{
		return (node->left_child != 0);
	}
	
	template <class T, class btree_node_allocator>
		bool btree<T, btree_node_allocator>::iterator_base::is_right_child() const
	{
		return (node->right_child != 0);
	}
	
	
	/*********************************************************************************
		member methods of pre_order_iterator class.
		
		*********************************************************************************/
	
	template <class T, class btree_node_allocator>
		typename btree<T, btree_node_allocator>::pre_order_iterator btree<T, btree_node_allocator>::set_root( const T& top ) {
			node_type* the_child = construct_node_();
			the_child->data = top;
			the_child->father = T_base_;
			the_child->left_child = 0;
			the_child->right_child = 0;
			T_base_->left_child = the_child;
			
			return the_child;
		}
	
	template <class T, class btree_node_allocator>
		typename btree<T, btree_node_allocator>::pre_order_iterator btree<T, btree_node_allocator>::begin() const{
			return T_base_->left_child;
		}
	
	template <class T, class btree_node_allocator>
		typename btree<T, btree_node_allocator>::pre_order_iterator btree<T, btree_node_allocator>::end() const {
			return T_base_;
		}
	
	template <class T, class btree_node_allocator>
		btree<T, btree_node_allocator>::pre_order_iterator::pre_order_iterator() : iterator_base(0) {}
	
	template <class T, class btree_node_allocator>
		btree<T, btree_node_allocator>::pre_order_iterator::pre_order_iterator( node_type* tn ) : btree<T, btree_node_allocator>::iterator_base(tn) {}
	
	template <class T, class btree_node_allocator>
		btree<T, btree_node_allocator>::pre_order_iterator::pre_order_iterator( const iterator_base& it ) : btree<T, btree_node_allocator>::iterator_base(it) {}
	
	/*!
		increment the iterator in the preorder transaction through the
	 binary tree.
	 
	 \return preorder incremented iterator.
	 */
	template <class T, class btree_node_allocator>
		typename btree<T, btree_node_allocator>::pre_order_iterator& btree<T, btree_node_allocator>::pre_order_iterator::operator++() {
			if( node->left_child != 0 ) {
				node = node->left_child;
			} else if ( node->right_child != 0 ) {
				node = node->right_child;
			} else {
				// end of transaction.
				while( next_sibling() == 0 ) {
					node = node->father;
					if( node == node->father ) return *this; // node = T_base_ ( base node )
				}
			}
			return *this;
		}
	
	
	/*!
		increment the iterator in the preorder transaction through the
	 binary tree.
	 
	 \return preorder incremented iterator.
	 */
	template <class T, class btree_node_allocator>
		typename btree<T, btree_node_allocator>::pre_order_iterator btree<T, btree_node_allocator>::pre_order_iterator::operator++( int n ) {
			pre_order_iterator copy = *this;
			++(*this);
			return copy;
		}
	
	template <class T, class btree_node_allocator>
		bool btree<T, btree_node_allocator>::pre_order_iterator::operator==( const pre_order_iterator& d ) const {
			if( d.node == this->node ) return true;
			else return false;
		}
	
	template <class T, class btree_node_allocator>
		bool btree<T, btree_node_allocator>::pre_order_iterator::operator!=( const pre_order_iterator& d ) const {
			if( d.node != this->node ) return true;
			else return false;
		}
	
	
	template <class T, class btree_node_allocator>
		typename btree<T, btree_node_allocator>::pre_order_iterator
		btree<T, btree_node_allocator>::pre_order_iterator::next_sibling() {
			assert( node != 0 );
			node_type* _father = node->father;
			
			// when the node is the left child
			if(_father->left_child == node) {
				if( _father->right_child == 0 ) {
					return 0;
				} else {
					node = _father->right_child;
					return *this;
				}
				// when the node is the right child no more shibling.
			} else {
				return 0;
			}
		}
	
}

#endif
