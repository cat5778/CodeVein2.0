
// ToolView.h : CToolView 클래스의 인터페이스
//

#pragma once
BEGIN(Engine)
class CGraphicDev;
class CManagement;
class CTransform;
class CKeyMgr;
class CGameObject;
END

class CToolDoc;
class CClassView;


class CToolView : public CView
{
protected: // serialization에서만 만들어집니다.
							CToolView();
	DECLARE_DYNCREATE(CToolView)

// 특성입니다.
public:
	CToolDoc*				GetDocument() const;

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual void			OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL			PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL			OnPreparePrinting(CPrintInfo* pInfo);
	virtual void			OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void			OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 구현입니다.
public:
	virtual					~CToolView();

#ifdef _DEBUG
	virtual void			AssertValid() const;
	virtual void			Dump(CDumpContext& dc) const;
#endif

public:
	HRESULT					View_Update(const _float& fTimeDelta);
	void					View_Render();
	Engine::CGraphicDev*	GetDeviceClass();
	LPDIRECT3DDEVICE9&		GetDevice();
	Engine::CScene*			GetScene();


private:
	HRESULT					SetUp_DefaultSetting(LPDIRECT3DDEVICE9* ppGraphicDev);
	HRESULT					Ready_Scene(LPDIRECT3DDEVICE9& pGraphicDev,Engine::CManagement** ppManagement);

private:
	Engine::CGraphicDev*	m_pDeviceClass = nullptr;
	Engine::CManagement*	m_pManagement = nullptr;
	LPDIRECT3DDEVICE9		m_pDevice = nullptr;
	Engine::CScene*			m_pScene = nullptr;

	//Debug
	_tchar					m_szFPS[256] = L"FPS =";
	_tchar					m_szMousePos[256] = L"";
	_float					m_fTime = 0;
	_ulong					m_dwRenderCnt=0;
	//_int					  m_MonsterCount = 0;


protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void			OnInitialUpdate();
	afx_msg void			OnMButtonDown(UINT nFlags, CPoint point);
};

#ifndef _DEBUG  // ToolView.cpp의 디버그 버전
inline CToolDoc* CToolView::GetDocument() const
   { return reinterpret_cast<CToolDoc*>(m_pDocument); }
#endif

