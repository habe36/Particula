/*
 
 Class Name --- HPPForce
 
 Description - An abstructed class for Particle Particle Force calculation.

 $Author: hiroshi $
 $Date: 2007/03/19 13:17:20 $
 $Revision: 1.3 $
 
 Copyrights 2006,2007 All Rights Reserved by HABE, aka ABE Hiroshi, JAPAN.

*/

#include "force.h"
#include "ppforce.h"

void
HPPForce::GetMethods( std::vector<std::string>& methods ) {
	methods.push_back("Direct");
	methods.push_back("OctTree");
	methods.push_back("ADTree");
	
	return;
}

HPPForce*
HPPForce::CalcMethod( int method ) {

	HPPForce*	_method_P;
	
	switch( method ) {
		case 0:
			_method_P = new HPPDirectForce();
			break;
		case 1:
			_method_P = new HPPOTreeForce();
			break;
		case 2:
			//_method_P = new HPPOTreeForce();
			_method_P = new HPPADTreeForce();
			break;
		default:
			assert( method < 3 );
			break;
	}
	return _method_P;
}
