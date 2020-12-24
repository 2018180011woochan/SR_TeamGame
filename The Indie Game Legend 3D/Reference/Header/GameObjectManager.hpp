#pragma once
#ifndef __GAMEOBJECT_MANAGER_HPP__
#include "GameObjectManager.h"
BEGIN(Engine)

/*�⺻ ���� 
��ü Ÿ�԰� �����̵� �ٰ� 
������ Ÿ�Կ��� ã�� ��ü�� Ŭ������ �����ѵ� ���� */
template<typename T>
CGameObject* CGameObjectManager::AddGameObject(const size_t _nSceneID)
{
	TSTRING sTypeName = GetTypeName<T>();

	auto iter_Prototype = m_Prototype.find(sTypeName);

	if (m_Prototype.end() == iter_Prototype)
	{
		TCHAR szBuf[128] = TEXT("");
		_stprintf_s(szBuf, 128, TEXT("Failed to Add GameObject. %s Prototype does not exist."), sTypeName.c_str());
		PrintLog(TEXT("Warning"), szBuf);
		return nullptr;
	}

	CGameObject* pClone = iter_Prototype->second->Clone();

	//pClone�� SceneID ����.
	pClone->SetSceneID(_nSceneID);

	if (nullptr == pClone)
	{
		TCHAR szBuf[128] = TEXT("");
		_stprintf_s(szBuf, 128, TEXT("Failed to Clone %s GameObject."), sTypeName.c_str());
		PrintLog(TEXT("Warning"), szBuf);
		return nullptr;
	}

	auto iter_GameObjects = m_GameObjects.find(_nSceneID);

	if (m_GameObjects.end() == iter_GameObjects)
	{
		iter_GameObjects = (m_GameObjects.emplace(_nSceneID, list<CGameObject*>())).first;
	}

	iter_GameObjects->second.emplace_back(pClone);
	return pClone;
}
/*
�� �����߿� �����Ǵ� ��ü �ʱ�ȭ �Լ� 
���ø�Ÿ������ Ŭ���� ��ü�� �ް� ��������� ID �ް�
�ش� ���� ���� ����Ʈ�� �����ϴ��� Ȯ�ε� 
���� ����Ʈ�� Ŭ�� �߰�
*/
template<typename T>
inline CGameObject * CGameObjectManager::Instantiate(const size_t _nSceneID)
{
	TSTRING sTypeName = GetTypeName<T>();

	auto iter_Prototype = m_Prototype.find(sTypeName);

	if (m_Prototype.end() == iter_Prototype)
	{
		TCHAR szBuf[128] = TEXT("");
		_stprintf_s(szBuf, 128, TEXT("Failed to Add GameObject. %s Prototype does not exist."), sTypeName.c_str());
		PrintLog(TEXT("Warning"), szBuf);
		return nullptr;
	}

	CGameObject* pClone = iter_Prototype->second->Clone();


	if (nullptr == pClone)
	{
		TCHAR szBuf[128] = TEXT("");
		_stprintf_s(szBuf, 128, TEXT("Failed to Clone %s GameObject."), sTypeName.c_str());
		PrintLog(TEXT("Warning"), szBuf);
		return nullptr;
	}

	//pClone�� SceneID ����.
	pClone->SetSceneID(_nSceneID);

	auto iter_GameObjects = m_GameObjects.find(_nSceneID);

	if (m_GameObjects.end() == iter_GameObjects)
	{
		iter_GameObjects = (m_GameObjects.emplace(_nSceneID, list<CGameObject*>())).first;
	}

	iter_GameObjects->second.emplace_back(pClone);

	pClone->Awake();
	pClone->Start();
	return pClone;
}

template<typename T>
CGameObject* CGameObjectManager::FindGameObjectOfType(const size_t _nSceneID)
{
	auto iter_GameObjects = m_GameObjects.find(_nSceneID);

	if (m_GameObjects.end() == iter_GameObjects)
	{
		TCHAR szBuf[128] = TEXT("");
		_stprintf_s(szBuf, 128, TEXT("Failed to Find GameObject of Type. %d container does not exist."), _nSceneID);
		PrintLog(TEXT("Warning"), szBuf);
		return nullptr;
	}

	for (CGameObject* pGameObject : iter_GameObjects->second)
	{
		if (nullptr != dynamic_cast<T*>(pGameObject))
		{
			return (T*)pGameObject;
		}
	}

	return nullptr;
}

template<typename T>
list<CGameObject*> CGameObjectManager::FindGameObjectsOfType(const size_t _nSceneID)
{
	auto iter_GameObjects = m_GameObjects.find(_nSceneID);

	list<CGameObject*> gameObjects;

	if (m_GameObjects.end() == iter_GameObjects)
	{
		TCHAR szBuf[128] = TEXT("");
		_stprintf_s(szBuf, 128, TEXT("Failed to Find GameObject of Type. %d container does not exist."), _nSceneID);
		PrintLog(TEXT("Warning"), szBuf);
		return gameObjects;
	}

	for (CGameObject* pGameObject : iter_GameObjects->second)
	{
		if (nullptr != dynamic_cast<T*>(pGameObject))
		{
			gameObjects.push_back(pGameObject);
		}
	}

	return gameObjects;
}
template<typename T>
inline list<CGameObject*> CGameObjectManager::FindGameObjectsOfBaseType(const size_t _nSceneID)
{
	auto iter_GameObjects = m_GameObjects.find(_nSceneID);

	list<CGameObject*> gameObjects;

	if (m_GameObjects.end() == iter_GameObjects)
	{
		TCHAR szBuf[128] = TEXT("");
		_stprintf_s(szBuf, 128, TEXT("Failed to Find GameObject of Type. %d container does not exist."), _nSceneID);
		PrintLog(TEXT("Warning"), szBuf);
		return gameObjects;
	}

	for (CGameObject* pGameObject : iter_GameObjects->second)
	{
		if (nullptr != dynamic_cast<T*>(pGameObject))
		{
			gameObjects.push_back(pGameObject);
		}
	}

	return gameObjects;
}
END

#define __GAMEOBJECT_MANAGER_HPP__
#endif // !__GAMEOBJECT_MANAGER_HPP__
