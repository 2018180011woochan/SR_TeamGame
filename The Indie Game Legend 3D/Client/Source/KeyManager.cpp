#include "stdafx.h"
#include "KeyManager.h"

IMPLEMENT_SINGLETON(CKeyManager)

CKeyManager::CKeyManager()
	:m_dwKeyDownState(0)
	,m_dwKeyState(0)
	,m_dwKeyUpState(0)
{
}

CKeyManager::~CKeyManager()
{
}

bool CKeyManager::Key_Down(DWORD key)
{
	if (m_dwKeyState & key && !(m_dwKeyDownState & key))
	{
		m_dwKeyDownState |= key;
		return true;
	}
	else if (!(m_dwKeyState & key) && m_dwKeyDownState & key)
	{
		m_dwKeyDownState ^= key;
		return false;
	}
	return false;
}

bool CKeyManager::Key_Up(DWORD key)
{
	if (m_dwKeyState & key)
	{
		m_dwKeyUpState |= key;
		return false;
	}
	else if (m_dwKeyUpState & key)
	{
		m_dwKeyUpState ^= key;
		return true;
	}
	return false;
}

bool CKeyManager::Key_Press(DWORD key)
{
	return (m_dwKeyState & key) != 0;

}

bool CKeyManager::Key_Toggle(int key)
{
	return GetKeyState(key) != 0;
}

void CKeyManager::Update_KeyManager()
{
	m_dwKeyState = 0;
	/*다이렉트 전용 키함수 있음*/
	if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)
		m_dwKeyState |= KEY_RBUTTON;
	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
		m_dwKeyState |= KEY_LBUTTON;
	if (GetAsyncKeyState('A') & 0x8000)
		m_dwKeyState |= KEY_A;
	if (GetAsyncKeyState('D') & 0x8000)
		m_dwKeyState |= KEY_D;
	if (GetAsyncKeyState('W') & 0x8000)
		m_dwKeyState |= KEY_W;
	if (GetAsyncKeyState('S') & 0x8000)
		m_dwKeyState |= KEY_S;

	if (GetAsyncKeyState('F') & 0x8000)
		m_dwKeyState |= KEY_F;
	if (GetAsyncKeyState('Q') & 0x8000)
		m_dwKeyState |= KEY_Q;
	if (GetAsyncKeyState('E') & 0x8000)
		m_dwKeyState |= KEY_E;

	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
		m_dwKeyState |= KEY_SPACE;
	if (GetAsyncKeyState(VK_LSHIFT) & 0x8000)
		m_dwKeyState |= KEY_LSHIFT;
}

void CKeyManager::Free()
{
}
