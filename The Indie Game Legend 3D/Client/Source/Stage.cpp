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
#include "GreenBoyHead.h"
#include "GreenBoyBody.h"
#include "Flame.h"
#include "GreenBoyLeftHand.h"
#include "GreenBoyRightHand.h"
#include "GreenBoyUpHand.h"
#include "GreenBoyDownHand.h"
#include "BloodHand.h"
#include "GreenBoyFace.h"
#pragma endregion

#pragma region SHOP
#include "ShopKeeper.h"
#include "Ammo.h"
#include "Armor.h"
#include "BigShot.h"
#include "SkillRunning.h"
#include "Price.h"
#include "ArmorPrice.h"
#include "SkillRunningPrice.h"
#pragma endregion

#pragma region CONTAINER
#include "ContainerBlack.h"
#include "ContainerBlue.h"
#include "ContainerNomal.h"
#include "ContainerRed.h"
#include "ContainerWhite.h"

#include "LazerGun.h"
#include "BigGun.h"
#include "SpreadGun.h"
#include "FireBat.h"
#include "LapidGun.h"
#pragma endregion


#pragma region EFFECT
#include "Blood.h"
#include "SmallExlode.h"
#include "Explosion.h"
#include "ExplosionBlue.h"
#include "SandBurst.h"
#pragma endregion

#pragma region NPC
#include "Phubans.h"
#include "Cactus.h"
#include "Derek.h"
#include "Edragon.h"
#pragma endregion

#include "FactoryManager.h"
#include "Player.h"
#include "PlayerCamera.h"
#include "Mouse.h"
#include "Sector.h"
#include "BulletSpawn.h"

//bullet
#include "NormalBullet.h"
#include "BigBullet.h"
#include "TripleBullet.h"
#include "FlameBullet.h"
#include "LaserBullet.h"
#include "FireBullet.h"
#include "IceBullet.h"
#include "PosionGas.h"

#pragma region INCLUDE_UI
#include "AmmoGauge.h"
#include "AmmoFrame.h"
#include "AmmoIcon.h"
#include "AmmoIconHighlight.h"
#include "SkillGauge.h"
#include "AmmoHUD.h"

#include "HeartManager.h"
#include "Heart.h"
#include "CrossHair.h"
#include "GemIcon.h"
#include "GemText.h"
#include "DiscIcon.h"
#include "DiscText.h"
#include "MiniMapFrame.h"
#include "CardinalPoint.h"
#include "MiniMapRenderer.h"
#include "MiniMapPartial.h"
#include "BossHPFrame.h"
#include "BossText.h"
#include "BossHPBar.h"
#include "BossHP.h"
#include "DialogFrame.h"
#include "DialogPortrait.h"
#include "DialogText.h"
#include "DialogTextArrow.h"
#include "DialogHUD.h"
#pragma endregion

#pragma region WORM
#include "Worm.h"
#include "WormBody.h"
#include "WormConnector.h"
#include "WormTail.h"
#include "WormBullet.h"
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

#include "Gun.h"

#include "Focus.h"


#include "Loading.h"
#include "ShootingMinigame.h"

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
	AddPrototype(CSandBurst::Create());

	AddPrototype(CShopKeeper::Create());
	AddPrototype(CAmmo::Create());
	AddPrototype(CArmor::Create());
	AddPrototype(CBigShot::Create());
	AddPrototype(CSkillRunning::Create());
	AddPrototype(CPrice::Create());
	AddPrototype(CArmorPrice::Create());
	AddPrototype(CSkillRunningPrice::Create());

	AddPrototype(CContainerBlack::Create());
	AddPrototype(CContainerBlue::Create());
	AddPrototype(CContainerNomal::Create());
	AddPrototype(CContainerRed::Create());
	AddPrototype(CContainerWhite::Create());

	AddPrototype(CLazerGun::Create());
	AddPrototype(CLapidGun::Create());
	AddPrototype(CSpreadGun::Create());
	AddPrototype(CFireBat::Create());
	AddPrototype(CBigGun::Create());

	AddPrototype(CPhubans::Create());
	AddPrototype(CCactus::Create());
	AddPrototype(CDerek::Create());
	AddPrototype(CEdragon::Create());

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
	AddPrototype(CGreenBoyHead::Create());
	AddPrototype(CGreenBoyBody::Create());
	AddPrototype(CFlame::Create());
	AddPrototype(CGreenBoyLeftHand::Create());
	AddPrototype(CGreenBoyRightHand::Create());
	AddPrototype(CGreenBoyUpHand::Create());
	AddPrototype(CGreenBoyDownHand::Create());
	AddPrototype(CGreenBoyFace::Create());
	// 그린보이가 쏘는 총알
	AddPrototype(CBloodHand::Create());
	AddPrototype(CFireBullet::Create());
	AddPrototype(CIceBullet::Create());
	AddPrototype(CPosionGas::Create());

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

	//bullet 
	AddPrototype(CNormalBullet::Create());
	AddPrototype(CBigBullet::Create());
	AddPrototype(CTripleBullet::Create());
	AddPrototype(CFlameBullet::Create());
	AddPrototype(CLaserBullet::Create());

	AddPrototype(CBulletSpawn::Create());

#pragma region GUN_TEST
	AddPrototype(CGun::Create());
	AddGameObject<CGun>();
#pragma endregion

	AddGameObject<CPlayer>();
	AddGameObject<CPlayerCamera>();
	AddGameObject<CBulletSpawn>();
	AddGameObject<CMouse>();

	//AddGameObject<CPiramidUnBrake>();
	//AddGameObject<CPiramid>();



	// Test용으로 추가함
	//AddGameObject<CSlider>();


	//CGameObject* pObj =  AddGameObject<CBub>();
	//((CTransform*)pObj->GetComponent<CTransform>())->Set_Position(_vector(0, 0, 9));
	//pObj = AddGameObject<CBub>();

	//CGameObject* pObj =  AddGameObject<CWormBoss>();
	//((CTransform*)pObj->GetComponent<CTransform>())->Set_Position(_vector(-20.f, 10.f, -20.f));

	// pObj = AddGameObject<CBub>();

	//((CTransform*)pObj->GetComponent<CTransform>())->Set_Position(_vector(-10, 0, 0));
	//pObj = AddGameObject<CBub>();
	//((CTransform*)pObj->GetComponent<CTransform>())->Set_Position(_vector(10, 0, 10));
	//pObj = AddGameObject<CBub>();
	//((CTransform*)pObj->GetComponent<CTransform>())->Set_Position(_vector(-20, 0, 10));
	//pObj = AddGameObject<CBub>();
	//((CTransform*)pObj->GetComponent<CTransform>())->Set_Position(_vector(15, 0, 25));
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


	//AddGameObject<CPhubans>();
	//AddGameObject<CCactus>();

	//AddGameObject<CDerek>();
	//AddGameObject<CEdragon>();
	//AddGameObject<CGreenBoyHead>();
	//AddGameObject<CGreenBoyFace>();
	//AddGameObject<CGreenBoyBody>();
	//AddGameObject<CGreenBoyLeftHand>();
	//AddGameObject<CGreenBoyRightHand>();
	//AddGameObject<CGreenBoyUpHand>();
	//AddGameObject<CGreenBoyDownHand>();
	//AddGameObject<CFlame>();
	//AddGameObject<CContainerBlack>();
	//AddGameObject<CContainerBlue>();
	//AddGameObject<CContainerNomal>();
	//AddGameObject<CContainerRed>();
	//AddGameObject<CContainerWhite>();

#pragma region SHOP
	AddGameObject<CShopKeeper>();
	AddGameObject<CAmmo>();
	AddGameObject<CArmor>();
	AddGameObject<CSkillRunning>();
	AddGameObject<CPrice>();
	AddGameObject<CArmorPrice>();
	AddGameObject<CSkillRunningPrice>();
#pragma endregion

	//컬링 테스트 
	
	//Light manager Test
	D3DXCOLOR color = D3DCOLOR_ARGB(255, 255, 255, 255);

	//Direction World Light
	CLightMananger::GetInstance()->CreateDirction(CLightMananger::World1,
		_vector(1, -0, 0), color*0.f, color* 0.9f, color*0.f);
	CLightMananger::GetInstance()->LightEnable(CLightMananger::World1, true);
	CLightMananger::GetInstance()->CreateDirction(CLightMananger::World2,
		_vector(-1, -0, 0), color*0.f, color* 0.9f, color*0.f);
	CLightMananger::GetInstance()->LightEnable(CLightMananger::World2, true);
	CLightMananger::GetInstance()->CreateDirction(CLightMananger::World3,
		_vector(0, -0, -1), color*0.f, color* 0.9f, color*0.f);
	CLightMananger::GetInstance()->LightEnable(CLightMananger::World3, true);
	CLightMananger::GetInstance()->CreateDirction(CLightMananger::World4,
		_vector(0, 0, 1), color*0.f, color* 0.9f, color*0.f);
	CLightMananger::GetInstance()->LightEnable(CLightMananger::World4, true);
	CLightMananger::GetInstance()->CreateDirction(CLightMananger::World5,
		_vector(0, -1, 0), color*0.f, color* 0.9f, color*0.f);
	CLightMananger::GetInstance()->LightEnable(CLightMananger::World5, true);

	//Point World Light
	//CLightMananger::GetInstance()->CreatePoint(CLightMananger::Player,
	//	_vector(0, 10, 0), color*0.f, color* 0.8f, color*0.f);
	//CLightMananger::GetInstance()->LightEnable(CLightMananger::Player, true);


	CLightMananger::GetInstance()->LightOn();
	

	//Sector
	CSector* pSector = (CSector*)AddGameObject<CSector>();
	pSector->SetSectorName(L"Sector1");

	
	AddUIObject();
#pragma region SKYBOX
	AddPrototype(CSkyBox::Create());
	//AddGameObject<CSkyBox>();
#pragma endregion

#pragma region Collider
	AddPrototype(CWall::Create());
	AddPrototype(CFloor::Create());
#pragma endregion
	AddGameObject<CFloor>();


	CSoundMgr::GetInstance()->PlayBGM(L"Sector1.wav");
	CSoundMgr::GetInstance()->StopAll();


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

	// Test용으로 return누르면 미니게임 전환
	//if (GetAsyncKeyState(VK_RETURN) && 0x8000)
	//{
	//	//CManagement::GetInstance()->SetUpCurrentScene(CShootingMinigame::Create());
	//}

	//Test
	static float fTestVolum = 1.f;
	static bool bLight = false;
	if (GetAsyncKeyState('1') & 0x8000)
	{
		fTestVolum -= 0.01f;
	}
	if (GetAsyncKeyState('2') & 0x8000)
	{
		//fTestVolum += 0.01f;
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
#pragma region PROTOTYPE

	AddPrototype(CAmmoGauge::Create());
	AddPrototype(CAmmoFrame::Create());
	AddPrototype(CAmmoIcon::Create());
	AddPrototype(CAmmoIconHighlight::Create());
	AddPrototype(CSkillGauge::Create());
	AddPrototype(CAmmoHUD::Create());

	AddPrototype(CWeaponHUD::Create());

	AddPrototype(CHeart::Create());
	AddPrototype(CHeartManager::Create());

	AddPrototype(CCrossHair::Create());

	AddPrototype(CGemIcon::Create());
	AddPrototype(CGemText::Create());

	AddPrototype(CDiscIcon::Create());
	AddPrototype(CDiscText::Create());

	AddPrototype(CMiniMapFrame::Create());
	AddPrototype(CMiniMapRenderer::Create());
	AddPrototype(CMiniMapPartial::Create());
	AddPrototype(CCardinalPoint::Create());

	AddPrototype(CBossHPFrame::Create());
	AddPrototype(CBossText::Create());
	AddPrototype(CBossHPBar::Create());
	AddPrototype(CBossHP::Create());

	AddPrototype(CDialogFrame::Create());
	AddPrototype(CDialogPortrait::Create());
	AddPrototype(CDialogText::Create());
	AddPrototype(CDialogTextArrow::Create());
	AddPrototype(CDialogHUD::Create());

	AddPrototype(CFocus::Create());


#pragma endregion

#pragma region GAMEOBJECT

	AddGameObject<CAmmoGauge>();
	AddGameObject<CAmmoFrame>();
	AddGameObject<CAmmoIcon>();
	AddGameObject<CSkillGauge>();
	AddGameObject<CAmmoHUD>();
	AddGameObject<CWeaponHUD>();
	AddGameObject<CAmmoIconHighlight>();

	AddGameObject<CCrossHair>();


	AddGameObject< CHeart>();
	AddGameObject< CHeart>();
	AddGameObject< CHeart>();
	AddGameObject< CHeart>();
	AddGameObject< CHeart>();
	AddGameObject< CHeart>();

	AddGameObject<CHeartManager>();

	AddGameObject<CGemIcon>();
	AddGameObject<CGemText>();
	AddGameObject<CDiscIcon>();
	AddGameObject<CDiscText>();

	AddGameObject<CCardinalPoint>();
	AddGameObject<CMiniMapRenderer>();
	AddGameObject<CMiniMapPartial>();
	AddGameObject<CMiniMapFrame>();

	AddGameObject<CBossHPFrame>()->SetEnable(false);
	AddGameObject<CBossText>()->SetEnable(false);
	AddGameObject<CBossHPBar>()->SetEnable(false);
	AddGameObject<CBossHP>()->SetEnable(false);

	AddGameObject<CDialogFrame>()->SetEnable(false);
	AddGameObject<CDialogPortrait>()->SetEnable(false);
	AddGameObject<CDialogText>()->SetEnable(false);
	AddGameObject<CDialogTextArrow>()->SetEnable(false);
	AddGameObject<CDialogHUD>()->SetEnable(true);

	AddGameObject<CFocus>()->SetEnable(false);

#pragma endregion
	//AddPrototype(CWorm::Create());
	//AddPrototype(CWormBody::Create());
	//AddPrototype(CWormConnector::Create());
	//AddPrototype(CWormTail::Create());
	//AddPrototype(CWormBullet::Create());
	//AddGameObject<CWormConnector>();
	//AddGameObject<CWormConnector>();
	//AddGameObject<CWormConnector>();
	//AddGameObject<CWormConnector>();
	//AddGameObject<CWormConnector>();

	//AddGameObject<CWorm>()->SetEnable(true);
	//AddGameObject<CWormBody>();
	//AddGameObject<CWormBody>();
	//AddGameObject<CWormBody>();
	//AddGameObject<CWormBody>();

	//AddGameObject<CWormTail>();
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
