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
#include "BloodHand.h"
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
#include "Pillar.h"

#include "SoundMgr.h"

#include "RoomTrigger.h"
#include "LightMananger.h"
#include "Piramid.h"
#include "Item.h"
#include "BulletSpark.h"
#include "PlayerSpawn.h"
#include "PiramidUnBrake.h"

#include "KeyManager.h"
#include "SoundMgr.h"


#include "WeaponHUD.h"



#include "Gun.h"

#include "Focus.h"


#include "Loading.h"
#include "Switch.h"
#include "Door.h"
#include "Scene.h"
#include "Gate.h"

#include "Switch.h"
#include "Door.h"
#include "Scene.h"

#include "PuzzleRoomCamera.h"
#include "CameraManager.h"
CStage::CStage()
	: CScene(GetTypeHashCode<CStage>())
{
}

HRESULT CStage::Awake()
{
	m_pPlayer = nullptr;
	m_nPreRoomID = (_int)RoomID::End;
	m_nCurRoomID = (_int)RoomID::End;

#pragma region ProtoType
	AddPrototype(CBlood::Create());
	AddPrototype(CSmallExlode::Create());
	AddPrototype(CExplosion::Create());
	AddPrototype(CExplosionBlue::Create());
	AddPrototype(CSandBurst::Create());

	AddPrototype(CShopKeeper::Create());
	AddPrototype(CAmmo::Create());
	AddPrototype(CArmor::Create());
	AddPrototype(CBigShot::Create());

	AddPrototype(CPhubans::Create());
	AddPrototype(CCactus::Create());
	AddPrototype(CDerek::Create());
	AddPrototype(CEdragon::Create());

	AddPrototype(CItem::Create());
	//ok
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
	// 그린보이가 쏘는 총알
	AddPrototype(CBloodHand::Create());

	AddPrototype(CFireBullet::Create());
	AddPrototype(CIceBullet::Create());


	AddPrototype(CElectricTile::Create());
	AddPrototype(CSandTile::Create());
	AddPrototype(CLavaTile::Create());
	AddPrototype(CSwampTile::Create());

	AddPrototype(CPiramid::Create());
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
	AddPrototype(CPillar::Create());
	AddPrototype(CGate::Create());

	//interaction obj
	AddPrototype(CDoor::Create());
	AddPrototype(CSwitch::Create());
	AddPrototype(CPuzzleRoomCamera::Create());
#pragma endregion


#pragma endregion

#pragma region GUN_TEST
	AddPrototype(CGun::Create());
	AddGameObject<CGun>();
#pragma endregion

	//Sector
	CSector* pSector = (CSector*)AddGameObject<CSector>();

	AddLight();
	AddUIObject();

	m_pPlayer = AddGameObject<CPlayer>();
	//SafeAddRef(m_pPlayer);
	//AddGameObject<CPlayerCamera>();
	CCameraManager::GetInstance()->RegisteCamera(CCameraManager::ECameraID::Player, (CCamera*)AddGameObject<CPlayerCamera>());
	auto pTest = AddGameObject<CPuzzleRoomCamera>();
	CCameraManager::GetInstance()->RegisteCamera(CCameraManager::ECameraID::PuzzleRoom, (CCamera*)pTest);
	AddGameObject<CBulletSpawn>();
	AddGameObject<CMouse>();
	CCameraManager::GetInstance()->SetCurrentMainCamera(CCameraManager::Player);
#pragma region SKYBOX
	AddPrototype(CSkyBox::Create());
	AddGameObject<CSkyBox>();
#pragma endregion

#pragma region Collider
	AddPrototype(CWall::Create());
	AddPrototype(CFloor::Create());
#pragma endregion


	
#ifdef _DEBUG
	//CFactoryManager::GetInstance()->LoadDataFile(L"Test2");
	//CFactoryManager::GetInstance()->LoadScene(this);
#else
	CFactoryManager::GetInstance()->LoadDataFile(L"111");
	CFactoryManager::GetInstance()->LoadInterationObj(this, L"Sector1_InterObj");;
	CFactoryManager::GetInstance()->LoadScene(this);
	CFactoryManager::GetInstance()->LoadCollider(this, TEXT("Sector1_Collider"));
#endif // _DEBUG


	CScene::Awake();
	return S_OK;
}

HRESULT CStage::Start()
{

	AttachObj();

	CScene::Start();
	return S_OK;
}

UINT CStage::Update(float _fDeltaTime)
{
	CScene::Update(_fDeltaTime);

	CheckRoomEvent();


	return 0;
}

UINT CStage::LateUpdate(float _fDeltaTime)
{
	CScene::LateUpdate(_fDeltaTime);
	if (GetKeyState('Z'))
	{
		CCameraManager::GetInstance()->SetCurrentMainCamera(CCameraManager::PuzzleRoom);
	}
	else
	{
		CCameraManager::GetInstance()->SetCurrentMainCamera(CCameraManager::Player);
	}
	return 0;
}

HRESULT CStage::AddLight()
{
	//Light manager Test
	D3DXCOLOR color = D3DCOLOR_ARGB(255, 255, 255, 255);
	//Direction World Light
	CLightMananger::GetInstance()->CreateDirction(CLightMananger::World1,
		_vector(1, -1, 1), color*0.f, color* 0.6f, color*0.f);
	CLightMananger::GetInstance()->LightEnable(CLightMananger::World1, true);

	CLightMananger::GetInstance()->CreateDirction(CLightMananger::World2,
		_vector(-1, -1, 1), color*0.f, color* 0.6f, color*0.f);
	CLightMananger::GetInstance()->LightEnable(CLightMananger::World2, true);

	CLightMananger::GetInstance()->CreateDirction(CLightMananger::World3,
		_vector(-1, -1, -1), color*0.f, color* 0.6f, color*0.f);
	CLightMananger::GetInstance()->LightEnable(CLightMananger::World3, true);
	CLightMananger::GetInstance()->CreateDirction(CLightMananger::World4,
		_vector(1, -1, -1), color*0.f, color* 0.6f, color*0.f);
	CLightMananger::GetInstance()->LightEnable(CLightMananger::World4, true);

	CLightMananger::GetInstance()->CreateDirction(CLightMananger::World5,
		_vector(0, 0, 1), color*0.f, color* 0.3f, color*0.f);
	CLightMananger::GetInstance()->LightEnable(CLightMananger::World5, true);
	CLightMananger::GetInstance()->CreateDirction(CLightMananger::World6,
		_vector(1, 0, 0), color*0.f, color* 0.3f, color*0.f);
	CLightMananger::GetInstance()->LightEnable(CLightMananger::World6, true);
	CLightMananger::GetInstance()->CreateDirction(CLightMananger::World7,
		_vector(-1, 0, 0), color*0.f, color* 0.3f, color*0.f);
	CLightMananger::GetInstance()->LightEnable(CLightMananger::World7, true);
	CLightMananger::GetInstance()->CreateDirction(CLightMananger::World8,
		_vector(0, 0, -1), color*0.f, color* 0.3f, color*0.f);
	CLightMananger::GetInstance()->LightEnable(CLightMananger::World8, true);

	//Point World Light
	CLightMananger::GetInstance()->CreateSpotlight(CLightMananger::Player,
		_vector(0, 10, 0), _vector(0, -1, 0), color*0.f, color* 0.8f, color*0.f,D3DXToRadian(60.f), D3DXToRadian(90.f));
	CLightMananger::GetInstance()->LightEnable(CLightMananger::Player, false);
	CLightMananger::GetInstance()->LightOn();
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
	AddPrototype(CWorm::Create());
	AddPrototype(CWormBody::Create());
	AddPrototype(CWormConnector::Create());
	AddPrototype(CWormTail::Create());
	AddPrototype(CWormBullet::Create());
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

HRESULT CStage::AttachObj()
{
	list<CGameObject*> listDoor;
	list<CGameObject*> listSwitch;
	listDoor = CManagement::GetInstance()->FindGameObjectsOfType<CDoor>(m_nSceneID);
	listSwitch = CManagement::GetInstance()->FindGameObjectsOfType<CSwitch>(m_nSceneID);

	for (auto& pDoor : listDoor)
	{
		for (auto& pSwitch : listSwitch)
		{
			CInteractionObj* pDoorObj = (CInteractionObj*)pDoor;
			CInteractionObj* pSwitchObj = (CInteractionObj*)pSwitch;
			int a = pDoorObj->GetAttachID();
			int b = pSwitchObj->GetAttachID();
			if (pDoorObj->GetAttachID() == pSwitchObj->GetAttachID())
			{
				pSwitchObj->AddObserver(pDoorObj);
			}
		}
	}
	return S_OK;
}

void CStage::CheckRoomEvent()
{

	m_nCurRoomID = m_pPlayer->GetTag();

	if (m_nPreRoomID == m_nCurRoomID)
		return;

	//최소한으로 건들려고 이전방이 미로방이 아니였다가 미로방 진입시에만 
	if (m_nCurRoomID == (_int)RoomID::MazeRoom && m_nPreRoomID != (_int)RoomID::MazeRoom)
	{
		((CPlayer*)m_pPlayer)->SetSpotLightTrigget(true);
		CLightMananger::GetInstance()->WorldOff();
		CLightMananger::GetInstance()->LightEnable(CLightMananger::Player, true);

	}
	//이전방이 미로방이 였다가 미로방 나갈때만
	else if(m_nCurRoomID != (_int)RoomID::MazeRoom && m_nPreRoomID == (_int)RoomID::MazeRoom)
	{
		((CPlayer*)m_pPlayer)->SetSpotLightTrigget(false);
		CLightMananger::GetInstance()->WorldOn();
		CLightMananger::GetInstance()->LightEnable(CLightMananger::Player, false);
	}

	//TileSound & BGM
	switch ((RoomID)m_nCurRoomID)
	{
	case CStage::RoomID::ShopRoom:
		CSoundMgr::GetInstance()->PlayBGM(L"Shop.wav");
		break;
	case CStage::RoomID::MazeRoom:
		((CPlayer*)m_pPlayer)->SetsfxTileID(ETileID::Nomal);
		CSoundMgr::GetInstance()->PlayBGM(L"Sector3.wav");
		break;
	case CStage::RoomID::SandRoom:
		((CPlayer*)m_pPlayer)->SetsfxTileID(ETileID::Sand);
		CSoundMgr::GetInstance()->PlayBGM(L"Sector2.wav");
		break;
	case CStage::RoomID::SwampRoom:
		((CPlayer*)m_pPlayer)->SetsfxTileID(ETileID::Stone);
		CSoundMgr::GetInstance()->PlayBGM(L"Boss.wav");
		break;
	case CStage::RoomID::ForestRoom:
		((CPlayer*)m_pPlayer)->SetsfxTileID(ETileID::Grass);
		CSoundMgr::GetInstance()->PlayBGM(L"Sector4.wav");
		break;
	case CStage::RoomID::MetalRoom:
		((CPlayer*)m_pPlayer)->SetsfxTileID(ETileID::Metal);
		CSoundMgr::GetInstance()->PlayBGM(L"Sector5_loop.wav");
		break;
	default:
		((CPlayer*)m_pPlayer)->SetsfxTileID(ETileID::Nomal);
		CSoundMgr::GetInstance()->PlayBGM(L"Sector1.wav");
		break;
	}

	CSoundMgr::GetInstance()->SetVolume(CSoundMgr::BGM, 0.05f);
	m_nPreRoomID = m_nCurRoomID;
}

CStage * CStage::Create()
{
	CStage* pInstance = new CStage;
	return pInstance;
}

void CStage::Free()
{
	//SafeRelease(m_pPlayer);
	SafeRelease(m_pPlayer);
	CScene::Free();
}
