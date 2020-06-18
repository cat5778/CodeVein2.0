#include "stdafx.h"
#include "PathManager.h"

IMPLEMENT_SINGLETON(CPathManager)

CPathManager::CPathManager()
{
}


CPathManager::~CPathManager()
{
	Release();
}

void CPathManager::Release()
{
}
