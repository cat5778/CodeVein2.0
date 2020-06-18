#include "Camera.h"
#include "Pipeline.h"

USING(Engine)

Engine::CCamera::CCamera(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{

}

Engine::CCamera::~CCamera(void)
{

}

HRESULT Engine::CCamera::Ready_GameObject(void)
{
	D3DXMatrixLookAtLH(&m_matView, &m_vEye, &m_vAt, &m_vUp);
	//CPipeline::MakeViewSpaceMatrix(&m_matView, &m_vEye, &m_vAt, &m_vUp);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);

	D3DXMatrixPerspectiveFovLH(&m_matProj, m_fFovY, m_fAspect, m_fNear, m_fFar);
	//CPipeline::MakeProjectionMatrix(&m_matProj, m_fFovY, m_fAspect, m_fNear, m_fFar);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);


	D3DXMatrixOrthoLH(&m_matOrtho, WINCX, WINCY, 0.f, 1.f);


	return S_OK;
}

_int Engine::CCamera::Update_GameObject(const _float& fTimeDelta)
{
	D3DXMatrixLookAtLH(&m_matView, &m_vEye, &m_vAt, &m_vUp);
	//CPipeline::MakeViewSpaceMatrix(&m_matView, &m_vEye, &m_vAt, &m_vUp);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);
	CGameObject::Update_GameObject(fTimeDelta);
	return 0;
}

void Engine::CCamera::Free(void)
{
	CGameObject::Free();
}


