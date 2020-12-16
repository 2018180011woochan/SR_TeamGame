#include "stdafx.h"
#include "Stage.h"
#include "Bub.h"
#include "Rub.h"
#include "sqrNub.h"
#include "Turret.h"
#include "TurretBullet.h"
#include "FactoryManager.h"
#include "Player.h"
#include "PlayerCamera.h"
#include "Mouse.h"
#include "Sector.h"
#include "PlayerBullet.h"
#include "BulletSpawn.h"

#pragma region INCLUDE_UI
#include "AmmoGauge.h"
#include "AmmoFrame.h"
#pragma endregion


CStage::CStage()
	: CScene(GetTypeHashCode<CStage>())
{
}

HRESULT CStage::Awake()
{
	AddPrototype(CBub::Create());
	AddPrototype(CRub::Create());
	AddPrototype(CsqrNub::Create());
	AddPrototype(CTurret::Create());
	AddPrototype(CTurretBullet::Create());
	AddPrototype(CPlayer::Create());
	AddPrototype(CPlayerCamera::Create());
	AddPrototype(CMouse::Create());
	AddPrototype(CSector::Create());
	AddPrototype(CPlayerBullet::Create());
	AddPrototype(CBulletSpawn::Create());
	AddGameObject<CPlayer>();
	AddGameObject<CPlayerCamera>();
	AddGameObject<CBulletSpawn>();
	AddGameObject<CMouse>();

	//AddGameObject<CBub>();
	// Test용으로 추가함
	AddGameObject<CRub>();
	AddGameObject<CsqrNub>();
	AddGameObject<CTurret>();

	CSector* pSector = (CSector*)AddGameObject<CSector>();
	pSector->SetSectorName(L"Sector1");

	AddUIObject();
	CScene::Awake();
	return S_OK;
}

HRESULT CStage::Start()
{

	CFactoryManager::GetInstance()->LoadDataFile(L"Test2");
	CFactoryManager::GetInstance()->LoadScene(this);

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


	return S_OK;
}

HRESULT CStage::AddUIObject()
{
	AddPrototype(CAmmoGauge::Create());
	AddPrototype(CAmmoFrame::Create());
	AddGameObject<CAmmoGauge>();
	AddGameObject<CAmmoFrame>();
	return S_OK;
}

CStage * CStage::Create()
{
	CStage* pInstance = new CStage;
	return pInstance;
}

void CStage::Free()
{
	CScene::Free();
}
