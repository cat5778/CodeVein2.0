#include "stdafx.h"
#include "Logo.h"
#include "Stage.h"
#include "MainApp.h"

#include "Export_Function.h"

CLogo::CLogo(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CScene(pGraphicDev)
{

}

CLogo::~CLogo(void)
{

}

HRESULT CLogo::Ready_Scene(void)
{
	FAILED_CHECK_RETURN(Engine::CScene::Ready_Scene(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Resource(m_pGraphicDev, RESOURCE_END), E_FAIL);

	FAILED_CHECK_RETURN(Ready_GameLogic_Layer(L"GameLogic"), E_FAIL);

	m_pLoading = CLoading::Create(m_pGraphicDev, LOADING_STAGE);
	NULL_CHECK_RETURN(m_pLoading, E_FAIL);

	return S_OK;
}

_int CLogo::Update_Scene(const _float& fTimeDelta)
{
	_int iExit = Engine::CScene::Update_Scene(fTimeDelta);
	m_ppPathlist;

	if (true == m_pLoading->Get_Finish())
	{
		*m_ppPathlist = m_pLoading->Get_PathList();

		if (Engine::Get_DIKeyState(DIK_RETURN) & 0x80)
		{
			Engine::CScene*		pScene = CStage::Create(m_pGraphicDev);
			NULL_CHECK_RETURN(pScene, E_FAIL);

			FAILED_CHECK_RETURN(Engine::SetUp_Scene(pScene), E_FAIL);

			return iExit;
		}		
	}



	return iExit;
}

void CLogo::Render_Scene(void)
{
	// DEBUG 용
	Engine::Render_Font(L"Font_Default", m_pLoading->Get_LoadString(CLoading::BASIC),	&_vec2(10.f, 10.f), D3DXCOLOR(1.f, 0.f, 0.f, 1.f));
	Engine::Render_Font(L"Font_Default", m_pLoading->Get_LoadString(CLoading::STATIC),	&_vec2(10.f, 30.f), D3DXCOLOR(1.f, 0.f, 0.f, 1.f));
	Engine::Render_Font(L"Font_Default", m_pLoading->Get_LoadString(CLoading::DYNAMIC), &_vec2(10.f, 50.f), D3DXCOLOR(1.f, 0.f, 0.f, 1.f));
	Engine::Render_Font(L"Font_Default", m_pLoading->Get_LoadString(CLoading::COLLIDER),&_vec2(10.f, 70.f), D3DXCOLOR(1.f, 0.f, 0.f, 1.f));

}

CLogo* CLogo::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CLogo*	pInstance = new CLogo(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CLogo::Free(void)
{
	Engine::Safe_Release(m_pLoading);
	Engine::CScene::Free();
}

HRESULT CLogo::Ready_GameLogic_Layer(const _tchar* pLayerTag)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);
	Engine::CGameObject*		pGameObject = nullptr;

	// BackGround
	pGameObject = CBackGround::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"BackGround", pGameObject), E_FAIL);



	m_pLayerMap.emplace(pLayerTag, pLayer);

	return S_OK;
}

HRESULT CLogo::Ready_Resource(LPDIRECT3DDEVICE9& pGraphicDev, RESOURCEID eMax)
{
	// 메모리 예약
	Engine::Reserve_ContainerSize(eMax);

	FAILED_CHECK_RETURN(Engine::Ready_Buffer(pGraphicDev,
		RESOURCE_STATIC,
		L"Buffer_TriCol",
		Engine::BUFFER_TRICOL),
		E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Buffer(pGraphicDev,
		RESOURCE_STATIC,
		L"Buffer_RcCol",
		Engine::BUFFER_RCCOL),
		E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Buffer(pGraphicDev,
		RESOURCE_STATIC,
		L"Buffer_RcTex",
		Engine::BUFFER_RCTEX),
		E_FAIL);

	// 텍스쳐
	FAILED_CHECK_RETURN(Engine::Ready_Texture(pGraphicDev,
											RESOURCE_LOGO,
											L"Texture_Logo",
											Engine::TEX_NORMAL,
											L"../../Resource/Texture/LogoBack/LogoBack_%d.png", 38),
											E_FAIL); 
	return S_OK;
}
