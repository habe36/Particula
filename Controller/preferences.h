/*

 Class Name --- Preference
 
 Description - Application Preference Dialog.

 $Author: hiroshi $
 $Date: 2008/01/15 06:47:51 $
 $Revision: 1.3 $
 
 Copyrights 2007-2008 All Rights Reserved by HABE, aka ABE Hiroshi, JAPAN.

*/

#ifndef __H_PREFERENCE_H__
#define __H_PREFERENCE_H__

#include <iostream>

#include <wx/wx.h>
#include <wx/dialog.h>
#include <wx/sizer.h>
#include <wx/spinctrl.h>
#include <wx/config.h>
#include <wx/statline.h>
#include <wx/valgen.h>
enum {
	ID_OK_BTN = 299,
	ID_CANCEL_BTN,
	ID_INTERVAL
};

class PrefDialog : public wxDialog {
	DECLARE_CLASS( PrefDialog )
    DECLARE_EVENT_TABLE()

public:
	PrefDialog(wxWindow *parent, wxWindowID id, const wxString &title );


	void		OnOK( wxCommandEvent& event );
	void		LoadValues();
	int			GetInterval();
	void		SaveValues();

public:
private:
	int			m_interval;

};



#endif
