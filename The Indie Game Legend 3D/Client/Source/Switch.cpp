#include "stdafx.h"
#include "..\Header\Switch.h"
#include "SoundMgr.h"


CSwitch::CSwitch()
{
}

CSwitch::CSwitch(const CSwitch & _rOther)
	:CInteractionObj(_rOther)
{
}

HRESULT CSwitch::InitializePrototype()
{
	m_sName = L"Button";
#ifdef _DEBUG
	m_nTag = 0;
#endif // DEBUG

	return S_OK;
}

HRESULT CSwitch::Awake()
{
	m_eRenderID = ERenderID::NoAlpha;
	m_bPress = false;
	return S_OK;
}

HRESULT CSwitch::Start()
{
	m_pMeshRenderer = (CMeshRenderer*)AddComponent<CMeshRenderer>();
	m_pMeshRenderer->SetMesh(TEXT("SwitchUp"));

	m_pCollider = (CCollider*)AddComponent<CCollider>();
	m_pCollider->SetMesh(TEXT("SwitchUp"), BOUND::BOUNDTYPE::BOX);
	m_pCollider->m_bIsRigid = true;

	m_pTransform->Add_Position(_vector(0, 0, 0));
	m_pTransform->UpdateTransform();
	return S_OK;
}

UINT CSwitch::Update(const float _fDeltaTime)
{
	return 0;
}

UINT CSwitch::LateUpdate(const float _fDeltaTime)
{
	CInteractionObj::LateUpdate(_fDeltaTime);
	return 0;
}

HRESULT CSwitch::Render()
{
	m_pTransform->UpdateWorld();
	m_pMeshRenderer->Render();
	return S_OK;
}

void CSwitch::OnCollision(CGameObject * _pGameObject)
{
	if (L"Player" == _pGameObject->GetName() && m_bPress == false )
	{
		CSoundMgr::GetInstance()->Play(L"sfxSwitch.wav", CSoundMgr::Object_SFX);
		SwitchDown();
		Notify();
	}
}
void CSwitch::SwitchUp()
{
	m_pMeshRenderer->SetMesh(TEXT("SwitchUp"));
	m_bPress = false;
}

void CSwitch::SwitchDown()
{
	m_bPress = true;
	m_pMeshRenderer->SetMesh(TEXT("SwitchDown"));
}


void CSwitch::OnNotify()
{
}

void CSwitch::Free()
{
	CInteractionObj::Free();
}

CGameObject * CSwitch::Clone()
{
	CGameObject* pIns = new CSwitch(*this);
	return pIns;
}

CSwitch * CSwitch::Create()
{
	CSwitch* pIns = new CSwitch();
	if (FAILED(pIns->InitializePrototype()))
	{
		SafeRelease(pIns);
		return nullptr;
	}
	return pIns;
}
