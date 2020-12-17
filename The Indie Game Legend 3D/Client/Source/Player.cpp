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
	, m_fAmmo(0)
	, m_fAmmoMax(0)
	, m_nAmmoDecrease(0)
	, m_nSetWeaponID(0)
	, m_fBulletFireDelay(NormalDelay)
	, m_fBulletFireTime(0.f)
	, m_eCurWeaponType(EWeaponType::Big)
	, m_ePreWeaponType(EWeaponType::End)

{
}

CPlayer::CPlayer(const CPlayer & _rOther)
	: CGameObject(_rOther)
	, m_bUseWeapon(false)
	, m_fMoveSpeed(_rOther.m_fMoveSpeed)
	, m_fMouseSpeedX(_rOther.m_fMouseSpeedX)
	, m_fAmmo(_rOther.m_fAmmo)
	, m_fAmmoMax(_rOther.m_fAmmoMax)
	, m_nAmmoDecrease(_rOther.m_nAmmoDecrease)
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
	if (m_pKeyMgr->Key_Down(KEY_RBUTTON)  && m_vecWeapons.empty() == false)
	{
		m_bUseWeapon = !m_bUseWeapon;
		ChangeWeaponUISetting();
	}
	//weapon Change
	if (m_pKeyMgr->Key_Down(KEY_Q) && m_vecWeapons.empty() == false)
	{
		--m_nSetWeaponID;
		m_nSetWeaponID = CLAMP(m_nSetWeaponID, 0, _int(m_vecWeapons.size()-1));
		ChangeWeaponUISetting();
	}
	else if (m_pKeyMgr->Key_Down(KEY_E) && m_vecWeapons.empty() == false)
	{
		++m_nSetWeaponID;
		m_nSetWeaponID = CLAMP(m_nSetWeaponID, 0, _int(m_vecWeapons.size() - 1));
		ChangeWeaponUISetting();
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
		cout << "not ready" << endl;
		return;
	}

	// 현재 저 조건 아니면 발사 했다는 경우이니 초기화
	m_fBulletFireTime = 0.f;

	if (m_bUseWeapon)
	{
		switch (m_eCurWeaponType)
		{
		case EWeaponType::Multiple:
			Fire(EWeaponType::Multiple);
			break;
		case EWeaponType::Big:
			Fire(EWeaponType::Big);
			break;
		case EWeaponType::Rapid:
			Fire(EWeaponType::Normal);
			break;
		default :
			cout << " default  Input by BulletFire() of switch" << endl;
			break;
		}

		m_fAmmo -= m_nAmmoDecrease;
		m_fAmmo = CLAMP(m_fAmmo, 0.f, m_fAmmoMax);
		m_pAmmobar->SetFillAmount(m_fAmmo / m_fAmmoMax);
	}
	else
	{
		Fire(EWeaponType::Normal);
	}
}

void CPlayer::ChangeWeaponUISetting()
{
	//UI 아이콘 변경등등 콜할 거 들어가야함 
	if (m_vecWeapons.empty())
	{
		return;
	}

	m_eCurWeaponType = m_vecWeapons[m_nSetWeaponID];
	//중복콜 방지용
	if (m_ePreWeaponType != m_eCurWeaponType)
	{
		switch (m_eCurWeaponType)
		{
		case EWeaponType::Multiple:
			cout << "now Weapon is Multiple " << endl;
			break;
		case EWeaponType::Big:
			cout << "now Weapon is Big " << endl;
			break;
		case EWeaponType::Rapid:
			cout << "now Weapon is Rapid " << endl;
			break;
		default:
#ifdef _DEBUG
			cout << " default Input by ChangeWeaponSetting() of switch" << endl;
#endif
			break;
		}
		m_ePreWeaponType = m_eCurWeaponType;
	}
	ChangeWeapon();
}

//
void CPlayer::ChangeWeapon()
{
	//현재 설정된 웨폰 상태에 따라 딜레이 수정
	// 여기서도 외부콜 해야겠네 
	if (m_bUseWeapon)
	{
		switch (m_eCurWeaponType)
		{
		case EWeaponType::Multiple:
			m_fBulletFireDelay = MultipleDelay;
			m_nAmmoDecrease = 3;
			break;
		case EWeaponType::Big:
			m_fBulletFireDelay = BigDelay;
			m_nAmmoDecrease = 6;
			break;
		case EWeaponType::Rapid:
			m_fBulletFireDelay = RapidDelay;
			m_nAmmoDecrease = 2;
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

	//처음 먹은 경우 바로 웨폰 세팅 호출 

	switch (_eWeaponType)
	{
	case EWeaponType::Multiple:
		cout << "add Multiple Weapon" << endl;
		break;
	case EWeaponType::Big:
		cout << "add Big Weapon" << endl;
		break;
	case EWeaponType::Rapid:
		cout << "add Rapid Weapon" << endl;
		break;
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
	m_fAmmoMax = 50.f;
	m_fAmmo = m_fAmmoMax;
	return S_OK;
}

HRESULT CPlayer::Start()
{
	//Test
	m_pAmmobar = (Image*)((CAmmoGauge*)FindGameObjectOfType<CAmmoGauge>())->GetComponent<Image>();


	return S_OK;
}

UINT CPlayer::Update(const float _fDeltaTime)
{
	Key_Input(_fDeltaTime);
	m_pTransform->UpdateTransform();

	//Test
	if (GetAsyncKeyState('Z') & 0x0001)
	{
		AddWeapon(EWeaponType::Big);
	}
	if (GetAsyncKeyState('X') & 0x0001)
	{
		AddWeapon(EWeaponType::Multiple);
	}
	if (GetAsyncKeyState('C') & 0x0001)
	{
		AddWeapon(EWeaponType::Rapid);
	}
	if (GetAsyncKeyState('P') & 0x8000)
	{
		m_fAmmo += 10.f;
		m_fAmmo = CLAMP(m_fAmmo, 0, m_fAmmoMax);
	}

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
