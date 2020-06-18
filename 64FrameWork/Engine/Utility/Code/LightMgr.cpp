#include "LightMgr.h"

USING(Engine)
IMPLEMENT_SINGLETON(CLightMgr)

Engine::CLightMgr::CLightMgr(void)
{

}

Engine::CLightMgr::~CLightMgr(void)
{
	Free();
}


void Engine::CLightMgr::Free(void)
{
	for_each(m_vecLight.begin(), m_vecLight.end(), CDeleteObj());
	m_vecLight.clear();
}

HRESULT Engine::CLightMgr::Ready_Light(LPDIRECT3DDEVICE9 pGraphicDev,
	const D3DLIGHT9* pLightInfo,
	const _uint& iIndex)
{
	CLight*		pLight = CLight::Create(pGraphicDev, pLightInfo, iIndex);
	NULL_CHECK_RETURN(pLight, E_FAIL);

	m_vecLight.push_back(pLight);

	return S_OK;
}

const D3DLIGHT9* Engine::CLightMgr::Get_LightInfo(const _uint& iIndex /*= 0*/)
{
	return m_vecLight[iIndex]->Get_LightInfo();
}

void Engine::CLightMgr::Render_Light(LPD3DXEFFECT& pEffect)
{
	for (auto& iter : m_vecLight)
		iter->Render_Light(pEffect);
}

