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

	AddGameObject<CBattleShip>();
	AddGameObject<CBoost>();
	AddGameObject<CFinalCamera>();
	CGameObject* pSkyBox = AddGameObject<CSkyBox>();
	((CTransform*)pSkyBox->GetComponent<CTransform>())->Set_Rotation(D3DXVECTOR3(0.f, 60.f, 0.f));

	for (UINT i = 0; i < 1; ++i)
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

