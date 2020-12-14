#include "..\Header\GameObjectManager.h"
#include "GameObject.h"
USING(Engine)
IMPLEMENT_SINGLETON(CGameObjectManager)

CGameObjectManager::CGameObjectManager()
{
}

HRESULT CGameObjectManager::Awake(const size_t _nSceneID)
{
	auto iter = m_GameObjects.find(_nSceneID);

	if (m_GameObjects.end() == iter)
	{
		TCHAR szBuf[128] = TEXT("");
		_stprintf_s(szBuf, 128, TEXT("Failed to Awake gameobject. %d container does not exist."), _nSceneID);
		PrintLog(TEXT("Warning"), szBuf);
		return E_FAIL;
	}

	for (auto pGameObject : iter->second)
	{
		if (FAILED(pGameObject->Awake()))
			return E_FAIL;
	}
	return S_OK;
}

HRESULT CGameObjectManager::Start(const size_t _nSceneID)
{
	auto iter = m_GameObjects.find(_nSceneID);

	if (m_GameObjects.end() == iter)
	{
		TCHAR szBuf[128] = TEXT("");
		_stprintf_s(szBuf, 128, TEXT("Failed to Start gameobject. %d container does not exist."), _nSceneID);
		PrintLog(TEXT("Warning"), szBuf);
		return E_FAIL;
	}

	for (auto pGameObject : iter->second)
	{
		if (FAILED(pGameObject->Start()))
			return E_FAIL;
	}
	return S_OK;
}

UINT CGameObjectManager::Update(const size_t _nSceneID, const float _fDeltaTime)
{
	auto iter = m_GameObjects.find(_nSceneID);

	if (m_GameObjects.end() == iter)
	{
		TCHAR szBuf[128] = TEXT("");
		_stprintf_s(szBuf, 128, TEXT("Failed to Update gameobject. %d container does not exist."), _nSceneID);
		PrintLog(TEXT("Warning"), szBuf);
		return 0;
	}

	for (auto pGameObject : iter->second)
	{
		pGameObject->Update(_fDeltaTime);
	}
	return 0;
}

UINT CGameObjectManager::LateUpdate(const size_t _nSceneID, const float _fDeltaTime)
{
	auto iter = m_GameObjects.find(_nSceneID);

	if (m_GameObjects.end() == iter)
	{
		TCHAR szBuf[128] = TEXT("");
		_stprintf_s(szBuf, 128, TEXT("Failed to LateUpdate gameobject. %d container does not exist."), _nSceneID);
		PrintLog(TEXT("Warning"), szBuf);
		return 0;
	}

	for (auto pGameObject : iter->second)
	{
		pGameObject->LateUpdate(_fDeltaTime);
	}
	return 0;
}

HRESULT CGameObjectManager::AddPrototype(CGameObject* const _pGameObject)
{
#ifdef UNICODE
	wstring sTypeName = StringAToStringW(typeid(*_pGameObject).name());
#else
	string sTypeName = typeid(*_pGameObject).name();
#endif
	int nFind = (sTypeName.find(' ') + 1);
	sTypeName = sTypeName.substr(nFind);

	auto iter = m_Prototype.find(sTypeName);

	if (m_Prototype.end() != iter)
	{
		PrintLog(TEXT("Warning"), TEXT("Prototype is already exist"));
		return E_FAIL;
	}

	m_Prototype.emplace(sTypeName, _pGameObject);
	_pGameObject->InitializePrototype();
	return S_OK;
}

CGameObject * CGameObjectManager::AddGameObject(const size_t _nSceneID, const TSTRING & _sTypeName)
{
	auto iter_Prototype = m_Prototype.find(_sTypeName);

	if (m_Prototype.end() == iter_Prototype)
	{
		TCHAR szBuf[128] = TEXT("");
		_stprintf_s(szBuf, 128, TEXT("Failed to Add GameObject. %s Prototype does not exist."), _sTypeName.c_str());
		PrintLog(TEXT("Warning"), szBuf);
		return nullptr;
	}

	CGameObject* pClone = iter_Prototype->second->Clone();

	//pCloneÀÇ SceneID ¼³Á¤.
	pClone->SetSceneID(_nSceneID);

	if (nullptr == pClone)
	{
		TCHAR szBuf[128] = TEXT("");
		_stprintf_s(szBuf, 128, TEXT("Failed to Clone %s GameObject."), _sTypeName.c_str());
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

void CGameObjectManager::Free()
{
	for (auto rPair : m_Prototype)
	{
		SafeRelease(rPair.second);
	}
	m_Prototype.clear();


	for (auto rPair : m_GameObjects)
	{
		for (CGameObject* pGameObject : rPair.second)
		{
			SafeRelease(pGameObject);
		}
		rPair.second.clear();
	}
	m_GameObjects.clear();
}
