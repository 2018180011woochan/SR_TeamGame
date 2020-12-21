#include "stdafx.h"
#include "RoboBird.h"
#include "VIBuffer_Rect.h"
#include "MeshRenderer.h"
#include "Transform.h"
#include "Player.h"
#include "Camera.h"

CRoboBird::CRoboBird()
	:m_pTexturePool(nullptr)
{
}

CRoboBird::CRoboBird(const CRoboBird & other)
	: CMonster(other)
{
}

HRESULT CRoboBird::InitializePrototype()
{
	if (FAILED(CMonster::InitializePrototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CRoboBird::Awake()
{
	if (FAILED(CMonster::Awake()))
		return E_FAIL;

	m_pMeshRenderer = (CMeshRenderer*)AddComponent<CMeshRenderer>();
	m_pMeshRenderer->SetMesh(TEXT("Quad"));

	m_pTexturePool = CTexturePoolManager::GetInstance()->GetTexturePool(TEXT("Robobird"));
	SafeAddRef(m_pTexturePool);

	m_fJumpPower = 10.f;
	m_fJumpTime = 0.f;
	m_fMoveSpeed = 15.f;
	m_bJump = false;
	nIndex = 0;

	m_fJumpDeltaTime = 0.f;
	m_fJumpSpeed = 2.f;

	m_fWalkSpeed = 0.1f;
	m_fWalkDeltaTime = 0.f;
	m_fYTest = 0.f;

	nIndex = 0;

	m_eRenderID = ERenderID::Alpha;
	return S_OK;
}

HRESULT CRoboBird::Start()
{
	CMonster::Start();
	m_pTransform->Set_Scale(_vector(3, 3, 3));
	// Test
	m_pTransform->Set_Position(_vector(0.f, 10.f, 0.f));
	m_pMeshRenderer->SetTexture(0, m_pTexturePool->GetTexture(TEXT("Idle"))[0]);

	return S_OK;
}

UINT CRoboBird::Update(const float _fDeltaTime)
{
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

	//Jumping(_fDeltaTime);
	m_pTransform->UpdateTransform();

	return _uint();
}

UINT CRoboBird::LateUpdate(const float _fDeltaTime)
{
	CMonster::LateUpdate(_fDeltaTime);
	return _uint();
}

HRESULT CRoboBird::Render()
{
	/*�������� ȣ���ϴ� ������*/
	if (FAILED(CMonster::Render()))
		return E_FAIL;

	m_pTransform->UpdateWorld();
	m_pMeshRenderer->Render();
	return S_OK;
}


HRESULT CRoboBird::Movement(float fDeltaTime)
{
	m_pPlayerTransform = (CTransform*)(FindGameObjectOfType<CPlayer>()->GetComponent<CTransform>());

	_vector vDir;
	vDir = m_pPlayerTransform->Get_Position() - m_pTransform->Get_Position();
	vDir.y = 0.f;
	D3DXVec3Normalize(&vDir, &vDir);

	m_pTransform->Add_Position(vDir * fDeltaTime * m_fMoveSpeed);

	return S_OK;
}

void CRoboBird::Jumping(float fDeltaTime)
{
	float fY = 0.f;
	if (m_bJump)
	{
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
		}
	}
}

void CRoboBird::SetEggPos(const _vector _EggPos)
{
	m_pTransform->Set_Position(_EggPos);
}

CGameObject * CRoboBird::Clone()
{
	CRoboBird* pClone = new CRoboBird(*this);
	return pClone;
}

CRoboBird * CRoboBird::Create()
{
	CRoboBird* pInstance = new CRoboBird();
	return pInstance;
}

void CRoboBird::Free()
{
	SafeRelease(m_pTexturePool);
	CGameObject::Free();
}