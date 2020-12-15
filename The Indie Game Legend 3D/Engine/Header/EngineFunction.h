#pragma once
#ifndef __ENGINE_FUNCTION_H__

#pragma region DELETE_DYNAMIC_MEMORY
template <typename T>
void SafeDelete(T& ptr)
{
	if (nullptr != ptr)
	{
		delete ptr;
		ptr = nullptr;
	}
}

template <typename T>
void SafeDeleteArray(T& ptr)
{
	if (nullptr != ptr)
	{
		delete[] ptr;
		ptr = nullptr;
	}
}
#pragma endregion

#pragma region REFERENCE
template <typename T>
UINT SafeAddRef(T& ptr)
{
	UINT nRefCount = 0;

	if (nullptr != ptr)
	{
		nRefCount = ptr->AddRef();
	}

	return nRefCount;
}

template <typename T>
UINT SafeRelease(T& ptr)
{
	UINT nRefCount = 0;

	if (nullptr != ptr)
	{
		nRefCount = ptr->Release();

		ptr = nullptr;
	}

	return nRefCount;
}
#pragma endregion

#pragma region TYPENAME
static wstring StringAToStringW(const string& _aStr)
{
	wstring wStr;
	wStr.assign(_aStr.begin(), _aStr.end());
	return wStr;
}

template<typename T>
TSTRING GetTypeName()
{
#ifdef UNICODE
	wstring sTypeName = StringAToStringW(typeid(T).name());
#else
	string sTypeName = typeid(T).name();
#endif
	int nFind = (sTypeName.find(' ') + 1);
	sTypeName = sTypeName.substr(nFind, sTypeName.length());

	return sTypeName;
}

template<typename T>
UINT GetTypeHashCode()
{
	UINT nHashCode = typeid(T).hash_code();
	return nHashCode;
}

//TSTRING GetTypeName(LPGAMEOBJECT* const _pGameObject)
//{
//#ifdef UNICODE
//	wstring sTypeName = StringAToStringW(typeid(*_pGameObject).name());
//#else
//	string sTypeName = typeid(*_pGameObject).name();
//#endif
//	int nFind = (sTypeName.find(' ') + 1);
//	sTypeName = sTypeName.substr(nFind, sTypeName.length());
//
//	return sTypeName;
//}
#pragma endregion

#define __ENGINE_FUNCTION_H__
#endif // !__ENGINE_FUNCTION_H__
