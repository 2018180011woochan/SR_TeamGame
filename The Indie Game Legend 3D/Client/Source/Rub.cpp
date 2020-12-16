#include "stdafx.h"
#include "Rub.h"
#include "VIBuffer_Rect.h"
#include "MeshRenderer.h"
#include "Transform.h"
#include "Player.h"
#include "Camera.h"

CRub::CRub()
	:m_pTexturePool(nullptr)
{
}

CRub::CRub(const CRub & other)
	: CMonster(other)
{
}

HRESULT CRub::InitializePrototype()
{
	if (FAILED(CMonster::InitializePrototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CRub::Awake()
{
	if (FAILED(CMonster::Awake()))
		return E_FAIL;

	m_pMeshRenderer = (CMeshRenderer*)AddComponent<CMeshRenderer>();
	m_pMeshRenderer->SetMesh(TEXT("Quad"));

	m_pTexturePool = CTexturePoolManager::GetInstance()->GetTexturePool(TEXT("Rub"));
	SafeAddRef(m_pTexturePool);

	m_eRenderID = ERenderID::Alpha;
	return S_OK;
}

HRESULT CRub::Start()
{
	CMonster::Start();
	m_pTransform->Set_Scale(_vector(1, 1, 1));
	m_pTransform->Add_Position(_vector(5.f, 3.f, 10.f));
	m_pMeshRenderer->SetTexture(0, m_pTexturePool->GetTexture(TEXT("Idle"))[0]);

	return S_OK;
}

UINT CRub::Update(const float _fDeltaTime)
{
	CMonster::Update(_fDeltaTime);

	if (nIndex >= 2)
		nIndex = 0;
	m_pMeshRenderer->SetTexture(0, m_pTexturePool->GetTexture(TEXT("Idle"))[nIndex]);

	Jumping(_fDeltaTime);

	if (FAILED(Movement(_fDeltaTime)))
		return 0;

	m_pTransform->UpdateTransform();


	return _uint();
}

UINT CRub::LateUpdate(const float _fDeltaTime)
{
	CMonster::LateUpdate(_fDeltaTime);
	return _uint();
}

HRESULT CRub::Render()
{
	/*엔진에서 호출하는 식으로*/
	if (FAILED(CMonster::Render()))
		return E_FAIL;

	m_pTransform->UpdateWorld();
	m_pMeshRenderer->Render();
	return S_OK;
}


HRESULT CRub::Movement(float fDeltaTime)
{
	m_pPlayerTransform = (CTransform*)(FindGameObjectOfType<CPlayer>()->GetComponent<CTransform>());

	_vector vDir;
	vDir = m_pPlayerTransform->Get_Position() - m_pTransform->Get_Position();
	D3DXVec3Normalize(&vDir, &vDir);

	m_pTransform->Add_Position(vDir * fDeltaTime * m_fMoveSpeed);

	return S_OK;
}

void CRub::Jumping(float fDeltaTime)
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

CGameObject * CRub::Clone()
{
	CRub* pClone = new CRub(*this);
	return pClone;
}

CRub * CRub::Create()
{
	CRub* pInstance = new CRub();
	return pInstance;
}

void CRub::Free()
{
	SafeRelease(m_pTexturePool);
	CGameObject::Free();
}
