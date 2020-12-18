#include "stdafx.h"
#include "Player.h"
#include "KeyManager.h"
#include "Mouse.h"
#include "PlayerBullet.h"
#include "PlayerCamera.h"
#include "AmmoGauge.h"
USING(Engine)



CPlayer::CPlayer()
	:CGameObject()
	, m_fMoveSpeed(0.f)
	, m_fDashSpeed(0.f)
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
	, m_fDashDelay(0.f)
	, m_fDashDelayTime(0.f)
	, m_fDashDuration(0.f)
	, m_fDashDurationTime(0.f)
	, m_eState(EState::Move)

{
}

CPlayer::CPlayer(const CPlayer & _rOther)
	: CGameObject(_rOther)
	, m_bUseWeapon(false)
	, m_fMoveSpeed(_rOther.m_fMoveSpeed)
	, m_fDashSpeed(_rOther.m_fDashSpeed)
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
	, m_eState(_rOther.m_eState)
	, m_fDashDurationTime(_rOther.m_fDashDurationTime)

{

}

HRESULT CPlayer::KeyInput(const float _fDeltaTime)
{
	if (m_eState == EState::Hurt)
		return S_OK;

	//Move Code
	MoveCheck();

	//camera code
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

HRESULT CPlayer::MoveCheck()
{
	if (m_eState == EState::Dash)
		return S_OK;

	m_vMoveDir = vZero;

	if (m_pKeyMgr->Key_Press(KEY_W))
	{
		m_vMoveDir += m_pTransform->Get_Look();
		if (m_pKeyMgr->Key_Press(KEY_A))
		{
			m_vMoveDir += -(m_pTransform->Get_Right());
		}
		else if (m_pKeyMgr->Key_Press(KEY_D))
		{
			m_vMoveDir += (m_pTransform->Get_Right());
		}
	}
	else if (m_pKeyMgr->Key_Press(KEY_S))
	{
		m_vMoveDir -= m_pTransform->Get_Look();
		if (m_pKeyMgr->Key_Press(KEY_A))
		{
			m_vMoveDir += -(m_pTransform->Get_Right());
		}
		else if (m_pKeyMgr->Key_Press(KEY_D))
		{
			m_vMoveDir += (m_pTransform->Get_Right());
		}
	}
	else if (m_pKeyMgr->Key_Press(KEY_A))
	{
		m_vMoveDir += -(m_pTransform->Get_Right());
	}
	else if (m_pKeyMgr->Key_Press(KEY_D))
	{
		m_vMoveDir += (m_pTransform->Get_Right());
	}

	if (m_pKeyMgr->Key_Press(KEY_LSHIFT))
		m_eState = EState::Run;

	if (m_pKeyMgr->Key_Down(KEY_SPACE) && m_fDashDelay <= m_fDashDelayTime)
	{
		m_eState = EState::Dash;
		m_fDashDurationTime = 0.f;
		m_fDashDelayTime = 0.f;
	}


	return S_OK;
}

void CPlayer::Move(const float& _fSpeed,  const float _fDeltaTime)
{
	D3DXVec3Normalize(&m_vMoveDir, &m_vMoveDir);
	m_pTransform->Add_Position(m_vMoveDir * _fSpeed *_fDeltaTime);
}

void CPlayer::UpdateState(const float _fDeltaTime)
{
	switch (m_eState)
	{
	case EState::Move:
		Move(m_fMoveSpeed, _fDeltaTime);
		break;
	case EState::Run:
	{
		Move(m_fRunSpeed, _fDeltaTime);
		m_fRunningTime += _fDeltaTime;
		float fPer = 0.9f;
		fPer += fabs(cosf(PI* (m_fRunningTime / RunCameraYCycle) ) / 10.f);
		fPer = CLAMP(fPer, 0.5f, 1.f);
		CPlayerCamera* pCamera = (CPlayerCamera*)FindGameObjectOfType<CPlayerCamera>();
		pCamera->SetHeghitPersent(fPer);
	}
		break;
	case EState::Dash:
	{
		Move(m_fDashSpeed, _fDeltaTime);
		m_fDashDurationTime += _fDeltaTime;
		CPlayerCamera* pCamera = (CPlayerCamera*)FindGameObjectOfType<CPlayerCamera>();
		float fPer = 0.7f;
		fPer += fabs(0.3f - m_fDashDurationTime / m_fDashDuration);
		fPer = CLAMP(fPer, 0.5f, 1.f);
		pCamera->SetHeghitPersent(fPer);
	}
		break;
	default:
		break;
	}
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
	m_fRunSpeed = 45.f;
	m_fDashSpeed = 60.f;
	m_fMouseSpeedX = 200.f;
	m_fAmmoMax = 50.f;
	m_fAmmo = m_fAmmoMax;
	m_fDashDelay = 2.f;
	m_fDashDelayTime = m_fDashDelay;
	m_fDashDuration = 0.4f;
	return S_OK;
}

HRESULT CPlayer::Start()
{
	//Test
	m_pAmmobar = (Image*)((CAmmoGauge*)FindGameObjectOfType<CAmmoGauge>())->GetComponent<Image>();
	//SafeAddRef(m_pAmmobar);

	return S_OK;
}

UINT CPlayer::Update(const float _fDeltaTime)
{

	KeyInput(_fDeltaTime);
	m_pTransform->UpdateTransform();
	UpdateState(_fDeltaTime);

	return OBJ_NOENVET;
}

UINT CPlayer::LateUpdate(const float _fDeltaTime)
{

	//cout << m_fBulletFireTime << endl;

	if (m_fBulletFireTime < m_fBulletFireDelay)
		m_fBulletFireTime += _fDeltaTime;

	//대쉬에서 복귀
	if (m_eState == EState::Dash && m_fDashDuration < m_fDashDurationTime)
		m_eState = EState::Move;

	if(m_eState == EState::Run && !m_pKeyMgr->Key_Press(KEY_LSHIFT))
		m_eState = EState::Move;

	if(m_fDashDelayTime < m_fDashDelay)
		m_fDashDelayTime += _fDeltaTime;

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
	//SafeRelease(m_pAmmobar);
	m_vecWeapons.clear();
	m_vecWeapons.shrink_to_fit();
	CGameObject::Free();
}
