// ShellExt.cpp : CShellExt 的实现

#include "stdafx.h"
#include "ShellExt.h"
#include "atlconv.h"
// 为使用 ATL 字符串转换宏而包含的头文件 

extern HINSTANCE g_hInstance;
static 	char s_init_folder[MAX_PATH*2] = {0};

HRESULT CShellExt::Initialize(LPCITEMIDLIST pidlFolder, LPDATAOBJECT pDataObj, HKEY hProgID)
{
	SHGetPathFromIDListA(pidlFolder, s_init_folder);
	memset(_vc_path, 0, MAX_PATH*2);
	char inifile[MAX_PATH] = {0};
	char drive[_MAX_DRIVE*2] = {0};
	char dir[_MAX_DIR*2] = {0};


	GetModuleFileNameA(g_hInstance, inifile, MAX_PATH);
	//MessageBoxA(NULL, inifile, "", MB_OK);

	errno_t err;
	err = _splitpath_s(inifile, 
		drive,
		_MAX_DRIVE,
		dir,
		_MAX_DIR,
		NULL, 0, 
		NULL, 0);
	err = _makepath_s(inifile, _MAX_PATH, drive, dir, "zShellExt", "ini");

	GetPrivateProfileStringA("VS2008", "path", "", _vc_path, MAX_PATH*2, inifile);
	//MessageBoxA(NULL, _vc_path, "", MB_OK);
	return S_OK;
}

HRESULT CShellExt::QueryContextMenu(HMENU hmenu,UINT uMenuIndex, UINT uidFirstCmd, UINT uidLastCmd, UINT uFlags)
{
	UINT uCmdID = uidFirstCmd;
	// If the flags include CMF_DEFAULTONLY then we shouldn't do anything.
	if ( uFlags & CMF_DEFAULTONLY )
		return MAKE_HRESULT ( SEVERITY_SUCCESS, FACILITY_NULL, 0 );

	// Add our register/unregister items.
	InsertMenu ( hmenu, uMenuIndex, MF_STRING | MF_BYPOSITION, uCmdID++,
		_T("&CMD") );


	uMenuIndex++;

	InsertMenu ( hmenu, uMenuIndex, MF_STRING | MF_BYPOSITION, uCmdID++,
		_T("&Visual Studio 2008 Command Prompt") );


	uMenuIndex++;

	// The return value tells the shell how many top-level items we added.
	return MAKE_HRESULT ( SEVERITY_SUCCESS, FACILITY_NULL, 2 );



}



HRESULT CShellExt::GetCommandString(UINT uCmdID, UINT uFlags, UINT* pwReserved, LPSTR szName, UINT cchMax) 
{
	USES_CONVERSION;
	LPCTSTR szPrompt;

	if ( uFlags & GCS_HELPTEXT )
	{
		switch ( uCmdID )
		{
		case 0:
			szPrompt = _T("Run cmd.exe");
			break;

		case 1:
			szPrompt = _T("Run vcvarsall.bat");
			break;

		default:
			ATLASSERT(0);           // should never get here
			return E_INVALIDARG;
			break;
		}

		// Copy the help text into the supplied buffer.  If the shell wants
		// a Unicode string, we need to case szName to an LPCWSTR.
		if ( uFlags & GCS_UNICODE )
		{
			lstrcpynW ( (LPWSTR) szName, T2CW(szPrompt), cchMax );
		}
		else
		{
			lstrcpynA ( szName, T2CA(szPrompt), cchMax );
		}
	}
	else if ( uFlags & GCS_VERB )
	{
		// Copy the verb name into the supplied buffer.  If the shell wants
		// a Unicode string, we need to case szName to an LPCWSTR.
		switch ( uCmdID )
		{
		case 0:
			if ( uFlags & GCS_UNICODE )
				lstrcpynW ( (LPWSTR) szName, L"CMD", cchMax );
			else
				lstrcpynA ( szName, "CMD", cchMax );
			break;

		case 1:
			if ( uFlags & GCS_UNICODE )
				lstrcpynW ( (LPWSTR) szName, L"Visual Studio 2008 Command Prompt", cchMax );
			else
				lstrcpynA ( szName, "Visual Studio 2008 Command Prompt", cchMax );
			break;

		default:
			ATLASSERT(0);           // should never get here
			return E_INVALIDARG;
			break;
		}
	}

	return S_OK;
}



HRESULT CShellExt::InvokeCommand ( LPCMINVOKECOMMANDINFO pInfo )
{

	char par[MAX_PATH*2] = {0};
	char folder[MAX_PATH*2] = {0};
	
	// If lpVerb really points to a string, ignore this function call and bail out.  
	if ( 0 != HIWORD( pInfo->lpVerb ) )
		return E_INVALIDARG;

	if (NULL == pInfo->lpDirectory)
	{
		strcpy_s(folder, MAX_PATH*2, s_init_folder);
	}
	else
	{
		strcpy_s(folder, MAX_PATH*2, pInfo->lpDirectory);
	}
	// Check that lpVerb is one of our commands (0 or 1)
	switch ( LOWORD(pInfo->lpVerb) )
	{
	case 0:
		strncat_s(par, MAX_PATH*2, "/k cd /d \"", MAX_PATH*2);		
		strncat_s(par, MAX_PATH*2, folder, MAX_PATH*2);
		strncat_s(par, MAX_PATH*2, "\"", MAX_PATH*2);
		ShellExecuteA(NULL, "open", "CMD.EXE", par/*_T("/k \"cd %L\"")*/, NULL, SW_SHOWNORMAL);
		return S_OK;
		break;
	case 1:

		strncat_s(par, MAX_PATH*2, "/k cd /d", MAX_PATH*2);
		strncat_s(par, MAX_PATH*2, folder, MAX_PATH*2);
		strncat_s(par, MAX_PATH*2, "&&call \"", MAX_PATH*2);
		strncat_s(par, MAX_PATH*2, _vc_path, MAX_PATH*2);
		strncat_s(par, MAX_PATH*2, "\" x86", MAX_PATH*2); 
		ShellExecuteA(NULL, "open", "CMD.EXE", par/*_T("/k \"cd %L\"")*/, NULL, SW_SHOWNORMAL);
		return S_OK;
		break;

	default:
		ATLASSERT(0);               // should never get here
		return E_INVALIDARG;
		break;
	}
}

// CShellExt

