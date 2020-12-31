#include "Transform.h"
#include "GameObject.h"

USING(Engine)

CTransform::CTransform(CGameObject * const _pGameObject, LPDIRECT3DDEVICE9 const _pDevice)
	:CComponent(_pGameObject,_pDevice)
	, m_pParent(nullptr)
{
}

CTransform::CTransform(const CTransform & _rOther)
	:CComponent(_rOther)
	, m_pParent(nullptr)
{
	PrintLog(L"warring", L"copy transform Danger");
}

HRESULT CTransform::Initialize()
{
	ZeroMemory(&m_tTransformDesc, sizeof(TRANSFORM_DESC));
	m_tTransformDesc.vScale = _vector(1, 1, 1);
	D3DXMatrixIdentity(&m_tTransformDesc.matWorld);
	D3DXMatrixIdentity(&m_tTransformDesc.vParent);
	return S_OK;
}

void CTransform::UpdateTransform()
{
	_matrix matScale, matRotX, matRotY, matRotZ, matTrans, matRevX, matRevY, matRevZ, matParent;
	D3DXMatrixScaling(&matScale, m_tTransformDesc.vScale.x, m_tTransformDesc.vScale.y, m_tTransformDesc.vScale.z);
	D3DXMatrixRotationX(&matRotX, D3DXToRadian(m_tTransformDesc.vRotation.x));
	D3DXMatrixRotationY(&matRotY, D3DXToRadian(m_tTransformDesc.vRotation.y));
	D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(m_tTransformDesc.vRotation.z));
	D3DXMatrixTranslation(&matTrans, m_tTransformDesc.vPosition.x, m_tTransformDesc.vPosition.y, m_tTransformDesc.vPosition.z);
	D3DXMatrixRotationX(&matRevX, D3DXToRadian(m_tTransformDesc.vRevolution.x));
	D3DXMatrixRotationY(&matRevY, D3DXToRadian(m_tTransformDesc.vRevolution.y));
	D3DXMatrixRotationZ(&matRevZ, D3DXToRadian(m_tTransformDesc.vRevolution.z));


	D3DXMatrixIsIdentity(&matParent);
	if (nullptr == m_pParent)
		D3DXMatrixIdentity(&matParent);
	else
		matParent = ((CTransform*)m_pParent->GetComponent<CTransform>())->Get_WorldMatrix();


	m_tTransformDesc.matWorld = matScale * matRotX * matRotY *matRotZ * matTrans
		*matRevX * matRevY * matRevZ * m_tTransformDesc.vParent;
}
/*행렬 세팅용*/
HRESULT CTransform::UpdateWorld()
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

const _vector CTransform::Get_WorldPosition() const
{
	_vector vPos;
	memcpy(&vPos, &m_tTransformDesc.matWorld.m[3][0], sizeof(_vector));
	return vPos;
}

const _vector CTransform::Get_Parent() const
{
	return  m_tTransformDesc.vParent;
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
void CTransform::Set_RotationZ(const float & _rRotationZ)
{
	m_tTransformDesc.vRotation.z = _rRotationZ;
}
void CTransform::Set_Revolution(const _vector & _rRevolution)
{
	m_tTransformDesc.vRevolution = _rRevolution;
}
void CTransform::Add_Revolution(const _vector & _rRevolution)
{
	m_tTransformDesc.vRevolution += _rRevolution;
}
void CTransform::Add_RevolutionX(const float & _rRevolutionX)
{
	m_tTransformDesc.vRevolution.x += _rRevolutionX;
}
void CTransform::Add_RevolutionY(const float & _rRevolutionY)
{
	m_tTransformDesc.vRevolution.y += _rRevolutionY;
}
void CTransform::Add_RevolutionZ(const float & _rRevolutionZ)
{
	m_tTransformDesc.vRevolution.z += _rRevolutionZ;
}
void CTransform::Set_Parent(const D3DMATRIX& _rParent)
{
	m_tTransformDesc.vParent = _rParent;
}

void CTransform::SetParent(CGameObject * _pParent)
{
	m_pParent = _pParent;
}
void CTransform::Add_parent(const _vector & _rParent)
{
	m_tTransformDesc.vParent += _rParent;
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