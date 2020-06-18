#ifndef Renderer_h__
#define Renderer_h__

#include "Engine_Defines.h"
#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL CRenderer : public CComponent
{
	DECLARE_SINGLETON(CRenderer)

private:
	explicit CRenderer(void);
	virtual ~CRenderer(void);
public:
	void		Set_Perspective(const _matrix& matPerspective);

public:
	HRESULT		Ready_Renderer(LPDIRECT3DDEVICE9& pGraphicDev);

	void		Add_RenderGroup(RENDERID eGroup, CGameObject* pGameObject);
	void		Render_GameObject(void);
	void		Render_GameObject(LPDIRECT3DDEVICE9& pGraphicDev);
	void		Clear_RenderGroup(void);

private:
	void		RenderState_Projection(PROJECTIONTYPE eProjType);

private:
	void		Render_Priority(LPDIRECT3DDEVICE9& pGraphicDev);
	void		Render_NonAlpha(LPDIRECT3DDEVICE9& pGraphicDev);
	void		Render_Alpha(LPDIRECT3DDEVICE9& pGraphicDev);
	void		Render_UI(LPDIRECT3DDEVICE9& pGraphicDev);

	void		Render_Defferd(LPDIRECT3DDEVICE9& pGraphicDev);
	void		Render_LightAcc(LPDIRECT3DDEVICE9& pGraphicDev);
	void		Render_Blend(LPDIRECT3DDEVICE9& pGraphicDev);


private:
	list<CGameObject*>		m_RenderGroup[RENDER_END];
	_matrix					m_matOrthograph, m_matPerspective;

	LPDIRECT3DVERTEXBUFFER9		m_pVB;
	LPDIRECT3DINDEXBUFFER9		m_pIB;


private:
	virtual void	Free(void);

};

END
#endif // Renderer_h__
