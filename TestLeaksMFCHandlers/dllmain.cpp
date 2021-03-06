// dllmain.cpp : Implementation of DllMain.

#include "stdafx.h"
#include "resource.h"
#include "TestLeaksMFCHandlers_i.h"
#include "dllmain.h"
#include "xdlldata.h"

CTestLeaksMFCHandlersModule _AtlModule;

class CTestLeaksMFCHandlersApp : public CWinApp
{
public:

// Overrides
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	DECLARE_MESSAGE_MAP()
};

BEGIN_MESSAGE_MAP(CTestLeaksMFCHandlersApp, CWinApp)
END_MESSAGE_MAP()

CTestLeaksMFCHandlersApp theApp;

BOOL CTestLeaksMFCHandlersApp::InitInstance()
{
	if (!PrxDllMain(m_hInstance, DLL_PROCESS_ATTACH, nullptr))
		return FALSE;
	return CWinApp::InitInstance();
}

int CTestLeaksMFCHandlersApp::ExitInstance()
{
	return CWinApp::ExitInstance();
}
