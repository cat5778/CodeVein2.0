#include "stdafx.h"
#include "DynamicObject.h"
#include "Export_Function.h"
#include "ColliderManager.h"


CDynamicObject::CDynamicObject(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrName,_uint uiIdx)
	: Engine::CGameObject(pGraphicDev)
{
	m_uiIdx = uiIdx;
	m_ObjName = wstrName;
}

CDynamicObject::CDynamicObject(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrName, _uint uiIdx, TRANSFORM_INFO tInfo)
	: Engine::CGameObject(pGraphicDev)
{
	m_uiIdx = uiIdx;
	m_ObjName = wstrName;
	m_tTransformInfo = tInfo;
	m_bIsRespawn = true;
}

CDynamicObject::~CDynamicObject()
{
}

HRESULT CDynamicObject::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	Set_TransformData();

	m_pMeshCom->Set_AnimationSet(0);

	return S_OK;
}

_int CDynamicObject::Update_GameObject(const _float & fTimeDelta)
{
	m_pMeshCom->Set_AnimationSet(m_uiAni);
	Engine::CGameObject::Update_GameObject(fTimeDelta);
	m_pMeshCom->Play_Animation(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_NONALPHA, this);

	return S_OK;
}

void CDynamicObject::Render_GameObject(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransformCom->m_matWorld);
	m_pMeshCom->Render_Meshes();
	_matrix		matWorld;
	m_pTransformCom->Get_WorldMatrix(&matWorld);
	m_pColliderGroupCom->Render_Collider();
	//m_pColliderCom->Render_Collider(Engine::COLLTYPE(m_bColl), &matWorld);
}


HRESULT CDynamicObject::Add_Component(void)
{
	Engine::CComponent*		pComponent = nullptr;

	pComponent = m_pMeshCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::Clone(RESOURCE_STAGE, m_ObjName.c_str()));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_pComponentMap[Engine::ID_STATIC].emplace(L"Com_Mesh", pComponent);// º¯°æ

	pComponent = m_pTransformCom = Engine::CTransform::Create();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_pComponentMap[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	pComponent = m_pRendererCom = Engine::Get_Renderer();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->AddRef();
	m_pComponentMap[Engine::ID_STATIC].emplace(L"Com_Renderer", pComponent);

	pComponent = m_pCalculatorCom = Engine::CCalculator::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_pComponentMap[Engine::ID_STATIC].emplace(L"Com_Calculator", pComponent);

	pComponent = m_pColliderGroupCom = Engine::CColliderGroup::Create(m_pGraphicDev,m_ObjName,m_pTransformCom,m_pMeshCom);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_pComponentMap[Engine::ID_DYNAMIC].emplace(L"Com_ColliderGroup", pComponent);
	
	CColliderManager::GetInstance()->Get_ObjCollider(m_pColliderGroupCom, m_ObjName);

	// collider
	//pComponent = m_pColliderCom = Engine::CCollider::Create(m_pGraphicDev,
	//	m_pMeshCom->Get_VtxPos(),
	//	m_pMeshCom->Get_NumVtx(),
	//	m_pMeshCom->Get_Stride());
	//NULL_CHECK_RETURN(pComponent, E_FAIL);
	//m_pComponentMap[Engine::ID_STATIC].emplace(L"Com_Collider", pComponent);

	return S_OK;
}

void CDynamicObject::Set_TransformData()
{
	if (m_bIsRespawn)
	{
		m_pTransformCom->m_vScale=m_tTransformInfo.vScale;
		m_pTransformCom->m_vAngle= m_tTransformInfo.vRotation;
		m_pTransformCom->m_vInfo[Engine::INFO_POS]= m_tTransformInfo.vPosition;
	}
	else
	{
		m_pTransformCom->Set_Scale(DEFAULT_MESH_SCALE);
		m_pTransformCom->Set_Pos(INIT_VEC3);
	}

}



CDynamicObject * CDynamicObject::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrName,_uint uiIdx)
{
	CDynamicObject*	pInstance = new CDynamicObject(pGraphicDev, wstrName, uiIdx);

	if (FAILED(pInstance->Ready_GameObject()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}
CDynamicObject * CDynamicObject::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrName, _uint uiIdx, TRANSFORM_INFO tInfo)
{
	CDynamicObject*	pInstance = new CDynamicObject(pGraphicDev, wstrName, uiIdx, tInfo);

	if (FAILED(pInstance->Ready_GameObject()))
		Engine::Safe_Release(pInstance);

	return pInstance;

}
void CDynamicObject::Free(void)
{
	Engine::CGameObject::Free();
}
