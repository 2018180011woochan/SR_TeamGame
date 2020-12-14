#include "..\Header\Group.h"

USING(Engine)
const DWORD VERTEX::FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;

CGroup::CGroup(LPDIRECT3DDEVICE9 const _pDevice, const MATERIAL _tMaterial, LPDIRECT3DVERTEXBUFFER9 const _pVertexBuffer, const UINT _nFaceCount)
	: m_pDevice(_pDevice)
	, m_tMaterial(_tMaterial)
	, m_pVertexBuffer(_pVertexBuffer)
	, m_nFaceCount(_nFaceCount)
{
	SafeAddRef(m_pDevice);
}

CGroup::CGroup(const CGroup & _rOther)
	: m_pVertexBuffer(_rOther.m_pVertexBuffer)
	, m_tMaterial(_rOther.m_tMaterial)
	, m_nFaceCount(_rOther.m_nFaceCount)
	, m_pDevice(_rOther.m_pDevice)
{
	SafeAddRef(m_pVertexBuffer);
	SafeAddRef(m_tMaterial.pTexture);
	SafeAddRef(m_pDevice);
}

void CGroup::Free()
{
	SafeRelease(m_pVertexBuffer);
	SafeRelease(m_tMaterial.pTexture);
	SafeRelease(m_pDevice);
}

HRESULT CGroup::Render()
{
	if (FAILED(m_pDevice->SetTexture(0, (m_tMaterial.pTexture == nullptr ? 0 : m_tMaterial.pTexture->GetTexture()))))
		return E_FAIL;
	if (FAILED(m_pDevice->SetMaterial(&m_tMaterial.tMaterial)))
		return E_FAIL;
	if (FAILED(m_pDevice->SetStreamSource(0, m_pVertexBuffer, 0, sizeof(VERTEX))))
		return E_FAIL;
	if (FAILED(m_pDevice->SetFVF(VERTEX::FVF)))
		return E_FAIL;
	return m_pDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, m_nFaceCount);
}

CGroup * CGroup::Create(LPDIRECT3DDEVICE9 const _pDevice, const MATERIAL _tMaterial, LPDIRECT3DVERTEXBUFFER9 const _pVertexBuffer, const UINT _nFaceCount)
{
	CGroup* pInstance = new CGroup(_pDevice, _tMaterial, _pVertexBuffer, _nFaceCount);

	if (nullptr == pInstance)
	{
		PrintLog(TEXT("Error"), TEXT("Failed to create CGroup"));
		return nullptr;
	}

	return pInstance;
	return nullptr;
}

CGroup * CGroup::Clone()
{
	CGroup* pClone = new CGroup(*this);

	return pClone;
}

HRESULT CGroup::SetTexture(CTexture* const _pTexture)
{
	//기존 텍스처 레퍼런스 카운트 감소.
	SafeRelease(m_tMaterial.pTexture);

	m_tMaterial.pTexture = _pTexture;

	//새로운 텍스처 레퍼런스 카운트 증가.
	SafeAddRef(m_tMaterial.pTexture);

	return S_OK;
}


