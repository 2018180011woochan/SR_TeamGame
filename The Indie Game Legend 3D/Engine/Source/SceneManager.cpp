#include "..\Header\SceneManager.h"
#include "Scene.h"
USING(Engine)
IMPLEMENT_SINGLETON(CSceneManager)
CSceneManager::CSceneManager()
	:m_pCurrentScene(nullptr)
{
}

void CSceneManager::Free()
{
	SafeRelease(m_pCurrentScene);
}

HRESULT CSceneManager::SetUpCurrentScene(CScene * const _pCurrentScene)
{
	if (nullptr == _pCurrentScene)
		return E_FAIL;

	if (_pCurrentScene != m_pCurrentScene)
	{
		SafeRelease(m_pCurrentScene);
		m_pCurrentScene = _pCurrentScene;
		_pCurrentScene->Awake();
		_pCurrentScene->Start();
	}

	return S_OK;
}

UINT CSceneManager::Update(const float _fDeltaTime)
{
	if (nullptr == m_pCurrentScene || false == m_pCurrentScene->IsActive())
		return 0;
	return m_pCurrentScene->Update(_fDeltaTime);
}

UINT CSceneManager::LateUpdate(const float _fDeltaTime)
{
	if (nullptr == m_pCurrentScene || false == m_pCurrentScene->IsActive())
		return 0;
	return m_pCurrentScene->LateUpdate(_fDeltaTime);
}

bool CSceneManager::IsCurrentSceneActive()
{
	if (nullptr == m_pCurrentScene || false == m_pCurrentScene->IsActive())
		return false;
	return true;
}
