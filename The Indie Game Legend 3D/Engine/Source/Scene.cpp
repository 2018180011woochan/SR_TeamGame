#include "..\Header\Scene.h"

USING(Engine)

CScene::CScene(const UINT _nSceneID)
	: m_pManagement(CManagement::GetInstance())
	, m_pDevice(m_pManagement->GetDevice())
	, m_bActive(true)
{
	SafeAddRef(m_pManagement);
	SafeAddRef(m_pDevice);
	m_nSceneID = _nSceneID;
}

void CScene::Free()
{
	SafeRelease(m_pDevice);
	SafeRelease(m_pManagement);
}

HRESULT CScene::Awake()
{
	if (nullptr == m_pManagement)
		return E_FAIL;
	return m_pManagement->AwakeGameObject(m_nSceneID);
}

HRESULT CScene::Start()
{
	if (nullptr == m_pManagement)
		return E_FAIL;
	return m_pManagement->StartGameObject(m_nSceneID);
}

UINT CScene::Update(const float _fDeltaTime)
{
	if (nullptr == m_pManagement)
		return E_FAIL;
	return m_pManagement->UpdateGameObject(m_nSceneID, _fDeltaTime);
}

UINT CScene::LateUpdate(const float _fDeltaTime)
{
	if (nullptr == m_pManagement)
		return E_FAIL;
	return m_pManagement->LateUpdateGameObject(m_nSceneID, _fDeltaTime);
}

HRESULT CScene::SetUpCurrentScene(CScene * const _pCurrentScene)
{
	if (nullptr == m_pManagement)
		return E_FAIL;
	return m_pManagement->SetUpCurrentScene(_pCurrentScene);
}

HRESULT CScene::AddPrototype(CGameObject * const _pGameObject)
{
	if (nullptr == m_pManagement)
		return E_FAIL;
	return m_pManagement->AddPrototype(_pGameObject);
}

CGameObject * CScene::AddGameObject(const TSTRING & _sTypeName)
{
	if (nullptr == m_pManagement)
		return nullptr;
	return m_pManagement->AddGameObject(m_nSceneID, _sTypeName);
}

bool CScene::IsActive()
{
	return m_bActive;
}

void CScene::SetActive(const bool _bActive)
{
	m_bActive = _bActive;
}
