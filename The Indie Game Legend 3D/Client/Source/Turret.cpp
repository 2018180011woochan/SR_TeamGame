#include "stdafx.h"
#include "..\Header\Turret.h"
#include "MeshRenderer.h"
#include "Player.h"
#include "TurretBullet.h"

CTurret::CTurret()
	: m_pTexturePool(nullptr)
{
}

CTurret::CTurret(const CTurret & other)
	: CMonster(other)
{
}

HRESULT CTurret::InitializePrototype()
{
	if (FAILED(CMonster::InitializePrototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CTurret::Awake()
{
	if (FAILED(CMonster::Awake()))
		return E_FAIL;

	m_pMeshRenderer = (CMeshRenderer*)AddComponent<CMeshRenderer>();
	m_pMeshRenderer->SetMesh(TEXT("Quad"));

	m_pTexturePool = CTexturePoolManager::GetInstance()->GetTexturePool(TEXT("Turret"));
	SafeAddRef(m_pTexturePool);

	m_fFireDeltaTime = 0;
	m_fFireSpeed = 3;
	m_pTransform->Set_Scale(_vector(10, 10, 10));
	//m_pTransform->Set_Position(_vector(-5.f, 5.f, 10.f));
	m_eRenderID = ERenderID::Alpha;
	return S_OK;
}

HRESULT CTurret::Start()
{
	CMonster::Start();

	m_pMeshRenderer->SetTexture(0, m_pTexturePool->GetTexture(TEXT("Idle"))[0]);
	m_pCollider = (CCollider*)AddComponent<CCollider>();
	m_pCollider->SetMesh(TEXT("SkyBox"));
	m_pCollider->m_bIsRigid = true;
	m_nTag = 0;
	return S_OK;
}

UINT CTurret::Update(const float _fDeltaTime)
{
	CMonster::Update(_fDeltaTime);

	/*
	if (�ͷ��� �μ�����)
		m_pMeshRenderer->SetTexture(0, m_pTexturePool->GetTexture(TEXT("Idle"))[1]);
	*/

	if (m_pTransform->Get_Position().y > 5.f || m_pTransform->Get_Position().y < 3.f)
	{
		m_pTransform->Set_Position(_vector(m_pTransform->Get_Position().x, 3.f, m_pTransform->Get_Position().z));
	}

	//off Test
	/*m_fFireDeltaTime += _fDeltaTime;
	if (m_fFireSpeed <= m_fFireDeltaTime)
	{
		m_fFireDeltaTime -= m_fFireSpeed;
		BulletFire();
	}*/


	m_pTransform->UpdateTransform();

	return _uint();
}

UINT CTurret::LateUpdate(const float _fDeltaTime)
{
	CMonster::LateUpdate(_fDeltaTime);
	return _uint();
}

HRESULT CTurret::Render()
{
	if (FAILED(CMonster::Render()))
		return E_FAIL;

	m_pTransform->UpdateWorld();
	m_pMeshRenderer->Render();
	m_pCollider->Draw();
	return S_OK;
}

void CTurret::BulletFire()
{
	CTurretBullet* pGameObject = (CTurretBullet*)AddGameObject<CTurretBullet>();
	pGameObject->SetTurretPos(m_pTransform->Get_Position());
}


CGameObject * CTurret::Clone()
{
	CTurret* pClone = new CTurret(*this);
	return pClone;
}

CTurret * CTurret::Create()
{
	CTurret* pInstance = new CTurret();
	return pInstance;
}

void CTurret::Free()
{
	SafeRelease(m_pTexturePool);
	CGameObject::Free();
}
