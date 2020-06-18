#include "Scene.h"

USING(Engine)

Engine::CScene::CScene(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
{
	m_pGraphicDev->AddRef();
}

Engine::CScene::~CScene(void)
{

}

HRESULT Engine::CScene::Ready_Scene(void)
{
	return S_OK;
}

HRESULT CScene::LateReady_Scene(void)
{
	for (auto& iter : m_pLayerMap)
		FAILED_CHECK_RETURN(iter.second->LateReady_Layer(), E_FAIL);

	return S_OK;
}

_int Engine::CScene::Update_Scene(const _float& fTimeDelta)
{
	_int iExit = 0;

	for (auto& iter : m_pLayerMap)
	{
		iExit = iter.second->Update_Layer(fTimeDelta);

		if (iExit & 0x00000001)
			return 1;

		if (iExit & 0x80000000)
			return -1;
	}

	return iExit;

}

_int CScene::LateUpdate_Scene(const _float & fTimeDelta)
{
	_int iExit = 0;

	for (auto& iter : m_pLayerMap)
	{
		iExit = iter.second->LateUpdate_Layer(fTimeDelta);

		if (iExit & 0x00000001)
			return 1;

		if (iExit & 0x80000000)
			return -1;
	}

	return iExit;
}

void Engine::CScene::Render_Scene(void)
{
	
}

void CScene::Set_SceneChange(const bool & flag)
{

}

void Engine::CScene::Free(void)
{
	for_each(m_pLayerMap.begin(), m_pLayerMap.end(), CDeleteMap());
	m_pLayerMap.clear();

	Safe_Release(m_pGraphicDev);
}

Engine::CComponent* Engine::CScene::Get_Component(const _tchar* pLayerTag, const _tchar* pObjTag, const _tchar* pComponentTag, COMPONENTID eID)
{
	auto iter = m_pLayerMap.find(pLayerTag);

	if (iter == m_pLayerMap.end())
		return nullptr;

	return iter->second->Get_Component(pObjTag, pComponentTag, eID);
}

CGameObject * CScene::Get_GameObject(const _tchar * pLayerTag, const _tchar * pObjTag)
{
	auto iter = m_pLayerMap.find(pLayerTag);

	if (iter == m_pLayerMap.end())
		return nullptr;

	return iter->second->Get_GameObject(pObjTag);
}

CLayer * CScene::Get_Layer(const _tchar * pLayerTag)
{
	auto iter = m_pLayerMap.find(pLayerTag);

	if (iter == m_pLayerMap.end())
		return nullptr;

	return iter->second;
}
