/*

 Class Name --- WorldController

 Description - A controller class for World class.

 $Author: habe $
 $Date: 2017/06/19 07:19:53 $
 $Revision: 1.23 $
 
 Copyrights 2006,2007 All Rights Reserved by HABE, aka ABE Hiroshi, JAPAN.

*/

#include <wx/wx.h>

#include <vector>

#include "Model/datatypes.h"
#include "Model/world.h"
#include "Model/distributor.h"
#include "Model/ppforce.h"
#include "Controller/world_controller.h"
#include "View/View3d.h"
#include "View/View2d.h"

WorldController::WorldController() :
	m_x( m_world.m_x), m_y( m_world.m_y), m_z( m_world.m_z),
	m_vx( m_world.m_vx), m_vy( m_world.m_vy), m_vz( m_world.m_vz),
	m_mass( m_world.m_mass),
	m_init_time(0.0),
	m_marching(false),
	m_adv_flag(false),
	m_calc_flag(false),
	m_plot_data(false),
	m_sample_interval(30),
	m_save_movie(false),
	m_wait_drawn(false),
	m_drawn(false),
	m_lock_sim(false)
{
	// temporary implemented
	m_plotter.RegisterSeries3D( "test" );

	m_plotter.RegisterSeries( "Kinetic Energy" );
	m_plotter.RegisterSeries( "Potential Energy" );	
	
	m_save_folder = ""; 
}

WorldController::~WorldController() {}

void
WorldController::Start() {
	m_marching = true;
	m_adv_flag = true;
	m_calc_flag = true;
	
	m_current_time = m_init_time;
	
	mt_tvec1.resize( m_nthread );
	mt_tvec2.resize( m_nthread );
	
	// mutex thread for image-saving.
	pthread_mutex_init( &mt_mutex, NULL );
	// barrier for CalcForce
	mt_bid1 = barrier_init( m_nthread+1 );
	// barrier for Advance
	mt_bid2 = barrier_init( m_nthread+1 );

	// cond thread for image-saving.
	pthread_cond_init( &mt_req_image, NULL);
	pthread_cond_init( &mt_done_image, NULL);
	
	// Simulation control main thread.
	pthread_create( &mt_tid, NULL, _RunWorld, (void *)this );
	// image-save control thread.
	pthread_create( &mt_wid, NULL, _ImgWorld, (void *)this );
/*
	for( int i = 0; i < m_vx.size(); i++ ) {
		std::cout << "Mass = " << m_mass[i] << " : ";
		std::cout << "(x,y,z) = (" << m_x[i] << ", " << m_y[i] << ", " << m_z[i] << ") ";
		std::cout << "(Vx,Vy,Vz) = (" << m_vx[i] << ", " << m_vy[i] << ", " << m_vz[i] << ") at " << i << std::endl;
	}
*/
}

void
WorldController::Stop() {
	if( !m_marching ) return;
	
	// trigger for end of the threads.
	m_marching = false;

	//
	m_drawn = true;
	m_init_time = m_current_time;

	// procedures for thread ending.f
	pthread_join( mt_tid, NULL );
	barrier_destroy( mt_bid1 );
	barrier_destroy( mt_bid2 );
	pthread_mutex_destroy(&mt_mutex);
	
}

void
WorldController::SetTemperature( scalarType temp ) {
	m_temperature = temp;
}

scalarType
WorldController::GetTemperature() const {
	return m_temperature;
}

void
WorldController::SetVelocity( int n, const Locus& vel ) {
	m_vx[n] = vel.x;
	m_vy[n] = vel.y;
	m_vz[n] = vel.z;
}

void
WorldController::SetLocation( int n, const Locus& loc ) {
	m_x[n] = loc.x;
	m_y[n] = loc.y;
	m_z[n] = loc.z;
}

void
WorldController::GetLocation( int n, Locus& loc ) const {
	assert( n < m_x.size() );
	loc.x = m_x[n];
	loc.y = m_y[n];
	loc.z = m_z[n];
}

void
WorldController::DefaultInitCondition() {
	int num = m_world.GetSize();
	vectorType x,y,z,m;
	x.resize( num );
	y.resize( num );	
	z.resize( num );	
	m.resize( num );	

	for( int i = 0; i < num; i++ ) {
		homo_sphere( x[i], y[i], z[i] );
		m[i] = 1.0/num;

		SetLocation( i, Locus(x[i],y[i],z[i]) );
		SetMass( i, m[i] );
	}

	double we = PotentialEnergy( num, x, y, z, m );
	scalarType _gamma = GetTemperature();
	double sigma = sqrt( 2*_gamma*abs(we)/3.0);

	for( int i = 0; i < num; i++ ) {
		scalarType vx,vy,vz;

		vx = gaussian() * sigma;
		vy = gaussian() * sigma;
		vz = gaussian() * sigma;

		m_world.SetVelocity( i, vx,vy,vz );
	}

	FixMassCentre( m_mass, m_vx, m_vy, m_vz );

	SetTime( 0.0 );
}

void
WorldController::SetNumParticles( int n ) {
	m_world.SetSize( n );
	m_ax.resize(n);
	m_ay.resize(n);
	m_az.resize(n);
}

int
WorldController::GetNumParticles() const {
	return m_world.GetSize();
}

void
WorldController::LoadNEMO( const char* path ) {
	FILE *fp;
	int n,dim;
	double t, dum,x,y,z;

	fp = fopen(path,"r");
	fscanf(fp,"%d%d%lf", &n, &dim, &t );
	SetNumParticles( n );
	
	SetTime( t );

	for( int i = 0; i < n; i++ ) {
		fscanf(fp,"%lf",&dum);
		m_world.SetMass( i, dum );
	}
	for( int i = 0; i < n; i++ ) {
		fscanf(fp,"%lf%lf%lf",&x,&y,&z);
		m_world.SetLocation( i, Locus(x,y,z) );
	}
	for( int i = 0; i < n; i++ ) {
		fscanf(fp,"%lf%lf%lf",&x,&y,&z);
		m_world.SetVelocity( i, x,y,z );
	}
	fclose(fp);
}

void
WorldController::SaveNEMO( const char* path ) {
	FILE *fp;
	int n,dim;
	scalarType t, dum,x,y,z;

	n = m_world.GetSize();
	t = GetTime();
	dim = 3;

	fp = fopen(path,"w");
	fprintf(fp,"%d\n%d\n%lf\n", n, dim, t );
	
	for( int i = 0; i < n; i++ ) {
		dum = m_world.GetMass(i);
		fprintf(fp,"%lf\n",dum);
	}
	for( int i = 0; i < n; i++ ) {
		Locus _loc;
		m_world.GetLocation( i, _loc );
		x = _loc.x;
		y = _loc.y;
		z = _loc.z;
		fprintf(fp,"%lf %lf %lf\n",x,y,z);
	}
	for( int i = 0; i < n; i++ ) {
		m_world.GetVelocity( i, x, y, z );
		fprintf(fp,"%lf %lf %lf\n",x,y,z);
	}
	fclose(fp);
}

void
WorldController::SetCalcMethod( int method ) {
	if( m_method_P.size() != 0 ) m_method_P.erase( m_method_P.begin(), m_method_P.end() );
	for( int i = 0; i < m_nthread; i++ ) {
		m_method_P.push_back( HPPForce::CalcMethod( method ) );
	}
}

strArray
WorldController::GetCalcMethods( int& n ) const {

	std::vector<std::string> methods;

	HPPForce::GetMethods( methods );
	n = methods.size();

	return methods;
}

bool
WorldController::IsDrawn() const {
	return m_drawn;
}

void
WorldController::SetDrawn( bool flg ) {
	m_drawn = flg;
}

int
WorldController::GetNumOfId(int id) const {
	assert( id < m_id_list.size() );
	return m_id_list[id];
}

void
WorldController::PreForce( int begin, int end, int tid ) {
#ifdef _H_ENABLE_EIGEN_
    m_method_P[tid]->PreForce( m_x, m_y, m_z, m_mass );
#else
	vectorType __x(m_x, begin, end );
	vectorType __y(m_y, begin, end );
	vectorType __z(m_z, begin, end );
	vectorType __m(m_mass, begin, end );

	m_method_P[tid]->PreForce( __x, __y, __z, __m );
#endif
	return;
}

void
WorldController::PostForce( int begin, int end, int tid ) {
#ifdef _H_ENABLE_EIGEN_
    m_method_P[tid]->PostForce( m_x, m_y, m_z, m_mass );
#else
	vectorType __x(m_x, begin, end );
	vectorType __y(m_y, begin, end );
	vectorType __z(m_z, begin, end );
	vectorType __m(m_mass, begin, end );
	m_method_P[tid]->PostForce( __x, __y, __z, __m );
#endif

	return;
}

void
WorldController::CalcForce( int begin, int end, int tid ) {
#ifdef _H_ENABLE_EIGEN_
    m_method_P[tid]->CalcForce( m_x, m_y, m_z, m_mass, m_ax, m_ay, m_az );
#else
	vectorType __ax(m_ax, begin, end );
	vectorType __ay(m_ay, begin, end );
	vectorType __az(m_az, begin, end );
	vectorType __x(m_x, begin, end );
	vectorType __y(m_y, begin, end );
	vectorType __z(m_z, begin, end );
	vectorType __m(m_mass, begin, end );
	
//	HPPAddForce calc;

//	_calc->PreForce(  __x, __y, __z, __m );
	
	m_method_P[tid]->CalcForce( __x, __y, __z, __m, __ax, __ay, __az );
	
//	_calc->PostForce( __x, __y, __z, __m );

#ifdef _HDEBUG
	for( int i = begin; i <= end; i++ ) {
		std::cout << i << ", " << __ax[i] << ", " << __ay[i] << ", " << __az[i] << std::endl;
	}
#endif
#endif

	return;
}

void
WorldController::Advance( int begin, int end ) {
	scalarType	_dt = GetDeltaT();

#ifdef _H_ENABLE_EIGEN_
    m_vx = m_vx + m_ax * _dt;
    m_vy = m_vy + m_ay * _dt;
    m_vz = m_vz + m_az * _dt;
    
    m_x = m_x + m_vx * _dt;
    m_y = m_y + m_vy * _dt;
    m_z = m_z + m_vz * _dt;
#else
	vectorType __vx(m_vx, begin, end );
	vectorType __vy(m_vy, begin, end );
	vectorType __vz(m_vz, begin, end );
	vectorType __ax(m_ax, begin, end );
	vectorType __ay(m_ay, begin, end );
	vectorType __az(m_az, begin, end );
	vectorType __x(m_x, begin, end );
	vectorType __y(m_y, begin, end );
	vectorType __z(m_z, begin, end );

	__vx = __vx + __ax * _dt;
	__vy = __vy + __ay * _dt;
	__vz = __vz + __az * _dt;
	
	__x = __x + __vx * _dt;
	__y = __y + __vy * _dt;
	__z = __z + __vz * _dt;
#endif

	return;
}

void
WorldController::CondSignal() {

	pthread_cond_signal(&mt_done_image);

}

// static functions

void *
WorldController::_ImgWorld( void *that ) {
	WorldController *fm = static_cast<WorldController *>(that);
	
	pthread_mutex_lock(&fm->mt_mutex);
	for(;;) {
		pthread_cond_wait( &fm->mt_req_image, &fm->mt_mutex );
		fm->m_drawn = false;
	}
	pthread_mutex_unlock(&fm->mt_mutex);
}

void *
WorldController::_RunWorld( void *that ) {
	
	static int num_plot = 0;
	
	WorldController *fm = static_cast<WorldController *>(that);
	_ArgType	*_args1, *_args2;
	_args1 = new _ArgType[fm->m_nthread];
	_args2 = new _ArgType[fm->m_nthread];

	for( int i = 0; i < fm->m_nthread; i++ ) {
		_args1[i].tid = i;  _args1[i].thp = fm;
		_args2[i].tid = i;  _args2[i].thp = fm;
		pthread_create( &(fm->mt_tvec1[i]), NULL, _CalcForce, (void *)&(_args1[i]) );
		pthread_create( &(fm->mt_tvec2[i]), NULL, _Advance, (void *)&(_args2[i]) );
	}

	int num = fm->m_world.GetSize();

	while(fm->m_marching) {

//		std::cout << fm->m_current_time << " : marching " << std::endl;
		
		fm->ZeroAccel();
		
		barrier_wait( fm->mt_bid1, 0);
		// _CalcForce is working.
		barrier_wait( fm->mt_bid1, 0);

		fm->m_plot_data = true;
		if( num_plot >= fm->m_sample_interval ) {
//			std::cout << fm->m_current_time << " : interval checked " << std::endl;
			if( fm->m_plot_data ) {
				int nn = fm->m_num_track;
				fm->m_plotter.Add( 0, fm->m_current_time, Locus(fm->m_x[nn], fm->m_y[nn], fm->m_z[nn]) );
			
				float ke = KineticEnergy( num, fm->m_vx, fm->m_vy, fm->m_vz, fm->m_mass);
				float pe = PotentialEnergy( num, fm->m_vx, fm->m_vy, fm->m_vz, fm->m_mass);
				fm->m_plotter.Add( 0, fm->m_current_time, ke );
				fm->m_plotter.Add( 1, fm->m_current_time, pe );
				// mathplot needs both data simultaneously to plot but there is no lock mechanism
				// so the Add function to mathplot must be as close as possible.
				// when plot data is activated between the Add functions, the latter's data is undefined
				// and the value is uncertain.
			}

			if( fm->m_save_movie ) {
				pthread_mutex_lock(&fm->mt_mutex);
				pthread_cond_signal(&fm->mt_req_image);
				
				pthread_cond_wait(&fm->mt_done_image, &fm->mt_mutex);
				pthread_mutex_unlock(&fm->mt_mutex);

			}
			
			
			// fix the accumrated error by forcing the mass centre stif.
			FixMassCentre( fm->m_mass, fm->m_vx, fm->m_vy, fm->m_vz );

			num_plot = 0;
		}
		num_plot++;
		
//		std::cout << "Barriering" << std::endl;

		barrier_wait( fm->mt_bid2, 0 );
		// _Advance is working.
		barrier_wait( fm->mt_bid2, 0 );
		
		fm->m_current_time += fm->m_dt;
		
		if( fm->m_current_time >= fm->m_total_time ) {
			fm->m_marching = false;
			fm->m_init_time = fm->m_current_time;
			fm->Stop();
		}
	}
	
	barrier_wait( fm->mt_bid1, 0);
	fm->m_calc_flag = false;
	barrier_wait( fm->mt_bid1, 0);
	
	barrier_wait( fm->mt_bid2, 0 );
	fm->m_adv_flag = false;
	barrier_wait( fm->mt_bid2, 0 );
	// post process for threads.
	for( int i = 0; i < fm->m_nthread; i ++ ) {
		pthread_join( fm->mt_tvec1[i], NULL );
		pthread_join( fm->mt_tvec2[i], NULL );
	}

	delete [] _args1;
	delete [] _args2;
	
	pthread_exit(NULL);

	return 0;
}

void *
WorldController::_Advance( void *that ) 
{
//	event.RequestMore( true );
	
	_ArgType *ag = static_cast<_ArgType *>(that);
	WorldController *fm = ag->thp;

	int n = fm->m_world.GetSize();
	int tid = ag->tid;
	
	int begin = n * tid / fm->m_nthread;
	int end = n * (tid+1) / fm->m_nthread - 1;
	
	std::cout << "Advance : " << tid << " from " << begin << " to " << end << std::endl;

	while(fm->m_adv_flag) {

		barrier_wait( fm->mt_bid2, 0);

		fm->Advance(begin,end);
		
		barrier_wait( fm->mt_bid2, 0);

	}

	pthread_exit(NULL);

	return 0;
}

void *
WorldController::_CalcForce( void *that ) {

	_ArgType *ag = static_cast<_ArgType *>(that);
	WorldController *fm = ag->thp;
	
	int n = fm->m_world.GetSize();
	int tid = ag->tid;
	
	int begin = n * tid / fm->m_nthread;
	int end = n * (tid+1) / fm->m_nthread - 1;

	while(fm->m_calc_flag) {

		barrier_wait( fm->mt_bid1, 0 );
		
		fm->PreForce(  begin, end, tid);
		fm->CalcForce( begin, end, tid);
		fm->PostForce( begin, end, tid);
		
		barrier_wait( fm->mt_bid1, 0 );

	}
	pthread_exit(NULL);

	return 0;
}
