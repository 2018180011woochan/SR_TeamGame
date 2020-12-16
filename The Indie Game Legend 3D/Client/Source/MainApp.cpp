#include "stdafx.h"
#include "MainApp.h"
#include "KeyManager.h"
#include "MsgManager.h"
#include "FactoryManager.h"
#include "Intro.h"
CMainApp::CMainApp()
	: m_pManagement(CManagement::GetInstance())
{
	SafeAddRef(m_pManagement);
}

void CMainApp::Free()
{
	m_pTexturePoolManager->Release();
	SafeRelease(m_pDevice);
    SafeRelease(m_pManagement);
	CManagement::GetInstance()->ReleaseEngine();
	CFactoryManager::DeleteInstance();
	CKeyManager::DeleteInstance();
	CMsgManager::DeleteInstance();
}

HRESULT CMainApp::Initialize()
{
	if (FAILED(m_pManagement->Initialize(g_hWnd, 1280, 720, EDisplayMode::Window,120.f)))
	{
		PrintLog(TEXT("Error"), TEXT("Failed to ready engine."));
		return E_FAIL;
	}
	m_pTexturePoolManager = CTexturePoolManager::GetInstance();
	m_pDevice = m_pManagement->GetDevice();
	if (nullptr == m_pDevice)
		return E_FAIL;

	SafeAddRef(m_pDevice);

	if (FAILED(ReadyStageResources()))
		return E_FAIL;

	if (FAILED(ReadyDefaultSetting()))
		return E_FAIL;

	if (FAILED(m_pManagement->SetUpCurrentScene(CIntro::Create())))
	{
		PrintLog(L"Error", L"Failed To SetUpCurrentScene");
		return E_FAIL;
	}

	//Test
	D3DLIGHT9 light;
	ZeroMemory(&light, sizeof(D3DLIGHT9));
	light.Diffuse.r = 1.f;
	light.Diffuse.g = 1.f;
	light.Diffuse.b = 1.f;
	light.Type = D3DLIGHT_DIRECTIONAL;

	_vector dir = _vector(1, -1, 1);
	D3DXVec3Normalize((_vector*)&light.Direction, &dir);
	m_pDevice->SetLight(0, &light);
	m_pDevice->LightEnable(0, TRUE);

	D3DLIGHT9 light2;
	ZeroMemory(&light, sizeof(D3DLIGHT9));
	light2.Diffuse.r = 1.f;
	light2.Diffuse.g = 1.f;
	light2.Diffuse.b = 1.f;
	light2.Type = D3DLIGHT_DIRECTIONAL;

	dir = _vector(-1, -1, -1);
	D3DXVec3Normalize((_vector*)&light2.Direction, &dir);
	m_pDevice->SetLight(1, &light2);
	m_pDevice->LightEnable(1, TRUE);

	m_pDevice->SetRenderState(D3DRS_LIGHTING, false);
	m_pDevice->SetRenderState(D3DRS_AMBIENT, 0x00808080);
	//Test
	return S_OK;
}

UINT CMainApp::Update()
{
	if (nullptr == m_pManagement)
		return 0;
	CKeyManager::GetInstance()->Update_KeyManager();
	m_pManagement->Running();
	return S_OK;
}

HRESULT CMainApp::ReadyStageResources()
{ 
	return S_OK;
}

HRESULT CMainApp::ReadyDefaultSetting()
{
	return S_OK;
}

CMainApp * CMainApp::Create()
{
	CMainApp* pInstance = new CMainApp;
	if (FAILED(pInstance->Initialize()))
	{
		PrintLog(TEXT("Error"), TEXT("Failed to create CMainApp"));
		SafeRelease(pInstance);
	}
	return pInstance;
}
