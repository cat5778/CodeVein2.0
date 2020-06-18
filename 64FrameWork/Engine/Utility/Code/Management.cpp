#include "Export_Utility.h"

USING(Engine)
IMPLEMENT_SINGLETON(CManagement)

Engine::CManagement::CManagement(void)
{

}

Engine::CManagement::~CManagement(void)
{
	Free();
}

HRESULT CManagement::Ready_Management(LPDIRECT3DDEVICE9 & pGraphicDev)
{
	D3DVIEWPORT9		ViewPort;
	pGraphicDev->GetViewport(&ViewPort);

	FAILED_CHECK_RETURN(Ready_RenderTarget(pGraphicDev, L"Target_Albedo", ViewPort.Width, ViewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 0.f)), E_FAIL);
	FAILED_CHECK_RETURN(Ready_DebugBuffer(L"Target_Albedo", 0.f, 0.f, 200.f, 200.f), E_FAIL);

	FAILED_CHECK_RETURN(Ready_RenderTarget(pGraphicDev, L"Target_Normal", ViewPort.Width, ViewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 1.f)), E_FAIL);
	FAILED_CHECK_RETURN(Ready_DebugBuffer(L"Target_Normal", 0.f, 200.f, 200.f, 200.f), E_FAIL);

	FAILED_CHECK_RETURN(Ready_RenderTarget(pGraphicDev, L"Target_Light", ViewPort.Width, ViewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 1.f)), E_FAIL);
	FAILED_CHECK_RETURN(Ready_DebugBuffer(L"Target_Light", 200.f, 0.f, 200.f, 200.f), E_FAIL);

	FAILED_CHECK_RETURN(Ready_RenderTarget(pGraphicDev, L"Target_Specular", ViewPort.Width, ViewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 0.f)), E_FAIL);
	FAILED_CHECK_RETURN(Ready_DebugBuffer(L"Target_Specular", 200.f, 200.f, 200.f, 200.f), E_FAIL);

	FAILED_CHECK_RETURN(Ready_RenderTarget(pGraphicDev, L"Target_Depth", ViewPort.Width, ViewPort.Height, D3DFMT_A32B32G32R32F, D3DXCOLOR(1.f, 1.f, 1.f, 1.f)), E_FAIL);
	FAILED_CHECK_RETURN(Ready_DebugBuffer(L"Target_Depth", 0.f, 400.f, 200.f, 200.f), E_FAIL);

	FAILED_CHECK_RETURN(Ready_MRT(L"MRT_Defferd", L"Target_Albedo"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_MRT(L"MRT_Defferd", L"Target_Normal"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_MRT(L"MRT_Defferd", L"Target_Depth"), E_FAIL);

	FAILED_CHECK_RETURN(Ready_MRT(L"MRT_LightAcc", L"Target_Light"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_MRT(L"MRT_LightAcc", L"Target_Specular"), E_FAIL);


	CShader*		pShader = nullptr;

	pShader = CShader::Create(pGraphicDev, L"../../Reference/Header/Shader_Sample.hpp");
	NULL_CHECK_RETURN(pShader, E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"Shader_Sample", pShader), E_FAIL);

	pShader = CShader::Create(pGraphicDev, L"../../Reference/Header/Shader_Terrain.hpp");
	NULL_CHECK_RETURN(pShader, E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"Shader_Terrain", pShader), E_FAIL);

	pShader = CShader::Create(pGraphicDev, L"../../Reference/Header/Shader_Mesh.hpp");
	NULL_CHECK_RETURN(pShader, E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"Shader_Mesh", pShader), E_FAIL);

	pShader = CShader::Create(pGraphicDev, L"../../Reference/Header/Shader_Light.hpp");
	NULL_CHECK_RETURN(pShader, E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"Shader_Light", pShader), E_FAIL);

	pShader = CShader::Create(pGraphicDev, L"../../Reference/Header/Shader_Blend.hpp");
	NULL_CHECK_RETURN(pShader, E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"Shader_Blend", pShader), E_FAIL);

	pShader = CShader::Create(pGraphicDev, L"../../Reference/Header/Shader_Effect.hpp");
	NULL_CHECK_RETURN(pShader, E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"Shader_Effect", pShader), E_FAIL);

	return S_OK;
}

HRESULT Engine::CManagement::SetUp_Scene(CScene* pScene)
{
	Safe_Release(m_pScene);

	Get_Renderer()->Clear_RenderGroup();

	m_pScene = pScene;

	FAILED_CHECK_RETURN(m_pScene->LateReady_Scene(), E_FAIL);

	return S_OK;
}

_int Engine::CManagement::Update_Scene(const _float& fTimeDelta)
{
	if (nullptr == m_pScene)
		return -1;

	_int iEvent = m_pScene->Update_Scene(fTimeDelta);

	return iEvent;
}

_int CManagement::LateUpdate_Scene(const _float & fTimeDelta)
{
	if (nullptr == m_pScene)
		return -1;

	m_pScene->LateUpdate_Scene(fTimeDelta);

	return 0;
}

void CManagement::Render_Scene(LPDIRECT3DDEVICE9 & pGraphicDev)
{
	Get_Renderer()->Render_GameObject(pGraphicDev);

	if (nullptr == m_pScene)
		return;

	m_pScene->Render_Scene();
}

void Engine::CManagement::Render_Scene(void)
{
	Get_Renderer()->Render_GameObject();

	if (nullptr == m_pScene)
		return;

	m_pScene->Render_Scene();
}

void Engine::CManagement::Free(void)
{
	Safe_Release(m_pScene);
}

Engine::CComponent* Engine::CManagement::Get_Component(const _tchar* pLayerTag, const _tchar* pObjTag, const _tchar* pComponentTag, COMPONENTID eID)
{
	if (nullptr == m_pScene)
		return nullptr;

	return m_pScene->Get_Component(pLayerTag, pObjTag, pComponentTag, eID);
}

CGameObject * CManagement::Get_GameObject(const _tchar * pLayerTag, const _tchar * pObjTag)
{
	if (nullptr == m_pScene)
		return nullptr;

	return m_pScene->Get_GameObject(pLayerTag, pObjTag);
}

CLayer * CManagement::Get_Layer(const _tchar * pLayerTag)
{
	if (nullptr == m_pScene)
		return nullptr;

	return m_pScene->Get_Layer(pLayerTag);
}

