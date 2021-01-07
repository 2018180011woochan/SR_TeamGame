#include "stdafx.h"
#include "..\Header\FinalStage.h"

#include "BattleShip.h"
#include "Boost.h"
#include "BattleShipBullet.h"

#include "FinalCamera.h"
#include "SkyBox.h"

#include "StarParticle.h"
#include "StarPool.h"

#include "FinalBoss.h"
#include "FinalNormal.h"
#include "FinalLaser.h"
#include "FinalExplosion.h"

#include "BossHPFrame.h"
#include "BossText.h"
#include "BossHPBar.h"
#include "BossHP.h"

#include "HeartManager.h"
#include "Heart.h"

#include "Explosion.h"
CFinalStage::CFinalStage()
	: CScene(GetTypeHashCode<CFinalStage>())
{
}

void CFinalStage::Free()
{
	CScene::Free();
}

CFinalStage * CFinalStage::Create()
{
	CFinalStage* pInstance = new CFinalStage;
	return pInstance;
}

HRESULT CFinalStage::Awake()
{
	AddPrototype(CBattleShip::Create());
	AddPrototype(CBoost::Create());
	AddPrototype(CBattleShipBullet::Create());

	AddPrototype(CFinalCamera::Create());
	AddPrototype(CSkyBox::Create());
	AddPrototype(CStarParticle::Create());
	AddPrototype(CStarPool::Create());

	AddPrototype(CFinalBoss::Create());
	AddPrototype(CFinalNormal::Create());
	AddPrototype(CFinalLaser::Create());
	AddPrototype(CFinalExplosion::Create());

	AddPrototype(CBossHPFrame::Create());
	AddPrototype(CBossText::Create());
	AddPrototype(CBossHPBar::Create());
	AddPrototype(CBossHP::Create());

	AddPrototype(CHeart::Create());
	AddPrototype(CHeartManager::Create());

	AddPrototype(CExplosion::Create());

	AddGameObject<CBossHPFrame>()->SetEnable(true);
	AddGameObject<CBossText>()->SetEnable(true);
	AddGameObject<CBossHPBar>()->SetEnable(true);
	AddGameObject<CBossHP>()->SetEnable(true);

	AddGameObject< CHeart>();
	AddGameObject< CHeart>();
	AddGameObject< CHeart>();
	AddGameObject< CHeart>();
	AddGameObject< CHeart>();
	AddGameObject< CHeart>();

	AddGameObject<CHeartManager>();


	AddGameObject<CBattleShip>();
	AddGameObject<CBoost>();
	AddGameObject<CFinalCamera>();
	CGameObject* pSkyBox = AddGameObject<CSkyBox>();
	((CTransform*)pSkyBox->GetComponent<CTransform>())->Set_Rotation(D3DXVECTOR3(0.f, 60.f, 0.f));

	for (UINT i = 0; i < 50; ++i)
		AddGameObject<CStarParticle>();

	AddGameObject<CStarPool>();

	AddGameObject<CFinalBoss>();



	CScene::Awake();
	return S_OK;
}

HRESULT CFinalStage::Start()
{
	CScene::Start();
	return S_OK;
}

UINT CFinalStage::Update(const float _fDeltaTime)
{
	CScene::Update(_fDeltaTime);
	return 0;
}

UINT CFinalStage::LateUpdate(const float _fDeltaTime)
{
	CScene::LateUpdate(_fDeltaTime);
	return 0;
}

