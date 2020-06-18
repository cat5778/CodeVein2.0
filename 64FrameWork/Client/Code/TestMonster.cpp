#include "stdafx.h"
#include "TestMonster.h"

#include "Export_Function.h"

CTestMonster::CTestMonster(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
	, m_vDir(0.f, 0.f, 0.f)
{

}

CTestMonster::~CTestMonster(void)
{

}

HRESULT CTestMonster::Ready_GameObject(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	return S_OK;
}

_int CTestMonster::Update_GameObject(const _float& fTimeDelta)
{
	Engine::CGameObject::Update_GameObject(fTimeDelta);

	Engine::CTransform*		pPlayerTransformCom = dynamic_cast<Engine::CTransform*>
												(Engine::Get_Component(L"GameLogic", 
													L"TestPlayer", 
													L"Com_Transform",
													Engine::ID_DYNAMIC));
	NULL_CHECK_RETURN(pPlayerTransformCom, -1);

	_vec3		vPlayerPos;
	pPlayerTransformCom->Get_Info(Engine::INFO_POS, &vPlayerPos);

	m_pTransformCom->Chase_Target(&vPlayerPos, (m_fSpeed * fTimeDelta));

	return 0;
}

void CTestMonster::Render_GameObject(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransformCom->m_matWorld);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CTestMonster::Add_Component(void)
{
	Engine::CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<Engine::CTriCol*>(Engine::Clone(RESOURCE_STATIC, L"Buffer_TriCol"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_pComponentMap[Engine::ID_STATIC].emplace(L"Com_Buffer", pComponent);

	pComponent = m_pTransformCom = Engine::CTransform::Create();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_pComponentMap[Engine::ID_STATIC].emplace(L"Com_Transform", pComponent);
	
	return S_OK;
}



CTestMonster* CTestMonster::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTestMonster*	pInstance = new CTestMonster(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CTestMonster::Free(void)
{
	Engine::CGameObject::Free();
}

