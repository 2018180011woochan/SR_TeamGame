#include "stdafx.h"
#include "..\Header\Bullet.h"
#include "Camera.h"


CBullet::CBullet()
{
}

CBullet::CBullet(const CBullet & _rOther)
	:CGameObject(_rOther)
	,m_fTimeCheck(_rOther.m_fTimeCheck)
	,m_fLiveTiem(_rOther.m_fLiveTiem)
	,m_fMoveSpeed(_rOther.m_fMoveSpeed)
	, m_vDiraction(_rOther.m_vDiraction)
{
}

HRESULT CBullet::InitializePrototype()
{

	return S_OK;
}

HRESULT CBullet::Awake()
{
	CGameObject::Awake();
	return S_OK;
}

HRESULT CBullet::Start()
{
	return S_OK;
}

UINT CBullet::Update(const float _fDeltaTime)
{
	return OBJ_NOENVET;
}

UINT CBullet::LateUpdate(const float _fDeltaTime)
{
	CGameObject::LateUpdate(_fDeltaTime);
	return OBJ_NOENVET;
}

HRESULT CBullet::Render()
{
	return S_OK;
}

HRESULT CBullet::IsBillboarding()
{
	CCamera* pCamera = (CCamera*)FindGameObjectOfType<CCamera>();
	if (pCamera == nullptr)
		return E_FAIL;

	_matrix matBill, matView;
	D3DXMatrixIdentity(&matBill);
	matView = pCamera->Get_Camera().matView;

	memcpy(&matBill.m[0][0], &matView.m[0][0], sizeof(_vector));
	memcpy(&matBill.m[1][0], &matView.m[1][0], sizeof(_vector));
	memcpy(&matBill.m[2][0], &matView.m[2][0], sizeof(_vector));

	D3DXMatrixInverse(&matBill, 0, &matBill);

	_matrix matWorld = m_pTransform->Get_WorldMatrix();
	_matrix NewWorld = matBill * matWorld;

	m_pTransform->Set_WorldMatrix(NewWorld);
	return S_OK;
}

void CBullet::Free()
{
	CGameObject::Free();
}

void CBullet::OnCollision(CGameObject * _pGameObject)
{
}
