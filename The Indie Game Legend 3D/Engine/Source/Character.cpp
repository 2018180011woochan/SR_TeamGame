#include "..\Header\Character.h"

USING(Engine)

CCharacter::CCharacter(LPDIRECT3DDEVICE9 const _pDevice, LPDIRECT3DVERTEXBUFFER9 const _pVertexBuffer, LPDIRECT3DINDEXBUFFER9 const _pIndexBuffer, const UINT _nWidth, const UINT _nHeight)
	: m_pDevice(_pDevice)
	, m_pVertexBuffer(_pVertexBuffer)
	, m_pIndexBuffer(_pIndexBuffer)
	, m_nWidth(_nWidth)
	, m_nHeight(_nHeight)
{
	SafeAddRef(m_pDevice);
}

CCharacter::CCharacter(const CCharacter & _rOther)
	: m_pDevice(_rOther.m_pDevice)
	, m_pVertexBuffer(_rOther.m_pVertexBuffer)
	, m_pIndexBuffer(_rOther.m_pIndexBuffer)
	, m_nWidth(_rOther.m_nWidth)
	, m_nHeight(_rOther.m_nHeight)
{
	SafeAddRef(m_pDevice);
	SafeAddRef(m_pVertexBuffer);
	SafeAddRef(m_pIndexBuffer);
}

void CCharacter::Free()
{
	SafeRelease(m_pIndexBuffer);
	SafeRelease(m_pVertexBuffer);
	SafeRelease(m_pDevice);
}

CCharacter * CCharacter::Create(LPDIRECT3DDEVICE9 const _pDevice, LPDIRECT3DVERTEXBUFFER9 const _pVertexBuffer, LPDIRECT3DINDEXBUFFER9 const _pIndexBuffer, const UINT _nWidth, const UINT _nHeight)
{
	CCharacter* pInstance = new CCharacter(_pDevice, _pVertexBuffer, _pIndexBuffer, _nWidth, _nHeight);
	return pInstance;
}

CCharacter * CCharacter::Clone()
{
	CCharacter* pClone = new CCharacter(*this);
	return pClone;
}

HRESULT CCharacter::Render()
{
	if (FAILED(m_pDevice->SetStreamSource(0, m_pVertexBuffer, 0, sizeof(VERTEXRECT))))
		return E_FAIL;
	if (FAILED(m_pDevice->SetIndices(m_pIndexBuffer)))
		return E_FAIL;
	return m_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);
}

UINT CCharacter::GetWidth()
{
	return m_nWidth;
}
