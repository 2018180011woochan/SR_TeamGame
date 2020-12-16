#include "stdafx.h"
#include "..\Header\PlayerBullet.h"
#include "MeshRenderer.h"
#include "PlayerCamera.h"
#include "BulletSpawn.h"

CPlayerBullet::CPlayerBullet()
	:m_eBulletType(EBulletType::MaxCount)
{
}

CPlayerBullet::CPlayerBullet(const CPlayerBullet & _rOther)
	:CBullet(_rOther)
	,m_eBulletType(_rOther.m_eBulletType)
{
}

void CPlayerBullet::Set_Type(const EBulletType & _eBulletType)
{
	m_eBulletType = _eBulletType;

	SafeRelease(m_pTexturePool);
	switch (m_eBulletType)
	{
	case EBulletType::Normal:
		m_pTexturePool = CTexturePoolManager::GetInstance()->GetTexturePool(TEXT("Bullet"));
		m_pMeshRenderer->SetTexture(0, m_pTexturePool->GetTexture(TEXT("NormalBullet"))[0]);
		m_fMoveSpeed = 100.f;
		break;
	default:
		PrintLog(L"worring", L"default Input by CPlayerBullet::Set_Type func");
		break;
	}
	if (m_pTexturePool != nullptr)
	{
		SafeAddRef(m_pTexturePool);
	}
	else
	{
		PrintLog(L"worring",L"Failed load Texture");
		return;
	}
}


HRESULT CPlayerBullet::Fire()
{
	CPlayerCamera* pCamera = (CPlayerCamera*)FindGameObjectOfType<CPlayerCamera>();
	CBulletSpawn* pSpawn = (CBulletSpawn*)FindGameObjectOfType<CBulletSpawn>();

	//Test
	auto  pSpawnTrans = ((CTransform*)pSpawn->GetComponent<CTransform>());
	CAMERA_DESC CameraDesc = pCamera->Get_Camera();
	m_vDiraction = CameraDesc.vAt - pSpawnTrans->Get_Position();
	D3DXVec3Normalize(&m_vDiraction, &m_vDiraction);
	m_pTransform->Set_Position(pSpawnTrans->Get_Position());

	//m_vDiraction = CameraDesc.vAt - pSpawnDecs->Get_Position();
	//m_pTransform->Set_Position(pSpawnDecs->Get_Position());

	return S_OK;
}

HRESULT CPlayerBullet::InitializePrototype()
{
	m_fTimeCheck = 0.f;
	m_fLiveTiem = 0.f;
	m_fMoveSpeed = 0;
	m_vDiraction = vZero;
	return S_OK;
}

HRESULT CPlayerBullet::Awake()
{
	CBullet::Awake();
	m_pTransform->Set_Scale(_vector(1, 1, 1));
	m_pMeshRenderer = (CMeshRenderer*)AddComponent<CMeshRenderer>();
	m_pMeshRenderer->SetMesh(TEXT("Quad"));
	m_fLiveTiem = 5.f;
	m_eRenderID = ERenderID::Alpha;

	return S_OK;
}

HRESULT CPlayerBullet::Start()
{

	return S_OK;
}

UINT CPlayerBullet::Update(const float _fDeltaTime)
{
	m_fTimeCheck += _fDeltaTime;
	if (m_fLiveTiem < m_fTimeCheck)
	{
		return OBJ_DEAD;
	}
	m_pTransform->Add_Position(m_vDiraction* m_fMoveSpeed * _fDeltaTime);
	m_pTransform->UpdateTransform();
	m_pTransform->UpdateWorld();

	return OBJ_NOENVET;
}

UINT CPlayerBullet::LateUpdate(const float _fDeltaTime)
{
	CBullet::LateUpdate(_fDeltaTime);
	return OBJ_NOENVET;
}

HRESULT CPlayerBullet::Render()
{
	if (FAILED(CGameObject::Render()))
		return E_FAIL;

	IsBillboarding();
	m_pTransform->UpdateWorld();
	m_pMeshRenderer->Render();
	return S_OK;
}

CPlayerBullet * CPlayerBullet::Create()
{
	CPlayerBullet* pInstance = new CPlayerBullet();
	return pInstance;
}

void CPlayerBullet::Free()
{
	SafeRelease(m_pTexturePool);
	CBullet::Free();
}

CGameObject * CPlayerBullet::Clone()
{
	CGameObject* pInstance = new CPlayerBullet(*this);
	return pInstance;
}