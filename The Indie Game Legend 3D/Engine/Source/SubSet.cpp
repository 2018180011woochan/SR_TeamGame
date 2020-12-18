#include "..\Header\SubSet.h"

USING(Engine)


CSubSet::CSubSet(LPDIRECT3DDEVICE9 const _pDevice, const MATERIAL _tMaterial, const UINT _nStartVertex, const UINT _nFaceCount)
	: m_pDevice(_pDevice)
	, m_tMaterial(_tMaterial)
	, m_nStartVertex(_nStartVertex)
	, m_nFaceCount(_nFaceCount)
{
	SafeAddRef(m_pDevice);
}

CSubSet::CSubSet(const CSubSet & _rOther)
	: m_pDevice(_rOther.m_pDevice)
	, m_tMaterial(_rOther.m_tMaterial)
	, m_nStartVertex(_rOther.m_nStartVertex)
	, m_nFaceCount(_rOther.m_nFaceCount)
{
	SafeAddRef(m_pDevice);
	SafeAddRef(m_tMaterial.pTexture);
}

void CSubSet::Free()
{
	SafeRelease(m_pDevice);
	SafeRelease(m_tMaterial.pTexture);
}

CSubSet * CSubSet::Create(LPDIRECT3DDEVICE9 const _pDevice, const MATERIAL _tMaterial, const UINT _nStartVertex, const UINT _nFaceCount)
{
	CSubSet* pInstance = new CSubSet(_pDevice, _tMaterial, _nStartVertex, _nFaceCount);
	return pInstance;
}

CSubSet * CSubSet::Clone()
{
	CSubSet* pClone = new CSubSet(*this);
	return pClone;
}

HRESULT CSubSet::DrawSubSet()
{
	if (FAILED(m_pDevice->SetTexture(0, (m_tMaterial.pTexture == nullptr ? 0 : m_tMaterial.pTexture->GetTexture()))))
		return E_FAIL;
	if (FAILED(m_pDevice->SetMaterial(&m_tMaterial.tMaterial)))
		return E_FAIL;
	return m_pDevice->DrawPrimitive(D3DPT_TRIANGLELIST, m_nStartVertex, m_nFaceCount);
}

HRESULT CSubSet::SetTexture(CTexture * const _pTexture)
{
	//기존 텍스처 레퍼런스 카운트 감소.
	SafeRelease(m_tMaterial.pTexture);

	m_tMaterial.pTexture = _pTexture;

	//새로운 텍스처 레퍼런스 카운트 증가.
	SafeAddRef(m_tMaterial.pTexture);

	return S_OK;
}


