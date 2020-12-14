#include "stdafx.h"
#include "MainApp.h"
#include "KeyManager.h"
#include "MsgManager.h"
#include "FactoryManager.h"
#include "Stage.h"
#include "Bub.h"
CMainApp::CMainApp()
	: m_pManagement(CManagement::GetInstance())
{
	SafeAddRef(m_pManagement);
}

void CMainApp::Free()
{
	CFactoryManager::DeleteInstance();
	CKeyManager::DeleteInstance();
	CMsgManager::DeleteInstance();
	m_pTexturePoolManager->Release();
	SafeRelease(m_pDevice);
	m_pManagement->ReleaseEngine();
	SafeRelease(m_pManagement);
	//CManagement::ReleaseEngine();
}

HRESULT CMainApp::Initialize()
{
	if (FAILED(m_pManagement->Initialize(g_hWnd, g_nWinCX, g_nWinCY, EDisplayMode::Window)))
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

	if (FAILED(m_pManagement->SetUpCurrentScene(CStage::Create())))
	{
		PrintLog(L"Error", L"Failed To SetUpCurrentScene");
		return E_FAIL;
	}
	//CFactoryManager::GetInstance()->LoadDataFile(L"Test");
	return S_OK;
}

UINT CMainApp::Update()
{
	if (nullptr == m_pManagement)
		return 0;
	//CKeyManager::GetInstance()->Update_KeyManager();
	m_pManagement->Update();
	m_pManagement->Render(g_hWnd);
	return S_OK;
}

HRESULT CMainApp::ReadyStageResources()
{
//#pragma region GameObject_Monster_Bub
//	//if (FAILED(m_pManagement->AddPrototype(CBub::Create())))
//	//	return E_FAIL;
//#pragma endregion
//
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
