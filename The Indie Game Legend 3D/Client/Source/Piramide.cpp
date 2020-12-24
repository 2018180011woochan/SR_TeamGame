#include "stdafx.h"
#include "..\Header\Piramide.h"


CPiramide::CPiramide()
{
}

CPiramide::CPiramide(const CPiramide & _rOther)
	:CObstacle(_rOther)
{
}

HRESULT CPiramide::InitializePrototype()
{
	return S_OK;
}

HRESULT CPiramide::Awake()
{
	CObstacle::Awake();
	return S_OK;
}

HRESULT CPiramide::Start()
{
	CObstacle::Start();
	m_pMeshRenderer = (CMeshRenderer*)AddComponent<CMeshRenderer>();
	m_pMeshRenderer->SetMesh(TEXT("TestWall"));
	m_eRenderID = ERenderID::Alpha;

	m_pTransform->Set_Scale(_vector(1, 1, 1));
	m_pTransform->Set_Position(_vector(0,12, 0));
	m_pTransform->UpdateTransform();
	m_pCollider = (CCollider*)AddComponent<CCollider>();
	m_pCollider->SetMesh(TEXT("TestWall"), BOUND::BOUNDTYPE::BOX);
	m_pCollider->m_bIsRigid = true;

	return S_OK;
}

UINT CPiramide::Update(const float _fDeltaTime)
{
	return OBJ_NOENVET;
}

UINT CPiramide::LateUpdate(const float _fDeltaTime)
{
	CObstacle::LateUpdate(_fDeltaTime);
	return OBJ_NOENVET;
}

HRESULT CPiramide::Render()
{
	m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransform->Get_WorldMatrix());
	m_pMeshRenderer->Render();
	return S_OK;
}

CGameObject * CPiramide::Clone()
{
	CGameObject* pInstance = new CPiramide(*this);
	return pInstance;
}

void CPiramide::Free()
{
	CObstacle::Free();
}

void CPiramide::OnCollision(CGameObject * _pGameObject)
{
	if (L"PlayerBullet" == _pGameObject->GetName())
	{

	}
}

CPiramide * CPiramide::Create()
{
	CPiramide* pInstance = new CPiramide();
	return pInstance;
}


