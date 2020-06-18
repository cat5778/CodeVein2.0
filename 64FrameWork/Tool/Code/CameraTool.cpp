// ../Code/CameraTool.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "../Code/CameraTool.h"
#include "afxdialogex.h"


// CCameraTool 대화 상자입니다.

IMPLEMENT_DYNAMIC(CCameraTool, CDialogEx)

CCameraTool::CCameraTool(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG2, pParent)
{

}

CCameraTool::~CCameraTool()
{
	//Engine::Safe_Release(m_pDevice);
	//Engine::Safe_Release(m_pDeviceClass);

}

void CCameraTool::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CCameraTool, CDialogEx)
END_MESSAGE_MAP()


// CCameraTool 메시지 처리기입니다.
