#include "stdafx.h"
#include "..\Header\Sector.h"


CSector::CSector()
{
}

CSector::CSector(const CSector & _rOther)
	: CGameObject(_rOther)
{
}

HRESULT CSector::InitializePrototype()
{
	if (FAILED(CGameObject::InitializePrototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CSector::Awake()
{
	if (FAILED(CGameObject::Awake()))
		return E_FAIL;
	m_pMeshRenderer = (CMeshRenderer*)AddComponent<CMeshRenderer>();
	m_pMeshRenderer->SetMesh(m_sSectorName);
	m_eRenderID = ERenderID::Alpha;

	return S_OK;
}

HRESULT CSector::Start()
{
	return S_OK;
}

UINT CSector::Update(const float _fDeltaTime)
{
	CGameObject::Update(_fDeltaTime);
	m_pTransform->UpdateTransform();
	return OBJ_NOENVET;
}

UINT CSector::LateUpdate(const float _fDeltaTime)
{
	CGameObject::LateUpdate(_fDeltaTime);

	return OBJ_NOENVET;
}

HRESULT CSector::Render()
{
	if (FAILED(CGameObject::Render()))
		return E_FAIL;
	m_pTransform->UpdateWorld();

	m_pMeshRenderer->Render();
	return S_OK;
}

CGameObject * CSector::Clone()
{
	CSector* pInstance = new CSector(*this);
	return pInstance;
}

CSector * CSector::Create()
{
	CSector* pInstance = new CSector();
	return pInstance;
}

void CSector::Free()
{
	CGameObject::Free();
}
/*addGameObject 하면 반환 된 오브젝트 주소로 반드시 이거 호출해서 네임 세팅*/
void CSector::SetSectorName(const TSTRING & _sSectorName)
{
	m_sSectorName = _sSectorName;
}
