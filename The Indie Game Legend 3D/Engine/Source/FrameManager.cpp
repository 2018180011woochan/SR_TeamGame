#include "..\Header\FrameManager.h"
#include "TimeManager.h"
USING(Engine)
IMPLEMENT_SINGLETON(CFrameManager)
CFrameManager::CFrameManager()
	:m_fSPF(0.f)
	, m_nCurFPS(0)
	, m_fSPFTimeDelta(0.f)
	, m_fFPSTiemDelta(0.f)
{
	ZeroMemory(&m_CPUTick, sizeof(LARGE_INTEGER));
	ZeroMemory(&m_EndTime, sizeof(LARGE_INTEGER));
	ZeroMemory(&m_BeginTime, sizeof(LARGE_INTEGER));
}

bool CFrameManager::LimitFrame()
{
	QueryPerformanceFrequency(&m_CPUTick);
	QueryPerformanceCounter(&m_EndTime);
	m_fSPFTimeDelta += float(m_EndTime.QuadPart - m_BeginTime.QuadPart) / m_CPUTick.QuadPart;;
	m_BeginTime.QuadPart = m_EndTime.QuadPart;
	if (m_fSPF < m_fSPFTimeDelta)
	{
		++m_nCurFPS;
		m_fSPFTimeDelta = 0.f;
		return true;
	}
	return false;
}

void CFrameManager::Set_LimitFrame(const float _fLimit)
{
	m_fSPF = 1.f / _fLimit;
	QueryPerformanceFrequency(&m_CPUTick);
	QueryPerformanceCounter(&m_EndTime);
	QueryPerformanceCounter(&m_BeginTime);
}

void CFrameManager::ShowFrame(const HWND _hWnd, const float _fDeltaTime)
{
	m_fFPSTiemDelta += _fDeltaTime;
	if (1.f < m_fFPSTiemDelta)
	{
		TCHAR	szFPS[16] = L"";
		swprintf_s(szFPS, L"FPS: %d", m_nCurFPS);
		SetWindowText(_hWnd, szFPS);
		m_fFPSTiemDelta = 0;
		m_nCurFPS = 0;
	}
}

void CFrameManager::Free()
{
}
