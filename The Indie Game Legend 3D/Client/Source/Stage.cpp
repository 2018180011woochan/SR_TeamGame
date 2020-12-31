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
#include "NubBoss.h"
#include "Robobird.h"
#include "DoomBird.h"
#include "TreeBoss.h"
#include "RootAttack.h"
#include "WormBoss.h"
#include "WormBossBody.h"
#include "WormBossBody2.h"
#include "WormBossBody3.h"
#include "WormBossBody4.h"
#include "WormBossTail.h"
#pragma endregion

#pragma region SHOP
#include "ShopKeeper.h"
#include "Ammo.h"
#include "Armor.h"
#include "BigShot.h"
#pragma endregion

#pragma region EFFECT
#include "Blood.h"
#include "SmallExlode.h"
#include "Explosion.h"
#include "ExplosionBlue.h"
#pragma endregion

#pragma region NPC
#include "Phubans.h"
#include "Cactus.h"
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
#include "GemIcon.h"
#include "DiscIcon.h"
#include "MiniMapFrame.h"
#include "CardinalPoint.h"
#pragma endregion

#pragma region Collider
#include "Wall.h"
#include "Floor.h"
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
#include "RoomTrigger.h"
#include "LightMananger.h"
#include "Piramid.h"
#include "Item.h"
#include "BulletSpark.h"
#include "PlayerSpawn.h"
#include "PiramidUnBrake.h"
#include "KeyManager.h"
#include "WeaponHUD.h"
CStage::CStage()
	: CScene(GetTypeHashCode<CStage>())
{
}

HRESULT CStage::Awake()
{
	AddPrototype(CBlood::Create());
	AddPrototype(CSmallExlode::Create());
	AddPrototype(CExplosion::Create());
	AddPrototype(CExplosionBlue::Create());

	AddPrototype(CShopKeeper::Create());
	AddPrototype(CAmmo::Create());
	AddPrototype(CArmor::Create());
	AddPrototype(CBigShot::Create());

	AddPrototype(CPhubans::Create());
	AddPrototype(CCactus::Create());

	AddPrototype(CItem::Create());
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
	AddPrototype(CNubBoss::Create());
	AddPrototype(CRoboBird::Create());
	AddPrototype(CDoomBird::Create());
	AddPrototype(CTreeBoss::Create());
	AddPrototype(CRootAttack::Create());
	AddPrototype(CWormBoss::Create());
	AddPrototype(CWormBossBody::Create());
	AddPrototype(CWormBossBody2::Create());
	AddPrototype(CWormBossBody3::Create());
	AddPrototype(CWormBossBody4::Create());
	AddPrototype(CWormBossTail::Create());

	AddPrototype(CSandTile::Create());
	AddPrototype(CPiramid::Create());

	AddPrototype(CElectricTile::Create());
	AddPrototype(CLavaTile::Create());
	AddPrototype(CSwampTile::Create());
	AddPrototype(CPiramidUnBrake::Create());

	AddPrototype(CSlider::Create());
	AddPrototype(CRoomTrigger::Create());

	AddPrototype(CPlayer::Create());
	AddPrototype(CPlayerCamera::Create());
	AddPrototype(CMouse::Create());
	AddPrototype(CBulletSpark::Create());

	AddPrototype(CSector::Create());
	AddPrototype(CPlayerSpawn::Create());

	AddPrototype(CPlayerBullet::Create());
	AddPrototype(CBulletSpawn::Create());

	AddGameObject<CPlayer>();
	AddGameObject<CPlayerCamera>();
	AddGameObject<CBulletSpawn>();
	AddGameObject<CMouse>();


	// Test용으로 추가함
	//AddGameObject<CSlider>();

	//AddGameObject<CBub>();
	//AddGameObject<CRub>();
	//AddGameObject<CsqrNub>();
	//AddGameObject<CTurret>();
	//AddGameObject<CWalker>();
	//AddGameObject<CCryder>();
	//AddGameObject<CEgg>();
	//AddGameObject<CWalkerBoss>();
	//AddGameObject<CNubBoss>();
	//AddGameObject<CRoboBird>();
	//AddGameObject<CDoomBird>();

	//AddGameObject<CTreeBoss>();
	//AddGameObject<CWormBoss>();

	//AddGameObject<CTreeBoss>();

	//AddGameObject<CShopKeeper>();
	//AddGameObject<CPhubans>();
	AddGameObject<CCactus>();

	//컬링 테스트 
	
	//Light manager Test
	D3DXCOLOR color = D3DCOLOR_ARGB(255, 255, 255, 255);

	CLightMananger::GetInstance()->CreateDirction(CLightMananger::World1,
		_vector(1, -1, 1), color*0.9f, color, color*1.f);

	CLightMananger::GetInstance()->LightEnable(CLightMananger::World1, true);

	CLightMananger::GetInstance()->CreateDirction(CLightMananger::World2,
		_vector(-1, -1, -1), color*0.9f, color, color*1.f);

	CLightMananger::GetInstance()->LightEnable(CLightMananger::World2, true);
	CLightMananger::GetInstance()->LightOff();

	CSector* pSector = (CSector*)AddGameObject<CSector>();
	pSector->SetSectorName(L"Sector1");

	
	AddUIObject();

#pragma region SKYBOX
	AddPrototype(CSkyBox::Create());
	AddGameObject<CSkyBox>();
#pragma endregion

#pragma region Collider
	AddPrototype(CWall::Create());
	AddPrototype(CFloor::Create());
#pragma endregion


	CSoundMgr::GetInstance()->Initialize();
	CSoundMgr::GetInstance()->PlayBGM(L"BGM_Test.mp3");

	//CFactoryManager::GetInstance()->LoadDataFile(L"s1");
	//CFactoryManager::GetInstance()->LoadScene(this);
	//CFactoryManager::GetInstance()->LoadCollider(this, TEXT("Sector1_Collider"));

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
	//Test
	static float fTestVolum = 1.f;
	static bool bLight = false;
	if (GetAsyncKeyState('1') & 0x8000)
	{
		fTestVolum -= 0.01f;
	}
	if (GetAsyncKeyState('2') & 0x8000)
	{
		fTestVolum += 0.01f;
	}
	fTestVolum = CLAMP(fTestVolum, 0.f, 1.f);

	if (CKeyManager::GetInstance()->Key_Down(KEY_F))
	{
		bLight = !bLight;

		if (bLight)
		{
			CLightMananger::GetInstance()->LightOn();

		}
		else
		{
			CLightMananger::GetInstance()->LightOff();

		}

	}


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
	AddPrototype(CWeaponHUD::Create());
	AddPrototype(CGemIcon::Create());
	AddPrototype(CDiscIcon::Create());
	AddPrototype(CMiniMapFrame::Create());
	AddPrototype(CCardinalPoint::Create());

	AddGameObject<CAmmoGauge>();
	AddGameObject<CAmmoFrame>();

	AddGameObject<CCrossHair>();

	AddGameObject<CWeaponHUD>();

	AddGameObject< CHeart>();
	AddGameObject< CHeart>();
	AddGameObject< CHeart>();
	AddGameObject< CHeart>();
	AddGameObject< CHeart>();
	AddGameObject< CHeart>();

	AddGameObject<CHeartManager>();

	AddGameObject<CGemIcon>();
	AddGameObject<CDiscIcon>();
	AddGameObject<CCardinalPoint>();
	AddGameObject<CMiniMapFrame>();
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
