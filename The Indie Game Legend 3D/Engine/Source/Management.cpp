#include "..\Header\Management.h"
#include "MeshManager.h"
#include "FontManager.h"

USING(Engine)
IMPLEMENT_SINGLETON(CManagement)

CManagement::CManagement()
	: m_pGraphicDevice(CGraphicDevice::GetInstance())
	, m_pGameObjectManager(CGameObjectManager::GetInstance())
	, m_pSceneManager(CSceneManager::GetInstance())
	, m_pTimeManager(CTimeManager::GetInstance())
	, m_pFrameManager(CFrameManager::GetInstance())
{
	SafeAddRef(m_pGraphicDevice);
	SafeAddRef(m_pGameObjectManager);
	SafeAddRef(m_pSceneManager);
	SafeAddRef(m_pTimeManager);
	SafeAddRef(m_pFrameManager);
}

void CManagement::Free()
{
}

void CManagement::ReleaseEngine()
{
	SafeRelease(m_pRenderer);
	SafeRelease(m_pGameObjectManager);
	SafeRelease(m_pSceneManager);
	SafeRelease(m_pGraphicDevice);
	SafeRelease(m_pTimeManager);
	SafeRelease(m_pFrameManager);
	if (CGraphicDevice::DeleteInstance())
		PrintLog(L"Warning", L"Failed To Release CGraphicDevice");
	if (CGameObjectManager::DeleteInstance())
		PrintLog(L"Warning", L"Failed To Release CGameObjectManager");
	if (CSceneManager::DeleteInstance())
		PrintLog(L"Warning", L"Failed To Release CSceneManager");
	if (CTimeManager::DeleteInstance())
		PrintLog(L"Warning", L"Failed To Release CTimeManager");
	if (FAILED(CMeshManager::DeleteInstance()))
		PrintLog(L"Warning", L"Failed To Release CMeshManager");
	if (FAILED(CFontManager::DeleteInstance()))
		PrintLog(L"Warning", L"Failed To Release CFontManager");
	if (FAILED(CFrameManager::DeleteInstance()))
		PrintLog(L"Warning", L"Failed To Release CFrameManager");
	if (CManagement::DeleteInstance())
		PrintLog(L"Warning", L"Failed To Release CManagement");
}

HRESULT CManagement::Initialize(const HWND _hWnd, const UINT _nWinCX, const UINT _nWinCY, const EDisplayMode _eDisplayMode,const float _fSetFrame)
{
	if (nullptr == m_pGraphicDevice || FAILED(m_pGraphicDevice->Initialize(_hWnd, _nWinCX, _nWinCY, _eDisplayMode)))
		return E_FAIL;
	if (nullptr == m_pTimeManager || FAILED(m_pTimeManager->Initialize()))
		return E_FAIL;

	m_pRenderer = CRenderer::Create(m_pGraphicDevice->GetDevice());
	if (nullptr == m_pRenderer)
	{
		PrintLog(TEXT("Error"), TEXT("Failed to create renderer"));
		return E_FAIL;
	}

	m_hWnd = _hWnd;
	m_pFrameManager->Set_LimitFrame(_fSetFrame);
	return S_OK;
}

UINT CManagement::Update()
{
	if (nullptr == m_pSceneManager)
		return 0;
	float fDeltaTime = m_pTimeManager->GetDeltaTime();
	if (m_pFrameManager->LimitFrame())
	{
		m_pSceneManager->Update(fDeltaTime);
		m_pSceneManager->LateUpdate(fDeltaTime);
	}
	m_pFrameManager->ShowFrame(m_hWnd, fDeltaTime);
	return 0;
}

HRESULT CManagement::Render(HWND _hWnd)
{
	if (nullptr == m_pRenderer)
		return E_FAIL;

	return m_pRenderer->Render(_hWnd);
} 

float CManagement::Running()
{
	if (nullptr == m_pSceneManager)
		return 0.f;

	if (m_pFrameManager->LimitFrame())
	{
		float fDeltaTime = m_pTimeManager->GetDeltaTime();
		m_pSceneManager->Update(fDeltaTime);
		m_pSceneManager->LateUpdate(fDeltaTime);
		m_pRenderer->Render(m_hWnd);
		m_pFrameManager->ShowFrame(m_hWnd, fDeltaTime);
		return fDeltaTime;
	}
	return 0.f;
}

HRESULT CManagement::AwakeGameObject(const size_t _nSceneID)
{
	if (nullptr == m_pGameObjectManager)
		return E_FAIL;
	return m_pGameObjectManager->Awake(_nSceneID);
}

HRESULT CManagement::StartGameObject(const size_t _nSceneID)
{
	if (nullptr == m_pGameObjectManager)
		return E_FAIL;
	return m_pGameObjectManager->Start(_nSceneID);
}

UINT CManagement::UpdateGameObject(const size_t _nSceneID, const float _fDeltaTime)
{
	if (nullptr == m_pGameObjectManager)
		return E_FAIL;
	return m_pGameObjectManager->Update(_nSceneID, _fDeltaTime);
}

UINT CManagement::LateUpdateGameObject(const size_t _nSceneID, const float _fDeltaTime)
{
	if (nullptr == m_pGameObjectManager)
		return E_FAIL;
	return m_pGameObjectManager->LateUpdate(_nSceneID, _fDeltaTime);
}

HRESULT CManagement::AddInRenderer(const ERenderID _eRenderID, CGameObject * const _pGameObject)
{
	if (nullptr == m_pRenderer)
		return E_FAIL;
	return m_pRenderer->AddInRenderer(_eRenderID, _pGameObject);
}

HRESULT CManagement::AddPrototype(CGameObject * const _pGameObject)
{
	if (nullptr == m_pGameObjectManager)
	{
		PrintLog(TEXT("Error"), TEXT("Failed to add prototype. m_pGameObjectManager is nullptr."));
		return E_FAIL;
	}
	return m_pGameObjectManager->AddPrototype(_pGameObject);
}

CGameObject * CManagement::AddGameObject(const UINT _nSceneID, const TSTRING & _sTypeName)
{
	if (nullptr == m_pGameObjectManager)
	{
		PrintLog(TEXT("Warning"), TEXT("Failed to add gameobject. m_pGameObjectManager is nullptr."));
		return nullptr;
	}
	return m_pGameObjectManager->AddGameObject(_nSceneID, _sTypeName);
}

LPDIRECT3DDEVICE9 CManagement::GetDevice()
{
	if(nullptr == m_pGraphicDevice)
	{
		PrintLog(TEXT("Error"), TEXT("Failed to get graphic device. m_pGraphicDevice is nullptr."));
		return nullptr;
	}
	return m_pGraphicDevice->GetDevice();
}

HRESULT CManagement::SetUpCurrentScene(CScene * const _pCurrentScene)
{
	if (nullptr == m_pSceneManager)
		return E_FAIL;
	return m_pSceneManager->SetUpCurrentScene(_pCurrentScene);
}
