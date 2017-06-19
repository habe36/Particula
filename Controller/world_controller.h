/*
 Class Name --- WorldController
 
 Description - Controller class to control World, Numerical Method and Threads.

 $Author: habe $
 $Date: 2017/06/19 07:19:53 $
 $Revision: 1.14 $
 
 Copyrights 2006,2007 All Rights Reserved by HABE, aka ABE Hiroshi, JAPAN.

*/

#ifndef __H_WORLD_CONTROLLER_H___
#define __H_WORLD_CONTROLLER_H___

#include <string>
#include <vector>

//#include <wx/wx.h>

#include "Model/world.h"
#include "Model/ppforce.h"
#include "Model/datatypes.h"
#include "Model/force.h"
#include "Model/diagnostics.h"
#include "Model/barrier.h"

typedef	std::vector<std::string>	strArray;

class WorldController {
public:
	WorldController();
	~WorldController();

// simulation control interfaces.
	void			Start();
	void			Stop();
	void			SetNumThread( int n ) { m_nthread = n; }
	int				GetNumThread() const { return m_nthread; }
	bool			IsRunning() const { return m_marching; }
	void			SetSaveFolder( const std::string fldr ) { m_save_folder = fldr; }
	std::string		GetSaveFolder() const { return m_save_folder; }

	bool			IsWaitDrawn() const { return m_wait_drawn; }
	void			ReleaseWaitDrawn() { m_wait_drawn = false; }
	bool			IsDrawn() const;
	void			SetDrawn( bool flg );
	int				GetNumOfId( int id ) const;
	int				GetSizeOfId() const { return m_id_list.size(); }
	void			PushId( int num ) {	m_id_list.push_back(num); }
	void			ResizeId( int size ) { m_id_list.resize(size); }

	void			PreForce( int begin, int end, int tid );
	void			PostForce( int begin, int end, int tid );
	void			CalcForce( int begin, int end, int tid );
	void			Advance( int begin, int end );

// physical data interfaces.
	void			SetNumParticles( int n );
	int				GetNumParticles() const;

	void			LoadNEMO( const char* path );
	void			SaveNEMO( const char* path );

	void			SetTemperature( scalarType temp );
	scalarType		GetTemperature() const;
	void			SetLocation( int n, const Locus& loc );
	void			GetLocation( int n, Locus& loc ) const;
	void			SetVelocity( int n, const Locus& vel );
	void			GetVelocity( int n, scalarType& vx, scalarType& vy, scalarType& vz ) const;
	scalarType		GetMass( int nth ) const { return m_mass[nth]; }
	void			SetMass( int nth, scalarType m ) { m_mass[nth] = m; }

	void			DefaultInitCondition();

// numerical method interfaces.
	void			SetTotalTime( scalarType t ) { m_total_time = t; }
	scalarType		GetTotalTime() const { return m_total_time; }
	void			SetTime( scalarType t ) { m_current_time = m_init_time = t;	}	
	scalarType		GetTime() const { return m_current_time; }
	void			SetDeltaT( scalarType dt ) { m_dt = dt; }
	scalarType		GetDeltaT() const { return m_dt; }
	void			SetCalcMethod( int method );
	strArray		GetCalcMethods( int& n ) const;
	void			SetSampleInterval( int smp ) { m_sample_interval = smp; }
	bool			IsSaveMovie() const { return m_save_movie; }
	void			EnableGLSaveMovie( bool flg ) {	m_save_movie = flg;	}

	void			SetTrackParticle( int n ) { m_num_track = n; }
	void			ZeroAccel() {
		// for initialize.
		for( int i = 0; i < m_world.GetSize(); i++ ) {
			m_ax[i] = 0.0;
			m_ay[i] = 0.0;
			m_az[i] = 0.0;
		}
	}
	void			CondSignal();

private:
	// Functions for pthread.
	static void*	_ImgWorld( void *that );
	static void*	_RunWorld( void *that );
	static void*	_Advance( void *that );
	static void*	_CalcForce( void *that );

private:
	int					dum;

	World				m_world;
	std::vector<HPPForce*>
						m_method_P;
	vectorType			m_ax,m_ay,m_az;
	vectorTypeR			m_x, m_y, m_z;
	vectorTypeR			m_vx, m_vy, m_vz;
	vectorTypeR			m_mass;
	scalarType			m_current_time;
	scalarType			m_total_time, m_init_time, m_dt;
	scalarType			m_temperature;
	std::vector<int>	m_id_list;							//!< list of the num. of stars of a bunch of galaxy
	bool				m_plot_data;								// true if plot data is registered.
	int					m_num_track;
	Plotter				m_plotter;
	
	bool				m_marching, m_adv_flag, m_calc_flag;		// flag to marching simulation. the treads look at this.
	int					m_sample_interval;							// plot or draw data in every m_sample_interval iteration.
	bool				m_save_movie;								// flag to save OpenGL buffer screen.
	std::string			m_save_folder;								// path of folder to save the screen data in.
	int					m_nthread;									// number of threads for calc/marching.

	bool				m_wait_drawn;
	bool				m_drawn;									// a flag the 3D view is drawn or not.	
	bool				m_lock_sim;
// for thread

	
	barrier_t			mt_bid1, mt_bid2;
	pthread_t			mt_tid, mt_wid;
	pthread_mutex_t		mt_mutex;
	pthread_cond_t		mt_req_image, mt_done_image;
	std::vector<pthread_t>
						mt_tvec1, mt_tvec2;

// Wrapper structure for passing data to each thread.
	struct  _ArgType {
		int					tid;
		WorldController*	thp;
	};
};

#endif

