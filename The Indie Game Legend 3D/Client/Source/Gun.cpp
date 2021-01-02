#include "stdafx.h"
#include "..\Header\Gun.h"
#include "PlayerCamera.h"

CGun::CGun()
	: m_pPlayerCamera(nullptr)
	, m_eState(CGun::STATE::NONE)
{
}

CGun::CGun(const CGun & _rOther)
	: CGameObject(_rOther)
	, m_pPlayerCamera(nullptr)
	, m_eState(CGun::STATE::NONE)
{
}

void CGun::Free()
{
	CGameObject::Free();
	SafeRelease(m_pPlayerCamera);
}

CGun * CGun::Create()
{
	CGun* pInstance = new CGun;
	if (FAILED(pInstance->InitializePrototype()))
	{
		SafeRelease(pInstance);
		return nullptr;
	}
	return pInstance;
}

CGameObject * CGun::Clone()
{
	CGun* pClone = new CGun(*this);
	return pClone;
}

HRESULT CGun::InitializePrototype()
{
	CGameObject::InitializePrototype();
	return S_OK;
}

HRESULT CGun::Awake()
{
	CGameObject::Awake();
	m_pMeshRenderer = (CMeshRenderer*)AddComponent<CMeshRenderer>();
	m_pTransform->Set_Scale(D3DXVECTOR3(1.f, 1.f, 1.f));
	m_pTransform->Set_Position(D3DXVECTOR3(1.09485126f, -2.05116796f, 1.58047402f));
	//m_pTransform->Set_Position(D3DXVECTOR3(0.f, 0.f, 0.f));
	m_pTransform->Add_RotationY(180.f);
	m_eRenderID = ERenderID::NoAlpha;
	return S_OK;
}

HRESULT CGun::Start()
{
	CGameObject::Start();
	m_pMeshRenderer->SetMesh(TEXT("Gun"));

	m_pPlayerCamera = (CPlayerCamera*)FindGameObjectOfType<CPlayerCamera>();
	SafeAddRef(m_pPlayerCamera);
	m_pTransform->SetParent(m_pPlayerCamera);
	return S_OK;
}

UINT CGun::Update(const float _fDeltaTime)
{
	CGameObject::Update(_fDeltaTime);

	//if (GetAsyncKeyState(VK_NUMPAD4) & 0x8000)
	//{
	//	m_pTransform->Add_Position(D3DXVECTOR3(-1.f * _fDeltaTime, 0.f, 0.f));
	//}
	//else if (GetAsyncKeyState(VK_NUMPAD6) & 0x8000)
	//{
	//	m_pTransform->Add_Position(D3DXVECTOR3(1.f * _fDeltaTime, 0.f, 0.f));
	//}

	//if (GetAsyncKeyState(VK_NUMPAD8) & 0x8000)
	//{
	//	m_pTransform->Add_Position(D3DXVECTOR3(0.f, 1.f * _fDeltaTime, 0.f));
	//}
	//else if (GetAsyncKeyState(VK_NUMPAD5) & 0x8000)
	//{
	//	m_pTransform->Add_Position(D3DXVECTOR3(0.f, -1.f * _fDeltaTime, 0.f));
	//}

	//if (GetAsyncKeyState(VK_NUMPAD1) & 0x8000)
	//{
	//	m_pTransform->Add_Position(D3DXVECTOR3(0.f, 0.f, 1.f * _fDeltaTime));
	//}
	//else if (GetAsyncKeyState(VK_NUMPAD3) & 0x8000)
	//{
	//	m_pTransform->Add_Position(D3DXVECTOR3(0.f, 0.f, -1.f * _fDeltaTime));
	//}

	////if (GetAsyncKeyState(VK_NUMPAD7) & 0x8000)
	////{
	////	m_pTransform->Add_RotationY(5.f*_fDeltaTime);
	////}
	////else if (GetAsyncKeyState(VK_NUMPAD9) & 0x8000)
	////{
	////	m_pTransform->Add_RotationY(-5.f*_fDeltaTime);
	////}


	//if (GetAsyncKeyState(VK_NUMPAD7) & 0x8000)
	//{
	//	SetActive(true);
	//}
	//else if (GetAsyncKeyState(VK_NUMPAD9) & 0x8000)
	//{
	//	SetActive(false);
	//}
	return 0;
}

UINT CGun::LateUpdate(const float _fDeltaTime)
{
	CGameObject::LateUpdate(_fDeltaTime);
	Animate(_fDeltaTime);
	m_pTransform->UpdateTransform();
	return 0;
}

HRESULT CGun::Render()
{
	CGameObject::Render();
	m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransform->Get_WorldMatrix());
	m_pMeshRenderer->Render();
	return S_OK;
}

void CGun::SetState(CGun::STATE _eState)
{
	if(m_eState != CGun::FIRE)
	m_eState = _eState;
}

void CGun::Animate(const float _fDeltaTime)
{
	switch (m_eState)
	{
	case CGun::NONE:
		break;
	case CGun::MOVE:
		Move(_fDeltaTime);
		break;
	case CGun::FIRE:
		
		break;
	default:
		break;
	}

	if(true == m_bFire)
		Fire(_fDeltaTime);
}

void CGun::Move(const float _fDeltaTime)
{
	static float fTime = 0.f;
	static bool bInverse = false;

	fTime += 0.025f * (bInverse ? -1.f : 1.f);

	static D3DXVECTOR3 vStart = m_pTransform->Get_Position();
	static D3DXVECTOR3 vVia = m_pTransform->Get_Position() + D3DXVECTOR3(0.15f, -0.1f, 0.f);
	static D3DXVECTOR3 vEnd = m_pTransform->Get_Position() + D3DXVECTOR3(0.15f, 0.f, 0.f);

	D3DXVECTOR3 vResult = D3DXVECTOR3(0.f, 0.f, 0.f);

	vResult = (1.f - fTime) * (1.f - fTime) * vStart + 2 * fTime * (1 - fTime) * vVia + fTime * fTime * vEnd;

	m_pTransform->Set_Position(vResult);

	if (fTime > 1.f || fTime < 0.f)
	{
		fTime = (fTime > 1.f ? 1.f : 0.f);
		bInverse = !bInverse;
	}
}

void CGun::Fire(const float _fDeltaTime)
{
	static float fTime = 0.f;
	static float fDegree = 0.f;

	static float fStart = 0.f;
	static float fVia = 20.f;
	static float fEnd = 45.f;

	static bool bInverse = false;
	
	fTime += 0.2f * (bInverse ? -1.f : 1.f);

	float fResult = (1 - fTime) * fStart + fTime * fEnd;
	fResult = (1.f - fTime) * (1.f - fTime) * fStart + 2 * fTime * (1 - fTime) * fVia + fTime * fTime * fEnd;

	D3DXVECTOR3 vRotation = m_pTransform->Get_TransformDesc().vRotation;
	vRotation.x = fResult;
	m_pTransform->Set_Rotation(vRotation);

	if (fTime > 1.f || fTime < 0.f)
	{
		fTime = (fTime > 1.f ? 1.f : 0.f);
		bInverse = !bInverse;

		if (0.f == fTime)
		{
			m_bFire = false;
			vRotation.x = 0.f;
			m_pTransform->Set_Rotation(vRotation);
		}
	}
}

bool CGun::SetFire()
{
	if (true == m_bFire)
		return false;
	m_bFire = true;
	return true;
}

void CGun::SetActive(const bool _bActive)
{
	D3DMATERIAL9 tMaterial;

	tMaterial = m_pMeshRenderer->GetMaterial(0);

	if (_bActive)
		tMaterial.Diffuse = D3DCOLORVALUE{ 0.f, 1.f, 0.94999998807907f, 1.f };
	else
		tMaterial.Diffuse = { 0.80000001192093f, 0.80000001192093f, 0.80000001192093f,1.f };

	m_pMeshRenderer->SetMaterial(0, tMaterial);
}

