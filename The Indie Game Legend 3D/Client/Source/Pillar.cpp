#include "stdafx.h"
#include "..\Header\Pillar.h"


CPillar::CPillar()
{
}

CPillar::CPillar(const CPillar & _rOther)
	:CObstacle(_rOther)
{
}

HRESULT CPillar::InitializePrototype()
{
	CObstacle::InitializePrototype();
	return S_OK;
}

HRESULT CPillar::Awake()
{
	CObstacle::Awake();
#ifdef _DEBUG
	m_nTag = 0;
#endif // DEBUG
	return S_OK;
}

HRESULT CPillar::Start()
{
	CObstacle::Start();
	m_pMeshRenderer = (CMeshRenderer*)AddComponent<CMeshRenderer>();
	m_pMeshRenderer->SetMesh(TEXT("Pillar"));
	m_eRenderID = ERenderID::NoAlpha;

	m_pTransform->UpdateTransform();

	m_pCollider = (CCollider*)AddComponent<CCollider>();
	m_pCollider->SetMesh(TEXT("Pillar"), BOUND::BOUNDTYPE::BOX);
	m_pCollider->m_bIsRigid = true;

	return S_OK;
}

UINT CPillar::Update(const float _fDeltaTime)
{
	return OBJ_NOENVET;
}

UINT CPillar::LateUpdate(const float _fDeltaTime)
{
	CObstacle::LateUpdate(_fDeltaTime);
	return OBJ_NOENVET;
}

HRESULT CPillar::Render()
{
	m_pTransform->UpdateWorld();
	m_pMeshRenderer->Render();
	return S_OK;
}

CGameObject * CPillar::Clone()
{
	CGameObject* pInstance = new CPillar(*this);

	return pInstance;
}

CPillar * CPillar::Create()
{
	CPillar* pInstance = new CPillar();
	if (FAILED(pInstance->InitializePrototype()))
	{
		SafeRelease(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CPillar::Free()
{
	CObstacle::Free();
}
