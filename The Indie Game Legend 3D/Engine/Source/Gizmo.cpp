#include "Gizmo.h"


USING(Engine)
CGizmo::CGizmo()
	:CGameObject()
{
}

CGizmo::CGizmo(const CGizmo & _rOther)
	:CGameObject(_rOther)
{
}

HRESULT CGizmo::InitializePrototype()
{
	if (FAILED(CGameObject::InitializePrototype()))
		return E_FAIL;
	return S_OK;
}

HRESULT CGizmo::Awake()
{
	if (FAILED(CGameObject::Awake()))
		return E_FAIL;
	m_pMeshRenderer = (CMeshRenderer*)AddComponent<CMeshRenderer>();
	m_pMeshRenderer->SetMesh(TEXT("Gizmo"));
	m_eRenderID = ERenderID::NoAlpha;
	return S_OK;
}

HRESULT CGizmo::Start()
{
	return S_OK;
}

UINT CGizmo::Update(const float _fDeltaTime)
{
	m_pTransform->UpdateTransform();
	return 0;
}

UINT CGizmo::LateUpdate(const float _fDeltaTime)
{
	CGameObject::LateUpdate(_fDeltaTime);
	return 0;
}

HRESULT CGizmo::Render()
{
#ifdef _DEBUG
	m_pTransform->UpdateWorld();
	m_pMeshRenderer->Render();
#endif // DEBUG
	return S_OK;
}

void CGizmo::Free()
{
	CGameObject::Free();
}
