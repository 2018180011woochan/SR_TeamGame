#include "stdafx.h"
#include "..\Header\Trigger.h"


CTrigger::CTrigger()
{
}

CTrigger::CTrigger(const CTrigger & _rOther)
	:CGameObject(_rOther)
{
}

HRESULT CTrigger::InitializePrototype()
{
	return S_OK;
}

HRESULT CTrigger::Awake()
{
	return S_OK;
}

HRESULT CTrigger::Start()
{
	return S_OK;
}

UINT CTrigger::Update(const float _fDeltaTime)
{
	return 0;
}

UINT CTrigger::LateUpdate(const float _fDeltaTime)
{
	return 0;
}

HRESULT CTrigger::Render()
{
	return S_OK;
}



void CTrigger::Free()
{
	CGameObject::Free();
}
