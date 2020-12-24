#include "stdafx.h"
#include "..\Header\Obstacle.h"


CObstacle::CObstacle()
{
}

CObstacle::CObstacle(const CObstacle & _rOther)
	:CGameObject(_rOther)
{
}

HRESULT CObstacle::InitializePrototype()
{
	return S_OK;
}

HRESULT CObstacle::Awake()
{
	CGameObject::Awake();

	m_sName = L"Obstacle";
	return S_OK;
}

HRESULT CObstacle::Start()
{
	CGameObject::Start();
	return S_OK;
}

UINT CObstacle::Update(const float _fDeltaTime)
{
	return OBJ_NOENVET;
}

UINT CObstacle::LateUpdate(const float _fDeltaTime)
{
	CGameObject::LateUpdate(_fDeltaTime);

	return OBJ_NOENVET;
}

HRESULT CObstacle::Render()
{
	return S_OK;
}

void CObstacle::Free()
{
	CGameObject::Free();
}
