#include "..\Header\Renderer.h"
/*추가->오브젝트 랜더 호출 */
#include "GameObject.h"

USING(Engine)

CRenderer::CRenderer(LPDIRECT3DDEVICE9 _pDevice)
	: m_pDevice(_pDevice)
	, m_bDrawCollider(false)
{
	SafeAddRef(_pDevice);
}

void CRenderer::Free()
{
	/*차피 랜더 끝나고 클리어하지만 예외적인 종료 상황시 처리*/
	for (_int i = 0; i < (_int)ERenderID::End; ++i)
	{
		for (auto& pObject : m_GameObjects[i])
		{
			SafeRelease(pObject);
		}

		m_GameObjects[i].clear();
	}
	SafeRelease(m_pDevice);
}

/* 매 프레임마다 렌더 리스트에 오브젝트를 추가한다. */
HRESULT CRenderer::AddInRenderer(const ERenderID _eRenderID, CGameObject * const _pGameObject)
{
	if (0 > (_int)_eRenderID ||
		ERenderID::End <= _eRenderID)
	{
		PrintLog(L"Error", L"Out of range Render list");
		return E_FAIL;
	}

	if (nullptr == _pGameObject)
		return E_FAIL;

	m_GameObjects[(_int)_eRenderID].push_back(_pGameObject);
	SafeAddRef(_pGameObject);
	//Collider 그리기 전용.
	m_ColliderObjects.push_back(_pGameObject);
	return S_OK;
}


HRESULT CRenderer::RenderPriority()
{
	for (auto& pObject : m_GameObjects[(_int)ERenderID::Priority])
	{
		if (FAILED(pObject->Render()))
			return E_FAIL;

		SafeRelease(pObject);
	}

	m_GameObjects[(_int)ERenderID::Priority].clear();

	return S_OK;
}

HRESULT CRenderer::RenderNoAlpha()
{
	for (auto& pObject : m_GameObjects[(_int)ERenderID::NoAlpha])
	{
		if (FAILED(pObject->Render()))
			return E_FAIL;

		SafeRelease(pObject);
	}

	m_GameObjects[(_int)ERenderID::NoAlpha].clear();

	return S_OK;
}

HRESULT CRenderer::RenderAlpha()
{
	/*알파 테스팅 */
	m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_ALPHAREF, 1); /*알파기준값*/
	m_pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);


	for (auto& pObject : m_GameObjects[(_int)ERenderID::Alpha])
	{
		if (FAILED(pObject->Render()))
			return E_FAIL;

		SafeRelease(pObject);
	}

	m_GameObjects[(_int)ERenderID::Alpha].clear();

	m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	return S_OK;

}

HRESULT CRenderer::RenderAlphaCullNone()
{
	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_ALPHAREF, 1); /*알파기준값*/
	m_pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	for (auto& pObject : m_GameObjects[(_int)ERenderID::AlphaCullNone])
	{
		if (FAILED(pObject->Render()))
			return E_FAIL;

		SafeRelease(pObject);
	}

	m_GameObjects[(_int)ERenderID::AlphaCullNone].clear();

	m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	return S_OK;
}

HRESULT CRenderer::RenderUI()
{
	//uiw 조명 off
	//m_pDevice->SetRenderState(D3DRS_LIGHTING, false);

	/*알파 테스팅 */
	m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_ALPHAREF, 1); /*알파기준값*/
	m_pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	//Off Z-Buffer
	m_pDevice->SetRenderState(D3DRS_ZENABLE, false);
	//뷰 행렬을 항등행렬로 변환.
	D3DXMATRIX matView;
	D3DXMatrixIdentity(&matView);
	m_pDevice->SetTransform(D3DTS_VIEW, &matView);
	/*직교 투영 변환*/
	D3DXMATRIX matOrtho;
	//직교 투영 변환에 필요한 화면 크기 정보 받아오기.
	LPDIRECT3DSURFACE9	pBackBuffer;
	m_pDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer);
	D3DSURFACE_DESC tSurfaceDesc;
	ZeroMemory(&tSurfaceDesc, sizeof(D3DSURFACE_DESC));
	pBackBuffer->GetDesc(&tSurfaceDesc);
	SafeRelease(pBackBuffer);
	//화면 정보 기반으로 직교 투영 행렬 생성 및 장치에 전달.
	D3DXMatrixOrthoLH(&matOrtho, float(tSurfaceDesc.Width), float(tSurfaceDesc.Height), 0.f, 1.f);

	//D3DXVECTOR3 vOffset = D3DXVECTOR3(0.5f, 0.f, 0.f);
	//D3DXMATRIX matOffset;
	//D3DXMatrixTranslation(&matOffset, vOffset.x, vOffset.y, vOffset.z);
	//matOrtho = matOffset * matOrtho;

	m_pDevice->SetTransform(D3DTS_PROJECTION, &matOrtho);

	for (auto& pObject : m_GameObjects[(_int)ERenderID::UI])
	{
		if (FAILED(pObject->Render()))
			return E_FAIL;

		SafeRelease(pObject);
	}

	m_GameObjects[(_int)ERenderID::UI].clear();

	//On Z-Buffer 
	m_pDevice->SetRenderState(D3DRS_ZENABLE, true);

	m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	//m_pDevice->SetRenderState(D3DRS_LIGHTING, true);

	return S_OK;
}

HRESULT CRenderer::RenderCollider()
{
	CCollider* pCollider = nullptr;
	CTransform* pTransform = nullptr;

	m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	for (auto& pGameObject : m_ColliderObjects)
	{
		pCollider = (CCollider*)(pGameObject->GetComponent<CCollider>());
		if (nullptr == pCollider)
			continue;
		pCollider->Draw();
	}
	m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	return S_OK;
}
 

HRESULT CRenderer::Render(HWND _hWnd)
{
	m_pDevice->Clear(0, nullptr, D3DCLEAR_STENCIL | D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(255, 0, 0, 255), 1.f, 0);
	m_pDevice->BeginScene();

	D3DXMATRIX matView, matProj;
	m_pDevice->GetTransform(D3DTS_VIEW, &matView);
	m_pDevice->GetTransform(D3DTS_PROJECTION, &matProj);

	if (FAILED(RenderPriority()))
		return E_FAIL;

	if (FAILED(RenderNoAlpha()))
		return E_FAIL;

	if (FAILED(RenderAlpha()))
		return E_FAIL;

	if (FAILED(RenderAlphaCullNone()))
		return E_FAIL;

	if (FAILED(RenderUI()))
		return E_FAIL;

	if (true == m_bDrawCollider)
	{
		m_pDevice->SetTransform(D3DTS_VIEW, &matView);
		m_pDevice->SetTransform(D3DTS_PROJECTION, &matProj);
		RenderCollider();
	}

	m_ColliderObjects.clear();

	m_pDevice->EndScene();
	m_pDevice->Present(nullptr, nullptr, _hWnd, nullptr);

	return S_OK;
}

CRenderer * CRenderer::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	if (nullptr == _pDevice)
		return nullptr;
	return new CRenderer(_pDevice);
}
