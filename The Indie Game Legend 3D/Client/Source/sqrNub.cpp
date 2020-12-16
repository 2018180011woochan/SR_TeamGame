#include "stdafx.h"
#include "sqrNub.h"
#include "VIBuffer_Rect.h"
#include "MeshRenderer.h"
#include "Transform.h"
#include "Player.h"
#include "Camera.h"

CsqrNub::CsqrNub()
	:m_pTexturePool(nullptr), m_dwSwitch(GetTickCount()),
	m_fJumpingCnt(0.f),
	m_fJumpSpeed(15.f),
	m_fMaxJump(6.f),
	m_fMoveSpeed(6.f),
	m_isMaxJump(false),
	m_isJumping(false)
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

	m_eRenderID = ERenderID::Alpha;
	return S_OK;
}

HRESULT CsqrNub::Start()
{
	CMonster::Start();
	m_pTransform->Set_Scale(_vector(1, 1, 1));
	m_pTransform->Add_Position(_vector(-5.f, 3.f, 10.f));
	m_pMeshRenderer->SetTexture(0, m_pTexturePool->GetTexture(TEXT("Idle"))[0]);

	return S_OK;
}

UINT CsqrNub::Update(const float _fDeltaTime)
{
	CMonster::Update(_fDeltaTime);

	if (nIndex >= 2)
		nIndex = 0;
	m_pMeshRenderer->SetTexture(0, m_pTexturePool->GetTexture(TEXT("Idle"))[nIndex]);

	if (m_dwSwitch + 4000 > GetTickCount())
	{
		m_isJumping = true;
		m_dwSwitch = GetTickCount();
	}

	if (m_isJumping)
		Jumping(_fDeltaTime);

	m_isJumping = false;
	if (!m_isJumping)
	{
		if (m_pTransform->Get_Position().y > m_fMaxJump || m_pTransform->Get_Position().y < 3.f)
		{
			m_pTransform->Set_Position(_vector(m_pTransform->Get_Position().x, 3.f, m_pTransform->Get_Position().z));
		}
	}


	if (FAILED(Movement(_fDeltaTime)))
		return 0;

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
	/*엔진에서 호출하는 식으로*/
	if (FAILED(CMonster::Render()))
		return E_FAIL;

	m_pTransform->UpdateWorld();
	m_pMeshRenderer->Render();
	return S_OK;
}


HRESULT CsqrNub::Movement(float fDeltaTime)
{
	m_pPlayerTransform = (CTransform*)(FindGameObjectOfType<CPlayer>()->GetComponent<CTransform>());

	_vector vDir;
	vDir = m_pPlayerTransform->Get_Position() - m_pTransform->Get_Position();
	D3DXVec3Normalize(&vDir, &vDir);

	m_pTransform->Add_Position(vDir * fDeltaTime * m_fMoveSpeed);

	return S_OK;
}

void CsqrNub::Jumping(float fDeltaTime)
{
	_vector vDir = { 0.f, m_pTransform->Get_Position().y + 1, 0.f };
	D3DXVec3Normalize(&vDir, &vDir);
	 

	if (m_pTransform->Get_Position().y < 3.f)
	{
		++nIndex;
		m_isMaxJump = false;
		m_pTransform->Set_Position(_vector(m_pTransform->Get_Position().x, 3.f, m_pTransform->Get_Position().z));
	}
	if (m_pTransform->Get_Position().y >= 3.f &&
		m_pTransform->Get_Position().y <= m_fMaxJump)
	{
		if (m_pTransform->Get_Position().y >= m_fMaxJump - 0.1f)
			m_isMaxJump = true;

		if (m_isMaxJump)
			m_pTransform->Add_Position(vDir * -fDeltaTime * m_fJumpSpeed);
		else
			m_pTransform->Add_Position(vDir * fDeltaTime * m_fJumpSpeed);

	}
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
