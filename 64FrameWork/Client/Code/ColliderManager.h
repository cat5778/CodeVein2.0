#pragma once


BEGIN(Engine)
class CNaviMesh;
class CDynamicMesh;
class CTransform;
class CRenderer;
class CCalculator;
class CCollider;
class CShader;
class CColliderGroup;
END

class CColliderManager
{
	DECLARE_SINGLETON(CColliderManager)
public:
	CColliderManager();
	~CColliderManager();

public:
	void	Add_Colldata(SPHERE_COL_DATA* pSpColData) { m_pCollDataVec.push_back(pSpColData); }
	void	Get_ObjCollider(Engine::CColliderGroup* pCollGroupCom, wstring wstrObjTag);
private:
	void Release();


	vector<SPHERE_COL_DATA*> m_pCollDataVec;


	//map<wstring, 
	//fout << CW2A(wstrObjTag.c_str()) << endl;
	//fout << i << endl;
	//fout << CW2A(wstrBoneTag.c_str()) << endl;
	//fout << CW2A(wstrCollTag.c_str()) << endl;
	//fout << vPos.x << endl;
	//fout << vPos.y << endl;
	//fout << vPos.z << endl;
	//fout << fRadius << endl;


};

