#include "TerrainTex.h"

USING(Engine)

Engine::CTerrainTex::CTerrainTex(LPDIRECT3DDEVICE9 pGraphicDev)
	: CVIBuffer(pGraphicDev)
	, m_pPos(nullptr)
	, m_bClone(false)
{

}

Engine::CTerrainTex::CTerrainTex(const CTerrainTex& rhs)
	: CVIBuffer(rhs)
	, m_pPos(rhs.m_pPos)
	, m_bClone(true)
	, m_fH(rhs.m_fH)
	, m_iH(rhs.m_iH)
{

}

Engine::CTerrainTex::~CTerrainTex(void)
{

}

HRESULT Engine::CTerrainTex::Ready_Buffer(const _ulong& dwCntX,
											const _ulong& dwCntZ,
											const _ulong dwVtxItv)
{
	m_dwVtxSize = sizeof(VTXTEX);
	m_dwVtxCnt = dwCntX * dwCntZ;
	m_dwVtxFVF = FVF_TEX;
	m_dwTriCnt = (dwCntX - 1) * (dwCntZ - 1) * 2;

	m_pPos = new _vec3[m_dwVtxCnt];

	m_dwIdxSize = sizeof(INDEX32);
	m_IdxFmt = D3DFMT_INDEX32;

	FAILED_CHECK_RETURN(CVIBuffer::Ready_Buffer(), E_FAIL);


	_ulong	dwByte = 0;

	m_hFile = CreateFile(L"../../Resource/Texture/Terrain/Height2.bmp", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	ReadFile(m_hFile, &m_fH, sizeof(BITMAPFILEHEADER), &dwByte, NULL);
	ReadFile(m_hFile, &m_iH, sizeof(BITMAPINFOHEADER), &dwByte, NULL);

	_ulong*		pPixel = new _ulong[m_iH.biHeight * m_iH.biWidth];

	ReadFile(m_hFile, pPixel, sizeof(_ulong) * m_iH.biHeight * m_iH.biWidth, &dwByte, NULL);

	CloseHandle(m_hFile);

	_ulong			dwIndex = 0;
	VTXTEX*			pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);		// 1. 메모리 잠그는 역할, 2. 3인자를 통해서 저장된 버텍스 접근 권한을 얻음
													// 오른쪽 위 
	for (_ulong z = 0; z < dwCntZ; ++z)
	{
		for (_ulong x = 0; x < dwCntX; ++x)
		{
			dwIndex = z * dwCntX + x;

			pVertex[dwIndex].vPos = _vec3(_float(x * dwVtxItv),
											(pPixel[dwIndex] & 0x000000ff) / 20.f,
											_float(z * dwVtxItv));

			m_pPos[dwIndex] = pVertex[dwIndex].vPos;

			pVertex[dwIndex].vNormal = _vec3(0.f, 0.f, 0.f);

			pVertex[dwIndex].vTexUV = _vec2(_float(x) / (dwCntX - 1) * 20.f,
											_float(z) / (dwCntZ - 1) * 20.f);
		}
	}

	
	Safe_Delete_Array(pPixel);

	_ulong	dwTriCnt = 0;

	INDEX32*		pIndex = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	for (_ulong z = 0; z < dwCntZ - 1; ++z)
	{
		for (_ulong x = 0; x < dwCntX - 1; ++x)
		{
			dwIndex = z * dwCntX + x;

			// 오른쪽 위
			pIndex[dwTriCnt]._0 = dwIndex + dwCntX;
			pIndex[dwTriCnt]._1 = dwIndex + dwCntX + 1;
			pIndex[dwTriCnt]._2 = dwIndex + 1;

			_vec3		vDest, vSour, vNormal;

			vDest = pVertex[pIndex[dwTriCnt]._1].vPos - pVertex[pIndex[dwTriCnt]._0].vPos;
			vSour = pVertex[pIndex[dwTriCnt]._2].vPos - pVertex[pIndex[dwTriCnt]._1].vPos;
			D3DXVec3Cross(&vNormal, &vDest, &vSour);
			
			pVertex[pIndex[dwTriCnt]._0].vNormal += vNormal;
			pVertex[pIndex[dwTriCnt]._1].vNormal += vNormal;
			pVertex[pIndex[dwTriCnt]._2].vNormal += vNormal;
			dwTriCnt++;
			
			// 왼쪽 아래
			pIndex[dwTriCnt]._0 = dwIndex + dwCntX;
			pIndex[dwTriCnt]._1 = dwIndex + 1;
			pIndex[dwTriCnt]._2 = dwIndex;

			vDest = pVertex[pIndex[dwTriCnt]._1].vPos - pVertex[pIndex[dwTriCnt]._0].vPos;
			vSour = pVertex[pIndex[dwTriCnt]._2].vPos - pVertex[pIndex[dwTriCnt]._1].vPos;
			D3DXVec3Cross(&vNormal, &vDest, &vSour);

			pVertex[pIndex[dwTriCnt]._0].vNormal += vNormal;
			pVertex[pIndex[dwTriCnt]._1].vNormal += vNormal;
			pVertex[pIndex[dwTriCnt]._2].vNormal += vNormal;

			dwTriCnt++;
		}
	}

	for (_ulong i = 0; i < m_dwVtxCnt; ++i)
		D3DXVec3Normalize(&pVertex[i].vNormal, &pVertex[i].vNormal);

	m_pVB->Unlock();
	m_pIB->Unlock();

	return S_OK;
}

Engine::CResources* Engine::CTerrainTex::Clone(void)
{
	return new CTerrainTex(*this);
}

Engine::CTerrainTex* Engine::CTerrainTex::Create(LPDIRECT3DDEVICE9 pGraphicDev,
												const _ulong& dwCntX,
												const _ulong& dwCntZ,
												const _ulong& dwVtxItv)
{
	CTerrainTex*	pInstance = new CTerrainTex(pGraphicDev);

	if (FAILED(pInstance->Ready_Buffer(dwCntX, dwCntZ, dwVtxItv)))
		Safe_Release(pInstance);

	return pInstance;
}

void Engine::CTerrainTex::Free(void)
{
	CVIBuffer::Free();

	if (false == m_bClone)
		Safe_Delete_Array(m_pPos);
}

