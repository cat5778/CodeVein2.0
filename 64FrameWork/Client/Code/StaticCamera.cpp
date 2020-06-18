#include "stdafx.h"
#include "StaticCamera.h"
#include "Export_Function.h"

CStaticCamera::CStaticCamera(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CCamera(pGraphicDev)
{

}

CStaticCamera::~CStaticCamera(void)
{

}

void CStaticCamera::SetUp_Target(Engine::CTransform* pTargetInfo)
{
	m_pTargetInfo = pTargetInfo;
}

HRESULT CStaticCamera::Ready_GameObject(const _vec3* pEye,
	const _vec3* pAt,
	const _vec3* pUp,
	const _float& fFovY,
	const _float& fAspect,
	const _float& fNear,
	const _float& fFar)
{
	m_vEye = *pEye;
	m_vAt = *pAt;
	m_vUp = *pUp;	 

	m_fFovY = fFovY;
	m_fAspect = fAspect;
	m_fNear = fNear;
	m_fFar = fFar;

	FAILED_CHECK_RETURN(Engine::CCamera::Ready_GameObject(), E_FAIL);


	return S_OK;
}

_int CStaticCamera::Update_GameObject(const _float& fTimeDelta)
{
	if (m_pTargetInfo == nullptr)
		m_pTargetInfo = dynamic_cast<Engine::CTransform*>(Engine::Get_Component(L"GameLogic", L"Player", L"Com_Transform", Engine::ID_DYNAMIC));

	Target_Renewal(fTimeDelta);
	Mouse_Move(fTimeDelta);
	Key_Input(fTimeDelta);
	_int iExit = Engine::CCamera::Update_GameObject(fTimeDelta);



	return iExit;
}

void CStaticCamera::Key_Input(const _float& fTimeDelta)
{

	//if (GetAsyncKeyState(VK_LEFT) & 0x8000)
	//	m_pTransformCom->Rotation(Engine::ROT_Y, D3DXToRadian(90.f * fTimeDelta));

	//if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
	//	m_pTransformCom->Rotation(Engine::ROT_Y, D3DXToRadian(-90.f * fTimeDelta));

	if (CKeyMgr::GetInstance()->KeyDown(KEY_NUM1))
	{
		m_fDistance += 0.5f;
		cout << m_fDistance << endl;
	}
	if (CKeyMgr::GetInstance()->KeyDown(KEY_NUM2))
	{
		m_fDistance -= 0.5f;
		cout << m_fDistance << endl;
	}
	if (CKeyMgr::GetInstance()->KeyDown(KEY_NUM3))
	{
		m_fZ += 10.0f;
		cout << m_fZ << endl;
	}
	if (CKeyMgr::GetInstance()->KeyDown(KEY_NUM4))
	{
		m_fZ -= 10.0f;
		cout << m_fZ << endl;
	}
	if (CKeyMgr::GetInstance()->KeyDown(KEY_UP))
	{
		m_fUp += 1.0f;
		cout << m_fUp << endl;
	}
	if (CKeyMgr::GetInstance()->KeyDown(KEY_DOWN))
	{
		m_fUp -= 1.0f;
		cout << m_fUp << endl;

	}
}

void CStaticCamera::Target_Renewal(const _float& fTimeDelta)
{
	if (nullptr == m_pParentBoneMatrix)
	{
		Engine::CDynamicMesh*	pPlayerMeshCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::Get_Component(L"GameLogic", L"Player", L"Com_Mesh", Engine::ID_STATIC));
		if (pPlayerMeshCom == nullptr)
			return;

		const Engine::D3DXFRAME_DERIVED* pBone = pPlayerMeshCom->Get_FrameByName("Head");
		if (pBone == nullptr)
			return;

		m_pParentBoneMatrix = &pBone->CombinedTransformationMatrix;

		Engine::CTransform*	pPlayerTransCom = dynamic_cast<Engine::CTransform*>(Engine::Get_Component(L"GameLogic", L"Player", L"Com_Transform", Engine::ID_DYNAMIC));

		m_pParentWorldMatrix = m_pTargetInfo->Get_WorldMatrixPointer();
		m_vOldEye= m_pTargetInfo->m_vInfo[Engine::INFO_LOOK] * -1.f;
	}
	
	m_matWorld = (*m_pParentBoneMatrix * *m_pParentWorldMatrix);

	_vec3 vHeadPos;
	memcpy(&vHeadPos, &m_matWorld.m[3][0], sizeof(_vec3));
	m_vEye = m_vOldEye;
	D3DXVec3Normalize(&m_vEye, &m_vEye);

	m_vEye *= m_fDistance;

	_vec3		vRight;
	memcpy(&vRight, &m_pTargetInfo->m_matWorld.m[0][0], sizeof(_vec3));

	_matrix		matRotAxis;
	D3DXMatrixRotationAxis(&matRotAxis, &vRight, m_fVerticalAngle);
	D3DXVec3TransformNormal(&m_vEye, &m_vEye, &matRotAxis);

	m_vEye += vHeadPos;
	m_vAt = vHeadPos; 

	_vec3	vUp = _vec3(0.f, 1.f, 0.f);
	_vec3	vCamPos = m_vEye - m_vAt;
	matRotAxis;

	D3DXMatrixRotationAxis(&matRotAxis, &vUp, D3DXToRadian(m_fHorizonAngle));
	D3DXVec3TransformNormal(&vCamPos, &vCamPos, &matRotAxis);

	m_vEye = m_vAt + vCamPos;
	


}

void CStaticCamera::Mouse_Move(const _float & fTimeDelta)
{
	_long	dwMouseMove = 0;
	if (dwMouseMove = Engine::Get_DIMouseMove(Engine::DIMS_Y))
	{
		m_fVerticalAngle += D3DXToRadian(dwMouseMove*10.f)*fTimeDelta;

		if (D3DXToDegree(m_fVerticalAngle) <= 10.f)
			m_fVerticalAngle = D3DXToRadian(10.f);
		if (D3DXToDegree(m_fVerticalAngle) >= 80.f)
			m_fVerticalAngle = D3DXToRadian(80.f);
	}


	if (dwMouseMove = Engine::Get_DIMouseMove(Engine::DIMS_X))
	{
		m_fHorizonAngle += D3DXToRadian(dwMouseMove*300.f)*fTimeDelta;
	}


}

CStaticCamera* CStaticCamera::Create(LPDIRECT3DDEVICE9 pGraphicDev,
	Engine::CTransform* pTargetInfo,
	const _vec3* pEye,
	const _vec3* pAt,
	const _vec3* pUp,
	const _float& fFovY,
	const _float& fAspect,
	const _float& fNear,
	const _float& fFar)
{
	CStaticCamera*	pInstance = new CStaticCamera(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(pEye, pAt, pUp, fFovY, fAspect, fNear, fFar)))
		Engine::Safe_Release(pInstance);

	pInstance->SetUp_Target(pTargetInfo);

	return pInstance;
}

void CStaticCamera::Free(void)
{
	Engine::CCamera::Free();
}

