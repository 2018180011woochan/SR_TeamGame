#include "stdafx.h"
#include "..\Header\ShootingMinigame.h"
#include "PlayerShip.h"

CShootingMinigame::CShootingMinigame()
	: CScene(GetTypeHashCode<CShootingMinigame>())
{
}

HRESULT CShootingMinigame::Awake()
{
	AddPrototype(CPlayerShip::Create());

	AddGameObject<CPlayerShip>();

	return S_OK;
}

HRESULT CShootingMinigame::Start()
{
	return S_OK;
}

UINT CShootingMinigame::Update(const float _fDeltaTime)
{
	return 0;
}

UINT CShootingMinigame::LateUpdate(const float _fDeltaTime)
{
	return 0;
}

CShootingMinigame * CShootingMinigame::Create()
{
	CShootingMinigame* pInstance = new CShootingMinigame;
	return pInstance;
}

void CShootingMinigame::Free()
{
}
