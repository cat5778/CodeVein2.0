
// ToolView.cpp : CToolView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "Tool.h"
#endif

#include "MainFrm.h"
#include "ToolDoc.h"
#include "ToolView.h"
#include "TestStage.h"




// 전역변수
HWND g_hWnd;
HINSTANCE g_hInst;                                // 현재 인스턴스입니다.


// CToolView
IMPLEMENT_DYNCREATE(CToolView, CView)

BEGIN_MESSAGE_MAP(CToolView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_MBUTTONDOWN()
END_MESSAGE_MAP()

// CToolView 생성/소멸

CToolView::CToolView()
{
	// TODO: 여기에 생성 코드를 추가합니다.
}

CToolView::~CToolView()
{
	Engine::Safe_Release(m_pDevice);
	Engine::Safe_Release(m_pDeviceClass);
	Engine::Safe_Release(m_pManagement);

	Engine::Release_Resources();
	Engine::Release_Utility();
	Engine::Release_System();

}

BOOL CToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CToolView 그리기

void CToolView::OnDraw(CDC* /*pDC*/)
{
	CToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
}


// CToolView 인쇄

BOOL CToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CToolView 진단

#ifdef _DEBUG
void CToolView::AssertValid() const
{
	CView::AssertValid();
}

void CToolView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

HRESULT CToolView::View_Update(const _float & fTimeDelta)
{
	m_fTime += fTimeDelta;
	Engine::Set_InputDev();
	if (m_pManagement != nullptr)
		m_pManagement->Update_Scene(fTimeDelta);

	CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();

	return S_OK;
}

void CToolView::View_Render()
{
	m_pDeviceClass->Render_Begin(D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.f));
	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	if (m_pManagement != nullptr)
		m_pManagement->Render_Scene();

	//POINT ptMousePos;

	//GetCursorPos(&ptMousePos);
	///*ScreenToClient();*/
	//swprintf_s(m_szMousePos, L"ScrollPos X=: %d  , Y= %d ", ptMousePos.x, ptMousePos.y);

	//++m_dwRenderCnt;

	//if (m_fTime >= 1.0f)
	//{
	//	wsprintf(m_szFPS, L"FPS : %d", m_dwRenderCnt);
	//	m_fTime = 0.f;
	//	m_dwRenderCnt = 0;
	//}

	//Engine::Render_Font(L"Font_Second", m_szFPS, &_vec2(10.f, 40.f), D3DXCOLOR(0.f, 1.f, 0.f, 1.f));
	//wsprintf(m_szMousePos, L"Mouse Pos X= %d Y= %d ", ptMousePos.x, ptMousePos.y);
	//Engine::Render_Font(L"Font_Second", m_szMousePos, &_vec2(10.f, 60.f), D3DXCOLOR(0.f, 1.f, 0.f, 1.f));


	m_pDeviceClass->Render_End();

}

Engine::CGraphicDev * CToolView::GetDeviceClass()
{
	if (m_pDeviceClass == nullptr)
		return  nullptr;
	return m_pDeviceClass;
}

LPDIRECT3DDEVICE9 & CToolView::GetDevice()
{
	return m_pDevice;
}

Engine::CScene * CToolView::GetScene()
{
	return m_pScene;
}

HRESULT CToolView::SetUp_DefaultSetting(LPDIRECT3DDEVICE9 * ppGraphicDev)
{
	FAILED(Engine::Ready_GraphicDev(g_hWnd,
		Engine::MODE_WIN,
		WINCX,
		WINCY,
		&m_pDeviceClass),
		E_FAIL);

	m_pDeviceClass->AddRef();

	(*ppGraphicDev) = m_pDeviceClass->Get_Device();
	(*ppGraphicDev)->AddRef();

	Engine::Ready_Font(m_pDeviceClass->Get_Device(), L"Font_Second", L"바탕", 20, 20, FW_THIN);

	// INPUTDEV
	FAILED(Engine::Ready_InputDev(g_hInst, g_hWnd), E_FAIL);

	return S_OK;
}

HRESULT CToolView::Ready_Scene(LPDIRECT3DDEVICE9 & pGraphicDev, Engine::CManagement ** ppManagement)
{
	FAILED(Engine::Create_Management(pGraphicDev,ppManagement));


	(*ppManagement)->AddRef();

	m_pScene = CTestStage::Create(pGraphicDev, this);
	NULL_CHECK_RETURN(m_pScene, E_FAIL);


	FAILED((*ppManagement)->SetUp_Scene(m_pScene), E_FAIL);

	return S_OK;

}

CToolDoc* CToolView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CToolDoc)));
	return (CToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CToolView 메시지 처리기


void CToolView::OnInitialUpdate()
{
	g_hWnd = m_hWnd;

	g_hInst = AfxGetInstanceHandle();

	// View 크기를 조정하기 위해서는 프레임 원도우의 크기를 건드린다.
	// 프레임 윈도우의 포인터 얻어오는 방법.
	CMainFrame* pFrameWnd = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	NULL_CHECK(pFrameWnd);
	RECT rcFrame = {};

	// 윈도우의 크기(테두리 + 클라이언트 영역) 얻어오는 함수.(스크린 좌표 기준)
	pFrameWnd->GetWindowRect(&rcFrame);
	::SetRect(&rcFrame, 0, 0, rcFrame.right - rcFrame.left, rcFrame.bottom - rcFrame.top);

	RECT rcView = {};

	// 윈도우의 클라이언트 영역(테두리를 포함하지 않은) 크기 얻어오는 함수. (클라이언트 좌표 기준)
	GetClientRect(&rcView);

	// 현재 프레임윈도우와 View 사이의 갭들을 구한다.
	int iGapX = rcFrame.right - rcView.right;
	int iGapY = rcFrame.bottom - rcView.bottom;

	// 프레임윈도우의 크기를 새로 설정.
	pFrameWnd->SetWindowPos(nullptr, 0, 0, WINCX + iGapX, WINCY + iGapY, SWP_NOZORDER | SWP_NOMOVE);

	FAILED(SetUp_DefaultSetting(&m_pDevice), E_FAIL);
	FAILED(Ready_Scene(m_pDevice, &m_pManagement), E_FAIL);

	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);


	
}


void CToolView::OnMButtonDown(UINT nFlags, CPoint point)
{
	ClientToScreen(&point);
}
