#ifndef Layer_h__
#define Layer_h__

#include "GameObject.h"


BEGIN(Engine)

class ENGINE_DLL CLayer : public CBase
{
private:
	explicit					CLayer(void);
	virtual						~CLayer(void);

public:
	CComponent*					Get_Component(const _tchar* pObjTag,  
												const _tchar* pComponentTag,
												COMPONENTID eID);
	CGameObject*				Get_GameObject(const _tchar * pObjTag);

public:
	HRESULT						Ready_Layer(void);
	HRESULT						LateReady_Layer(void);
	_int						Update_Layer(const _float& fTimeDelta);
	_int						LateUpdate_Layer(const _float& fTimeDelta);

	HRESULT						Add_GameObject(const _tchar* pObjTag, CGameObject* pGameObject);
	map<wstring, CGameObject*>& Get_ObjectMap();

private:
	map<wstring, CGameObject*>	m_pObjectMap;
	//wstring						m_wstrLayerName=nullptr;
public:
	static CLayer*				Create(void);
private:
	virtual void				Free(void);

};

END
#endif // Layer_h__
