#ifndef AniCtrl_h__
#define AniCtrl_h__

#include "Engine_Defines.h"
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CAniCtrl : public CBase
{
private:
	explicit CAniCtrl(LPD3DXANIMATIONCONTROLLER pAniCtrl);
	explicit CAniCtrl(const CAniCtrl& rhs);
	virtual ~CAniCtrl(void);

public:
	LPD3DXANIMATIONCONTROLLER		Get_AnimationCtrl(void) { return m_pAniCtrl; }
	_bool							Is_AnimationSetEnd(void);

	_double							Get_Period() { return m_dPeriod; }
	_double							Get_TrackPosition() { return m_dPosition; }
	void							Set_AddTrackTime(float fTime) { m_fAddTime = fTime; }
public:
	HRESULT			Ready_AnimationCtrl(void);

	void			Set_AnimationSet(const _uint& iIndex);

	void			Play_Animation(const _float& fTimeDelta);

private:
	LPD3DXANIMATIONCONTROLLER			m_pAniCtrl;

	_uint								m_iCurrentTrack;
	_uint								m_iNewTrack;

	_float								m_fAccTime;
	_uint								m_iOldAniIdx;

	_double								m_dPeriod = 0.0;
	_double								m_dPosition= 0.0;
	_float								m_fAddTime = 0.f;
	D3DXTRACK_DESC						m_tTrackInfo;

public:
	static CAniCtrl*			Create(LPD3DXANIMATIONCONTROLLER pAniCtrl);
	static CAniCtrl*			Create(const CAniCtrl& rhs);
	virtual void				Free(void);
};

END

#endif // AniCtrl_h__
