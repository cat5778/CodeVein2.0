#include "stdafx.h"
#include "TimerManager.h"

IMPLEMENT_SINGLETON(CTimerManager)

CTimerManager::CTimerManager()
{
}


CTimerManager::~CTimerManager()
{
}

void CTimerManager::Update(_float fTimeDelta)
{
	for (auto &iter : m_pTimerMap)
	{
		if (iter.second->bIsStart)
		{
			if (iter.second->fCurTime >= iter.second->fEndTime)
			{
				iter.second->fCurTime = 0.f;
				iter.second->bIsEnd = true;
				iter.second->bIsStart = false;
			}
			else
				iter.second->fCurTime += fTimeDelta;
		}
	}
}

void CTimerManager::Add_Timer(wstring wstrTag, _float fEndTime)
{
	Time_Data* tTimeDat= new TIME_DATA(fEndTime);
	m_pTimerMap.insert(make_pair(wstrTag, tTimeDat));
}

_float CTimerManager::Get_CurTime(wstring wstrTag)
{
	auto iter = m_pTimerMap.find(wstrTag);
	if (iter == m_pTimerMap.end())
		return -1.f;

	return iter->second->fCurTime;
}

void CTimerManager::Start_Timer(wstring wstrTag)
{
	auto iter = m_pTimerMap.find(wstrTag);
	if (iter == m_pTimerMap.end())
		return;
	
	iter->second->bIsStart = true;
	iter->second->bIsStart = 0.f;
}

_bool CTimerManager::IsEnd(wstring wstrTag)
{
	auto iter = m_pTimerMap.find(wstrTag);
	if (iter == m_pTimerMap.end())
		return -1.f;

	return iter->second->bIsEnd;
}

void CTimerManager::Init_TimerMap()
{
	for (auto &iter : m_pTimerMap)
	{
		if(iter.second!=nullptr)
			delete iter.second;
	
		iter.second = nullptr;
	}
	m_pTimerMap.clear();
}
