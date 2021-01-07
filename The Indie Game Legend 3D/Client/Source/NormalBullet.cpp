#include "stdafx.h"
#include "..\Header\NormalBullet.h"
#include "BulletSpark.h"


CNormalBullet::CNormalBullet()
{
}

CNormalBullet::CNormalBullet(const CNormalBullet & _rOther)
	:CBullet(_rOther)
{
}

HRESULT CNormalBullet::InitializePrototype()
{

	if (FAILED(CBullet::InitializePrototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CNormalBullet::Awake()
{
	CBullet::Awake();

	m_sName = L"PlayerBullet";

	return S_OK;
}

HRESULT CNormalBullet::Start()
{

	m_pTexturePool = CTexturePoolManager::GetInstance()->GetTexturePool(TEXT("Bullet"));
	SafeAddRef(m_pTexturePool);
	m_pMeshRenderer->SetTexture(0, m_pTexturePool->GetTexture(TEXT("NormalBullet"))[0]);
	m_fMoveSpeed = 100.f;

	//아직 충돌 이외의 삭제 처리가 필요 함
	m_fLive = 5.f;

	return S_OK;
}

UINT CNormalBullet::Update(const float _fDeltaTime)
{
	if (m_bDead)
		return OBJ_DEAD;
	CMsgManager::GetInstance()->Freeze(&_fDeltaTime);

	m_pTransform->Add_Position(m_vDiraction* m_fMoveSpeed * _fDeltaTime);
	m_pTransform->UpdateTransform();
	m_pTransform->UpdateWorld();

	return OBJ_NOENVET;
}

UINT CNormalBullet::LateUpdate(const float _fDeltaTime)
{
	CBullet::LateUpdate(_fDeltaTime);
	CMsgManager::GetInstance()->Freeze(&_fDeltaTime);

	m_fLivetime += _fDeltaTime;
	if (m_fLivetime >= m_fLive)
		m_bDead = true;

	return OBJ_NOENVET;
}

HRESULT CNormalBullet::Render()
{
	if (FAILED(CGameObject::Render()))
		return E_FAIL;

	IsBillboarding();
	m_pTransform->UpdateWorld();
	m_pMeshRenderer->Render();

	return S_OK;
}

CGameObject * CNormalBullet::Clone()
{
	CGameObject* pIns = new CNormalBullet(*this);
	return pIns;
}

//HRESULT CNormalBullet::Fire()
//{
//	return CBullet::Fire();
//}

void CNormalBullet::Free()
{
	CBullet::Free();
}

CNormalBullet * CNormalBullet::Create()
{
	CNormalBullet* pIns = new CNormalBullet();
	return pIns;
}

void CNormalBullet::OnCollision(CGameObject * _pGameObject)
{
	if (L"Monster" == _pGameObject->GetName() || L"Obstacle" == _pGameObject->GetName()
		|| L"Floor" == _pGameObject->GetName() || L"Wall" == _pGameObject->GetName())
	{
		CBulletSpark* pEffect = nullptr;
		pEffect = (CBulletSpark*)AddGameObject<CBulletSpark>();
		pEffect->SetPosition(m_pTransform->Get_Position());
		m_bDead = true;
	}
}
