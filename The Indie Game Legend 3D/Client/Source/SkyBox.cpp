#include "stdafx.h"
#include "..\Header\SkyBox.h"
#include "Camera.h"
CSkyBox::CSkyBox()
	: m_pMeshRenderer(nullptr)
	, m_pCamera(nullptr)
{
}

CSkyBox::CSkyBox(const CSkyBox & _rOther)
	:CGameObject(_rOther)
{
}

void CSkyBox::Free()
{
	CGameObject::Free();
	SafeRelease(m_pCamera);
}

CSkyBox * CSkyBox::Create()
{
	CSkyBox* pInstance = new CSkyBox;

	if (FAILED(pInstance->InitializePrototype()))
	{
		delete pInstance;
		return nullptr;
	}

	return pInstance;
}

CGameObject * CSkyBox::Clone()
{
	CSkyBox* pClone = new CSkyBox(*this);
	return pClone;
}

HRESULT CSkyBox::InitializePrototype()
{
	CGameObject::InitializePrototype();
	return S_OK;
}

HRESULT CSkyBox::Awake()
{
	CGameObject::Awake();

	m_pMeshRenderer = (CMeshRenderer*)AddComponent<CMeshRenderer>();
	m_pMeshRenderer->SetMesh(TEXT("SkyBox"));

	m_pTransform->Set_Scale(D3DXVECTOR3(5.f, 5.f, 5.f));

	m_eRenderID = ERenderID::Priority;
	return S_OK;
}

HRESULT CSkyBox::Start()
{
	CGameObject::Start();

	m_pCamera = (CCamera*)FindGameObjectOfType<CCamera>();
	SafeAddRef(m_pCamera);
	return S_OK;
}

UINT CSkyBox::Update(const float _fDeltaTime)
{
	CGameObject::Update(_fDeltaTime);

	m_pTransform->Set_Position(m_pCamera->Get_Camera().vEye);
	m_pTransform->UpdateTransform();
	return 0;
}

UINT CSkyBox::LateUpdate(const float _fDeltaTime)
{
	CGameObject::LateUpdate(_fDeltaTime);
	return 0;
}

HRESULT CSkyBox::Render()
{
	CGameObject::Render();
	m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransform->Get_WorldMatrix());
	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	m_pMeshRenderer->Render();
	m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	return S_OK;
}

