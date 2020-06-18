#include "stdafx.h"
#include "SphereCollider.h"
#include "Export_Function.h"

CSphereCollider::CSphereCollider(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{

}

CSphereCollider::CSphereCollider(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrObjName, wstring wstrBone)
	: Engine::CGameObject(pGraphicDev)
{
	m_ObjName = wstrObjName;
	m_wstrBone = wstrBone;
}

CSphereCollider::CSphereCollider(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrObjName, wstring wstBone, _bool bisStaticMesh)
	: Engine::CGameObject(pGraphicDev)
{
	m_ObjName = wstrObjName;
	m_wstrBone = wstBone;
	m_bIsStatic = bisStaticMesh;
}
CSphereCollider::~CSphereCollider()
{
}

HRESULT CSphereCollider::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	Ready_SphereMesh();
	return S_OK;
}

_int CSphereCollider::Update_GameObject(const _float & fTimeDelta)
{
	if (m_bIsStatic)
		Set_SMParentMatrix(fTimeDelta);
	else
		Set_DMParentMatrix(fTimeDelta);


	return S_OK;
}

void CSphereCollider::Render_GameObject(void)
{
#ifdef _DEBUG
	if (m_pSphereMesh!= nullptr)
	{
		m_pGraphicDev->BeginScene();

		//_matrix matOldWorld;
		//m_pTransformCom->Get_WorldMatrix(&matOldWorld);

		m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransformCom->m_matWorld);

		m_pGraphicDev->GetTransform(D3DTS_WORLD, &m_matSphereWorld);

		if (m_tCollData.eCollOpt == COLL_OPT_OBJECT)
			m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		else if (m_tCollData.eCollOpt == COLL_OPT_OBJECT)
			m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_POINT);
		else if (m_tCollData.eCollOpt == COLL_OPT_OBJECT)
			m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

		m_pSphereMesh->DrawSubset(0);

		m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

		//m_pGraphicDev->SetTransform(D3DTS_WORLD, &matOldWorld);

		m_pGraphicDev->EndScene();
	}
#endif
}

HRESULT CSphereCollider::Add_Component(void)
{
	Engine::CComponent*		pComponent = nullptr;

	pComponent = m_pTransformCom = Engine::CTransform::Create();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_pComponentMap[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);


#ifdef _DEBUG
	pComponent = m_pRendererCom = Engine::Get_Renderer();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->AddRef();
	m_pComponentMap[Engine::ID_STATIC].emplace(L"Com_Renderer", pComponent);

#endif // _DEBUG
	return S_OK;
}

void CSphereCollider::Ready_SphereMesh()
{
	LPD3DXMESH	pSphereMesh = nullptr;
	Engine::VTXCOL* pVertices = nullptr;

	D3DXCreateSphere(m_pGraphicDev, 1.f, 10, 10, &pSphereMesh, nullptr);

	LPDIRECT3DVERTEXBUFFER9 pSphereBuffer;

	//m_tSphereData[i].fRadius = NAV_RADIUS;
	//m_tSphereData[i].vPosition = m_vPoint[i];
	pSphereMesh->CloneMeshFVF(0, Engine::FVF_COL, m_pGraphicDev, &m_pSphereMesh);
	m_pSphereMesh->GetVertexBuffer(&pSphereBuffer);
	_uint uiNumVtx = m_pSphereMesh->GetNumVertices();

	pSphereBuffer->Lock(0, 0, (void**)&pVertices, 0);

	for (_uint i = 0; i < uiNumVtx; i++)
		pVertices[i].dwColor = D3DCOLOR_RGBA(0, 255, 0, 255);

	pSphereBuffer->Unlock();
	pSphereBuffer->Release();

	pSphereMesh->Release();


}



HRESULT CSphereCollider::Set_DMParentMatrix(_float fTimeDelta)
{
	if (nullptr == m_pParentBoneMatrix && !m_ObjName.empty())
	{
		Engine::CDynamicMesh*	pMeshCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::Get_Component(L"GameLogic", m_ObjName.c_str(), L"Com_Mesh", Engine::ID_STATIC));
		NULL_CHECK_RETURN(pMeshCom, 0);

		string strBone;
		strBone.assign(m_wstrBone.begin(), m_wstrBone.end());
		const char* pszName = strBone.c_str();

		//const Engine::D3DXFRAME_DERIVED* pBone = pMeshCom->Get_FrameByName("RightHandAttach");
		const Engine::D3DXFRAME_DERIVED* pBone = pMeshCom->Get_FrameByName(pszName);
		NULL_CHECK_RETURN(pBone, 0);

		m_pParentBoneMatrix = &pBone->CombinedTransformationMatrix;

		Engine::CTransform*	pObjTransCom = dynamic_cast<Engine::CTransform*>(Engine::Get_Component(L"GameLogic", m_ObjName.c_str(), L"Com_Transform", Engine::ID_DYNAMIC));
		NULL_CHECK_RETURN(pObjTransCom, 0);

		m_pParentWorldMatrix = pObjTransCom->Get_WorldMatrixPointer();

	}
	else
	{
		Engine::CGameObject::Update_GameObject(fTimeDelta);
		m_pTransformCom->Set_ParentMatrix(&(*m_pParentBoneMatrix * *m_pParentWorldMatrix));

		m_pTransformCom->Set_Scale(m_tCollData.fRadius, m_tCollData.fRadius, m_tCollData.fRadius);
		//m_pTransformCom->Move_Pos(&m_tCollData.vPos);


#ifdef _DEBUG
		m_pRendererCom->Add_RenderGroup(Engine::RENDER_NONALPHA, this);
#endif // _DEBUG

	}
	return S_OK;
}

HRESULT CSphereCollider::Set_SMParentMatrix(_float fTimeDelta)
{

	return S_OK;
}


CSphereCollider * CSphereCollider::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CSphereCollider*	pInstance = new CSphereCollider(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}
CSphereCollider * CSphereCollider::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrObjName, wstring wstBone)
{
	CSphereCollider*	pInstance = new CSphereCollider(pGraphicDev, wstrObjName,  wstBone);

	if (FAILED(pInstance->Ready_GameObject()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}
CSphereCollider * CSphereCollider::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrObjName, wstring wstBone, _bool bisStaticMesh)
{
	CSphereCollider*	pInstance = new CSphereCollider(pGraphicDev, wstrObjName, wstBone,bisStaticMesh);

	if (FAILED(pInstance->Ready_GameObject()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}
void CSphereCollider::Free(void)
{
	Safe_Release(m_pSphereMesh);

	Engine::CGameObject::Free();
}
