#ifndef Texture_h__
#define Texture_h__

#include "Resources.h"

BEGIN(Engine)

class ENGINE_DLL CTexture : public CResources
{
protected:
	explicit		CTexture(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit		CTexture(const CTexture& rhs);
	virtual			~CTexture(void);

public:
	const D3DXIMAGE_INFO&	Get_ImageInfo(const _uint& iIndex = 0) const;
	const _uint				Get_TextureFrameSize(void) const;
	IDirect3DBaseTexture9*	Get_Texture(const _uint& iIndex = 0);
	const int&				GetVecSize() const;
	void					Set_Texture(LPD3DXEFFECT& pEffect, const char* pShaderName, const _uint& iIndex = 0);

public:
	HRESULT			Ready_Texture(const _tchar* pPath, TEXTURETYPE eType, const _uint& iCnt = 1);
	void			Render_Texture(const _uint& iIndex = 0);

private:
	vector<IDirect3DBaseTexture9*>		m_vecTexture;
	vector<D3DXIMAGE_INFO>				m_vecImageInfo;

public:
	static CTexture*		Create(LPDIRECT3DDEVICE9 pGraphicDev, 
									const _tchar* pPath, 
									TEXTURETYPE eType,
									const _uint& iCnt = 1);

	virtual CResources*		Clone(void);
	
private:
	virtual void	Free(void);

};

END
#endif // Texture_h__
