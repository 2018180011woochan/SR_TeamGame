#include "stdafx.h"
#include "GreenBoyUpHand.h"
#include "VIBuffer_Rect.h"
#include "MeshRenderer.h"
#include "Transform.h"
#include "Player.h"
#include "Camera.h"
#include "Item.h"
#include "Blood.h"
#include "Flame.h"
#include "GreenBoyBody.h"
#include "FireBullet.h"
#include "BloodHand.h"

CGreenBoyUpHand::CGreenBoyUpHand()
	:m_pTexturePool(nullptr)
{
}

CGreenBoyUpHand::CGreenBoyUpHand(const CGreenBoyUpHand & other)
	: CMonster(other)
{
}

HRESULT CGreenBoyUpHand::InitializePrototype()
{
	if (FAILED(CMonster::InitializePrototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CGreenBoyUpHand::Awake()
{
	if (FAILED(CMonster::Awake()))
		return E_FAIL;

	m_pMeshRenderer = (CMeshRenderer*)AddComponent<CMeshRenderer>();
	m_pMeshRenderer->SetMesh(TEXT("Quad"));

	m_pTexturePool = CTexturePoolManager::GetInstance()->GetTexturePool(TEXT("GreenBoy"));
	SafeAddRef(m_pTexturePool);

	m_fJumpPower = 4.f;
	m_fJumpTime = 0.f;
	m_fMoveSpeed = 100.f;
	m_bJump = false;
	nIndex = 0;

	m_fJumpDeltaTime = 0.f;
	m_fJumpSpeed = 1.f;

	m_fWalkSpeed = 0.1f;
	m_fWalkDeltaTime = 0.f;

	m_fFireDeltaTime = 0.f;
	m_fFireSpeed = 0.3f;

	m_fYTest = 0.f; 

	m_bDead = false;
	m_bisAttack = false;
	m_bCrush = false;

	m_iHP = 2;

	m_bIsBossDead = false;

	m_eRenderID = ERenderID::Alpha;
	return S_OK;
}

HRESULT CGreenBoyUpHand::Start()
{
	CMonster::Start();
	m_pTransform->Set_Scale(_vector(7, 7, 7));
	// Test
	m_pMeshRenderer->SetTexture(0, m_pTexturePool->GetTexture(TEXT("Hand"))[0]);

	m_nTag = 30;
	m_pCollider = (CCollider*)AddComponent<CCollider>();
	m_pCollider->SetMesh(TEXT("Quad"),BOUND::BOUNDTYPE::SPHERE);
	m_pCollider->m_bIsRigid = true;
	//m_nTag = 0;
	return S_OK;
}

UINT CGreenBoyUpHand::Update(const float _fDeltaTime)
{
	CMsgManager::GetInstance()->Freeze(&_fDeltaTime);

	CMonster::Update(_fDeltaTime);
	if (m_bIsBossDead)
		return OBJ_DEAD;
	m_pPlayerTransform = (CTransform*)(FindGameObjectOfType<CPlayer>()->GetComponent<CTransform>());
	pBodyTransform = (CTransform*)(FindGameObjectOfType<CGreenBoyBody>()->GetComponent<CTransform>());

	// 나중에 head 다른 조각 찾으면??


	if (!m_bCrush)
	{
		m_pMeshRenderer->SetTexture(0, m_pTexturePool->GetTexture(TEXT("Hand"))[0]);
		if (!isCloseToPlayer())
		{
			if (FAILED(Movement(_fDeltaTime)))
				return 0;
		}
	}

	m_fJumpDeltaTime += _fDeltaTime;
	if (m_fJumpSpeed <= m_fJumpDeltaTime)
	{
		m_fJumpDeltaTime -= m_fJumpSpeed;

		if (false == m_bJump)
			m_bJump = true;

		m_fYTest = m_pTransform->Get_Position().y;

	}

	if (m_bisAttack)
	{
		m_fFireDeltaTime += _fDeltaTime;
		if (m_fFireSpeed <= m_fFireDeltaTime)
		{
			m_fFireDeltaTime -= m_fFireSpeed;
			BulletFire();
		}
	}


	if (m_bCrush)
		Crush(_fDeltaTime);

	if (!m_bCrush)
	{
		m_pTransform->Add_Position(_vector(0.f, 0.3f, 0.f));
		m_pTransform->Set_RotationZ(0.f);
		if (m_pTransform->Get_Position().y > 12.f)
			m_pTransform->Set_Position(_vector(m_pTransform->Get_Position().x, 12.f, m_pTransform->Get_Position().z));
	}

	m_pTransform->UpdateTransform();
	//Jumping(_fDeltaTime);
	return _uint();
}

UINT CGreenBoyUpHand::LateUpdate(const float _fDeltaTime)
{
	CMonster::LateUpdate(_fDeltaTime);
	return _uint();
}

HRESULT CGreenBoyUpHand::Render()
{
	/*엔진에서 호출하는 식으로*/
	if (FAILED(CMonster::Render()))
		return E_FAIL;

	m_pTransform->UpdateWorld();
	m_pMeshRenderer->Render();
	return S_OK;
}

void CGreenBoyUpHand::OnCollision(CGameObject * _pGameObject)
{
	if (L"PlayerBullet" == _pGameObject->GetName())
	{
		CBlood* pBlood = (CBlood*)AddGameObject<CBlood>();
		pBlood->SetPos(m_pTransform->Get_Position());
	}
}


HRESULT CGreenBoyUpHand::Movement(float fDeltaTime)
{
	_vector vDir;
	vDir = _vector(pBodyTransform->Get_Position().x, pBodyTransform->Get_Position().y, pBodyTransform->Get_Position().z - 6.f)
		- m_pTransform->Get_Position();
	//vDir.y = 0.f;
	D3DXVec3Normalize(&vDir, &vDir);

	m_pTransform->Add_Position(vDir * fDeltaTime * m_fMoveSpeed);

	return S_OK;
}

void CGreenBoyUpHand::Jumping(float fDeltaTime)
{
	float fY = 0.f;
	if (m_bJump)
	{
		fY = m_fYTest + (m_fJumpPower * m_fJumpTime - 9.8f * m_fJumpTime * m_fJumpTime * 0.5f);
		m_pTransform->Set_Position(_vector(m_pTransform->Get_Position().x,
			fY,
			m_pTransform->Get_Position().z));

		m_fJumpTime += 0.01f;

		if (fY < 12.f)
		{
			m_bJump = false;
			m_pTransform->Set_Position(_vector(m_pTransform->Get_Position().x,
				12.f,
				m_pTransform->Get_Position().z));
			m_fJumpTime = 0.f;
		}
	}
}

bool CGreenBoyUpHand::isCloseToPlayer()
{
	_vector vecPlayerPos = m_pPlayerTransform->Get_Position();
	float fDistance = 0.f;

	float fWidth = abs(vecPlayerPos.x - m_pTransform->Get_Position().x);
	float fHeight = abs(vecPlayerPos.z - m_pTransform->Get_Position().z);
	fDistance = sqrt((fWidth * fWidth) + (fHeight * fHeight));

	if (fDistance < 25.f)
		return true;

	return false;
}

void CGreenBoyUpHand::BulletFire()
{
	CFireBullet* pGameObject = (CFireBullet*)AddGameObject<CFireBullet>();
	pGameObject->SetTurretPos(m_pTransform->Get_Position());
}

void CGreenBoyUpHand::Crush(float fDeltaTime)
{
	m_fWalkDeltaTime += fDeltaTime;
	if (m_fWalkSpeed <= m_fWalkDeltaTime)
	{
		nIndex++;
		if (nIndex >= 3)
			nIndex = 3;
		m_fWalkDeltaTime -= m_fWalkSpeed;
	}
	m_pMeshRenderer->SetTexture(0, m_pTexturePool->GetTexture(TEXT("Hand"))[nIndex]);

	m_pTransform->Add_Position(_vector(0.f, -0.3f, 0.f));
	m_pTransform->Set_RotationZ(90.f);
	if (m_pTransform->Get_Position().y < 5.f)
	{
		m_pTransform->Set_Position(_vector(m_pTransform->Get_Position().x, 5.f, m_pTransform->Get_Position().z));

		m_fFireDeltaTime += fDeltaTime;
		if (m_fFireSpeed <= m_fFireDeltaTime)
		{
			m_fFireDeltaTime -= m_fFireSpeed;
			CBloodHand* pGameObject = (CBloodHand*)AddGameObject<CBloodHand>();
			pGameObject->SetTurretPos(m_pTransform->Get_Position(), m_pPlayerTransform->Get_Position());

			pGameObject = (CBloodHand*)AddGameObject<CBloodHand>();
			pGameObject->SetTurretPos(m_pTransform->Get_Position(), _vector(m_pPlayerTransform->Get_Position().x - 5.f, m_pPlayerTransform->Get_Position().y, m_pPlayerTransform->Get_Position().z));
			
			pGameObject = (CBloodHand*)AddGameObject<CBloodHand>();
			pGameObject->SetTurretPos(m_pTransform->Get_Position(), _vector(m_pPlayerTransform->Get_Position().x + 5.f, m_pPlayerTransform->Get_Position().y, m_pPlayerTransform->Get_Position().z));
		}

		
	}
}

void CGreenBoyUpHand::SetPos(const _vector _Pos)
{
	m_pTransform->Set_Position(_Pos);
}

void CGreenBoyUpHand::SetIsAttack(const bool _bAttack)
{
	m_bisAttack = _bAttack;
}

void CGreenBoyUpHand::SetIsCrush(const bool _bAttack)
{
	m_bCrush = _bAttack;
}

void CGreenBoyUpHand::SetBossDead(const bool _isBossDead)
{
	m_bIsBossDead = _isBossDead;
}

CGameObject * CGreenBoyUpHand::Clone()
{
	CGreenBoyUpHand* pClone = new CGreenBoyUpHand(*this);
	return pClone;
}

CGreenBoyUpHand * CGreenBoyUpHand::Create()
{
	CGreenBoyUpHand* pInstance = new CGreenBoyUpHand();
	return pInstance;
}

void CGreenBoyUpHand::Free()
{
	SafeRelease(m_pTexturePool);
	CGameObject::Free();
}
