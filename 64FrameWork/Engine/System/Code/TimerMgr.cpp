#include "TimerMgr.h"

USING(Engine)
IMPLEMENT_SINGLETON(CTimerMgr)

Engine::CTimerMgr::CTimerMgr(void)
{

}

Engine::CTimerMgr::~CTimerMgr(void)
{
	Free();
}

HRESULT Engine::CTimerMgr::Ready_Timer(const _tchar* pTimerTag)
{
	CTimer*		pInstance = Find_Timer(pTimerTag);

	if (nullptr != pInstance)
		return E_FAIL;

	pInstance = CTimer::Create();
	NULL_CHECK_RETURN(pInstance, E_FAIL);

	m_mapTimer.emplace(pTimerTag, pInstance);

	return S_OK;
}

void Engine::CTimerMgr::Free(void)
{
	for_each(m_mapTimer.begin(), m_mapTimer.end(), CDeleteMap());
	m_mapTimer.clear();
}

CTimer* Engine::CTimerMgr::Find_Timer(const _tchar* pTimerTag)
{
	auto	iter = find_if(m_mapTimer.begin(), m_mapTimer.end(), CTag_Finder(pTimerTag));

	if (iter == m_mapTimer.end())
		return nullptr;

	return iter->second;
}

void CTimerMgr::Set_TimeDelta(const _tchar * pTimerTag)
{
	CTimer*		pInstance = Find_Timer(pTimerTag);

	if (nullptr == pInstance)
		return;

	pInstance->Set_TimeDelta();
}

_float CTimerMgr::Get_TimeDelta(const _tchar * pTimerTag)
{
	CTimer*		pInstance = Find_Timer(pTimerTag);

	if (nullptr == pInstance)
		return 0.f;

	return pInstance->Get_TimeDelta();
}
