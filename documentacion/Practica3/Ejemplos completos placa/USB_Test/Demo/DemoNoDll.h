// DemoNoDll.h : main header file for the DEMONODLL application
//

#if !defined(AFX_DEMONODLL_H__C9C85650_9BA9_44F7_A8C8_06AD69368B83__INCLUDED_)
#define AFX_DEMONODLL_H__C9C85650_9BA9_44F7_A8C8_06AD69368B83__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CDemoNoDllApp:
// See DemoNoDll.cpp for the implementation of this class
//

class CDemoNoDllApp : public CWinApp
{
public:
	CDemoNoDllApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDemoNoDllApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CDemoNoDllApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DEMONODLL_H__C9C85650_9BA9_44F7_A8C8_06AD69368B83__INCLUDED_)
