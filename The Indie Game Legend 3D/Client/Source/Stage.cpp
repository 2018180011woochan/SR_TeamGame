#include "stdafx.h"
#include "Stage.h"

#pragma region MONSTER
#include "Bub.h"
#include "Rub.h"
#include "sqrNub.h"
#include "Turret.h"
#include "TurretBullet.h"
#include "Walker.h"
#include "WalkerBullet.h"
#include "Cryder.h"
#include "Egg.h"
#include "WalkerBoss.h"
#include "WalkBossBullet.h"
#pragma endregion

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
#include "HeartManager.h"
#include "Heart.h"
#include "CrossHair.h"
#pragma endregion
#include "SkyBox.h"

//tile
#include "LavaTile.h"
#include "ElectricTile.h"
#include "SwampTile.h"
#include "SandTile.h"

//Objct
#include "Slider.h"

//tset
#include "SoundMgr.h"

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
	AddPrototype(CWalker::Create());
	AddPrototype(CWalkerBullet::Create());
	AddPrototype(CCryder::Create());
	AddPrototype(CEgg::Create());
	AddPrototype(CWalkerBoss::Create());
	AddPrototype(CWalkBossBullet::Create());

	AddPrototype(CSandTile::Create());
	AddPrototype(CElectricTile::Create());
	AddPrototype(CLavaTile::Create());
	AddPrototype(CSwampTile::Create());

	AddPrototype(CSlider::Create());


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

	// Test용으로 추가함
	AddGameObject<CSlider>();

	//AddGameObject<CBub>();
	//AddGameObject<CRub>();
	AddGameObject<CsqrNub>();
	//AddGameObject<CTurret>();
	AddGameObject<CWalker>();
	AddGameObject<CCryder>();
	AddGameObject<CEgg>();
	AddGameObject<CWalkerBoss>();

	CSector* pSector = (CSector*)AddGameObject<CSector>();
	pSector->SetSectorName(L"Sector1");

	AddUIObject();

#pragma region SKYBOX
	AddPrototype(CSkyBox::Create());
	AddGameObject<CSkyBox>();
#pragma endregion
	CScene::Awake();

	CSoundMgr::GetInstance()->Initialize();
	CSoundMgr::GetInstance()->PlayBGM(L"BGM_Test.mp3");
	return S_OK;
}

HRESULT CStage::Start()
{

	CFactoryManager::GetInstance()->LoadDataFile(L"TileTest");
	CFactoryManager::GetInstance()->LoadScene(this);

	CScene::Start();

	return S_OK;
}

UINT CStage::Update(float _fDeltaTime)
{
	CScene::Update(_fDeltaTime);
	//Test
	static float fTestVolum = 1.f;

	if (GetAsyncKeyState('1') & 0x8000)
	{
		fTestVolum -= 0.01f;
	}
	if (GetAsyncKeyState('2') & 0x8000)
	{
		fTestVolum += 0.01f;
	}
	fTestVolum = CLAMP(fTestVolum, 0.f, 1.f);

	CSoundMgr::GetInstance()->SetVolume(CSoundMgr::BGM, fTestVolum);
	//Test
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
	AddPrototype(CHeart::Create());
	AddPrototype(CHeartManager::Create());
	AddPrototype(CCrossHair::Create());
	AddGameObject<CAmmoGauge>();
	AddGameObject<CAmmoFrame>();
	AddGameObject<CHeartManager>();
	AddGameObject<CCrossHair>();
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
