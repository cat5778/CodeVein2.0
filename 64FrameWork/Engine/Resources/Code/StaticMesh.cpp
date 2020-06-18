#include "StaticMesh.h"

USING(Engine)

Engine::CStaticMesh::CStaticMesh(LPDIRECT3DDEVICE9 pGraphicDev)
	: CMesh(pGraphicDev)
{

}

Engine::CStaticMesh::CStaticMesh(const CStaticMesh& rhs)
	: CMesh(rhs)
	, m_pMesh(rhs.m_pMesh)
	, m_pAdjacency(rhs.m_pAdjacency)
	, m_pSubsetBuffer(rhs.m_pSubsetBuffer)
	, m_pMtrl(rhs.m_pMtrl)
	, m_dwSubsetCnt(rhs.m_dwSubsetCnt)
	, m_dwStride(rhs.m_dwStride)
	, m_dwNumVtx(rhs.m_dwNumVtx)
	, m_pVtxPos(rhs.m_pVtxPos)
	, m_pOriMesh(rhs.m_pOriMesh)
{
	m_ppTexture = new LPDIRECT3DTEXTURE9[rhs.m_dwSubsetCnt];

	for (_ulong i = 0; i < rhs.m_dwSubsetCnt; ++i)
	{
		m_ppTexture[i] = rhs.m_ppTexture[i];
		m_ppTexture[i]->AddRef();
	}
	
	m_pMesh->AddRef();
	m_pOriMesh->AddRef();
	m_pAdjacency->AddRef();
	m_pSubsetBuffer->AddRef();
}

Engine::CStaticMesh::~CStaticMesh(void)
{

}

_bool CStaticMesh::Find_Alpha(const char * pFileName)
{
	_uint	iLength = strlen(pFileName);

	for (_uint i = 0; i < iLength; ++i)
	{
		if (pFileName[i] == '.')
		{
			if (pFileName[i - 1] == 'A')
				return true;
		}
	}

	return false;
}

HRESULT Engine::CStaticMesh::Ready_Meshes(const _tchar* pFilePath, const _tchar* pFileName)
{
	_tchar		szFullPath[256] = L"";

	lstrcpy(szFullPath, pFilePath);
	lstrcat(szFullPath, pFileName);

	FAILED_CHECK_RETURN(D3DXLoadMeshFromX(szFullPath, 
											D3DXMESH_MANAGED, 
											m_pGraphicDev, 
											&m_pAdjacency, 
											&m_pSubsetBuffer, 
											NULL, 
											&m_dwSubsetCnt, 
											&m_pOriMesh),
											E_FAIL);

	// 만약에 메쉬에 노말 정보가 없는 경우
	_ulong	dwFVF = m_pOriMesh->GetFVF();	// 메쉬의 정점 정보를 얻어오는 함수

	if (!(dwFVF & D3DFVF_NORMAL))
	{
		// fvf 정보만 채워준 채 메쉬 객체를 생성
		m_pOriMesh->CloneMeshFVF(m_pOriMesh->GetOptions(), dwFVF | D3DFVF_NORMAL, m_pGraphicDev, &m_pMesh);
		
		// 각 폴리곤 또는 정점 기준의 법선을 계산하여 삽입해주는 함수
		D3DXComputeNormals(m_pMesh, (_ulong*)m_pAdjacency->GetBufferPointer());
	}
	else
	{
		m_pOriMesh->CloneMeshFVF(m_pOriMesh->GetOptions(), dwFVF, m_pGraphicDev, &m_pMesh);
	}

	void*		pVertex = nullptr;

	m_dwNumVtx = m_pMesh->GetNumVertices();	// 현재 메쉬가 지닌 정점의 개수를 반환하는 함수

	m_pVtxPos = new _vec3[m_dwNumVtx];

	m_pMesh->LockVertexBuffer(0, &pVertex);

	D3DVERTEXELEMENT9		Decl[MAX_FVF_DECL_SIZE];
	ZeroMemory(Decl, sizeof(D3DVERTEXELEMENT9) * MAX_FVF_DECL_SIZE);

	m_pMesh->GetDeclaration(Decl); // 메쉬 정점 안에 들어있는 fvf 정보를 얻어오는 함수

	_ushort		sOffset = 0;

	for (_ulong i = 0; i < MAX_FVF_DECL_SIZE; ++i)
	{
		if (Decl[i].Usage == D3DDECLUSAGE_POSITION)
		{
			sOffset = Decl[i].Offset;
			break;
		}
	}
	// fvf의 사이즈를 통해 메쉬가 지닌 정점의 크기를 얻어온다
	m_dwStride = D3DXGetFVFVertexSize(dwFVF);

	for (_ulong i = 0; i < m_dwNumVtx; ++i)
	{
		m_pVtxPos[i] = *((_vec3*)(((_ubyte*)pVertex) + (m_dwStride * i + sOffset)));
	}

	m_pMesh->UnlockVertexBuffer();

	m_pMtrl = (D3DXMATERIAL*)m_pSubsetBuffer->GetBufferPointer();
	
	m_ppTexture = new LPDIRECT3DTEXTURE9[m_dwSubsetCnt];

	for (_ulong i = 0; i < m_dwSubsetCnt; ++i)
	{
		_tchar	szFileName[256] = L"";
		lstrcpy(szFullPath, pFilePath);

		MultiByteToWideChar(CP_ACP, 0, m_pMtrl[i].pTextureFilename, strlen(m_pMtrl[i].pTextureFilename), szFileName, 256);

		lstrcat(szFullPath, szFileName);
		D3DXCreateTextureFromFile(m_pGraphicDev, szFullPath, &m_ppTexture[i]);
		//FAILED_CHECK_RETURN(D3DXCreateTextureFromFile(m_pGraphicDev, szFullPath, &m_ppTexture[i]), E_FAIL);

	}

	return S_OK;
}

void Engine::CStaticMesh::Render_Meshes(void)
{
	for (_ulong i = 0; i < m_dwSubsetCnt; ++i)
	{
		m_pGraphicDev->SetTexture(0, m_ppTexture[i]);
		m_pMesh->DrawSubset(i);
	}
}

void CStaticMesh::Render_Meshes(LPD3DXEFFECT & pEffect)
{
	for (_ulong i = 0; i < m_dwSubsetCnt; ++i)
	{
		_bool	bAlpha = false;
		_uint	iPass = 0;

		if (bAlpha = Find_Alpha(m_pMtrl[i].pTextureFilename))
			iPass = 1;

		pEffect->SetTexture("g_BaseTexture", m_ppTexture[i]);
		pEffect->CommitChanges();

		pEffect->BeginPass(iPass);

		m_pMesh->DrawSubset(i);

		pEffect->EndPass();
	}
}

Engine::CStaticMesh* Engine::CStaticMesh::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pFilePath, const _tchar* pFileName)
{
	CStaticMesh*	pInstance = new CStaticMesh(pGraphicDev);

	if (FAILED(pInstance->Ready_Meshes(pFilePath, pFileName)))
		Safe_Release(pInstance);

	return pInstance;
}

Engine::CResources* Engine::CStaticMesh::Clone(void)
{
	return new CStaticMesh(*this);
}

void Engine::CStaticMesh::Free(void)
{
	CMesh::Free();

	for (_ulong i = 0; i < m_dwSubsetCnt; ++i)
		Safe_Release(m_ppTexture[i]);

	Safe_Delete_Array(m_ppTexture);

	if (false == m_bClone)
		Safe_Delete_Array(m_pVtxPos);

	Safe_Release(m_pAdjacency);
	Safe_Release(m_pSubsetBuffer);
	Safe_Release(m_pMesh);
	Safe_Release(m_pOriMesh);
}

