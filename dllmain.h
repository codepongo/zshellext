// dllmain.h : 模块类的声明。

class CzShellExtModule : public CAtlDllModuleT< CzShellExtModule >
{
public :
	DECLARE_LIBID(LIBID_zShellExtLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_ZSHELLEXT, "{9B84839A-94E3-44B9-8900-812BFB948DC6}")
};

extern class CzShellExtModule _AtlModule;
