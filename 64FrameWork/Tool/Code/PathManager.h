#pragma once


class CPathManager
{
	DECLARE_SINGLETON(CPathManager)
public:
	CPathManager();
	~CPathManager();
private:
	void Release();

public: // User Variables
	list<MESH_PATH*>	m_pMeshList;
	list<Effect_Path*>	m_pEffectList;

};

