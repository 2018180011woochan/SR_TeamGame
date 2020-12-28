#include "stdafx.h"
#include "..\Header\PlayerPortrait.h"
#include "TexturePoolManager.h"

CPlayerPortrait::CPlayerPortrait()
	: m_pImage(nullptr)
	, m_pTexturePool(nullptr)
	, m_sTextureKey(TEXT(""))
	, m_nIndex(0)
	, m_nMaxFrame(0)
	, m_fTime(0.f)
	, m_fAnimationSpeed(0.1f)
	, m_bPlaying(true)
{
}

CPlayerPortrait::CPlayerPortrait(const CPlayerPortrait & _rOther)
	: CGameObject(_rOther)
	, m_pImage(nullptr)
	, m_pTexturePool(nullptr)
	, m_sTextureKey(TEXT(""))
	, m_nIndex(0)
	, m_nMaxFrame(0)
	, m_fTime(0.f)
	, m_fAnimationSpeed(0.1f)
	, m_bPlaying(true)
{
}

void CPlayerPortrait::Free()
{
	CGameObject::Free();
	SafeRelease(m_pTexturePool);
}

CPlayerPortrait * CPlayerPortrait::Create()
{
	CPlayerPortrait* pInstance = new CPlayerPortrait;
	if (FAILED(pInstance->InitializePrototype()))
	{
		SafeRelease(pInstance);
		return nullptr;
	}
	return pInstance;
}

CGameObject * CPlayerPortrait::Clone()
{
	CPlayerPortrait* pClone = new CPlayerPortrait(*this);
	return pClone;
}

HRESULT CPlayerPortrait::InitializePrototype()
{
	CGameObject::InitializePrototype();
	return S_OK;
}

HRESULT CPlayerPortrait::Awake()
{
	CGameObject::Awake();
	m_pImage = (Image*)AddComponent<Image>();

	m_pTransform->Set_Scale(D3DXVECTOR3(5.f, 5.f, 1.f));
	m_pTransform->Set_Position(D3DXVECTOR3(471.f, 189.f, 0.f));
	m_pTransform->UpdateTransform();

	m_eRenderID = ERenderID::UI;
	return S_OK;
}

HRESULT CPlayerPortrait::Start()
{
	CGameObject::Start();
	m_pTexturePool = CTexturePoolManager::GetInstance()->GetTexturePool(TEXT("Portrait"));
	SafeAddRef(m_pTexturePool);

	m_sTextureKey = TEXT("Player");

	m_nMaxFrame = m_pTexturePool->GetTexture(m_sTextureKey).size();

	m_pImage->SetTexture(m_pTexturePool->GetTexture(m_sTextureKey)[0]);
	m_pImage->SetPivot(1.f, 0.f);
	return S_OK;
}

UINT CPlayerPortrait::Update(const float _fDeltaTime)
{
	CGameObject::Update(_fDeltaTime);
	AnimatePortrait(_fDeltaTime);
	return 0;
}

UINT CPlayerPortrait::LateUpdate(const float _fDeltaTime)
{
	CGameObject::LateUpdate(_fDeltaTime);
	return 0;
}

HRESULT CPlayerPortrait::Render()
{
	CGameObject::Render();
	m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransform->Get_WorldMatrix());
	m_pImage->Render();
	return S_OK;
}

void CPlayerPortrait::OnAnimation(bool _bPlaying)
{
	m_bPlaying = _bPlaying;

	m_nIndex = 0;
}

void CPlayerPortrait::AnimatePortrait(const float _fDeltaTime)
{
	if (false == m_bPlaying)
		return;

	m_fTime += _fDeltaTime;

	if (m_fTime >= m_fAnimationSpeed)
	{
		//누적 시간 초기화.
		m_fTime -= m_fAnimationSpeed;
		//인덱스 증가
		++m_nIndex;
		//인덱스 예외 처리
		if (m_nMaxFrame <= m_nIndex)
			m_nIndex = 0;

		//이미지 컴포넌트 텍스처 변경.
		m_pImage->SetTexture(m_pTexturePool->GetTexture(m_sTextureKey)[m_nIndex]);
	}
}

