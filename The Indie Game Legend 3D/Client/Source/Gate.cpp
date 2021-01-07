#include "stdafx.h"
#include "..\Header\Gate.h"


CGate::CGate()
{
}

CGate::CGate(const CGate & _rOther)
	:CObstacle(_rOther)

{
}

HRESULT CGate::InitializePrototype()
{
	CObstacle::InitializePrototype();
	return S_OK;
}

HRESULT CGate::Awake()
{
	CObstacle::Awake();
#ifdef _DEBUG
	m_nTag = 0;
#endif // DEBUG

	return S_OK;
}

HRESULT CGate::Start()
{
	CObstacle::Start();
	m_pMeshRenderer = (CMeshRenderer*)AddComponent<CMeshRenderer>();
	m_pMeshRenderer->SetMesh(TEXT("Gate"));
	m_eRenderID = ERenderID::NoAlpha;

	m_pTransform->UpdateTransform();
	return S_OK;
}

UINT CGate::Update(const float _fDeltaTime)
{

	return OBJ_NOENVET;
}

UINT CGate::LateUpdate(const float _fDeltaTime)
{
	CObstacle::LateUpdate(_fDeltaTime);
	return OBJ_NOENVET;
}

HRESULT CGate::Render()
{
	m_pTransform->UpdateWorld();
	m_pMeshRenderer->Render();
	return S_OK;
}

CGameObject * CGate::Clone()
{
	CGameObject* pInstance = new CGate(*this);
	return pInstance;
}

CGate * CGate::Create()
{
	CGate* pInstance = new CGate();
	return pInstance;
}

void CGate::Free()
{
	CObstacle::Free();
}
