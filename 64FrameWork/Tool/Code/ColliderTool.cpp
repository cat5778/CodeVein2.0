// ../Code/ColliderTool.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "../Code/ColliderTool.h"
#include "afxdialogex.h"
#include "MainFrm.h"
#include "ToolView.h"
#include "MyForm.h"
#include "StaticObject.h"
#include "DynamicObject.h"
#include "TestStage.h"
#include "DynamicCamera.h"
#include "SphereCollider.h"
#include "ColliderManager.h"

// CColliderTool 대화 상자입니다.

IMPLEMENT_DYNAMIC(CColliderTool, CDialogEx)

CColliderTool::CColliderTool(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG4, pParent)
{

}

CColliderTool::~CColliderTool()
{
}

void CColliderTool::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DyMeshTREE, m_ObjectTree);
	DDX_Control(pDX, IDC_BoneTREE, m_BoneTree);
	DDX_Control(pDX, IDC_EditPosX, m_EditPositionX);
	DDX_Control(pDX, IDC_EditPosY, m_EditPositionY);
	DDX_Control(pDX, IDC_EditPosZ, m_EditPositionZ);
	DDX_Control(pDX, IDC_EditRadius, m_EditRadius);
	DDX_Control(pDX, IDC_EDITAnim, m_EditAnim);
	DDX_Control(pDX, IDC_ColliderTree, m_ColliderTree);
	DDX_Control(pDX, IDC_CHECKBone, m_CheckBone);
	DDX_Control(pDX, IDC_EDITOption, m_EditOption);
}


BEGIN_MESSAGE_MAP(CColliderTool, CDialogEx)
	ON_BN_CLICKED(IDC_ColliDeleteButton, &CColliderTool::OnBnClickedCollideleteButton)
	ON_BN_CLICKED(IDC_ClliderCreateButton, &CColliderTool::OnBnClickedClliderCreateButton)
	ON_BN_CLICKED(IDC_ColliderSaveButton, &CColliderTool::OnBnClickedColliderSaveButton)
	ON_BN_CLICKED(IDC_ColliderLoadButton, &CColliderTool::OnBnClickedColliderLoadButton)
	ON_NOTIFY(TVN_SELCHANGED, IDC_DyMeshTREE, &CColliderTool::OnTvnSelchangedDymeshTree)
	ON_NOTIFY(TVN_SELCHANGED, IDC_BoneTREE, &CColliderTool::OnTvnSelchangedBoneTree)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPINPosX, &CColliderTool::OnDeltaposSpinPosX)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPINPosY, &CColliderTool::OnDeltaposSpinPosY)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPINPosZ, &CColliderTool::OnDeltaposSpinPosZ)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPINRadius, &CColliderTool::OnDeltaposSpinRadius)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPINAnim, &CColliderTool::OnDeltaposSpinAnim)
	ON_EN_CHANGE(IDC_EditRadius, &CColliderTool::OnEnChangeEditRadius)
	ON_EN_CHANGE(IDC_EDITBone, &CColliderTool::OnEnChangeEditBone)
	ON_NOTIFY(TVN_SELCHANGED, IDC_ColliderTree, &CColliderTool::OnTvnSelchangedColliderTree)
	ON_EN_CHANGE(IDC_EDITOption, &CColliderTool::OnEnChangeEditOption)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPINOption, &CColliderTool::OnDeltaposSpinOption)
END_MESSAGE_MAP()


// CColliderTool 메시지 처리기입니다.



void CColliderTool::OnBnClickedCollideleteButton()
{
}


void CColliderTool::OnBnClickedClliderCreateButton()
{
	Engine::CGameObject* pGameObject=nullptr;
	if (m_CheckBone.GetCheck())
	{
		m_wstrSelectBone = m_csEditBone;
		//pGameObject = m_pSphereColl = CSphereCollider::Create(m_pDevice, m_wstrSelectObject, m_wstrSelectBone);
		
		m_pSphereCol=m_pColliderGroup->Add_Collider((Engine::COLLOPTION)m_uiOption, m_wstrSelectBone, 15.f);

	}
	else
	{
		m_pSphereCol=m_pColliderGroup->Add_Collider((Engine::COLLOPTION)m_uiOption, m_wstrSelectBone, 15.f);

		//pGameObject = m_pSphereColl = CSphereCollider::Create(m_pDevice, m_wstrSelectObject, m_wstrSelectBone);
	}
	//_uint uiIdx = 0;
	wstring wstrColl = m_pSphereCol->Get_CollTag();
	//
	//wstrColl += L"_" + m_wstrSelectBone + L"_SphereCollider" + L"_" + to_wstring(uiIdx);

	//while ((*m_ppGameObjectMap).find(wstrColl) != (*m_ppGameObjectMap).end())
	//{
	//	wstrColl = m_csSelectMesh;
	//	uiIdx++;
	//	wstrColl +=  L"_"+ m_wstrSelectBone + L"_SphereCollider" + L"_" + to_wstring(uiIdx);
	//}


	//(*m_ppGameObjectMap).insert(make_pair(wstrColl, pGameObject));
	m_vPosition = *m_pSphereCol->Get_Pos();
	m_csPosition[0].Format(_T("%f"),m_vPosition.x);
	m_csPosition[1].Format(_T("%f"),m_vPosition.y);
	m_csPosition[2].Format(_T("%f"),m_vPosition.z);

	SetDlgItemTextW(IDC_EditPosX, m_csPosition[0]);
	SetDlgItemTextW(IDC_EditPosY, m_csPosition[1]);
	SetDlgItemTextW(IDC_EditPosZ, m_csPosition[2]);

	_bool bIsObject=false;
	if (m_ColliderTree.GetCount()>0)
	{ 
		CString csObject= m_ColliderTree.GetItemText(m_hObject);
		if (csObject.Find(m_wstrSelectObject.c_str()) != -1) //같은 그룹 잇음
		{
			bIsObject = true;
			m_hBone = m_ColliderTree.InsertItem(wstrColl.c_str(), 0, 0, m_hObject, TVI_SORT);
		}
		if (!bIsObject)
		{
			while (m_hObject = m_ColliderTree.GetNextSiblingItem(m_hObject))//
			{
				csObject= m_ColliderTree.GetItemText(m_hObject);
				if (csObject.Find(m_wstrSelectObject.c_str()) != -1) //같은 그룹 잇음
				{
					bIsObject= true;
					m_hBone = m_ColliderTree.InsertItem(wstrColl.c_str(), 0, 0, m_hObject, TVI_SORT);
				}
			}
		}
		if (!bIsObject)
		{
			m_hObject = m_ColliderTree.InsertItem(m_wstrSelectObject.c_str(), 0, 0, TVI_ROOT, TVI_SORT);
			m_hBone = m_ColliderTree.InsertItem(wstrColl.c_str(), 0, 0, m_hObject, TVI_SORT);
		}
	}
	else 
	{
		m_hObject = m_ColliderTree.InsertItem(m_wstrSelectObject.c_str(), 0, 0, TVI_ROOT, TVI_SORT);
		m_hBone = m_ColliderTree.InsertItem(wstrColl.c_str(), 0, 0, m_hObject, TVI_SORT);
	}

	//m_BoneTree.InsertItem(wstrBone.c_str(), 0, 0, TVI_ROOT, TVI_SORT); //<-TODO: 고치는중 
}


void CColliderTool::OnBnClickedColliderSaveButton()
{
	CFileDialog dlgFile(FALSE, L".txt", L".txt", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"Text Files(*.txt)|*.txt||Data Files(*.dat) | *.dat | ", this);

	dlgFile.m_ofn.lpstrTitle = L"Colliders Save";

	_tchar szPath[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, szPath);
	::PathRemoveFileSpec(szPath);
	::PathRemoveFileSpec(szPath);

	CString csDataPath = szPath;
	csDataPath += ".\\Resource.\\Data.\\Collider";
	dlgFile.m_ofn.lpstrInitialDir = csDataPath;
	if (IDOK == dlgFile.DoModal())
	{
		Save_Text(dlgFile.GetPathName());

	}
}


void CColliderTool::OnBnClickedColliderLoadButton()
{

	CFileDialog dlgFile(TRUE, L".txt", L".txt", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"Text Files(*.txt)|*.txt||Data Files(*.dat) | *.dat | ", this);

	dlgFile.m_ofn.lpstrTitle = L"Object Load";

	_tchar szPath[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, szPath);
	::PathRemoveFileSpec(szPath);
	::PathRemoveFileSpec(szPath);
	CString csDataPath = szPath;
	csDataPath += ".\\Resource.\\Data.\\Collider";
	dlgFile.m_ofn.lpstrInitialDir = csDataPath;

	if (IDOK == dlgFile.DoModal())
	{
		Load_Text(dlgFile.GetPathName());

	}




}


void CColliderTool::OnOK()
{
	//CDialogEx::OnOK();
}
void CColliderTool::OnCancel()
{
	//CDialogEx::OnCancel();
}

HRESULT CColliderTool::Update(const _float & fTimeDelta)
{
	if (m_pScene != nullptr)
	{
		int iMapSize = 0;
		if (m_ppGameObjectMap == nullptr)
			m_ppGameObjectMap = &dynamic_cast<CTestStage*>(m_pScene)->Get_Layer(L"GameLogic")->Get_ObjectMap();
		else
			iMapSize = (int)(*m_ppGameObjectMap).size();
		if (0 < iMapSize)
			m_pCamera = dynamic_cast<CDynamicCamera*>(((*m_ppGameObjectMap).find(L"DynamicCamera")->second));

		if (iMapSize != m_uiMapSize)
		{
			m_uiMapSize = iMapSize;
			for (auto pObject : (*m_ppGameObjectMap))
			{
				_bool bIsCompare = false;
				m_pDynamicObject =dynamic_cast<CDynamicObject*>(pObject.second);
				wstring wstrObjName= pObject.second->Get_ObjName();

				if (m_pDynamicObject != nullptr)
				{
					HTREEITEM hChild = m_ObjectTree.GetChildItem(m_hDynamicRoot);
					CString csText = m_ObjectTree.GetItemText(hChild);

					if (csText.Find(wstrObjName.c_str()) != -1) //있음
						bIsCompare = true;
					while (hChild = m_ObjectTree.GetNextSiblingItem(hChild))//
					{
						CString csText = m_ObjectTree.GetItemText(hChild);
						if (csText.Find(wstrObjName.c_str()) != -1)
							bIsCompare = true;
					}


					if (!bIsCompare)
					{
						m_hInstDynamic = m_ObjectTree.InsertItem(wstrObjName.c_str(), 0, 0, m_hDynamicRoot, TVI_LAST);

						m_wstrSelectObject= pObject.first;
					}
				}


			}

		}

	}

	return S_OK;
}

void CColliderTool::Get_BoneName()
{
	m_pDynamicMesh = dynamic_cast<Engine::CDynamicMesh*>(m_pCurSelectObj->Get_Component(L"Com_Mesh", Engine::ID_STATIC));
	if (m_pDynamicMesh == nullptr)
	{
#ifdef _DEBUG
		DE_COUT("Collider Tool 141 Error DynamicMesh is nullptr");
#endif // 
		return;


	}
	m_BoneTree.DeleteAllItems();
	int a = 0;
	list<Engine::D3DXMESHCONTAINER_DERIVED*>* list_Mesh = m_pDynamicMesh->Get_MeshContainerList();
	list<Engine::D3DXMESHCONTAINER_DERIVED*>::iterator iter = list_Mesh->begin();
	list<Engine::D3DXMESHCONTAINER_DERIVED*>::iterator end_iter = list_Mesh->end();

	//const Engine::D3DXFRAME_DERIVED* ptr = nullptr;
	//ptr = pDynamicMesh->Get_FrameByName("RightHandAttach");


	for (iter; iter != end_iter; ++iter)
	{
		//DWORD iNum= (*iter)->dwNumBones;

		DWORD iNum = (*iter)->pSkinInfo->GetNumBones();

		for (DWORD i = 0; i < iNum; ++i)
		{
			string strBone= (*iter)->pSkinInfo->GetBoneName(i);
			wstring wstrBone;
			wstrBone.assign(strBone.begin(), strBone.end());
			//Engine::D3DXFRAME_DERIVED*	pBone = (Engine::D3DXFRAME_DERIVED*)D3DXFrameFind(pDynamicMesh->Get_RootFrame(), ch);

			m_BoneTree.InsertItem(wstrBone.c_str(), 0, 0, TVI_ROOT, TVI_SORT); //<-TODO: 고치는중 
		}
	}


}

HRESULT CColliderTool::Save_Text(const TCHAR * pFilePath)
{
	ofstream fout;
	fout.open(pFilePath, ios::trunc);
	if (fout.fail())
		return E_FAIL;
	for (auto mapItem : (*m_ppGameObjectMap))
	{

		Engine::CColliderGroup*	pCollGroup= dynamic_cast<Engine::CColliderGroup*>
								(mapItem.second->Get_Component(L"Com_ColliderGroup", Engine::ID_DYNAMIC));
		if (pCollGroup == nullptr)
			continue;

		Engine::CSphereColliderCom*	pSphereCol = nullptr;


		for (int i = 0; i < Engine::COLOPT_END; i++)
		{
			vector<Engine::CSphereColliderCom*>  pCollVec = *pCollGroup->Get_CollVec((Engine::COLLOPTION)i);
			
			for (auto pColl : pCollVec)
			{
				wstring wstrObjTag = pColl->Get_ObjTag();
				wstring wstrBoneTag = pColl->Get_BoneTag();
				wstring wstrCollTag = pColl->Get_CollTag();
				_vec3	vPos = *pColl->Get_Pos();
				_float fRadius = *pColl->Get_Radius();

				

				fout << CW2A(wstrObjTag.c_str()) << endl;
				fout << i << endl;
				fout << CW2A(wstrBoneTag.c_str()) << endl;
				fout << CW2A(wstrCollTag.c_str()) << endl;
				fout << vPos.x << endl;
				fout << vPos.y << endl;
				fout << vPos.z  << endl;
				fout << fRadius << endl;


			}
		}
		//fout << dynamic_cast<CSphereCollider*>(mapItem.second)->Get_Radius() << endl;
		//fout << dynamic_cast<CSphereCollider*>(mapItem.second)->Get_Option() << endl;

	}
	fout.close();

	return S_OK;

}

HRESULT CColliderTool::Load_Text(const TCHAR * pFilePath)
{
	ifstream fin;

	fin.open(pFilePath);

	if (fin.fail())
		return E_FAIL;

	wstring wstrTemp;

	CString csTemp;
	char cTemp[MIN_STR];
	_vec3 vPos = { INIT_VEC3 };
	_float fRadius = 0.f;
	_uint uiOpt= 0;
	while (!fin.eof())
	{
		
		SPHERE_COL_DATA* pColData = new SPHERE_COL_DATA;

		fin.getline(cTemp, MIN_STR);
		csTemp = cTemp;
		wstrTemp = csTemp.GetString();

		if (wstrTemp.compare(L"") == 0) break;

		pColData->wstrObjTag = wstrTemp;

		fin.getline(cTemp, MIN_STR); // 공백을 포함한 문장 단위(개행 단위)로 읽어오기.
		pColData->uiOption = atoi(cTemp);

		fin.getline(cTemp, MIN_STR);
		csTemp = cTemp;
		wstrTemp = csTemp.GetString();
		pColData->wstrBoneTag = wstrTemp;

		fin.getline(cTemp, MIN_STR);
		csTemp = cTemp;
		wstrTemp = csTemp.GetString();
		pColData->wstrCollTag= wstrTemp;

		fin.getline(cTemp, MIN_STR); // 공백을 포함한 문장 단위(개행 단위)로 읽어오기.
		pColData->vPos.x = (float)atof(cTemp);
		fin.getline(cTemp, MIN_STR); // 공백을 포함한 문장 단위(개행 단위)로 읽어오기.
		pColData->vPos.y = (float)atof(cTemp);
		fin.getline(cTemp, MIN_STR);
		pColData->vPos.z = (float)atof(cTemp);

		fin.getline(cTemp, MIN_STR);
		pColData->fRadius= (float)atof(cTemp);

		
		m_pCollManager->Add_Colldata(pColData);
		
		/////////요기까지
		//D3DXVECTOR3 vPos;

		//fin.getline(cTemp, MIN_STR);
		//csTemp = cTemp;
		//wstrTemp = csTemp.GetString();

		//if (wstrTemp.compare(L"") == 0)
		//	break;
		////fin.getline(cTemp, MIN_STR); // 공백을 포함한 문장 단위(개행 단위)로 읽어오기.

		//fin.getline(cTemp, MIN_STR); // 공백을 포함한 문장 단위(개행 단위)로 읽어오기.
		//vPos.x = (float)atof(cTemp);
		//fin.getline(cTemp, MIN_STR); // 공백을 포함한 문장 단위(개행 단위)로 읽어오기.
		//vPos.y = (float)atof(cTemp);
		//fin.getline(cTemp, MIN_STR);
		//vPos.z = (float)atof(cTemp);

		//fin.getline(cTemp, MIN_STR);
		//fRadius = (float)atof(cTemp);
		//fin.getline(cTemp, MIN_STR);
		//uiOpt= atoi(cTemp);

		////COLL_DATA* pCollData = new COLL_DATA;
		//
		//wstring wstObject, wstrBone;
		//_uint uiIdx = 0;
		//DividString(wstrTemp, wstObject, wstrBone, uiIdx);


		//wstObject += L"_0";
		//Engine::CGameObject* pGameObject = nullptr;
		//pGameObject = m_pSphereColl = CSphereCollider::Create(m_pDevice, wstObject, wstrBone);
		//m_pSphereColl->Set_Radius(fRadius);
		//m_pSphereColl->Set_Position(vPos);
		//m_pSphereColl->Set_Option((COLLIDEROPTION)uiOpt);

		//(*m_ppGameObjectMap).insert(make_pair(wstrTemp, pGameObject));

	}
	fin.close();
}

void CColliderTool::DividString(wstring wstrOrigin, wstring & wstrObject, wstring & wstrBone, _uint & uiIdx)
{

	_uint uiObjLine = 0;
	uiObjLine = (_uint)wstrOrigin.find(L"_");

	_uint uiBoneLine = (_uint)wstrOrigin.find(L"_", uiObjLine + 1);
	wstrObject = wstrOrigin.substr(0, uiObjLine);

	wstrBone = wstrOrigin.substr(uiObjLine + 1, uiBoneLine - uiObjLine - 1);

	wcout << wstrObject << endl;
	wcout << wstrBone << endl;

}



BOOL CColliderTool::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	
	for(int i=0; i<3; i++)
		m_csPosition[i].Format(_T("%f"), 0.f);
	m_csRadius.Format(_T("%f"), m_fRadius);

	if (m_pDeviceClass == nullptr)
	{
		CMainFrame* pFrameWnd = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
		NULL_CHECK_RETURN(pFrameWnd, L"df");

		m_pMyform = dynamic_cast<CMyForm*>(pFrameWnd->m_MainSplitter.GetPane(0, 0));
		m_pDeviceClass = m_pMyform->Get_DeviceClass();
		m_pDevice = m_pMyform->Get_Device();
		m_pScene = m_pMyform->Get_Scene();
		m_pKeyManager = CKeyMgr::GetInstance();
		m_pCollManager = CColliderManager::GetInstance();
	}

	//m_hStaticRoot	=	m_ObjectTree.InsertItem(TEXT("StaticObject"), 0, 0, TVI_ROOT, TVI_LAST);
	m_hDynamicRoot = m_ObjectTree.InsertItem(TEXT("DynamicObject"), 0, 0, TVI_ROOT, TVI_LAST);



	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CColliderTool::OnTvnSelchangedDymeshTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

	HTREEITEM hCurITem = m_ObjectTree.GetSelectedItem();
	m_csSelectMesh = m_ObjectTree.GetItemText(hCurITem);
	
	
	m_pDynamicObject = nullptr;
	m_pCurSelectObj = nullptr; 
	m_pDynamicMesh = nullptr;
	//m_pSphereColl = nullptr;
	wstring wstrSelectMesh = m_csSelectMesh;
	wstrSelectMesh += L"_0";
	if (wstrSelectMesh.find(L"Object") == wstring::npos)
	{
		if (m_ppGameObjectMap != nullptr)
		{
			
			m_pCurSelectObj = dynamic_cast<CDynamicObject*>((*(*m_ppGameObjectMap).find(wstrSelectMesh)).second);
			if (m_pCurSelectObj != nullptr)
			{
				m_wstrSelectObject = (*(*m_ppGameObjectMap).find(wstrSelectMesh)).first;
				m_pColliderGroup = dynamic_cast<Engine::CColliderGroup*>(m_pCurSelectObj->Get_Component(L"Com_ColliderGroup", Engine::ID_DYNAMIC));

				Get_BoneName();
			}
		}
	}

	*pResult = 0;
}




void CColliderTool::OnTvnSelchangedBoneTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);


	m_hSelectBone =m_BoneTree.GetSelectedItem();
	CString csBone = m_BoneTree.GetItemText(m_hSelectBone);

	m_wstrSelectBone = csBone;

	CStringA strA(csBone);
	const char* pszBone = (char *)(LPWSTR)(LPCSTR)strA;
	if (m_pDynamicMesh->Get_FrameByName(pszBone) != NULL)
	{
		m_csPosition[0].Format(_T("%f"), m_pDynamicMesh->Get_FrameByName(pszBone)->TransformationMatrix(4, 1));
		m_csPosition[1].Format(_T("%f"), m_pDynamicMesh->Get_FrameByName(pszBone)->TransformationMatrix(4, 2));
		m_csPosition[2].Format(_T("%f"), m_pDynamicMesh->Get_FrameByName(pszBone)->TransformationMatrix(4, 3));

		SetDlgItemTextW(IDC_EditPosX, m_csPosition[0]);
		SetDlgItemTextW(IDC_EditPosY, m_csPosition[1]);
		SetDlgItemTextW(IDC_EditPosZ, m_csPosition[2]);
	}

	*pResult = 0;
}

void CColliderTool::OnTvnSelchangedColliderTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

	m_hCollider = m_ColliderTree.GetSelectedItem();
	CString csCollider = m_BoneTree.GetItemText(m_hCollider);
	m_wstrSelectCollider = csCollider;
	//wstring wstrBone, wstrObj;
	//_uint uiIdx = 0;
	//DividString(m_wstrSelectCollider, wstrObj, wstrBone, uiIdx);


	m_pSphereCol=m_pColliderGroup->Get_SphereColl(m_wstrSelectCollider);
	//m_pSphereColl=dynamic_cast<CSphereCollider*>((*m_ppGameObjectMap).find(m_wstrSelectCollider)->second);

	if (m_pSphereCol !=nullptr)
	{
		_vec3 vPos =*m_pSphereCol->Get_Pos();
		m_vPosition = vPos;
		m_csPosition[0].Format(_T("%f"),vPos.x);
		m_csPosition[1].Format(_T("%f"),vPos.y);
		m_csPosition[2].Format(_T("%f"),vPos.z);
		m_csRadius.Format(_T("%f"),*m_pSphereCol->Get_Radius());
		//m_csOption.Format(_T("%d"), m_pSphereCol->Get_Option());
		SetDlgItemTextW(IDC_EditPosX, m_csPosition[0]);
		SetDlgItemTextW(IDC_EditPosY, m_csPosition[1]);
		SetDlgItemTextW(IDC_EditPosZ, m_csPosition[2]);
		//SetDlgItemTextW(IDC_EDITOption, m_csOption);

	}

	*pResult = 0;

}


void CColliderTool::OnDeltaposSpinPosX(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	m_vPosition.x = (_float)_tstof(m_csPosition[0]);

	if (pNMUpDown->iDelta < 0)
		m_vPosition.x += 1.0f;
	else
		m_vPosition.x -= 1.0f;

	m_csPosition[0].Format(_T("%f"), m_vPosition.x);
	SetDlgItemTextW(IDC_EditPosX, m_csPosition[0]);

	if (m_pSphereCol != nullptr)
	{
		_vec3 vPos=*m_pSphereCol->Get_Pos();
		vPos.x = m_vPosition.x;
		m_pSphereCol->Set_Pos(vPos);
	}
	//if (m_pSphereColl!=nullptr)
	//	dynamic_cast<Engine::CTransform*>(m_pSphereColl->Get_Component(L"Com_Transform", Engine::ID_DYNAMIC))->m_vInfo[Engine::INFO_POS].x = m_vPosition.x;

	*pResult = 0;
}


void CColliderTool::OnDeltaposSpinPosY(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	m_vPosition.y = (_float)_tstof(m_csPosition[1]);

	if (pNMUpDown->iDelta < 0)
		m_vPosition.y += 1.0f;
	else
		m_vPosition.y -= 1.0f;

	m_csPosition[1].Format(_T("%f"), m_vPosition.y);
	SetDlgItemTextW(IDC_EditPosY, m_csPosition[1]);

	if (m_pSphereCol != nullptr)
	{
		_vec3 vPos = *m_pSphereCol->Get_Pos();
		vPos.y = m_vPosition.y;
		m_pSphereCol->Set_Pos(vPos);
	}


	//if (m_pSphereColl != nullptr)
	//	dynamic_cast<Engine::CTransform*>(m_pSphereColl->Get_Component(L"Com_Transform", Engine::ID_DYNAMIC))->m_vInfo[Engine::INFO_POS].y = m_vPosition.y;



	*pResult = 0;
}


void CColliderTool::OnDeltaposSpinPosZ(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	m_vPosition.z = (_float)_tstof(m_csPosition[2]);

	if (pNMUpDown->iDelta < 0)
		m_vPosition.z += 1.0f;
	else
		m_vPosition.z -= 1.0f;

	m_csPosition[2].Format(_T("%f"), m_vPosition.z);
	SetDlgItemTextW(IDC_EditPosZ, m_csPosition[2]);

	if (m_pSphereCol != nullptr)
	{
		_vec3 vPos = *m_pSphereCol->Get_Pos();
		vPos.z = m_vPosition.z;
		m_pSphereCol->Set_Pos(vPos);
	}

	//if (m_pSphereColl != nullptr)
	//	dynamic_cast<Engine::CTransform*>(m_pSphereColl->Get_Component(L"Com_Transform", Engine::ID_DYNAMIC))->m_vInfo[Engine::INFO_POS].z = m_vPosition.z;

	*pResult = 0;
}


void CColliderTool::OnDeltaposSpinRadius(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	m_fRadius = (_float)_tstof(m_csRadius);

	if (pNMUpDown->iDelta < 0)
		m_fRadius += 1.f;
	else
		m_fRadius -= 1.f;

	m_csRadius.Format(_T("%f"), m_fRadius);
	SetDlgItemTextW(IDC_EditRadius, m_csRadius);


	if (m_pSphereCol != nullptr)
		m_pSphereCol->Set_Radius(m_fRadius);


	//if (m_pSphereColl != nullptr)
	//	m_pSphereColl->Set_Radius(m_fRadius);


	*pResult = 0;
}


void CColliderTool::OnDeltaposSpinAnim(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	
	m_uiAnim = _tstoi(m_csAnim);

	if (pNMUpDown->iDelta < 0)
		m_uiAnim += 1;
	else
	{
		if (m_uiAnim > 0)
			m_uiAnim -= 1;
	}
	m_csAnim.Format(_T("%d"), m_uiAnim);
	SetDlgItemTextW(IDC_EDITAnim, m_csAnim);

	if (m_pSphereCol != nullptr)
		m_pCurSelectObj->Set_AnimationIdx(m_uiAnim);
	
	
	*pResult = 0;
}


void CColliderTool::OnEnChangeEditRadius()
{
	GetDlgItemText(IDC_EditRadius, m_csRadius);
	//if (m_pSphereColl != nullptr)
	//	m_pSphereColl->Set_Radius((_float)_tstof(m_csRadius));

	if (m_pSphereCol != nullptr)
		m_pSphereCol->Set_Radius((_float)_tstof(m_csRadius));

}


void CColliderTool::OnEnChangeEditBone()
{
	if (m_CheckBone.GetCheck())
	{
		GetDlgItemText(IDC_EDITBone, m_csEditBone);
	}

}



void CColliderTool::OnEnChangeEditOption()
{
	GetDlgItemText(IDC_EDITOption, m_csOption);


	if (m_pSphereColl != nullptr)
		m_pSphereColl->Set_Option((COLLIDEROPTION)_tstoi(m_csOption));

}


void CColliderTool::OnDeltaposSpinOption(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	
	GetDlgItemText(IDC_EDITOption, m_csOption);

	m_uiOption = (COLLIDEROPTION)_tstoi(m_csOption);

	if (pNMUpDown->iDelta < 0)
		m_uiOption += 1;
	else
	{
		if (m_uiOption > 0)
			m_uiOption -= 1;
	}

	m_csOption.Format(_T("%d"), m_uiOption);
	SetDlgItemTextW(IDC_EDITOption, m_csOption);



	*pResult = 0;
}
