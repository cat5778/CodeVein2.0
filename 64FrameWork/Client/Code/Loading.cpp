#include "stdafx.h"
#include "Loading.h"
#include <fstream>
#include "Export_Function.h"
#include "DirectoryMgr.h"
#include <iostream>
#include "ColliderManager.h"
CLoading::CLoading(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
	, m_bFinish(false)
{
	ZeroMemory(m_szLoading, sizeof(_tchar) * 256);
	m_pGraphicDev->AddRef();
}

CLoading::~CLoading(void)
{

}

_uint	CALLBACK CLoading::Thread_Main(void* pArg)
{
	CLoading*		pLoading = (CLoading*)pArg;
	_uint iFlag = 0;

	EnterCriticalSection(pLoading->Get_Crt());

	switch (pLoading->Get_LoadingID())
	{
	case LOADING_STAGE:
		iFlag = pLoading->Loading_ForStage();
		break;

	case LOADING_BOSS:
		break;
	}
	
	LeaveCriticalSection(pLoading->Get_Crt());
	_endthreadex(0);

	return iFlag;
}

HRESULT CLoading::Ready_Loading(LOADINGID eLoading)
{
	InitializeCriticalSection(&m_Crt);

	m_hThread = (HANDLE)_beginthreadex(NULL, 0, Thread_Main, this, 0, NULL);

	m_eLoading = eLoading;

	return S_OK;
}

_uint CLoading::Loading_ForStage(void)
{
	lstrcpy(m_szLoading, L"Buffer Loading.............................");

	int i = 0;

	FAILED_CHECK_RETURN(Engine::Ready_Buffer(m_pGraphicDev,
												RESOURCE_STATIC,
												L"Buffer_CubeTex",
												Engine::BUFFER_CUBETEX),
												E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Navi",
		Engine::TYPE_NAVI,
		NULL,
		NULL),
		E_FAIL);

	lstrcpy(m_szLoading, L"Texture Loading.............................");
	
	// 텍스쳐

	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
												RESOURCE_STAGE,
												L"Texture_Cube",
												Engine::TEX_CUBE,
												L"../../Resource/Texture/SkyBox/burger%d.dds",
												4),
												E_FAIL);


	
	lstrcpy(m_szLoading, L"Mesh Loading.............................");


	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Player",
		Engine::TYPE_DYNAMIC,
		L"../../Resource/Mesh/DynamicMesh/Player2/",
		L"Player2.X"),
		E_FAIL);

	switch ((LOADMODE)LOAD_MODE)
	{
	case LOAD_NOMAL:
		Loading_Mesh();

		FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
			RESOURCE_STAGE,
			L"RussianHat",
			Engine::TYPE_DYNAMIC,
			L"../../Resource/Mesh/DynamicMesh/RussianHat/",
			L"RussianHat.X"),
			E_FAIL);
		break;
	case LOAD_NOMAL2:
		FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
			RESOURCE_STAGE,
			L"Map3",
			Engine::TYPE_STATIC,
			L"../../Resource/Mesh/StaticMesh/Snow/Map3/Map3/",
			L"Map3.X"),
			E_FAIL);
		FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
			RESOURCE_STAGE,
			L"SM_NormalGreatSwordA_ba01",
			Engine::TYPE_STATIC,
			L"../../Resource/Mesh/StaticMesh/Base/Weapon/SM_NormalGreatSwordA_ba01/",
			L"SM_NormalGreatSwordA_ba01.X"),
			E_FAIL);
		FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
			RESOURCE_STAGE,
			L"RussianHat",
			Engine::TYPE_DYNAMIC,
			L"../../Resource/Mesh/DynamicMesh/RussianHat/",
			L"RussianHat.X"),
			E_FAIL);
		FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
			RESOURCE_STAGE,
			L"RussianHatShield",
			Engine::TYPE_STATIC,
			L"../../Resource/Mesh/StaticMesh/Public/Weapon/RussianHatShield/",
			L"RussianHatShield.X"),
			E_FAIL);
		break;
	case LOAD_NOMAL3:
		Loading_Mesh();
		break;
	case LOAD_PLAYER:
		FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
			RESOURCE_STAGE,
			L"SM_NormalGreatSwordA_ba01",
			Engine::TYPE_STATIC,
			L"../../Resource/Mesh/StaticMesh/Base/Weapon/SM_NormalGreatSwordA_ba01/",
			L"SM_NormalGreatSwordA_ba01.X"),
			E_FAIL);
		FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
			RESOURCE_STAGE,
			L"RussianHat",
			Engine::TYPE_DYNAMIC,
			L"../../Resource/Mesh/DynamicMesh/RussianHat/",
			L"RussianHat.X"),
			E_FAIL);
		FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
			RESOURCE_STAGE,
			L"RussianHatShield",
			Engine::TYPE_STATIC,
			L"../../Resource/Mesh/StaticMesh/Public/Weapon/RussianHatShield/",
			L"RussianHatShield.X"),
			E_FAIL);
		break;
	case LOAD_BATTLE:
	{
		FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
			RESOURCE_STAGE,
			L"SM_NormalGreatSwordA_ba01",
			Engine::TYPE_STATIC,
			L"../../Resource/Mesh/StaticMesh/Base/Weapon/SM_NormalGreatSwordA_ba01/",
			L"SM_NormalGreatSwordA_ba01.X"),
			E_FAIL);

		FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
			RESOURCE_STAGE,
			L"RussianHat",
			Engine::TYPE_DYNAMIC,
			L"../../Resource/Mesh/DynamicMesh/RussianHat/",
			L"RussianHat.X"),
			E_FAIL);
		FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
			RESOURCE_STAGE,
			L"RussianHatShield",
			Engine::TYPE_STATIC,
			L"../../Resource/Mesh/StaticMesh/Public/Weapon/RussianHatShield/",
			L"RussianHatShield.X"),
			E_FAIL);


	}
		break;
	case LOAD_END:
		break;
	default:
		break;
	}

	Loading_Collider();
	
	lstrcpy(m_szLoading, L"Loading Complete!!!");

	m_bFinish = true;


	return 0;
}

_bool CLoading::Ready_Mesh(MESH_PATH * pPathInfo)
{
	if (Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_STAGE, pPathInfo->wstrObjectType.c_str(), Engine::TYPE_STATIC, pPathInfo->wstrRelative.c_str(), pPathInfo->wstrName.c_str()) < 0)
		return false;

	return 	true;
}

_bool CLoading::Loading_Mesh() //텍스트 읽고와서 메쉬 로딩
{
	TCHAR szFileName[MAX_STR] = L"../../Resource/Data/PathInfo.txt";

	ifstream fin;

	fin.open(szFileName,ios::in);

	if (fin.fail())
		return E_FAIL;
	
	char cTemp[MIN_STR];
	while (!fin.eof())
	{
		
		MESH_PATH* pPathInfo = new MESH_PATH;
		
		fin.getline(cTemp, MIN_STR);
		wchar_t* ppwchar = CharToWChar(cTemp);
		pPathInfo->wstrGroup = ppwchar;
		delete ppwchar;

		fin.getline(cTemp, MIN_STR);
		ppwchar = CharToWChar(cTemp);
		pPathInfo->wstrMap = ppwchar;
		delete ppwchar;

		fin.getline(cTemp, MIN_STR);
		ppwchar = CharToWChar(cTemp);
		pPathInfo->wstrMeshType= ppwchar;
		delete ppwchar;
		
		fin.getline(cTemp, MIN_STR);
		ppwchar = CharToWChar(cTemp);
		pPathInfo->wstrName= ppwchar;
		delete ppwchar;

		fin.getline(cTemp, MIN_STR);
		ppwchar = CharToWChar(cTemp);
		pPathInfo->wstrObjectType= ppwchar;
		delete ppwchar;

		fin.getline(cTemp, MIN_STR);
		ppwchar = CharToWChar(cTemp);
		pPathInfo->wstrRelative= ppwchar;
		delete ppwchar;

		if (pPathInfo->wstrName.empty())
		{
			delete pPathInfo;
			pPathInfo = nullptr;
			continue;
		}
		else
			m_pPathList.push_back(pPathInfo);
	}

	for (auto pPathInfo : m_pPathList)
	{
		if (pPathInfo->wstrMeshType.compare(L"StaticMesh") == 0)
		{
			if (pPathInfo->wstrObjectType.compare(L"SM_Ceiling_a_ba01"))
			{
				int a = 3;
			}
			if (Engine::Ready_Meshes(m_pGraphicDev,	RESOURCE_STAGE,pPathInfo->wstrObjectType.c_str(),Engine::TYPE_STATIC,pPathInfo->wstrRelative.c_str(),pPathInfo->wstrName.c_str()) < 0)
			{
				wstring wstr =L"Loading.cpp ReadyMesh Failed!!!  Object =" + pPathInfo->wstrObjectType;
				DE_COUT(wstr.c_str())
				continue;
			}

		}
		else
			continue;
	}

	


	return false;
}

_bool CLoading::Loading_Collider()
{
	TCHAR szFileName[MAX_STR] = L"../../Resource/Data/Collider/Test.txt";

	ifstream fin;

	fin.open(szFileName, ios::in);

	if (fin.fail())
		return E_FAIL;

	char cTemp[MIN_STR];
	while (!fin.eof())
	{
		SPHERE_COL_DATA* pColData = new SPHERE_COL_DATA;


		fin.getline(cTemp, MIN_STR);
		wchar_t* ppwchar = CharToWChar(cTemp);
		pColData->wstrObjTag = ppwchar;
		delete ppwchar;

		fin.getline(cTemp, MIN_STR);
		pColData->uiOption = (_uint)atoi(cTemp);

		fin.getline(cTemp, MIN_STR);
		ppwchar = CharToWChar(cTemp);
		pColData->wstrBoneTag = ppwchar;
		delete ppwchar;

		fin.getline(cTemp, MIN_STR);
		ppwchar = CharToWChar(cTemp);
		pColData->wstrCollTag = ppwchar;
		delete ppwchar;

		fin.getline(cTemp, MIN_STR);
		pColData->vPos.x = (float)atof(cTemp);

		fin.getline(cTemp, MIN_STR);
		pColData->vPos.y = (float)atof(cTemp);

		fin.getline(cTemp, MIN_STR);
		pColData->vPos.z = (float)atof(cTemp);

		fin.getline(cTemp, MIN_STR);
		pColData->fRadius = (float)atof(cTemp);

		if (pColData->wstrObjTag.empty())
		{
			delete pColData;
			pColData = nullptr;
			continue;
		}
		else
			CColliderManager::GetInstance()->Add_Colldata(pColData);
	}


}

//_bool CLoading::Loading_Collider()
//{
	//TCHAR szFileName[MAX_STR] = L"../../Resource/Data/Collider.txt";

	//ifstream fin;

	//fin.open(szFileName);

	//if (fin.fail())
	//	return E_FAIL;

	//wstring wstrTemp;

	//char cTemp[MIN_STR];
	//_vec3 vPos = { INIT_VEC3 };
	//_float fRadius = 0.f;
	//while (!fin.eof())
	//{
	//	D3DXVECTOR3 vPos;

	//	fin.getline(cTemp, MIN_STR);
	//	wchar_t* ppwchar = CharToWChar(cTemp);
	//	wstrTemp = ppwchar;
	//	delete ppwchar;

	//	if (wstrTemp.compare(L"") == 0)
	//		break;

	//	fin.getline(cTemp, MIN_STR); // 공백을 포함한 문장 단위(개행 단위)로 읽어오기.
	//	vPos.x = atof(cTemp);
	//	fin.getline(cTemp, MIN_STR); // 공백을 포함한 문장 단위(개행 단위)로 읽어오기.
	//	vPos.y = atof(cTemp);
	//	fin.getline(cTemp, MIN_STR);
	//	vPos.z = atof(cTemp);

	//	fin.getline(cTemp, MIN_STR);
	//	fRadius = atof(cTemp);

	//	COLL_DATA* pCollData = new COLL_DATA;

	//	wstring wstObject, wstrBone;
	//	_uint uiIdx = 0;

	//	DividString(wstrTemp, wstObject, wstrBone, uiIdx);
	//	//wstObject += L"_0";


	//	//Engine::CGameObject* pGameObject = nullptr;
	//	//pGameObject = m_pSphereColl = CSphereCollider::Create(m_pDevice, wstObject, wstrBone);
	//	//m_pSphereColl->Set_Radius(fRadius);
	//	//m_pSphereColl->Set_Position(vPos);

	//	//(*m_ppGameObjectMap).insert(make_pair(wstrTemp, pGameObject));
	//}
	//fin.close();

//}

wchar_t * CLoading::CharToWChar(const char * pstrSrc)
{
	int nLen = strlen(pstrSrc) + 1;
	wchar_t* pwstr = (LPWSTR)malloc(sizeof(wchar_t)* nLen);
	mbstowcs(pwstr, pstrSrc, nLen);
	return pwstr;

}

void CLoading::DividString(wstring wstrOrigin, wstring & wstrObject, wstring & wstrBone, _uint & uiIdx)
{
	_uint uiObjLine = 0;
	uiObjLine = wstrOrigin.find(L"_");

	_uint uiBoneLine = wstrOrigin.find(L"_", uiObjLine + 1);
	wstrObject = wstrOrigin.substr(0, uiObjLine);

	wstrBone = wstrOrigin.substr(uiObjLine + 1, uiBoneLine - uiObjLine - 1);

}


CLoading* CLoading::Create(LPDIRECT3DDEVICE9 pGraphicDev, LOADINGID eLoading)
{
	CLoading*	pInstance = new CLoading(pGraphicDev);

	if (FAILED(pInstance->Ready_Loading(eLoading)))
		Safe_Release(pInstance);

	return pInstance;

}

void CLoading::Free(void)
{
	WaitForSingleObject(m_hThread, INFINITE);
	CloseHandle(m_hThread);
	DeleteCriticalSection(&m_Crt);

	Engine::Safe_Release(m_pGraphicDev);
}

