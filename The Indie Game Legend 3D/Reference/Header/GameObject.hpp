#pragma once
#ifndef __GAMEOBJECT_HPP__
#include "Management.h"
#include "Component.h"
#include "GameObject.h"

BEGIN(Engine)
template<typename T>
inline CGameObject * CGameObject::AddGameObject()
{
	if (nullptr == m_pManagement)
		return nullptr;
	return m_pManagement->Instantiate<T>(m_nSceneID);
}

template<typename T>
inline CGameObject * CGameObject::FindGameObjectOfType()
{
	if (nullptr == m_pManagement)
		return nullptr;
	return m_pManagement->FindGameObjectOfType<T>(m_nSceneID);
}

template<typename T>
inline list<CGameObject*> CGameObject::FindGameObjectsOfType()
{
	if (nullptr == m_pManagement)
		return list<CGameObject*>();
	return m_pManagement->FindGameObjectsOfType<T>(m_nSceneID);
}
template<typename T>
inline CComponent * CGameObject::AddComponent()
{
	TSTRING sTypeName = GetTypeName<T>();

	auto iter = m_Components.find(sTypeName);

	if (m_Components.end() != iter)
	{
		TCHAR szBuf[128] = TEXT("");
   		_stprintf_s(szBuf, 128, TEXT("Failed to add component. %s is already exist."), sTypeName.c_str());
		PrintLog(TEXT("Warning"), szBuf);
		return iter->second;
	}

	CComponent* pComponent = T::Create(this, m_pDevice);

	m_Components.emplace(sTypeName, pComponent);

	return pComponent;
}
template<typename T>
inline CComponent * CGameObject::GetComponent()
{
	TSTRING sTypeName = GetTypeName<T>();

	auto iter = m_Components.find(sTypeName);

	if (m_Components.end() == iter)
	{
		TCHAR szBuf[128] = TEXT("");
		_stprintf_s(szBuf, 128, TEXT("Failed to get component. %s does not exist."), sTypeName.c_str());
		return nullptr;
	}

	return iter->second;
}
END
#define __GAMEOBJECT_HPP__
#endif // !__GAMEOBJECT_HPP__
