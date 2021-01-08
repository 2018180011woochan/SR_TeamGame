#include "stdafx.h"
#include "..\Header\Monster.h"
#include "Camera.h"


CMonster::CMonster()
	:m_pBossHP(nullptr)
{
}

CMonster::CMonster(const CMonster & other)
	: CGameObject(other)
	, m_bHit(false)
	, m_fHitDelay(0.6f)
	, m_fHitDelayTime(0.f)
	, m_pBossHP(other.m_pBossHP)
{
}

HRESULT CMonster::InitializePrototype()
{
	if (FAILED(CGameObject::InitializePrototype()))
		return E_FAIL;

	m_sName = L"Monster";

	return S_OK;
}

HRESULT CMonster::Awake()
{
	if (FAILED(CGameObject::Awake()))
		return E_FAIL;
	m_fHitDelay = 0.5f;

	//  [1/6/2021 wades]
#ifdef _DEBUG
	SetMonsterPos();
#endif // !_DEBUG
	return S_OK;
}

HRESULT CMonster::Start()
{
	if (FAILED(CGameObject::Start()))
		return E_FAIL;

	SetMonsterPos();

	return S_OK;
}

UINT CMonster::Update(const float _fDeltaTime)
{
	CGameObject::Update(_fDeltaTime);
	

	return _uint();
}

UINT CMonster::LateUpdate(const float _fDeltaTime)
{
	CGameObject::LateUpdate(_fDeltaTime);
	if (m_bHit)
		m_fHitDelayTime += _fDeltaTime;


	if (m_bHit  && m_fHitDelay < m_fHitDelayTime)
	{
		m_bHit = false;
		m_fHitDelayTime = 0.f;
	}

	IsBillboarding();
	return _uint();
}

HRESULT CMonster::Render()
{
	if (FAILED(CGameObject::Render()))
		return E_FAIL;
	return S_OK;
}

HRESULT CMonster::IsBillboarding()
{
	CCamera* pCamera = (CCamera*)FindGameObjectOfType<CCamera>();
	if (pCamera == nullptr)
		return E_FAIL;

	_matrix matBillY, matView;
	D3DXMatrixIdentity(&matBillY);
	matView = pCamera->Get_Camera().matView;

	matBillY._11 = matView._11;
	matBillY._13 = matView._13;
	matBillY._31 = matView._31;
	matBillY._33 = matView._33;

	D3DXMatrixInverse(&matBillY, 0, &matBillY);

	_matrix matWorld = m_pTransform->Get_WorldMatrix();
	
	D3DXVECTOR3 vPos = m_pTransform->Get_Position();
	D3DXVECTOR3 vScale = m_pTransform->Get_TransformDesc().vScale;
	D3DXVECTOR3 vRot = m_pTransform->Get_TransformDesc().vRotation;

	D3DXMATRIX matScale, matRX, matRY, matRZ, matTrans;

	D3DXMatrixScaling(&matScale, vScale.x, vScale.y, vScale.z);
	D3DXMatrixRotationX(&matRX, D3DXToRadian(vRot.x));
	D3DXMatrixRotationY(&matRY, D3DXToRadian(vRot.y));
	D3DXMatrixRotationZ(&matRZ, D3DXToRadian(vRot.z));
	D3DXMatrixTranslation(&matTrans, vPos.x, vPos.y, vPos.z);



	_matrix NewWorld = matScale * matRX * matRY * matRZ * matBillY * matTrans;
	m_pTransform->Set_WorldMatrix(NewWorld);

	return S_OK;
}

void CMonster::sfxMetalHit()
{
	int nSfx = rand() % 2;
	if (nSfx)
		CSoundMgr::GetInstance()->Play(L"sfxMetalHit1.mp3", CSoundMgr::MonsterHitM);
	else
		CSoundMgr::GetInstance()->Play(L"sfxMetalHit1.mp3", CSoundMgr::MonsterHitM);
}

void CMonster::OnCollision(CGameObject * _pGameObject)
{
}

void CMonster::SetMonsterPos()
{
	m_vecStartPos = m_pTransform->Get_Position();
	//  [1/6/2021 wades]
	m_vecStartPos.y += 1.f;
}

void CMonster::ResetMonsterPos()
{
	m_pTransform->Set_Position(m_vecStartPos);
}

void CMonster::OnEnable()
{
	ResetMonsterPos();
}

void CMonster::AddHp(const _int & _nHp)
{
	m_iHP += _nHp;
	m_iHP = CLAMP(m_iHP, 0, m_iMaxHP);
	if (m_iHP < 1)
	{
		m_bDead = true;
		m_bDelete = true;
	}

}

