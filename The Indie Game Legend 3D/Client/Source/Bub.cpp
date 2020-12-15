#include "stdafx.h"
#include "Bub.h"
#include "VIBuffer_Rect.h"
#include "MeshRenderer.h"
#include "Transform.h"
#include "Player.h"
CBub::CBub()
	:m_pTexturePool(nullptr)
{
}

CBub::CBub(const CBub & other)
	: CGameObject(other)
{
}

HRESULT CBub::InitializePrototype()
{
	if (FAILED(CGameObject::InitializePrototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CBub::Awake()
{
	if (FAILED(CGameObject::Awake()))
		return E_FAIL;

	m_pDevice->SetRenderState(D3DRS_LIGHTING, false);

	D3DXMATRIX matView, matProj;
	/* 뷰행렬 만드는 함수 */
	D3DXMatrixLookAtLH(
		&matView, /* 반환 값 */
		&D3DXVECTOR3(0.f,100.f, -5.f), /* 월드상 카메라 위치 Eye */
		&D3DXVECTOR3(0.f, 0.f, 0.f), /* 카메라가 바라보는 지점 At */
		&D3DXVECTOR3(0.f, 1.f, 0.f) /* Up벡터 */);

	if (FAILED(m_pDevice->SetTransform(D3DTS_VIEW, &matView)))
		return E_FAIL;

	/* 원근 투영 행렬을 만드는 함수 */
	D3DXMatrixPerspectiveFovLH(
		&matProj, /* 반환 값 */
		D3DXToRadian(90.f), /* 시야각 FovY */
		float(g_nWinCX) / g_nWinCY, /* 종횡비 Aspect */
		1.f, /* 카메라와 근편명과의 Z거리 Near */
		1000.f /* 카메라와 원평면과의 Z거리 Far */);

	if (FAILED(m_pDevice->SetTransform(D3DTS_PROJECTION, &matProj)))
		return E_FAIL;
	/*****************************************************************************************************/
	m_pMeshRenderer = (CMeshRenderer*)AddComponent<CMeshRenderer>();
	m_pMeshRenderer->SetMesh(TEXT("Sector1"));

	m_pTexturePool = CTexturePoolManager::GetInstance()->GetTexturePool(TEXT("Bub"));
	SafeAddRef(m_pTexturePool);

	m_eRenderID = ERenderID::Alpha;



	return S_OK;
}

HRESULT CBub::Start()
{
	CGameObject::Start();

	return S_OK;
}

UINT CBub::Update(const float _fDeltaTime)
{
	CGameObject::Update(_fDeltaTime);
	static UINT nIndex = 0;
	if (GetAsyncKeyState('I') & 0x8000)
	{
		m_bIsTPS = !m_bIsTPS;
	}

	D3DXVECTOR3 vAdd;
	float fSpeed = 10.f;
	if (GetAsyncKeyState('W') & 0x8000)
	{
		vAdd = m_pTransform->Get_Up() * _fDeltaTime * fSpeed;
		m_pTransform->Add_Position(vAdd);
	}
	if (GetAsyncKeyState('S') & 0x8000)
	{
		vAdd = m_pTransform->Get_Up() * -_fDeltaTime * fSpeed;
		m_pTransform->Add_Position(vAdd);
	}
	if (m_bIsTPS)
	{
		D3DXMATRIX matView;
		D3DXVECTOR3 vecDir = D3DXVECTOR3(0.f, -1.f, 1.f);
		D3DXMATRIX matRotY;
		D3DXMatrixRotationY(&matRotY, D3DXToRadian(m_pTransform->Get_TransformDesc().vRotation.y));
		D3DXVec3TransformNormal(&vecDir, &vecDir, &matRotY);
		D3DXVec3Normalize(&vecDir, &vecDir);
		float fDistance = 25.f;

		D3DXVECTOR3 vecEye = m_pTransform->Get_TransformDesc().vPosition - vecDir * fDistance;
		D3DXMatrixLookAtLH(&matView, &vecEye, &(m_pTransform->Get_TransformDesc().vPosition), &D3DXVECTOR3(0.f, 1.f, 0.f));
		m_pDevice->SetTransform(D3DTS_VIEW, &matView);
	}
	else
	{
		D3DXMATRIX matView;
		D3DXVECTOR3 vecDir = D3DXVECTOR3(0.f, 0.f, 1.f);
		D3DXMATRIX matRotY;
		D3DXMatrixRotationY(&matRotY, D3DXToRadian(m_pTransform->Get_TransformDesc().vRotation.y));
		D3DXVec3TransformNormal(&vecDir, &vecDir, &matRotY);
		D3DXVec3Normalize(&vecDir, &vecDir);
		float fDistance = 25.f;

		D3DXVECTOR3 vecPos = m_pTransform->Get_TransformDesc().vPosition;
		vecPos.y += 1.f;
		D3DXVECTOR3 vecLookAt = vecPos + vecDir*fDistance;
		D3DXMatrixLookAtLH(&matView, &(vecPos), &vecLookAt, &D3DXVECTOR3(0.f, 1.f, 0.f));
		m_pDevice->SetTransform(D3DTS_VIEW, &matView);
	}
	return _uint();
}

UINT CBub::LateUpdate(const float _fDeltaTime)
{
	CGameObject::LateUpdate(_fDeltaTime);

	auto pManagement = CManagement::GetInstance();
	if (nullptr == pManagement)
		return 0;
	//// 씬이 아직 없으니 그냥 0번
	//pManagement->AddGameObject<CBub*>(0);
	//AddGameObject<CBullet>();
	return _uint();
}

HRESULT CBub::Render()
{
	/*엔진에서 호출하는 식으로*/
	if (FAILED(CGameObject::Render()))
		return E_FAIL;
	static float fAngle = 0.f;
	if (GetAsyncKeyState('Q') & 0x8000)
	{
		fAngle -= 0.001f;
	}
	if (GetAsyncKeyState('E') & 0x8000)
	{
		fAngle += 0.001f;
	}

	D3DXMATRIX matRot;
	D3DXMatrixRotationY(&matRot, fAngle);
	m_pDevice->SetTransform(D3DTS_WORLD, &matRot);
	m_pMeshRenderer->Render();

	return S_OK;
}

CGameObject * CBub::Clone()
{
	CBub* pClone = new CBub(*this);
	return pClone;
}

CBub * CBub::Create()
{
	CBub* pInstance = new CBub();
	return pInstance;
}

void CBub::Free()
{
	CGameObject::Free();
	SafeRelease(m_pTexturePool);
}
