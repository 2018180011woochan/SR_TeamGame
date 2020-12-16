#include "stdafx.h"
#include "Player.h"
#include "KeyManager.h"
#include "Mouse.h"

#include "PlayerBullet.h"
USING(Engine)

CPlayer::CPlayer()
	:CGameObject()
{
}

CPlayer::CPlayer(const CPlayer & _rOther)
	:CGameObject(_rOther)
{

}

HRESULT CPlayer::Key_Input(const float _fDeltaTime)
{
	_vector vMoveDir = vZero;
	if (m_pKeyMgr->Key_Press(KEY_W))
	{
		vMoveDir += m_pTransform->Get_Look();
		if (m_pKeyMgr->Key_Press(KEY_A))
		{
			vMoveDir += -(m_pTransform->Get_Right());
		}
		else if (m_pKeyMgr->Key_Press(KEY_D))
		{
			vMoveDir += (m_pTransform->Get_Right());
		}
	}
	else if (m_pKeyMgr->Key_Press(KEY_S))
	{
		vMoveDir -= m_pTransform->Get_Look();
		if (m_pKeyMgr->Key_Press(KEY_A))
		{
			vMoveDir += -(m_pTransform->Get_Right());
		}
		else if (m_pKeyMgr->Key_Press(KEY_D))
		{
			vMoveDir += (m_pTransform->Get_Right());
		}
	}
	else if (m_pKeyMgr->Key_Press(KEY_A))
	{
		vMoveDir += -(m_pTransform->Get_Right());
	}
	else if (m_pKeyMgr->Key_Press(KEY_D))
	{
		vMoveDir += (m_pTransform->Get_Right());
	}

	D3DXVec3Normalize(&vMoveDir, &vMoveDir);
	vMoveDir *= m_fMoveSpeed;
	m_pTransform->Add_Position(vMoveDir*_fDeltaTime);

	CMouse* pMouse = (CMouse*)FindGameObjectOfType<CMouse>();
	m_pTransform->Add_RotationY(pMouse->Get_MouseDir().x *  m_fMouseSpeedX * _fDeltaTime);

	//

	if (m_pKeyMgr->Key_Toggle(VK_F2))
	{
		if (m_pKeyMgr->Key_Down(KEY_LBUTTON))
		{
			CPlayerBullet* pBullet = (CPlayerBullet*)AddGameObject<CPlayerBullet>();
			pBullet->Set_Type(EBulletType::Normal);
			pBullet->Fire();
		}
	}
	return S_OK;
}

HRESULT CPlayer::InitializePrototype()
{
	return S_OK;
}

HRESULT CPlayer::Awake()
{
	CGameObject::Awake();
	m_pKeyMgr = CKeyManager::GetInstance();
	m_pTransform->Set_Position(_vector(0, 0, 0));
	m_eRenderID = ERenderID::Alpha;
	m_fMoveSpeed = 50.f;
	m_fMouseSpeedX = 200.f;
	return S_OK;
}

HRESULT CPlayer::Start()
{
	return S_OK;
}

UINT CPlayer::Update(const float _fDeltaTime)
{
	Key_Input(_fDeltaTime);
	m_pTransform->UpdateTransform();

	return OBJ_NOENVET;
}

UINT CPlayer::LateUpdate(const float _fDeltaTime)
{
	m_pTransform->UpdateWorld();
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
	CGameObject::Free();
}
