#include "stdafx.h"
#include "..\Header\BigBullet.h"
#include "ExplosionBlue.h"
#include "SoundMgr.h"


CBigBullet::CBigBullet()
{
}

CBigBullet::CBigBullet(const CBigBullet & _rOther)
	:CBullet(_rOther)
{
}


HRESULT CBigBullet::InitializePrototype()
{

	if (FAILED(CBullet::InitializePrototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CBigBullet::Awake()
{
	CBullet::Awake();

	m_sName = L"PlayerBullet";

	return S_OK;
}

HRESULT CBigBullet::Start()
{
	m_pTexturePool = CTexturePoolManager::GetInstance()->GetTexturePool(TEXT("Bullet"));
	m_pMeshRenderer->SetTexture(0, m_pTexturePool->GetTexture(TEXT("BigBullet"))[0]);
	m_fMoveSpeed = 100.f;
	m_pTransform->Set_Scale(_vector(4, 4, 4));
	SafeAddRef(m_pTexturePool);

	//아직 충돌 이외의 삭제 처리가 필요 함
	m_fLive = 5.f;

	return S_OK;
}

UINT CBigBullet::Update(const float _fDeltaTime)
{
	if (m_bDead)
		return OBJ_DEAD;
	CMsgManager::GetInstance()->Freeze(&_fDeltaTime);

	m_pTransform->Add_Position(m_vDiraction* m_fMoveSpeed * _fDeltaTime);
	m_pTransform->UpdateTransform();
	m_pTransform->UpdateWorld();

	return OBJ_NOENVET;
}

UINT CBigBullet::LateUpdate(const float _fDeltaTime)
{
	CBullet::LateUpdate(_fDeltaTime);
	CMsgManager::GetInstance()->Freeze(&_fDeltaTime);

	m_fLivetime += _fDeltaTime;
	if (m_fLivetime >= m_fLive)
		m_bDead = true;
	return OBJ_NOENVET;
}

HRESULT CBigBullet::Render()
{
	if (FAILED(CGameObject::Render()))
		return E_FAIL;

	IsBillboarding();
	m_pTransform->UpdateWorld();
	m_pMeshRenderer->Render();

	return S_OK;
}

CGameObject * CBigBullet::Clone()
{
	CGameObject* pIns = new CBigBullet(*this);
	return pIns;
}

HRESULT CBigBullet::Fire()
{
	CBullet::Fire();
	return S_OK;
}

HRESULT CBigBullet::Fire(const _vector & _vPos, const _vector & _vDir)
{
	m_vDiraction = _vDir;
	m_pTransform->Set_Position(_vPos);
	return S_OK;
}

void CBigBullet::Free()
{
	CBullet::Free();
}

CBigBullet * CBigBullet::Create()
{
	CBigBullet* pIns = new CBigBullet();
	return pIns;
}

void CBigBullet::OnCollision(CGameObject * _pGameObject)
{
	if (L"Monster" == _pGameObject->GetName() || L"Obstacle" == _pGameObject->GetName()
		|| L"Floor" == _pGameObject->GetName() || L"Wall" == _pGameObject->GetName())
	{
		CExplosionBlue* pEffect = nullptr;
		pEffect = (CExplosionBlue*)AddGameObject<CExplosionBlue>();
		pEffect->SetPos(m_pTransform->Get_Position());
		CSoundMgr::GetInstance()->Play(L"sfxExplode.wav", CSoundMgr::Player_Bullet);
		m_bDead = true;
	}
}
