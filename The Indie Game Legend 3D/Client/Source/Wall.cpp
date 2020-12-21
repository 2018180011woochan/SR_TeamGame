#include "stdafx.h"
#include "..\Header\Wall.h"


CWall::CWall()
{
}

CWall::CWall(const CWall & _rOther)
	: CGameObject(_rOther)
{
}

void CWall::Free()
{
	CGameObject::Free();
}

CWall * CWall::Create()
{
	CWall* pInstance = new CWall;
	if (FAILED(pInstance->InitializePrototype()))
	{
		SafeRelease(pInstance);
		return nullptr;
	}
	return pInstance;
}

CGameObject * CWall::Clone()
{
	CWall* pClone = new CWall(*this);
	return pClone;
}

HRESULT CWall::InitializePrototype()
{
	CGameObject::InitializePrototype();
	return S_OK;
}

HRESULT CWall::Awake()
{
	CGameObject::Awake();
	return S_OK;
}

HRESULT CWall::Start()
{
	CGameObject::Start();
	return S_OK;
}

UINT CWall::Update(const float _fDeltaTime)
{
	CGameObject::Update(_fDeltaTime);
	return 0;
}

UINT CWall::LateUpdate(const float _fDeltaTime)
{
	CGameObject::LateUpdate(_fDeltaTime);
	return 0;
}

HRESULT CWall::Render()
{
	CGameObject::Render();
	return S_OK;
}

