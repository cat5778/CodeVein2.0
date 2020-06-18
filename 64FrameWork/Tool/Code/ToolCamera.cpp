#include "pch.h"
#include "ToolCamera.h"
#include "Export_Function.h"
#include <iostream>
using namespace std;
CToolCamera::CToolCamera(LPDIRECT3DDEVICE9 pDevice) :
	Engine::CGameObject(pDevice)
{
}

CToolCamera::~CToolCamera()
{
}

HRESULT CToolCamera::Ready_GameObject(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransformCom->m_vInfo[Engine::INFO_POS] = { 0.f,10.f,-5.f };
	m_pKeyMgr = Engine::GetKeyMgr();
	m_pTransformCom->Rotation(Engine::ROT_X, D3DXToRadian(60));



	return S_OK;
}

_int CToolCamera::Update_GameObject(const _float& fTimeDelta)
{


	//Key_Input(fTimeDelta);

	_vec3 tempFront = {};
	Engine::CGameObject::Update_GameObject(fTimeDelta);
	D3DXVec3Cross(&tempFront, &m_pTransformCom->m_vInfo[Engine::INFO_RIGHT], &m_pTransformCom->m_vInfo[Engine::INFO_UP]);
	D3DXVec3Normalize(&tempFront, &tempFront);
	_vec3 tempCamLook = {};
	tempCamLook = m_pTransformCom->m_vInfo[Engine::INFO_POS] + tempFront;



	D3DXMatrixLookAtLH(&m_matView, &m_pTransformCom->m_vInfo[Engine::INFO_POS], &tempCamLook, &_vec3(0.f, 1.f, 0.f));
	D3DXMatrixPerspectiveFovLH(&m_matProj, D3DXToRadian(60.f), _float(WINCX) / WINCY, 1.f, 1000.f);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);


	m_pKeyMgr->Update();


	return S_OK;

}

void CToolCamera::Render_GameObject(void)
{

}

CToolCamera * CToolCamera::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CToolCamera* pInstance = new CToolCamera(pDevice);
	if (FAILED(pInstance->Ready_GameObject()))
	{
		Engine::Safe_Release(pInstance);
	}
	return pInstance;
}

void CToolCamera::Free(void)
{
	Engine::CGameObject::Free();
}


HRESULT CToolCamera::Add_Component(void)
{
	Engine::CComponent*		pComponent = nullptr;

	pComponent = m_pTransformCom = Engine::CTransform::Create();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Cam_Com_Transform", pComponent);

	return S_OK;
}

void CToolCamera::Key_Input(const _float& fTimeDelta)
{
	_vec3 tempFront = {};

	D3DXVec3Cross(&tempFront, &m_pTransformCom->m_vInfo[Engine::INFO_RIGHT], &m_pTransformCom->m_vInfo[Engine::INFO_UP]);
	if (m_pKeyMgr->KeyPressing(Engine::KEY_UP))
	{
		D3DXVec3Normalize(&m_vDir, &tempFront);
		m_pTransformCom->Move_Pos(&(m_vDir * m_fSpeed * fTimeDelta));
	}

	if (m_pKeyMgr->KeyPressing(Engine::KEY_DOWN))
	{
		D3DXVec3Normalize(&m_vDir, &tempFront);
		m_pTransformCom->Move_Pos(&(m_vDir * -m_fSpeed * fTimeDelta));
	}


	if (m_pKeyMgr->KeyPressing(Engine::KEY_LEFT))
	{
		m_pTransformCom->Get_Info(Engine::INFO_RIGHT, &m_vDir);
		D3DXVec3Normalize(&m_vDir, &m_vDir);
		m_pTransformCom->Move_Pos(&(m_vDir * -m_fSpeed * fTimeDelta));
	}
	if (m_pKeyMgr->KeyPressing(Engine::KEY_RIGHT))
	{
		m_pTransformCom->Get_Info(Engine::INFO_RIGHT, &m_vDir);
		D3DXVec3Normalize(&m_vDir, &m_vDir);
		m_pTransformCom->Move_Pos(&(m_vDir * m_fSpeed * fTimeDelta));
	}


	if (m_pKeyMgr->KeyPressing(Engine::KEY_Q))
		m_pTransformCom->Rotation(Engine::ROT_X, D3DXToRadian(90.f * fTimeDelta));

	if (m_pKeyMgr->KeyPressing(Engine::KEY_A))
		m_pTransformCom->Rotation(Engine::ROT_X, D3DXToRadian(-90.f * fTimeDelta));

	if (m_pKeyMgr->KeyPressing(Engine::KEY_W))
		m_pTransformCom->Rotation(Engine::ROT_Y, D3DXToRadian(90.f * fTimeDelta));

	if (m_pKeyMgr->KeyPressing(Engine::KEY_S))
		m_pTransformCom->Rotation(Engine::ROT_Y, D3DXToRadian(-90.f * fTimeDelta));



}