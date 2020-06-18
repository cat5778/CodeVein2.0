#include "NaviMesh.h"

USING(Engine)

Engine::CNaviMesh::CNaviMesh(LPDIRECT3DDEVICE9 pGraphicDev)
	:CMesh(pGraphicDev)
	, m_dwIndex(0)
{

}

Engine::CNaviMesh::CNaviMesh(const CNaviMesh& rhs)
	: CMesh(rhs)
	, m_vecCell(rhs.m_vecCell)
	, m_dwIndex(rhs.m_dwIndex)	
{
	for (auto& iter : m_vecCell)
		iter->AddRef();
}

Engine::CNaviMesh::~CNaviMesh(void)
{

}

HRESULT CNaviMesh::Add_Cell(NAVI_DATA* naviData)
{
	CCell*		pCell = CCell::Create(m_pGraphicDev,
										m_vecCell.size(),
										&naviData->vPosition1,
										&naviData->vPosition2,
										&naviData->vPosition3);
	
	if(pCell==nullptr)
		return E_FAIL;
	
	m_vecCell.push_back(pCell);
	pCell->Set_CellIdx(m_vecCell.size());
	FAILED_CHECK_RETURN(Link_Cell(), E_FAIL);


}

vector<CCell*>& CNaviMesh::Get_CellVec()
{
	return m_vecCell;
}

HRESULT Engine::CNaviMesh::Ready_NaviMesh(void)
{
	//m_vecCell.reserve(4);

	//CCell*		pCell = nullptr;

	//// 0
	//pCell = CCell::Create(m_pGraphicDev,
	//					 m_vecCell.size(),
	//					 &_vec3(0.f, 5.f, 2.f), 
	//					 &_vec3(2.f, 0.f, 0.f), 
	//					 &_vec3(0.f, 0.f, 0.f));
	//NULL_CHECK_RETURN(pCell, E_FAIL);
	//m_vecCell.push_back(pCell);

	//// 1
	//pCell = CCell::Create(m_pGraphicDev,
	//					 m_vecCell.size(),
	//					 &_vec3(0.f, 0.f, 2.f),
	//					 &_vec3(2.f, 0.f, 2.f),
	//					 &_vec3(2.f, 0.f, 0.f));
	//NULL_CHECK_RETURN(pCell, E_FAIL);
	//m_vecCell.push_back(pCell);

	//// 2
	//pCell = CCell::Create(m_pGraphicDev,
	//					m_vecCell.size(),
	//					&_vec3(0.f, 0.f, 4.f),
	//					&_vec3(2.f, 0.f, 2.f),
	//					&_vec3(0.f, 0.f, 2.f));
	//NULL_CHECK_RETURN(pCell, E_FAIL);
	//m_vecCell.push_back(pCell);

	//// 3
	//pCell = CCell::Create(m_pGraphicDev,
	//	m_vecCell.size(),
	//	&_vec3(2.f, 0.f, 2.f),
	//					  &_vec3(4.f, 0.f, 0.f),
	//					  &_vec3(2.f, 0.f, 0.f));
	//NULL_CHECK_RETURN(pCell, E_FAIL);
	//m_vecCell.push_back(pCell);

	//FAILED_CHECK_RETURN(Link_Cell(), E_FAIL);

	return S_OK;
}

void CNaviMesh::Render_NaviMesh(void)
{
	for (auto& iter : m_vecCell)
		iter->Render_Cell();

}

_vec3 CNaviMesh::Move_OnNaviMesh(const _vec3 * pTargetPos, const _vec3 * pTargetDir)
{
	_vec3	vEndPos = *pTargetPos + *pTargetDir;

	if (CCell::COMPARE_MOVE == m_vecCell[m_dwIndex]->Compare(&vEndPos, &m_dwIndex))
		return vEndPos;

	else if(CCell::COMPARE_STOP == m_vecCell[m_dwIndex]->Compare(&vEndPos, &m_dwIndex))
		return *pTargetPos;


}
bool CNaviMesh::Move_OnNaviMesh(const _vec3 * pTargetPos, const _vec3 * pTargetDir, _vec3* _out)
{
	_vec3   vEndPos = *pTargetPos + *pTargetDir;

	D3DXPLANE tPlane;
	_vec3 vPos0, vPos1, vPos2;

	vPos0 = *m_vecCell[m_dwIndex]->Get_Point(CCell::POINT_A);
	vPos1 = *m_vecCell[m_dwIndex]->Get_Point(CCell::POINT_B);
	vPos2 = *m_vecCell[m_dwIndex]->Get_Point(CCell::POINT_C);

	D3DXPlaneFromPoints(&tPlane, &vPos0, &vPos1, &vPos2);
	float fResult = D3DXPlaneDotCoord(&tPlane, &vEndPos);

	float y = (tPlane.a * vEndPos.x) + (tPlane.c * vEndPos.z) + tPlane.d;
	y /= tPlane.b * -1;
	vEndPos.y = y;

	memcpy(_out, &vEndPos, sizeof(_vec3));

	if (CCell::COMPARE_MOVE == m_vecCell[m_dwIndex]->Compare(&vEndPos, &m_dwIndex))
		return true;
	else if (CCell::COMPARE_STOP == m_vecCell[m_dwIndex]->Compare(&vEndPos, &m_dwIndex))
	{
		*_out = *pTargetPos;
		return false;
	}
	else
	{
		*_out = *pTargetPos;

		return false;
	}
}

bool CNaviMesh::Jump_OnNaviMesh(const _vec3 * pTargetPos, const _vec3 * pTargetDir, _vec3 * _out)
{
	_vec3   vEndPos = *pTargetPos + *pTargetDir;

	D3DXPLANE tPlane;
	_vec3 vPos0, vPos1, vPos2;

	vPos0 = *m_vecCell[m_dwIndex]->Get_Point(CCell::POINT_A);
	vPos1 = *m_vecCell[m_dwIndex]->Get_Point(CCell::POINT_B);
	vPos2 = *m_vecCell[m_dwIndex]->Get_Point(CCell::POINT_C);

	D3DXPlaneFromPoints(&tPlane, &vPos0, &vPos1, &vPos2);
	float fResult = D3DXPlaneDotCoord(&tPlane, &vEndPos);


	float y = (tPlane.a * vEndPos.x) + (tPlane.c * vEndPos.z) + tPlane.d;
	y /= tPlane.b * -1;
	if (vEndPos.y <= y)
		vEndPos.y = y;

	memcpy(_out, &vEndPos, sizeof(_vec3));

	if (CCell::COMPARE_MOVE == m_vecCell[m_dwIndex]->Compare(&vEndPos, &m_dwIndex))
		return true;
	else if (CCell::COMPARE_STOP == m_vecCell[m_dwIndex]->Compare(&vEndPos, &m_dwIndex))
	{
		*_out = *pTargetPos;
		return false;
	}
	else
	{
		*_out = *pTargetPos;

		return false;
	}
}


HRESULT Engine::CNaviMesh::Link_Cell(void)
{
	_ulong	dwSize = m_vecCell.size();

	for (_ulong i = 0; i < dwSize; ++i)
	{
		for (_ulong j = 0; j < dwSize; ++j)
		{
			if(i == j)
				continue;

			if(nullptr == m_vecCell[i]->Get_Neighbor(CCell::NEIGHBOR_AB) &&
				true == m_vecCell[j]->Compare_Point(m_vecCell[i]->Get_Point(CCell::POINT_A), 
													m_vecCell[i]->Get_Point(CCell::POINT_B), 
													m_vecCell[i]))
			{
				m_vecCell[i]->Set_Neighbor(CCell::NEIGHBOR_AB, m_vecCell[j]);
				continue;
			}

			if (nullptr == m_vecCell[i]->Get_Neighbor(CCell::NEIGHBOR_BC) &&
				true == m_vecCell[j]->Compare_Point(m_vecCell[i]->Get_Point(CCell::POINT_B),
													m_vecCell[i]->Get_Point(CCell::POINT_C),
													m_vecCell[i]))
			{
				m_vecCell[i]->Set_Neighbor(CCell::NEIGHBOR_BC, m_vecCell[j]);
				continue;
			}


			if (nullptr == m_vecCell[i]->Get_Neighbor(CCell::NEIGHBOR_CA) &&
				true == m_vecCell[j]->Compare_Point(m_vecCell[i]->Get_Point(CCell::POINT_C),
													m_vecCell[i]->Get_Point(CCell::POINT_A),
													m_vecCell[i]))
			{
				m_vecCell[i]->Set_Neighbor(CCell::NEIGHBOR_CA, m_vecCell[j]);
				continue;
			}
		}
	}

	return S_OK;
}

Engine::CNaviMesh* Engine::CNaviMesh::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CNaviMesh*	pInstance = new CNaviMesh(pGraphicDev);

	if (FAILED(pInstance->Ready_NaviMesh()))
		Safe_Release(pInstance);

	return pInstance;
}

Engine::CResources* Engine::CNaviMesh::Clone(void)
{
	return new CNaviMesh(*this);
}

void Engine::CNaviMesh::Free(void)
{
	for_each(m_vecCell.begin(), m_vecCell.end(), CDeleteObj());
	m_vecCell.clear();

	CMesh::Free();
}

