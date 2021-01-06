#include "stdafx.h"
#include "Player.h"
#include "KeyManager.h"
#include "Mouse.h"

#include "PlayerCamera.h"
#include "AmmoGauge.h"
#include "UtilityManger.h"
#include "LightMananger.h"
#include "HeartManager.h"
#include "Item.h"
#include "SoundMgr.h"
#include "PlayerSpawn.h"
#include "WeaponHUD.h"

#include "BigBullet.h"
#include "TripleBullet.h"
#include "NormalBullet.h"
#include "FlameBullet.h"
#include "LaserBullet.h"
#include "MsgManager.h"
#include "GemText.h"
#include "DiscText.h"
USING(Engine)
#include "Gun.h"
#include "Focus.h"
#include "CrossHair.h"
#include "AmmoHUD.h"

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
	, m_eTileID(ETileID::Nomal)
	, m_pGun(nullptr)
	, m_pPlayerCamera(nullptr)
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
	, m_eTileID(_rOther.m_eTileID)
	, m_pGun(nullptr)
	, m_bSpotLightTrigger(false)
	, m_fHighNoonDmg(0.f)
	, m_pPlayerCamera(nullptr)
{

}

HRESULT CPlayer::KeyInput(const float _fDeltaTime)
{
	//Move Code
	MoveCheck();

	//camera code
	CMouse* pMouse = (CMouse*)FindGameObjectOfType<CMouse>();
	m_pTransform->Add_RotationY(pMouse->Get_MouseDir().x *  m_fMouseSpeedX * _fDeltaTime);

	//Fire & airStrike
	//����� �ϱ� ���ϰ� ��Ŭ�� ���� �ɾ�� 
	if (m_pKeyMgr->Key_Toggle(VK_F2))
	{
		//������ų ��� �������� üũ
		if (CMsgManager::GetInstance()->GetAirStrikeReady())
		{
			//���� ��� üũ 
			_vector AirStrikePos = vZero;
			//�ٴ��̶� �����浹�� üũ�ؼ� �浹�� && ��Ŭ�� �ٿ� �� 
			if (CUtilityManger::AirstrikePicking(m_nSceneID, AirStrikePos) 
				&& m_pKeyMgr->Key_Down(KEY_LBUTTON))
			{
				CMsgManager::GetInstance()->AirStrikeStart(m_nSceneID, AirStrikePos);
				m_pCrossHair->SetEnable(true);
			}
			AirStrikePos.y = 0.1f;
			m_pFocus->SetFocusPos(AirStrikePos);
		}
		//��ũ����//  [1/5/2021 wades]
		else if (CMsgManager::GetInstance()->GetHighNoonReady())
		{
			m_bIsDeBuff = true;
			m_listHighNoon.clear();

			if (CUtilityManger::AutoAim(m_nSceneID, m_listHighNoon))
			{
				//��� UI ȣ�� 
				m_fHighNoonDmg += HighNoonAmount * _fDeltaTime;
				m_fHighNoonDmg = CLAMP(m_fHighNoonDmg, 0, HighNoonMaxDmg);

				// Log [1/6/2021 wades]
				cout << "Target Count : " << m_listHighNoon.size() << ", Dmg: " << m_fHighNoonDmg << endl;
				if (m_pKeyMgr->Key_Down(KEY_LBUTTON))
				{
					m_pPlayerCamera->StopCameraWorking();
					m_bIsDeBuff = false;
					CMsgManager::GetInstance()->HighNoonStart(m_nSceneID, m_listHighNoon, (_int)m_fHighNoonDmg);
					m_fHighNoonDmg = 0;
					m_listHighNoon.clear();
				}
			}
		}
		else // �⺻���� 
		{
			//�÷��� ���� �ƴҰ�� �ݵ� �ʿ� 
			if (m_ePreWeaponType != EWeaponType::Flame &&  m_pKeyMgr->Key_Down(KEY_LBUTTON))
			{
				BulletFire();
			}
			//�÷��� ���ϰ�� �ݵ� ���ʿ� 
			else if (m_ePreWeaponType == EWeaponType::Flame &&  m_pKeyMgr->Key_Press(KEY_LBUTTON))
			{
				BulletFire();
			}
		}
	}

	//Use Weapon
	if (m_pKeyMgr->Key_Down(KEY_RBUTTON)  && m_vecWeapons.empty() == false)
	{
		m_bUseWeapon = !m_bUseWeapon;
		ChangeWeaponUISetting();
		m_pAmmoHud->SetActive(m_bUseWeapon);
		m_pGun->SetActive(m_bUseWeapon);
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

	//Use Skill
	if (true  /*m_bEnableSkill*/)
	{
		if (m_pKeyMgr->Key_Down(KEY_1))
		{
			CMsgManager::GetInstance()->FreezingStart(5.f);
		}
		if (m_pKeyMgr->Key_Down(KEY_2))
		{
			CMsgManager::GetInstance()->HighNoonReady(5.f);
			m_pPlayerCamera->SetCameraZoomIn(60.f, 5.f);
			m_fHighNoonDmg = 0;
		}
		else if (m_pKeyMgr->Key_Down(KEY_3))
		{
			CMsgManager::GetInstance()->AirStrikeReady();
			m_pFocus->SetEnable(true);
			m_pCrossHair->SetEnable(false);
		}
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

	if (m_eState != EState::Hit &&  m_pKeyMgr->Key_Press(KEY_LSHIFT))
		m_eState = EState::Run;

	if (m_pKeyMgr->Key_Down(KEY_SPACE) && m_fDashDelay <= m_fDashDelayTime)
	{
		m_eState = EState::Dash;
		m_fDashDurationTime = 0.f;
		m_fDashDelayTime = 0.f;
		SoundPlay(ESoundID::Dash);
	}


	return S_OK;
}

void CPlayer::Move(float _fResultSpeed,  const float _fDeltaTime)
{
	D3DXVec3Normalize(&m_vMoveDir, &m_vMoveDir);

	//  [1/6/2021 wades] ����� ������ ���� �ӵ� ��ȭ�� ����ҵ� �б� ������ 
	if (m_bIsDeBuff)
	{
		_fResultSpeed /= 2.f;
	}
	m_pTransform->Add_Position(m_vMoveDir * _fResultSpeed *_fDeltaTime);

	if (D3DXVECTOR3(0.f, 0.f, 0.f) != m_vMoveDir)
		m_pGun->SetState(CGun::STATE::MOVE);
	else
		m_pGun->SetState(CGun::STATE::NONE);
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

		if (m_fRunningTime > 0.3f)
		{
			SoundPlay(ESoundID::Run);
			m_fRunningTime = 0.f;
		}
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
	case EState::Hit:
	{
		m_fHitAnimatTime += _fDeltaTime * 3.f;
		//Camera ��鸲 �ڵ� 
		CPlayerCamera* pCamera = (CPlayerCamera*)FindGameObjectOfType<CPlayerCamera>();
		float fPer = 0.f;
		float fInterval = 1.f;
		fPer =  -0.5f + fabs(cosf((2 * PI)* (m_fHitAnimatTime / m_fHitDelay)));
		fInterval *= fPer;
		pCamera->SetShakeVertical(fInterval);
		Move(m_fMoveSpeed/2.f, _fDeltaTime);
	}
		break;
	default:
		break;
	}
}

void CPlayer::TakeItem(const EItemID & _eID)
{
	switch (_eID)
	{
	case EItemID::Disc :
		++m_nDisc;
		m_nDisc = CLAMP(m_nDisc, 0, DiscMax);
		m_pDiscText->SetCount(m_nDisc);
		break;
	case EItemID::Heart:
		AddHp(4);
		break;
	case EItemID::Ammo:
		++m_fAmmo;
		m_fAmmo = CLAMP(m_fAmmo, 0.f, m_fAmmoMax);
		m_pAmmoHud->SetAmmoCount(m_fAmmo, m_fAmmoMax);
		SoundPlay(ESoundID::AddAmmo);
		break;
	case EItemID::sprCoin:
		++m_nGem;
		m_pGemText->SetCount(m_nGem);
		SoundPlay(ESoundID::AddCoin);
		break;
	case EItemID::sprBigCoin:
		m_nGem += 10;
		m_pGemText->SetCount(m_nGem);
		SoundPlay(ESoundID::AddCoin);
		break;
	case EItemID::End:
		break;
	}
}

void CPlayer::BulletFire()
{
	if (m_fBulletFireTime < m_fBulletFireDelay)
		return;
	
	// ���� �� ���� �ƴϸ� �߻� �ߴٴ� ����̴� �ʱ�ȭ
	m_fBulletFireTime = 0.f;
	CBullet* pBullet = nullptr;

	if (m_bUseWeapon)
	{
		if (m_fAmmo <= 0)
		{
			SoundPlay(ESoundID::EmptyShot);
			m_pGun->SetFire();
			return;
		}
		switch (m_eCurWeaponType)
		{
		case EWeaponType::Multiple:
			pBullet = (CTripleBullet*)AddGameObject<CTripleBullet>();
			pBullet->Fire();			
			m_pGun->SetFire();
			SoundPlay(ESoundID::NormaBullet);
			break;
		case EWeaponType::Big:
			 pBullet = (CBigBullet*)AddGameObject<CBigBullet>();
			pBullet->Fire();
			m_pGun->SetFire();
			SoundPlay(ESoundID::BigBullet);
			break;
		case EWeaponType::Rapid:
			 pBullet = (CNormalBullet*)AddGameObject<CNormalBullet>();
			pBullet->Fire();
			m_pGun->SetFire();
			SoundPlay(ESoundID::NormaBullet);
			break;
		case EWeaponType::Flame:
			pBullet = (CFlameBullet*)AddGameObject<CFlameBullet>();
			pBullet->Fire();
			SoundPlay(ESoundID::FlameBullet);
			break;
		case EWeaponType::Lazer:
			pBullet = (CLaserBullet*)AddGameObject<CLaserBullet>();
			pBullet->Fire();
			m_pGun->SetFire();
			SoundPlay(ESoundID::LaserBullet);
			break;
		default :
			break;
		}

		m_fAmmo -= m_nAmmoDecrease;
		m_fAmmo = CLAMP(m_fAmmo, 0.f, m_fAmmoMax);
		m_pAmmoHud->SetAmmoCount(m_fAmmo, m_fAmmoMax);
	}
	else
	{
		CNormalBullet* pBullet = (CNormalBullet*)AddGameObject<CNormalBullet>();
		pBullet->Fire();
		SoundPlay(ESoundID::NormaBullet);
		m_pGun->SetFire();
	}
}

void CPlayer::ChangeWeaponUISetting()
{
	//UI ������ ������ ���� �� ������ 
	if (m_vecWeapons.empty())
	{
		return;
	}

	m_eCurWeaponType = m_vecWeapons[m_nSetWeaponID];
	//�ߺ��� ������
	if (m_ePreWeaponType != m_eCurWeaponType)
	{
		m_pAmmoHud->SetAmmoIcon((UINT)m_eCurWeaponType);
		m_ePreWeaponType = m_eCurWeaponType;
	}
	ChangeWeapon();
}

//
void CPlayer::ChangeWeapon()
{
	//���� ������ ���� ���¿� ���� ������ ����
	// ���⼭�� �ܺ��� �ؾ߰ڳ� 
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
			m_nAmmoDecrease = 1;
			break;
		case EWeaponType::Flame:
			m_fBulletFireDelay = FlameDelay;
			m_nAmmoDecrease = 2;
			break;
		case EWeaponType::Lazer:
			m_fBulletFireDelay = LaserDelay;
			m_nAmmoDecrease = 4;
			break;
		default:
			break;
		}
	}
	else
	{
		m_fBulletFireDelay = NormalDelay;
	}
}

void CPlayer::UpdateLight()
{
	if (m_bSpotLightTrigger == false)
		return;
	D3DLIGHT9& playerLight = *CLightMananger::GetInstance()->GetLight(CLightMananger::Player);
	playerLight.Position = m_pTransform->Get_Position();
	playerLight.Position.y = 30.f;
	CLightMananger::GetInstance()->SetLight(CLightMananger::Player);
}

void CPlayer::SetSpotLightTrigget(const bool & _bTrigger)
{
	m_bSpotLightTrigger = _bTrigger;
}

void CPlayer::SetsfxTileID(const ETileID & _eID)
{
	m_eTileID = _eID;
}

void CPlayer::AddHp(_int _nHp)
{
	m_nHp += _nHp;
	m_nHp = CLAMP(m_nHp, 0, m_nHpMax *4 );

	m_pHeartManager->SetGauge(m_nHp);
}

void CPlayer::AddSkillGauge(_int _nPoint)
{
	m_nSkillPoint += _nPoint;
	m_nSkillPoint = CLAMP(m_nSkillPoint, 0, SkillPGaugeMax);
	
	if (m_nSkillPoint == SkillPGaugeMax)
		m_bEnableSkill = true;
	else
		m_bEnableSkill = false;

	m_pAmmoHud->SetSkillGauge(float(m_nSkillPoint) / SkillPGaugeMax);
}

void CPlayer::AddHpMax()
{
	++m_nHpMax;
	m_nHpMax = CLAMP(m_nHpMax, 0, HPMax);
	m_pHeartManager->SetHeartCount(m_nHpMax);
}

void CPlayer::TileSound(const ETileID& _eID)
{
	switch (_eID)
	{
	case ETileID::Nomal:
		if (m_bsfxStep)
			CSoundMgr::GetInstance()->Play(L"sfxStep1.mp3", CSoundMgr::Player_Action);
		else
			CSoundMgr::GetInstance()->Play(L"sfxStep2.mp3", CSoundMgr::Player_Action);
		break;
	case ETileID::Sand:
		if (m_bsfxStep)
			CSoundMgr::GetInstance()->Play(L"sfxSandStep1.mp3", CSoundMgr::Player_Action);
		else
			CSoundMgr::GetInstance()->Play(L"sfxSandStep2.mp3", CSoundMgr::Player_Action);
		break;
	case ETileID::Grass:
		if (m_bsfxStep)
			CSoundMgr::GetInstance()->Play(L"sfxGrass1.mp3", CSoundMgr::Player_Action);
		else
			CSoundMgr::GetInstance()->Play(L"sfxGrass2.mp3", CSoundMgr::Player_Action);
		break;
	case ETileID::Stone:
		if (m_bsfxStep)
			CSoundMgr::GetInstance()->Play(L"sfxStoneStep1.mp3", CSoundMgr::Player_Action);
		else
			CSoundMgr::GetInstance()->Play(L"sfxStoneStep2.mp3", CSoundMgr::Player_Action);
		break;
	case ETileID::Metal:
		if (m_bsfxStep)
			CSoundMgr::GetInstance()->Play(L"sfxMetalStep1.mp3", CSoundMgr::Player_Action);
		else
			CSoundMgr::GetInstance()->Play(L"sfxMetalStep2.mp3", CSoundMgr::Player_Action);
		break;
	}
}

void CPlayer::SoundPlay(const ESoundID & _eID)
{

	switch (_eID)
	{
	case ESoundID::EmptyShot:
		CSoundMgr::GetInstance()->Play(L"sfxGunEmpty.wav", CSoundMgr::Player_Bullet);
		break;
	case ESoundID::AmmoLvUp:
		CSoundMgr::GetInstance()->Play(L"sfxUpgrade.wav", CSoundMgr::Player_Event);
		break;
	case ESoundID::NormaBullet:
		CSoundMgr::GetInstance()->Play(L"sfxBullet.wav", CSoundMgr::Player_Bullet);
		break;
	case ESoundID::BigBullet:
		CSoundMgr::GetInstance()->Play(L"sfxBigBullet.wav", CSoundMgr::Player_Bullet);
		break;
	case ESoundID::FlameBullet:
		CSoundMgr::GetInstance()->Play(L"Flame.wav", CSoundMgr::Player_Bullet);
		break;
	case ESoundID::Hit:
		CSoundMgr::GetInstance()->Play(L"sfxHurt.wav", CSoundMgr::Player_Hit);
		break;
	case ESoundID::Trap:
		CSoundMgr::GetInstance()->Play(L"sfxTrap.wav", CSoundMgr::Player_Hit);
		break;
	case ESoundID::AddHeart:
		CSoundMgr::GetInstance()->Play(L"sfxHeart.mp3", CSoundMgr::Item_Heart);
	case ESoundID::AddCoin:
		CSoundMgr::GetInstance()->Play(L"sfxCoin.wav", CSoundMgr::Item_Coin);
		break;
	case ESoundID::Dash:
		CSoundMgr::GetInstance()->Play(L"sfxDash.mp3", CSoundMgr::Player_Action);
		break;
	case ESoundID::Run:
		m_bsfxStep = !m_bsfxStep;
		TileSound(m_eTileID);
		break;
	case ESoundID::LaserBullet:
		CSoundMgr::GetInstance()->Play(L"sfxLaser.wav", CSoundMgr::Player_Bullet);
		break;
	case ESoundID::AddAmmo:
		CSoundMgr::GetInstance()->Play(L"sfxEnergy.wav", CSoundMgr::Item_Ammo);
		break;
	case ESoundID::AddDisc:
		CSoundMgr::GetInstance()->Play(L"Secret.wav", CSoundMgr::Item_Disc);
		break;
	default:
		break;
	}
}

void CPlayer::AmmoLvUp()
{
	//  [1/4/2021 wades]
	++m_nAmmoLv;
	m_fAmmoMax = float(50 + (m_nAmmoLv * 15));
	m_nAmmoLv = CLAMP(m_nAmmoLv, 0, AmmoLvMax);
	SoundPlay(ESoundID::AmmoLvUp);
	m_pAmmoHud->SetAmmoLevel(m_nAmmoLv);
}

void CPlayer::AddWeapon(const EWeaponType _eWeaponType)
{
	for (auto& eType : m_vecWeapons)
	{
		if (eType == _eWeaponType)
		{
			return;
		}
	}

	//ó�� ���� ��� �ٷ� ���� ���� ȣ�� 
	m_vecWeapons.emplace_back(_eWeaponType);
	m_nSetWeaponID = m_vecWeapons.size() - 1;

	m_eCurWeaponType = m_vecWeapons[m_nSetWeaponID];

	ChangeWeapon();
	m_pAmmoHud->SetAmmoIcon((UINT)m_eCurWeaponType);
}

HRESULT CPlayer::InitializePrototype()
{
	m_sName = L"Player";
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
	// test [1/6/2021 wades]
	//m_fAmmo = m_fAmmoMax;
	m_fAmmo = 10;
	m_fDashDelay = 2.f;
	m_fDashDelayTime = m_fDashDelay;
	m_fDashDuration = 0.4f;

	//State
	m_nHp = 12;
	m_nHpMax = 4;
	m_fHitDelay = 0.5f;
	m_fHitDelayTime = 0.f;
	m_nDiscMax = 1;
	m_nGem = 0;
	m_nDisc = 0;
	m_nSkillPoint = 0;

	m_pTransform->Set_Scale(D3DXVECTOR3(3.9f, 3.9f, 3.9f));

	CCollider* pCollider = (CCollider*)(AddComponent<CCollider>());
	pCollider->m_bIsRigid = true;
	pCollider->SetMesh(L"Cube",BOUND::SPHERE);

	m_bsfxStep = false;

	return S_OK;
}

HRESULT CPlayer::Start()
{
	m_pGun = (CGun*)FindGameObjectOfType<CGun>();

	//Reference Setting
#pragma region Reference Setting
	m_pPlayerCamera = (CPlayerCamera*)FindGameObjectOfType<CPlayerCamera>();

	m_pAmmoHud = (CAmmoHUD*)FindGameObjectOfType<CAmmoHUD>();
	m_pHeartManager = (CHeartManager*)FindGameObjectOfType<CHeartManager>();
	m_pCrossHair = FindGameObjectOfType<CCrossHair>();
	m_pGemText = (CGemText*)FindGameObjectOfType<CGemText>();
	m_pDiscText = (CDiscText*)FindGameObjectOfType<CDiscText>();
	m_pFocus = (CFocus*)FindGameObjectOfType<CFocus>();

	//SafeAddRef(m_pPlayerCamera);
	SafeAddRef(m_pAmmoHud);
	SafeAddRef(m_pHeartManager);
	SafeAddRef(m_pCrossHair);
	SafeAddRef(m_pGemText);
	SafeAddRef(m_pDiscText);
	SafeAddRef(m_pFocus);

#pragma endregion

	//�������� ���ð� �� �ʹ� �ø�
#ifdef _DEBUG
	m_nTag = 0;
	CUtilityManger::ObjectCulling(m_nSceneID, m_nTag);
#else
	CPlayerSpawn* pSpawn = (CPlayerSpawn*)FindGameObjectOfType<CPlayerSpawn>();
	_vector vPosition = ((CTransform*)pSpawn->GetComponent<CTransform>())->Get_Position();
	m_pTransform->Set_Position(vPosition);
	m_nTag = pSpawn->GetTag();
	CUtilityManger::ObjectCulling(m_nSceneID, m_nTag);
#endif


	m_pTransform->Add_Position(D3DXVECTOR3(0.f,3.f,0.f));

	 //UI Setting
	 m_vecWeapons.emplace_back(EWeaponType::Big);
	 //Ammo
	 m_pAmmoHud->SetAmmoCount(m_fAmmo, m_fAmmoMax);
	 m_pAmmoHud->SetAmmoIcon((UINT)EWeaponType::Rapid);
	 m_pAmmoHud->SetAmmoLevel(0);
	 m_pAmmoHud->SetActive(false);

	 //Disc
	 m_pDiscText->SetMaxCount(DiscMax);
	 m_pDiscText->SetCount(m_nDisc);

	 //hp
	 m_pHeartManager->SetHeartCount(m_nHpMax);
	 m_pHeartManager->SetGauge(m_nHp);
	 // gun
	 m_pGun->SetActive(false);

	 AddWeapon(EWeaponType::Rapid);
	 AddWeapon(EWeaponType::Multiple);
	 AddWeapon(EWeaponType::Flame);
	 AddWeapon(EWeaponType::Lazer);

	 //Skill Setting

	// m_vecSkillID.push_back(ESkillID::TimeStop);
	 return S_OK;
}

UINT CPlayer::Update(const float _fDeltaTime)
{

	KeyInput(_fDeltaTime);
	m_pTransform->UpdateTransform();
	UpdateState(_fDeltaTime);
	UpdateLight();

	return OBJ_NOENVET;
}

UINT CPlayer::LateUpdate(const float _fDeltaTime)
{

	if (m_fBulletFireTime < m_fBulletFireDelay)
		m_fBulletFireTime += _fDeltaTime;
	if(m_fHitDelay > m_fHitDelayTime)
		m_fHitDelayTime += _fDeltaTime;

	//�뽬���� ����
	if (m_eState == EState::Dash && m_fDashDuration < m_fDashDurationTime)
		m_eState = EState::Move;

	if(m_eState == EState::Run && !m_pKeyMgr->Key_Press(KEY_LSHIFT))
		m_eState = EState::Move;

	if (m_eState == EState::Hit && m_fHitDelay < m_fHitDelayTime)
	{
		//�ӽ� 
		CPlayerCamera* pCamera = (CPlayerCamera*)FindGameObjectOfType<CPlayerCamera>();
		pCamera->SetShakeVertical(0.f);
		m_bIsDeBuff = false;
		m_eState = EState::Move;
	}

	if(m_fDashDelayTime < m_fDashDelay)
		m_fDashDelayTime += _fDeltaTime;

	m_pTransform->UpdateWorld();

	CGameObject::LateUpdate(_fDeltaTime);
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

void CPlayer::OnCollision(CGameObject * _pGameObject)
{
	if (L"RoomTrigger" == _pGameObject->GetName() && m_nTag != _pGameObject->GetTag())
	{
		m_nTag = _pGameObject->GetTag();
		cout << "Change RoomID : " <<m_nTag << endl;
		CUtilityManger::ObjectCulling(m_nSceneID, m_nTag);
	}

	if (L"Item" == _pGameObject->GetName())
	{
		TakeItem(((CItem*)(_pGameObject))->GetItemID());
	}

	//Dmg 
	if (m_eState != EState::Hit && m_fHitDelay < m_fHitDelayTime)
	{
		if (L"Monster" == _pGameObject->GetName() || L"Slider" == _pGameObject->GetName())
		{
			AddHp(-1);
			SoundPlay(ESoundID::Hit);
			m_fHitAnimatTime = 0.f;
			m_fHitDelayTime = 0.f;
			m_eState = EState::Hit;
		}
		else if (L"Electric" == _pGameObject->GetName() || L"Lava" == _pGameObject->GetName() )
		{
			AddHp(-1);
			SoundPlay(ESoundID::Trap);
			m_fHitAnimatTime = 0.f;
			m_fHitDelayTime = 0.f;
			m_eState = EState::Hit;
		}
		else if (L"Swamp" == _pGameObject->GetName())
		{
			m_bIsDeBuff = true;
			AddHp(-1);
			SoundPlay(ESoundID::Trap);
			m_fHitAnimatTime = 0.f;
			m_fHitDelayTime = 0.f;
			m_eState = EState::Hit;
		}
	}

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
	//SafeRelease(m_pPlayerCamera);
	SafeRelease(m_pAmmoHud);
	SafeRelease(m_pHeartManager);
	SafeRelease(m_pCrossHair);
	SafeRelease(m_pGemText);
	SafeRelease(m_pDiscText);
	SafeRelease(m_pFocus);
	m_vecWeapons.clear();
	m_vecWeapons.shrink_to_fit();
	CGameObject::Free();
}
