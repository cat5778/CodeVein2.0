#include "VIBuffer.h"

USING(Engine)

Engine::CVIBuffer::CVIBuffer(LPDIRECT3DDEVICE9 pGraphicDev)
	: CResources(pGraphicDev)
	, m_pVB(nullptr)
	, m_pIB(nullptr)
	, m_dwVtxSize(0)
	, m_dwVtxCnt(0)
	, m_dwVtxFVF(0)
	, m_dwTriCnt(0)
	, m_dwIdxSize(0)
{

}

Engine::CVIBuffer::CVIBuffer(const CVIBuffer& rhs)
	: CResources(rhs)
	, m_pVB(rhs.m_pVB)
	, m_pIB(rhs.m_pIB)
	, m_dwVtxSize(rhs.m_dwVtxSize)
	, m_dwVtxCnt(rhs.m_dwVtxCnt)
	, m_dwVtxFVF(rhs.m_dwVtxFVF)
	, m_dwTriCnt(rhs.m_dwTriCnt)
	, m_dwIdxSize(rhs.m_dwIdxSize)

{
	m_pVB->AddRef();
	m_pIB->AddRef();
}

Engine::CVIBuffer::~CVIBuffer(void)
{

}

void CVIBuffer::SendToClient(void * pVertex)
{
	void*	pOriVtx = nullptr;

	m_pVB->Lock(0, 0, &pOriVtx, 0);

	memcpy(pVertex, pOriVtx,m_dwVtxSize * m_dwVtxCnt);

	m_pVB->Unlock();
}

void CVIBuffer::ReceiveToEngine(void * pVertex)
{
	void*	pOriVtx = nullptr;

	m_pVB->Lock(0, 0, &pOriVtx, 0);

	memcpy(pOriVtx, pVertex, m_dwVtxSize * m_dwVtxCnt);

	m_pVB->Unlock();
}

HRESULT Engine::CVIBuffer::Ready_Buffer(void)
{
	FAILED_CHECK_RETURN(m_pGraphicDev->CreateVertexBuffer(m_dwVtxSize * m_dwVtxCnt,	// 생성할 버퍼의 크기
															0,					// 버퍼의 종류(숫자가 0인 경우 정적 버퍼)
															m_dwVtxFVF,			// 생성하는 버텍스의 속성값
															D3DPOOL_MANAGED,	// 메모리 풀의 설정
															&m_pVB, 
															NULL), 
															E_FAIL);


	FAILED_CHECK_RETURN(m_pGraphicDev->CreateIndexBuffer(m_dwIdxSize * m_dwTriCnt,	// 생성할 인덱스 버퍼의 크기
															0,					// 버퍼의 종류(숫자가 0인 경우 정적 버퍼)
															m_IdxFmt,			// 생성하는 인덱스의 속성값
															D3DPOOL_MANAGED,	// 메모리 풀의 설정
															&m_pIB,
															NULL),
															E_FAIL);

	

	return S_OK;
}

void Engine::CVIBuffer::Render_Buffer(void)
{
	// 디바이스 장치와 버텍스 버퍼를 링크하는 함수
	m_pGraphicDev->SetStreamSource(0, m_pVB, 0, m_dwVtxSize);
	m_pGraphicDev->SetFVF(m_dwVtxFVF);
	m_pGraphicDev->SetIndices(m_pIB);
	
	m_pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_dwVtxCnt, 0, m_dwTriCnt);

	//m_pGraphicDev->DrawPrimitive(D3DPT_TRIANGLELIST, 0, m_dwTriCnt);	// 3인자 : 삼각형의 개수


}

void Engine::CVIBuffer::Free(void)
{
	CResources::Free();

	Safe_Release(m_pVB);
	Safe_Release(m_pIB);
}

