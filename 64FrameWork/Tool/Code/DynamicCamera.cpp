#include "stdafx.h"
#include "DynamicCamera.h"

#include "Export_Function.h"
#include "StaticObject.h"
#include "StaticMesh.h"
CDynamicCamera::CDynamicCamera(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CCamera(pGraphicDev)
	, m_bClick(false)
	, m_bMouseFix(true)
{

}

CDynamicCamera::~CDynamicCamera(void)
{

}

HRESULT CDynamicCamera::Ready_GameObject(const _vec3* pEye, const _vec3* pAt, const _vec3* pUp, const _float& fFovY, const _float& fAspect, const _float& fNear, const _float& fFar, const _bool& bMainCamera)
{
	m_vEye = *pEye;
	m_vAt = *pAt;
	m_vUp = *pUp;

	m_fFovY = fFovY;
	m_fAspect = fAspect;
	m_fNear = fNear;
	m_fFar = fFar;
	m_fSpeed = 20.f;

	FAILED_CHECK_RETURN(Engine::CCamera::Ready_GameObject(), E_FAIL);
	m_pKeyMgr = CKeyMgr::GetInstance();
	
	return S_OK;
}

_int CDynamicCamera::Update_GameObject(const _float& fTimeDelta)
{
	m_pKeyMgr->Update();

	Key_Input(fTimeDelta);

	m_bIsNavPick = false;
	if (m_pKeyMgr->KeyUp(KEY_LBUTTON))
	{
	if (m_iPickMode == 0)
		Picking_Mesh();
		else if (m_iPickMode == 1)
		{
			Pickint_Nav();
			if (!m_bIsNavPick)
				Picking_Mesh();
		}

	}
	else
	{
		m_bIsPick = false;
	}




	_int iExit = Engine::CCamera::Update_GameObject(fTimeDelta);

	return iExit;
}

void CDynamicCamera::Init_PickName()
{
	m_wstrPickName = L"No Pick";
}

void CDynamicCamera::Key_Input(const _float& fTimeDelta)
{
	_matrix	matCamWorld;
	D3DXMatrixInverse(&matCamWorld, NULL, &m_matView);


	if (Engine::Get_DIKeyState(DIK_W)&0x80)
	{
		_vec3	vLook;
		memcpy(&vLook, &matCamWorld.m[2][0], sizeof(_vec3));

		_vec3 vLength = *D3DXVec3Normalize(&vLook, &vLook) * m_fSpeed* fTimeDelta;

		m_vEye += vLength;
		m_vAt += vLength;
	}

	if (Engine::Get_DIKeyState(DIK_S) & 0x80)
	{
		_vec3	vLook;
		memcpy(&vLook, &matCamWorld.m[2][0], sizeof(_vec3));

		_vec3 vLength = *D3DXVec3Normalize(&vLook, &vLook) * m_fSpeed* fTimeDelta;

		m_vEye -= vLength;
		m_vAt -= vLength;
	}
	if (Engine::Get_DIKeyState(DIK_A) & 0x80)
	{
		_vec3	vRight;
		memcpy(&vRight, &matCamWorld.m[0][0], sizeof(_vec3));

		_vec3 vLength = *D3DXVec3Normalize(&vRight, &vRight) * m_fSpeed * fTimeDelta;

		m_vEye -= vLength;
		m_vAt -= vLength;
	}

	if (Engine::Get_DIKeyState(DIK_D) & 0x80)
	{
		_vec3	vRight;
		memcpy(&vRight, &matCamWorld.m[0][0], sizeof(_vec3));

		_vec3 vLength = *D3DXVec3Normalize(&vRight, &vRight) * m_fSpeed * fTimeDelta;

		m_vEye += vLength;
		m_vAt += vLength;
	}
	if (m_pKeyMgr->KeyDown(KEY_O))
	{
		m_iPickMode ? m_iPickMode = 0 : m_iPickMode = 1;
	}
	if (Engine::Get_DIKeyState(DIK_TAB) & 0x80)
	{
		if (true == m_bClick)
			return;

		m_bClick = true;

		if (true == m_bMouseFix)
			m_bMouseFix = false;

		else
			m_bMouseFix = true;

	}
	else
		m_bClick = false;

	if (Engine::Get_DIMouseState(Engine::DIM_RB) & 0x80)
	{
		Mouse_Move(fTimeDelta);
	}

	/*if (Engine::Get_DIMouseState(Engine::DIM_LB) & 0x80)
	{
		if (m_iPickMode == 0)
			Picking_Mesh();
		else if (m_iPickMode == 1)
		{
			Pickint_Nav();
			if(!m_bIsNavPick)
				Picking_Mesh();
		}

	}*/



	if (false == m_bMouseFix)
		return;
}

void CDynamicCamera::Mouse_Move(const _float& fTimeDelta)
{
	_long	dwMouseMove = 0;

	_matrix	matCamWorld;
	D3DXMatrixInverse(&matCamWorld, NULL, &m_matView);

	if (dwMouseMove = Engine::Get_DIMouseMove(Engine::DIMS_Y))
	{
		_vec3	vRight;
		memcpy(&vRight, &matCamWorld.m[0][0], sizeof(_vec3));

		_vec3	vLook = m_vAt - m_vEye;

		_matrix		matRotAxis;
		D3DXMatrixRotationAxis(&matRotAxis, &vRight, D3DXToRadian(dwMouseMove / 10.f));

		D3DXVec3TransformNormal(&vLook, &vLook, &matRotAxis);

		m_vAt = m_vEye + vLook;
	}

	if (dwMouseMove = Engine::Get_DIMouseMove(Engine::DIMS_X))
	{
		_vec3	vUp = _vec3(0.f, 1.f, 0.f);

		_vec3	vLook = m_vAt - m_vEye;

		_matrix		matRotAxis;
		D3DXMatrixRotationAxis(&matRotAxis, &vUp, D3DXToRadian(dwMouseMove / 10.f));

		D3DXVec3TransformNormal(&vLook, &vLook, &matRotAxis);

		m_vAt = m_vEye + vLook;
	}

}

void CDynamicCamera::Mouse_Fix(void)
{
	POINT		ptMouse{ WINCX >> 1, WINCY >> 1 };

	ClientToScreen(g_hWnd, &ptMouse);
	SetCursorPos(ptMouse.x, ptMouse.y);
}

void CDynamicCamera::Picking_Mesh()
{
	
	m_wstrPickName = L"No Pick";
	m_pPickTransform = nullptr;
	m_bIsPick = false;

	POINT	ptMouse{};
	RAY		tRay;
	m_vPickPos = { 0.f, 0.f,0.f };
	// 윈도우 영역 == 뷰포트 영역
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);
	if (ptMouse.x<0 || ptMouse.x>WINCX || ptMouse.y < 0 ||ptMouse.y>WINCY)
		return;

	// 뷰 포트에서 투영으로
	D3DVIEWPORT9			ViewPort;
	ZeroMemory(&ViewPort, sizeof(D3DVIEWPORT9));
	m_pGraphicDev->GetViewport(&ViewPort);

	_vec3	vMousePos;
	vMousePos.x = ptMouse.x / (ViewPort.Width * 0.5f) - 1.f;
	vMousePos.y = -ptMouse.y / (ViewPort.Height * 0.5f) + 1.f;
	vMousePos.z = 0.f;

	// 투영에서 뷰 스페이스로
	_matrix		matProj;
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
	D3DXMatrixInverse(&matProj, NULL, &matProj);
	D3DXVec3TransformCoord(&vMousePos, &vMousePos, &matProj);

	tRay.vPos= _vec3(0.f, 0.f, 0.f);
	tRay.vDir= vMousePos - tRay.vPos;

	// 뷰 스페이스에서 월드 스페이스로
	_matrix		matView;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, NULL, &matView);
	D3DXVec3TransformCoord(&tRay.vPos, &tRay.vPos, &matView);
	D3DXVec3TransformNormal(&tRay.vDir, &tRay.vDir, &matView);

	FLOAT fMinDist=INT_MAX;
	_matrix PickWorldMat;
	RAY tPickRay;

	for (auto pGameObject : Engine::Get_Layer(L"GameLogic")->Get_ObjectMap())
	{
		if (pGameObject.first.find(L"Map") != wstring::npos)
			return;
		CStaticObject* pStaticObject = dynamic_cast<CStaticObject*>(pGameObject.second);
		if (pStaticObject == nullptr)
			continue;


		Engine::CComponent* pComponent = pStaticObject->Get_Component(L"Com_StaticMesh", Engine::ID_STATIC);
		Engine::CTransform* pTransform = dynamic_cast<Engine::CTransform*>(pStaticObject->Get_Component(L"Com_Transform", Engine::ID_DYNAMIC));
		Engine::CStaticMesh* pStaticMesh = dynamic_cast<Engine::CStaticMesh*>(pComponent);
		
		LPD3DXMESH pMesh;
		pStaticMesh->Get_Mesh()->CloneMeshFVF(D3DXMESH_MANAGED,
											pStaticMesh->Get_Mesh()->GetFVF(),
											m_pGraphicDev,
											&pMesh);
		if (pMesh == nullptr)
			continue;
		_matrix pObjWorldMat = pTransform->m_matWorld;
		_matrix pObjMatIvs;
		
		RAY tConvertRay;
		D3DXMatrixInverse(&pObjMatIvs, NULL, &pObjWorldMat);
		D3DXVec3TransformCoord(&tConvertRay.vPos, &tRay.vPos, &pObjMatIvs);
		D3DXVec3TransformNormal(&tConvertRay.vDir, &tRay.vDir, &pObjMatIvs);

		LPDIRECT3DVERTEXBUFFER9 pVB;
		LPDIRECT3DINDEXBUFFER9 pIB;
		D3DXVECTOR3 v01, v02, v03;  // Need Convert WorldPos Value
		struct VERT {D3DXVECTOR3 p; D3DXVECTOR3 n; float tu, tv; };

		WORD*  pIndices;
		VERT*  pVertices;
		pMesh->GetVertexBuffer(&pVB);
		pMesh->GetIndexBuffer(&pIB);

		pIB->Lock(0, 0, (void**)&pIndices, 0);
		pVB->Lock(0, 0, (void**)&pVertices, 0);
		_float fU, fV, fDist;

		for (UINT i = 0; i < pMesh->GetNumFaces(); i++)
		{
			D3DXVECTOR3 v0 = pVertices[pIndices[3 * i + 0]].p;
			D3DXVECTOR3 v1 = pVertices[pIndices[3 * i + 1]].p;
			D3DXVECTOR3 v2 = pVertices[pIndices[3 * i + 2]].p;

			if(D3DXIntersectTri(&v0, &v1, &v2, &tConvertRay.vPos, &tConvertRay.vDir,&fU, &fV, &fDist))
			{
				if (fDist<fMinDist)
				{
					m_wstrPickName = pStaticObject->Get_ObjName() + L"_"+to_wstring(pStaticObject->Get_ObjIdx());
					fMinDist = fDist;
					m_bIsPick = true;
					PickWorldMat = pObjWorldMat;
					tPickRay = tConvertRay;
					m_pPickTransform = pTransform;
				}
			}
		}

		pVB->Unlock();
		pIB->Unlock();

		Safe_Release(pVB);
		Safe_Release(pIB);
		Safe_Release(pMesh);

	}
	if (m_bIsPick)
	{
		m_vPickPos = tPickRay.vPos + tPickRay.vDir * fMinDist;
		D3DXVec3TransformCoord(&m_vPickPos, &m_vPickPos, &PickWorldMat);
	}

}

void CDynamicCamera::Pickint_Nav()
{
	m_wstrPickName = L"No Pick";
	m_pPickTransform = nullptr;
	m_bIsNavPick = false;

	POINT	ptMouse{};
	RAY		tRay;
	m_vPickPos = { 0.f, 0.f,0.f };
	// 윈도우 영역 == 뷰포트 영역
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);
	if (ptMouse.x<0 || ptMouse.x>WINCX || ptMouse.y < 0 || ptMouse.y>WINCY)
		return;

	// 뷰 포트에서 투영으로
	D3DVIEWPORT9			ViewPort;
	ZeroMemory(&ViewPort, sizeof(D3DVIEWPORT9));
	m_pGraphicDev->GetViewport(&ViewPort);

	_vec3	vMousePos;
	vMousePos.x = ptMouse.x / (ViewPort.Width * 0.5f) - 1.f;
	vMousePos.y = -ptMouse.y / (ViewPort.Height * 0.5f) + 1.f;
	vMousePos.z = 0.f;

	// 투영에서 뷰 스페이스로
	_matrix		matProj;
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
	D3DXMatrixInverse(&matProj, NULL, &matProj);
	D3DXVec3TransformCoord(&vMousePos, &vMousePos, &matProj);

	tRay.vPos = _vec3(0.f, 0.f, 0.f);
	tRay.vDir = vMousePos - tRay.vPos;

	// 뷰 스페이스에서 월드 스페이스로
	_matrix		matView;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, NULL, &matView);
	D3DXVec3TransformCoord(&tRay.vPos, &tRay.vPos, &matView);
	D3DXVec3TransformNormal(&tRay.vDir, &tRay.vDir, &matView);

	FLOAT fMinDist = INT_MAX;
	_matrix PickWorldMat;
	RAY tPickRay;
	m_ppCellVec = &dynamic_cast<Engine::CNaviMesh*>(Engine::Get_Layer(L"GameLogic")->Get_GameObject(L"Player")->Get_Component(L"Com_Navi", Engine::ID_STATIC))->Get_CellVec();
	int iCellIdx = 0;
	int iPoint = 0;
	int iMinCellIdx = 0;
	int iMinPointIdx = 0;
	_vec3 vNavPos = { INIT_VEC3 };
	for (auto pCell : (*m_ppCellVec))//TODO: 피킹 이함함 체크
	{
		if (pCell == nullptr)
			continue;

		for (iPoint = 0; iPoint < Engine::CCell::POINT_END; iPoint++)
		{
			
			if (pCell->Get_SphereMesh(iPoint) == nullptr)
				continue;
			LPD3DXMESH pMesh;
			pCell->Get_SphereMesh(iPoint)->CloneMeshFVF(D3DXMESH_MANAGED,
				pCell->Get_SphereMesh(iPoint)->GetFVF(),
				m_pGraphicDev,
				&pMesh);
			if (pMesh == nullptr)
				continue;

			_matrix pCellWorldMat = pCell->Get_MatWorldSphere(iPoint);
			_matrix pCellMatIvs;

			RAY tConvertRay;
			D3DXMatrixInverse(&pCellMatIvs, NULL, &pCellWorldMat);
			D3DXVec3TransformCoord(&tConvertRay.vPos, &tRay.vPos, &pCellMatIvs);
			D3DXVec3TransformNormal(&tConvertRay.vDir, &tRay.vDir, &pCellMatIvs);

			LPDIRECT3DVERTEXBUFFER9 pVB;
			LPDIRECT3DINDEXBUFFER9 pIB;
			D3DXVECTOR3 v01, v02, v03;  // Need Convert WorldPos Value
			struct VERT { D3DXVECTOR3 p; D3DXVECTOR3 n; float tu, tv; };

			WORD*  pIndices;
			VERT*  pVertices;
			pMesh->GetVertexBuffer(&pVB);
			pMesh->GetIndexBuffer(&pIB);
			pIB->Lock(0, 0, (void**)&pIndices, 0);
			pVB->Lock(0, 0, (void**)&pVertices, 0);
			_float fU, fV, fDist;
			for (UINT i = 0; i < pMesh->GetNumFaces(); i++)
			{
				D3DXVECTOR3 v0 = pVertices[pIndices[3 * i + 0]].p;
				D3DXVECTOR3 v1 = pVertices[pIndices[3 * i + 1]].p;
				D3DXVECTOR3 v2 = pVertices[pIndices[3 * i + 2]].p;

				if (D3DXIntersectTri(&v0, &v1, &v2, &tConvertRay.vPos, &tConvertRay.vDir, &fU, &fV, &fDist))
				{
					cout << "피킹" << endl;

					if (fDist < fMinDist)
					{
						m_wstrPickName = L"NavPoint " + to_wstring(iCellIdx) + L"_" + to_wstring(iPoint);
						fMinDist = fDist;
						m_bIsNavPick = true;

						iMinCellIdx = iCellIdx;
						iMinPointIdx = iPoint;
						break;

					}
				}
			}
			pVB->Unlock();
			pIB->Unlock();

			Safe_Release(pVB);
			Safe_Release(pIB);
			Safe_Release(pMesh);

		}
		iCellIdx++;
	
	}
	if (m_bIsNavPick)
	{
		m_bIsPick = false;
		m_vPickPos = *(*m_ppCellVec)[iMinCellIdx]->Get_Point((Engine::CCell::POINT)iMinPointIdx);
		m_wstrPickName = L"NavPoint " + to_wstring(iMinCellIdx) + L"_" + to_wstring(iMinPointIdx);

		cout << m_vPickPos.x << m_vPickPos.y << m_vPickPos.z << endl;
		return;
	}

}

CDynamicCamera* CDynamicCamera::Create(LPDIRECT3DDEVICE9 pGraphicDev,
	const _vec3* pEye, const _vec3* pAt, const _vec3* pUp,
	const _float& fFovY, const _float& fAspect,
	const _float& fNear, const _float& fFar,
	const _bool& bMainCamera)
{
	CDynamicCamera*	pInstance = new CDynamicCamera(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(pEye, pAt, pUp, fFovY, fAspect, fNear, fFar, bMainCamera)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CDynamicCamera::Free(void)
{
	Engine::CCamera::Free();
}

