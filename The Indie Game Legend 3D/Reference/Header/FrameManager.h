#pragma once
#ifndef __FRAMEMANAGER_H__
#include "Base.h"
BEGIN(Engine)
class CFrameManager final : public CBase
{
	DECLARE_SINGLETON(CFrameManager)
private:
	explicit CFrameManager();
	virtual ~CFrameManager() = default;
public:
	bool LimitFrame();
	void Set_LimitFrame(const float _fLimit);
	void ShowFrame(const HWND _hWnd, const float _fDeltaTime);
private:
	virtual void Free();
private:
	float m_fSPF;
	_int  m_nCurFPS;
	float m_fSPFTimeDelta;
	float m_fFPSTiemDelta;
	LARGE_INTEGER m_CPUTick;
	LARGE_INTEGER m_BeginTime;
	LARGE_INTEGER m_EndTime;
};
END
#define __FRAMEMANAGER_H__
#endif
