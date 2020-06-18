#include "stdafx.h"
#include "Stone.h"

#include "Export_Function.h"

CStone::CStone(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{

}

CStone::~CStone(void)
{

}

HRESULT CStone::Ready_GameObject(const _uint& iFlag)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransformCom->Set_Scale(0.01f, 0.01f, 0.01f);

	m_pTransformCom->Set_Pos(rand() % 10, 0.f, rand() % 10);
	
	m_iFlag = iFlag;

	return S_OK;
}

_int CStone::Update_GameObject(const _float& fTimeDelta)
{
	Engine::CGameObject::Update_GameObject(fTimeDelta);

	//SetUp_OnTerrain();
	
	//m_bColl = Collision_ToObject(L"GameLogic", L"Player");
		
	m_pRendererCom->Add_RenderGroup(Engine::RENDER_NONALPHA, this);


	return 0;
}

void CStone::Render_GameObject(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransformCom->m_matWorld);

	
	m_pMeshCom->Render_Meshes();

	_matrix		matWorld;
	m_pTransformCom->Get_WorldMatrix(&matWorld);

	m_pColliderCom->Render_Collider(Engine::COLLTYPE(m_bColl), &matWorld);

}

HRESULT CStone::Add_Component(void)
{
	Engine::CComponent*		pComponent = nullptr;

	pComponent = m_pMeshCom = dynamic_cast<Engine::CStaticMesh*>(Engine::Clone(RESOURCE_STAGE, L"Mesh_Stone"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_pComponentMap[Engine::ID_STATIC].emplace(L"Com_Mesh", pComponent);

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

	// collider
	pComponent = m_pColliderCom = Engine::CCollider::Create(m_pGraphicDev, 
															m_pMeshCom->Get_VtxPos(),
															m_pMeshCom->Get_NumVtx(), 
															m_pMeshCom->Get_Stride());
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_pComponentMap[Engine::ID_STATIC].emplace(L"Com_Collider", pComponent);




	return S_OK;
}

void CStone::Key_Input(const _float& fTimeDelta)
{
	m_pTransformCom->Get_Info(Engine::INFO_LOOK, &m_vDir);

	if (GetAsyncKeyState(VK_UP) & 0x8000)
	{
		D3DXVec3Normalize(&m_vDir, &m_vDir);
		m_pTransformCom->Move_Pos(&(m_vDir * m_fSpeed * fTimeDelta));
	}

	if (GetAsyncKeyState(VK_DOWN) & 0x8000)
	{
		D3DXVec3Normalize(&m_vDir, &m_vDir);
		m_pTransformCom->Move_Pos(&(m_vDir * -m_fSpeed * fTimeDelta));
	}

	if (GetAsyncKeyState(VK_LEFT) & 0x8000)
		m_pTransformCom->Rotation(Engine::ROT_Y, D3DXToRadian(90.f * fTimeDelta));

	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
		m_pTransformCom->Rotation(Engine::ROT_Y, D3DXToRadian(-90.f * fTimeDelta));

	if (Engine::Get_DIMouseState(Engine::DIM_LB) & 0x80)
	{
		//_vec3		vPickPos = PickUp_OnTerrain();

		//m_pTransformCom->Move_TargetPos(&vPickPos, m_fSpeed, fTimeDelta);
	}

}

_bool CStone::Collision_ToObject(const _tchar* pLayerTag, const _tchar* pObjTag)
{
	Engine::CCollider*	pPlayerColliderCom = dynamic_cast<Engine::CCollider*>(Engine::Get_Component(pLayerTag, pObjTag, L"Com_Collider", Engine::ID_STATIC));
	NULL_CHECK_RETURN(pPlayerColliderCom, false);
	

	if(0 == m_iFlag)
		return m_pCalculatorCom->Collision_OBB(pPlayerColliderCom->Get_Min(),
											pPlayerColliderCom->Get_Max(),
											pPlayerColliderCom->Get_ColliderMatrix(),
											m_pColliderCom->Get_Min(),
											m_pColliderCom->Get_Max(),
											m_pColliderCom->Get_ColliderMatrix());


	else
		return m_pCalculatorCom->Collision_AABB(pPlayerColliderCom->Get_Min(),
											pPlayerColliderCom->Get_Max(),
											pPlayerColliderCom->Get_ColliderMatrix(), 
											m_pColliderCom->Get_Min(), 
											m_pColliderCom->Get_Max(), 
											m_pColliderCom->Get_ColliderMatrix());
}

CStone* CStone::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _uint& iFlag)
{
	CStone*	pInstance = new CStone(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(iFlag)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CStone::Free(void)
{
	Engine::CGameObject::Free();
}

//
//void CStone::SetUp_OnTerrain(void)
//{
//	_vec3		vPos;
//	m_pTransformCom->Get_Info(Engine::INFO_POS, &vPos);
//
//	Engine::CTerrainTex*		pTerrainBufferCom = dynamic_cast<Engine::CTerrainTex*>
//		(Engine::Get_Component(L"GameLogic", L"Terrain", L"Com_Buffer", Engine::ID_STATIC));
//	NULL_CHECK(pTerrainBufferCom);
//
//	_float	fHeight = m_pCalculatorCom->Compute_HeightOnTerrain(&vPos,
//		pTerrainBufferCom->Get_VtxPos(),
//		pTerrainBufferCom->Get_VtxCntX(),
//		pTerrainBufferCom->Get_VtxCntZ());
//
//	m_pTransformCom->Set_Pos(vPos.x, fHeight, vPos.z);
//}

//_vec3 CStone::PickUp_OnTerrain(void)
//{
//	//Engine::CTerrainTex*	pTerrainBufferCom = dynamic_cast<Engine::CTerrainTex*>(Engine::Get_Component(L"GameLogic", L"Terrain", L"Com_Buffer", Engine::ID_STATIC));
//	////NULL_CHECK_RETURN(pTerrainBufferCom, _vec3(0.f, 0.f, 0.f));
//	//if (pTerrainBufferCom == nullptr)
//	//	return _vec3(INIT_VEC3);
//	//Engine::CTransform*	pTerrainTransformCom = dynamic_cast<Engine::CTransform*>(Engine::Get_Component(L"GameLogic", L"Terrain", L"Com_Transform", Engine::ID_DYNAMIC));
//	//NULL_CHECK_RETURN(pTerrainTransformCom, _vec3(0.f, 0.f, 0.f));
//
//	return m_pCalculatorCom->Picking_OnTerrain(g_hWnd, pTerrainBufferCom, pTerrainTransformCom);
//}
