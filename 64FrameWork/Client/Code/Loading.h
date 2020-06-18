#ifndef Loading_h__
#define Loading_h__

#include "Defines.h"
#include "Base.h"
class CMainApp;
class CLoading : public CBase 
{
private:
	explicit CLoading(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CLoading(void);

public:
	LOADINGID			Get_LoadingID(void) const { return m_eLoading; }
	CRITICAL_SECTION*	Get_Crt(void) { return &m_Crt; }
	_bool				Get_Finish(void) const { return m_bFinish; }
	const _tchar*		Get_LoadString(void) { return m_szLoading; }
	list<MESH_PATH*>&	Get_PathList() {return m_pPathList;}

public:
	static _uint	CALLBACK Thread_Main(void* pArg);
	
public:
	HRESULT					Ready_Loading(LOADINGID eLoading);
	_uint					Loading_ForStage(void);
	_bool					Ready_Mesh(MESH_PATH * pPathInfo);
	_bool					Loading_Mesh();
	_bool					Loading_Collider();
	wchar_t*				CharToWChar(const char* pstrSrc);
	void					DividString(wstring wstrOrigin, wstring& wstrObject, wstring& wstrBone, _uint& uiIdx);


private:
	HANDLE					m_hThread;

	CRITICAL_SECTION		m_Crt;
	LOADINGID				m_eLoading;
	LPDIRECT3DDEVICE9		m_pGraphicDev;
	_bool					m_bFinish;
	_tchar					m_szLoading[256];
	list<MESH_PATH*>		m_pPathList;
	LOADMODE				m_eLoadmode=LOAD_PLAYER;
public:
	static CLoading*		Create(LPDIRECT3DDEVICE9 pGraphicDev, LOADINGID eLoading);
private:
	virtual void	Free(void);

};


#endif // Loading_h__
