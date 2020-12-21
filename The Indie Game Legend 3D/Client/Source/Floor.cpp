#include "stdafx.h"
#include "..\Header\Floor.h"


CFloor::CFloor()
{
}

CFloor::CFloor(const CFloor & _rOther)
	: CGameObject(_rOther)
{
}

void CFloor::Free()
{
	CGameObject::Free();
}

CFloor * CFloor::Create()
{
	CFloor* pInstance = new CFloor;
	if (FAILED(pInstance->InitializePrototype()))
	{
		SafeRelease(pInstance);
		return nullptr;
	}
	return pInstance;
}

CGameObject * CFloor::Clone()
{
	CFloor* pClone = new CFloor(*this);
	return pClone;
}

HRESULT CFloor::InitializePrototype()
{
	CGameObject::InitializePrototype();
	return S_OK;
}

HRESULT CFloor::Awake()
{
	return E_NOTIMPL;
}

HRESULT CFloor::Start()
{
	return E_NOTIMPL;
}

UINT CFloor::Update(const float _fDeltaTime)
{
	return 0;
}

UINT CFloor::LateUpdate(const float _fDeltaTime)
{
	return 0;
}

HRESULT CFloor::Render()
{
	return E_NOTIMPL;
}


