#include "stdafx.h"
#include "GreenBoyHead.h"
#include "VIBuffer_Rect.h"
#include "MeshRenderer.h"
#include "Transform.h"
#include "Player.h"
#include "Camera.h"
#include "Item.h"
#include "Blood.h"
#include "GreenBoyBody.h"


CGreenBoyHead::CGreenBoyHead()
	:m_pTexturePool(nullptr)
{
}

CGreenBoyHead::CGreenBoyHead(const CGreenBoyHead & other)
	: CMonster(other)
{
}

HRESULT CGreenBoyHead::InitializePrototype()
{
	if (FAILED(CMonster::InitializePrototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CGreenBoyHead::Awake()
{
	if (FAILED(CMonster::Awake()))
		return E_FAIL;

	m_pMeshRenderer = (CMeshRenderer*)AddComponent<CMeshRenderer>();
	m_pMeshRenderer->SetMesh(TEXT("Quad"));

	m_pTexturePool = CTexturePoolManager::GetInstance()->GetTexturePool(TEXT("GreenBoy"));
	SafeAddRef(m_pTexturePool);

	m_fJumpPower = 4.f;
	m_fJumpTime = 0.f;
	m_fMoveSpeed = 8.f;
	m_bJump = false;
	nIndex = 0;

	m_fJumpDeltaTime = 0.f;
	m_fJumpSpeed = 1.f;

	m_fWalkSpeed = 0.5f;
	m_fWalkDeltaTime = 0.f;
	m_fYTest = 0.f;

	m_bDead = false;
	nIndex = 0;



	m_iHP = 30;
	m_iMaxHP = m_iHP;

	m_bIsBossDead = false;

	m_eRenderID = ERenderID::Alpha;
	return S_OK;
}

HRESULT CGreenBoyHead::Start()
{
	CMonster::Start();
	m_pTransform->Set_Scale(_vector(5, 5, 5));
	// Test
	m_pTransform->Set_Position(_vector(-240.f, 20.f, -240.f));
	//m_pTransform->Set_Position(_vector(0, 20.f, 0.f));
	m_pMeshRenderer->SetTexture(0, m_pTexturePool->GetTexture(TEXT("GreenBoy_Head"))[0]);

	m_pCollider = (CCollider*)AddComponent<CCollider>();
	m_pCollider->SetMesh(TEXT("Quad"),BOUND::BOUNDTYPE::SPHERE);
	m_pCollider->m_bIsRigid = true;
	//m_nTag = 0;
	//Test


	return S_OK;
}

UINT CGreenBoyHead::Update(const float _fDeltaTime)
{


	if (m_bIsBossDead)
		return OBJ_DEAD;
	m_pPlayerTransform = (CTransform*)(FindGameObjectOfType<CPlayer>()->GetComponent<CTransform>());
	CMonster::Update(_fDeltaTime);

	// ���߿� head �ٸ� ���� ã����??
	m_fWalkDeltaTime += _fDeltaTime;
	if (m_fWalkSpeed <= m_fWalkDeltaTime)
	{
		//nIndex++;
		//if (nIndex >= 2)
		//	nIndex = 0;
		m_fWalkDeltaTime -= m_fWalkSpeed;
	}
	m_pMeshRenderer->SetTexture(0, m_pTexturePool->GetTexture(TEXT("GreenBoy_Head"))[nIndex]);

	if (!isCloseToPlayer())
	{
		if (FAILED(Movement(_fDeltaTime)))
			return 0;
	}

	m_fJumpDeltaTime += _fDeltaTime;
	if (m_fJumpSpeed <= m_fJumpDeltaTime)
	{
		m_fJumpDeltaTime -= m_fJumpSpeed;

		if (false == m_bJump)
			m_bJump = true;

		m_fYTest = m_pTransform->Get_Position().y;

	}
	m_pTransform->UpdateTransform();
	Jumping(_fDeltaTime);
	return _uint();
}

UINT CGreenBoyHead::LateUpdate(const float _fDeltaTime)
{
	CMonster::LateUpdate(_fDeltaTime);
	return _uint();
}

HRESULT CGreenBoyHead::Render()
{
	/*�������� ȣ���ϴ� ������*/
	if (FAILED(CMonster::Render()))
		return E_FAIL;

	m_pTransform->UpdateWorld();
	m_pMeshRenderer->Render();
	return S_OK;
}

void CGreenBoyHead::OnCollision(CGameObject * _pGameObject)
{
	if (L"PlayerBullet" == _pGameObject->GetName())
	{
		CBlood* pBlood = (CBlood*)AddGameObject<CBlood>();
		pBlood->SetPos(m_pTransform->Get_Position());
	}
}


HRESULT CGreenBoyHead::Movement(float fDeltaTime)
{
	_vector vDir;
	vDir = m_pPlayerTransform->Get_Position() - m_pTransform->Get_Position();
	vDir.y = 0.f;
	D3DXVec3Normalize(&vDir, &vDir);

	m_pTransform->Add_Position(vDir * fDeltaTime * m_fMoveSpeed);

	return S_OK;
}

void CGreenBoyHead::Jumping(float fDeltaTime)
{
	float fY = 0.f;
	if (m_bJump)
	{
		fY = m_fYTest + (m_fJumpPower * m_fJumpTime - 9.8f * m_fJumpTime * m_fJumpTime * 0.5f);
		m_pTransform->Set_Position(_vector(m_pTransform->Get_Position().x,
			fY,
			m_pTransform->Get_Position().z));

		m_fJumpTime += 0.01f;

		if (fY < 20.f)
		{
			m_bJump = false;
			m_pTransform->Set_Position(_vector(m_pTransform->Get_Position().x,
				20.f,
				m_pTransform->Get_Position().z));
			m_fJumpTime = 0.f;
		}
	}
}

bool CGreenBoyHead::isCloseToPlayer()
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

void CGreenBoyHead::SetEggPos(const _vector _EggPos)
{
	m_pTransform->Set_Position(_EggPos);
}

void CGreenBoyHead::SetBossDead(const bool _isBossDead)
{
	m_bIsBossDead = _isBossDead;
}

CGameObject * CGreenBoyHead::Clone()
{
	CGreenBoyHead* pClone = new CGreenBoyHead(*this);
	return pClone;
}

CGreenBoyHead * CGreenBoyHead::Create()
{
	CGreenBoyHead* pInstance = new CGreenBoyHead();
	return pInstance;
}

void CGreenBoyHead::Free()
{
	SafeRelease(m_pTexturePool);
	CGameObject::Free();
}
