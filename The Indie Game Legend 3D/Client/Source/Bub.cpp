#include "stdafx.h"
#include "Bub.h"
#include "VIBuffer_Rect.h"
#include "MeshRenderer.h"
#include "Transform.h"
#include "Player.h"
#include "Camera.h"

CBub::CBub()
	:m_pTexturePool(nullptr)
{
}

CBub::CBub(const CBub & other)
	: CMonster(other)
{
}

HRESULT CBub::InitializePrototype()
{
	if (FAILED(CMonster::InitializePrototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CBub::Awake()
{
	if (FAILED(CMonster::Awake()))
		return E_FAIL;

	m_pMeshRenderer = (CMeshRenderer*)AddComponent<CMeshRenderer>();
	m_pMeshRenderer->SetMesh(TEXT("Quad"));

	m_pTexturePool = CTexturePoolManager::GetInstance()->GetTexturePool(TEXT("Bub"));
	SafeAddRef(m_pTexturePool);
	m_eRenderID = ERenderID::Alpha;
	return S_OK;
}

HRESULT CBub::Start()
{
	CMonster::Start();
	m_pTransform->Set_Scale(_vector(5, 5, 5));
	m_pTransform->Add_Position(_vector(0, 3, 0));
	m_pMeshRenderer->SetTexture(0, m_pTexturePool->GetTexture(TEXT("Idle"))[0]);

	return S_OK;
}

UINT CBub::Update(const float _fDeltaTime)
{
	CMonster::Update(_fDeltaTime);

	if (m_pTransform->Get_Position().y > m_fMaxJump || m_pTransform->Get_Position().y < 3.f)
	{
		m_pTransform->Set_Position(_vector(m_pTransform->Get_Position().x, 3.f, m_pTransform->Get_Position().z));
	}

	m_fJumpingCnt++;

	if (nIndex >= 2)
		nIndex = 0;
	m_pMeshRenderer->SetTexture(0, m_pTexturePool->GetTexture(TEXT("Idle"))[nIndex]);

	if (m_fJumpingCnt / 100.f > 1.f)
	{
		//Jumping(_fDeltaTime);
		++nIndex;
		m_fJumpingCnt = 0.f;
	}

	if (FAILED(Movement(_fDeltaTime)))
		return 0;

	m_pTransform->UpdateTransform();


	return _uint();
}

UINT CBub::LateUpdate(const float _fDeltaTime)
{
	CMonster::LateUpdate(_fDeltaTime);
	return _uint();
}

HRESULT CBub::Render()
{
	/*엔진에서 호출하는 식으로*/
	if (FAILED(CMonster::Render()))
		return E_FAIL;

	m_pTransform->UpdateWorld();
	m_pMeshRenderer->Render();
	return S_OK;
}


HRESULT CBub::Movement(float fDeltaTime)
{
	m_pPlayerTransform = (CTransform*)(FindGameObjectOfType<CPlayer>()->GetComponent<CTransform>());

	_vector vDir;
	vDir = m_pPlayerTransform->Get_Position() - m_pTransform->Get_Position();
	D3DXVec3Normalize(&vDir, &vDir);

	m_pTransform->Add_Position(vDir * fDeltaTime * m_fMoveSpeed);

	return S_OK;
}

void CBub::Jumping(float fDeltaTime)
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

CGameObject * CBub::Clone()
{
	CBub* pClone = new CBub(*this);
	return pClone;
}

CBub * CBub::Create()
{
	CBub* pInstance = new CBub();
	return pInstance;
}

void CBub::Free()
{
	SafeRelease(m_pTexturePool);
	CGameObject::Free();
}
