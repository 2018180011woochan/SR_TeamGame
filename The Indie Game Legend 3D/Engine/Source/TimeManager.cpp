#include "..\Header\TimeManager.h"

USING(Engine)
IMPLEMENT_SINGLETON(CTimeManager)

CTimeManager::CTimeManager()
{
	ZeroMemory(&m_CPUCount, sizeof(LARGE_INTEGER));
	ZeroMemory(&m_StartTime, sizeof(LARGE_INTEGER));
	ZeroMemory(&m_GoalTime, sizeof(LARGE_INTEGER));
}

void CTimeManager::Free()
{
}

HRESULT CTimeManager::Initialize()
{
	QueryPerformanceCounter(&m_StartTime);
	QueryPerformanceCounter(&m_GoalTime);
	QueryPerformanceFrequency(&m_CPUCount);
	return S_OK;
}

float CTimeManager::GetDeltaTime()
{
	QueryPerformanceFrequency(&m_CPUCount);
	QueryPerformanceCounter(&m_GoalTime);
	float fDeltaTime = float(m_GoalTime.QuadPart - m_StartTime.QuadPart) / m_CPUCount.QuadPart;
	m_StartTime.QuadPart = m_GoalTime.QuadPart;

	return fDeltaTime;
}
