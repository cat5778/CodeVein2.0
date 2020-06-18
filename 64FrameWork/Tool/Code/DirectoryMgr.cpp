#include "stdafx.h"
#include "DirectoryMgr.h"


CDirectoryMgr::CDirectoryMgr()
{
}


CDirectoryMgr::~CDirectoryMgr()
{
}

CString CDirectoryMgr::ConvertRelativePath(const TCHAR * pFullPath)
{
	// From에서 To로 가는 상대경로를 Output에 저장하는 함수.
	//::PathRelativePathTo(Output, From, FILE_ATTRIBUTE_DIRECTORY, To, FILE_ATTRIBUTE_DIRECTORY);
	
	TCHAR szCurrent[MAX_STR] = L"";
	TCHAR szRelative[MAX_STR] = L"";
	
	::GetCurrentDirectory(MAX_STR, szCurrent);
	::PathRelativePathTo(szRelative, szCurrent, FILE_ATTRIBUTE_DIRECTORY, pFullPath, FILE_ATTRIBUTE_DIRECTORY);



	return CString(szRelative);
}

void CDirectoryMgr::ExtractPathInfo(const TCHAR * pFullPath, list<MESH_PATH*>& rPathInfoList)
{
	CString strFindPath = CString(pFullPath) + L"\\*.*";

	// 파일 탐색 관련 MFC 클래스.
	CFileFind	find;

	// 대상을 검색해서 존재한다면 TRUE를 반환한다.
	// 만약 "Test\\*.*" 형태로 인자를 전달하면 Test폴더 안에 뭐든 하나라도 존재하면 TRUE 반환.
	BOOL bIsFind = find.FindFile(strFindPath);

	while (bIsFind)
	{
		// FindFile 함수 호출 후 대상에게 접근하고자 한다면 반드시 FindNextFile함수를 호출해주어야 한다.
		// 만약 FindFile 함수에 "Test\\*.*" 형태로 인자를 전달한 경우에 FindNextFile함수를 호출하면
		// 가장 첫번째 대상에게 접근할 수 있다.
		bIsFind = find.FindNextFile(); // 대상이 없거나 마지막 대상에게 도달한 경우 FALSE를 반환.

		if (find.IsDots()) // 접근한 대상이 .이나 ..과 같은 마커라면 건너뜀.
			continue;
		else if (find.IsDirectory()) // 폴더냐
			ExtractPathInfo(find.GetFilePath(), rPathInfoList);
		else // 파일이냐
		{
			if (find.IsSystem()) // 시스템 파일이면 건너뜀.
				continue;

			MESH_PATH* pPathInfo = new MESH_PATH;

			// 파일 개수 추출.
			TCHAR szFilePath[MAX_STR] = L"";
			lstrcpy(szFilePath, find.GetFilePath());
			//::PathRemoveFileSpec(szFilePath);

			//pPathInfo->iImgCount = GetFileCount(szFilePath);

			// 상대 경로 추출
			CString strRelative = ConvertRelativePath(find.GetFilePath());
			TCHAR tcConvert[MAX_STR] = L"";
			lstrcpy(tcConvert, strRelative);
			::PathRemoveFileSpec(tcConvert);
			strRelative = tcConvert;
			pPathInfo->wstrRelative = strRelative;
			pPathInfo->wstrRelative += L"\\";

			strRelative.Replace(L"0.", L"%d."); // (old, new)
			//::PathRemoveFileSpec(szFilePath);

			lstrcpy(szFilePath, strRelative);
			if (pPathInfo->wstrRelative.find(L"DynamicMesh") != wstring::npos) //DynamicMesh
			{
				// MeshName
				pPathInfo->wstrName = ::PathFindFileName(szFilePath);
				pPathInfo->wstrName += L".X";
				pPathInfo->wstrObjectType = ::PathFindFileName(szFilePath);
				//::PathRemoveFileSpec(szFilePath);

				// MeshName
				//::PathRemoveFileSpec(szFilePath);
				//pPathInfo->wstrObjectType = ::PathFindFileName(szFilePath);

				// MeshType
				::PathRemoveFileSpec(szFilePath);
				pPathInfo->wstrMeshType = ::PathFindFileName(szFilePath);

			}
			else //StaticMesh
			{
				// MeshName
				//::PathRemoveFileSpec(szFilePath);
				pPathInfo->wstrName = ::PathFindFileName(szFilePath);
				pPathInfo->wstrName += L".X";
				pPathInfo->wstrObjectType = ::PathFindFileName(szFilePath);

				// MeshType
				pPathInfo->wstrMeshType = ::PathRemoveFileSpec(szFilePath);

				// MeshGroup 
				pPathInfo->wstrGroup = ::PathFindFileName(szFilePath);

				// MeshMap 
				::PathRemoveFileSpec(szFilePath);
				pPathInfo->wstrMap = ::PathFindFileName(szFilePath);

				// MeshType 
				::PathRemoveFileSpec(szFilePath);
				pPathInfo->wstrMeshType = ::PathFindFileName(szFilePath);
			}


			rPathInfoList.push_back(pPathInfo);
			break;
		}
	}

}

int CDirectoryMgr::GetFileCount(const TCHAR * pFullPath)
{
	CString strFindPath = CString(pFullPath) + L"\\*.*";

	CFileFind	find;
	int iCount = 0;

	BOOL bIsFind = find.FindFile(strFindPath);

	while (bIsFind)
	{
		bIsFind = find.FindNextFile();

		if (find.IsDots() || find.IsDirectory() || find.IsSystem())
			continue;

		++iCount;
	}

	return iCount;

}
