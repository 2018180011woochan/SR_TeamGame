#include "stdafx.h"
#include "..\Header\NPCPortrait.h"
#include "TexturePoolManager.h"

CNPCPortrait::CNPCPortrait()
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

CNPCPortrait::CNPCPortrait(const CNPCPortrait & _rOther)
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

void CNPCPortrait::Free()
{
	CGameObject::Free();
	SafeRelease(m_pTexturePool);
}

CNPCPortrait * CNPCPortrait::Create()
{
	CNPCPortrait* pInstance = new CNPCPortrait;
	if (FAILED(pInstance->InitializePrototype()))
	{
		SafeRelease(pInstance);
		return nullptr;
	}
	return pInstance;
}

CGameObject * CNPCPortrait::Clone()
{
	CNPCPortrait* pClone = new CNPCPortrait(*this);
	return pClone;
}

HRESULT CNPCPortrait::InitializePrototype()
{
	CGameObject::InitializePrototype();
	return S_OK;
}

HRESULT CNPCPortrait::Awake()
{
	CGameObject::Awake();
	m_pImage = (Image*)AddComponent<Image>();

	m_pTransform->Set_Scale(D3DXVECTOR3(5.f, 5.f, 1.f));
	m_pTransform->Set_Position(D3DXVECTOR3(-470.f, 189.f, 0.f));
	m_pTransform->UpdateTransform();

	m_eRenderID = ERenderID::UI;
	return S_OK;
}

HRESULT CNPCPortrait::Start()
{
	CGameObject::Start();
	m_pTexturePool = CTexturePoolManager::GetInstance()->GetTexturePool(TEXT("Portrait"));
	SafeAddRef(m_pTexturePool);

	m_pImage->SetPivot(0.f, 0.f);

	SetPortrait(TEXT("Boss"));
	return S_OK;
}

UINT CNPCPortrait::Update(const float _fDeltaTime)
{
	CGameObject::Update(_fDeltaTime);
	AnimatePortrait(_fDeltaTime);
	return 0;
}

UINT CNPCPortrait::LateUpdate(const float _fDeltaTime)
{
	CGameObject::LateUpdate(_fDeltaTime);
	return 0;
}

HRESULT CNPCPortrait::Render()
{
	CGameObject::Render();
	m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransform->Get_WorldMatrix());
	m_pImage->Render();
	return S_OK;
}

void CNPCPortrait::OnAnimation(bool _bPlaying)
{
	m_bPlaying = _bPlaying;

	m_nIndex = 0;

	m_pImage->SetTexture(m_pTexturePool->GetTexture(m_sTextureKey)[m_nIndex]);
}

void CNPCPortrait::SetPortrait(const TSTRING & _sPortrait)
{
	m_sTextureKey = _sPortrait;
	m_nIndex = 0;
	m_nMaxFrame = m_pTexturePool->GetTexture(m_sTextureKey).size();
	m_pImage->SetTexture(m_pTexturePool->GetTexture(m_sTextureKey)[m_nIndex]);
}

void CNPCPortrait::AnimatePortrait(const float _fDeltaTime)
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

