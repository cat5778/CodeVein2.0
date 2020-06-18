#include "Cell.h"
#include "Line.h"
#define NAV_RADIUS 0.1f
USING(Engine)

Engine::CCell::CCell(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
{
	m_pGraphicDev->AddRef();
	ZeroMemory(m_pNeighbor, sizeof(CCell*) * NEIGHBOR_END);
}

Engine::CCell::~CCell(void)
{

}

void CCell::Set_NaviData(Engine::NAVI_DATA naviData)
{
	m_vPoint[POINT_A] = naviData.vPosition1;
	m_vPoint[POINT_B] = naviData.vPosition2;
	m_vPoint[POINT_C] = naviData.vPosition3;
	m_uiIdx= naviData.uiIdx;
	for (int i = 0; i < POINT_END; i++)
		m_tSphereData[i].vPosition = m_vPoint[i];



}

void CCell::Set_NaviData(Engine::CCell::POINT ePoint,_vec3 vPos)
{
	switch (ePoint)
	{
	case Engine::CCell::POINT_A:
		m_vPoint[POINT_A] = vPos;
		m_tSphereData[POINT_A].vPosition = vPos;
		break;
	case Engine::CCell::POINT_B:
		m_vPoint[POINT_B] = vPos;
		m_tSphereData[POINT_B].vPosition = vPos;
		break;
	case Engine::CCell::POINT_C:
		m_vPoint[POINT_C] = vPos;
		m_tSphereData[POINT_C].vPosition = vPos;
		break;
	default:
		break;
	}

}

Engine::NAVI_DATA CCell::Get_NaviData()
{
	Engine::NAVI_DATA naviMesh;
	naviMesh.vPosition1 = m_vPoint[POINT_A];
	naviMesh.vPosition2 = m_vPoint[POINT_B];
	naviMesh.vPosition3 = m_vPoint[POINT_C];
	naviMesh.uiIdx = m_uiIdx;
	return naviMesh;
}

HRESULT Engine::CCell::Ready_Cell(const _ulong& dwIndex, const _vec3* pPointA, const _vec3* pPointB, const _vec3* pPointC)
{
	m_dwIndex = dwIndex;

	m_vPoint[POINT_A] = *pPointA;
	m_vPoint[POINT_B] = *pPointB;
	m_vPoint[POINT_C] = *pPointC;

	m_pLine[LINE_AB] = CLine::Create(&_vec3(m_vPoint[POINT_A].x,m_vPoint[POINT_A].y, m_vPoint[POINT_A].z), 
									 &_vec3(m_vPoint[POINT_B].x,m_vPoint[POINT_B].y, m_vPoint[POINT_B].z));

	m_pLine[LINE_BC] = CLine::Create(&_vec3(m_vPoint[POINT_B].x,m_vPoint[POINT_B].y, m_vPoint[POINT_B].z),
									 &_vec3(m_vPoint[POINT_C].x,m_vPoint[POINT_C].y, m_vPoint[POINT_C].z));

	m_pLine[LINE_CA] = CLine::Create(&_vec3(m_vPoint[POINT_C].x,m_vPoint[POINT_C].y, m_vPoint[POINT_C].z),
									 &_vec3(m_vPoint[POINT_A].x,m_vPoint[POINT_A].y, m_vPoint[POINT_A].z));
	

#ifdef _DEBUG

	LPD3DXMESH	pSphereMesh = nullptr;
	VTXCOL* pVertices = nullptr;

	D3DXCreateSphere(m_pGraphicDev, 1.f, 10, 10, &pSphereMesh, nullptr);

	for (int i = 0; i < POINT_END; i++)
	{	
		LPDIRECT3DVERTEXBUFFER9 pSphereBuffer ;

		m_tSphereData[i].fRadius = NAV_RADIUS;
		m_tSphereData[i].vPosition = m_vPoint[i];
		pSphereMesh->CloneMeshFVF(0, FVF_COL, m_pGraphicDev, &m_pSphereMesh[i]);
		m_pSphereMesh[i]->GetVertexBuffer(&pSphereBuffer);
		_uint uiNumVtx = m_pSphereMesh[i]->GetNumVertices();

		pSphereBuffer->Lock(0, 0, (void**)&pVertices, 0);

		for (_uint i = 0; i < uiNumVtx; i++)
			pVertices[i].dwColor = D3DCOLOR_RGBA(0, 255, 0, 255);

		pSphereBuffer->Unlock();
		pSphereBuffer->Release();

	}
	pSphereMesh->Release();


	FAILED_CHECK_RETURN(D3DXCreateLine(m_pGraphicDev, &m_pD3DXLine), E_FAIL);
#endif

	return S_OK;
}

_bool Engine::CCell::Compare_Point(const _vec3* pPointF, const _vec3* pPointS, CCell* pCell)
{
	if (m_vPoint[POINT_A] == *pPointF)
	{
		if (m_vPoint[POINT_B] == *pPointS)
		{
			m_pNeighbor[NEIGHBOR_AB] = pCell;
			return true;
		}
		if (m_vPoint[POINT_C] == *pPointS)
		{
			m_pNeighbor[NEIGHBOR_CA] = pCell;
			return true;
		}
	}

	if (m_vPoint[POINT_B] == *pPointF)
	{
		if (m_vPoint[POINT_A] == *pPointS)
		{
			m_pNeighbor[NEIGHBOR_AB] = pCell;
			return true;
		}
		if (m_vPoint[POINT_C] == *pPointS)
		{
			m_pNeighbor[NEIGHBOR_BC] = pCell;
			return true;
		}
	}

	if (m_vPoint[POINT_C] == *pPointF)
	{
		if (m_vPoint[POINT_B] == *pPointS)
		{
			m_pNeighbor[NEIGHBOR_BC] = pCell;
			return true;
		}
		if (m_vPoint[POINT_A] == *pPointS)
		{
			m_pNeighbor[NEIGHBOR_CA] = pCell;
			return true;
		}
	}


	return false;
}

void Engine::CCell::Render_Cell(void)
{
	//TODO: Debug Sphere Check


	//m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	//m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	_vec3	vPoint[4];

	vPoint[0] = m_vPoint[POINT_A];
	vPoint[1] = m_vPoint[POINT_B];
	vPoint[2] = m_vPoint[POINT_C];
	vPoint[3] = m_vPoint[POINT_A];

	_matrix		matView, matProj;

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	for (_ulong i = 0; i < 4; ++i)
	{
		D3DXVec3TransformCoord(&vPoint[i], &vPoint[i], &matView);

		if (vPoint[i].z <= 0.1f)
			vPoint[i].z = 0.1f;
		D3DXVec3TransformCoord(&vPoint[i], &vPoint[i], &matProj);

	}


#ifdef _DEBUG
	m_pD3DXLine->SetWidth(3.f);	// 라인의 굵기를 결정하는 함수
	m_pGraphicDev->EndScene();
	m_pGraphicDev->BeginScene();

	m_pD3DXLine->Begin();


	_matrix matTemp;

	m_pD3DXLine->DrawTransform(vPoint, 4, D3DXMatrixIdentity(&matTemp), D3DXCOLOR(1.f, 0.f, 0.f, 1.f));

	m_pD3DXLine->End();



	m_pGraphicDev->BeginScene();
	_matrix MatoldWorld;
	m_pGraphicDev->GetTransform(D3DTS_WORLD, &MatoldWorld);

	for (int i = 0; i < 3; i++)
	{
		if (m_pSphereMesh[i] == nullptr)
			continue;

		_matrix  matScale, matTrans;
		D3DXMatrixTranslation(&matTrans, m_tSphereData[i].vPosition.x, m_tSphereData[i].vPosition.y, m_tSphereData[i].vPosition.z);
		D3DXMatrixScaling(&matScale, m_tSphereData[i].fRadius, m_tSphereData[i].fRadius, m_tSphereData[i].fRadius);
		m_matSphereWorld[i] = matScale * matTrans;

		m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matSphereWorld[i]);

		m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		m_pSphereMesh[i]->DrawSubset(0);
		m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	}
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &MatoldWorld);

	m_pGraphicDev->EndScene();
#endif


}

Engine::CCell* Engine::CCell::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _ulong& dwIndex, const _vec3* pPointA, const _vec3* pPointB, const _vec3* pPointC)
{
	CCell*	pInstance = new CCell(pGraphicDev);

	if (FAILED(pInstance->Ready_Cell(dwIndex, pPointA, pPointB, pPointC)))
		Safe_Release(pInstance);

	return pInstance;
}

void Engine::CCell::Free(void)
{
	for (_uint i = 0; i < LINE_END; ++i)
		Safe_Release(m_pLine[i]);
#ifdef _DEBUG
	for (_uint i = 0; i < POINT_END; i++)
		Safe_Release(m_pSphereMesh[i]);
	Safe_Release(m_pD3DXLine);
#endif

	Safe_Release(m_pGraphicDev);
}

Engine::CCell::COMPARE Engine::CCell::Compare(const _vec3* pEndPos, _ulong* pIndex)
{
	for (_ulong i = 0; i < LINE_END; ++i)
	{
		if (CLine::COMPARE_LEFT == m_pLine[i]->Compare(&_vec3(pEndPos->x, pEndPos->y, pEndPos->z)))
		{
			if (nullptr == m_pNeighbor[i])
				return COMPARE_STOP;
			else
			{
				*pIndex = *m_pNeighbor[i]->Get_Index();
				return COMPARE_MOVE;
			}
		}
	}

	return COMPARE_MOVE;
}

