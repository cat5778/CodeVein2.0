#include "stdafx.h"
#include "TestStage.h"
#include "MainFrm.h"
#include "Export_Function.h"
#include "Terrain.h"
#include "Player.h"
#include "StaticObject.h"
#include "DynamicCamera.h"
#include "Sword.h"
#include "Cell.h"
CTestStage::CTestStage(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CScene(pGraphicDev)
{
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(5356);
}

CTestStage::~CTestStage(void)
{


}

HRESULT CTestStage::Ready_Scene(void)
{
	FAILED_CHECK_RETURN(Ready_Resource(m_pGraphicDev, RESOURCE_END), E_FAIL);
	FAILED_CHECK_RETURN(Engine::CScene::Ready_Scene(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_GameLogic_Layer(L"GameLogic"), E_FAIL);
	return S_OK;
}

HRESULT CTestStage::LateReady_Scene(void)
{
	FAILED_CHECK_RETURN(Engine::CScene::LateReady_Scene(), E_FAIL);

	//m_pCamera = dynamic_cast<Engine::CCamera*>(Engine::Get_GameObject(L"GameLogic", L"DynamicCamera"));
	//NULL_CHECK_RETURN(m_pCamera, E_FAIL);

	//if (pCamera->Get_IsMainCamera())
	Engine::Get_Renderer()->Set_Perspective(*m_pCamera->Get_Projection());

	return S_OK;
}

_int CTestStage::Update_Scene(const _float& fTimeDelta)
{
	m_fTime += fTimeDelta;

	if (m_pCamera->IsPick())
	{

	}
	//m_pKeyMgr->Update();
	Engine::CScene::Update_Scene(fTimeDelta);

	return S_OK;
}

void CTestStage::Render_Scene(void)
{
	POINT ptMousePos;
	_vec3 vPickPos = { 0.f,0.f,0.f };
	GetCursorPos(&ptMousePos);
	ScreenToClient(g_hWnd,&ptMousePos);
	swprintf_s(m_szMousePos, L"ScrollPos X=: %d  , Y= %d ", ptMousePos.x, ptMousePos.y);

	++m_dwRenderCnt;

	if (m_fTime >= 1.0f)
	{
		wsprintf(m_szFPS, L"FPS : %d", m_dwRenderCnt);
		m_fTime = 0.f;
		m_dwRenderCnt = 0;
	}

	Engine::Render_Font(L"Font_Second", m_szFPS, &_vec2(10.f, 40.f), D3DXCOLOR(0.f, 1.f, 0.f, 1.f));
	wsprintf(m_szMousePos, L"Mouse Pos X= %d Y= %d ", ptMousePos.x, ptMousePos.y);
	Engine::Render_Font(L"Font_Second", m_szMousePos, &_vec2(10.f, 20.f), D3DXCOLOR(0.f, 1.f, 0.f, 1.f));

	vPickPos = m_pCamera->Get_PickPos();
	_stprintf_s(m_szMousePos, L"CurPickPos X= %.3f Y= %.3f Z=%.3f", vPickPos.x, vPickPos.y, vPickPos.z);
	Engine::Render_Font(L"Font_Second", m_szMousePos, &_vec2(10.f, 60.f), D3DXCOLOR(0.f, 1.f, 0.f, 1.f));

	//wstring Temp = m_pCamera->Get_PickName();
	//wstrPickObjName += Temp;

	wstring wstrPickObjName =L"ObjectName= " + m_pCamera->Get_PickName();
	Engine::Render_Font(L"Font_Second", wstrPickObjName.c_str(), &_vec2(10.f, 80.f), D3DXCOLOR(0.f, 1.f, 0.f, 1.f));

	//m_pGraphicDev->DrawTriPatch

}

CTestStage* CTestStage::Create(LPDIRECT3DDEVICE9 pGraphicDev, CToolView* m_pView)
{
	CTestStage*	pInstance = new CTestStage(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
		Engine::Safe_Release(pInstance);
	pInstance->m_pView = m_pView;
	return pInstance;
}

void CTestStage::Free(void)
{
	Engine::CScene::Free();
}

HRESULT CTestStage::Ready_GameLogic_Layer(const _tchar* pLayerTag)
{

	m_pLayer= Engine::CLayer::Create();
	NULL_CHECK_RETURN(m_pLayer, E_FAIL);

	m_pColLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(m_pColLayer, E_FAIL);

	Engine::CGameObject*		pGameObject = nullptr;
	//
	//// Terrain
	//pGameObject = CTerrain::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(m_pLayer->Add_GameObject(L"Terrain", pGameObject), E_FAIL);
	//

	pGameObject = m_pCamera = CDynamicCamera::Create(m_pGraphicDev,
		&_vec3(0.f, 5.f, -5.f),
		&_vec3(0.f, 0.f, 0.f),
		&_vec3(0.f, 1.f, 0.f),
		D3DXToRadian(60.f),
		_float(WINCX) / WINCY,
		1.f,
		1000.f,
		true);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(m_pLayer->Add_GameObject(L"DynamicCamera", pGameObject), E_FAIL);

	// Player
	pGameObject = CPlayer::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(m_pLayer->Add_GameObject(L"Player", pGameObject), E_FAIL);



	////Sword
	//pGameObject = CSword::Create(m_pGraphicDev, 0);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Sword", pGameObject), E_FAIL);


	m_pLayerMap.emplace(pLayerTag, m_pLayer);
	return S_OK;
}

HRESULT CTestStage::Ready_Resource(LPDIRECT3DDEVICE9& pGraphicDev, RESOURCEID eMax)
{
	// 메모리 예약
	Engine::Reserve_ContainerSize(eMax);



	FAILED_CHECK_RETURN(Engine::Ready_Buffer(pGraphicDev,
		RESOURCE_STATIC,
		L"Buffer_RcTex",
		Engine::BUFFER_RCTEX),
		E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Buffer(pGraphicDev,
		RESOURCE_STATIC,
		L"Buffer_CubeTex",
		Engine::BUFFER_CUBETEX),
		E_FAIL);
	
	FAILED_CHECK_RETURN(Engine::Ready_Buffer(pGraphicDev,
		RESOURCE_STATIC,
		L"Buffer_TriCol",
		Engine::BUFFER_TRICOL),
		E_FAIL);
	//FAILED_CHECK_RETURN(Engine::Ready_Buffer(pGraphicDev,
	//	RESOURCE_STATIC,
	//	L"Buffer_LightTex",
	//	Engine::BUFFER_LIGHTTEX),
	//	E_FAIL);

	//FAILED_CHECK_RETURN(Engine::Ready_Buffer(pGraphicDev,
	//	RESOURCE_STATIC,
	//	L"Tool_Buffer",
	//	Engine::BUFFER_TOOLTEX),
	//	E_FAIL);
	//	
	//FAILED_CHECK_RETURN(Engine::Ready_Buffer(pGraphicDev,
	//										RESOURCE_STATIC,
	//										L"Buffer_TerrainTex",
	//										Engine::BUFFER_TERRAINTEX,
	//										VTXCNTX,
	//										VTXCNTZ,
	//										VTXITV),
	//										E_FAIL);

	//// DebugMesh
	//FAILED_CHECK_RETURN(Engine::Ready_DebugMesh(m_pGraphicDev), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Navi",
		Engine::TYPE_NAVI,
		NULL,
		NULL),
		E_FAIL);

	//// 텍스쳐
	//FAILED_CHECK_RETURN(Engine::Ready_Texture(pGraphicDev,
	//	RESOURCE_STAGE,
	//	L"Texture_Test",
	//	Engine::TEX_NORMAL,
	//	L"../../Resource/Texture/Terrain/Terrain0.png"),
	//	E_FAIL);

	//// Stone
	//FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
	//	RESOURCE_STAGE,
	//	L"Mesh_Stone",
	//	Engine::TYPE_STATIC,
	//	L"../../Client/Bin/Resource/Mesh/StaticMesh/TombStone/",
	//	L"TombStone.X"),
	//	E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Player",
		Engine::TYPE_DYNAMIC,
		L"../../Resource/Mesh/DynamicMesh/PlayerXfile/",
		L"Player.X"),
		E_FAIL);

	//FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
	//	RESOURCE_STAGE,
	//	L"Mesh_Sword",
	//	Engine::TYPE_STATIC,
	//	L"../../Client/Bin/Resource/Mesh/StaticMesh/Sword/",
	//	L"Sword.X"),
	//	E_FAIL);


	return S_OK;
}
HRESULT CTestStage::Add_StaticObject(wstring wstrObjName, TRANSFORM_INFO tInfo)
{
	_uint uiObjIdx = 0;
	wstring wstrObjEraseIdx= wstrObjName;
	_uint uiEraseIdx = wstrObjEraseIdx.find_last_of(L'_');
	uiObjIdx = _wtoi(wstrObjEraseIdx.substr(uiEraseIdx + 1, wstring::npos).c_str());
	wstrObjEraseIdx.erase(uiEraseIdx);
	
	Engine::CGameObject*		pGameObject = nullptr;
	pGameObject = CStaticObject::Create(m_pGraphicDev, wstrObjEraseIdx, uiObjIdx);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(m_pLayer->Add_GameObject(wstrObjName.c_str(), pGameObject), E_FAIL);



	return S_OK;
}
void CTestStage::Set_MeshVec(vector<NavMesh*>& pMeshVec)
{
	m_pNavMeshVec = pMeshVec;
	
}
//
//HRESULT CTestStage::Add_GameObject(Engine::OBJECTTYPE objType, int monsterType, const _tchar * pObjName, const _tchar* pObjTypeName)
//{
	//Engine::CGameObject*		pGameObject = nullptr;

	//pGameObject = CTempObject::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//pGameObject->Set_ObjType(objType);
	//pGameObject->Set_ObjName(pObjTypeName);
	//if (objType == Engine::OBJ_MONSTER)
	//	pGameObject->Set_MonsterType((Engine::MONSTERTYPE)monsterType);
	//else if (objType == Engine::OBJ_ITEM)
	//	pGameObject->Set_ItemType((Engine::ITEMTYPE)monsterType);
	//dynamic_cast<CTempObject*>(pGameObject)->SetTestTexure(objType);
	//FAILED_CHECK_RETURN(m_pLayer->Add_GameObject(pObjName, pGameObject), E_FAIL);
	//m_mapLayer.emplace(L"GameObject", m_pLayer);
	//return S_OK;
//}
//
//
//
//HRESULT CTestStage::Copy_GameObject(Engine::CGameObject * object, const _tchar * pObjName)
//{
//	Engine::CGameObject*		pGameObject = nullptr;
//
//	pGameObject = CTempObject::Create(m_pGraphicDev, object);
//	NULL_CHECK_RETURN(pGameObject, E_FAIL);
//	dynamic_cast<CTempObject*>(pGameObject)->SetTestTexure(pGameObject->Get_ObjType());
//	FAILED_CHECK_RETURN(m_pLayer->Add_GameObject(pObjName, pGameObject), E_FAIL);
//
//	m_mapLayer.emplace(L"GameObject", m_pLayer);
//
//	return S_OK;
//}
//
//HRESULT CTestStage::Copy_GameObject(Engine::CGameObject * object, const _tchar * pObjName, _vec3 tempPos)
//{
//	Engine::CGameObject*		pGameObject = nullptr;
//
//	pGameObject = CTempObject::Create(m_pGraphicDev, object);
//	NULL_CHECK_RETURN(pGameObject, E_FAIL);
//	dynamic_cast<CTempObject*>(pGameObject)->SetTestTexure(pGameObject->Get_ObjType());
//	pGameObject->GetTransformCom()->m_vInfo[Engine::INFO_POS] += tempPos;
//	FAILED_CHECK_RETURN(m_pLayer->Add_GameObject(pObjName, pGameObject), E_FAIL);
//
//	m_mapLayer.emplace(L"GameObject", m_pLayer);
//
//	return S_OK;
//}



//HRESULT CTestStage::Add_ColObject(const _tchar * pObjName)
//{
//	Engine::CGameObject*		pGameObject = nullptr;
//
//	pGameObject = CColObject::Create(m_pGraphicDev);
//	NULL_CHECK_RETURN(pGameObject, E_FAIL);
//
//	FAILED_CHECK_RETURN(m_pColLayer->Add_GameObject(pObjName, pGameObject), E_FAIL);
//	m_mapLayer.emplace(L"ColObject", m_pColLayer);
//}
//
//void CTestStage::Save(const TCHAR * pFilePath)
//{
//	CToolView* pToolView = (CToolView*)((CMainFrame*)(AfxGetApp()->m_pMainWnd))->GetActiveView();
//
//	HANDLE hFile = ::CreateFile(pFilePath, GENERIC_WRITE, 0, nullptr,
//		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
//
//	if (INVALID_HANDLE_VALUE == hFile)
//		return;
//	DWORD dwBytes = 0;
//	for (int i = 0; i < Engine::OBJ_END; i++)
//	{
//		WriteFile(hFile, &pToolView->m_DObj.m_iObjCount[i], sizeof(int), &dwBytes, nullptr);
//		WriteFile(hFile, &pToolView->m_DObj.m_iObjNameCount[i], sizeof(int), &dwBytes, nullptr);
//	}
//
//	auto iter = m_mapLayer.begin();
//	for (; iter != m_mapLayer.end(); iter++)
//	{
//		if (wcscmp(iter->first.c_str(), L"GameObject") == 0)
//		{
//			iter->second->Save_Object(hFile, dwBytes, pFilePath, true);
//			break;
//		}
//	}
//}
//
//void CTestStage::Load(const TCHAR * pFilePath, int &count)
//{
//	bool flag = false;
//	auto iter = m_mapLayer.begin();
//	CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
//	pMain->m_wndClassView.ClearGameObject();
//	for (; iter != m_mapLayer.end();)
//	{
//
//		if (wcscmp(iter->first.c_str(), L"GameObject") == 0)
//		{
//			Engine::Safe_Release(iter->second);
//			Engine::Safe_Release(m_pLayer);
//			iter = m_mapLayer.erase(iter);
//
//			m_pLayer = Engine::CLayer::Create();
//			flag = true;
//			break;
//		}
//		else
//		{
//			iter++;
//		}
//	}
//	if (!flag)
//	{
//		if (m_pLayer == nullptr)
//		{
//			m_pLayer = Engine::CLayer::Create();
//		}
//	}
//	CToolView* pToolView = (CToolView*)((CMainFrame*)(AfxGetApp()->m_pMainWnd))->GetActiveView();
//
//	HANDLE hFile = ::CreateFile(pFilePath, GENERIC_READ, 0, nullptr,
//		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
//
//	if (INVALID_HANDLE_VALUE == hFile)
//		return;
//	DWORD dwBytes = 0;
//	int tempCount = 0;
//	for (int i = 0; i < Engine::OBJ_END; i++)
//	{
//		ReadFile(hFile, &pToolView->m_DObj.m_iObjCount[i], sizeof(int), &dwBytes, nullptr);
//		ReadFile(hFile, &pToolView->m_DObj.m_iObjNameCount[i], sizeof(int), &dwBytes, nullptr);
//		tempCount += pToolView->m_DObj.m_iObjCount[i];
//	}
//
//	m_pLayer->Load_Object(hFile, dwBytes, pFilePath, count);
//
//	auto iter_Layer = m_pLayer->GetMapObj().begin();
//	for (int i = 0; i < tempCount; i++)
//	{
//		Engine::CGameObject*		pGameObject = nullptr;
//
//
//		pGameObject = CTempObject::Create(m_pGraphicDev);
//		NULL(pGameObject, E_FAIL);
//		iter_Layer->second = pGameObject->Load_Component(dwBytes, hFile, true);
//		pMain->m_wndClassView.UpdateList(iter_Layer->second->Get_ObjType(), iter_Layer->second->Get_MonsterType(), iter_Layer->first.c_str());
//		dynamic_cast<CTempObject*>(pGameObject)->SetTestTexure(iter_Layer->second->Get_ObjType());
//		iter_Layer++;
//
//	}
//	CloseHandle(hFile);
//	m_mapLayer.emplace(L"GameObject", m_pLayer);
//}
//
//void CTestStage::Col_Save(const TCHAR * pFilePath)
//{
//	CToolView* pToolView = (CToolView*)((CMainFrame*)(AfxGetApp()->m_pMainWnd))->GetActiveView();
//
//	HANDLE hFile = ::CreateFile(pFilePath, GENERIC_WRITE, 0, nullptr,
//		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
//
//	if (INVALID_HANDLE_VALUE == hFile)
//		return;
//	DWORD dwBytes = 0;
//
//	auto iter = m_mapLayer.begin();
//	for (; iter != m_mapLayer.end(); iter++)
//	{
//		if (wcscmp(iter->first.c_str(), L"ColObject") == 0)
//		{
//			iter->second->Save_Object(hFile, dwBytes, pFilePath, false);
//			break;
//		}
//	}
//}
//
//void CTestStage::Col_Load(const TCHAR * pFilePath, int & count)
//{
//	bool flag = false;
//	auto iter = m_mapLayer.begin();
//	CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
//	pMain->m_wndClassView.ClearColObject();
//	for (; iter != m_mapLayer.end();)
//	{
//		if (wcscmp(iter->first.c_str(), L"ColObject") == 0)
//		{
//			Engine::Safe_Release(iter->second);
//			Engine::Safe_Release(m_pColLayer);
//			iter = m_mapLayer.erase(iter);
//
//			m_pColLayer = Engine::CLayer::Create();
//			flag = true;
//			break;
//		}
//		else
//		{
//			iter++;
//		}
//	}
//	if (!flag)
//	{
//		if (m_pColLayer == nullptr)
//		{
//			m_pColLayer = Engine::CLayer::Create();
//		}
//	}
//	CToolView* pToolView = (CToolView*)((CMainFrame*)(AfxGetApp()->m_pMainWnd))->GetActiveView();
//
//	HANDLE hFile = ::CreateFile(pFilePath, GENERIC_READ, 0, nullptr,
//		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
//
//	if (INVALID_HANDLE_VALUE == hFile)
//		return;
//	DWORD dwBytes = 0;
//
//	m_pColLayer->Load_Object(hFile, dwBytes, pFilePath, count);
//
//	auto iter_Layer = m_pColLayer->GetMapObj().begin();
//	for (int i = 0; i < m_pColLayer->GetMapObj().size(); i++)
//	{
//		Engine::CGameObject*		pGameObject = nullptr;
//
//		pGameObject = CColObject::Create(m_pGraphicDev);
//		NULL(pGameObject, E_FAIL);
//		iter_Layer->second = pGameObject->Load_Component(dwBytes, hFile, false);
//		pMain->m_wndClassView.UpdateColList(iter_Layer->first.c_str());
//		iter_Layer++;
//
//	}
//	CloseHandle(hFile);
//	m_mapLayer.emplace(L"ColObject", m_pColLayer);
//}
//
//HRESULT CTestStage::Delete_Object(const _tchar * pObjectName, int &count)
//{
//	Engine::CGameObject* tempObject = Get_GameObject(L"GameObject", pObjectName);
//
//	auto iter = m_mapLayer.begin();
//	for (; iter != m_mapLayer.end(); iter++)
//	{
//		if (wcscmp(iter->first.c_str(), L"GameObject") == 0)
//		{
//			auto iter_Layer = iter->second->GetMapObj().begin();
//			for (; iter_Layer != iter->second->GetMapObj().end(); iter_Layer++)
//			{
//				if (iter_Layer->second == tempObject)
//				{
//					CMainFrame * pMain = (CMainFrame*)AfxGetMainWnd();
//					pMain->m_wndClassView.DeleteList(iter_Layer->first.c_str());
//					Engine::Safe_Release(iter_Layer->second);
//					iter->second->GetMapObj().erase(iter_Layer->first);
//
//					count--;
//					return S_OK;
//				}
//			}
//		}
//	}
//
//
//	return E_FAIL;
//}
//
//HRESULT CTestStage::Delete_Col_Object(const _tchar * pObjectName, int & count)
//{
//	Engine::CGameObject* tempObject = Get_GameObject(L"ColObject", pObjectName);
//
//	auto iter = m_mapLayer.begin();
//	for (; iter != m_mapLayer.end(); iter++)
//	{
//		if (wcscmp(iter->first.c_str(), L"ColObject") == 0)
//		{
//			auto iter_Layer = iter->second->GetMapObj().begin();
//			for (; iter_Layer != iter->second->GetMapObj().end(); iter_Layer++)
//			{
//				if (iter_Layer->second == tempObject)
//				{
//					CMainFrame * pMain = (CMainFrame*)AfxGetMainWnd();
//					pMain->m_wndClassView.DeleteList(iter_Layer->first.c_str());
//					Engine::Safe_Release(iter_Layer->second);
//					iter->second->GetMapObj().erase(iter_Layer->first);
//
//					count--;
//					return S_OK;
//				}
//			}
//		}
//	}
//
//}
