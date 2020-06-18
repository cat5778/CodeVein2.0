#include "Layer.h"

USING(Engine)

Engine::CLayer::CLayer(void)
{

}

Engine::CLayer::~CLayer(void)
{
}

HRESULT Engine::CLayer::Ready_Layer(void)
{
	return S_OK;
}
HRESULT CLayer::LateReady_Layer(void)
{
	for (auto& iter : m_pObjectMap)
		FAILED_CHECK_RETURN(iter.second->LateReady_GameObject(), E_FAIL);

	return S_OK;
}
_int Engine::CLayer::Update_Layer(const _float& fTimeDelta)
{
	_int iExit = 0;

	for (auto& iter : m_pObjectMap)
	{
		if (!iter.second->Get_Enable())
			continue;
		iExit = iter.second->Update_GameObject(fTimeDelta);

		if (iExit & 0x00000001)
			return 1;

		if (iExit & 0x80000000)
			return -1;
	}

	return iExit;
}

_int CLayer::LateUpdate_Layer(const _float & fTimeDelta)
{
	_int iExit = 0;

	for (auto& iter : m_pObjectMap)
	{
		if (!iter.second->Get_Enable())
			continue;

		iExit = iter.second->LateUpdate_GameObject(fTimeDelta);

		if (iExit & 0x00000001)
			return 1;

		if (iExit & 0x80000000)
			return -1;
	}
	return iExit;
}
//
//void CLayer::Set_LayerName(wstring wstrName)
//{
//	m_wstrLayerName = wstrName;
//}

HRESULT Engine::CLayer::Add_GameObject(const _tchar* pObjTag, CGameObject* pGameObject)
{
	if (nullptr == pGameObject)
		return E_FAIL;

	m_pObjectMap.emplace(pObjTag, pGameObject);

	return S_OK;
}

map<wstring, CGameObject*>& CLayer::Get_ObjectMap()
{
	return m_pObjectMap;

}

Engine::CLayer* Engine::CLayer::Create(void)
{
	CLayer*		pInstance = new CLayer;

	if (FAILED(pInstance->Ready_Layer()))
		Safe_Release(pInstance);

	return pInstance;
}

void Engine::CLayer::Free(void)
{
	for_each(m_pObjectMap.begin(), m_pObjectMap.end(), CDeleteMap());
		
	m_pObjectMap.clear();
}

Engine::CComponent* Engine::CLayer::Get_Component(const _tchar* pObjTag, const _tchar* pComponentTag, COMPONENTID eID)
{
	auto	iter = m_pObjectMap.begin();

	for (; iter != m_pObjectMap.end(); iter++)
	{
		if (iter->first == pObjTag)
			return iter->second->Get_Component(pComponentTag, eID);
	}
	return nullptr;
}

CGameObject * CLayer::Get_GameObject(const _tchar * pObjTag)
{
	auto	iter = m_pObjectMap.begin();

	for (; iter != m_pObjectMap.end(); iter++)
	{
		if (iter->first == pObjTag)
			return iter->second;
	}
	return nullptr;
}

