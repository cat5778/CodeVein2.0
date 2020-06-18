#include "stdafx.h"
#include "MainApp.h"

#include "Export_Function.h"
#include "Logo.h"
#include "Stage.h"
CMainApp::CMainApp(void)
{
	m_pSoundMgr = CSoundMgr::GetInstance();
}

CMainApp::~CMainApp(void)
{

}

HRESULT CMainApp::Ready_MainApp(void)
{
	FAILED_CHECK_RETURN(SetUp_DefaultSetting(&m_pGraphicDev), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Scene(m_pGraphicDev, &m_pManagement), E_FAIL);

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	Ready_Managers();
	m_pSoundMgr->Initialize();

	return S_OK;
}

_int CMainApp::Update_MainApp(const _float& fTimeDelta)
{
	Engine::Set_InputDev();
	m_pKeyMgr->Update();

	if (nullptr != m_pManagement)
		m_pManagement->Update_Scene(fTimeDelta);
	if (!m_ppPathList.empty())
	{
		int a = 3;
	}

	m_pSoundMgr->UpdateSound();

	return 0;
}

_int CMainApp::LateUpdate_MainApp(const _float & fTimeDelta)
{
	if (CHANGE_SCENE == m_iEvent)
		return m_iEvent;

	if (nullptr != m_pManagement)
		m_pManagement->LateUpdate_Scene(fTimeDelta);

	return m_iEvent;
}

void CMainApp::Render_MainApp(void)
{
	//m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, TRUE);			// Z버퍼에 저장은 하되, 자동 정렬을 수행 할지 말지를 판단하는 옵션
	//m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);	// Z버퍼에 픽셀들의 깊이 값을 저장 할지 말지를 판단하는 옵션

	Engine::Render_Begin(D3DXCOLOR(0.f, 0.f, 1.f, 1.f));
	
	if (nullptr != m_pManagement)
		m_pManagement->Render_Scene(m_pGraphicDev);
		//m_pManagement->Render_Scene();

	Engine::Render_End();
}

void CMainApp::Ready_Managers(void)
{
	m_pKeyMgr = CKeyMgr::GetInstance();
	m_pTimerMgr = CTimerManager::GetInstance();

}

HRESULT CMainApp::SetUp_DefaultSetting(LPDIRECT3DDEVICE9 * ppGraphicDev)
{
	FAILED_CHECK_RETURN(Engine::Ready_GraphicDev(g_hWnd,
		Engine::MODE_WIN,
		WINCX,
		WINCY,
		&m_pDeviceClass),
		E_FAIL);

	m_pDeviceClass->AddRef();

	(*ppGraphicDev) = m_pDeviceClass->Get_Device();
	(*ppGraphicDev)->AddRef();

	// FONT

	FAILED_CHECK_RETURN(Engine::Ready_Font(m_pGraphicDev, L"Font_Default", L"궁서", 20, 20, FW_HEAVY), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Font(m_pGraphicDev, L"Font_Second", L"바탕", 10, 10, FW_THIN), E_FAIL);

	// INPUTDEV
	FAILED_CHECK_RETURN(Engine::Ready_InputDev(g_hInst, g_hWnd), E_FAIL);

	return S_OK;
}

HRESULT CMainApp::Ready_Scene(LPDIRECT3DDEVICE9& pGraphicDev, 
								Engine::CManagement ** ppManagement)
{
	Engine::CScene*		pScene = nullptr;

	FAILED_CHECK_RETURN(Engine::Create_Management(pGraphicDev, ppManagement), E_FAIL);
	(*ppManagement)->AddRef();

	//FAILED_CHECK_RETURN(Engine::Create_Management(ppManagement), E_FAIL);
	//(*ppManagement)->AddRef();

	pScene =CLogo::Create(pGraphicDev);

	NULL_CHECK_RETURN(pScene, E_FAIL);

	FAILED_CHECK_RETURN((*ppManagement)->SetUp_Scene(pScene), E_FAIL);

	dynamic_cast<CLogo*>(pScene)->Set_PathList(&m_ppPathList);
	
	return S_OK;
}

CMainApp* CMainApp::Create(void)
{
	CMainApp*	pInstance = new CMainApp;

	if (FAILED(pInstance->Ready_MainApp()))
		Safe_Release(pInstance);

	return pInstance;
}

void CMainApp::Free(void)
{
	m_pKeyMgr->DestroyInstance();
	m_pTimerMgr->DestroyInstance();
	m_pSoundMgr->DestroyInstance();
	CColliderManager::GetInstance()->DestroyInstance();

	Engine::Safe_Release(m_pGraphicDev);
	Engine::Safe_Release(m_pDeviceClass);
	Engine::Safe_Release(m_pManagement);


	for (auto itr = m_ppPathList.begin(); itr != m_ppPathList.end();)
	{
		if ((*itr) != nullptr)
		{
			delete (*itr);
			(*itr) = nullptr;
			itr = m_ppPathList.erase(itr);
		}
		else
			itr++;
	}
	Engine::Release_Resources();
	Engine::Release_Utility();
	Engine::Release_System();
}

