#include "stdafx.h"
#include "..\Header\Wall.h"


CWall::CWall()
	: m_pCollider(nullptr)
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
	m_sName = L"Wall";
	return S_OK;
}

HRESULT CWall::Awake()
{
	CGameObject::Awake();
	m_pTransform->Set_Scale(D3DXVECTOR3(1.f, 1.f, 1.f));
	m_pTransform->UpdateTransform();
	m_pCollider = (CCollider*)(AddComponent<CCollider>());
	m_pCollider->SetMesh(m_sMesh, BOUND::BOUNDTYPE::BOX);
	m_pCollider->m_bIsRigid = true;

	m_eRenderID = ERenderID::NoAlpha;
	return S_OK;
}

HRESULT CWall::Start()
{
	CGameObject::Start();
	return S_OK;
}

UINT CWall::Update(const float _fDeltaTime)
{
	return 0;
}

UINT CWall::LateUpdate(const float _fDeltaTime)
{
	CGameObject::LateUpdate(_fDeltaTime);
	return 0;
}

HRESULT CWall::Render()
{
	return S_OK;
}

void CWall::SetMesh(const TSTRING & _sMesh)
{
	m_sMesh = _sMesh;
}

void CWall::SetDirection(const TSTRING & _sDirection)
{
	if (TEXT("Left") == _sDirection)
		m_eDirection = Direction::Left;
	else if (TEXT("Right") == _sDirection)
		m_eDirection = Direction::Right;
	else if (TEXT("Top") == _sDirection)
		m_eDirection = Direction::Top;
	else if (TEXT("Bottom") == _sDirection)
		m_eDirection = Direction::Bottom;
}

