#include "stdafx.h"
#include "..\Header\PlayerSpawn.h"


CPlayerSpawn::CPlayerSpawn()
{
}

CPlayerSpawn::CPlayerSpawn(const CPlayerSpawn & _rOther)
	:CGizmo(_rOther)
{
}

HRESULT CPlayerSpawn::InitializePrototype()
{
	return S_OK;
}

HRESULT CPlayerSpawn::Awake()
{
	CGizmo::Awake();
	return S_OK;
}

HRESULT CPlayerSpawn::Start()
{
	return S_OK;
}

UINT CPlayerSpawn::Update(const float _fDeltaTime)
{
	CGizmo::Update(_fDeltaTime);
	return 0;
	/*
	//CGameObject::GetTage();
	룸 트리거는 기즈모에 테그 값 넣어서 그걸로 룸위치 확인하는걸로 
	*/
}

UINT CPlayerSpawn::LateUpdate(const float _fDeltaTime)
{
	CGizmo::LateUpdate(_fDeltaTime);
	return 0;
}

HRESULT CPlayerSpawn::Render()
{
	CGizmo::Render();
	return S_OK;
}

CGameObject * CPlayerSpawn::Clone()
{
	return nullptr;
}

CPlayerSpawn * CPlayerSpawn::Create()
{
	return nullptr;
}

void CPlayerSpawn::Free()
{
	CGizmo::Free();
}

