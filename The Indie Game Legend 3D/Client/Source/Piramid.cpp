#include "stdafx.h"
#include "..\Header\Piramid.h"
#include "Item.h"

CPiramid::CPiramid()
{
}

CPiramid::CPiramid(const CPiramid & _rOther)
	:CObstacle(_rOther)
{
}

HRESULT CPiramid::InitializePrototype()
{
	return S_OK;
}

HRESULT CPiramid::Awake()
{
	CObstacle::Awake();
	m_bDead = false;
	return S_OK;
}

HRESULT CPiramid::Start()
{
	CObstacle::Start();
	m_pMeshRenderer = (CMeshRenderer*)AddComponent<CMeshRenderer>();
	m_pMeshRenderer->SetMesh(TEXT("Pyramid_Level1"));
	m_eRenderID = ERenderID::NoAlpha;

	m_pTransform->Set_Scale(_vector(1, 1, 1));


	m_pCollider = (CCollider*)AddComponent<CCollider>();
	m_pCollider->SetMesh(TEXT("Pyramid_Level1"));
	m_pCollider->m_bIsRigid = true;

	m_pTransform->UpdateTransform();
	return S_OK;
}

UINT CPiramid::Update(const float _fDeltaTime)
{
	if (m_bDead)
		return OBJ_DEAD;

	return OBJ_NOENVET;
}

UINT CPiramid::LateUpdate(const float _fDeltaTime)
{
	CObstacle::LateUpdate(_fDeltaTime);
	return OBJ_NOENVET;
}

HRESULT CPiramid::Render()
{
	m_pTransform->UpdateWorld();
	m_pMeshRenderer->Render();
	return S_OK;
}

CGameObject * CPiramid::Clone()
{
	CGameObject* pInstance = new CPiramid(*this);
	return pInstance;
}

void CPiramid::Free()
{
	CObstacle::Free();
}

void CPiramid::OnCollision(CGameObject * _pGameObject)
{
	if (L"PlayerBullet" == _pGameObject->GetName())
	{
		m_bDead = true;
	}
	if (m_bDead)
	{
		CItem* pHeart = (CItem*)AddGameObject<CItem>();
		pHeart->SetPos(_vector(m_pTransform->Get_Position().x, m_pTransform->Get_Position().y + 3.f, m_pTransform->Get_Position().z));
		pHeart->SetItemType(EItemID::sprBigCoin);

		CItem* psqrCoin = (CItem*)AddGameObject<CItem>();
		psqrCoin->SetPos(_vector(m_pTransform->Get_Position().x, m_pTransform->Get_Position().y + 3.f, m_pTransform->Get_Position().z));
		psqrCoin->SetItemType(EItemID::sprCoin);
	}
}

CPiramid * CPiramid::Create()
{
	CPiramid* pInstance = new CPiramid();
	return pInstance;
}


