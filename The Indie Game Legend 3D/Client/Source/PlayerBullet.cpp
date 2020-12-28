#include "stdafx.h"
#include "..\Header\PlayerBullet.h"
#include "MeshRenderer.h"


//effect
#include "BulletSpark.h"
#include "Explosion.h"
#include "ExplosionBlue.h"
CPlayerBullet::CPlayerBullet()
	:m_eBulletType(EWeaponType::Normal)
{
}

CPlayerBullet::CPlayerBullet(const CPlayerBullet & _rOther)
	:CBullet(_rOther)
	,m_eBulletType(_rOther.m_eBulletType)
{
}

void CPlayerBullet::Set_Type(const EWeaponType & _eBulletType)
{
	m_eBulletType = _eBulletType;

	SafeRelease(m_pTexturePool);
	switch (m_eBulletType)
	{
	case EWeaponType::Big:
		m_pTexturePool = CTexturePoolManager::GetInstance()->GetTexturePool(TEXT("Bullet"));
		m_pMeshRenderer->SetTexture(0, m_pTexturePool->GetTexture(TEXT("BigBullet"))[0]);
		m_fMoveSpeed = 60.f;
		m_pTransform->Set_Scale(_vector(2, 2, 2));
		break;
	case EWeaponType::Normal:
		m_pTexturePool = CTexturePoolManager::GetInstance()->GetTexturePool(TEXT("Bullet"));
		m_pMeshRenderer->SetTexture(0, m_pTexturePool->GetTexture(TEXT("NormalBullet"))[0]);
//		m_fMoveSpeed = 100.f;
		//test
		m_fMoveSpeed = 100.f;
		break;
	case EWeaponType::Multiple:
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
		_matrix matRotY;

		CPlayerBullet* pBullet = (CPlayerBullet*)AddGameObject<CPlayerBullet>();
		pBullet->Set_Type(EWeaponType::Normal);
		_vector vDir;
		
		D3DXMatrixRotationY(&matRotY, D3DXToRadian(15.f));
		D3DXVec3TransformNormal(&vDir, &m_vDiraction, &matRotY);
		D3DXVec3Normalize(&vDir, &vDir);

		pBullet->Fire(vDir);

		pBullet = (CPlayerBullet*)AddGameObject<CPlayerBullet>();
		pBullet->Set_Type(EWeaponType::Normal);
		D3DXMatrixRotationY(&matRotY, D3DXToRadian(-15.f));
		D3DXVec3TransformNormal(&vDir, &m_vDiraction, &matRotY);
		D3DXVec3Normalize(&vDir, &vDir);
		pBullet->Fire(vDir);
	
	return S_OK;
}

HRESULT CPlayerBullet::Fire(const _vector _Dir)
{
	//CBulletSpawn* pSpawn = (CBulletSpawn*)FindGameObjectOfType<CBulletSpawn>();
	//auto  pSpawnTrans = ((CTransform*)pSpawn->GetComponent<CTransform>());
	//m_vDiraction = _Dir;
	//D3DXVec3Normalize(&m_vDiraction, &m_vDiraction);
	//m_pTransform->Set_Position(pSpawnTrans->Get_WorldPosition());

	return S_OK;
}


HRESULT CPlayerBullet::InitializePrototype()
{
	

	return S_OK;
}

HRESULT CPlayerBullet::Awake()
{
	CBullet::Awake();



	return S_OK;
}

HRESULT CPlayerBullet::Start()
{
	CBullet::Start();

	return S_OK;
}

UINT CPlayerBullet::Update(const float _fDeltaTime)
{
	if (m_bDead)
		return OBJ_DEAD;


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

void CPlayerBullet::OnCollision(CGameObject * _pGameObject)
{
	if (L"Monster" == _pGameObject->GetName()
		|| L"Obstacle" == _pGameObject->GetName())
	{

		if (m_eBulletType == EWeaponType::Big)
		{
			CExplosionBlue* pEffect = nullptr;
			pEffect = (CExplosionBlue*)AddGameObject<CExplosionBlue>();
			pEffect->SetPos(m_pTransform->Get_Position());
			m_bDead = true;
		}
		else
		{
			CBulletSpark* pEffect = nullptr;
			pEffect = (CBulletSpark*)AddGameObject<CBulletSpark>();
			pEffect->SetPosition(m_pTransform->Get_Position());
			m_bDead = true;
			
		}

	}
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
