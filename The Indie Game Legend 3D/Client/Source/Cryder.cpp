#include "stdafx.h"
#include "Cryder.h"
#include "VIBuffer_Rect.h"
#include "MeshRenderer.h"
#include "Transform.h"
#include "Player.h"
#include "Camera.h"

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

	m_fJumpPower = 50.f;
	m_fJumpTime = 0.f;	
	m_fMoveSpeed = 3.f;
	m_isMaxJump = false;
	m_isJumping = false;

	nIndex = 0;

	m_fWalkSpeed = 0.5f;
	m_fWalkDeltaTime = 0.f;
	m_fYTest = 1.f;

	m_eRenderID = ERenderID::Alpha;
	return S_OK;
}

HRESULT CCryder::Start()
{
	CMonster::Start();
	m_pTransform->Set_Scale(_vector(1, 1, 1));
	// Test
	m_pTransform->Add_Position(_vector(5.f, 2.f, 10.f));
	m_pMeshRenderer->SetTexture(0, m_pTexturePool->GetTexture(TEXT("Idle"))[0]);

	return S_OK;
}

UINT CCryder::Update(const float _fDeltaTime)
{
	CMonster::Update(_fDeltaTime);

	m_fWalkDeltaTime += _fDeltaTime;
	if (m_fWalkSpeed <= m_fWalkDeltaTime)
	{
		if (nIndex >= 3)
			nIndex = 0;
		nIndex++;
		m_fWalkDeltaTime -= m_fWalkSpeed;
	}
	m_pMeshRenderer->SetTexture(0, m_pTexturePool->GetTexture(TEXT("Idle"))[nIndex]);

	if (FAILED(Movement(_fDeltaTime)))
		return 0;

	//Jumping(_fDeltaTime);

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


HRESULT CCryder::Movement(float fDeltaTime)
{
	m_pPlayerTransform = (CTransform*)(FindGameObjectOfType<CPlayer>()->GetComponent<CTransform>());

	_vector vDir;
	vDir = m_pPlayerTransform->Get_Position() - m_pTransform->Get_Position();
	D3DXVec3Normalize(&vDir, &vDir);

	m_pTransform->Add_Position(vDir * fDeltaTime * m_fMoveSpeed);

	return S_OK;
}

void CCryder::Jumping(float fDeltaTime)
{
	float fY = m_fYTest - (m_fJumpPower * m_fJumpTime - 9.8f * m_fJumpTime * m_fJumpTime * 0.5f);
	m_pTransform->Set_Position(_vector(m_pTransform->Get_Position().x,
		fY,
		m_pTransform->Get_Position().z));

	if (m_pTransform->Get_Position().y < 1.f)
		m_pTransform->Set_Position(_vector(m_pTransform->Get_Position().x,
			1.f,
			m_pTransform->Get_Position().z));

	m_fJumpTime += 0.2f;
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
