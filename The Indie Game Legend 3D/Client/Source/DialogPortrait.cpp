#include "stdafx.h"
#include "..\Header\DialogPortrait.h"
#include "TexturePoolManager.h"

CDialogPortrait::CDialogPortrait()
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

CDialogPortrait::CDialogPortrait(const CDialogPortrait & _rOther)
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

void CDialogPortrait::Free()
{
	CGameObject::Free();
	SafeRelease(m_pTexturePool);
}

CDialogPortrait* CDialogPortrait::Create()
{
	CDialogPortrait* pInstance = new CDialogPortrait;
	if (FAILED(pInstance->InitializePrototype()))
	{
		SafeRelease(pInstance);
		return nullptr;
	}
	return pInstance;
}

CGameObject * CDialogPortrait::Clone()
{
	CDialogPortrait* pClone = new CDialogPortrait(*this);
	return pClone;
}

HRESULT CDialogPortrait::InitializePrototype()
{
	CGameObject::InitializePrototype();
	return S_OK;
}

HRESULT CDialogPortrait::Awake()
{
	CGameObject::Awake();
	m_pImage = (Image*)AddComponent<Image>();

	
	m_pTransform->Set_Scale(D3DXVECTOR3(5.f, 5.f, 1.f));
	//플레이어 위치
	m_pTransform->Set_Position(D3DXVECTOR3(471.f, 189.f, 0.f));
	//npc 위치
	m_pTransform->Set_Position(D3DXVECTOR3(-470.f, 189.f, 0.f));
	m_pTransform->UpdateTransform();
	m_pTransform->UpdateTransform();

	m_eRenderID = ERenderID::UI;
	return S_OK;
}

HRESULT CDialogPortrait::Start()
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

UINT CDialogPortrait::Update(const float _fDeltaTime)
{
	CGameObject::Update(_fDeltaTime);
	AnimatePortrait(_fDeltaTime);
	return 0;
}

UINT CDialogPortrait::LateUpdate(const float _fDeltaTime)
{
	CGameObject::LateUpdate(_fDeltaTime);
	return 0;
}

HRESULT CDialogPortrait::Render()
{
	CGameObject::Render();
	m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransform->Get_WorldMatrix());
	m_pImage->Render();
	return S_OK;
}

void CDialogPortrait::OnAnimation(bool _bPlaying)
{
	m_bPlaying = _bPlaying;

	m_nIndex = 0;

	m_pImage->SetTexture(m_pTexturePool->GetTexture(m_sTextureKey)[m_nIndex]);
}

void CDialogPortrait::SetPortrait(const TSTRING & _sPortrait, const D3DXVECTOR3 _vPosition)
{
	m_sTextureKey = _sPortrait;
	m_nMaxFrame = m_pTexturePool->GetTexture(m_sTextureKey).size();
	OnAnimation(true);
	m_pTransform->Set_Position(_vPosition);
	m_pTransform->UpdateTransform();
}

void CDialogPortrait::AnimatePortrait(const float _fDeltaTime)
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
