#include "stdafx.h"
#include "..\Header\GemIcon.h"
#include "TexturePoolManager.h"

CGemIcon::CGemIcon()
	: m_pTexturePool(nullptr)
	, m_nMaxFrame(0)
	, m_nIndex(0)
{
}

CGemIcon::CGemIcon(const CGemIcon & _rOther)
	: CGameObject(_rOther)
{
}

void CGemIcon::Free()
{
	CGameObject::Free();
	SafeRelease(m_pTexturePool);
}

CGemIcon * CGemIcon::Create()
{
	CGemIcon* pInstance = new CGemIcon;
	if (FAILED(pInstance->InitializePrototype()))
	{
		SafeRelease(pInstance);
		return nullptr;
	}
	return pInstance;
}

CGameObject * CGemIcon::Clone()
{
	CGemIcon* pClone = new CGemIcon(*this);
	return pClone;
}

HRESULT CGemIcon::InitializePrototype()
{
	CGameObject::InitializePrototype();
	return S_OK;
}

HRESULT CGemIcon::Awake()
{
	CGameObject::Awake();
	m_pImage = (Image*)(AddComponent<Image>());
	m_eRenderID = ERenderID::UI;

	m_fTime = 0.f;
	m_fAnimationSpeed = 0.1f;
	return S_OK;
}

HRESULT CGemIcon::Start()
{
	CGameObject::Start();
	m_pTransform->Set_Position(D3DXVECTOR3(475.f, 305.f, 0.f));
	m_pTransform->Set_Scale(D3DXVECTOR3(5.f, 5.f, 1.f));
	m_pTransform->UpdateTransform();
	m_pTexturePool = CTexturePoolManager::GetInstance()->GetTexturePool(TEXT("UI"));
	SafeAddRef(m_pTexturePool);
	m_sTextureKey = TEXT("GemIcon");
	m_nMaxFrame = m_pTexturePool->GetTexture(m_sTextureKey).size();
	m_pImage->SetTexture(m_pTexturePool->GetTexture(m_sTextureKey)[0]);
	return S_OK;
}

UINT CGemIcon::Update(const float _fDeltaTime)
{
	CGameObject::Update(_fDeltaTime);

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

	return 0;
}

UINT CGemIcon::LateUpdate(const float _fDeltaTime)
{
	CGameObject::LateUpdate(_fDeltaTime);
	return 0;
}

HRESULT CGemIcon::Render()
{
	CGameObject::Render();
	m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransform->Get_WorldMatrix());
	m_pImage->Render();
	return S_OK;
}
