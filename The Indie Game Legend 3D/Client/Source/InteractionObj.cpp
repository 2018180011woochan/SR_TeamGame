#include "stdafx.h"
#include "..\Header\InteractionObj.h"


CInteractionObj::CInteractionObj()
{
}

CInteractionObj::CInteractionObj(const CInteractionObj & _rOther)
	:CGameObject(_rOther)
	, m_nAttachID(-1)
	, m_pMeshRenderer(nullptr)
	, m_pTexturePool(nullptr)
	, m_pCollider(nullptr)
{
}

HRESULT CInteractionObj::InitializePrototype()
{
	return S_OK;
}

HRESULT CInteractionObj::Awake()
{


	return S_OK;
}

HRESULT CInteractionObj::Start()
{
	return S_OK;
}

UINT CInteractionObj::Update(const float _fDeltaTime)
{
	return 0;
}

UINT CInteractionObj::LateUpdate(const float _fDeltaTime)
{
	CGameObject::LateUpdate(_fDeltaTime);
	return 0;
}

HRESULT CInteractionObj::Render()
{
	return S_OK;
}

void CInteractionObj::Free()
{
	CGameObject::Free();
	//for (auto& pObj : m_listObservers)
	//{

	//}
	m_listObservers.clear();
}

void CInteractionObj::AddObserver(CInteractionObj * _pObj)
{
	m_listObservers.emplace_back(_pObj);
}

void CInteractionObj::RemoveObserver(CInteractionObj * _pObj)
{
	m_listObservers.remove(_pObj);
}

void CInteractionObj::Notify()
{
	for (auto& pObj : m_listObservers)
	{
		pObj->OnNotify();
	}
}
