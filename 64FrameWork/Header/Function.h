#ifndef Function_h__
#define Function_h__

template <typename T> 
DWORD Safe_Release(T& pointer)
{
	DWORD	dwRefCnt = 0;
	if (NULL != pointer)
	{
		dwRefCnt = pointer->Release();

		if (dwRefCnt == 0)
			pointer = NULL;
	}
	return dwRefCnt;
}

template <typename T> 
DWORD Safe_Release_Delete(T& pointer)
{
	DWORD	dwRefCnt = 0;
	if (NULL != pointer)
	{
		dwRefCnt = pointer->Release();
		if (dwRefCnt == 0)
		{
			delete pointer;
			pointer = NULL;
		}
	}
	return dwRefCnt;
}

template <typename T> 
void Safe_Single_Destory(T& pointer)
{
	if (NULL != pointer)
	{
		pointer->DestroyInstance();
		pointer = NULL;
	}
}

template <typename T> 
void Safe_Delete(T& pointer)
{
	if (NULL != pointer)
	{
		delete pointer;
		pointer = NULL;
	}
}

template <typename T>
void Safe_Delete_Array(T& pointer)
{
	if (NULL != pointer)
	{
		delete[] pointer;
		pointer = NULL;
	}
}
template <typename T>
void Null_Check_Assert(T& pointer)
{
	//HRESULT hrd= pointer;
	assert(E_FAIL != pointer);
}

template <typename T>
T Lerp(T Start, T End, T Time)
{
	return (1 - Time)*Start + Time*End;
}


#endif // Function_h__