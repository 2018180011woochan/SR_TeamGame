#include "stdafx.h"
#include "..\Header\Door.h"


CDoor::CDoor()
{
}

CDoor::CDoor(const CDoor & _rOther)
	:CInteractionObj(_rOther)
	, m_bOpen(_rOther.m_bOpen)
{
}

HRESULT CDoor::InitializePrototype()
{
	m_bOpen = false;
	m_fTextureIndex = 0.f;
	m_fAnimateSpeed = 0.f;
	m_fAnimateOneCycleTime = 0.f;
	m_nMaxFrame = 0;

	m_sName = L"Door";
	m_eRenderID = ERenderID::Alpha;

	return S_OK;
}

HRESULT CDoor::Awake()
{
	//Mesh Setting
	m_pMeshRenderer = (CMeshRenderer*)AddComponent<CMeshRenderer>();
	m_pMeshRenderer->SetMesh(TEXT("Quad"));

	m_pCollider = (CCollider*)AddComponent<CCollider>();
	m_pCollider->SetMesh(L"Quad", BOUND::BOUNDTYPE::BOX);
	m_eRenderID = ERenderID::Alpha;


	m_pTexturePool = CTexturePoolManager::GetInstance()->GetTexturePool(TEXT("Object"));
	SafeAddRef(m_pTexturePool);

	m_pMeshRenderer->SetTexture(0, m_pTexturePool->GetTexture(TEXT("Electric"))[0]);



	return S_OK;
}

HRESULT CDoor::Start()
{
	return S_OK;
}

UINT CDoor::Update(const float _fDeltaTime)
{
	return OBJ_NOENVET;
}

UINT CDoor::LateUpdate(const float _fDeltaTime)
{
	CInteractionObj::LateUpdate(_fDeltaTime);

	return OBJ_NOENVET;
}

HRESULT CDoor::Render()
{
	return S_OK;
}


void CDoor::Open()
{
}

void CDoor::Close()
{
}




void CDoor::Animate(const float _fDeltaTime)
{
}


void CDoor::OnCollision(CGameObject * _pGameObject)
{
}


void CDoor::OnNotify()
{
}

void CDoor::Free()
{
	SafeRelease(m_pTexturePool);
}

CGameObject * CDoor::Clone()
{
	CGameObject* pIns = new CDoor(*this);
	return pIns;
}

CDoor * CDoor::Create()
{
	CDoor* pIns = new CDoor();
	if (FAILED(pIns->InitializePrototype()))
	{
		SafeRelease(pIns);
		return nullptr;
	}
	return pIns;
}