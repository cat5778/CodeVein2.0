// ../Code/NavMeshTool.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "../Code/NavMeshTool.h"
#include "afxdialogex.h"

#include "MainFrm.h"
#include "ToolView.h"
#include "MyForm.h"
#include "StaticObject.h"
#include "TestStage.h"
#include "DynamicCamera.h"
// CNavMeshTool 대화 상자입니다.

IMPLEMENT_DYNAMIC(CNavMeshTool, CDialogEx)

CNavMeshTool::CNavMeshTool(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG3, pParent)
{

}

CNavMeshTool::~CNavMeshTool()
{
	//for (auto &pNavMesh : m_pNavMeshVec)
	//{
	//	if (pNavMesh != nullptr)
	//	{
	//		delete pNavMesh;
	//		pNavMesh = nullptr;
	//	}
	//}
	//m_pNavMeshVec.clear();
	//Engine::Safe_Release(m_pDevice);
	//Engine::Safe_Release(m_pDeviceClass);
}

void CNavMeshTool::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, m_NavMeshTree);
	DDX_Control(pDX, IDC_EDIT4, m_EditMeshLeaf);
	DDX_Control(pDX, IDC_EDIT1, m_EditPosX);
	DDX_Control(pDX, IDC_EDIT2, m_EditPosY);
	DDX_Control(pDX, IDC_EDIT3, m_EditPosZ);
}

BEGIN_MESSAGE_MAP(CNavMeshTool, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CNavMeshTool::OnNavMeshCreateButton)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, &CNavMeshTool::OnSelectNavMesh)
	ON_BN_CLICKED(IDC_BUTTON2, &CNavMeshTool::OnBnClickedDeleteButton)
	ON_BN_CLICKED(IDC_BUTTON3, &CNavMeshTool::OnBnClickedSaveButton)
	ON_BN_CLICKED(IDC_BUTTON4, &CNavMeshTool::OnBnClickedLoadButton)
	ON_EN_CHANGE(IDC_EditPosX, &CNavMeshTool::OnEnChangeEditPosX)
	ON_EN_CHANGE(IDC_EditPosZ, &CNavMeshTool::OnEnChangeEditPosZ)
	ON_EN_CHANGE(IDC_EditPosY, &CNavMeshTool::OnEnChangeEditPosY)
	ON_NOTIFY(UDN_DELTAPOS, IDC_NavPosXSPIN, &CNavMeshTool::OnDeltaposNavPosXSpin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_NavPosYSPIN, &CNavMeshTool::OnDeltaposNavPosYSpin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_NavPosZSPIN, &CNavMeshTool::OnDeltaposNavPosZSpin)
END_MESSAGE_MAP()

void CNavMeshTool::OnNavMeshCreateButton()
{
	_uint uiNavIdx;
	uiNavIdx = m_ppCellVec->size();
	
	if (m_ppCellVec != nullptr)
	{
		Engine::NAVI_DATA* pNaviData = new Engine::NAVI_DATA;
		memset(pNaviData,0, sizeof(Engine::NAVI_DATA));
		pNaviData->uiIdx =(*m_ppCellVec).size();

		CString csTemp;
		csTemp.Format(_T("Nav%d"), pNaviData->uiIdx);

		m_hNavMeshIdx = m_NavMeshTree.InsertItem(csTemp, 0, 0, m_hRoot, TVI_LAST);
		csTemp.Format(_T("%d (X= %f  Y= %f  Z=%f)"), 0, pNaviData->vPosition1.x, pNaviData->vPosition1.y, pNaviData->vPosition1.z);
		m_hNavMeshPoint = m_NavMeshTree.InsertItem(csTemp, 0, 0, m_hNavMeshIdx, TVI_LAST);

		csTemp.Format(_T("%d (X= %f  Y= %f  Z=%f)"), 1, pNaviData->vPosition2.x, pNaviData->vPosition2.y, pNaviData->vPosition2.z);
		m_hNavMeshPoint = m_NavMeshTree.InsertItem(csTemp, 0, 0, m_hNavMeshIdx, TVI_LAST);
		
		csTemp.Format(_T("%d (X= %f  Y= %f  Z=%f)"), 2, pNaviData->vPosition3.x, pNaviData->vPosition3.y, pNaviData->vPosition3.z);
		m_hNavMeshPoint = m_NavMeshTree.InsertItem(csTemp, 0, 0, m_hNavMeshIdx, TVI_LAST);

		m_pNavDataVec.push_back(pNaviData);
		m_pNaviCom->Add_Cell(pNaviData);
	}

	//csTemp.Format(_T("Nav%d"), pNavMesh->uiNavMeshIdx);
	//wstring wstrNavName = csTemp;
	//m_pScene->Get_Layer(L"GameLogic")->Add_GameObject(wstrNavName.c_str(), pCell);
	//m_pNavMap.insert(make_pair(wstrNavName, pCell));

	//TODO: 매쉬 넣으면 트리 컨트룰에 넣어서 표현한후 클릭하면 확인하기 현재  create 하면 m_pNavMap 안에 들어감  
	//NAV_MESH *pNavMesh= new NAV_MESH;
	//ZeroMemory(pNavMesh, sizeof(NAV_MESH));
	//uiNavIdx = m_pNavMeshVec.size();
	//pNavMesh->uiNavMeshIdx = uiNavIdx;
	//
	//m_pCell = CCell::Create(m_pDevice);

	////m_pNavMeshVec.push_back(pNavMesh);
	//
}

HRESULT CNavMeshTool::Update(const _float & fTimeDelta)
{
	if (m_pScene != nullptr)
	{
		dynamic_cast<CTestStage*>(m_pScene)->Get_Layer(L"GameLogic")->Get_ObjectMap();
		
		int iMapSize = dynamic_cast<CTestStage*>(m_pScene)->Get_Layer(L"GameLogic")->Get_ObjectMap().size();
		if (m_pNaviCom == nullptr)
		{
			m_pNaviCom = dynamic_cast<Engine::CNaviMesh*>(m_pScene->Get_GameObject(L"GameLogic", L"Player")->Get_Component(L"Com_Navi", Engine::ID_STATIC));
			m_ppCellVec = &m_pNaviCom->Get_CellVec();
		}
		if (0 < iMapSize)
			m_pCamera = dynamic_cast<CDynamicCamera*>(dynamic_cast<CTestStage*>(m_pScene)->Get_Layer(L"GameLogic")->Get_ObjectMap().find(L"DynamicCamera")->second);
	}


	if (m_pCamera != nullptr)
	{
		if (m_pCamera->IsPick())
		{
			m_vPos = m_pCamera->Get_PickPos();
			m_bIsColl = true;
	
		}
		if (m_pCamera->IsNavPick())
		{
			m_vPos = m_pCamera->Get_PickPos();
			m_bIsColl = true;

		}

		if (m_bIsColl)
		{
			Set_NavMeshData();
			if (m_uiChangeIdx < 2)
				m_uiChangeIdx++;
			else
				m_uiChangeIdx = 0;
		}
		

	}

	return S_OK;
}

void CNavMeshTool::Set_NavMeshData()
{
	if (m_csCurParentItem.Compare(L"NavMesh") != 0)
		return;
	m_bIsColl = false;
	CString csTemp;

	HTREEITEM hCurITem = m_NavMeshTree.GetSelectedItem();
	HTREEITEM hChildItem = m_NavMeshTree.GetNextItem(hCurITem, TVGN_CHILD);

	for (int i = 0; i < m_uiChangeIdx; i++)
		hChildItem = m_NavMeshTree.GetNextItem(hChildItem, TVGN_NEXT);

	//m_pNavDataVec[m_uiSelcetCellIdx]->uiIdx = m_uiSelcetCellIdx;
	
	switch (m_uiChangeIdx)
	{
	case 0:
		m_pNavDataVec[m_uiSelcetCellIdx]->vPosition1 = m_vPos;
		csTemp.Format(_T("%d (X= %f  Y= %f  Z=%f)"), 0, m_pNavDataVec[m_uiSelcetCellIdx]->vPosition1.x,
														m_pNavDataVec[m_uiSelcetCellIdx]->vPosition1.y, 
														m_pNavDataVec[m_uiSelcetCellIdx]->vPosition1.z);
		break;
	case 1:
		m_pNavDataVec[m_uiSelcetCellIdx]->vPosition2 = m_vPos;
		csTemp.Format(_T("%d (X= %f  Y= %f  Z=%f)"), 1, m_pNavDataVec[m_uiSelcetCellIdx]->vPosition2.x,
														m_pNavDataVec[m_uiSelcetCellIdx]->vPosition2.y,
														m_pNavDataVec[m_uiSelcetCellIdx]->vPosition2.z);
		break;
	case 2:
		m_pNavDataVec[m_uiSelcetCellIdx]->vPosition3 = m_vPos;
		csTemp.Format(_T("%d (X= %f  Y= %f  Z=%f)"), 2, m_pNavDataVec[m_uiSelcetCellIdx]->vPosition3.x,
														m_pNavDataVec[m_uiSelcetCellIdx]->vPosition3.y,
														m_pNavDataVec[m_uiSelcetCellIdx]->vPosition3.z);
		break;
	}
	m_NavMeshTree.SetItemText(hChildItem, csTemp);
	
	if (m_pCamera->IsNavPick())
		(*m_ppCellVec)[m_uiSelcetCellIdx]->Set_NaviData((Engine::CCell::POINT) m_uiChangeIdx, m_vPos);
	if(m_pCamera->IsPick())
		(*m_ppCellVec)[m_uiSelcetCellIdx]->Set_NaviData(*m_pNavDataVec[m_uiSelcetCellIdx]);

	m_pNaviCom->Link_Cell();
	//dynamic_cast<CTestStage*>(m_pScene)->Set_MeshVec(m_pNavDataVec);
			
}

HRESULT CNavMeshTool::Save_Text(const TCHAR * pFilePath)
{
	ofstream fout;
	fout.open(pFilePath, ios::trunc);
	if (fout.fail())
		return E_FAIL;
	for (auto pCell : (*m_ppCellVec))
	{
		Engine::NAVI_DATA tNaviData = pCell->Get_NaviData();
		fout << tNaviData.vPosition1.x<< endl;
		fout << tNaviData.vPosition1.y << endl;
		fout << tNaviData.vPosition1.z << endl;
		fout << tNaviData.vPosition2.x << endl;
		fout << tNaviData.vPosition2.y << endl;
		fout << tNaviData.vPosition2.z << endl;
		fout << tNaviData.vPosition3.x << endl;
		fout << tNaviData.vPosition3.y << endl;
		fout << tNaviData.vPosition3.z << endl;

	}
	fout.close();

	return S_OK;
}

HRESULT CNavMeshTool::Load_Text(const TCHAR * pFilePath)
{
	ifstream fin;
	fin.open(pFilePath);
	if (fin.fail())
		return E_FAIL;
	wstring wstrTemp;
	CString csTemp;
	char cTemp[MIN_STR];
	_uint uidx = 0;
	while (!fin.eof())
	{
		fin.getline(cTemp, MIN_STR); // 공백을 포함한 문장 단위(개행 단위)로 읽어오기.
		csTemp = cTemp;
		wstrTemp = csTemp.GetString();
		if (wstrTemp.compare(L"") == 0)
			break;

		Engine::NAVI_DATA* pNaviData = new Engine::NAVI_DATA;
		pNaviData->vPosition1.x = atof(cTemp);
		fin.getline(cTemp, MIN_STR); // 공백을 포함한 문장 단위(개행 단위)로 읽어오기.
		pNaviData->vPosition1.y = atof(cTemp);
		fin.getline(cTemp, MIN_STR);
		pNaviData->vPosition1.z = atof(cTemp);

		fin.getline(cTemp, MIN_STR); // 공백을 포함한 문장 단위(개행 단위)로 읽어오기.
		pNaviData->vPosition2.x = atof(cTemp);
		fin.getline(cTemp, MIN_STR); // 공백을 포함한 문장 단위(개행 단위)로 읽어오기.
		pNaviData->vPosition2.y = atof(cTemp);
		fin.getline(cTemp, MIN_STR);
		pNaviData->vPosition2.z = atof(cTemp);

		fin.getline(cTemp, MIN_STR); // 공백을 포함한 문장 단위(개행 단위)로 읽어오기.
		pNaviData->vPosition3.x = atof(cTemp);
		fin.getline(cTemp, MIN_STR); // 공백을 포함한 문장 단위(개행 단위)로 읽어오기.
		pNaviData->vPosition3.y = atof(cTemp);
		fin.getline(cTemp, MIN_STR);
		pNaviData->vPosition3.z = atof(cTemp);

		pNaviData->uiIdx = uidx;

		m_pNaviCom->Add_Cell(pNaviData);
		(*m_ppCellVec)[uidx]->Set_NaviData(*pNaviData);
		m_pNavDataVec.push_back(pNaviData);
		uidx++;
	}
	fin.close();

	_uint uiNavIdx;
	uiNavIdx = m_ppCellVec->size();
	m_NavMeshTree.DeleteAllItems();
	m_hRoot = m_NavMeshTree.InsertItem(TEXT("NavMesh"), 0, 0, TVI_ROOT, TVI_LAST);
	//

	//
	if (m_ppCellVec != nullptr)
	{
		_uint uiCellIdx = 0;
		for (auto pCell : (*m_ppCellVec))
		{
			Engine::NAVI_DATA pNaviData = pCell->Get_NaviData();

			CString csTemp;
			csTemp.Format(_T("Nav%d"), pNaviData.uiIdx);

			m_hNavMeshIdx = m_NavMeshTree.InsertItem(csTemp, 0, 0, m_hRoot, TVI_LAST);
			csTemp.Format(_T("%d (X= %f  Y= %f  Z=%f)"), 0, pNaviData.vPosition1.x, pNaviData.vPosition1.y, pNaviData.vPosition1.z);
			m_hNavMeshPoint = m_NavMeshTree.InsertItem(csTemp, 0, 0, m_hNavMeshIdx, TVI_LAST);

			csTemp.Format(_T("%d (X= %f  Y= %f  Z=%f)"), 1, pNaviData.vPosition2.x, pNaviData.vPosition2.y, pNaviData.vPosition2.z);
			m_hNavMeshPoint = m_NavMeshTree.InsertItem(csTemp, 0, 0, m_hNavMeshIdx, TVI_LAST);

			csTemp.Format(_T("%d (X= %f  Y= %f  Z=%f)"), 2, pNaviData.vPosition3.x, pNaviData.vPosition3.y, pNaviData.vPosition3.z);
			m_hNavMeshPoint = m_NavMeshTree.InsertItem(csTemp, 0, 0, m_hNavMeshIdx, TVI_LAST);
			uiCellIdx++;
		}
	}
	return S_OK;
}

//vector<NAV_MESH*>& CNavMeshTool::Get_NavMeshVec()
//{
//	return m_pNavMeshVec;
//}


BOOL CNavMeshTool::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_hRoot = m_NavMeshTree.InsertItem(TEXT("NavMesh"), 0, 0, TVI_ROOT, TVI_LAST);

	if (m_pDeviceClass == nullptr)
	{
		CMainFrame* pFrameWnd = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
		if (pFrameWnd == nullptr)
			return false;


		m_pMyform = dynamic_cast<CMyForm*>(pFrameWnd->m_MainSplitter.GetPane(0, 0));
		m_pDeviceClass = m_pMyform->Get_DeviceClass();
		m_pDevice = m_pMyform->Get_Device();
		m_pScene = m_pMyform->Get_Scene();
	}


	return TRUE;  
}


void CNavMeshTool::OnSelectNavMesh(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	m_uiChangeIdx = 0;

	HTREEITEM hCurITem = m_NavMeshTree.GetSelectedItem();
	HTREEITEM hParentItem = m_NavMeshTree.GetNextItem(hCurITem, TVGN_PARENT);
	CString csCurText, csCurTextIdx, csNaviDataIdx,csSub;
	
	m_csCurNav= m_csCurParentItem = m_NavMeshTree.GetItemText(hParentItem);
	csCurText = m_NavMeshTree.GetItemText(hCurITem);
	m_uiSelcetCellIdx = csCurText.Find('v');
	csNaviDataIdx = csCurText.Right(m_uiSelcetCellIdx - 1);
	m_uiSelcetCellIdx = _ttoi(csNaviDataIdx);

	
	//_int iCellIdx = csCurText.Find(L'v');
	//CString csCellIdx = csCurText.Mid(iCellIdx + 1, iPointIdx - iCellIdx - 1);

	//m_uiPointIdx = iPointIdx = _tstoi(csPointIdx);
	//m_uiSelcetCellIdx = iCellIdx = _tstoi(csCellIdx);


	//csCurText.Find
	
	//csCurTextIdx = csCurText.Right(csCurText.GetLength()- m_uiSelcetCellIdx-1);
	//m_uiSelcetCellIdx = _ttoi(csCurTextIdx);


	_uint uiChildIdx = csCurText.Find('(');
	csSub = csCurText.Left(uiChildIdx - 1);

	//uiChildIdx = _ttoi(csSub);
	if (m_csCurNav.Compare(L"NavMesh") < 0)
		m_csCurNav = m_csCurNav + "_" + csSub;
	else
	{
		m_csCurNav = csCurText;
		m_uiSelcetCellIdx = csCurText.Find('v');
		csNaviDataIdx = csCurText.Right(csCurText.GetLength()-(m_uiSelcetCellIdx+1));
		m_uiSelcetCellIdx = _ttoi(csNaviDataIdx);


	}
	m_EditMeshLeaf.SetWindowTextW(m_csCurNav);
	
	if (m_ppCellVec!=nullptr&&(*m_ppCellVec).size()>= m_uiSelcetCellIdx)
	{
		int i = (*m_ppCellVec).size();

		
		GetDlgItemText(IDC_EditPosX, m_csPosition[0]);
		_float fPosX = _tstof(m_csPosition[0]);
			

		_int iPointIdx = m_csCurNav.ReverseFind(L'_');
		if (iPointIdx == -1 || m_csCurNav.GetLength() == 1)
			return;
			
		CString csPointIdx = m_csCurNav.Right(m_csCurNav.GetLength() - (iPointIdx + 1));

		_int iCellIdx = m_csCurNav.Find(L'v');
		CString csCellIdx = m_csCurNav.Mid(iCellIdx + 1, iPointIdx - iCellIdx - 1);

		iCellIdx = _tstoi(csCellIdx);

		Engine::NAVI_DATA  tNavData = (*m_ppCellVec)[iCellIdx]->Get_NaviData();
		m_uiPointIdx = _ttoi(csSub);

		switch (m_uiPointIdx)
		{
		case 0:
		{
			m_csPosition[0].Format(_T("%f"), tNavData.vPosition1.x);
			SetDlgItemTextW(IDC_EditPosX, m_csPosition[0]);
			m_csPosition[1].Format(_T("%f"), tNavData.vPosition1.y);
			SetDlgItemTextW(IDC_EditPosY, m_csPosition[1]);
			m_csPosition[2].Format(_T("%f"), tNavData.vPosition1.z);
			SetDlgItemTextW(IDC_EditPosZ, m_csPosition[2]);
		}
			break;
		case 1:
		{
			m_csPosition[0].Format(_T("%f"), tNavData.vPosition2.x);
			SetDlgItemTextW(IDC_EditPosX, m_csPosition[0]);
			m_csPosition[1].Format(_T("%f"), tNavData.vPosition2.y);
			SetDlgItemTextW(IDC_EditPosY, m_csPosition[1]);
			m_csPosition[2].Format(_T("%f"), tNavData.vPosition2.z);
			SetDlgItemTextW(IDC_EditPosZ, m_csPosition[2]);
		}
			break;
		case 2:
		{
			m_csPosition[0].Format(_T("%f"), tNavData.vPosition3.x);
			SetDlgItemTextW(IDC_EditPosX, m_csPosition[0]);
			m_csPosition[1].Format(_T("%f"), tNavData.vPosition3.y);
			SetDlgItemTextW(IDC_EditPosY, m_csPosition[1]);
			m_csPosition[2].Format(_T("%f"), tNavData.vPosition3.z);
			SetDlgItemTextW(IDC_EditPosZ, m_csPosition[2]);

		}
			break;

		default:
			break;
		}
	}
	//m_csScale[1].Format(_T("%f"), m_pTransform->m_vScale.y);
	//SetDlgItemTextW(IDC_EditScaleY, m_csScale[1]);

	//m_csScale[2].Format(_T("%f"), m_pTransform->m_vScale.z);
	//SetDlgItemTextW(IDC_EditScaleZ, m_csScale[2]);







	*pResult = 0;
}


void CNavMeshTool::OnOK()
{


	//CDialogEx::OnOK();
}


void CNavMeshTool::OnCancel()
{
	//CDialogEx::OnCancel();
}


void CNavMeshTool::OnBnClickedDeleteButton()
{
	int i = 0;
	for (auto itr = (*m_ppCellVec).begin(); itr != (*m_ppCellVec).end();i++)
	{
		if (m_uiSelcetCellIdx == i)
		{
			(*itr)->Release();
			itr = (*m_ppCellVec).erase(itr);
			break;
		}
		else
			itr++;
	}
	for (auto itr = m_pNavDataVec.begin(); itr != m_pNavDataVec.end(); i++)
	{
		if (m_uiSelcetCellIdx == i)
		{
			delete (*itr);
			itr = m_pNavDataVec.erase(itr);
			break;
		}
		else
			itr++;
	}
	_uint uiNavIdx;
	uiNavIdx = m_ppCellVec->size();
	m_NavMeshTree.DeleteAllItems();
	m_hRoot = m_NavMeshTree.InsertItem(TEXT("NavMesh"), 0, 0, TVI_ROOT, TVI_LAST);

	if (m_ppCellVec != nullptr)
	{
		_uint uiCellIdx= 0;
		for (auto pCell : (*m_ppCellVec))
		{
			Engine::NAVI_DATA* pNaviData = &pCell->Get_NaviData();
			memset(pNaviData, 0, sizeof(Engine::NAVI_DATA));
			//pNaviData->uiIdx = 0;
			//pNaviData->vPosition1 = {0.f,0.f,0.f};
			//pNaviData->vPosition2 = {0.f,0.f,0.f};
			//pNaviData->vPosition3 = {0.f,0.f,0.f};
			//pNaviData->uiIdx=(*m_ppCellVec).size();

			pNaviData->uiIdx = uiCellIdx;

			CString csTemp;
			csTemp.Format(_T("Nav%d"), pNaviData->uiIdx);

			m_hNavMeshIdx = m_NavMeshTree.InsertItem(csTemp, 0, 0, m_hRoot, TVI_LAST);
			csTemp.Format(_T("%d (X= %f  Y= %f  Z=%f)"), 0, pNaviData->vPosition1.x, pNaviData->vPosition1.y, pNaviData->vPosition1.z);
			m_hNavMeshPoint = m_NavMeshTree.InsertItem(csTemp, 0, 0, m_hNavMeshIdx, TVI_LAST);

			csTemp.Format(_T("%d (X= %f  Y= %f  Z=%f)"), 1, pNaviData->vPosition2.x, pNaviData->vPosition2.y, pNaviData->vPosition2.z);
			m_hNavMeshPoint = m_NavMeshTree.InsertItem(csTemp, 0, 0, m_hNavMeshIdx, TVI_LAST);

			csTemp.Format(_T("%d (X= %f  Y= %f  Z=%f)"), 2, pNaviData->vPosition3.x, pNaviData->vPosition3.y, pNaviData->vPosition3.z);
			m_hNavMeshPoint = m_NavMeshTree.InsertItem(csTemp, 0, 0, m_hNavMeshIdx, TVI_LAST);
			uiCellIdx++;
		}
	}



}


void CNavMeshTool::OnBnClickedSaveButton()
{
	CFileDialog dlgFile(FALSE, L".txt", L".txt", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"Text Files(*.txt)|*.txt||Data Files(*.dat) | *.dat | ", this);

	dlgFile.m_ofn.lpstrTitle = L"NavMesh Save";

	_tchar szPath[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, szPath);
	::PathRemoveFileSpec(szPath);
	::PathRemoveFileSpec(szPath);
	CString csDataPath = szPath;
	csDataPath += ".\\Resource.\\Data.\\NavMash";
	dlgFile.m_ofn.lpstrInitialDir = csDataPath;
	if (IDOK == dlgFile.DoModal())
	{
		Save_Text(dlgFile.GetPathName());
		//CString pathName = dlgFile.GetPathName();
		//MessageBox(pathName);

	}
}


void CNavMeshTool::OnBnClickedLoadButton()
{
	CFileDialog dlgFile(TRUE, L".txt", L".txt", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"Text Files(*.txt)|*.txt||Data Files(*.dat) | *.dat | ", this);

	dlgFile.m_ofn.lpstrTitle = L"NavMesh Load";

	_tchar szPath[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, szPath);
	::PathRemoveFileSpec(szPath);
	::PathRemoveFileSpec(szPath);
	CString csDataPath = szPath;
	csDataPath += ".\\Resource.\\Data.\\NavMash";
	dlgFile.m_ofn.lpstrInitialDir = csDataPath;

	if (IDOK == dlgFile.DoModal())
	{
		Load_Text(dlgFile.GetPathName());

	}
}

//

void CNavMeshTool::OnEnChangeEditPosX()
{

	if (m_ppCellVec != nullptr)
	{
		GetDlgItemText(IDC_EditPosX, m_csPosition[0]);
		_float fPosX = _tstof(m_csPosition[0]);

		_int iPointIdx = m_csCurNav.ReverseFind(L'_');
		if (iPointIdx == -1 || m_csCurNav.GetLength() == 1)
			return;

		CString csPointIdx = m_csCurNav.Right(m_csCurNav.GetLength() - (iPointIdx + 1));

		_int iCellIdx= m_csCurNav.Find(L'v');
		CString csCellIdx = m_csCurNav.Mid(iCellIdx+1, iPointIdx-iCellIdx-1);

		m_uiPointIdx = iPointIdx = _tstoi(csPointIdx);
		m_uiSelcetCellIdx = iCellIdx = _tstoi(csCellIdx);

		switch (iPointIdx)
		{
		case 0:
			m_pNavDataVec[iCellIdx]->vPosition1.x = fPosX;
			break;
		case 1:
			m_pNavDataVec[iCellIdx]->vPosition2.x = fPosX;
			break;
		case 2:
			m_pNavDataVec[iCellIdx]->vPosition3.x = fPosX;
			break;
		}
		(*m_ppCellVec)[iCellIdx]->Set_NaviData(*m_pNavDataVec[iCellIdx]);

		_vec3 PointPos = *(*m_ppCellVec)[iCellIdx]->Get_Point((Engine::CCell::POINT)iPointIdx);
		HTREEITEM hCurITem = m_NavMeshTree.GetSelectedItem();
		CString csTemp;
		csTemp.Format(_T("%d (X= %f  Y= %f  Z=%f)"), iPointIdx, fPosX, PointPos.y, PointPos.z);


		m_NavMeshTree.SetItemText(hCurITem, csTemp);

	
	}
}

void CNavMeshTool::OnEnChangeEditPosY()
{
	if (m_ppCellVec != nullptr)
	{
		GetDlgItemText(IDC_EditPosY, m_csPosition[1]);
		_float fPosY = _tstof(m_csPosition[1]);

		_int iPointIdx = m_csCurNav.ReverseFind(L'_');
		if (iPointIdx == -1 || m_csCurNav.GetLength() == 1)
			return;
		CString csPointIdx = m_csCurNav.Right(m_csCurNav.GetLength() - (iPointIdx + 1));

		_int iCellIdx = m_csCurNav.Find(L'v');
		CString csCellIdx = m_csCurNav.Mid(iCellIdx + 1, iPointIdx - iCellIdx - 1);

		m_uiPointIdx = iPointIdx = _tstoi(csPointIdx);
		m_uiSelcetCellIdx= iCellIdx = _tstoi(csCellIdx);

		switch (iPointIdx)
		{
		case 0:
			m_pNavDataVec[iCellIdx]->vPosition1.y = fPosY;
			break;
		case 1:
			m_pNavDataVec[iCellIdx]->vPosition2.y = fPosY;
			break;
		case 2:
			m_pNavDataVec[iCellIdx]->vPosition3.y = fPosY;
			break;
		}
		(*m_ppCellVec)[iCellIdx]->Set_NaviData(*m_pNavDataVec[iCellIdx]);
		_vec3 PointPos = *(*m_ppCellVec)[iCellIdx]->Get_Point((Engine::CCell::POINT)iPointIdx);
		HTREEITEM hCurITem = m_NavMeshTree.GetSelectedItem();
		CString csTemp;
		csTemp.Format(_T("%d (X= %f  Y= %f  Z=%f)"), iPointIdx, PointPos.x, fPosY, PointPos.z);
		

		m_NavMeshTree.SetItemText(hCurITem, csTemp);



	}

}

void CNavMeshTool::OnEnChangeEditPosZ()
{
	if (m_ppCellVec != nullptr)
	{
		GetDlgItemText(IDC_EditPosZ, m_csPosition[2]);
		_float fPosZ = _tstof(m_csPosition[2]);

		_int iPointIdx = m_csCurNav.ReverseFind(L'_');
		if (iPointIdx == -1 || m_csCurNav.GetLength() == 1)
			return;

		CString csPointIdx = m_csCurNav.Right(m_csCurNav.GetLength() - (iPointIdx + 1));

		_int iCellIdx = m_csCurNav.Find(L'v');
		CString csCellIdx = m_csCurNav.Mid(iCellIdx + 1, iPointIdx - iCellIdx - 1);

		m_uiPointIdx= iPointIdx = _tstoi(csPointIdx);
		m_uiSelcetCellIdx = iCellIdx = _tstoi(csCellIdx);


		switch (iPointIdx)
		{
		case 0:
			m_pNavDataVec[iCellIdx]->vPosition1.z = fPosZ;
			break;
		case 1:
			m_pNavDataVec[iCellIdx]->vPosition2.z = fPosZ;
			break;
		case 2:
			m_pNavDataVec[iCellIdx]->vPosition3.z = fPosZ;
			break;
		}
		(*m_ppCellVec)[iCellIdx]->Set_NaviData(*m_pNavDataVec[iCellIdx]);

		_vec3 PointPos = *(*m_ppCellVec)[iCellIdx]->Get_Point((Engine::CCell::POINT)iPointIdx);
		HTREEITEM hCurITem = m_NavMeshTree.GetSelectedItem();
		CString csTemp;
		csTemp.Format(_T("%d (X= %f  Y= %f  Z=%f)"), iPointIdx, PointPos.x, PointPos.y, fPosZ);
		m_NavMeshTree.SetItemText(hCurITem, csTemp);
	}
}

void CNavMeshTool::OnDeltaposNavPosXSpin(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	_float fPosX = _tstof(m_csPosition[0]);

	if (pNMUpDown->iDelta < 0)
		fPosX += 0.01f;
	else
		fPosX -= 0.01f;

	m_csPosition[0].Format(_T("%f"), fPosX);
	SetDlgItemTextW(IDC_EditPosX, m_csPosition[0]);

	switch (m_uiPointIdx)
	{
	case 0:
		m_pNavDataVec[m_uiSelcetCellIdx]->vPosition1.x = fPosX;
		break;
	case 1:
		m_pNavDataVec[m_uiSelcetCellIdx]->vPosition2.x = fPosX;
		break;
	case 2:
		m_pNavDataVec[m_uiSelcetCellIdx]->vPosition3.x = fPosX;
		break;
	}
		
	(*m_ppCellVec)[m_uiSelcetCellIdx]->Set_NaviData(*m_pNavDataVec[m_uiSelcetCellIdx]);

	*pResult = 0;
}


void CNavMeshTool::OnDeltaposNavPosYSpin(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	_float fPosY = _tstof(m_csPosition[1]);

	if (pNMUpDown->iDelta < 0)
		fPosY += 0.01f;
	else
		fPosY -= 0.01f;

	m_csPosition[1].Format(_T("%f"), fPosY);
	SetDlgItemTextW(IDC_EditPosY, m_csPosition[1]);

	switch (m_uiPointIdx)
	{
	case 0:
		m_pNavDataVec[m_uiSelcetCellIdx]->vPosition1.y = fPosY;
		break;
	case 1:
		m_pNavDataVec[m_uiSelcetCellIdx]->vPosition2.y = fPosY;
		break;
	case 2:
		m_pNavDataVec[m_uiSelcetCellIdx]->vPosition3.y = fPosY;
		break;
	}

	(*m_ppCellVec)[m_uiSelcetCellIdx]->Set_NaviData(*m_pNavDataVec[m_uiSelcetCellIdx]);
	*pResult = 0;
}


void CNavMeshTool::OnDeltaposNavPosZSpin(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	_float fPosZ = _tstof(m_csPosition[2]);

	if (pNMUpDown->iDelta < 0)
		fPosZ += 0.01f;
	else
		fPosZ -= 0.01f;

	m_csPosition[2].Format(_T("%f"), fPosZ);
	SetDlgItemTextW(IDC_EditPosZ, m_csPosition[2]);

	switch (m_uiPointIdx)
	{
	case 0:
		m_pNavDataVec[m_uiSelcetCellIdx]->vPosition1.z = fPosZ;
		break;
	case 1:
		m_pNavDataVec[m_uiSelcetCellIdx]->vPosition2.z = fPosZ;
		break;
	case 2:
		m_pNavDataVec[m_uiSelcetCellIdx]->vPosition3.z = fPosZ;
		break;
	}

	(*m_ppCellVec)[m_uiSelcetCellIdx]->Set_NaviData(*m_pNavDataVec[m_uiSelcetCellIdx]);
	*pResult = 0;
}
