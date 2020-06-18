#pragma once

#include "Defines.h"

class CTimerManager
{
public:
	DECLARE_SINGLETON(CTimerManager)
private:
	explicit CTimerManager(void);
	virtual ~CTimerManager();
public:
	void					Update(_float fTimeDelta);
public:
	void					Add_Timer(wstring wstrTag, _float fEndTime);
	_float					Get_CurTime(wstring wstrTag);

	void					Start_Timer(wstring wstrTag);
	_bool					IsEnd(wstring wstrTag);
	void					Init_TimerMap();


private:
	map<wstring, TIME_DATA*> m_pTimerMap;


};

