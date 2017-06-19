/*
 
 Class Name --- View3D
 
 Description - A class to show the three dimensional diagnosis of simulation
			Derived from wxGLCanvas.

 $Author: hiroshi $
 $Date: 2009/04/16 01:59:44 $
 $Revision: 1.24 $
 
 Copyrights 2007 All Rights Reserved by HABE, aka ABE Hiroshi, JAPAN.

*/

#include <iostream>

#include <wx/wx.h>
#include <wx/glcanvas.h>

#ifdef __DARWIN__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#ifndef WIN32
#include <GL/glut.h>
#endif
#endif

#include "View/View3D.h"
#include "Model/distributor.h"
#include "H/Hquat.h"

BEGIN_EVENT_TABLE(View3D, wxGLCanvas)
    EVT_SIZE(View3D::OnSize)
    EVT_PAINT(View3D::OnPaint)
    EVT_ERASE_BACKGROUND(View3D::OnEraseBackground)
	EVT_LEFT_UP(View3D::UpMouse)
    EVT_MOUSE_EVENTS(View3D::OnMouse)
END_EVENT_TABLE()

View3D::View3D(wxWindow *parent, wxWindowID id,
    const wxPoint& pos, const wxSize& size, long style, const wxString& name)
    : wxGLCanvas(parent, id, pos, size, style|wxFULL_REPAINT_ON_RESIZE, name)
{
    block = false;
	_viewAspect = (float)size.x / size.y;
	_simpleView = false;

	m_oldx = 0.0;
	m_oldy = 0.0;
	m_zoom = 10.0;
	
//	mgl_gl_lock = false;
	m_rcx = 0.0;
	m_rcy = 0.0;
	m_rcz = 0.0;
	
	m_eyex = 0.0;
	m_eyey = 0.0;
	m_eyez = 100.0;
	
	m_upx = 0.0;
	m_upy = 1.0;
	m_upz = 0.0;
	
	DoResize();
	InitGL();
}

View3D::~View3D()
{
}

void
View3D::ShowTime( const char* tm ) {

	glPushAttrib(GL_CURRENT_BIT);
	glPushMatrix();
	glLoadIdentity();

	glNewList( mgl_time, GL_COMPILE_AND_EXECUTE );
	glDisable(GL_LIGHTING);
	glCallLists(strlen(tm), GL_BYTE, tm);
	glEnable(GL_LIGHTING);
	glEndList();

	glPopMatrix();
	glPopAttrib();

	glFlush();

}

void 
View3D::PutObject( GLUquadricObj* obj, const Locus& loc, float size = 0.1 ) {
	glPushMatrix();
	glTranslatef( loc.x, loc.y, loc.z );
	gluSphere( obj, size, 10,10);
	glPopMatrix();
}

Colour
View3D::GenColourFromId( int id ) {
	Colour c;
	switch( id ) {
		case 0:
			c.r = 1.0;
			c.g = 1.0;
			c.b = 1.0;
			break;
		case 1:
			c.r = 0.0;
			c.g = 1.0;
			c.b = 0.0;
			break;
		case 2:
			c.r = 1.0;
			c.g = 0.0;
			c.b = 1.0;
			break;
		case 3:
			c.r = 1.0;
			c.g = 0.0;
			c.b = 0.0;
			break;
		default:
			c.r = 0.7;
			c.g = 0.6;
			c.b = 0.2;
			break;
	}
	return c;
}

GLuint 
View3D::_AxisDraw() {
	const float futosa = 0.1;
	const float length = 5;
	const float kasa = 0.3;
	
	GLuint _axis = ::glGenLists(1);
	
	glNewList( _axis, GL_COMPILE );
	glPushAttrib(GL_CURRENT_BIT);
	
	GLUquadricObj* head = gluNewQuadric();
	GLUquadricObj* arrw = gluNewQuadric();
	glPushMatrix();
	glColor3f(0.0, 0.0, 1.0);		// z-vector ->
	gluCylinder( arrw, futosa, futosa, 0.8*length, 12, 12 );
	glTranslatef( 0.0, 0.0, 0.8*length );
	gluCylinder( head, kasa, 0.0, 0.2*length, 12, 12 );
	glPopMatrix();
	gluDeleteQuadric( head );
	gluDeleteQuadric( arrw );
	
	head = gluNewQuadric();
	arrw = gluNewQuadric();
	glPushMatrix();
	glColor3f(1.0, 0.0, 0.0);		// x-vector ->Red
	glRotatef( 90.0, 0.0, 1.0, 0.0 );
	gluCylinder( arrw, futosa, futosa, 0.8*length, 12, 12 );
	glTranslatef( 0.0, 0.0, 0.8*length );
	gluCylinder( head, kasa, 0.0, 0.2*length, 12, 12 );
	glPopMatrix();
	gluDeleteQuadric( head );
	gluDeleteQuadric( arrw );
	
	head = gluNewQuadric();
	arrw = gluNewQuadric();
	glPushMatrix();
	glColor3f(0.0, 1.0, 0.0);		// y-vector ->
	glRotatef( -90.0, 1.0, 0.0, 0.0 );
	gluCylinder( arrw, futosa, futosa, 0.8*length, 12, 12 );
	glTranslatef( 0.0, 0.0, 0.8*length );
	gluCylinder( head, kasa, 0.0, 0.2*length, 12, 12 );
	glPopMatrix();
	gluDeleteQuadric( head );
	gluDeleteQuadric( arrw );
	
	glPopAttrib();
	glEndList();
	
	return _axis;
}

void
View3D::DeleteGLLists() {
//	glDeleteLists(mgl_time,1);
//	glDeleteLists(mgl_obj_list,1);
}

void
View3D::_ObjDraw() {

	SetCurrent();

	GLUquadricObj *sphere;
	sphere = gluNewQuadric();
	gluQuadricDrawStyle(sphere, GLU_FILL);

//	glNewList( mgl_obj_list, GL_COMPILE );
	glPushAttrib(GL_CURRENT_BIT);
	glMatrixMode( GL_MODELVIEW );

	int n = 0;
	for( int id = 0; id < m_wc->GetSizeOfId(); id++ ) {
		Colour c = GenColourFromId( id );
		for( int i = 0; i < m_wc->GetNumOfId(id); i++ ) {
			Locus loc;

			m_wc->GetLocation(n,loc);

			glPushAttrib(GL_CURRENT_BIT);
			glPushMatrix();
			glTranslatef( loc.x, loc.y, loc.z );
			glColor3f(c.r, c.g, c.b);
//			gluSphere( sphere, 10.0*m_wc->GetMass(n), 10,10 );
			gluSphere( sphere, 0.01*m_zoom, 10,10 );
			glPopMatrix();
			
			glPopAttrib();
			
			n++;
		}
	}

	glPopAttrib();
//	glEndList();

	gluDeleteQuadric( sphere );
}

/*! 
Generate GL object list of particles.
This routine should be called when the particles are moved.
*/
void
View3D::ReDraw( const WorldController& univ ) {

//	std::cout << "ReDraw is called" << std::endl;
//	while( mgl_gl_lock ) {}
//	mgl_gl_lock = true;
	
    SetCurrent();

	m_wc = &univ;

	_ObjDraw();

	_Render();
}

void
View3D::_Render() {
	SetCurrent();
	glMatrixMode( GL_MODELVIEW );
	
	glClearColor( 0.1f, 0.1f, 0.1f, 0.1f );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    // Transformations
    GLfloat m[4][4];
	// Set unit matrix to the current matrix.
    glLoadIdentity();
	// get the matrix from Trackball
    m_trackball.build_rotmatrix( m );
	// then multiply the matrix.
    glMultMatrixf( &m[0][0] );
	glTranslatef( m_rcx, m_rcy, m_rcz );

	glPushAttrib(GL_CURRENT_BIT);
	glPushMatrix();

	glCallList( mgl_obj_list );
//	glCallList( mgl_time );
	_ObjDraw();
	// if _simpleView flag is stood then draw the axes.
	if( _simpleView ) {
		glCallList(mgl_axes);
	}

	glPopMatrix();
	glPopAttrib();
	
	glFlush();
	SwapBuffers();
}

void View3D::OnPaint( wxPaintEvent& WXUNUSED(event) )
{
    /* must always be here */
    wxPaintDC dc(this);

#ifndef __WXMOTIF__
    if (!GetContext()) return;
#endif

	DoResize();

    SetCurrent();
//	std::cout << "OnPaint is called" << std::endl;

	_Render();
}


void View3D::OnSize(wxSizeEvent& event)
{
    // this is also necessary to update the context on some platforms
    wxGLCanvas::OnSize(event);
#ifndef __WXMOTIF__
    if ( GetContext() )
#endif
    {
		SetCurrent();
		DoResize();
	}
//	std::cout << "OnSize is called" << std::endl;
}

void View3D::DoResize() {
    // set GL viewport (not called by wxGLCanvas::OnSize on all platforms...)
    int w, h;
    GetClientSize(&w, &h);
//	std::cout << "DoResize is called" << std::endl;

	_viewAspect = (float)w/h;
	glViewport(0, 0, (GLint) w, (GLint) h);

	glMatrixMode(GL_PROJECTION); 
	glLoadIdentity();
	gluPerspective( m_zoom, _viewAspect, 1.0, 1000.0 );
	gluLookAt(m_eyex,m_eyey,m_eyez, 0,0,0, m_upx,m_upy,m_upz );
	glMatrixMode(GL_MODELVIEW);
}

void View3D::OnEraseBackground(wxEraseEvent& WXUNUSED(event))
{
    /* Do nothing, to avoid flashing on MSW */
}

void
View3D::UpMouse( wxMouseEvent& event ) { 
//	std::cout << "mouse up" << std::endl;
	
	_simpleView = false;
	
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glCallList( mgl_obj_list );
	glPushAttrib(GL_CURRENT_BIT);
	glCallList( mgl_time );
	glPopAttrib();

	glFlush();
	SwapBuffers();
	
}

void View3D::OnMouse( wxMouseEvent& event )
{
//	event.Skip(true);

	if( event.Dragging() ) {
		_simpleView = true;
		wxSize sz( GetClientSize() );

		if(event.m_shiftDown) {
//			std::cout << "mouse dragging with shift" << std::endl;
			wxSize sz( GetClientSize() );

			float y = event.GetY();
			m_zoom += 50*( (sz.y - 2.0*m_oldy) - (sz.y - 2.0*y) ) / sz.y;
		} else if(event.m_altDown) {
			float x = event.GetX();
			float y = event.GetY();
			GLfloat mx[16];
			GLfloat imx[9];
			glGetFloatv( GL_MODELVIEW_MATRIX, mx );
			// calc. the inverse rot. matrix.
			imx[0] = mx[0];
			imx[1] = mx[4];
			imx[2] = mx[8];
			imx[3] = mx[1];
			imx[4] = mx[5];
			imx[5] = mx[9];

			float rdx =  0.04*(x-m_oldx);
			float rdy = -0.04*(y-m_oldy);
			m_rcx += imx[0] * rdx + imx[3] * rdy;
			m_rcy += imx[1] * rdx + imx[4] * rdy;
			m_rcz += imx[2] * rdx + imx[5] * rdy;
			gluLookAt(m_eyex,m_eyey,m_eyez, 0,0,0, m_upx,m_upy,m_upz);
		} else {
//			std::cout << "mouse dragging" << std::endl;

			m_trackball.track(
							  ( 2.0*m_oldx - sz.x) / sz.x,
							  ( sz.y - 2.0*m_oldy) / sz.y,
							  ( 2.0*event.GetX() - sz.x) / sz.x,
							  ( sz.y - 2.0*event.GetY()) / sz.y );
			
		}

		/* orientation has changed, redraw mesh */
        Refresh(false);
    }

	m_oldx = event.GetX();
	m_oldy = event.GetY();

//	_idle_num = 0;
}

void View3D::InitGL()
{
//    static const GLfloat light0_pos[4]   = { -50.0, 50.0, 0.0, 0.0 };
    static const GLfloat light0_pos[4]   = { 0.0, 0.0, 10.0, 0.0 };

    // white light
    static const GLfloat light0_color[4] = { 0.6, 0.6, 0.6, 1.0 };

//    static const GLfloat light1_pos[4]   = {  50.0, 50.0, 0.0, 0.0 };
    static const GLfloat light1_pos[4]   = {  0.0, 0.0, 1.0, 0.0 };

    // cold blue light
    static const GLfloat light1_color[4] = { 0.4, 0.4, 1.0, 1.0 };

	SetCurrent();

    /* remove back faces */
    glDisable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    /* speedups */
    glEnable(GL_DITHER);
    glShadeModel(GL_SMOOTH);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_FASTEST);

    /* light */
    glLightfv(GL_LIGHT0, GL_POSITION, light0_pos);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light0_color);
//    glLightfv(GL_LIGHT1, GL_POSITION, light1_pos);
//    glLightfv(GL_LIGHT1, GL_DIFFUSE,  light1_color);
    glEnable(GL_LIGHT0);
//    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHTING);

    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);

	mgl_obj_list = ::glGenLists(1);

	mgl_axes = _AxisDraw();
	
/*
	glEnable(GL_FOG);
	glFogi(GL_FOG_MODE, GL_EXP);
	glFogf(GL_FOG_DENSITY, 0.01f);
	glFogf(GL_FOG_START, 0.0f );
	glFogf(GL_FOG_END, 1.0f );
	
*/	
	mgl_base = glGenLists(128);
#if defined(WIN32)
	HDC hdc = GetDC((HWND)this->GetHandle());
	wglUseFontBitmaps( hdc, 0, 128, mgl_base );
#else
	for(int i = 0; i < 128; i++){
		glNewList(mgl_base+i, GL_COMPILE);
		glColor3f(1.0,1.0,1.0);
 		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, i);
		/*    glutStrokeCharacter(GLUT_STROKE_ROMAN, i);*/
		glEndList();
	}
#endif
	glListBase(mgl_base);
	mgl_time = glGenLists(1);

}

wxImage*
View3D::GetGLBuffer() {
	GLint view[4];

//	while( mgl_gl_lock ) {
//		wxSleep(1);
//	}
//	mgl_gl_lock = true;
	
	SetCurrent();
	
	wxSize _sz = GetSize();
	GLint	width, height;
	
	width = _sz.x;
	height = _sz.y;
	glGetIntegerv(GL_VIEWPORT, view);
//	size_t sz = 3*sizeof(char)* view[2]*view[3];

	size_t sz = 3*sizeof(char)* width*height;
	void* pixels = (char *)::malloc( sz );
	
	glPixelStorei(GL_PACK_ALIGNMENT, 1); // set the alignment to 1 instead of 4 (default).
	
	glReadBuffer(GL_FRONT); // to get the image from FRONT buffer (one of Double Buffer).
						   //		glReadBuffer(GL_FRONT); // to get the image from FRONT buffer (one of Double Buffer).
	
	glReadPixels( 0,0, width, height, GL_RGB, GL_UNSIGNED_BYTE, pixels );

//	mgl_gl_lock = false;
	
	// put the image into a wxImage buffer.
	wxImage* img = new wxImage( (int)width, (int)height );
	img->SetData((unsigned char*)pixels);
	*img = img->Mirror(false);
	
	//		free(pixels); // the pixels data is now stored in wxImage and it keeps inside, don't free.
	
	return img;
}

