#include "stdafx.h"
#include "..\Header\HeartManager.h"


CHeartManager::CHeartManager()
	: m_nMaxHeart(6)
	, m_pHeart(nullptr)
{
}

void CHeartManager::Free()
{
	CGameObject::Free();
}

CHeartManager * CHeartManager::Create()
{
	CHeartManager* pInstance = new CHeartManager;

	if (FAILED(pInstance->InitializePrototype()))
	{
		delete pInstance;
		return nullptr;
	}
	return pInstance;
}

CGameObject * CHeartManager::Clone()
{
	CHeartManager* pClone = new CHeartManager(*this);
	return pClone;
}

HRESULT CHeartManager::InitializePrototype()
{
	CGameObject::InitializePrototype();
	return S_OK;
}

HRESULT CHeartManager::Awake()
{
	CGameObject::Awake();
	return S_OK;
}

HRESULT CHeartManager::Start()
{
	CGameObject::Start();
	return S_OK;
}

UINT CHeartManager::Update(const float _fDeltaTime)
{
	CGameObject::Update(_fDeltaTime);
	return 0;
}

UINT CHeartManager::LateUpdate(const float _fDeltaTime)
{
	CGameObject::LateUpdate(_fDeltaTime);
	return 0;
}

HRESULT CHeartManager::Render()
{
	CGameObject::Render();
	return S_OK;
}
