#include "ColliderGroup.h"
#include "SphereColliderCom.h"
#include "DynamicMesh.h"
USING(Engine)

CColliderGroup::CColliderGroup(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrObjTag,Engine::CTransform* pTransform, CDynamicMesh* pMesh)
	: m_pGraphicDev(pGraphicDev)
{
	m_pGraphicDev->AddRef();
	m_pPartentTransform = pTransform;
	m_pDynamicMesh = pMesh;
	m_wstrObjTag = wstrObjTag;
	for(int i=0; i<STATE_END;i++)
		m_ePrevState[i]=m_eCurState[i]= STATE_END;
}

CColliderGroup::~CColliderGroup(void)
{
}


HRESULT CColliderGroup::Ready_Component(void)
{


	return S_OK;
}

_int CColliderGroup::Update_Component(const _float & fTimeDelta)
{
	for (int i = 0; i < COLOPT_END; i++)
	{


		_bool bIsColl=false;
		for (auto &pColl : m_pCollVec[i])
		{
			pColl->Update_Component(fTimeDelta);
			StateMachine((COLLOPTION)i);
			
			//if (!m_bIsCollEnable[i])
			//{
			//	if (m_eCurState[(COLLOPTION)i] == STATE_EXIT)
			//		m_eCurState[(COLLOPTION)i] = STATE_END;
			//	else if (m_eCurState[(COLLOPTION)i] != STATE_END)
			//		m_eCurState[(COLLOPTION)i] = STATE_EXIT;

			//	break;
			//}

			if (bIsColl == false)
			{
				if (pColl->IsColl())
				{
					if (m_eCurState[(COLLOPTION)i] == STATE_ENTER)
						m_eCurState[(COLLOPTION)i] = STATE_STAY;

					else if (m_eCurState[(COLLOPTION)i] != STATE_STAY )
						m_eCurState[(COLLOPTION)i] = STATE_ENTER;

					bIsColl = true;
				}
			}
		}
		if (!bIsColl)
		{
			if (m_eCurState[(COLLOPTION)i] == STATE_EXIT)
				m_eCurState[(COLLOPTION)i] = STATE_END;
			else if (m_eCurState[(COLLOPTION)i] != STATE_END)
				m_eCurState[(COLLOPTION)i] = STATE_EXIT;
		}
	}
		
	return 0;

}

void CColliderGroup::Render_Collider()
{

	for (int i = 0; i < COLOPT_END; i++)
	{
		for (auto &pColl : m_pCollVec[i])
			pColl->Render_Collider();
	}


}


CSphereColliderCom* CColliderGroup::Add_Collider(Engine::COLLOPTION eOption,wstring wstrBoneTag, _float fRadius)
{
	string strBone;
	strBone.assign(wstrBoneTag.begin(), wstrBoneTag.end());
	
	const Engine::D3DXFRAME_DERIVED* pBone = m_pDynamicMesh->Get_FrameByName(strBone.c_str());
	
	_uint uiIdx = 0;
		
	for (int i = 0; i < COLOPT_END; i++)
	{	
		for(auto &pColl : m_pCollVec[i])
			if (pColl->Get_CollTag().find(wstrBoneTag) != wstring::npos)
				uiIdx++;
	}

	CSphereColliderCom* pSphereCollCom = CSphereColliderCom::Create(m_pGraphicDev,
																	m_wstrObjTag,
																	wstrBoneTag,
																	m_pPartentTransform,
																	pBone,
																	uiIdx);
	if (pSphereCollCom == nullptr)
		return nullptr;
	pSphereCollCom->Set_Radius(fRadius);

	m_pCollVec[eOption].push_back(pSphereCollCom);



	return pSphereCollCom;
}

CSphereColliderCom * CColliderGroup::Add_Collider(Engine::COLLOPTION eOption, wstring wstrBoneTag, wstring wstrCollTag, _vec3 vPos, _float fRadius ,_uint uiIdx)
{
	string strBone;
	strBone.assign(wstrBoneTag.begin(), wstrBoneTag.end());

	const Engine::D3DXFRAME_DERIVED* pBone = m_pDynamicMesh->Get_FrameByName(strBone.c_str());

	CSphereColliderCom* pSphereCollCom = CSphereColliderCom::Create(m_pGraphicDev, 
																	m_wstrObjTag, 
																	wstrBoneTag, 
																	m_pPartentTransform, 
																	pBone, 
																	uiIdx);
	if (pSphereCollCom == nullptr)
		return nullptr;

	pSphereCollCom->Set_Radius(fRadius);
	pSphereCollCom->Set_Pos(vPos);


	m_pCollVec[eOption].push_back(pSphereCollCom);


	return pSphereCollCom;
}

void CColliderGroup::Set_Pos(Engine::COLLOPTION eOption, wstring wstrColliderTag, _vec3 vPos)
{
	for (auto &pColl : m_pCollVec[eOption])
	{
		if (pColl->Get_BoneTag().compare(wstrColliderTag) == 0)
			pColl->Set_Pos(vPos);
	}


}

void CColliderGroup::Set_Radius(Engine::COLLOPTION eOption, wstring wstrColliderTag,_float fRadius)
{
	for (auto &pColl : m_pCollVec[eOption])
	{
		if (pColl->Get_BoneTag().compare(wstrColliderTag) == 0)
			pColl->Set_Radius(fRadius);
	}


}

Engine::CSphereColliderCom * CColliderGroup::Get_SphereColl(wstring eOption)
{
	Engine::CSphereColliderCom* pSphereColl = nullptr;
	
	for (int i = 0; i < COLOPT_END; i++)
	{
		for (auto &pColl : m_pCollVec[i])
		{
			if (pColl->Get_CollTag().compare(eOption) == 0)
			{
				
				return pSphereColl= pColl;
			}
		}
	}
	return nullptr;
}

vector<Engine::CSphereColliderCom*>* CColliderGroup::Get_CollVec(Engine::COLLOPTION eOption)
{
	return &m_pCollVec[eOption];

}

vector<wstring>* CColliderGroup::Get_CollNameVec(COLLOPTION eCollOption)
{
	return m_wstrColObjVec[eCollOption];

}



void CColliderGroup::StateMachine(COLLOPTION eOption)
{
	if (m_eCurState[eOption] != m_ePrevState[eOption])
	{
		m_ePrevState[eOption] = m_eCurState[eOption];
		switch (m_ePrevState[eOption])
		{
		case Engine::STATE_ENTER:
			m_bisColl[eOption][STATE_ENTER] = true;
			m_bisColl[eOption][STATE_STAY] = false;
			m_bisColl[eOption][STATE_EXIT] = false;
			break;
		case Engine::STATE_STAY:
			m_bisColl[eOption][STATE_ENTER] = false;
			m_bisColl[eOption][STATE_STAY] = true;
			m_bisColl[eOption][STATE_EXIT] = false;
			break;
		case Engine::STATE_EXIT:
			m_bisColl[eOption][STATE_ENTER] = false;
			m_bisColl[eOption][STATE_STAY] = false;
			m_bisColl[eOption][STATE_EXIT] = true;
			break;
		default:
			m_bisColl[eOption][STATE_ENTER] = false;
			m_bisColl[eOption][STATE_STAY] = false;
			m_bisColl[eOption][STATE_EXIT] = false;
			m_ePrevState[eOption] = m_eCurState[eOption] = STATE_END;
			break;
		}
	}

}

_bool CColliderGroup::IsColl(COLLOPTION eCollType,COLLSTATE eCollState)
{
	return m_bisColl[eCollType][eCollState];


}

_bool CColliderGroup::Get_ColliderEnable(COLLOPTION eOption)
{
	return m_bIsCollEnable[eOption];
}

void CColliderGroup::Set_ColliderEnable(COLLOPTION eOption, _bool bIsEnable)
{
	m_bIsCollEnable[eOption] = bIsEnable;
}


CColliderGroup * CColliderGroup::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrObjTag, Engine::CTransform* pTransform, CDynamicMesh * pMesh)
{
	CColliderGroup*	pInstance = new CColliderGroup(pGraphicDev, wstrObjTag, pTransform, pMesh);

	if (FAILED(pInstance->Ready_Component()))
		Engine::Safe_Release(pInstance);

	return pInstance;

}

void CColliderGroup::Free(void)
{
	for (int i = 0; i < COLOPT_END; i++)
	{
		for (auto &pColl : m_pCollVec[i])
		{
			pColl->Release();
		}
	}


	Safe_Release(m_pGraphicDev);

}
