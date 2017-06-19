/*
 Class Name --- View3D
 
 Description - A class to show the three dimensional diagnosis of simulation
			Derived from wxGLCanvas.

 $Author: hiroshi $
 $Date: 2009/04/16 01:59:44 $
 $Revision: 1.14 $
 
 Copyrights 2007 All Rights Reserved by HABE, aka ABE Hiroshi, JAPAN.

*/

#ifndef __H_View3DWIN_H__
#define __H_View3DWIN_H__

#if defined(__MACH__)
#include <OpenGL/glu.h>
#else
#include <GL/glu.h>
#endif

#include <wx/wx.h>
#include <wx/image.h>
#include <wx/glcanvas.h>

#include "Controller/world_controller.h"
#include "H/HTrackBall.h"

struct Colour {
	GLfloat		r,g,b;
};

class View3D : public wxGLCanvas {
public:
    View3D(wxWindow *parent, wxWindowID id = wxID_ANY,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize, long style = 0,
        const wxString& name = wxT("View3D"));

    ~View3D();
	void	SetWorldController( const WorldController& wc ) { m_wc = &wc; }
	GLuint GetListID() { return mgl_obj_list; }
	void DeleteGLLists();
	void ShowTime( const char* tm );
	void ReDraw(const WorldController& univ);
    void OnPaint(wxPaintEvent& event);
    void OnSize(wxSizeEvent& event);
    void OnEraseBackground(wxEraseEvent& event);
 	void PutObject( GLUquadricObj* obj, const Locus& loc, float size );
    void OnMouse( wxMouseEvent& event );
	void UpMouse( wxMouseEvent& event );
    void OnIdle(wxIdleEvent& event);
    void InitGL();
	
	void DoResize();

    bool       block;

// for Movie
	wxImage*	GetGLBuffer();

// for debug
//	int			_idle_num;
protected:
	void		_SimpleDraw(const WorldController& univ);	
	void		_FullDraw(const WorldController& univ);
	void		_Render();
	void		_ObjDraw();
	GLuint		_AxisDraw();
	Colour		GenColourFromId( int id );
private:
	DECLARE_EVENT_TABLE()
	
	H::TrackBall	m_trackball;
	float		m_zoom;
	float		m_oldx, m_oldy; /* previous mouse point */
	GLfloat		m_eyex, m_eyey, m_eyez, m_upx, m_upy, m_upz;
	GLfloat		m_rcx, m_rcy, m_rcz; /* View Rotation Centre Coord. */

	GLuint		mgl_base, mgl_time;
	GLuint		mgl_obj_list;
	GLuint		mgl_axes;
	
	float					_viewAspect;
	bool					_simpleView;
//	bool					mgl_gl_lock;
	WorldController const*	m_wc;
};

#endif