#include "stdafx.h"
#include "MsgManager.h"

IMPLEMENT_SINGLETON(CMsgManager)
POINT CMsgManager::Get_ClientCenterPoint()
{
	POINT pt = { (_int)m_vClinetCenterPos.x,(_int)m_vClinetCenterPos.y };
	return pt;
}

/*WndProc size �޽��� ȣ��� Ŭ��ũ�� ����Ǿ����� �缳��*/
void CMsgManager::ReSizeClient()
{
	GetClientRect(g_hWnd, &m_tClinetRect);
	m_vClinetCenterPos = _vector((float)(m_tClinetRect.right - m_tClinetRect.left) / 2.f,
		float(m_tClinetRect.bottom - m_tClinetRect.top) / 2.f, 0.f);
}

CMsgManager::CMsgManager()
{
}

CMsgManager::~CMsgManager()
{
}

void CMsgManager::Free()
{
}
 