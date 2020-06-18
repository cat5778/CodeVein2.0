#include "Line.h"

USING(Engine)

Engine::CLine::CLine(void)
{

}

Engine::CLine::~CLine(void)
{

}

HRESULT Engine::CLine::Ready_Line(const _vec3* pPointA, const _vec3* pPointB)
{
	m_vPoint[POINT_START]	= *pPointA;
	m_vPoint[POINT_FINISH]	= *pPointB;

	m_vDirection = m_vPoint[POINT_FINISH] - m_vPoint[POINT_START];
	
	D3DXVec3Cross(&m_vNormal,&m_vDirection,&_vec3(0.f,1.f,0.f));
	
	//D3DXVec2Normalize(&m_vNormal, &m_vNormal);


	return S_OK;
}

Engine::CLine::COMPARE Engine::CLine::Compare(const _vec3* pEndPos)
{
	_vec3		vDest = *pEndPos - m_vPoint[POINT_START];

	_float		fResult = D3DXVec3Dot(D3DXVec3Normalize(&vDest, &vDest), &m_vNormal);

	if (0.f <= fResult)
		return COMPARE_LEFT;

	else
		return COMPARE_RIGHT;

}

Engine::CLine* Engine::CLine::Create(const _vec3* pPointA, const _vec3* pPointB)
{
	CLine*	pInstance = new CLine;

	if (FAILED(pInstance->Ready_Line(pPointA, pPointB)))
		Safe_Release(pInstance);

	return pInstance;
}

void Engine::CLine::Free(void)
{

}

