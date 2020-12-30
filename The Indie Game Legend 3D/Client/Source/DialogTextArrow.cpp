#include "stdafx.h"
#include "..\Header\DialogTextArrow.h"
#include "TexturePoolManager.h"

CDialogTextArrow::CDialogTextArrow()
	: m_pImage(nullptr)
	, m_pTexturePool(nullptr)
	, m_sTextureKey(TEXT(""))
	, m_nIndex(0)
	, m_nMaxFrame(0)
	, m_fTime(0.f)
	, m_fAnimationSpeed(0.1f)
{
}

CDialogTextArrow::CDialogTextArrow(const CDialogTextArrow & _rOther)
	: CGameObject(_rOther)
	, m_pImage(nullptr)
	, m_pTexturePool(nullptr)
	, m_sTextureKey(TEXT(""))
	, m_nIndex(0)
	, m_nMaxFrame(0)
	, m_fTime(0.f)
	, m_fAnimationSpeed(0.1f)
{
}

void CDialogTextArrow::Free()
{
	CGameObject::Free();
	SafeRelease(m_pTexturePool);
}

CDialogTextArrow * CDialogTextArrow::Create()
{
	CDialogTextArrow* pInstance = new CDialogTextArrow;
	if (FAILED(pInstance->InitializePrototype()))
	{
		SafeRelease(pInstance);
		return nullptr;
	}
	return pInstance;
}

CGameObject * CDialogTextArrow::Clone()
{
	CDialogTextArrow* pClone = new CDialogTextArrow(*this);
	return pClone;
}

HRESULT CDialogTextArrow::InitializePrototype()
{
	CGameObject::InitializePrototype();
	return S_OK;
}

HRESULT CDialogTextArrow::Awake()
{
	CGameObject::Awake();
	m_pImage = (Image*)AddComponent<Image>();
	m_eRenderID = ERenderID::UI;
	return S_OK;
}

HRESULT CDialogTextArrow::Start()
{
	CGameObject::Start();
	m_pTexturePool = CTexturePoolManager::GetInstance()->GetTexturePool(TEXT("UI"));
	SafeAddRef(m_pTexturePool);

	m_sTextureKey = TEXT("TextArrow");
	
	m_nMaxFrame = m_pTexturePool->GetTexture(m_sTextureKey).size();

	m_pImage->SetTexture(m_pTexturePool->GetTexture(m_sTextureKey)[m_nIndex]);
	m_pImage->SetPivot(0.f, 0.f);

	m_pTransform->Set_Scale(D3DXVECTOR3(2.f, 2.f, 1.f));
	return S_OK;
}

UINT CDialogTextArrow::Update(const float _fDeltaTime)
{
	CGameObject::Update(_fDeltaTime);
	AnimateArrow(_fDeltaTime);
	return 0;
}

UINT CDialogTextArrow::LateUpdate(const float _fDeltaTime)
{
	CGameObject::LateUpdate(_fDeltaTime);
	return 0;
}

HRESULT CDialogTextArrow::Render()
{
	CGameObject::Render();
	m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransform->Get_WorldMatrix());
	m_pImage->Render();
	return S_OK;
}

void CDialogTextArrow::SetArrow(const D3DXVECTOR3 _vPosition)
{
	m_pTransform->Set_Position(_vPosition);
	m_pTransform->UpdateTransform();
}

void CDialogTextArrow::AnimateArrow(const float _fDeltaTime)
{
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

