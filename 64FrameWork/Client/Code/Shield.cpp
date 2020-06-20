#include "stdafx.h"
#include "Shield.h"

#include "Export_Function.h"

CShield::CShield(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{

}

CShield::~CShield(void)
{

}

HRESULT CShield::Ready_GameObject(const _uint& iFlag)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransformCom->Rotation(Engine::ROT_X, D3DXToRadian(90.f));

	m_iFlag = iFlag;

	return S_OK;
}

_int CShield::Update_GameObject(const _float& fTimeDelta)
{
	if (nullptr == m_pParentBoneMatrix)
	{
		Engine::CDynamicMesh*	pMonsterMeshCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::Get_Component(L"GameLogic", L"RussianHat_0", L"Com_Mesh", Engine::ID_STATIC));
		NULL_CHECK_RETURN(pMonsterMeshCom, 0);

		const Engine::D3DXFRAME_DERIVED* pBone = pMonsterMeshCom->Get_FrameByName("RightHandAttach");
		NULL_CHECK_RETURN(pBone, 0);

		m_pParentBoneMatrix = &pBone->CombinedTransformationMatrix;

		Engine::CTransform*	pPlayerTransCom = dynamic_cast<Engine::CTransform*>(Engine::Get_Component(L"GameLogic", L"RussianHat_0", L"Com_Transform", Engine::ID_DYNAMIC));
		NULL_CHECK_RETURN(pPlayerTransCom, 0);

		m_pParentWorldMatrix = pPlayerTransCom->Get_WorldMatrixPointer();
		m_bIsEquip = true;
	}




	//m_bColl = Collision_ToObject(L"GameLogic", L"Player");
	Engine::CGameObject::Update_GameObject(fTimeDelta);

	if (m_bIsEquip)
	{	
		m_pTransformCom->Set_ParentMatrix(&(*m_pParentBoneMatrix * *m_pParentWorldMatrix));
		m_pTransformCom->Get_WorldMatrix(&m_MatOldWorld);
		memcpy(m_vOldPos, &m_MatOldWorld._41, sizeof(_vec3));
		
	}
	else
	{
		D3DXVec3TransformNormal(&m_vThrow, &_vec3(0.f, -0.25f, 1.f), &m_MatOldWorld);
		m_fAccThrow += fTimeDelta;
		m_vThrow *= m_fAccThrow;
		m_vOldPos+= m_vThrow;
		m_pTransformCom->Set_Pos(&m_vOldPos);

		_matrix matRotY, matTemp;
		D3DXMatrixRotationY(&matRotY,D3DXToRadian(60.f));
		matTemp= matRotY*m_MatOldWorld ;
				
		m_pTransformCom->Set_ParentMatrix(&matTemp);

	}

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_NONALPHA, this);


	return 0;
}

void CShield::Render_GameObject(void)
{
	//m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransformCom->m_matWorld);

	//
	//m_pMeshCom->Render_Meshes();

	//_matrix		matWorld;
	//m_pTransformCom->Get_WorldMatrix(&matWorld);

	//m_pColliderCom->Render_Collider(Engine::COLLTYPE(false), &matWorld);


	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();
	_uint	iPassMax = 0;
	SetUp_ConstantTable(pEffect);
	pEffect->Begin(&iPassMax, 0);
	pEffect->BeginPass(0);
	m_pMeshCom->Render_Meshes(pEffect);
	pEffect->EndPass();

	pEffect->End();

	Safe_Release(pEffect);


}

HRESULT CShield::SetUp_ConstantTable(LPD3DXEFFECT & pEffect)
{
	_matrix			matWorld, matView, matProj;

	m_pTransformCom->Get_WorldMatrix(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	const D3DLIGHT9*		pLight = Engine::Get_LightInfo(0);
	NULL_CHECK_RETURN(pLight, S_OK);

	pEffect->SetVector("g_vLightDir", &_vec4(pLight->Direction, 0.f));
	pEffect->SetVector("g_vLightDiffuse", (_vec4*)&pLight->Diffuse);
	pEffect->SetVector("g_vLightSpecular", (_vec4*)&pLight->Specular);
	pEffect->SetVector("g_vLightAmbient", (_vec4*)&pLight->Ambient);

	D3DMATERIAL9		tMtrlInfo;
	ZeroMemory(&tMtrlInfo, sizeof(D3DMATERIAL9));

	tMtrlInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tMtrlInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tMtrlInfo.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tMtrlInfo.Emissive = D3DXCOLOR(0.f, 0.f, 0.f, 1.f);
	tMtrlInfo.Power = 100.f;

	pEffect->SetVector("g_vMtrlDiffuse", (_vec4*)&tMtrlInfo.Diffuse);
	pEffect->SetVector("g_vMtrlSpecular", (_vec4*)&tMtrlInfo.Specular);
	pEffect->SetVector("g_vMtrlAmbient", (_vec4*)&tMtrlInfo.Ambient);

	_vec4		vCamPos;
	D3DXMatrixInverse(&matView, NULL, &matView);
	memcpy(&vCamPos, &matView.m[3][0], sizeof(_vec4));

	pEffect->SetVector("g_vCamPos", &vCamPos);
	pEffect->SetFloat("g_fPower", tMtrlInfo.Power);

	return E_NOTIMPL;
}

HRESULT CShield::Add_Component(void)
{
	Engine::CComponent*		pComponent = nullptr;

	pComponent = m_pMeshCom = dynamic_cast<Engine::CStaticMesh*>(Engine::Clone(RESOURCE_STAGE, L"RussianHatShield"));
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

	pComponent = m_pShaderCom = dynamic_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_Mesh"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_pComponentMap[Engine::ID_STATIC].emplace(L"Com_Shader", pComponent);

	pComponent = m_pNaviCom = dynamic_cast<Engine::CNaviMesh*>(Engine::CNaviMesh::Create(m_pGraphicDev));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_pComponentMap[Engine::ID_STATIC].emplace(L"Com_Navi", pComponent);


	return S_OK;
}

_bool CShield::Collision_ToObject(const _tchar* pLayerTag, const _tchar* pObjTag)
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

void CShield::Set_Equip(_bool bIsEquip)
{
	  m_bIsEquip = bIsEquip; 
	  
}

void CShield::Set_Throw()
{

}


void CShield::Set_CellIdx(_uint uiCellIdx)
{
	m_pNaviCom->Set_Index(uiCellIdx);
	
}

CShield* CShield::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _uint& iFlag)
{
	CShield*	pInstance = new CShield(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(iFlag)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CShield::Free(void)
{
	Engine::CGameObject::Free();
}


