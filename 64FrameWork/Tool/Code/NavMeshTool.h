#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CNavMeshTool 대화 상자입니다.
class CMyForm;
class CDynamicCamera;
class CNavMeshTool : public CDialogEx
{
	DECLARE_DYNAMIC(CNavMeshTool)

public:
	CNavMeshTool(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CNavMeshTool();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG3 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnNavMeshCreateButton();
	afx_msg void OnSelectNavMesh(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedDeleteButton();
public:
	HRESULT								Update(const _float& fTimeDelta);
private:
	void								Set_NavMeshData();
	HRESULT								Save_Text(const TCHAR * pFilePath);
	HRESULT								Load_Text(const TCHAR * pFilePath);
	//vector<NAV_MESH*>&					Get_NavMeshVec();
private:
	HTREEITEM							m_hRoot, m_hNavMeshIdx, m_hNavMeshPoint;
	CTreeCtrl							m_NavMeshTree;
	CEdit								m_EditMeshLeaf;
	CEdit								m_EditPosX;
	CEdit								m_EditPosY;
	CEdit								m_EditPosZ;
	CString								m_csCurNav;
	CString								m_csCurParentItem;

	Engine::CGraphicDev*				m_pDeviceClass = nullptr;
	LPDIRECT3DDEVICE9					m_pDevice = nullptr;
	CMyForm*							m_pMyform = nullptr;
	CDynamicCamera*						m_pCamera = nullptr;
	Engine::CScene*						m_pScene = nullptr;
	_vec3								m_vPos;
	_uint								m_uiChangeIdx = 0;
	_uint								m_uiSelectNavIdx = 0;
	_uint								m_uiSelcetCellIdx = 0;
	_uint								m_uiPointIdx = 0;
	bool								m_bIsColl = false;

	_uint								m_uiOldCellVecIDx=0;
	Engine::CNaviMesh*					m_pNaviCom = nullptr;
	vector<Engine::CCell*>*				m_ppCellVec = nullptr;
	vector<Engine::NAVI_DATA*>			m_pNavDataVec;
	CString								m_csPosition[3];
	//Engine::CNaviMesh*					m_pNavMesh;

	//vector<NAV_MESH*>					m_pNavMeshVec;
	//map<wstring,pair<CCell*,NAV_MESH*>>	m_pCellMap;
	

public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	
	afx_msg void OnBnClickedSaveButton();
	afx_msg void OnBnClickedLoadButton();
	afx_msg void OnEnChangeEditPosX();
	afx_msg void OnEnChangeEditPosZ();
	afx_msg void OnEnChangeEditPosY();
	afx_msg void OnDeltaposNavPosXSpin(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposNavPosYSpin(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposNavPosZSpin(NMHDR *pNMHDR, LRESULT *pResult);
};
