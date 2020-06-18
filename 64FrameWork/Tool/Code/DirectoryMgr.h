#pragma once
class CDirectoryMgr
{
public:
	CDirectoryMgr();
	~CDirectoryMgr();


public:
	static CString ConvertRelativePath(const TCHAR* pFullPath);
	static void ExtractPathInfo(const TCHAR* pFullPath, list<MESH_PATH*>& rPathInfoList);
	static void ExtractPathInfo(const TCHAR* pFullPath, list<EFFECT_PATH*>& rPathInfoList);
	static int GetFileCount(const TCHAR* pFullPath);



};

