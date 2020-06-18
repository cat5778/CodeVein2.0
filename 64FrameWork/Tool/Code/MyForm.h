#pragma once
#include "afxcmn.h"


class CObjectTool;
class CCameraTool;
class CNavMeshTool;
class CColliderTool;
// CMyForm 폼 뷰입니다.

class CMyForm : public CFormView
{
	DECLARE_DYNCREATE(CMyForm)
public:
	CObjectTool*	m_pObjectTool;
	CCameraTool*	m_pCameraTool;
	CNavMeshTool*	m_pNavMeshTool;
	CColliderTool*	m_pColliderTool;
protected:
	CMyForm();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CMyForm();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MYFORM };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif
public:
	Engine::CGraphicDev*	Get_DeviceClass() { return m_pDeviceClass; }
	LPDIRECT3DDEVICE9		Get_Device() { return m_pDevice; }
	Engine::CScene*			Get_Scene() { return m_pScene; }

private:
	Engine::CGraphicDev*	m_pDeviceClass = nullptr;
	LPDIRECT3DDEVICE9		m_pDevice = nullptr;
	CToolView*				m_pToolView =nullptr;
	Engine::CScene*			m_pScene = nullptr;




protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CTabCtrl m_TabCtrl;
	virtual void OnInitialUpdate();
	afx_msg void OnTcnSelchangeTab2(NMHDR *pNMHDR, LRESULT *pResult);
};


