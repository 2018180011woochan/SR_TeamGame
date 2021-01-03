#include "stdafx.h"
#include "Cryder.h"
#include "VIBuffer_Rect.h"
#include "MeshRenderer.h"
#include "Transform.h"
#include "Player.h"
#include "Camera.h"
#include "Item.h"
#include "Blood.h"

CCryder::CCryder()
	:m_pTexturePool(nullptr)
{
}

CCryder::CCryder(const CCryder & other)
	: CMonster(other)
{
}

HRESULT CCryder::InitializePrototype()
{
	if (FAILED(CMonster::InitializePrototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCryder::Awake()
{
	if (FAILED(CMonster::Awake()))
		return E_FAIL;

	m_pMeshRenderer = (CMeshRenderer*)AddComponent<CMeshRenderer>();
	m_pMeshRenderer->SetMesh(TEXT("Quad"));

	m_pTexturePool = CTexturePoolManager::GetInstance()->GetTexturePool(TEXT("Cryder"));
	SafeAddRef(m_pTexturePool);

	m_fJumpPower = 15.f;
	m_fJumpTime = 0.f;	
	m_fMoveSpeed = 3.f;
	m_bJump = false;
	nIndex = 0;
	
	m_fJumpDeltaTime = 0.f;
	m_fJumpSpeed = 2.f;

	m_fWalkSpeed = 0.5f;
	m_fWalkDeltaTime = 0.f;
	m_fYTest = 0.f;

	m_iHP = 2;

	m_eRenderID = ERenderID::Alpha;
	return S_OK;
}

HRESULT CCryder::Start()
{
	CMonster::Start();
	m_pTransform->Set_Scale(_vector(3, 3, 3));
	m_pTransform->Add_Position(_vector(0.f, 2.f, 0.f));
	m_pMeshRenderer->SetTexture(0, m_pTexturePool->GetTexture(TEXT("Idle"))[0]);

	//Test
	m_pCollider = (CCollider*)AddComponent<CCollider>();
	m_pCollider->SetMesh(TEXT("Quad"),BOUND::BOUNDTYPE::SPHERE);
	return S_OK;
}

UINT CCryder::Update(const float _fDeltaTime)
{
	if (m_bDead)
		return OBJ_DEAD;

	CMonster::Update(_fDeltaTime);

	m_fWalkDeltaTime += _fDeltaTime;
	if (m_fWalkSpeed <= m_fWalkDeltaTime)
	{
		nIndex++;
		if (nIndex >= 4)
			nIndex = 0;
		m_fWalkDeltaTime -= m_fWalkSpeed;
	}
	m_pMeshRenderer->SetTexture(0, m_pTexturePool->GetTexture(TEXT("Idle"))[nIndex]);


	if (FAILED(Movement(_fDeltaTime)))
		return 0;

	if (isCloseToPlayer())
	{
		m_fJumpDeltaTime += _fDeltaTime;
		if (m_fJumpSpeed <= m_fJumpDeltaTime)
		{
			m_fJumpDeltaTime -= m_fJumpSpeed;

			if (false == m_bJump)
				m_bJump = true;

			m_fYTest = m_pTransform->Get_Position().y;

		}
	}
	Jumping(_fDeltaTime);
	m_pTransform->UpdateTransform();


	return _uint();
}

UINT CCryder::LateUpdate(const float _fDeltaTime)
{
	CMonster::LateUpdate(_fDeltaTime);
	return _uint();
}

HRESULT CCryder::Render()
{
	/*엔진에서 호출하는 식으로*/
	if (FAILED(CMonster::Render()))
		return E_FAIL;

	m_pTransform->UpdateWorld();
	m_pMeshRenderer->Render();
	return S_OK;
}

void CCryder::OnCollision(CGameObject * _pGameObject)
{
	if (L"PlayerBullet" == _pGameObject->GetName())
	{
		m_iHP--;
		CBlood* pBlood = (CBlood*)AddGameObject<CBlood>();
		pBlood->SetPos(m_pTransform->Get_Position());
	}
	if (m_iHP <= 0)
	{
		CItem* psqrCoin = (CItem*)AddGameObject<CItem>();
		psqrCoin->SetPos(_vector(m_pTransform->Get_Position().x, m_pTransform->Get_Position().y + 3.f, m_pTransform->Get_Position().z));
		psqrCoin->SetItemType(EItemID::sprCoin);
		m_bDead = true;
	}
}


HRESULT CCryder::Movement(float fDeltaTime)
{
	m_pPlayerTransform = (CTransform*)(FindGameObjectOfType<CPlayer>()->GetComponent<CTransform>());

	_vector vDir;
	vDir = m_pPlayerTransform->Get_Position() - m_pTransform->Get_Position();
	vDir.y = 0;
	D3DXVec3Normalize(&vDir, &vDir);

	m_pTransform->Add_Position(vDir * fDeltaTime * m_fMoveSpeed);

	return S_OK;
}

void CCryder::Jumping(float fDeltaTime)
{
	float fY = 0.f;
	if (m_bJump)
	{
		m_fMoveSpeed = 100.f;
		fY = m_fYTest + (m_fJumpPower * m_fJumpTime - 9.8f * m_fJumpTime * m_fJumpTime * 0.5f);
		m_pTransform->Set_Position(_vector(m_pTransform->Get_Position().x,
			fY,
			m_pTransform->Get_Position().z));

		m_fJumpTime += 0.1f;

		if (fY < 2.f)
		{
			m_bJump = false;
			m_pTransform->Set_Position(_vector(m_pTransform->Get_Position().x,
				2.f,
				m_pTransform->Get_Position().z));
			m_fJumpTime = 0.f;
			m_fMoveSpeed = 3.f;
		}
	}
}

bool CCryder::isCloseToPlayer()
{
	_vector vecPlayerPos = m_pPlayerTransform->Get_Position();
	float fDistance = 0.f;

	float fWidth = abs(vecPlayerPos.x - m_pTransform->Get_Position().x);
	float fHeight = abs(vecPlayerPos.z - m_pTransform->Get_Position().z);
	fDistance = sqrt((fWidth * fWidth) + (fHeight * fHeight));

	if (fDistance < 30.f)
		return true;

	return false;
}

CGameObject * CCryder::Clone()
{
	CCryder* pClone = new CCryder(*this);
	return pClone;
}

CCryder * CCryder::Create()
{
	CCryder* pInstance = new CCryder();
	return pInstance;
}

void CCryder::Free()
{
	SafeRelease(m_pTexturePool);
	CGameObject::Free();
}
