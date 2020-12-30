#include "stdafx.h"
#include "..\Header\Loading.h"
#include "LoadingText.h"
#include "LoadingBackGround.h"

CLoading::CLoading(LPDIRECT3DDEVICE9 const _pDevice)
	: m_pDevice(_pDevice)
	, m_hThread(NULL)
{
	SafeAddRef(m_pDevice);
}

void CLoading::Free()
{
	CloseHandle(m_hThread);
	SafeRelease(m_pDevice);
}

CLoading * CLoading::Create(LPDIRECT3DDEVICE9 const _pDevice)
{
	CLoading* pInstance = new CLoading(_pDevice);

	return pInstance;
}

LPDIRECT3DDEVICE9 CLoading::GetDevice()
{
	return m_pDevice;
}

bool CLoading::IsSceneReady()
{
	return m_bSceneReady;
}

void CLoading::SetActiveScene(const bool _bActiveScene)
{
	m_pLoadedScene->SetActive(_bActiveScene);
}

unsigned CLoading::LoadingThread(void * pParam)
{
	CLoading* pLoading = (CLoading*)pParam;
	SafeAddRef(pLoading);
	CLoadingText* pLoadingText = CLoadingText::Create();
	CLoadingBackGround* pLoadingBackGround = CLoadingBackGround::Create();
	pLoadingText->Awake();
	pLoadingText->Start();
	pLoadingBackGround->Awake();
	pLoadingBackGround->Start();
	/*알파 테스팅 */
	pLoading->GetDevice()->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pLoading->GetDevice()->SetRenderState(D3DRS_ALPHAREF, 1); /*알파기준값*/
	pLoading->GetDevice()->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	//Off Z-Buffer
	pLoading->GetDevice()->SetRenderState(D3DRS_ZENABLE, false);
	//뷰 행렬을 항등행렬로 변환.
	D3DXMATRIX matView;
	D3DXMatrixIdentity(&matView);
	pLoading->GetDevice()->SetTransform(D3DTS_VIEW, &matView);
	/*직교 투영 변환*/
	D3DXMATRIX matOrtho;
	//직교 투영 변환에 필요한 화면 크기 정보 받아오기.
	LPDIRECT3DSURFACE9	pBackBuffer;
	pLoading->GetDevice()->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer);
	D3DSURFACE_DESC tSurfaceDesc;
	ZeroMemory(&tSurfaceDesc, sizeof(D3DSURFACE_DESC));
	pBackBuffer->GetDesc(&tSurfaceDesc);
	SafeRelease(pBackBuffer);
	//화면 정보 기반으로 직교 투영 행렬 생성 및 장치에 전달.
	D3DXMatrixOrthoLH(&matOrtho, float(tSurfaceDesc.Width), float(tSurfaceDesc.Height), 0.f, 1.f);
	pLoading->GetDevice()->SetTransform(D3DTS_PROJECTION, &matOrtho);


	while (true)
	{
		pLoading->GetDevice()->Clear(0, nullptr, D3DCLEAR_STENCIL | D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(255, 0, 0, 0), 1.f, 0);
		pLoading->GetDevice()->BeginScene();
		if (false == pLoading->IsSceneReady())
			pLoadingText->SetText(TEXT("Loading"));
		else
		{
			pLoadingText->SetText(TEXT("Press Enter To Start"));

			if (GetAsyncKeyState(VK_RETURN) && 0x8000)
			{
				break;
			}
		}
		pLoadingBackGround->Render();
		pLoadingText->Render();
		pLoading->GetDevice()->EndScene();
		pLoading->GetDevice()->Present(nullptr, nullptr, NULL, nullptr);
	}

	//On Z-Buffer 
	pLoading->GetDevice()->SetRenderState(D3DRS_ZENABLE, true);

	pLoading->GetDevice()->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	SafeRelease(pLoadingText);
	SafeRelease(pLoadingBackGround);

	pLoading->SetActiveScene(true);
	SafeRelease(pLoading);
	return 0;
}

HRESULT CLoading::OnLoading(CScene* const _pScene)
{
	m_bSceneReady = false;
	m_pLoadedScene = _pScene;
	m_hThread = (HANDLE)_beginthreadex(0, 0, LoadingThread, this, 0, nullptr);

	if (nullptr == m_hThread)
		return E_FAIL;

	m_pLoadedScene->SetActive(false);
	CManagement::GetInstance()->SetUpCurrentScene(_pScene);
	
	m_bSceneReady = true;

	/*while (true)
	{
		if (m_pLoadedScene->IsActive())
			break;
	}*/

	return S_OK;
}

