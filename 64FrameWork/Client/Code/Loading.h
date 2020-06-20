#ifndef Loading_h__
#define Loading_h__

#include "Defines.h"
#include "Base.h"
class CMainApp;
class CLoading : public CBase 
{
public:
static enum THREAD_OPTION {BASIC,STATIC,DYNAMIC,COLLIDER,OPTION_END};
private:
	explicit CLoading(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CLoading(void);

public:
	LOADINGID			Get_LoadingID(void) const { return m_eLoading; }
	CRITICAL_SECTION*	Get_Crt(void) { return &m_Crt; }
	_bool				Get_Finish(void);
	const _tchar*		Get_LoadString(THREAD_OPTION eOption) { return m_szLoading[eOption]; }
	
	list<MESH_PATH*>&	Get_PathList() {return m_pPathList;}

public:
	static _uint	CALLBACK Thread_Main(void* pArg);
	static _uint	CALLBACK Thread_Dynamic(void* pArg);
	static _uint	CALLBACK Thread_Basic(void* pArg);
	static _uint	CALLBACK Thread_Collider(void* pArg);

public:
	HRESULT					Ready_Loading(LOADINGID eLoading);
	_bool					Ready_Mesh(MESH_PATH * pPathInfo);
	_uint					Loading_ForStage(void);
	_bool					Loading_Basic();
	_bool					Loading_Dynamic();
	_bool					Loading_Collider();
	_bool					Loading_Mesh();
	_bool					Loading_Mesh2();
	wchar_t*				CharToWChar(const char* pstrSrc);
	void					DividString(wstring wstrOrigin, wstring& wstrObject, wstring& wstrBone, _uint& uiIdx);


private:
	HANDLE					m_hThread[OPTION_END];// 로딩스레드 
	_uint					m_uiLoadingCheck[OPTION_END] = { 0, };
	CRITICAL_SECTION		m_Crt;
	LOADINGID				m_eLoading;
	LPDIRECT3DDEVICE9		m_pGraphicDev;
	_bool					m_bFinish;
	_tchar					m_szLoading[OPTION_END][256];
	list<MESH_PATH*>		m_pPathList;
	LOADMODE				m_eLoadmode=LOAD_PLAYER;
public:
	static CLoading*		Create(LPDIRECT3DDEVICE9 pGraphicDev, LOADINGID eLoading);
private:
	virtual void	Free(void);

};


#endif // Loading_h__
