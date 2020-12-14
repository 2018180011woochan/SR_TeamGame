#pragma once
#ifndef __TIME_MANAGER_H__
#include "Base.h"
BEGIN(Engine)
class CTimeManager : public CBase
{
	DECLARE_SINGLETON(CTimeManager)
private:
	LARGE_INTEGER m_CPUCount;
	LARGE_INTEGER m_StartTime;
	LARGE_INTEGER m_GoalTime;
private:
	explicit CTimeManager();
	virtual ~CTimeManager() = default;
	// CBase을(를) 통해 상속됨
	virtual void Free() override;
public:
	HRESULT	Initialize();
	float	GetDeltaTime();


};
END
#define __TIME_MANAGER_H__
#endif // !__TIME_MANAGER_H__
