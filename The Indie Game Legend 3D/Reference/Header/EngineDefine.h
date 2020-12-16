#pragma once
#ifndef __ENGINE_DEFINE_H__

#pragma region EXPORT_DLL
#ifdef ENGINE_EXPORT
#define ENGINE_DLL _declspec(dllexport)
#else
#define ENGINE_DLL _declspec(dllimport)
#endif
#pragma endregion

#pragma region PRINT LOG
#define PrintLog(_sCaption, _sMessage) ::MessageBox(0, _sMessage, _sCaption, MB_OK);
#pragma endregion

#pragma region NAMESPACE

#define BEGIN(Name) namespace Name {
#define END }
#define USING(Name) using namespace Name;

#pragma endregion

#pragma region PREVENT_COPY_CONSTRUCTOR

#define NO_COPY(ClassName)							\
private:											\
	ClassName(const ClassName&) = delete;			\
	ClassName& operator=(const ClassName&) = delete;

#pragma endregion

#pragma region SINGLETON

#define DECLARE_SINGLETON(ClassName)			\
		NO_COPY(ClassName)						\
public:											\
	static ClassName* GetInstance();			\
	static UINT		  DeleteInstance();			\
private:										\
	static ClassName* m_pInstance;

#define IMPLEMENT_SINGLETON(ClassName)			\
ClassName*	ClassName::m_pInstance = nullptr;	\
ClassName*	ClassName::GetInstance()			\
{												\
	if (nullptr == m_pInstance)					\
		m_pInstance = new ClassName;			\
	return m_pInstance;							\
}												\
UINT ClassName::DeleteInstance()				\
{												\
	UINT nRefCount = 0;							\
	if(nullptr != m_pInstance)					\
	{											\
		nRefCount = m_pInstance->Release();		\
	}											\
	return nRefCount;							\
}

#pragma endregion


#pragma region GAMEOBJECT

#define OBJ_DEAD 1
#define OBJ_NOENVET 0

#pragma endregion
#define __ENGINE_DEFINE_H__
#endif // !__ENGINE_DEFINE_H__
