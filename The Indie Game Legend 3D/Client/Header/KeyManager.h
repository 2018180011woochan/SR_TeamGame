#pragma once
#ifndef __KEYMANAGER_H__
#include "Base.h"

#define KEY_LBUTTON 0x00000001 << 0
#define KEY_RBUTTON 0x00000001 << 1
#define KEY_A		0x00000001 << 2
#define KEY_D		0x00000001 << 3
#define KEY_W       0x00000001 << 4
#define KEY_S		0x00000001 << 5

#define KEY_Q		0x00000001 << 6
#define KEY_E		0x00000001 << 7
#define KEY_F		0x00000001 << 8

#define KEY_SPACE   0x00000001 << 9
#define KEY_LSHIFT  0x00000001 << 10

#define KEY_Z		0x00000001 << 11
#define KEY_X		0x00000001 << 12
#define KEY_C		0x00000001 << 13

#define KEY_1		0x00000001 << 14
#define KEY_2		0x00000001 << 15
#define KEY_3		0x00000001 << 16

#define KEY_F1		0x00000001 << 17

#define KEY_4		0x00000001 << 18
#define KEY_5		0x00000001 << 19
#define KEY_6		0x00000001 << 20


USING(Engine)
class CKeyManager : public CBase
{
private:
	DWORD m_dwKeyState;
	DWORD m_dwKeyUpState;
	DWORD m_dwKeyDownState;

	DECLARE_SINGLETON(CKeyManager)
private:
	explicit CKeyManager();
	virtual ~CKeyManager();
public:
	bool Key_Down(DWORD key);
	bool Key_Up(DWORD key);
	bool Key_Press(DWORD key);
	bool Key_Toggle(int key);
	void Update_KeyManager();

	// CBase을(를) 통해 상속됨
	virtual void Free() override;
};
#define __KEYMANAGER_H__
#endif
