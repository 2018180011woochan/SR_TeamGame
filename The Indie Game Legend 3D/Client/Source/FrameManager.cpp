#include "..\Header\FrameManager.h"

USING(Engine)
IMPLEMENT_SINGLETON(CFrameManager)
CFrameManager::CFrameManager()
	:m_fSPF(0.f)
	,m_nCurFPS(0)
	,m_fSPFTimeDelta(0.f)
	, m_fFPSTiemDelta(0.f)
{
}

bool CFrameManager::LimitFrame(const float _fDeltaTime)
{
	m_fSPFTimeDelta += _fDeltaTime;

	if (m_fSPF < m_fSPFTimeDelta)
	{
		++m_nCurFPS;
		m_fSPFTimeDelta = 0.f;
		return true;
	}
	return false;
}

void CFrameManager::Set_LimitFrame(const float& _fLimit)
{
	m_fSPF = 1.f / _fLimit;
}

void CFrameManager::ShowFrame(const float _fDeltaTime)
{
	m_fFPSTiemDelta += _fDeltaTime;
	if (1.f < m_fFPSTiemDelta)
	{
		TCHAR	szFPS[16] = L"";
		swprintf_s(szFPS, L"FPS: %d", m_nCurFPS);
		SetWindowText(g_hWnd, szFPS);
		m_nCurFPS = 0;
	}
}

void CFrameManager::Free()
{
}
