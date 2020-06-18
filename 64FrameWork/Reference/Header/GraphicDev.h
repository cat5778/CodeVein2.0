#ifndef GraphicDev_h__
#define GraphicDev_h__

#include "Base.h"
#include "Engine_Defines.h"

BEGIN(Engine)

class ENGINE_DLL CGraphicDev : public CBase
{
	DECLARE_SINGLETON(CGraphicDev)

private:
	explicit	CGraphicDev(void);
	virtual		~CGraphicDev(void);

public:
	LPDIRECT3DDEVICE9		Get_Device(void) { return m_pGraphicDev; }
	void					Get_Resolution(RESOLUTION& tResolution) const;

public:
	HRESULT		Ready_GraphicDev(HWND hWnd,
								WINMODE eMode,
								const _uint& iSizeX,
								const _uint& iSizeY,
								Engine::CGraphicDev** ppGraphicDev);

	void		Render_Begin(D3DXCOLOR Color);
	void		Render_End(void);

private:
	LPDIRECT3D9				m_pSDK = nullptr;
	LPDIRECT3DDEVICE9		m_pGraphicDev = nullptr;
	RESOLUTION				m_tResolution;

public:
		inline	virtual void Free(void);
};

END

#endif // GraphicDev_h__
