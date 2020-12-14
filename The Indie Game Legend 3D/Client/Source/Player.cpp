#include "stdafx.h"
#include "Player.h"
#include "KeyManager.h"
USING(Engine)

CPlayer::CPlayer()
	:CGameObject()
{
}

CPlayer::CPlayer(const CPlayer & _rOther)
	:CGameObject()
	, m_pTransform(_rOther.m_pTransform)

{

}

HRESULT CPlayer::Key_Input(const float _fDeltaTime)
{
	_vector vMoveDir = vZero;
	if (m_pKeyMgr->Key_Press(KEY_W))
	{
		vMoveDir.z = 1;
		if (m_pKeyMgr->Key_Press(KEY_A))
		{
			vMoveDir.x = -1;
		}
		else if (m_pKeyMgr->Key_Press(KEY_D))
		{
			vMoveDir.x = 1;
		}
		D3DXVec3Normalize(&vMoveDir, &vMoveDir);
		m_pTransform->Add_Position(vMoveDir*_fDeltaTime);
	}
	else if (m_pKeyMgr->Key_Press(KEY_S))
	{
		vMoveDir.z = -1;
		if (m_pKeyMgr->Key_Press(KEY_A))
		{
			vMoveDir.x = -1;
		}
		else if (m_pKeyMgr->Key_Press(KEY_D))
		{
			vMoveDir.x = 1;
		}
		D3DXVec3Normalize(&vMoveDir, &vMoveDir);
		m_pTransform->Add_Position(vMoveDir*_fDeltaTime);
	}
	else if (m_pKeyMgr->Key_Press(KEY_A))
	{
		m_pTransform->Add_Position(vMoveDir*_fDeltaTime);
	}
	else if (m_pKeyMgr->Key_Press(KEY_D))
	{
		m_pTransform->Add_Position(vMoveDir*_fDeltaTime);
	}
	return S_OK;
}

HRESULT CPlayer::InitializePrototype()
{
	m_fMoveSpeed = 5.f;
	return S_OK;
}

HRESULT CPlayer::Awake()
{
	m_pTransform = (CTransform*)AddComponent<CTransform>();
	m_pKeyMgr = CKeyManager::GetInstance();
	SafeAddRef(m_pTransform);
	SafeAddRef(m_pKeyMgr);
	m_pTransform->Set_Position(_vector(0, 0, 0));
	return S_OK;
}

HRESULT CPlayer::Start()
{
	return S_OK;
}

UINT CPlayer::Update(const float _fDeltaTime)
{
	Key_Input(_fDeltaTime);

	return OBJ_NOENVET;
}

UINT CPlayer::LateUpdate(const float _fDeltaTime)
{
	return OBJ_NOENVET;
}

HRESULT CPlayer::Render()
{
	return S_OK;
}

CGameObject * CPlayer::Clone()
{
	CPlayer* pClone = new CPlayer(*this);
	
	return pClone;
}

CPlayer * CPlayer::Create()
{
	CPlayer* pInstance = new CPlayer();
	if (FAILED(pInstance->InitializePrototype()))
	{
		SafeDelete(pInstance);
		PrintLog(L"Error", L"Faild player Create");
	}
	return pInstance;
}

void CPlayer::Free()
{
	SafeRelease(m_pTransform);
	SafeRelease(m_pKeyMgr);
	CGameObject::Free();
}
