#include "Export_Utility.h"
// Management Instance
// Get
CComponent*			Get_Component(const _tchar* pLayerTag, const _tchar* pObjTag, const _tchar* pComponentTag, COMPONENTID eID)
{
	return CManagement::GetInstance()->Get_Component(pLayerTag, pObjTag, pComponentTag, eID);
}
CGameObject * Get_GameObject(const _tchar * pLayerTag, const _tchar * pObjTag)
{
	return CManagement::GetInstance()->Get_GameObject(pLayerTag, pObjTag);
}
CLayer * Get_Layer(const _tchar * pLayerTag)
{
	return CManagement::GetInstance()->Get_Layer(pLayerTag);
}
// Set
HRESULT			SetUp_Scene(CScene* pScene)
{
	return CManagement::GetInstance()->SetUp_Scene(pScene);
}

// General
HRESULT		Create_Management(LPDIRECT3DDEVICE9& pGraphicDev, CManagement** ppManagement)
{
	CManagement*		pInstance = CManagement::GetInstance();

	if (nullptr == pInstance)
		return E_FAIL;

	FAILED_CHECK_RETURN(pInstance->Ready_Management(pGraphicDev), E_FAIL);

	*ppManagement = pInstance;

	return S_OK;
}

// Renderer
CRenderer*		Get_Renderer(void)
{
	return CRenderer::GetInstance();
}
// LightMgr
// Get
// Set
// General
const D3DLIGHT9*		Get_LightInfo(const _uint& iIndex)
{
	return CLightMgr::GetInstance()->Get_LightInfo(iIndex);
}

HRESULT		Ready_Light(LPDIRECT3DDEVICE9 pGraphicDev,
	const D3DLIGHT9* pLightInfo,
	const _uint& iIndex)
{
	return CLightMgr::GetInstance()->Ready_Light(pGraphicDev, pLightInfo, iIndex);
}

void			Render_Light(LPD3DXEFFECT& pEffect)
{
	CLightMgr::GetInstance()->Render_Light(pEffect);
}

// PrototypeMgr
// Get
// Set
// General
HRESULT			Ready_Prototype(const _tchar* pProtoTag, CComponent* pInstance)
{
	return CPrototypeMgr::GetInstance()->Ready_Prototype(pProtoTag, pInstance);
}
CComponent*		Clone_Prototype(const _tchar* pProtoTag)
{
	return CPrototypeMgr::GetInstance()->Clone_Prototype(pProtoTag);
}
void SetUp_OnShader(LPD3DXEFFECT & pEffect, const _tchar * pTargetTag, const char * pContantName)
{
	CRenderTargetMgr::GetInstance()->SetUp_OnShader(pEffect, pTargetTag, pContantName);

}
// RenderTargetMgr
// Get
// Set
// General
HRESULT			Ready_RenderTarget(LPDIRECT3DDEVICE9 pGraphicDev,
	const _tchar* pTargetTag,
	const _uint& iWidth,
	const _uint& iHeight,
	D3DFORMAT Format,
	D3DXCOLOR Color)
{
	return CRenderTargetMgr::GetInstance()->Ready_RenderTarget(pGraphicDev, pTargetTag, iWidth, iHeight, Format, Color);
}
HRESULT			Ready_MRT(const _tchar* pMRTTag, const _tchar* pTargetTag)
{
	return CRenderTargetMgr::GetInstance()->Ready_MRT(pMRTTag, pTargetTag);
}
HRESULT			Begin_MRT(const _tchar* pMRTTag)
{
	return CRenderTargetMgr::GetInstance()->Begin_MRT(pMRTTag);
}
HRESULT			End_MRT(const _tchar* pMRTTag)
{
	return CRenderTargetMgr::GetInstance()->End_MRT(pMRTTag);
}

HRESULT			Ready_DebugBuffer(const _tchar* pTargetTag,
	const _float& fX,
	const _float& fY,
	const _float& fSizeX,
	const _float& fSizeY)
{
	return CRenderTargetMgr::GetInstance()->Ready_DebugBuffer(pTargetTag, fX, fY, fSizeX, fSizeY);
}

void			Render_DebugBuffer(const _tchar* pMRTTag)
{
	CRenderTargetMgr::GetInstance()->Render_DebugBuffer(pMRTTag);
}


// Utility Release
void			Release_Utility(void)
{
	CRenderTargetMgr::GetInstance()->DestroyInstance();
	CPrototypeMgr::GetInstance()->DestroyInstance();
	CLightMgr::GetInstance()->DestroyInstance();
	CRenderer::GetInstance()->DestroyInstance();
	CManagement::GetInstance()->DestroyInstance();
}
