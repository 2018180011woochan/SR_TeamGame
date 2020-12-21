#include "Management.h"
#pragma once
#ifndef __MANAGEMENT_HPP__
#define __MANAGEMENT_HPP__
//#include "Management.h"
BEGIN(Engine)
#pragma region GAMEOBJECT_MANAGER
template<typename T>
inline CGameObject * CManagement::AddGameObject(const UINT _nSceneID)
{
	if (nullptr == m_pGameObjectManager)
	{
		PrintLog(TEXT("Warning"), TEXT("Failed to add gameobject. m_pGameObjectManager is nullptr."));
		return nullptr;
	}
	return m_pGameObjectManager->AddGameObject<T>(_nSceneID);
}
template<typename T>
inline CGameObject * CManagement::Instantiate(const size_t _nSceneID)
{
	if (nullptr == m_pGameObjectManager)
	{
		PrintLog(TEXT("Warning"), TEXT("Failed to add gameobject. m_pGameObjectManager is nullptr."));
		return nullptr;
	}
	return m_pGameObjectManager->Instantiate<T>(_nSceneID);
}
template<typename T>
inline CGameObject * CManagement::FindGameObjectOfType(const size_t _nSceneID)
{
	if (nullptr == m_pGameObjectManager)
	{
		PrintLog(TEXT("Warning"), TEXT("Failed to find gameobject. m_pGameObjectManager is nullptr."));
		return nullptr;
	}
	return m_pGameObjectManager->FindGameObjectOfType<T>(_nSceneID);
}
template<typename T>
inline list<CGameObject*> CManagement::FindGameObjectsOfType(const size_t _nSceneID)
{
	if (nullptr == m_pGameObjectManager)
	{
		PrintLog(TEXT("Warning"), TEXT("Failed to find gameobjects. m_pGameObjectManager is nullptr."));
		return list<CGameObject*>();
	}
	return m_pGameObjectManager->FindGameObjectsOfType<T>(_nSceneID);
}
template<typename T>
inline list<CGameObject*> CManagement::FindGameObjectsOfBaseType(const size_t _nSceneID)
{
	if (nullptr == m_pGameObjectManager)
	{
		PrintLog(TEXT("Warning"), TEXT("Failed to find gameobjects. m_pGameObjectManager is nullptr."));
		return list<CGameObject*>();
	}
	return m_pGameObjectManager->FindGameObjectsOfBaseType<T>(_nSceneID);
}
#pragma endregion
END
#endif // !__MANAGEMENT_HPP__
