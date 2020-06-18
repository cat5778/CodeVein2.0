#ifndef Timer_h__
#define Timer_h__

#include "Engine_Defines.h"
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CTimer : public CBase
{
private:
	explicit CTimer(void);
	virtual ~CTimer(void);

public:
	HRESULT		Ready_Timer(void);
	void		Set_TimeDelta(void);
	_float		Get_TimeDelta(void) { return m_fTimeDelta; }

private:
	LARGE_INTEGER			m_FrameTime;
	LARGE_INTEGER			m_LastTime;
	LARGE_INTEGER			m_FixTime;
	LARGE_INTEGER			m_CpuTick;

	_float					m_fTimeDelta = 0.f;

public:
	static CTimer*	Create(void);
	virtual void	Free(void);

};
END
#endif // Timer_h__
