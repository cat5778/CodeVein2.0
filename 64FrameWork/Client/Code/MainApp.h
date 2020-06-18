#ifndef MainApp_h__
#define MainApp_h__

#include "Defines.h"
#include "Base.h"
#include "Engine_Defines.h"
#include "Export_Function.h"

BEGIN(Engine)

class CGraphicDev;
class CManagement;

END
class CLogo;
class CMainApp : public CBase
{
private: // 생성자, 소멸자
	explicit CMainApp(void);
	virtual ~CMainApp(void);

	// public 함수, 변수
public:
	HRESULT		Ready_MainApp(void);
	_int		Update_MainApp(const _float& fTimeDelta);
	_int		LateUpdate_MainApp(const _float& fTimeDelta);
	void		Render_MainApp(void);
	void		Ready_Managers(void);

	// protected 함수, 변수
	// private 함수, 변수

private:
	HRESULT					SetUp_DefaultSetting(LPDIRECT3DDEVICE9* ppGraphicDev);

	HRESULT					Ready_Scene(LPDIRECT3DDEVICE9& pGraphicDev, 
										Engine::CManagement** ppManagement);



private:
	Engine::CGraphicDev*			m_pDeviceClass = nullptr;
	Engine::CManagement*			m_pManagement = nullptr;
	LPDIRECT3DDEVICE9				m_pGraphicDev = nullptr;
	CKeyMgr*						m_pKeyMgr = nullptr;
	CTimerManager*					m_pTimerMgr = nullptr;
	CSoundMgr*						m_pSoundMgr = nullptr;

	_int							m_iEvent;
	list<MESH_PATH*>				m_ppPathList;
	
public:
	// 생성 함수
	static CMainApp*		Create(void);
	// 소멸 함수
private:
	virtual void Free(void);


};

#endif // MainApp_h__

