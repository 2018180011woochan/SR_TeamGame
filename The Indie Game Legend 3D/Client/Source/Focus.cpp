#include "stdafx.h"
#include "..\Header\Focus.h"
#include "TexturePoolManager.h"


CFocus::CFocus()

{
}

CFocus::CFocus(const CFocus & _rOther)
	:CGameObject(_rOther)
{
}

HRESULT CFocus::InitializePrototype()
{
	CGameObject::InitializePrototype();
	return S_OK;

}

HRESULT CFocus::Awake()
{
	CGameObject::Awake();
	m_pImage = (Image*)AddComponent<Image>();

	m_pTransform->Set_Scale(D3DXVECTOR3(0.01f, 0.01f, 0.01f));
	m_pTransform->Set_Rotation(D3DXVECTOR3(90, 0, 0));

	//Set RenderID
	m_eRenderID = ERenderID::Alpha;

	m_fTime = 0.f;
	m_fAnimationSpeed = 0.1f;

	m_fMaxScale = 0.2f;
	m_fMinScale = 0.01f;
	m_fLerpAmount = 0.f;
	m_fLerpSpeed = 0.5f;
	m_bIncrease = true;

	m_bEnable = false;
	return S_OK;
}

HRESULT CFocus::Start()
{
	CGameObject::Start();
	m_pTexturePool = CTexturePoolManager::GetInstance()->GetTexturePool(TEXT("UI"));

	m_pImage->SetTexture(m_pTexturePool->GetTexture(L"Focus")[0]);
	return S_OK;
}

UINT CFocus::Update(const float _fDeltaTime)
{
	CGameObject::Update(_fDeltaTime);
	if (m_fLerpAmount < m_fMinScale)
	{
		m_fLerpAmount = m_fMinScale;
		m_fLerpSpeed *= -1;
	}
	if (m_fLerpAmount > m_fMaxScale)
	{
		m_fLerpAmount = m_fMaxScale;
		m_fLerpSpeed *= -1;
	}

	m_fLerpAmount += m_fLerpSpeed * _fDeltaTime;
	LerpScale();
	m_pTransform->UpdateTransform();
	return OBJ_NOENVET;
}

UINT CFocus::LateUpdate(const float _fDeltaTime)
{
	CGameObject::LateUpdate(_fDeltaTime);

	return 0;
}

HRESULT CFocus::Render()
{
	CGameObject::Render();
	m_pTransform->UpdateWorld();
	m_pImage->Render();
	return S_OK;
}

CGameObject * CFocus::Clone()
{
	CFocus* pClone = new CFocus(*this);
	return pClone;
}

CFocus * CFocus::Create()
{
	CFocus* pInstance = new CFocus;

	if (FAILED(pInstance->InitializePrototype()))
	{
		delete pInstance;
		return nullptr;
	}

	return pInstance;
}

void CFocus::SetFocusPos(const _vector & _vPos)
{
	m_pTransform->Set_Position(_vPos);
}

void CFocus::LerpScale()
{
	float m_fScale = (1.f - m_fLerpAmount) * m_fMinScale + (m_fLerpAmount * m_fMaxScale);
	m_pTransform->Set_Scale(D3DXVECTOR3(m_fScale, m_fScale, m_fScale));

}

void CFocus::Free()
{
	CGameObject::Free();
}
