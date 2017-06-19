/*

 Class Name --- MyApp
 
 Description - A dummy class for application framework.

 $Author: hiroshi $
 $Date: 2007/03/19 13:17:23 $
 $Revision: 1.2 $
 
 Copyrights 2006,2007 All Rights Reserved by HABE, aka ABE Hiroshi, JAPAN.

*/

#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif //WX_PRECOMP

#include "ParticulaApp.h"
#include "Controller/base_controller.h"

class MyApp : public wxApp {
public:
  bool OnInit() {
	  // Create the main frame window
    BaseController *frame = new BaseController(NULL);

    frame->Show(true);

    return true;
  }
};

IMPLEMENT_APP(MyApp)

