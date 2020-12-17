#include "stdafx.h"
#include "..\Header\CrossHair.h"
#include "TexturePoolManager.h"

CCrossHair::CCrossHair()
	: m_pImage(nullptr)
	, m_pTexturePool(nullptr)
	, m_nMaxFrame(0)
	, m_nIndex(0)
	, m_fAnimationSpeed(0.5f)
	, m_fTime(0.f)
	, m_sTextureKey(TEXT(""))
{
}

CCrossHair::CCrossHair(const CCrossHair & _rOther)
	: CGameObject(_rOther)
{

}

void CCrossHair::Free()
{
	CGameObject::Free();
	SafeRelease(m_pTexturePool);
}

CCrossHair * CCrossHair::Create()
{
	CCrossHair* pInstance = new CCrossHair;

	if (FAILED(pInstance->InitializePrototype()))
	{
		delete pInstance;
		return nullptr;
	}

	return pInstance;
}

CGameObject * CCrossHair::Clone()
{
	CCrossHair* pClone = new CCrossHair(*this);
	return pClone;
}

HRESULT CCrossHair::InitializePrototype()
{
	CGameObject::InitializePrototype();
	return S_OK;
}

HRESULT CCrossHair::Awake()
{
	CGameObject::Awake();
	m_pImage = (Image*)AddComponent<Image>();

	m_pTransform->Set_Scale(D3DXVECTOR3(5.f, 5.f, 1.f));
	m_pTransform->UpdateTransform();
	//Set RenderID
	m_eRenderID = ERenderID::UI;

	m_fTime = 0.f;
	m_fAnimationSpeed = 0.1f;

	m_fMaxScale = 10.f;
	m_fMinScale = 5.f;
	m_fLerpAmount = 0.f;
	m_fLerpSpeed = 5.f;
	m_bIncrease = true;
	return S_OK;
}

HRESULT CCrossHair::Start()
{
	CGameObject::Start();
	m_pTexturePool = CTexturePoolManager::GetInstance()->GetTexturePool(TEXT("UI"));
	SafeAddRef(m_pTexturePool);

	m_sTextureKey = TEXT("CrossHair");

	m_nMaxFrame = m_pTexturePool->GetTexture(m_sTextureKey).size();

	m_pImage->SetTexture(m_pTexturePool->GetTexture(m_sTextureKey)[0]);
	return S_OK;
}

UINT CCrossHair::Update(const float _fDeltaTime)
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

	m_fLerpAmount += (m_bIncrease ? _fDeltaTime : -_fDeltaTime) * m_fLerpSpeed;

	LerpScale();

	m_pTransform->UpdateTransform();
	return 0;
}

UINT CCrossHair::LateUpdate(const float _fDeltaTime)
{
	CGameObject::LateUpdate(_fDeltaTime);
	return 0;
}

HRESULT CCrossHair::Render()
{
	CGameObject::Render();
	m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransform->Get_WorldMatrix());
	m_pImage->Render();
	return S_OK;
}

void CCrossHair::LerpScale()
{
	if (1.f < m_fLerpAmount)
	{
		m_fLerpAmount = 1.f;
		m_bIncrease = false;
	}
	else if (0.f > m_fLerpAmount)
	{
		m_fLerpAmount = 0.f;
		m_bIncrease = true;
	}

	float m_fScale = float(m_fMinScale + (float(m_fMaxScale - m_fMinScale) * m_fLerpAmount));
	m_pTransform->Set_Scale(D3DXVECTOR3(m_fScale, m_fScale, 1.f));
}

