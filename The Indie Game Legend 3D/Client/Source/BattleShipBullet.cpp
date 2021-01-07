#include "stdafx.h"
#include "..\Header\BattleShipBullet.h"
#include "TexturePoolManager.h"

CBattleShipBullet::CBattleShipBullet()
	: m_pMeshRenderer(nullptr)
	, m_pTexturePool(nullptr)
	, m_sTextureKey(TEXT(""))
	, m_nIndex(0)
	, m_nMaxFrame(0)
	, m_fTime(0.f)
	, m_fAnimationSpeed(0.5f)
	, m_fMoveSpeed(75.f)
	, m_bRemove(false)
{
}

CBattleShipBullet::CBattleShipBullet(const CBattleShipBullet & _rOther)
	: CGameObject(_rOther)
	, m_pMeshRenderer(nullptr)
	, m_pTexturePool(nullptr)
	, m_sTextureKey(TEXT(""))
	, m_nIndex(0)
	, m_nMaxFrame(0)
	, m_fTime(0.f)
	, m_fAnimationSpeed(0.05f)
	, m_fMoveSpeed(75.f)
	, m_bRemove(false)
{
}

void CBattleShipBullet::Free()
{
	CGameObject::Free();
	SafeRelease(m_pTexturePool);
}

CBattleShipBullet * CBattleShipBullet::Create()
{
	CBattleShipBullet* pInstance = new CBattleShipBullet;
	if (FAILED(pInstance->InitializePrototype()))
	{
		SafeRelease(pInstance);
		return nullptr;
	}
	return pInstance;
}

CGameObject * CBattleShipBullet::Clone()
{
	CBattleShipBullet* pClone = new CBattleShipBullet(*this);
	return pClone;
}

HRESULT CBattleShipBullet::InitializePrototype()
{
	CGameObject::InitializePrototype();
	return S_OK;
}

HRESULT CBattleShipBullet::Awake()
{
	CGameObject::Awake();
	m_pMeshRenderer = (CMeshRenderer*)AddComponent<CMeshRenderer>();
	m_pMeshRenderer->SetMesh(TEXT("Quad"));

	m_pTransform->Set_Rotation(D3DXVECTOR3(90.f, 0.f, 0.f));
	m_pTransform->Set_Scale(D3DXVECTOR3(0.9f, 0.6f, 1.f));
	m_pTransform->UpdateTransform();

	m_eRenderID = ERenderID::Alpha;
	return S_OK;
}

HRESULT CBattleShipBullet::Start()
{
	CGameObject::Start();
	m_pTexturePool = CTexturePoolManager::GetInstance()->GetTexturePool(TEXT("BattleShip"));
	SafeAddRef(m_pTexturePool);

	m_sTextureKey = TEXT("Bullet");
	m_nMaxFrame = m_pTexturePool->GetTexture(m_sTextureKey).size();

	m_pCollider = (CCollider*)AddComponent<CCollider>();
	m_pCollider->SetMesh(TEXT("Quad"), BOUND::BOUNDTYPE::SPHERE);
	m_pCollider->SetRadius(1.5f);
	return S_OK;
}

UINT CBattleShipBullet::Update(const float _fDeltaTime)
{
	CGameObject::Update(_fDeltaTime);
	if (true == m_bRemove)
		return OBJ_DEAD;

	Animate(_fDeltaTime);
	Move(_fDeltaTime);
	return 0;
}

UINT CBattleShipBullet::LateUpdate(const float _fDeltaTime)
{
	CGameObject::LateUpdate(_fDeltaTime);
	return 0;
}

HRESULT CBattleShipBullet::Render()
{
	CGameObject::Render();
	m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransform->Get_WorldMatrix());
	m_pMeshRenderer->Render();
	return S_OK;
}

void CBattleShipBullet::SetBullet(D3DXVECTOR3 _vStart)
{
	m_pTransform->Set_Position(_vStart);
	m_pTransform->UpdateTransform();
}

void CBattleShipBullet::Animate(const float _fDeltaTime)
{
	m_fTime += _fDeltaTime;

	if (m_fTime >= m_fAnimationSpeed)
	{
		m_fTime -= m_fAnimationSpeed;

		++m_nIndex;

		if (m_nIndex >= m_nMaxFrame)
			return;
		float fHeight = (float)m_pTexturePool->GetTexture(m_sTextureKey)[m_nIndex]->GetDesc().Height * 0.1f;
		float fWidth = (float)m_pTexturePool->GetTexture(m_sTextureKey)[m_nIndex]->GetDesc().Width * 0.1f;

		m_pTransform->Set_Scale(D3DXVECTOR3(fWidth * 1.5f, fHeight * 1.5f, 1.f));
		m_pTransform->UpdateTransform();

		m_pMeshRenderer->SetTexture(0, m_pTexturePool->GetTexture(m_sTextureKey)[m_nIndex]);
	}
}

void CBattleShipBullet::Move(const float _fDeltaTime)
{
	m_pTransform->Add_Position(D3DXVECTOR3(1.f, 0.f, 0.f) * m_fMoveSpeed * _fDeltaTime);
	m_pTransform->UpdateTransform();

	if (m_pTransform->Get_Position().x > 45.f)
		m_bRemove = true;
}

