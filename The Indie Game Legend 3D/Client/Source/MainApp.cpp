#include "stdafx.h"
#include "MainApp.h"
#include "KeyManager.h"
#include "MsgManager.h"
#include "FactoryManager.h"
#include "Intro.h"
#include "SoundMgr.h"
#include "LightMananger.h"
#include "UtilityManger.h"
#include "CameraManager.h"

#include "FinalStage.h"

CMainApp::CMainApp()
	: m_pManagement(CManagement::GetInstance())
{
	SafeAddRef(m_pManagement);
}

void CMainApp::Free()
{
	CCameraManager::DeleteInstance(); // Camera Reference
	CLightMananger::DeleteInstance();//Device Reference
	CUtilityManger::Release(); //GameObject Reference
	m_pTexturePoolManager->Release();
	SafeRelease(m_pDevice);
    SafeRelease(m_pManagement);
	CManagement::GetInstance()->ReleaseEngine();
	//m_pTexturePoolManager->Release();
	CFactoryManager::DeleteInstance();
	CKeyManager::DeleteInstance();
	CMsgManager::DeleteInstance();
	CSoundMgr::DeleteInstance();
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

	if (FAILED(m_pManagement->SetUpCurrentScene(CFinalStage::Create())))
	{
		PrintLog(L"Error", L"Failed To SetUpCurrentScene");
		return E_FAIL;
	}

	m_pDevice->SetRenderState(D3DRS_LIGHTING, false);
	return S_OK;
}

UINT CMainApp::Update()
{
	if (nullptr == m_pManagement)
		return 0;
	CKeyManager::GetInstance()->Update_KeyManager();
	CMsgManager::GetInstance()->UpdateSkillTime(m_pManagement->Running());
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
