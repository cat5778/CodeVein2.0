#ifndef Export_Utility_h__
#define Export_Utility_h__

#include "Management.h"
#include "Transform.h"
#include "Renderer.h"
#include "Component.h"
#include "Camera.h"
#include "Pipeline.h"
#include "Calculator.h"
#include "LightMgr.h"
#include "Collider.h"
#include "ColliderGroup.h"
#include "SphereColliderCom.h"
#include "PrototypeMgr.h"
#include "Shader.h"
#include "RenderTargetMgr.h"

BEGIN(Engine)

// Management Instance
// Get
inline 	CComponent*			Get_Component(const _tchar* pLayerTag, const _tchar* pObjTag, const _tchar* pComponentTag, COMPONENTID eID);
inline 	CGameObject*		Get_GameObject(const _tchar* pLayerTag, const _tchar* pObjTag);
inline 	CLayer*				Get_Layer(const _tchar* pLayerTag);
// Set
inline HRESULT			SetUp_Scene(CScene* pScene);
// General
//inline HRESULT		Create_Management(CManagement** ppManagement);
inline HRESULT		Create_Management(LPDIRECT3DDEVICE9& pGraphicDev, CManagement** ppManagement);


// Renderer
inline CRenderer*		Get_Renderer(void);

// LightMgr
// Get
// Set
// General
inline const D3DLIGHT9*		Get_LightInfo(const _uint& iIndex = 0);


inline HRESULT		Ready_Light(LPDIRECT3DDEVICE9 pGraphicDev,
								const D3DLIGHT9* pLightInfo,
								const _uint& iIndex);
inline 	void			Render_Light(LPD3DXEFFECT& pEffect);

// PrototypeMgr
// Get
// Set
// General
inline HRESULT			Ready_Prototype(const _tchar* pProtoTag, CComponent* pInstance);
inline CComponent*		Clone_Prototype(const _tchar* pProtoTag);

// RenderTargetMgr
// Get
// Set
inline void				SetUp_OnShader(LPD3DXEFFECT& pEffect, const _tchar* pTargetTag, const char* pContantName);

// General
inline HRESULT			Ready_RenderTarget(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pTargetTag, const _uint& iWidth, const _uint& iHeight, D3DFORMAT Format, D3DXCOLOR Color);
inline HRESULT			Ready_MRT(const _tchar* pMRTTag, const _tchar* pTargetTag);
inline HRESULT			Begin_MRT(const _tchar* pMRTTag);
inline HRESULT			End_MRT(const _tchar* pMRTTag);
inline HRESULT			Ready_DebugBuffer(const _tchar* pTargetTag,
										const _float& fX,
										const _float& fY,
										const _float& fSizeX,
										const _float& fSizeY);

inline void			Render_DebugBuffer(const _tchar* pMRTTag);

// Utility Release
inline void			Release_Utility(void);


#include "Export_Utility.inl"

END
#endif // Export_Utility_h__
