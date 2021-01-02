#include "stdafx.h"
#include "..\Header\TripleBullet.h"
#include "BulletSpark.h"


CTripleBullet::CTripleBullet()
{
}

CTripleBullet::CTripleBullet(const CTripleBullet & _rOther)
	:CBullet(_rOther)
{
}

HRESULT CTripleBullet::InitializePrototype()
{
	if (FAILED(CBullet::InitializePrototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CTripleBullet::Awake()
{
	CBullet::Awake();

	m_sName = L"PlayerBullet";

	return S_OK;
}

HRESULT CTripleBullet::Start()
{

	m_pTexturePool = CTexturePoolManager::GetInstance()->GetTexturePool(TEXT("Bullet"));
	SafeAddRef(m_pTexturePool);
	m_pMeshRenderer->SetTexture(0, m_pTexturePool->GetTexture(TEXT("NormalBullet"))[0]);
	m_fMoveSpeed = 100.f;

	//아직 충돌 이외의 삭제 처리가 필요 함
	m_fLive = 5.f;

	return S_OK;
}

UINT CTripleBullet::Update(const float _fDeltaTime)
{
	if (m_bDead)
		return OBJ_DEAD;
	CMsgManager::GetInstance()->Freeze(&_fDeltaTime);

	m_pTransform->Add_Position(m_vDiraction* m_fMoveSpeed * _fDeltaTime);
	m_pTransform->UpdateTransform();
	m_pTransform->UpdateWorld();

	return OBJ_NOENVET;
}

UINT CTripleBullet::LateUpdate(const float _fDeltaTime)
{
	CBullet::LateUpdate(_fDeltaTime);
	CMsgManager::GetInstance()->Freeze(&_fDeltaTime);

	m_fLivetime += _fDeltaTime;
	if (m_fLivetime >= m_fLive)
		m_bDead = true;

	return OBJ_NOENVET;
}

HRESULT CTripleBullet::Render()
{
	if (FAILED(CGameObject::Render()))
		return E_FAIL;

	IsBillboarding();
	m_pTransform->UpdateWorld();
	m_pMeshRenderer->Render();

	return S_OK;
}

CGameObject * CTripleBullet::Clone()
{
	CGameObject* pIns = new CTripleBullet(*this);
	return pIns;
}

HRESULT CTripleBullet::Fire()
{
	CBullet::Fire();
	_matrix matRotY;
	float	fAngle = 15.f;
	_vector vDir;
	CTripleBullet* pBullet = (CTripleBullet*)AddGameObject<CTripleBullet>();

	D3DXMatrixRotationY(&matRotY, D3DXToRadian(fAngle));
	D3DXVec3TransformNormal(&vDir, &m_vDiraction, &matRotY);
	D3DXVec3Normalize(&vDir, &vDir);
	pBullet->Fire(m_pTransform->Get_Position(), vDir);

	pBullet = (CTripleBullet*)AddGameObject<CTripleBullet>();
	D3DXMatrixRotationY(&matRotY, D3DXToRadian(-fAngle));
	D3DXVec3TransformNormal(&vDir, &m_vDiraction, &matRotY);
	D3DXVec3Normalize(&vDir, &vDir);
	pBullet->Fire(m_pTransform->Get_Position(), vDir);
	return S_OK;
}

HRESULT CTripleBullet::Fire(_vector _Pos, _vector _vDir)
{
	CBullet::Fire();
	m_pTransform->Set_Position(_Pos);
	m_vDiraction = _vDir;
	return S_OK;
}

void CTripleBullet::Free()
{
	CBullet::Free();
}

CTripleBullet * CTripleBullet::Create()
{
	CTripleBullet* pIns = new CTripleBullet();
	return pIns;
}

void CTripleBullet::OnCollision(CGameObject * _pGameObject)
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
