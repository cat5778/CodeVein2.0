#include "Transform.h"

USING(Engine)

Engine::CTransform::CTransform(void)
	: m_vScale(1.f, 1.f, 1.f)
	, m_vAngle(0.f, 0.f, 0.f)
{
	ZeroMemory(m_vInfo, sizeof(_vec3) * INFO_END);
}

Engine::CTransform::~CTransform(void)
{

}

HRESULT Engine::CTransform::Ready_Component(void)
{
	D3DXMatrixIdentity(&m_matWorld);

	for (_uint i = 0; i < INFO_END; ++i)
		memcpy(&m_vInfo[i], &m_matWorld.m[i][0], sizeof(_vec3));

	return S_OK;
}

_int Engine::CTransform::Update_Component(const _float& fTimeDelta)
{
	D3DXMatrixIdentity(&m_matWorld);

	for (_uint i = 0; i < INFO_POS; ++i)
		memcpy(&m_vInfo[i], &m_matWorld.m[i][0], sizeof(_vec3));


	// 크기 변환 적용
	for(_uint i = 0; i < INFO_POS; ++i)
	{
		D3DXVec3Normalize(&m_vInfo[i], &m_vInfo[i]);
		m_vInfo[i] *= *(((_float*)&m_vScale) + i);	
	}

	// 회전 적용
	_matrix			matRot[ROT_END];

	D3DXMatrixRotationX(&matRot[ROT_X], m_vAngle.x);
	D3DXMatrixRotationY(&matRot[ROT_Y], m_vAngle.y);
	D3DXMatrixRotationZ(&matRot[ROT_Z], m_vAngle.z);

	for (_uint i = 0; i < INFO_POS; ++i)
	{
		for (_uint j = 0; j < ROT_END; ++j)
		{
			D3DXVec3TransformNormal(&m_vInfo[i], &m_vInfo[i], &matRot[j]);
		}
	}

	for (_uint i = 0; i < INFO_END; ++i)
		memcpy(&m_matWorld.m[i][0], &m_vInfo[i], sizeof(_vec3));


	return 0;
}

Engine::CTransform* Engine::CTransform::Create(void)
{
	CTransform*		pInstance = new CTransform;

	if (FAILED(pInstance->Ready_Component()))
		Safe_Release(pInstance);

	return pInstance;
}

void Engine::CTransform::Free(void)
{

}

void CTransform::Get_Info(INFO eType, _vec3 * pInfo)
{
	memcpy(pInfo, &m_matWorld.m[eType][0], sizeof(_vec3));
}

const _vec3 * CTransform::Get_Info(INFO eType)
{
	return &m_vInfo[eType];
}

void CTransform::Move_Pos(const _vec3 * const pDir)
{
	m_vInfo[INFO_POS] += *pDir;
}

void Engine::CTransform::Move_TargetPos(const _vec3* pPos, const _float& fSpeed, const _float& fTimeDelta)
{
	_vec3	vDir = *pPos - m_vInfo[INFO_POS];

	m_vInfo[INFO_POS] += *D3DXVec3Normalize(&vDir, &vDir) * fSpeed * fTimeDelta;
}

void CTransform::Rotation(ROTATION eType, const _float & fAngle)
{
	*(((_float*)&m_vAngle) + eType) += fAngle;
}

const _matrix* CTransform::Compute_LookAtTarget(const _vec3 * pTargetPos)
{
	_vec3		vDir = *pTargetPos - m_vInfo[INFO_POS];

	_vec3		vAxis, vUp;
	_matrix		matRot;
	
	return D3DXMatrixRotationAxis(&matRot,
									D3DXVec3Cross(&vAxis, &m_vInfo[INFO_UP], &vDir),
									acosf(D3DXVec3Dot(D3DXVec3Normalize(&vDir, &vDir),
													  D3DXVec3Normalize(&vUp, &m_vInfo[INFO_UP]))));
}

void CTransform::Chase_Target(const _vec3 * pTargetPos, const _float & fSpeed)
{
	_vec3		vDir = *pTargetPos - m_vInfo[INFO_POS];

	m_vInfo[INFO_POS] += *D3DXVec3Normalize(&vDir, &vDir) * fSpeed;

	_matrix		matRot = *Compute_LookAtTarget(pTargetPos);
	_matrix		matScale, matTrans;

	D3DXMatrixScaling(&matScale, m_vScale.x, m_vScale.y, m_vScale.z);
	D3DXMatrixTranslation(&matTrans, m_vInfo[INFO_POS].x, m_vInfo[INFO_POS].y, m_vInfo[INFO_POS].z);

	m_matWorld = matScale * matRot * matTrans;

}

void Engine::CTransform::Set_ParentMatrix(const _matrix* pMatrix)
{
	m_matWorld *= *pMatrix;
}

void CTransform::Set_WorldMatrix(const _matrix * pMatrix)
{
	m_matWorld = *pMatrix;
}

void Engine::CTransform::Get_WorldMatrix(_matrix* pWorld) const
{
	*pWorld = m_matWorld;
}

void Engine::CTransform::Set_Pos(const _vec3* pPos)
{
	memcpy(&m_matWorld.m[3][0], pPos, sizeof(_vec3));
}

void Engine::CTransform::Set_Pos(const _float& fX, const _float& fY, const _float& fZ)
{
	m_vInfo[INFO_POS].x = fX;
	m_vInfo[INFO_POS].y = fY;
	m_vInfo[INFO_POS].z = fZ;
}

void Engine::CTransform::Set_Scale(const _float& fX, const _float& fY, const _float& fZ)
{
	m_vScale.x = fX;
	m_vScale.y = fY;
	m_vScale.z = fZ;
}
