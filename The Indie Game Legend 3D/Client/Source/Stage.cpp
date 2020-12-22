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
#include "RoomTrigger.h"
#include "LightMananger.h"
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
	AddPrototype(CNubBoss::Create());
	AddPrototype(CRoboBird::Create());
	AddPrototype(CDoomBird::Create());
	AddPrototype(CTreeBoss::Create());
	AddPrototype(CRootAttack::Create());
	AddPrototype(CWormBoss::Create());
	AddPrototype(CWormBossBody::Create());

	AddPrototype(CSandTile::Create());
	AddPrototype(CElectricTile::Create());
	AddPrototype(CLavaTile::Create());
	AddPrototype(CSwampTile::Create());

	AddPrototype(CSlider::Create());
	AddPrototype(CRoomTrigger::Create());



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
	//AddGameObject<CSlider>();

	AddGameObject<CBub>();
	AddGameObject<CRub>();
	//AddGameObject<CsqrNub>();
	AddGameObject<CTurret>();
	//AddGameObject<CWalker>();
	//AddGameObject<CCryder>();
	//AddGameObject<CEgg>();
	//AddGameObject<CWalkerBoss>();
	AddGameObject<CNubBoss>();
	//AddGameObject<CRoboBird>();
	//AddGameObject<CDoomBird>();

	AddGameObject<CTreeBoss>();
	//AddGameObject<CWormBoss>();
	//AddGameObject<CWormBossBody>();

	//AddGameObject<CTreeBoss>();

	//컬링 테스트 
	CGameObject* pObj = AddGameObject<CRoomTrigger>();
	pObj->SetTag(0);
	((CTransform*)(pObj->GetComponent<CTransform>()))->Set_Position(_vector(30, 0, -30));
	pObj = AddGameObject<CRoomTrigger>();
	pObj->SetTag(1);
	((CTransform*)(pObj->GetComponent<CTransform>()))->Set_Position(_vector(-30, 0, -30));


	//Light manager Test
	D3DXCOLOR color = D3DCOLOR_ARGB(255, 255, 255, 255);
	//CLightMananger::GetInstance()->CreateDirction(CLightMananger::World1, _vector(1, -1, 1), color * 0.5f,
	//	color, color *0.5f);
	//CLightMananger::GetInstance()->LightEnable(CLightMananger::World1, true);

	//CLightMananger::GetInstance()->CreateDirction(CLightMananger::World2, _vector(-1, -1, -1), color * 0.5f,
	//	color, color *0.5f);
	//CLightMananger::GetInstance()->LightEnable(CLightMananger::World2, true);


	//CLightMananger::GetInstance()->CreatePoint(CLightMananger::World3, _vector(0, 50, 0), color,
	//	color, color);
	//CLightMananger::GetInstance()->LightEnable(CLightMananger::World3, true);

	CLightMananger::GetInstance()->CreateSpotlight(CLightMananger::Player,
		_vector(0,40,0), _vector(0, -1, 0), color*1.f, color*0.7f, color*0.f, D3DXToRadian(15.f), D3DXToRadian(90.f));
	CLightMananger::GetInstance()->LightEnable(CLightMananger::Player, true);
	CLightMananger::GetInstance()->LightOn();



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
	static bool  bTestLight = true;
	if (GetAsyncKeyState('1') & 0x8000)
	{
		fTestVolum -= 0.01f;
	}
	if (GetAsyncKeyState('2') & 0x8000)
	{
		fTestVolum += 0.01f;
	}
	fTestVolum = CLAMP(fTestVolum, 0.f, 1.f);

	if (GetAsyncKeyState(VK_F5) & 0x8000)
	{
		bTestLight = !bTestLight;
		if (!bTestLight)
		{
			CLightMananger::GetInstance()->LightOff();
		}
		else
		{
			CLightMananger::GetInstance()->LightOn();

		}

		//CLightMananger::GetInstance()->LightEnable(CLightMananger::Player, bTestLight);

	}


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
