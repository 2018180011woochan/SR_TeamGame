#include "stdafx.h"
#include "..\Header\Egg.h"
#include "Bub.h"
#include "sqrNub.h"
#include "Rub.h"
#include "Item.h"
#include "Blood.h"

CEgg::CEgg()
	: m_pTexturePool(nullptr)
{
}

CEgg::CEgg(const CEgg & other)
	: CMonster(other)
{
}


HRESULT CEgg::InitializePrototype()
{
	if (FAILED(CMonster::InitializePrototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CEgg::Awake()
{
	if (FAILED(CMonster::Awake()))
		return E_FAIL;

	m_pMeshRenderer = (CMeshRenderer*)AddComponent<CMeshRenderer>();
	m_pMeshRenderer->SetMesh(TEXT("Quad"));

	m_pTexturePool = CTexturePoolManager::GetInstance()->GetTexturePool(TEXT("Egg"));
	SafeAddRef(m_pTexturePool);

	m_fBreathDeltaTime = 0;
	m_fBreathSpeed = 1;
	m_pTransform->Set_Scale(_vector(15, 15, 15));

	m_iHP = 10;

	m_eRenderID = ERenderID::Alpha;
	return S_OK;
}

HRESULT CEgg::Start()
{
	CMonster::Start();

	m_pMeshRenderer->SetTexture(0, m_pTexturePool->GetTexture(TEXT("Idle"))[0]);

	m_pCollider = (CCollider*)AddComponent<CCollider>();
	m_pCollider->SetMesh(TEXT("Quad"),BOUND::BOUNDTYPE::SPHERE);
	m_pCollider->m_bIsRigid = true;
	m_nTag = 0;

	return S_OK;
}

UINT CEgg::Update(const float _fDeltaTime)
{
	if (m_bDead)
		return OBJ_DEAD;

	CMonster::Update(_fDeltaTime);

	if (m_pTransform->Get_Position().y > 5.f || m_pTransform->Get_Position().y < 3.f)
	{
		m_pTransform->Set_Position(_vector(m_pTransform->Get_Position().x, 7.f, m_pTransform->Get_Position().z));
	}

	m_fBreathDeltaTime += _fDeltaTime;
	if (m_fBreathSpeed <= m_fBreathDeltaTime)
	{
		//nIndex++;
		if (nIndex >= 4)
			nIndex = 0;
		m_fBreathDeltaTime -= m_fBreathSpeed;
	}
	m_pMeshRenderer->SetTexture(0, m_pTexturePool->GetTexture(TEXT("Idle"))[nIndex]);

	m_pTransform->UpdateTransform();

	return OBJ_NOENVET;
}

UINT CEgg::LateUpdate(const float _fDeltaTime)
{
	CMonster::LateUpdate(_fDeltaTime);
	return _uint();
}

HRESULT CEgg::Render()
{
	if (FAILED(CMonster::Render()))
		return E_FAIL;

	m_pTransform->UpdateWorld();
	m_pMeshRenderer->Render();
	return S_OK;
}

void CEgg::OnCollision(CGameObject * _pGameObject)
{

	if (m_bHit == false && (L"PlayerBullet" == _pGameObject->GetName() || L"ExplosionBlue" == _pGameObject->GetName()))
	{
		m_bHit = true;
		m_iHP--;
		CBlood* pBlood = (CBlood*)AddGameObject<CBlood>();
		pBlood->SetPos(m_pTransform->Get_Position());
	}
	if (m_iHP <= 0)
	{
		EggDrop();
		CSoundMgr::GetInstance()->Play(L"sfxKill.wav", CSoundMgr::MonsterKill);

		m_bDead = true;
	}
}

void CEgg::EggDrop()
{
	CBub* pBubObject = (CBub*)AddGameObject<CBub>();
	pBubObject->SetEggPos(m_pTransform->Get_Position());

	CsqrNub* pNubObject = (CsqrNub*)AddGameObject<CsqrNub>();
	pNubObject->SetEggPos(m_pTransform->Get_Position());

	CRub* pRubObject = (CRub*)AddGameObject<CRub>();
	pRubObject->SetEggPos(m_pTransform->Get_Position());
}

CGameObject * CEgg::Clone()
{
	CEgg* pClone = new CEgg(*this);
	return pClone;
}

CEgg * CEgg::Create()
{
	CEgg* pInstance = new CEgg();
	return pInstance;
}

void CEgg::Free()
{
	SafeRelease(m_pTexturePool);
	CGameObject::Free();
}
