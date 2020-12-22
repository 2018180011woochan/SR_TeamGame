#include "stdafx.h"
#include "..\Header\Piramide.h"
#include "Item.h"

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
	m_bDead = false;
	return S_OK;
}

HRESULT CPiramide::Start()
{
	CObstacle::Start();
	m_pMeshRenderer = (CMeshRenderer*)AddComponent<CMeshRenderer>();
	m_pMeshRenderer->SetMesh(TEXT("Pyramid_Level1"));
	m_eRenderID = ERenderID::NoAlpha;

	m_pTransform->Set_Scale(_vector(1, 1, 1));
	m_pTransform->Set_Position(_vector(40, 0, 0));

	m_pCollider = (CCollider*)AddComponent<CCollider>();
	m_pCollider->SetMesh(TEXT("SkyBox"));
	m_pCollider->m_bIsRigid = true;

	m_pTransform->UpdateTransform();
	return S_OK;
}

UINT CPiramide::Update(const float _fDeltaTime)
{
	if (m_bDead)
		return OBJ_DEAD;

	return OBJ_NOENVET;
}

UINT CPiramide::LateUpdate(const float _fDeltaTime)
{
	CObstacle::LateUpdate(_fDeltaTime);
	return OBJ_NOENVET;
}

HRESULT CPiramide::Render()
{
	m_pTransform->UpdateWorld();
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

CPiramide * CPiramide::Create()
{
	CPiramide* pInstance = new CPiramide();
	return pInstance;
}


