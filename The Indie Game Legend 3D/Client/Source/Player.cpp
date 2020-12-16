#include "stdafx.h"
#include "Player.h"
#include "KeyManager.h"
#include "Mouse.h"

#include "PlayerBullet.h"

#include "AmmoGauge.h"
USING(Engine)



CPlayer::CPlayer()
	:CGameObject()
	, m_fMoveSpeed(0.f)
	, m_fMouseSpeedX(0.f)
	, m_bUseWeapon(false)
	, m_nAmmo(0)
	, m_nAmmoMax(0)
	, m_nSetWeaponID(0)
	, m_fBulletFireDelay(NormalDelay)
	, m_fBulletFireTime(0.f)
	, m_eCurWeaponType(EWeaponType::End)
	, m_ePreWeaponType(EWeaponType::End)

{
}

CPlayer::CPlayer(const CPlayer & _rOther)
	: CGameObject(_rOther)
	, m_bUseWeapon(false)
	, m_fMoveSpeed(_rOther.m_fMoveSpeed)
	, m_fMouseSpeedX(_rOther.m_fMouseSpeedX)
	, m_nAmmo(_rOther.m_nAmmo)
	, m_nAmmoMax(_rOther.m_nAmmoMax)
	, m_nSetWeaponID(0)
	, m_fBulletFireDelay(NormalDelay)
	, m_fBulletFireTime(0.f)
	, m_eCurWeaponType(_rOther.m_eCurWeaponType)
	, m_ePreWeaponType(_rOther.m_ePreWeaponType)
	, m_vecWeapons(_rOther.m_vecWeapons)
{

}

HRESULT CPlayer::Key_Input(const float _fDeltaTime)
{
	_vector vMoveDir = vZero;

	//Move Code
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

	//Fire

	if (m_pKeyMgr->Key_Toggle(VK_F2))
	{
		if (m_pKeyMgr->Key_Down(KEY_LBUTTON))
		{
			BulletFire();
		}
	}

	//Use Weapon
	if (m_pKeyMgr->Key_Down(KEY_RBUTTON))
	{
		ChangeWeapon();
	}
	//weapon Change
	if (m_pKeyMgr->Key_Down(KEY_Q) && m_vecWeapons.empty() == false)
	{
		--m_nSetWeaponID;
		m_nSetWeaponID = CLAMP(m_nSetWeaponID, 0, _int(m_vecWeapons.size()-1));
		ChangeWeaponSetting();
	}
	else if (m_pKeyMgr->Key_Down(KEY_E) && m_vecWeapons.empty() == false)
	{
		++m_nSetWeaponID;
		m_nSetWeaponID = CLAMP(m_nSetWeaponID, 0, _int(m_vecWeapons.size() - 1));
		ChangeWeaponSetting();
	}
	return S_OK;
}

void CPlayer::BulletFire()
{
	auto Fire = [&](const EWeaponType& _Type){
		CPlayerBullet* pBullet = (CPlayerBullet*)AddGameObject<CPlayerBullet>();
		pBullet->Set_Type(_Type);
		pBullet->Fire();
	};
	if (m_fBulletFireTime < m_fBulletFireDelay)
	{
		cout << "not Ready" << endl;
		return;
	}

	// 현재 저 조건 아니면 발사 했다는 경우이니 초기화
	m_fBulletFireTime = 0.f;

	if (m_bUseWeapon)
	{
		switch (static_cast<EWeaponType>(m_nSetWeaponID))
		{
		case EWeaponType::Multiple:
			Fire(EWeaponType::Multiple);
			break;
		case EWeaponType::Big:
			Fire(EWeaponType::Big);
			break;
		default :
			cout << " default  Input by BulletFire() of switch" << endl;
			break;
		}
	}
	else
	{
		Fire(EWeaponType::Normal);
	}
}

void CPlayer::ChangeWeaponSetting()
{
	if (m_vecWeapons.empty())
	{
		return;
	}

	m_eCurWeaponType = m_vecWeapons[m_nSetWeaponID];
	//중복콜 방지용
	if (m_ePreWeaponType != m_eCurWeaponType)
	{
		//UI 아이콘 변경등등 콜 들어가야함 
		switch (m_vecWeapons[m_nSetWeaponID])
		{
		case EWeaponType::Multiple:
			cout << "now Weapon is Multiple " << endl;
			break;
		case EWeaponType::Big:
			cout << "now Weapon is Big " << endl;
			break;
		default:
#ifdef _DEBUG
			cout << " default Input by ChangeWeaponSetting() of switch" << endl;
#endif
			break;
		}

		m_ePreWeaponType = m_eCurWeaponType;
	}
}

//
void CPlayer::ChangeWeapon()
{
	if (0 == m_vecWeapons.size())
	{
		m_bUseWeapon = false;
		return;
	}
	//현재 설정된 웨폰 상태에 따라 딜레이 수정
	// ammo 소모값도 추가해야함 
	// 여기서도 외부콜 해야겠네 
	m_bUseWeapon = !m_bUseWeapon;
	if (m_bUseWeapon)
	{
		switch (m_eCurWeaponType)
		{
		case EWeaponType::Multiple:
			m_fBulletFireDelay = MultipleDelay;
			break;
		case EWeaponType::Big:
			m_fBulletFireDelay = BigDelay;
			break;
		default:
#ifdef _DEBUG
			cout << " default Input by ChangeWeaponSetting() of switch" << endl;
#endif
			break;
		}
	}
	else
	{
		m_fBulletFireDelay = NormalDelay;
	}

}

void CPlayer::AddWeapon(const EWeaponType _eWeaponType)
{
	for (auto& eType : m_vecWeapons)
	{
		if (eType == _eWeaponType)
		{
#ifdef _DEBUG
			cout << "Weapon already exists by CPlayer::AddWeapon()" << endl;
#endif
			return;
		}
	}
	m_vecWeapons.emplace_back(_eWeaponType);
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
	m_fMoveSpeed = 30.f;
	m_fMouseSpeedX = 200.f;
	return S_OK;
}

HRESULT CPlayer::Start()
{
	//Test
	AddWeapon(EWeaponType::Big);
	AddWeapon(EWeaponType::Multiple);
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

	//cout << m_fBulletFireTime << endl;

	if (m_fBulletFireTime < m_fBulletFireDelay)
		m_fBulletFireTime += _fDeltaTime;

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
	m_vecWeapons.clear();
	m_vecWeapons.shrink_to_fit();
	CGameObject::Free();
}
