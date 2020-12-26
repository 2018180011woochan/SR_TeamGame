#include "stdafx.h"
#include "..\Header\DiscIcon.h"
#include "TexturePoolManager.h"

CDiscIcon::CDiscIcon()
	: m_pTexturePool(nullptr)
	, m_pImage(nullptr)
	, m_sTextureKey(TEXT(""))
	, m_nMaxFrame(0)
{
}

CDiscIcon::CDiscIcon(const CDiscIcon & _rOther)
	:CGameObject(_rOther)
{
}

void CDiscIcon::Free()
{
	CGameObject::Free();
	SafeRelease(m_pTexturePool);
}

CDiscIcon * CDiscIcon::Create()
{
	CDiscIcon* pInstance = new CDiscIcon;
	if (FAILED(pInstance->InitializePrototype()))
	{
		SafeRelease(pInstance);
		return nullptr;
	}
	return pInstance;
}

CGameObject * CDiscIcon::Clone()
{
	CDiscIcon* pClone = new CDiscIcon(*this);
	return pClone;
}

HRESULT CDiscIcon::InitializePrototype()
{
	CGameObject::InitializePrototype();
	return S_OK;
}

HRESULT CDiscIcon::Awake()
{
	CGameObject::Awake();
	m_pImage = (Image*)AddComponent<Image>();
	m_eRenderID = ERenderID::UI;

	m_fTime = 0.f;
	m_fAnimationSpeed = 0.25f;
	m_nIndex = 0;
	return S_OK;
}

HRESULT CDiscIcon::Start()
{
	CGameObject::Start();	
	m_pTransform->Set_Position(D3DXVECTOR3(475.f, 255.f, 0.f));
	m_pTransform->Set_Scale(D3DXVECTOR3(2.5f, 2.5f, 1.f));
	m_pTransform->UpdateTransform();
	m_pTexturePool = CTexturePoolManager::GetInstance()->GetTexturePool(TEXT("UI"));
	SafeAddRef(m_pTexturePool);
	m_sTextureKey = TEXT("DiscIcon");
	m_nMaxFrame = m_pTexturePool->GetTexture(m_sTextureKey).size();
	m_pImage->SetTexture(m_pTexturePool->GetTexture(m_sTextureKey)[0]);
	return S_OK;
}

UINT CDiscIcon::Update(const float _fDeltaTime)
{
	CGameObject::Update(_fDeltaTime);

	m_fTime += _fDeltaTime;

	if (m_fTime >= m_fAnimationSpeed)
	{
		//���� �ð� �ʱ�ȭ.
		m_fTime -= m_fAnimationSpeed;
		//�ε��� ����
		++m_nIndex;
		//�ε��� ���� ó��
		if (m_nMaxFrame <= m_nIndex)
			m_nIndex = 0;

		//�̹��� ������Ʈ �ؽ�ó ����.
		m_pImage->SetTexture(m_pTexturePool->GetTexture(m_sTextureKey)[m_nIndex]);
	}

	return 0;
}

UINT CDiscIcon::LateUpdate(const float _fDeltaTime)
{
	CGameObject::LateUpdate(_fDeltaTime);
	return 0;
}

HRESULT CDiscIcon::Render()
{
	CGameObject::Render();
	m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransform->Get_WorldMatrix());
	m_pImage->Render();
	return S_OK;
}

