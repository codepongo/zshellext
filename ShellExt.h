// ShellExt.h : CShellExt 的声明

#pragma once
#include "resource.h"       // 主符号

#include "zShellExt_i.h"
#include "shlobj.h"
#include "comdef.h"

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE 平台(如不提供完全 DCOM 支持的 Windows Mobile 平台)上无法正确支持单线程 COM 对象。定义 _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA 可强制 ATL 支持创建单线程 COM 对象实现并允许使用其单线程 COM 对象实现。rgs 文件中的线程模型已被设置为“Free”，原因是该模型是非 DCOM Windows CE 平台支持的唯一线程模型。"
#endif



// CShellExt

class ATL_NO_VTABLE CShellExt :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CShellExt, &CLSID_ShellExt>,
	public IDispatchImpl<IShellExt, &IID_IShellExt, &LIBID_zShellExtLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public IShellExtInit,
	public IContextMenu
{
public:
	CShellExt()
	{
	}

	DECLARE_REGISTRY_RESOURCEID(IDR_SHELLEXT)


	BEGIN_COM_MAP(CShellExt)
		COM_INTERFACE_ENTRY(IShellExt)
		COM_INTERFACE_ENTRY(IDispatch)
		COM_INTERFACE_ENTRY(IShellExtInit)
		COM_INTERFACE_ENTRY(IContextMenu) 
	END_COM_MAP()



	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

public:
	// IShellExtInit 
	STDMETHOD(Initialize)(LPCITEMIDLIST, LPDATAOBJECT, HKEY);
	// IContextMenu
	STDMETHOD(GetCommandString)(UINT, UINT, UINT*, LPSTR, UINT);
	STDMETHOD(InvokeCommand)(LPCMINVOKECOMMANDINFO); 
	STDMETHOD(QueryContextMenu)(HMENU, UINT, UINT, UINT, UINT);

	
	char _vc_path[MAX_PATH*2];
};

OBJECT_ENTRY_AUTO(__uuidof(ShellExt), CShellExt)
