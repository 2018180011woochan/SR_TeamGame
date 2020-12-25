#include "stdafx.h"
#include "sqrNub.h"
#include "VIBuffer_Rect.h"
#include "MeshRenderer.h"
#include "Transform.h"
#include "Player.h"
#include "Camera.h"
#include "Item.h"
#include "Blood.h"

CsqrNub::CsqrNub()
	:m_pTexturePool(nullptr)
{
}

CsqrNub::CsqrNub(const CsqrNub & other)
	: CMonster(other)
{
}

HRESULT CsqrNub::InitializePrototype()
{
	if (FAILED(CMonster::InitializePrototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CsqrNub::Awake()
{
	if (FAILED(CMonster::Awake()))
		return E_FAIL;

	m_pMeshRenderer = (CMeshRenderer*)AddComponent<CMeshRenderer>();
	m_pMeshRenderer->SetMesh(TEXT("Quad"));

	m_pTexturePool = CTexturePoolManager::GetInstance()->GetTexturePool(TEXT("sprNub"));
	SafeAddRef(m_pTexturePool);

	m_fJumpPower = 15.f;
	m_fJumpTime = 0.f;
	m_fMoveSpeed = 8.f;
	m_bJump = false;
	nIndex = 0;

	m_fJumpDeltaTime = 0.f;
	m_fJumpSpeed = 1.f;

	m_fWalkSpeed = 0.5f;
	m_fWalkDeltaTime = 0.f;
	m_fYTest = 0.f;

	m_iHP = 3;

	nIndex = 0;


	m_eRenderID = ERenderID::Alpha;
	return S_OK;
}

HRESULT CsqrNub::Start()
{
	CMonster::Start();

	m_pTransform->Set_Scale(_vector(3, 3, 3));
	m_pTransform->Add_Position(_vector(0.f, 1.5f, 0.f));

	m_pMeshRenderer->SetTexture(0, m_pTexturePool->GetTexture(TEXT("Idle"))[0]);

	m_pCollider = (CCollider*)AddComponent<CCollider>();
	m_pCollider->SetMesh(TEXT("Sphere"), BOUND::SPHERE);
	m_pCollider->m_bIsRigid = true;

	return S_OK;
}

UINT CsqrNub::Update(const float _fDeltaTime)
{
	if (m_bDead)
		return OBJ_DEAD;

	CMonster::Update(_fDeltaTime);

	m_fWalkDeltaTime += _fDeltaTime;
	if (m_fWalkSpeed <= m_fWalkDeltaTime)
	{
		nIndex++;
		if (nIndex >= 2)
			nIndex = 0;
		m_fWalkDeltaTime -= m_fWalkSpeed;
	}
	m_pMeshRenderer->SetTexture(0, m_pTexturePool->GetTexture(TEXT("Idle"))[nIndex]);


	if (FAILED(Movement(_fDeltaTime)))
		return 0;


	m_fJumpDeltaTime += _fDeltaTime;
	if (m_fJumpSpeed <= m_fJumpDeltaTime)
	{
		m_fJumpDeltaTime -= m_fJumpSpeed;

		if (false == m_bJump)
			m_bJump = true;

		m_fYTest = m_pTransform->Get_Position().y;

	}

	Jumping(_fDeltaTime);
	m_pTransform->UpdateTransform();

	return _uint();
}

UINT CsqrNub::LateUpdate(const float _fDeltaTime)
{
	CMonster::LateUpdate(_fDeltaTime);
	return _uint();
}

HRESULT CsqrNub::Render()
{
	/*�������� ȣ���ϴ� ������*/
	if (FAILED(CMonster::Render()))
		return E_FAIL;

	m_pTransform->UpdateWorld();
	m_pMeshRenderer->Render();
	return S_OK;
}

void CsqrNub::OnCollision(CGameObject * _pGameObject)
{
	if (L"PlayerBullet" == _pGameObject->GetName())
	{
		m_iHP--;
		CBlood* pBlood = (CBlood*)AddGameObject<CBlood>();
		pBlood->SetPos(m_pTransform->Get_Position());
	}
	if (m_iHP <= 0)
	{
		CItem* pHeart = (CItem*)AddGameObject<CItem>();
		pHeart->SetPos(_vector(m_pTransform->Get_Position().x, m_pTransform->Get_Position().y + 3.f, m_pTransform->Get_Position().z));
		pHeart->SetItemType(EItemID::sprBigCoin);

		CItem* psqrCoin = (CItem*)AddGameObject<CItem>();
		psqrCoin->SetPos(_vector(m_pTransform->Get_Position().x, m_pTransform->Get_Position().y + 3.f, m_pTransform->Get_Position().z));
		psqrCoin->SetItemType(EItemID::sprCoin);
		m_bDead = true;
	}
}

HRESULT CsqrNub::Movement(float fDeltaTime)
{
	m_pPlayerTransform = (CTransform*)(FindGameObjectOfType<CPlayer>()->GetComponent<CTransform>());

	_vector vDir;
	vDir = m_pPlayerTransform->Get_Position() - m_pTransform->Get_Position();
	vDir.y = 0.f;
	D3DXVec3Normalize(&vDir, &vDir);

	m_pTransform->Add_Position(vDir * fDeltaTime * m_fMoveSpeed);

	return S_OK;
}

void CsqrNub::Jumping(float fDeltaTime)
{
	float fY = 0.f;
	if (m_bJump)
	{
		fY = m_fYTest + (m_fJumpPower * m_fJumpTime - 9.8f * m_fJumpTime * m_fJumpTime * 0.5f);
		m_pTransform->Set_Position(_vector(m_pTransform->Get_Position().x,
			fY,
			m_pTransform->Get_Position().z));

		m_fJumpTime += 0.05f;

		if (fY < 1.5f)
		{
			m_bJump = false;
			m_pTransform->Set_Position(_vector(m_pTransform->Get_Position().x,
				1.5f,
				m_pTransform->Get_Position().z));
			m_fJumpTime = 0.f;
		}
	}
}

void CsqrNub::SetEggPos(const _vector _EggPos)
{
	m_pTransform->Set_Position(_EggPos);
}

CGameObject * CsqrNub::Clone()
{
	CsqrNub* pClone = new CsqrNub(*this);
	return pClone;
}

CsqrNub * CsqrNub::Create()
{
	CsqrNub* pInstance = new CsqrNub();
	return pInstance;
}

void CsqrNub::Free()
{
	SafeRelease(m_pTexturePool);
	CGameObject::Free();
}
