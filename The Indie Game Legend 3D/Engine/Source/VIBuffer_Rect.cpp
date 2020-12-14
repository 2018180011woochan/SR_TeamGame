#include "VIBuffer_Rect.h"


USING(Engine)
CVIBuffer_Rect::CVIBuffer_Rect(CGameObject * const _pGameObject, LPDIRECT3DDEVICE9 const _pDevice)
	:CVIBuffer(_pGameObject, _pDevice)
{
}

CVIBuffer_Rect::CVIBuffer_Rect(const CVIBuffer_Rect & _rOther)
	:CVIBuffer(_rOther)
{
}

HRESULT CVIBuffer_Rect::Initialize()
{
	m_iVertexSize = sizeof(VTX_TEXTURE);
	m_iVertexCount = 4;
	m_iFVF = FVF_VTX_TEXTURE;
	m_iTriCount = 2;

	m_iIndexSize = sizeof(INDEX16);
	m_IndexFormat = D3DFMT_INDEX16;

	if (FAILED(CVIBuffer::Initialize()))
		return E_FAIL;

	VTX_TEXTURE* pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	pVertex[0].vPosition = D3DXVECTOR3(-1.f, 1.f, 0.f);
	pVertex[0].vUV = D3DXVECTOR2(0.f, 0.f);

	pVertex[1].vPosition = D3DXVECTOR3(1.f, 1.f, 0.f);
	pVertex[1].vUV = D3DXVECTOR2(1.f, 0.f);

	pVertex[2].vPosition = D3DXVECTOR3(1.f, -1.f, 0.f);
	pVertex[2].vUV = D3DXVECTOR2(1.f, 1.f);

	pVertex[3].vPosition = D3DXVECTOR3(-1.f, -1.f, 0.f);
	pVertex[3].vUV = D3DXVECTOR2(0.f, 1.f);

	m_pVertices = new VTX_TEXTURE[m_iVertexCount];
	memcpy(m_pVertices, pVertex, sizeof(VTX_TEXTURE) * m_iVertexCount);

	m_pVB->Unlock();

	INDEX16* pIndex = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	pIndex[0]._1 = 0;
	pIndex[0]._2 = 1;
	pIndex[0]._3 = 2;

	pIndex[1]._1 = 0;
	pIndex[1]._2 = 2;
	pIndex[1]._3 = 3;
	m_pIB->Unlock();

	return S_OK;
}


HRESULT CVIBuffer_Rect::RenderVIBuffer()
{
	/*싹다 부모로 올릴까 생각*/
	if (FAILED(CVIBuffer::RenderVIBuffer()))
		return E_FAIL;

	if (FAILED(m_pDevice->SetIndices(m_pIB)))
		return E_FAIL;

	return m_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_iVertexCount, 0, m_iTriCount);
}

CVIBuffer_Rect * CVIBuffer_Rect::Create(CGameObject * const _pGameObject, LPDIRECT3DDEVICE9 const _pDevice)
{
	CVIBuffer_Rect* pInstance = new CVIBuffer_Rect(_pGameObject, _pDevice);
	if (FAILED(pInstance->Initialize()))
	{
		PrintLog(L"Error", L"Failed To Create CVIBuffer_RectTexture");
		SafeRelease(pInstance);
	}
	return pInstance;
}

void CVIBuffer_Rect::Free()
{
	CVIBuffer::Free();
}
