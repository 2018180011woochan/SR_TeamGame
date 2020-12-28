#include "stdafx.h"
#include "MsgManager.h"
#include "SoundMgr.h"

IMPLEMENT_SINGLETON(CMsgManager)
POINT CMsgManager::Get_ClientCenterPoint()
{
	POINT pt = { (_int)m_vClinetCenterPos.x,(_int)m_vClinetCenterPos.y };
	return pt;
}

/*WndProc size 메시지 호출시 클라크기 변경되었으니 재설정*/
void CMsgManager::ReSizeClient()
{
	GetClientRect(g_hWnd, &m_tClinetRect);
	m_vClinetCenterPos = _vector(
		(float)((m_tClinetRect.right - m_tClinetRect.left) / 2),
		(float)((m_tClinetRect.bottom - m_tClinetRect.top) / 2), 0.f);
}

void CMsgManager::Freeze(const float* _fTimeDeleta)
{
	if (m_bTrigger)
	{
		float* fTime = const_cast<float*>(_fTimeDeleta);
		*fTime = 0.f;
	}
}

void CMsgManager::UpdateFreezing(const float & _fTimeDelta)
{
	if (m_bTrigger)
	{
		m_fFreezingTime += _fTimeDelta;
		if (m_fFreezing < m_fFreezingTime)
		{
			m_bTrigger = false;
			m_fFreezingTime = 0.f;
			CSoundMgr::GetInstance()->StopSound(CSoundMgr::TimeStopSkill);
		}
	}
}

void CMsgManager::Freezingstart(const float & _fFreeze)
{
	if (m_bTrigger == false)
	{
		m_bTrigger = true;
		m_fFreezing = _fFreeze;
		/*CSoundMgr::GetInstance()->Play(L"sfxClack.wma", CSoundMgr::TimeStopSkill);*/
		CSoundMgr::GetInstance()->Play(L"sfxTheWorld.mp3", CSoundMgr::TimeStopSkill);
	}
}

CMsgManager::CMsgManager()
	:m_fFreezing(0.f)
	,m_fFreezingTime(0.f)
	, m_bTrigger(false)
{
	ZeroMemory(&m_tClinetRect, sizeof(RECT));
}

CMsgManager::~CMsgManager()
{
}

void CMsgManager::Free()
{
}
 