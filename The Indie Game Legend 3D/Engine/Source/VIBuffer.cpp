#include "VIBuffer.h"
USING(Engine)

CVIBuffer::CVIBuffer(CGameObject * const _pGameObject, LPDIRECT3DDEVICE9 const _pDevice)
	:CComponent(_pGameObject,_pDevice)
{
}

CVIBuffer::CVIBuffer(const CVIBuffer & _rOther)
	:CComponent(_rOther)
	, m_pVB(_rOther.m_pVB)
	, m_pIB(_rOther.m_pIB)
	, m_iVertexSize(_rOther.m_iVertexSize)
	, m_iVertexCount(_rOther.m_iVertexCount)
	, m_iTriCount(_rOther.m_iTriCount)
	, m_iFVF(_rOther.m_iFVF)
	, m_iIndexSize(_rOther.m_iIndexSize)
	, m_IndexFormat(_rOther.m_IndexFormat)
	, m_pVertices(_rOther.m_pVertices)
	, m_IsClone(true)
{
}

HRESULT CVIBuffer::Initialize()
{
	if (FAILED(m_pDevice->CreateVertexBuffer(
		m_iVertexSize * m_iVertexCount, 0, m_iFVF, 
		D3DPOOL_MANAGED,&m_pVB,  nullptr)))
	{
		return E_FAIL;
	}

	/* 인덱스버퍼 생성 */
	if (FAILED(m_pDevice->CreateIndexBuffer(
		m_iIndexSize * m_iTriCount,  0, 
		m_IndexFormat, D3DPOOL_MANAGED, &m_pIB, nullptr)))
		return E_FAIL;

	return S_OK;
}

void CVIBuffer::Free()
{
	if (false == m_IsClone)
	{
		SafeDeleteArray(m_pVertices);
	}

	SafeRelease(m_pVB);
	SafeRelease(m_pIB);

	CComponent::Free();
}
/*버텍스와 정점포펫 세팅*/
HRESULT CVIBuffer::RenderVIBuffer()
{
	if (FAILED(m_pDevice->SetStreamSource(0, m_pVB, 0, m_iVertexSize)))
		return E_FAIL;

	if (FAILED(m_pDevice->SetFVF(m_iFVF)))
		return E_FAIL;

	return S_OK;
}

_bool CVIBuffer::IsRayPicking(HWND _hwnd, _uint _iWinCX, _uint _iWinCY, const CAMERA_DESC& _rCameraDecs, const _matrix * _pWorldMatrix, _vector * _pOut)
{
	if (nullptr == _hwnd ||
		nullptr == _pWorldMatrix)
		return false;

	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(_hwnd, &pt);

	/* 뷰포트 -> 투영 스페이스 */
	_vector vMouse = { 0.f, 0.f, 0.f };
	vMouse.x = pt.x / (_iWinCX * 0.5f) - 1.f;
	vMouse.y = 1.f - pt.y / (_iWinCY * 0.5f);
	vMouse.z = 1.f; 

	/* 투영 스페이스 -> 뷰 스페이스 */
	_matrix InvProjmatrix = _rCameraDecs.matProj;
	D3DXMatrixInverse(&InvProjmatrix, 0, &InvProjmatrix);
	D3DXVec3TransformCoord(&vMouse, &vMouse, &InvProjmatrix);

    m_vRayPivot = _vector(0.f, 0.f, 0.f);
    m_vRayDirection = vMouse - m_vRayPivot;
	D3DXVec3Normalize(&m_vRayDirection, &m_vRayDirection);

	/* 뷰 스페이스 -> 월드 스페이스 */
	_matrix InvViewmatrix = _rCameraDecs.matView;
	D3DXMatrixInverse(&InvViewmatrix, 0, &InvViewmatrix);
	D3DXVec3TransformCoord(&m_vRayPivot, &m_vRayPivot, &InvViewmatrix);
	D3DXVec3TransformNormal(&m_vRayDirection, &m_vRayDirection, &InvViewmatrix);
	D3DXVec3Normalize(&m_vRayDirection, &m_vRayDirection);

	/* 월드 스페이스 -> 로컬 스페이스 */
	_matrix InsWorldMatrix;
	D3DXMatrixInverse(&InsWorldMatrix, 0, _pWorldMatrix);
	D3DXVec3TransformCoord(&m_vRayPivot, &m_vRayPivot, &InsWorldMatrix);
	D3DXVec3TransformNormal(&m_vRayDirection, &m_vRayDirection, &InsWorldMatrix);
	D3DXVec3Normalize(&m_vRayDirection, &m_vRayDirection);
	return true;
}

