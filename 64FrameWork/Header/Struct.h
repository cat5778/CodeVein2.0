#ifndef Struch_h__
#define Struch_h__


typedef struct Mesh_Path
{
	wstring wstrRelative;	//
	wstring wstrMeshType;   //Static or Dynamic
	wstring wstrGroup;		//Mesh Group
	wstring wstrMap;		//Mesh Map Folder
	wstring wstrObjectType;	//Mesh Type Ex: Pillar, Grass, Tree
	wstring wstrName;		//Mesh File Name
}MESH_PATH;

typedef struct Effect_Path
{
	wstring wstrType;
	wstring wstrName;
}EFFECT_PATH;

typedef struct Transform_Info
{
	_vec3	vScale;
	_vec3	vRotation;
	_vec3	vPosition;
}TRANSFORM_INFO;
typedef struct NavMesh
{
	_vec3	vPoint1;
	_vec3	vPoint2;
	_vec3	vPoint3;
	_uint	uiNavMeshIdx;
	_float	fRadius;

}NAV_MESH;

typedef struct RAY
{
	_vec3	vDir;
	_vec3	vPos;
}RAY;

typedef struct Collider_data
{
	wstring			wstrObject;
	wstring			wstrBone;
	_float			fRadius;
	COLLIDEROPTION	eCollOpt;
	Collider_data() :wstrObject(L""), wstrBone(L""),fRadius(0.f) 
	{
		eCollOpt = COLL_OPT_HURT;
	};
}COLL_DATA;

typedef struct Time_Data
{
	_bool	bIsStart;
	_bool	bIsEnd;
	_float  fCurTime;
	_float  fEndTime;
	Time_Data() :bIsStart(false), bIsEnd(false), fCurTime(0.f), fEndTime(0.f) {};
	Time_Data(_float fTime) :bIsStart(false), bIsEnd(false), fCurTime(0.f), fEndTime(fTime) {};

}TIME_DATA;

typedef struct Sphere_Coll
{
	wstring wstrObjTag;
	wstring wstrBoneTag;
	wstring wstrCollTag;
	_vec3  vPos;
	_float fRadius;
	_uint  uiOption;

}SPHERE_COL_DATA;



#endif // Struch_h__
