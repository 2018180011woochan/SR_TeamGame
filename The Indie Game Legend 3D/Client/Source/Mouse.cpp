#include "stdafx.h"
#include "Mouse.h"
#include "KeyManager.h"
#include "MsgManager.h"
USING(Engine)
CMouse::CMouse()
	:CGameObject()
{
}

CMouse::CMouse(const CMouse & _rOther)
	:CGameObject()
	,m_vMouseDir(_rOther.m_vMouseDir)
{
}

void CMouse::MoveToCentorCursor()
{
	POINT ClientCenter = CMsgManager::GetInstance()->Get_ClientCenterPoint();
	ClientToScreen(g_hWnd, &ClientCenter);
	SetCursorPos(ClientCenter.x, ClientCenter.y);
}

void CMouse::UpdateMouseDir()
{
	_vector vClinetCenter = CMsgManager::GetInstance()->Get_ClientCenterVector();
	POINT tMousePos;
	GetCursorPos(&tMousePos);
	ScreenToClient(g_hWnd, &tMousePos);

	_vector vMousePos = _vector(float(tMousePos.x), float(tMousePos.y), 0.f);
	m_vMouseDir = vMousePos - vClinetCenter;
	D3DXVec3Normalize(&m_vMouseDir, &m_vMouseDir);
}

_vector CMouse::Get_MouseDir()
{
	return m_vMouseDir;
}

HRESULT CMouse::InitializePrototype()
{
	m_vMouseDir = vZero;
	return S_OK;
}

HRESULT CMouse::Awake()
{
	return S_OK;
}

HRESULT CMouse::Start()
{
	return S_OK;
}

UINT CMouse::Update(const float _fDeltaTime)
{
	m_vMouseDir = vZero;
	if (CKeyManager::GetInstance()->Key_Toggle(VK_F1))
	{
		UpdateMouseDir();
		MoveToCentorCursor();
	}
	return OBJ_NOENVET;
}

UINT CMouse::LateUpdate(const float _fDeltaTime)
{
	return OBJ_NOENVET;
}

HRESULT CMouse::Render()
{

	/*텍스쳐 생기면 추가*/
	return S_OK;
}

CGameObject * CMouse::Clone()
{
	CMouse* pInstance = new CMouse(*this);
	return pInstance;
}

CMouse * CMouse::Create()
{
	CMouse* pInstance = new CMouse();
	if (FAILED(pInstance->InitializePrototype()))
	{
		SafeRelease(pInstance);
		PrintLog(L"Error", L"Faild Create Mouse");
	}
	return pInstance;
}

void CMouse::Free()
{
	CGameObject::Free();
}
