#include "stdafx.h"
#include "..\Header\Sector.h"
#include "TexturePoolManager.h"

CSector::CSector()
	: m_pTexturePool(nullptr)
	, m_sTextureKey(TEXT(""))
	, m_nIndex(0)
	, m_nMaxFrame(0)
	, m_fTime(0.f)
	, m_fAnimationSpeed(0.1f)
{
}

CSector::CSector(const CSector & _rOther)
	: CGameObject(_rOther)
	, m_pTexturePool(nullptr)
	, m_sTextureKey(TEXT(""))
	, m_nIndex(0)
	, m_nMaxFrame(0)
	, m_fTime(0.f)
	, m_fAnimationSpeed(0.1f)
{
}

HRESULT CSector::InitializePrototype()
{
	if (FAILED(CGameObject::InitializePrototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CSector::Awake()
{
	if (FAILED(CGameObject::Awake()))
		return E_FAIL;
	m_pMeshRenderer = (CMeshRenderer*)AddComponent<CMeshRenderer>();
	m_pMeshRenderer->SetMesh(m_sSectorName);
	m_eRenderID = ERenderID::NoAlpha;

	return S_OK;
}

HRESULT CSector::Start()
{
	m_pTexturePool = CTexturePoolManager::GetInstance()->GetTexturePool(TEXT("SectorZ"));
	SafeAddRef(m_pTexturePool);
	m_sTextureKey = TEXT("Wall");

	m_nMaxFrame = m_pTexturePool->GetTexture(m_sTextureKey).size();
	return S_OK;
}

UINT CSector::Update(const float _fDeltaTime)
{
	CGameObject::Update(_fDeltaTime);
	Animate(_fDeltaTime);
	//m_pTransform->UpdateTransform();
	return OBJ_NOENVET;
}

UINT CSector::LateUpdate(const float _fDeltaTime)
{
	CGameObject::LateUpdate(_fDeltaTime);

	return OBJ_NOENVET;
}

HRESULT CSector::Render()
{
	if (FAILED(CGameObject::Render()))
		return E_FAIL;
	m_pTransform->UpdateWorld();
	m_pMeshRenderer->Render();
	return S_OK;
}

CGameObject * CSector::Clone()
{
	CSector* pInstance = new CSector(*this);
	return pInstance;
}

CSector * CSector::Create()
{
	CSector* pInstance = new CSector();
	return pInstance;
}

void CSector::Free()
{
	CGameObject::Free();
	SafeRelease(m_pTexturePool);
}
/*addGameObject 하면 반환 된 오브젝트 주소로 반드시 이거 호출해서 네임 세팅*/
void CSector::SetSectorName(const TSTRING & _sSectorName)
{
	m_sSectorName = _sSectorName;
}

void CSector::Animate(const float _fDeltaTime)
{
	m_fTime += _fDeltaTime;

	if (m_fTime >= m_fAnimationSpeed)
	{
		m_fTime -= m_fAnimationSpeed;

		++m_nIndex;
		if (m_nIndex >= m_nMaxFrame)
			m_nIndex = 0;

		m_pMeshRenderer->SetTexture(0, m_pTexturePool->GetTexture(m_sTextureKey)[m_nIndex]);
	}
}
