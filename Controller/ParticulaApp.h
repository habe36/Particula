/*

 Class Name --- MyApp
 
 Description - A dummy class for application framework.

 $Author: hiroshi $
 $Date: 2007/03/19 13:17:23 $
 $Revision: 1.2 $
 
 Copyrights 2006,2007 All Rights Reserved by HABE, aka ABE Hiroshi, JAPAN.

*/

#ifndef __ParticulaApp__
#define __ParticulaApp__

// Define WX_GCH in order to support precompiled headers with GCC compiler.
// You have to create the header "wx_pch.h" and include all files needed
// for compile your gui inside it.
// Then, compile it and place the file "wx_pch.h.gch" into the same
// directory that "wx_pch.h".

#ifdef WX_GCH
#include <wx_pch.h>
#else
#include <wx/wx.h>
#endif

#define ID_DEFAULT wxID_ANY // Default
#define MENU_NEWFILE 1000

#endif 
