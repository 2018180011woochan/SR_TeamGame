#include "stdafx.h"
#include "..\Header\FinalExplosion.h"
#include "TexturePoolManager.h"

CFinalExplosion::CFinalExplosion()
	: m_pMeshRenderer(nullptr)
	, m_pCollider(nullptr)
	, m_pTexturePool(nullptr)
	, m_sTextureKey(TEXT(""))
	, m_nIndex(0)
	, m_nMaxFrame(0)
	, m_fTime(0.f)
	, m_fAnimationSpeed(0.1f)
	, m_bRemove(false)
{
}

CFinalExplosion::CFinalExplosion(const CFinalExplosion & _rOther)
	: CGameObject(_rOther)
	, m_pMeshRenderer(nullptr)
	, m_pCollider(nullptr)
	, m_pTexturePool(nullptr)
	, m_sTextureKey(TEXT(""))
	, m_nIndex(0)
	, m_nMaxFrame(0)
	, m_fTime(0.f)
	, m_fAnimationSpeed(0.1f)
	, m_bRemove(false)
{
}

void CFinalExplosion::Free()
{
	CGameObject::Free();
	SafeRelease(m_pTexturePool);
}

CFinalExplosion * CFinalExplosion::Create()
{
	CFinalExplosion* pInstance = new CFinalExplosion;
	if (FAILED(pInstance->InitializePrototype()))
	{
		SafeRelease(pInstance);
		return nullptr;
	}
	return pInstance;
}

CGameObject * CFinalExplosion::Clone()
{
	CFinalExplosion* pClone = new CFinalExplosion(*this);
	return pClone;
}

HRESULT CFinalExplosion::InitializePrototype()
{
	CGameObject::InitializePrototype();
	return S_OK;
}

HRESULT CFinalExplosion::Awake()
{
	CGameObject::Awake();

	m_pTransform->Set_Rotation(D3DXVECTOR3(90.f, 180.f, 0.f));
	m_pTransform->Set_Scale(D3DXVECTOR3(2.f, 2.f, 1.f));
	m_pTransform->UpdateTransform();

	m_eRenderID = ERenderID::Alpha;

	m_pCollider = (CCollider*)AddComponent<CCollider>();
	m_pCollider->SetMesh(TEXT("Quad"), BOUND::BOUNDTYPE::SPHERE);
	return S_OK;
}

HRESULT CFinalExplosion::Start()
{
	CGameObject::Start();
	m_sName = TEXT("FinalBullet");

	return S_OK;
}

UINT CFinalExplosion::Update(const float _fDeltaTime)
{
	CGameObject::Update(_fDeltaTime);
	if (true == m_bRemove)
		return OBJ_DEAD;
	return 0;
}

UINT CFinalExplosion::LateUpdate(const float _fDeltaTime)
{
	CGameObject::LateUpdate(_fDeltaTime);
	return 0;
}

HRESULT CFinalExplosion::Render()
{
	CGameObject::Render();
	return S_OK;
}

void CFinalExplosion::SetPosition(D3DXVECTOR3 _vPosition)
{
	m_pTransform->Set_Position(_vPosition);
	m_pTransform->UpdateTransform();
}

void CFinalExplosion::Remove()
{
	m_bRemove = true;
}


