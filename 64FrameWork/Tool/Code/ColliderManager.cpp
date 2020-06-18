#include "stdafx.h"
#include "ColliderManager.h"
IMPLEMENT_SINGLETON(CColliderManager)

CColliderManager::CColliderManager()
{
}


CColliderManager::~CColliderManager()
{
	Release();
}

void CColliderManager::Get_ObjCollider(Engine::CColliderGroup* pCollGroupCom,wstring wstrObjTag)
{
	for (auto pCollData : m_pCollDataVec)
	{
		if (pCollData->wstrObjTag.find(wstrObjTag) != wstring::npos)
		{
			pCollGroupCom->Add_Collider((Engine::COLLOPTION)pCollData->uiOption, pCollData->wstrBoneTag, pCollData->wstrCollTag, pCollData->vPos ,pCollData->fRadius);
		}
	}
}

void CColliderManager::Release()
{
}
