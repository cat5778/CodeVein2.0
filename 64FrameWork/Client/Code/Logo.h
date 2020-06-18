#ifndef Logo_h__
#define Logo_h__

#include "Defines.h"
#include "Scene.h"

#include "BackGround.h"
#include "TestPlayer.h"
#include "TestMonster.h"

#include "Loading.h"
class CMainApp;
class CLogo : public Engine::CScene
{
private:
	explicit CLogo(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual  ~CLogo(void);


public:
	virtual HRESULT				Ready_Scene(void) override;
	virtual _int				Update_Scene(const _float& fTimeDelta) override;
	virtual void				Render_Scene(void) override;

	void						Set_PathList(list<MESH_PATH*>* ppPathlist) { m_ppPathlist =ppPathlist; }
private:
	HRESULT						Ready_Environment_Layer(const _tchar* pLayerTag) { return S_OK; }
	HRESULT						Ready_GameLogic_Layer(const _tchar* pLayerTag);
	HRESULT						Ready_UI_Layer(const _tchar* pLayerTag) { return S_OK; }
	HRESULT						Ready_Resource(LPDIRECT3DDEVICE9& pGraphicDev, RESOURCEID eMax);

private:
	_matrix						m_matView, m_matProj;
	CLoading*					m_pLoading;
	list<MESH_PATH*>*			m_ppPathlist = nullptr;

public:
	static CLogo*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free(void) override;
};


#endif // Logo_h__
