#ifndef Field_H__
#define Field_H__

#include "Defines.h"
#include "Scene.h"

#include "SkyBox.h"
#include "Terrain.h"
#include "Player.h"
#include "Monster.h"
#include "StaticCamera.h"
//#include "DynamicCamera.h"
#include "ThirdPersonCamera.h"
//#include "Effect.h"

#include "Stone.h"
#include "Sword.h"

class CField : public Engine::CScene
{
private:
	explicit CField(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CField(void);


public:
	virtual HRESULT Ready_Scene(void) override;
	virtual HRESULT	LateReady_Scene(void) override;

	virtual _int Update_Scene(const _float& fTimeDelta) override;
	virtual _int LateUpdate_Scene(const _float& fTimeDelta) override;
	virtual void Render_Scene(void) override;

private:
	//HRESULT		Ready_Environment_Layer(const _tchar* pLayerTag) { return S_OK; }
	HRESULT		Ready_GameLogic_Layer(const _tchar* pLayerTag);
	HRESULT		Ready_UI_Layer(const _tchar* pLayerTag);
	HRESULT		Ready_LightInfo(void);
	//HRESULT		Ready_Prototype(void);

	HRESULT		Load_Text(const _tchar * pFilePath);
	wchar_t*	CharToWChar(const char* pstrSrc);


private:
	_matrix								m_matView, m_matProj;
	_ulong								m_dwRenderCnt = 0;
	_tchar								m_szFPS[256];
	_float								m_fTime = 0.f;

	map<wstring, Engine::CGameObject*>*	m_ppGameObjectMap = nullptr;

public:
	static CField*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free(void) override;
};


#endif // Field_H__
