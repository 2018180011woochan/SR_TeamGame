#include "stdafx.h"
#include "..\Header\Selector.h"
#include "TexturePoolManager.h"
#include "KeyManager.h"
#include "SoundMgr.h"

#include "Loading.h"
#include "Stage.h"

CSelector::CSelector()
	: m_pImage(nullptr)
	, m_pTexturePool(nullptr)
	, m_pKeyManager(nullptr)
{
}

CSelector::CSelector(const CSelector & _rOther)
	: CGameObject(_rOther)
	, m_pImage(nullptr)
	, m_pTexturePool(nullptr)
	, m_pKeyManager(nullptr)
{
}

void CSelector::Free()
{
	CGameObject::Free();
	SafeRelease(m_pKeyManager);
	SafeRelease(m_pTexturePool);
}

CSelector * CSelector::Create()
{
	CSelector* pInstance = new CSelector;
	if (FAILED(pInstance->InitializePrototype()))
	{
		SafeRelease(pInstance);
		return nullptr;
	}
	return pInstance;
}

CGameObject * CSelector::Clone()
{
	CSelector* pClone = new CSelector(*this);
	return pClone;
}

HRESULT CSelector::InitializePrototype()
{
	CGameObject::InitializePrototype();
	return S_OK;
}

HRESULT CSelector::Awake()
{
	CGameObject::Awake();
	m_pImage = (Image*)AddComponent<Image>();
	m_pImage->SetPivot(0.f, 0.5f);
	m_eRenderID = ERenderID::UI;

	m_pTransform->Set_Position(D3DXVECTOR3(-200.f, -125.f, 0.f));
	m_pTransform->Set_Scale(D3DXVECTOR3(3.f, 3.f, 1.f));
	m_pTransform->UpdateTransform();
	return S_OK;
}

HRESULT CSelector::Start()
{
	CGameObject::Start();
	m_pKeyManager = CKeyManager::GetInstance();
	SafeAddRef(m_pKeyManager);
	m_pTexturePool = CTexturePoolManager::GetInstance()->GetTexturePool(TEXT("UI"));
	m_sTextureKey = TEXT("SelectArrow");
	SafeAddRef(m_pTexturePool);

	m_pImage->SetTexture(m_pTexturePool->GetTexture(m_sTextureKey)[0]);

	CSoundMgr::GetInstance()->Initialize();
	return S_OK;
}

UINT CSelector::Update(const float _fDeltaTime)
{
	CGameObject::Update(_fDeltaTime);
	static int nIndex = 0;
	if (m_pKeyManager->Key_Down(KEY_W))
	{
		--nIndex;
		CSoundMgr::GetInstance()->Play(TEXT("sfxSelect.wav"),CSoundMgr::CHANNELID::UI);
	}
	else if (m_pKeyManager->Key_Down(KEY_S))
	{
		++nIndex;
		CSoundMgr::GetInstance()->Play(TEXT("sfxSelect.wav"), CSoundMgr::CHANNELID::UI);
	}
	if (nIndex < 0)
		nIndex = 1;
	else if (nIndex > 1)
		nIndex = 0;
	if (m_pKeyManager->Key_Down(KEY_SPACE))
	{
		CSoundMgr::GetInstance()->Play(TEXT("sfxSelect.wav"), CSoundMgr::CHANNELID::UI);
		if (0 == nIndex)
		{
			CLoading* pLoading = CLoading::Create(m_pDevice);
			pLoading->OnLoading(CStage::Create());
			SafeRelease(pLoading);
		}
		else if (1 == nIndex)
		{
			exit(0);
		}
	}
	

	m_pTransform->Set_Position(D3DXVECTOR3(-200.f, -125.f + nIndex * (-50), 0.f));
	m_pTransform->UpdateTransform();

	return 0;
}

UINT CSelector::LateUpdate(const float _fDeltaTime)
{
	CGameObject::LateUpdate(_fDeltaTime);
	return 0;
}

HRESULT CSelector::Render()
{
	CGameObject::Render();
	m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransform->Get_WorldMatrix());
	m_pImage->Render();
	return S_OK;
}

