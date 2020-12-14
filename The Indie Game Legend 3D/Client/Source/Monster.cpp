#include "stdafx.h"
#include "..\Header\Monster.h"
#include "Camera.h"

CMonster::CMonster()
{
}

CMonster::CMonster(const CMonster & other)
	: CGameObject(other)
{
}

HRESULT CMonster::InitializePrototype()
{
	if (FAILED(CGameObject::InitializePrototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMonster::Awake()
{
	if (FAILED(CGameObject::Awake()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMonster::Start()
{
	if (FAILED(CGameObject::Start()))
		return E_FAIL;

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

	_matrix matWorld = m_pTransformCom->Get_WorldMatrix();

	_matrix NewWorld = matBillY * matWorld;
	m_pTransformCom->Set_WorldMatrix(NewWorld);

	return S_OK;
}

