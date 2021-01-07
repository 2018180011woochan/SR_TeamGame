#include "stdafx.h"
#include "..\Header\PiramidUnBrake.h"


CPiramidUnBrake::CPiramidUnBrake()
{
}

CPiramidUnBrake::CPiramidUnBrake(const CPiramidUnBrake & _rOther)
	:CObstacle(_rOther)
{
}

HRESULT CPiramidUnBrake::InitializePrototype()
{
	CObstacle::InitializePrototype();
	return S_OK;
}

HRESULT CPiramidUnBrake::Awake()
{
	CObstacle::Awake();
	m_bDead = false;
	return S_OK;
}

HRESULT CPiramidUnBrake::Start()
{
	CObstacle::Start();
	m_pMeshRenderer = (CMeshRenderer*)AddComponent<CMeshRenderer>();
	m_pMeshRenderer->SetMesh(TEXT("Pyramid_Level2"));
	m_eRenderID = ERenderID::NoAlpha;

	m_pTransform->Set_Scale(_vector(1, 1, 1));

	m_pCollider = (CCollider*)AddComponent<CCollider>();
	m_pCollider->SetMesh(TEXT("Pyramid_Level2"),BOUND::BOUNDTYPE::BOX);
	m_pCollider->m_bIsRigid = true;

	m_pTransform->UpdateTransform();
	return S_OK;
}

UINT CPiramidUnBrake::Update(const float _fDeltaTime)
{
	if (m_bDead)
		return OBJ_DEAD;

	return OBJ_NOENVET;
}

UINT CPiramidUnBrake::LateUpdate(const float _fDeltaTime)
{
	CObstacle::LateUpdate(_fDeltaTime);
	return OBJ_NOENVET;
}

HRESULT CPiramidUnBrake::Render()
{
	m_pTransform->UpdateWorld();
	m_pMeshRenderer->Render();
	return S_OK;
}

CGameObject * CPiramidUnBrake::Clone()
{
	CGameObject* pInstance = new CPiramidUnBrake(*this);
	return pInstance;
}

void CPiramidUnBrake::OnCollision(CGameObject * _pGameObject)
{
	//
}

CPiramidUnBrake * CPiramidUnBrake::Create()
{
	CPiramidUnBrake* pInstance = new CPiramidUnBrake();
	return pInstance;
}

void CPiramidUnBrake::Free()
{
	CObstacle::Free();
}
