#include "stdafx.h"
#include "Stage.h"
#include "Bub.h"

CStage::CStage()
	: CScene(this)
{
}

HRESULT CStage::Awake()
{
	if (FAILED(AddMonsterLayer(L"Layer_Monster")))
		return E_FAIL;

	CScene::Awake();
	return S_OK;
}

HRESULT CStage::Start()
{
	CScene::Start();

	return S_OK;
}

UINT CStage::Update(float _fDeltaTime)
{
	CScene::Update(_fDeltaTime);

	return 0;
}

UINT CStage::LateUpdate(float _fDeltaTime)
{
	CScene::LateUpdate(_fDeltaTime);

	return 0;
}

HRESULT CStage::AddMonsterLayer(const wstring & LayerTag)
{
	auto pManagement = CManagement::GetInstance();
	if (nullptr == pManagement)
		return E_FAIL;

	AddPrototype(CBub::Create());
	AddGameObject<CBub>();


	return S_OK;
}

CStage * CStage::Create()
{
	CStage* pInstance = new CStage;
	//if (FAILED(pInstance->Awake()))
	//{
	//	PrintLog(L"Error", L"Failed To Create CStage");
	//	SafeRelease(pInstance);
	//}

	return pInstance;
}

void CStage::Free()
{
	CScene::Free();
}
