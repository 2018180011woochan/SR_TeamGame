#include "stdafx.h"
#include "..\Header\WormBody.h"
#include "TexturePoolManager.h"
#include "Player.h"

CWormBody::CWormBody()
{
}

CWormBody::CWormBody(const CWormBody & _rOther)
	: CWormPart(_rOther)
{
}

void CWormBody::Free()
{
	CWormPart::Free();
	SafeRelease(m_pTexturePool);
}

CWormBody * CWormBody::Create()
{
	CWormBody* pInstance = new CWormBody;
	if (FAILED(pInstance->InitializePrototype()))
	{
		SafeRelease(pInstance);
		return nullptr;
	}
	return pInstance;
}

CGameObject * CWormBody::Clone()
{
	CWormBody* pClone = new CWormBody(*this);
	return pClone;
}

HRESULT CWormBody::InitializePrototype()
{
	CWormPart::InitializePrototype();
	return S_OK;
}

HRESULT CWormBody::Awake()
{
	CWormPart::Awake();
	m_pMeshRenderer = (CMeshRenderer*)AddComponent<CMeshRenderer>();
	m_pMeshRenderer->SetMesh(TEXT("Quad"));
	m_pTransform->Set_Scale(D3DXVECTOR3(6.4f, 5.6f, 1.f));
	m_pTransform->Set_Position(D3DXVECTOR3(240.f, -5.f, 208.f));
	m_pTransform->UpdateTransform();

	m_pCollider = (CCollider*)AddComponent<CCollider>();

	m_pCollider->SetMesh(TEXT("Quad"), BOUND::BOUNDTYPE::SPHERE);
	m_pCollider->SetRadius(3.f);
	m_pCollider->m_bExcept = true;
	m_eRenderID = ERenderID::Alpha;
	return S_OK;
}

HRESULT CWormBody::Start()
{
	CWormPart::Start();
	m_pTexturePool = CTexturePoolManager::GetInstance()->GetTexturePool(TEXT("WormBody"));
	SafeAddRef(m_pTexturePool);
	m_sTextureKey = TEXT("Side");

	m_pMeshRenderer->SetTexture(0, m_pTexturePool->GetTexture(m_sTextureKey)[0]);
	m_pPlayerTransform = (CTransform*)(FindGameObjectOfType<CPlayer>()->GetComponent<CTransform>());

	return S_OK;
}

UINT CWormBody::Update(const float _fDeltaTime)
{
	CWormPart::Update(_fDeltaTime);
	if (true == m_bRemove)
		return OBJ_DEAD;
	return 0;
}

UINT CWormBody::LateUpdate(const float _fDeltaTime)
{
	CWormPart::LateUpdate(_fDeltaTime);
	return 0;
}

HRESULT CWormBody::Render()
{
	CWormPart::Render();
	return S_OK;
}

void CWormBody::SetTextureKey()
{
	D3DXVECTOR3 vDir = m_vMoveDir;
	D3DXVec3Normalize(&vDir, &vDir);
	D3DXVECTOR3 vPlayerLook = m_pPlayerTransform->Get_Look();
	vPlayerLook = -vPlayerLook;
	D3DXVec3Normalize(&vPlayerLook, &vPlayerLook);

	float fDot = D3DXVec3Dot(&vDir, &vPlayerLook);
	float fRadian = acosf(fDot);
	float fDegree = D3DXToDegree(fRadian);

	if (fDegree < 90.f)
	{
		m_sTextureKey = TEXT("Front");
		m_pTransform->Set_Scale(D3DXVECTOR3(5.6f, 6.4f, 1.f));
	}
	else
	{
		m_sTextureKey = TEXT("Side");
		m_pTransform->Set_Scale(D3DXVECTOR3(6.4f, 5.6f, 1.f));
	}
	m_pMeshRenderer->SetTexture(0, m_pTexturePool->GetTexture(m_sTextureKey)[0]);
	m_pTransform->UpdateTransform();
}






