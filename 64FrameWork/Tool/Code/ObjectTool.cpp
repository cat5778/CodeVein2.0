// ObjectTool.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "ObjectTool.h"
#include "afxdialogex.h"
#include "DirectoryMgr.h"
#include "MainFrm.h"
#include "ToolView.h"
#include "MyForm.h"
#include "StaticObject.h"
#include "DynamicObject.h"
#include "TestStage.h"
#include "DynamicCamera.h"


// CObjectTool 대화 상자입니다.

IMPLEMENT_DYNAMIC(CObjectTool, CDialogEx)

CObjectTool::CObjectTool(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{

}

CObjectTool::~CObjectTool()
{
	//for_each((*m_ppGameObjectMap).begin(), (*m_ppGameObjectMap).end(), Engine::CDeleteMap());
	//(*m_ppGameObjectMap).clear();

	//Engine::Safe_Release(m_pDevice);
	//Engine::Safe_Release(m_pDeviceClass);

}

void CObjectTool::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE_CONTROL,	m_StaticTree);
	DDX_Control(pDX, IDC_StCreateBT,	m_StaticCreateButton);
	DDX_Control(pDX, IDC_StDeleteBT,	m_StaticDeleteButton);
	DDX_Control(pDX, IDC_InstanceTree,	m_InstanceTree);
	DDX_Control(pDX, IDC_EditScaleX,	m_EditScaleX);
	DDX_Control(pDX, IDC_EditScaleY,	m_EditScaleY);
	DDX_Control(pDX, IDC_EditScaleZ,	m_EditScaleZ);
	DDX_Control(pDX, IDC_EditRotationX, m_EditRotationX);
	DDX_Control(pDX, IDC_EditRotationY, m_EditRotationY);
	DDX_Control(pDX, IDC_EditRotationZ, m_EditRotationZ);
	DDX_Control(pDX, IDC_EditPositionX, m_EditPositionX);
	DDX_Control(pDX, IDC_EditPositionY, m_EditPositionY);
	DDX_Control(pDX, IDC_EditPositionZ, m_EditPositionZ);
}

HRESULT CObjectTool::Update(const _float & fTimeDelta)
{
	m_pKeyManager->Update();
	if (m_pScene != nullptr)
	{
		int iMapSize = 0;
		if (m_ppGameObjectMap == nullptr)
			m_ppGameObjectMap =&dynamic_cast<CTestStage*>(m_pScene)->Get_Layer(L"GameLogic")->Get_ObjectMap();
		else
			iMapSize = (*m_ppGameObjectMap).size();
		if (0 < iMapSize)
			m_pCamera = dynamic_cast<CDynamicCamera*>(((*m_ppGameObjectMap).find(L"DynamicCamera")->second));
	}

	if (m_pCamera != nullptr)
	{
		if (m_pKeyManager->KeyDown(KEY_G))
			OnBnClickedMeshCreate();
		if (m_pKeyManager->KeyDown(KEY_F))
			OnBnClickedStDeleteBT();
		Move_Obj();


		if (m_pCamera->IsPick())
		{
			m_wstrSelectObject = m_pCamera->Get_PickName();
			m_pTransform = m_pCamera->Get_PickTransform();

			m_csScale[0].Format(_T("%f"), m_pTransform->m_vScale.x);
			SetDlgItemTextW(IDC_EditScaleX, m_csScale[0]);

			m_csScale[1].Format(_T("%f"), m_pTransform->m_vScale.y);
			SetDlgItemTextW(IDC_EditScaleY, m_csScale[1]);

			m_csScale[2].Format(_T("%f"), m_pTransform->m_vScale.z);
			SetDlgItemTextW(IDC_EditScaleZ, m_csScale[2]);

			m_csRotation[0].Format(_T("%f"), D3DXToDegree(m_pTransform->m_vAngle.x));
			SetDlgItemTextW(IDC_EditRotationX, m_csRotation[0]);

			m_csRotation[1].Format(_T("%f"), D3DXToDegree(m_pTransform->m_vAngle.y));
			SetDlgItemTextW(IDC_EditRotationY, m_csRotation[1]);

			m_csRotation[2].Format(_T("%f"), D3DXToDegree(m_pTransform->m_vAngle.z));
			SetDlgItemTextW(IDC_EditRotationZ, m_csRotation[2]);

			m_csPosition[0].Format(_T("%f"), m_pTransform->m_vInfo[Engine::INFO_POS].x);
			SetDlgItemTextW(IDC_EditPositionX, m_csPosition[0]);

			m_csPosition[1].Format(_T("%f"), m_pTransform->m_vInfo[Engine::INFO_POS].y);
			SetDlgItemTextW(IDC_EditPositionY, m_csPosition[1]);

			m_csPosition[2].Format(_T("%f"), m_pTransform->m_vInfo[Engine::INFO_POS].z);
			SetDlgItemTextW(IDC_EditPositionZ, m_csPosition[2]);


		}
	}
	
	return S_OK;
}


bool CObjectTool::Move_Obj()
{
	
	Engine::CGameObject* pGameObject;
	if ( (*m_ppGameObjectMap).find(m_wstrSelectObject)!=(*m_ppGameObjectMap).end())
	{
		pGameObject = (*m_ppGameObjectMap).find(m_wstrSelectObject)->second;
		Engine ::CTransform* pTransform = dynamic_cast<Engine::CTransform*>(pGameObject->Get_Component(L"Com_Transform", Engine::ID_DYNAMIC));
		_vec3 vPos = pTransform->m_vInfo[Engine::INFO_POS];
		_vec3 vRot = pTransform->m_vAngle;

		if (Engine::Get_DIKeyState(DIK_H) & 0x80)
			vPos.x -= 0.1;
		if (Engine::Get_DIKeyState(DIK_J) & 0x80)
			vPos.z -= 0.1;
		if (Engine::Get_DIKeyState(DIK_K) & 0x80)
			vPos.x += 0.1;
		if (Engine::Get_DIKeyState(DIK_U) & 0x80)
			vPos.z += 0.1;
		if (m_pKeyManager->KeyUp(KEY_Q))
			vRot.y += 90;
		if (Engine::Get_DIKeyState(DIK_Y) & 0x80)
			vRot.y -= 0.1;
		if (Engine::Get_DIKeyState(DIK_I) & 0x80)
			vRot.y += 0.1;
		if (Engine::Get_DIKeyState(DIK_SPACE) & 0x80)
			vPos.y += 0.1;
		pTransform->m_vInfo[Engine::INFO_POS] = vPos;
		pTransform->m_vAngle = vRot;
		return true;
	}
	return false; 
}

HRESULT CObjectTool::Save_Data(const TCHAR * pFilePath)
{
	//ofstream fout;
	//fout.open(wstrFilePath, ios::trunc);
	//if (fout.fail())
	//	return;

	//for (auto pObj : m_pObjList)
	//{
	//	if (pObj->wstrObjectName.compare(L"") == 0)
	//		continue;
	//	fout << CW2A(pObj->wstrObjectName.c_str()) << endl;
	//	fout << CW2A(pObj->wstrObjectKey.c_str()) << endl;
	//	fout << CW2A(pObj->wstrStateKey.c_str()) << endl;
	//	fout << pObj->IsAni << endl;
	//	fout << pObj->ImageIDX << endl;
	//	fout << pObj->eObjectType << endl;
	//}
	//fout.close();


	return S_OK;
}

HRESULT CObjectTool::Load_Data(const TCHAR * pFilePath)
{
	
	//HANDLE hFile = ::CreateFile(pFilePath, GENERIC_READ, 0, nullptr,
	//	OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

	//if (INVALID_HANDLE_VALUE == hFile)
	//	return;
	//DWORD dwBytes = 0;
	//OBJ_INFO* objInfo;
	//D3DXVECTOR3 vPos;
	//size_t iLen = 0;
	//wchar_t* pString = nullptr;

	//while (true)
	//{
	//	objInfo = new OBJ_INFO;

	//	//ReadFile(hFile, &tTile, sizeof(TILE_INFO), &dwBytes, nullptr);
	//	ReadFile(hFile, &iLen, sizeof(size_t), &dwBytes, nullptr);
	//	pString = new wchar_t[iLen];
	//	ReadFile(hFile, pString, sizeof(wchar_t) * iLen, &dwBytes, nullptr);
	//	objInfo->wstrObjectName = pString;

	//	ReadFile(hFile, &iLen, sizeof(size_t), &dwBytes, nullptr);
	//	pString = new wchar_t[iLen];
	//	ReadFile(hFile, pString, sizeof(wchar_t) * iLen, &dwBytes, nullptr);
	//	objInfo->wstrObjectKey = pString;

	//	ReadFile(hFile, &iLen, sizeof(size_t), &dwBytes, nullptr);
	//	pString = new wchar_t[iLen];
	//	ReadFile(hFile, pString, sizeof(wchar_t) * iLen, &dwBytes, nullptr);
	//	objInfo->wstrStateKey = pString;

	//	ReadFile(hFile, &objInfo->IsAni, sizeof(bool), &dwBytes, nullptr);
	//	ReadFile(hFile, &objInfo->ImageIDX, sizeof(WORD), &dwBytes, nullptr);
	//	ReadFile(hFile, &objInfo->eObjectType, sizeof(OBJECT_TYPE), &dwBytes, nullptr);
	//	ReadFile(hFile, &vPos.x, sizeof(float), &dwBytes, nullptr);
	//	ReadFile(hFile, &vPos.y, sizeof(float), &dwBytes, nullptr);
	//	ReadFile(hFile, &vPos.z, sizeof(float), &dwBytes, nullptr);


	//	if (0 == dwBytes)
	//		break;

	//	m_mObjects.insert(make_pair(objInfo, vPos));
	//}

	//CloseHandle(hFile);

	return S_OK;
}

HRESULT CObjectTool::Save_Text(const TCHAR * pFilePath)
{
	ofstream fout;
	fout.open(pFilePath, ios::trunc);
	if (fout.fail())
		return E_FAIL;
	for (auto mapItem : (*m_ppGameObjectMap))
	{
		if (mapItem.first.compare(L"") == 0|| mapItem.first.compare(L"DynamicCamera")==0|| mapItem.first.compare(L"Player") == 0)
				continue;
		Engine::CTransform* pTransform = dynamic_cast<Engine::CTransform*>(mapItem.second->Get_Component(L"Com_Transform", Engine::ID_DYNAMIC));
		
		fout << CW2A(mapItem.first.c_str()) << endl;
		fout << pTransform->m_vScale.x << endl;
		fout << pTransform->m_vScale.y << endl;
		fout << pTransform->m_vScale.z << endl;
		fout << pTransform->m_vAngle.x << endl;
		fout << pTransform->m_vAngle.y << endl;
		fout << pTransform->m_vAngle.z << endl;
		fout << pTransform->m_vInfo[Engine::INFO_POS].x << endl;
		fout << pTransform->m_vInfo[Engine::INFO_POS].y << endl;
		fout << pTransform->m_vInfo[Engine::INFO_POS].z<< endl;
	}
	fout.close();

	return S_OK;
}

HRESULT CObjectTool::Load_Text(const TCHAR * pFilePathS)
{
	ifstream fin;

	fin.open(pFilePathS);

	if (fin.fail())
		return E_FAIL;

	wstring wstrTemp;

	CString csTemp;
	char cTemp[MIN_STR];
	Transform_Info tInfo;
	while (!fin.eof())
	{
		D3DXVECTOR3 vPos;
		
		fin.getline(cTemp, MIN_STR); 
		csTemp =  cTemp;
		wstrTemp= csTemp.GetString();

		if (wstrTemp.compare(L"") == 0)
			break;
		fin.getline(cTemp, MIN_STR); // 공백을 포함한 문장 단위(개행 단위)로 읽어오기.

		tInfo.vScale.x = atof(cTemp);
		fin.getline(cTemp, MIN_STR); // 공백을 포함한 문장 단위(개행 단위)로 읽어오기.
		tInfo.vScale.y = atof(cTemp);
		fin.getline(cTemp, MIN_STR);
		tInfo.vScale.z = atof(cTemp);

		fin.getline(cTemp, MIN_STR); // 공백을 포함한 문장 단위(개행 단위)로 읽어오기.
		tInfo.vRotation.x = atof(cTemp);
		fin.getline(cTemp, MIN_STR); // 공백을 포함한 문장 단위(개행 단위)로 읽어오기.
		tInfo.vRotation.y = atof(cTemp);
		fin.getline(cTemp, MIN_STR);
		tInfo.vRotation.z = atof(cTemp);

		fin.getline(cTemp, MIN_STR); // 공백을 포함한 문장 단위(개행 단위)로 읽어오기.
		tInfo.vPosition.x = atof(cTemp);
		fin.getline(cTemp, MIN_STR); // 공백을 포함한 문장 단위(개행 단위)로 읽어오기.
		tInfo.vPosition.y = atof(cTemp);
		fin.getline(cTemp, MIN_STR);
		tInfo.vPosition.z = atof(cTemp);

		_uint uiNameCnt = wstrTemp.find_last_of(L'_');
		wstring wstrObjectName = wstrTemp.substr(0, uiNameCnt);

		_uint uiObjIdx = 0;
		uiObjIdx = _wtoi(wstrTemp.substr(uiNameCnt + 1, wstring::npos).c_str());

		Engine::CGameObject*		pGameObject = nullptr;
		pGameObject = CStaticObject::Create(m_pDevice, wstrObjectName, uiObjIdx,tInfo);


		NULL_CHECK_RETURN(pGameObject,E_FAIL);
		(*m_ppGameObjectMap).insert(make_pair(wstrTemp, pGameObject));

		m_hInstStatic = m_InstanceTree.InsertItem(wstrTemp.c_str(), 0, 0, m_hStaticRoot, TVI_LAST);


		//m_InstanceTree.get


	}
	fin.close();

	return S_OK;
}

bool CObjectTool::Ready_Mesh(MESH_PATH * pPathInfo)
{
	if (Engine::Ready_Meshes(m_pDevice,	RESOURCE_STAGE,pPathInfo->wstrObjectType.c_str(),Engine::TYPE_STATIC,pPathInfo->wstrRelative.c_str(),pPathInfo->wstrName.c_str()) < 0)
		return false;

	return 	true;	
}




BEGIN_MESSAGE_MAP(CObjectTool, CDialogEx)
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_StCreateBT, &CObjectTool::OnBnClickedMeshCreate)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_CONTROL, &CObjectTool::OnSelectMesh)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SpinScaleX, &CObjectTool::OnDeltaposSpinScaleX)
	ON_NOTIFY(TVN_SELCHANGED, IDC_InstanceTree, &CObjectTool::OnTvnSelchangedInstancetree)
	ON_EN_CHANGE(IDC_EditScaleX, &CObjectTool::OnEnChangeEditScaleX)
	ON_EN_CHANGE(IDC_EditScaleY, &CObjectTool::OnEnChangeEditScaleY)
	ON_EN_CHANGE(IDC_EditScaleZ, &CObjectTool::OnEnChangeEditScaleZ)
	ON_EN_CHANGE(IDC_EditRotationX, &CObjectTool::OnEnChangeEditRotationX)
	ON_EN_CHANGE(IDC_EditRotationY, &CObjectTool::OnEnChangeEditRotationY)
	ON_EN_CHANGE(IDC_EditRotationZ, &CObjectTool::OnEnChangeEditRotationZ)
	ON_EN_CHANGE(IDC_EditPositionX, &CObjectTool::OnEnChangeEditPositionX)
	ON_EN_CHANGE(IDC_EditPositionY, &CObjectTool::OnEnChangeEditPositionY)
	ON_EN_CHANGE(IDC_EditPositionZ, &CObjectTool::OnEnChangeEditPositionZ)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN5, &CObjectTool::OnDeltaposSpinScaleY)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN6, &CObjectTool::OnDeltaposSpinScaleZ)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN7, &CObjectTool::OnDeltaposSpinRotationX)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN8, &CObjectTool::OnDeltaposSpinRotationY)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN9, &CObjectTool::OnDeltaposSpinRotationZ)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN10, &CObjectTool::OnDeltaposSpinPositionX)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN11, &CObjectTool::OnDeltaposSpinPositionY)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN12, &CObjectTool::OnDeltaposSpinPositionZ)
	ON_BN_CLICKED(IDC_ObjectSaveButton, &CObjectTool::OnBnClickedObjectSaveButton)
	ON_BN_CLICKED(IDC_ObjectLoadButton, &CObjectTool::OnBnClickedObjectLoadButton)
	ON_BN_CLICKED(IDC_StDeleteBT, &CObjectTool::OnBnClickedStDeleteBT)
END_MESSAGE_MAP()


// CObjectTool 메시지 처리기입니다.


BOOL CObjectTool::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_hStaticMesh = m_StaticTree.InsertItem(TEXT("StaticMesh"), 0, 0, TVI_ROOT, TVI_LAST);
	m_hDynamicMesh = m_StaticTree.InsertItem(TEXT("DynamicMesh"), 0, 0, TVI_ROOT, TVI_LAST);


	if (m_pDeviceClass == nullptr)
	{
		CMainFrame* pFrameWnd = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
		
		NULL_CHECK_RETURN(pFrameWnd, L"df");

		m_pMyform = dynamic_cast<CMyForm*>(pFrameWnd->m_MainSplitter.GetPane(0, 0));
		m_pDeviceClass = m_pMyform->Get_DeviceClass();
		m_pDevice = m_pMyform->Get_Device();
		m_pScene = m_pMyform->Get_Scene();
		m_pKeyManager = CKeyMgr::GetInstance();
	}
	m_hStaticRoot = m_InstanceTree.InsertItem(TEXT("StaticObject"), 0, 0, TVI_ROOT, TVI_LAST);
	m_hDynamicRoot = m_InstanceTree.InsertItem(TEXT("DynamicObject"), 0, 0, TVI_ROOT, TVI_LAST);

	TCHAR szFileName[MAX_STR] = L"../../Resource/Mesh";

	CDirectoryMgr::ExtractPathInfo(szFileName, m_pMeshList);

	wstring wstrCombined = L"";
	TCHAR szCount[MIN_STR] = L"";

	ofstream fout;
	fout.open(L"../../Resource/Data/PathInfo.txt", ios::trunc);
	if (fout.fail())
		return E_FAIL;
	for (auto pPathInfo : m_pMeshList) //패스 저장
	{
		fout << CW2A(pPathInfo->wstrGroup.c_str()) << endl;
		fout << CW2A(pPathInfo->wstrMap.c_str()) << endl;
		fout << CW2A(pPathInfo->wstrMeshType.c_str()) << endl;
		fout << CW2A(pPathInfo->wstrName.c_str()) << endl;
		fout << CW2A(pPathInfo->wstrObjectType.c_str()) << endl;
		fout << CW2A(pPathInfo->wstrRelative.c_str()) << endl;
	}
		fout.close();


	for (auto pPathInfo : m_pMeshList)
	{
		//bool f;
		if (pPathInfo->wstrMeshType.compare(L"StaticMesh") == 0) //스태틱매쉬 인지 검사
		{
			bool bIsMap = false;
			if (m_StaticTree.ItemHasChildren(m_hStaticMesh))
			{
				m_hMap = m_StaticTree.GetChildItem(m_hStaticMesh);
				CString csMap = m_StaticTree.GetItemText(m_hMap);
				bool bIsGroup = false;
				if (csMap.Find(pPathInfo->wstrMap.c_str()) != -1) //같은 맵 있음
				{
					bIsMap = true;
					//그룹검사
					m_hGroup = m_StaticTree.GetChildItem(m_hMap);
					CString csGroup = m_StaticTree.GetItemText(m_hGroup);
					if (csGroup.Find(pPathInfo->wstrGroup.c_str()) != -1) //같은 그룹 잇음
					{
						bIsGroup = true;
						m_hFloor = m_StaticTree.InsertItem(pPathInfo->wstrObjectType.c_str(), 0, 0, m_hGroup, TVI_SORT);
					}
					if(!bIsGroup)
					{
						while (m_hGroup = m_StaticTree.GetNextSiblingItem(m_hGroup))//
						{
							csGroup = m_StaticTree.GetItemText(m_hGroup);
							if (csGroup.Find(pPathInfo->wstrGroup.c_str()) != -1) //같은 그룹 잇음
							{
								bIsGroup = true;
								m_hFloor = m_StaticTree.InsertItem(pPathInfo->wstrObjectType.c_str(), 0, 0, m_hGroup, TVI_SORT);
							}
						}
					}
					if(!bIsGroup)
					{
						m_hGroup = m_StaticTree.InsertItem(pPathInfo->wstrGroup.c_str(), 0, 0, m_hMap, TVI_SORT);
						m_hFloor = m_StaticTree.InsertItem(pPathInfo->wstrObjectType.c_str(), 0, 0, m_hGroup, TVI_SORT);
					}

				}
				if (!bIsMap) //
				{
					//while ( m_hMap = m_StaticTree.GetNextSiblingItem(m_hMap))//
					//{
						//csMap = m_StaticTree.GetItemText(m_hMap);
						//if (csMap.Find(pPathInfo->wstrMap.c_str()) != -1) //있음
						//{
							//bIsMap = true;
							m_hMap = m_StaticTree.InsertItem(pPathInfo->wstrMap.c_str(), 0, 0, m_hStaticMesh, TVI_SORT);
							m_hGroup = m_StaticTree.InsertItem(pPathInfo->wstrGroup.c_str(), 0, 0, m_hMap, TVI_SORT);
							m_hFloor = m_StaticTree.InsertItem(pPathInfo->wstrObjectType.c_str(), 0, 0, m_hGroup, TVI_SORT);
						//}

					//}
				}

			}
			else //같은 맵 없음
			{
				m_hMap = m_StaticTree.InsertItem(pPathInfo->wstrMap.c_str(), 0, 0, m_hStaticMesh, TVI_SORT);
				m_hGroup = m_StaticTree.InsertItem(pPathInfo->wstrGroup.c_str(), 0, 0, m_hMap, TVI_SORT);
				m_hFloor = m_StaticTree.InsertItem(pPathInfo->wstrObjectType.c_str(), 0, 0, m_hGroup, TVI_SORT);
			}

			Ready_Mesh(pPathInfo);//스태틱매쉬 
		}
		else if (pPathInfo->wstrMeshType.compare(L"DynamicMesh") == 0)
		{
			if (pPathInfo->wstrObjectType.compare(L"PlayerXfile") == 0)
				continue;;
			m_hEtc = m_StaticTree.InsertItem(pPathInfo->wstrObjectType.c_str(), 0, 0, m_hDynamicMesh, TVI_SORT);
			if (Engine::Ready_Meshes(m_pDevice, RESOURCE_STAGE, pPathInfo->wstrObjectType.c_str(), Engine::TYPE_DYNAMIC, pPathInfo->wstrRelative.c_str(), pPathInfo->wstrName.c_str()) < 0)
				return false;

		}
		else
		{
			//m_hEtc = m_StaticTree.InsertItem(pPathInfo->wstrObjectType.c_str(), 0, 0, m_hStaticMesh, TVI_SORT);
			
			if (Engine::Ready_Meshes(m_pDevice,
				RESOURCE_STAGE,
				pPathInfo->wstrObjectType.c_str(),
				Engine::TYPE_STATIC,
				pPathInfo->wstrRelative.c_str(),
				pPathInfo->wstrName.c_str()) < 0)
			{
				return false;
			}

		}


	}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CObjectTool::OnBnClickedMeshCreate()
{
	HTREEITEM hCurITem = m_StaticTree.GetSelectedItem();
	m_csSelectMesh = m_StaticTree.GetItemText(hCurITem);
	TRANSFORM_INFO tTemp;
	ZeroMemory(&tTemp, sizeof(TRANSFORM_INFO));
	wstring wstrName = m_csSelectMesh;
	_uint uiChildCount = 0;
	wstring wstrNameIdx;
	_bool bIsDynamic=false;
	if (m_InstanceTree.ItemHasChildren(m_hStaticRoot)) //스태틱매쉬에있는지 검사
	{
		HTREEITEM hChild = m_InstanceTree.GetChildItem(m_hStaticRoot);
		CString csText = m_InstanceTree.GetItemText(hChild);
		if (csText.Find(wstrName.c_str()) != -1) //있음
			uiChildCount++;
		while (hChild = m_InstanceTree.GetNextSiblingItem(hChild))//
		{
			CString csText = m_InstanceTree.GetItemText(hChild);
			if (csText.Find(wstrName.c_str()) != -1)
				uiChildCount++;
		}

	}
	if (m_InstanceTree.ItemHasChildren(m_hDynamicMesh)) //다이나믹매쉬에있는지 검사
	{
		HTREEITEM hChild = m_InstanceTree.GetChildItem(m_hDynamicMesh);
		CString csText = m_InstanceTree.GetItemText(hChild);
		if (csText.Find(wstrName.c_str()) != -1) //있음
			bIsDynamic = true;
		while (hChild = m_InstanceTree.GetNextSiblingItem(hChild))//
		{
			CString csText = m_InstanceTree.GetItemText(hChild);
			if (csText.Find(wstrName.c_str()) != -1)
				bIsDynamic = true;
		}
	}
	if (m_InstanceTree.ItemHasChildren(m_hDynamicRoot)) //인스턴스화된 매쉬 검사 인덱스 추출
	{
		HTREEITEM hChild = m_InstanceTree.GetChildItem(m_hDynamicRoot);
		CString csText = m_InstanceTree.GetItemText(hChild);
		if (csText.Find(wstrName.c_str()) != -1) //있음
		{
			uiChildCount++;
		}
		while (hChild = m_InstanceTree.GetNextSiblingItem(hChild))//
		{
			CString csText = m_InstanceTree.GetItemText(hChild);
			if (csText.Find(wstrName.c_str()) != -1)
				uiChildCount++;
		}

	}
	
	wstrNameIdx = wstrName + L"_" + to_wstring(uiChildCount);
	m_hInstStatic = m_InstanceTree.InsertItem(wstrNameIdx.c_str(), 0, 0, m_hStaticRoot, TVI_LAST);

	_uint uiObjIdx = 0;
	wstring wstrObjEraseIdx = wstrNameIdx;
	_uint uiEraseIdx = wstrObjEraseIdx.find_last_of(L'_');
	uiObjIdx = _wtoi(wstrObjEraseIdx.substr(uiEraseIdx + 1, wstring::npos).c_str());
	wstrObjEraseIdx.erase(uiEraseIdx);

	Engine::CGameObject*		pGameObject = nullptr;
	
	if (m_pCamera->IsPick())
	{
		TRANSFORM_INFO tInfo;
		tInfo.vPosition	= m_pCamera->Get_PickPos();
		tInfo.vRotation = { INIT_VEC3 };
		tInfo.vScale	= { DEFAULT_MESH_SCALE};

		if (bIsDynamic)
			pGameObject = CDynamicObject::Create(m_pDevice, wstrObjEraseIdx, uiObjIdx, tInfo);
		else
			pGameObject = CStaticObject::Create(m_pDevice, wstrObjEraseIdx, uiObjIdx, tInfo);

		NULL_CHECK(pGameObject);
	}
	else
	{
		if (bIsDynamic)
			pGameObject = CDynamicObject::Create(m_pDevice, wstrObjEraseIdx, uiObjIdx);
		else
			pGameObject = CStaticObject::Create(m_pDevice, wstrObjEraseIdx, uiObjIdx);

		NULL_CHECK(pGameObject);
	}
	

	(*m_ppGameObjectMap).insert(make_pair(wstrNameIdx.c_str(), pGameObject));
}


void CObjectTool::OnSelectMesh(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

	HTREEITEM hCurITem = m_StaticTree.GetSelectedItem();
	m_csSelectMesh = m_StaticTree.GetItemText(hCurITem);

	*pResult = 0;
}

void CObjectTool::OnDeltaposSpinScaleX(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	m_vScale.x = _tstof(m_csScale[0]);

	if (pNMUpDown->iDelta < 0)
		m_vScale.x+=0.001f;
	else
		m_vScale.x-=0.001f;

	m_csScale[0].Format(_T("%f"), m_vScale.x);
	SetDlgItemTextW(IDC_EditScaleX, m_csScale[0]);
	if (m_pTransform)
		m_pTransform->m_vScale.x = m_vScale.x;

	*pResult = 0;
}


void CObjectTool::OnTvnSelchangedInstancetree(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

	HTREEITEM hCurITem = m_InstanceTree.GetSelectedItem();
	m_csSelectMesh = m_InstanceTree.GetItemText(hCurITem);
	wstring wstr = m_csSelectMesh;

	for (auto &pGameObject : (*m_ppGameObjectMap))
	{
		if ( pGameObject.first.compare(wstr)==0)
		{

			m_pGameObject = pGameObject.second;
			m_pTransform = dynamic_cast<Engine::CTransform*>(m_pGameObject->Get_Component(L"Com_Transform", Engine::ID_DYNAMIC));

			m_csScale[0].Format(_T("%f"), m_pTransform->m_vScale.x);
			SetDlgItemTextW(IDC_EditScaleX, m_csScale[0]);

			m_csScale[1].Format(_T("%f"), m_pTransform->m_vScale.y);
			SetDlgItemTextW(IDC_EditScaleY, m_csScale[1]);

			m_csScale[2].Format(_T("%f"), m_pTransform->m_vScale.z);
			SetDlgItemTextW(IDC_EditScaleZ, m_csScale[2]);

			m_csRotation[0].Format(_T("%f"), D3DXToDegree(m_pTransform->m_vAngle.x));
			SetDlgItemTextW(IDC_EditRotationX, m_csRotation[0]);

			m_csRotation[1].Format(_T("%f"), D3DXToDegree(m_pTransform->m_vAngle.y));
			SetDlgItemTextW(IDC_EditRotationY, m_csRotation[1]);

			m_csRotation[2].Format(_T("%f"), D3DXToDegree(m_pTransform->m_vAngle.z));
			SetDlgItemTextW(IDC_EditRotationZ, m_csRotation[2]);

			m_csPosition[0].Format(_T("%f"), m_pTransform->m_vInfo[Engine::INFO_POS].x);
			SetDlgItemTextW(IDC_EditPositionX, m_csPosition[0]);

			m_csPosition[1].Format(_T("%f"), m_pTransform->m_vInfo[Engine::INFO_POS].y);
			SetDlgItemTextW(IDC_EditPositionY, m_csPosition[1]);

			m_csPosition[2].Format(_T("%f"), m_pTransform->m_vInfo[Engine::INFO_POS].z);
			SetDlgItemTextW(IDC_EditPositionZ, m_csPosition[2]);

		}
	}
	*pResult = 0;
}


void CObjectTool::OnEnChangeEditScaleX()
{
	GetDlgItemText(IDC_EditScaleX, m_csScale[0]);
	if (m_pTransform)
		m_pTransform->m_vScale.x= _tstof(m_csScale[0]);


}


void CObjectTool::OnEnChangeEditScaleY()
{
	GetDlgItemText(IDC_EditScaleY, m_csScale[1]);
	if (m_pTransform)
		m_pTransform->m_vScale.y = _tstof(m_csScale[1]);
}


void CObjectTool::OnEnChangeEditScaleZ()
{
	GetDlgItemText(IDC_EditScaleZ, m_csScale[2]);
	if (m_pTransform)
		m_pTransform->m_vScale.z = _tstof(m_csScale[2]);
}


void CObjectTool::OnEnChangeEditRotationX()
{
	GetDlgItemText(IDC_EditRotationX, m_csRotation[0]);
	if (m_pTransform)
		m_pTransform->m_vAngle.x = D3DXToRadian(_tstof(m_csRotation[0]));
}


void CObjectTool::OnEnChangeEditRotationY()
{
	GetDlgItemText(IDC_EditRotationY, m_csRotation[1]);
	if (m_pTransform)
		m_pTransform->m_vAngle.y = D3DXToRadian(_tstof(m_csRotation[1]));
}


void CObjectTool::OnEnChangeEditRotationZ()
{
	GetDlgItemText(IDC_EditRotationZ, m_csRotation[2]);
	if (m_pTransform)
		m_pTransform->m_vAngle.z = D3DXToRadian(_tstof(m_csRotation[2]));

}


void CObjectTool::OnEnChangeEditPositionX()
{
	GetDlgItemText(IDC_EditPositionX, m_csPosition[0]);
	if (m_pTransform)
		m_pTransform->m_vInfo[Engine::INFO_POS].x = _tstof(m_csPosition[0]);
}


void CObjectTool::OnEnChangeEditPositionY()
{
	GetDlgItemText(IDC_EditPositionY, m_csPosition[1]);
	if (m_pTransform)
		m_pTransform->m_vInfo[Engine::INFO_POS].y = _tstof(m_csPosition[1]);
}


void CObjectTool::OnEnChangeEditPositionZ()
{
	GetDlgItemText(IDC_EditPositionZ, m_csPosition[2]);
	if (m_pTransform)
		m_pTransform->m_vInfo[Engine::INFO_POS].z = _tstof(m_csPosition[2]);
}


void CObjectTool::OnOK()
{
}


void CObjectTool::OnCancel()
{

	//CDialogEx::OnCancel();
}


void CObjectTool::OnDeltaposSpinScaleY(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	m_vScale.y = _tstof(m_csScale[1]);

	if (pNMUpDown->iDelta < 0)
		m_vScale.y += 0.001f;
	else
		m_vScale.y -= 0.001f;

	m_csScale[1].Format(_T("%f"), m_vScale.y);
	SetDlgItemTextW(IDC_EditScaleY, m_csScale[1]);
	if (m_pTransform)
		m_pTransform->m_vScale.y = m_vScale.y;

	*pResult = 0;
}


void CObjectTool::OnDeltaposSpinScaleZ(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	m_vScale.z = _tstof(m_csScale[2]);

	if (pNMUpDown->iDelta < 0)
		m_vScale.z += 0.001f;
	else
		m_vScale.z -= 0.001f;

	m_csScale[2].Format(_T("%f"), m_vScale.z);
	SetDlgItemTextW(IDC_EditScaleZ, m_csScale[2]);
	if (m_pTransform)
		m_pTransform->m_vScale.z = m_vScale.z;

	*pResult = 0;
}


void CObjectTool::OnDeltaposSpinRotationX(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	m_vRotation.x = _tstof(m_csRotation[0]);

	if (pNMUpDown->iDelta < 0)
		m_vRotation.x += 0.1f;
	else
		m_vRotation.x -= 0.1f;

	m_csRotation[0].Format(_T("%f"), m_vRotation.x);
	SetDlgItemTextW(IDC_EditRotationX, m_csRotation[0]);
	if (m_pTransform)
		m_pTransform->m_vAngle.x = m_vRotation.x;

	*pResult = 0;
}


void CObjectTool::OnDeltaposSpinRotationY(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	m_vRotation.y = _tstof(m_csRotation[1]);

	if (pNMUpDown->iDelta < 0)
		m_vRotation.y += 0.1f;
	else
		m_vRotation.y -= 0.1f;

	m_csRotation[1].Format(_T("%f"), m_vRotation.y);
	SetDlgItemTextW(IDC_EditRotationY, m_csRotation[1]);
	if (m_pTransform)
		m_pTransform->m_vAngle.y = m_vRotation.y;

	*pResult = 0;
}


void CObjectTool::OnDeltaposSpinRotationZ(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	m_vRotation.z = _tstof(m_csRotation[2]);

	if (pNMUpDown->iDelta < 0)
		m_vRotation.z += 0.1f;
	else
		m_vRotation.z -= 0.1f;

	m_csRotation[2].Format(_T("%f"),m_vRotation.z);
	SetDlgItemTextW(IDC_EditRotationZ, m_csRotation[2]);
	if (m_pTransform)
		m_pTransform->m_vAngle.z = m_vRotation.z;
	*pResult = 0;
}


void CObjectTool::OnDeltaposSpinPositionX(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	m_vPosition.x = _tstof(m_csPosition[0]);

	if (pNMUpDown->iDelta < 0)
		m_vPosition.x += 0.1f;
	else
		m_vPosition.x -= 0.1f;

	m_csPosition[0].Format(_T("%f"), m_vPosition.x);
	SetDlgItemTextW(IDC_EditPositionX, m_csPosition[0]);
	if (m_pTransform)
		m_pTransform->m_vInfo[Engine::INFO_POS].x = m_vPosition.x;
	*pResult = 0;
}


void CObjectTool::OnDeltaposSpinPositionY(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	m_vPosition.y = _tstof(m_csPosition[1]);

	if (pNMUpDown->iDelta < 0)
		m_vPosition.y += 0.1f;
	else
		m_vPosition.y -= 0.1f;

	m_csPosition[1].Format(_T("%f"), m_vPosition.y);
	SetDlgItemTextW(IDC_EditPositionY, m_csPosition[1]);
	if (m_pTransform)
		m_pTransform->m_vInfo[Engine::INFO_POS].y = m_vPosition.y;
	*pResult = 0;
}


void CObjectTool::OnDeltaposSpinPositionZ(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	m_vPosition.z = _tstof(m_csPosition[2]);

	if (pNMUpDown->iDelta < 0)
		m_vPosition.z += 0.1f;
	else
		m_vPosition.z -= 0.1f;

	m_csPosition[2].Format(_T("%f"), m_vPosition.z);
	SetDlgItemTextW(IDC_EditPositionZ, m_csPosition[2]);
	if (m_pTransform)
		m_pTransform->m_vInfo[Engine::INFO_POS].z = m_vPosition.z;
	*pResult = 0;
}


void CObjectTool::OnBnClickedObjectSaveButton()
{
	CFileDialog dlgFile(FALSE, L".txt", L".txt", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"Text Files(*.txt)|*.txt||Data Files(*.dat) | *.dat | ",this);
	
	dlgFile.m_ofn.lpstrTitle = L"Object Save";

	_tchar szPath[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, szPath);
	::PathRemoveFileSpec(szPath);
	::PathRemoveFileSpec(szPath);
	CString csDataPath = szPath;
	csDataPath += ".\\Resource.\\Data";
	dlgFile.m_ofn.lpstrInitialDir = csDataPath;
	if (IDOK == dlgFile.DoModal())
	{
		Save_Text(dlgFile.GetPathName());
		//CString pathName = dlgFile.GetPathName();
		//MessageBox(pathName);

	}
}


void CObjectTool::OnBnClickedObjectLoadButton()
{
	CFileDialog dlgFile(TRUE, L".txt", L".txt", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"Text Files(*.txt)|*.txt||Data Files(*.dat) | *.dat | ", this);

	dlgFile.m_ofn.lpstrTitle = L"Object Load";

	_tchar szPath[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, szPath);
	::PathRemoveFileSpec(szPath);
	::PathRemoveFileSpec(szPath);
	CString csDataPath = szPath;
	csDataPath += ".\\Resource.\\Data";
	dlgFile.m_ofn.lpstrInitialDir = csDataPath;

	if (IDOK == dlgFile.DoModal())
	{
		Load_Text(dlgFile.GetPathName());

	}
}


void CObjectTool::OnBnClickedStDeleteBT()
{

	//TODO: 맵 피킹 오류때문에 막아놓음 사용시 다시켜야함
	//wstring wstrInstName = m_pCamera->Get_PickName();

	//if (wstrInstName.compare(L"No Pick")==0)
	//	return;

	wstring wstrInstName = m_csSelectMesh; //맵치킹 떄문에 해놓음 다시 주석처리해야함

	if (dynamic_cast<CStaticObject*>((*m_ppGameObjectMap).find(wstrInstName)->second) != nullptr)
	{
		if (m_InstanceTree.ItemHasChildren(m_hStaticRoot))
		{
			HTREEITEM hChild = m_InstanceTree.GetChildItem(m_hStaticRoot);
			CString csText = m_InstanceTree.GetItemText(hChild);
			if (csText.Compare(wstrInstName.c_str()) == 0) //있음
				m_InstanceTree.DeleteItem(hChild);
			while (hChild = m_InstanceTree.GetNextSiblingItem(hChild))//
			{
				csText = m_InstanceTree.GetItemText(hChild);
				if (csText.Compare(wstrInstName.c_str()) == 0) //있음
					m_InstanceTree.DeleteItem(hChild);
			}
		}

	}
	
	Engine::Safe_Release((*m_ppGameObjectMap).find(wstrInstName)->second);
	(*m_ppGameObjectMap).erase(wstrInstName);
	m_pCamera->Init_PickName();

}
