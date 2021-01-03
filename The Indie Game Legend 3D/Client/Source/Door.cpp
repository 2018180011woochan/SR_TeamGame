#include "stdafx.h"
#include "..\Header\Door.h"


CDoor::CDoor()
{
}

CDoor::CDoor(const CDoor & _rOther)
	:CInteractionObj(_rOther)
	, m_bOpen(_rOther.m_bOpen)
	, m_fTextureIndex(_rOther.m_fTextureIndex)
	, m_fAnimateSpeed(_rOther.m_fAnimateSpeed)
	, m_fAnimateOneCycleTime(_rOther.m_fAnimateOneCycleTime)
	, m_bAnimate(true)
	, m_pTexturePool(nullptr)
{
}

HRESULT CDoor::InitializePrototype()
{
	m_bOpen = false;
	m_pTexturePool = nullptr;
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

	m_pTransform->Set_Scale(_vector(8, 16, 1));
	m_pTransform->Add_Position(_vector(0, 8, 0));

	return S_OK;
}

HRESULT CDoor::Start()
{
	m_pMeshRenderer = (CMeshRenderer*)AddComponent<CMeshRenderer>();
	m_pMeshRenderer->SetMesh(TEXT("Quad"));

	m_pCollider = (CCollider*)AddComponent<CCollider>();
	m_pCollider->SetMesh(L"Quad", BOUND::BOUNDTYPE::BOX);
	m_eRenderID = ERenderID::Alpha;
	m_pCollider->m_bIsRigid = true;


	m_pTexturePool = CTexturePoolManager::GetInstance()->GetTexturePool(TEXT("Object"));
	SafeAddRef(m_pTexturePool);
	m_pMeshRenderer->SetTexture(0, m_pTexturePool->GetTexture(TEXT("Door"))[0]);

	m_nMaxFrame = m_pTexturePool->GetTexture(TEXT("Door")).size();
	m_fAnimateOneCycleTime = 1.f;

	m_fAnimateSpeed = (m_nMaxFrame) / 1.f * m_fAnimateOneCycleTime;
	return S_OK;
}

UINT CDoor::Update(const float _fDeltaTime)
{
	Animate(_fDeltaTime);

	return OBJ_NOENVET;
}

UINT CDoor::LateUpdate(const float _fDeltaTime)
{
	CInteractionObj::LateUpdate(_fDeltaTime);
	return OBJ_NOENVET;
}

HRESULT CDoor::Render()
{
	m_pTransform->UpdateTransform();
	m_pTransform->UpdateWorld();
	m_pMeshRenderer->Render();
	return S_OK;
}


void CDoor::Open()
{
	m_bAnimate = true;
}

void CDoor::Close()
{
	m_bAnimate = true;
	m_bEnable = true;
}

void CDoor::Animate(const float _fDeltaTime)
{
	if (m_bAnimate == false)
		return;

	m_fTextureIndex += _fDeltaTime * (m_bOpen ?  -m_fAnimateSpeed :  m_fAnimateSpeed);

	if (m_bOpen == false && m_nMaxFrame < m_fTextureIndex)
	{
		m_bAnimate = false;
		m_fTextureIndex = (float)m_nMaxFrame - 1;
		m_pCollider->m_bIsRigid = true;

	}
	else if(m_bOpen  && 0 > m_fTextureIndex)
	{
		m_bAnimate = false;
		m_fTextureIndex = 0.f;
		m_bEnable = false;
		m_pCollider->m_bIsRigid = false;
	}

	m_pMeshRenderer->SetTexture(0, m_pTexturePool->GetTexture(TEXT("Door"))[(_int)m_fTextureIndex]);
}

void CDoor::OnCollision(CGameObject * _pGameObject)
{
}

void CDoor::OnNotify()
{
	m_bOpen = !m_bOpen;
	if (m_bOpen)
		Open();
	else
		Close();
}

void CDoor::Free()
{
	SafeRelease(m_pTexturePool);
	CInteractionObj::Free();
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