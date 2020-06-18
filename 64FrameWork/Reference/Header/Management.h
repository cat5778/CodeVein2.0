#ifndef Management_h__
#define Management_h__

#include "Scene.h"

BEGIN(Engine)

class ENGINE_DLL CManagement : public CBase
{
	DECLARE_SINGLETON(CManagement)

private:
	explicit CManagement(void);
	virtual ~CManagement(void);

public:
	CComponent*			Get_Component(const _tchar* pLayerTag,
										const _tchar* pObjTag,
										const _tchar* pComponentTag,
										COMPONENTID eID);
	CGameObject*	Get_GameObject(const _tchar* pLayerTag,
									const _tchar* pObjTag);

	CLayer*			Get_Layer(const _tchar* pLayerTag);

public:
	HRESULT			Ready_Management(LPDIRECT3DDEVICE9& pGraphicDev);
	HRESULT			SetUp_Scene(CScene* pScene);
	_int			Update_Scene(const _float& fTimeDelta);
	_int			LateUpdate_Scene(const _float& fTimeDelta);
	void			Render_Scene(void);//옛날 확인하고 지울것
	void			Render_Scene(LPDIRECT3DDEVICE9& pGraphicDev);

private:
	CScene*			m_pScene = nullptr;

public: 
	virtual void	Free(void);
};

END
#endif // Management_h__
