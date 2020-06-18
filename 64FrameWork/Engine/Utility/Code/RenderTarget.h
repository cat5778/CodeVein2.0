#ifndef RenderTarget_h__
#define RenderTarget_h__

#include "Engine_Defines.h"
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CRenderTarget : public CBase
{
private:
	explicit					CRenderTarget(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual						~CRenderTarget(void);

public:
	HRESULT						Ready_RenderTarget(const _uint& iWidth, 
													const _uint& iHeight, 
													D3DFORMAT Format, 
													D3DXCOLOR Color);

	void						SetUp_RenderTarget(const _uint& iIndex);

	void						Clear_RenderTarget(void);
	void						Release_OnGraphicDev(const _uint& iIndex);

	HRESULT						Ready_DebugBuffer(const _float& fX,
													const _float& fY,
													const _float& fSizeX,
													const _float& fSizeY);

	void						Render_DebugBuffer(void);
	void						SetUp_OnShader(LPD3DXEFFECT& pEffect, const char* pContantName);


private:
	LPDIRECT3DDEVICE9			m_pGraphicDev;
	LPDIRECT3DTEXTURE9			m_pTargetTexture;

	LPDIRECT3DSURFACE9			m_pTargetSurface;
	LPDIRECT3DSURFACE9			m_pOldTargetSurface;

	LPDIRECT3DVERTEXBUFFER9		m_pVB;
	LPDIRECT3DINDEXBUFFER9		m_pIB;

private:
	D3DXCOLOR					m_ClearColor;

public:
	static CRenderTarget*		Create(LPDIRECT3DDEVICE9 pGraphicDev,
										const _uint& iWidth,
										const _uint& iHeight,
										D3DFORMAT Format, 
										D3DXCOLOR Color);
	
	virtual	void				Free(void);
};
END
#endif // RenderTarget_h__
