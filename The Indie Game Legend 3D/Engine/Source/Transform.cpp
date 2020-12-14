#include "Transform.h"


USING(Engine)

CTransform::CTransform(CGameObject * const _pGameObject, LPDIRECT3DDEVICE9 const _pDevice)
	:CComponent(_pGameObject,_pDevice)
{
}

CTransform::CTransform(const CTransform & _rOther)
	:CComponent(_rOther)
{
	PrintLog(L"warring", L"copy transform Danger");
}

HRESULT CTransform::Initialize()
{
	ZeroMemory(&m_tTransformDesc, sizeof(TRANSFORM_DESC));
	D3DXMatrixIdentity(&m_tTransformDesc.matWorld);
	return S_OK;
}

void CTransform::UpdateTransform()
{
	_matrix matScale, matRotX, matRotY, matRotZ, matTrans;
	D3DXMatrixScaling(&matScale, m_tTransformDesc.vScale.x, m_tTransformDesc.vScale.y, m_tTransformDesc.vScale.z);
	D3DXMatrixRotationX(&matRotX, m_tTransformDesc.vRotation.x);
	D3DXMatrixRotationY(&matRotY, m_tTransformDesc.vRotation.y);
	D3DXMatrixRotationZ(&matRotZ, m_tTransformDesc.vRotation.z);
	D3DXMatrixTranslation(&matTrans, m_tTransformDesc.vPosition.x, m_tTransformDesc.vPosition.y, m_tTransformDesc.vPosition.z);

	m_tTransformDesc.matWorld = matScale * matRotX * matRotY *matRotZ * matTrans;
}
/*행렬 세팅용*/
HRESULT CTransform::DeviceSetting()
{
	if (FAILED(m_pDevice->SetTransform(D3DTS_WORLD, &m_tTransformDesc.matWorld)))
		return E_FAIL;

	return S_OK;
}


CTransform * CTransform::Create(CGameObject * const _pGameObject, LPDIRECT3DDEVICE9 const _pDevice)
{
	CTransform* pInstance = new CTransform(_pGameObject, _pDevice);
	if (FAILED(pInstance->Initialize()))
	{
		SafeDelete(pInstance);
		PrintLog(L"Error",L"Failed Transform Create")
	}
	return pInstance;
}

void CTransform::Free()
{
	CComponent::Free();
}

#pragma region Get&Set
const TRANSFORM_DESC & CTransform::Get_TransformDesc() const
{
	return m_tTransformDesc;
}

const _vector CTransform::Get_Right() const
{
	_vector vRight;
	memcpy(&vRight, &m_tTransformDesc.matWorld.m[0][0], sizeof(_vector));
	return vRight;
}

const _vector CTransform::Get_Look() const
{
	_vector vLook;
	memcpy(&vLook, &m_tTransformDesc.matWorld.m[2][0], sizeof(_vector));
	return vLook;
}

const _vector CTransform::Get_Up() const
{
	_vector vUp;
	memcpy(&vUp, &m_tTransformDesc.matWorld.m[1][0], sizeof(_vector));
	return vUp;
}

const _vector CTransform::Get_Position() const
{
	return m_tTransformDesc.vPosition;
}

const _matrix CTransform::Get_WorldMatrix() const
{
	return m_tTransformDesc.matWorld;
}

void CTransform::Set_Position(const _vector & _rPosition)
{
	m_tTransformDesc.vPosition = _rPosition;
}

void CTransform::Add_Position(const _vector & _rPosition)
{
	m_tTransformDesc.vPosition += _rPosition;
}

void CTransform::Set_Rotation(const _vector & _rRotaticon)
{
	m_tTransformDesc.vRotation = _rRotaticon;
}

void CTransform::Add_Rotation(const _vector & _rRotaticon)
{
	m_tTransformDesc.vRotation += _rRotaticon;
}

void CTransform::Add_RotationX(const float & _rRotationX)
{
	m_tTransformDesc.vRotation.x += _rRotationX;
}

void CTransform::Add_RotationY(const float & _rRotationY)
{
	m_tTransformDesc.vRotation.y += _rRotationY;
}

void CTransform::Add_RotationZ(const float & _rRotationZ)
{
	m_tTransformDesc.vRotation.z += _rRotationZ;
}
void CTransform::Set_Scale(const _vector & _rScale)
{
	m_tTransformDesc.vScale = _rScale;
}
void CTransform::Set_WorldMatrix(const _matrix & matWorld)
{
	m_tTransformDesc.matWorld = matWorld;
}

#pragma endregion GetSet