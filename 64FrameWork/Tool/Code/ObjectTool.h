#pragma once
#include "afxcmn.h"
#include "afxwin.h"

class CDynamicCamera;
class CMyForm;
class CKeyMgr;
class CObjectTool : public CDialogEx
{
	DECLARE_DYNAMIC(CObjectTool)

public:
	CObjectTool(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CObjectTool();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void						DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL						OnInitDialog();

public:
	HTREEITEM							m_hDynamicMesh, m_hStaticMesh, m_hMap, m_hGroup,m_hFloor, m_hBuilding, m_hTree, m_hPillar, m_hGrass, m_hEtc;
	HTREEITEM							m_hStaticRoot, m_hDynamicRoot, m_hInstDynamic, m_hInstStatic;
	CTreeCtrl							m_StaticTree;
	CButton								m_StaticCreateButton;
	CButton								m_StaticDeleteButton;
	CEdit								m_EditScaleX;
	CEdit								m_EditScaleY;
	CEdit								m_EditScaleZ;
	CEdit								m_EditRotationX;
	CEdit								m_EditRotationY;
	CEdit								m_EditRotationZ;
	CEdit								m_EditPositionX;
	CEdit								m_EditPositionY;
	CEdit								m_EditPositionZ;
	CTreeCtrl							m_InstanceTree;

public:
	afx_msg void						OnBnClickedMeshCreate();   //Create Mesh
	afx_msg void						OnBnClickedStDeleteBT();   //Delete Mesh
	afx_msg void						OnSelectMesh(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void						OnDeltaposSpinScaleX(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void						OnTvnSelchangedInstancetree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void						OnEnChangeEditScaleX();
	afx_msg void						OnEnChangeEditScaleY();
	afx_msg void						OnEnChangeEditScaleZ();
	afx_msg void						OnEnChangeEditRotationX();
	afx_msg void						OnEnChangeEditRotationY();
	afx_msg void						OnEnChangeEditRotationZ();
	afx_msg void						OnEnChangeEditPositionX();
	afx_msg void						OnEnChangeEditPositionY();
	afx_msg void						OnEnChangeEditPositionZ();
	afx_msg void						OnDeltaposSpinScaleY(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void						OnDeltaposSpinScaleZ(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void						OnDeltaposSpinRotationX(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void						OnDeltaposSpinRotationY(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void						OnDeltaposSpinRotationZ(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void						OnDeltaposSpinPositionX(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void						OnDeltaposSpinPositionY(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void						OnDeltaposSpinPositionZ(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void						OnBnClickedObjectSaveButton();
	afx_msg void						OnBnClickedObjectLoadButton();
	virtual void						OnOK();
	virtual void						OnCancel();

public:
	HRESULT								Update(const _float& fTimeDelta);
private:
	bool								Move_Obj();
	HRESULT								Save_Data(const TCHAR * pFilePath);
	HRESULT								Load_Data(const TCHAR * pFilePath);
	HRESULT								Save_Text(const TCHAR * pFilePath);
	HRESULT								Load_Text(const TCHAR * pFilePath);
	bool								Ready_Mesh(MESH_PATH* pPathInfo);

private:
	Engine::CGraphicDev*				m_pDeviceClass = nullptr;
	LPDIRECT3DDEVICE9					m_pDevice = nullptr;
	CMyForm*							m_pMyform = nullptr;
	CKeyMgr*							m_pKeyManager = nullptr;
	CDynamicCamera*						m_pCamera = nullptr;
	Engine::CScene*						m_pScene = nullptr;
	
	list<MESH_PATH*>					m_pMeshList;
	list<Effect_Path*>					m_pEffectList;
	map<wstring, Engine::CGameObject*>*	m_ppGameObjectMap = nullptr;
	Engine::CGameObject*				m_pGameObject;
	Engine::CTransform*					m_pTransform=nullptr;

	wstring								m_wstrSelectObject;
	CString								m_csSelectMesh;
	CString								m_csScale[3];
	CString								m_csRotation[3];
	CString								m_csPosition[3];

	_vec3								m_vScale   = { INIT_VEC3 };
	_vec3								m_vRotation= { INIT_VEC3 };
	_vec3								m_vPosition= { INIT_VEC3 };

};
