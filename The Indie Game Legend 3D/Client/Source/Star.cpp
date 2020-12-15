#include "stdafx.h"
#include "..\Header\Star.h"
#include "TexturePoolManager.h"

CStar::CStar()
	: m_pImage(nullptr)
	, m_pTexturePool(nullptr)
	, m_nIndex(0)
	, m_nMaxFrame(0)
	, m_fTime(0)
	, m_fAnimationSpeed(0)
{
}

CStar::CStar(const CStar & _rOther)
	: CGameObject(_rOther)
{
}

void CStar::Free()
{
	SafeRelease(m_pTexturePool);
	SafeRelease(m_pImage);
	CGameObject::Free();
}

CStar * CStar::Create()
{
	CStar* pInstance = new CStar;

	return pInstance;
}

CGameObject * CStar::Clone()
{
	CStar* pClone = new CStar(*this);
	return pClone;
}

HRESULT CStar::InitializePrototype()
{
	return S_OK;
}

HRESULT CStar::Awake()
{
	CGameObject::Awake();
	
	//Add Component
	m_pImage = (Image*)AddComponent<Image>();
	//Set RenderID
	m_eRenderID = ERenderID::UI;

	m_fTime = 0.f;
	m_fAnimationSpeed = 0.2f;
	return S_OK;
}

HRESULT CStar::Start()
{
	CGameObject::Start();

	m_pTransform->Set_Scale(D3DXVECTOR3(2.8f, 2.8f, 1.f));
	m_pTransform->UpdateTransform();
	//Star 스프라이트 텍스처들을 담고있는 텍스처 풀 가져옴.
	m_pTexturePool = CTexturePoolManager::GetInstance()->GetTexturePool(TEXT("UI"));
	SafeAddRef(m_pTexturePool);
	//스프라이트 최대 인덱스 설정 및 시작 인덱스 난수 설정.
	m_nMaxFrame = m_pTexturePool->GetTexture(TEXT("Star")).size();
	m_nIndex = rand() % m_nMaxFrame;
	//이미지 컴포넌트의 텍스처 설정.
	m_pImage->SetTexture(m_pTexturePool->GetTexture(TEXT("Star"))[m_nIndex]);
	return S_OK;
}

UINT CStar::Update(const float _fDeltaTime)
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
		m_pImage->SetTexture(m_pTexturePool->GetTexture(TEXT("Star"))[m_nIndex]);
	}
	
	return 0;
}

UINT CStar::LateUpdate(const float _fDeltaTime)
{
	CGameObject::LateUpdate(_fDeltaTime);
	return 0;
}

HRESULT CStar::Render()
{
	CGameObject::Render();
	m_pDevice->SetTransform(D3DTS_WORLD, &(m_pTransform->Get_WorldMatrix()));
	m_pImage->Render();
	return S_OK;
}


