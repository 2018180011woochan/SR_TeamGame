#pragma once
#ifndef __PLAYER_H__
#include "GameObject.h"
USING(Engine)

class CGun;

#define NormalDelay 0.35f
#define BigDelay 1.5f
#define MultipleDelay 0.5f
#define RapidDelay 0.1f
#define RunCameraYCycle 0.3f
#define FlameDelay 0.1f
#define LaserDelay 0.5f
#define HPMax 6
#define SkillPGaugeMax 20
#define DiscMax 4
#define AmmoLvMax 3
#define HighNoonAmount 30.f
#define HighNoonMaxDmg 100.f
#define DashDmg 10

enum class EState : _uint
{
	Move,
	Dash,
	Run,
	Hit,
	DashAttack,
	End
};

enum class ESkillID : _uint
{
	TimeStop,
	AutoAim,
	AirStrike,
	End
};
enum class ESoundID :_uint
{
	NormaBullet,
	BigBullet,
	DashHit,
	FlameBullet,
	LaserBullet,
	Hit,
	YoShi,
	Trap,
	EmptyShot,
	AddHeart,
	AddCoin,
	AddAmmo,
	AmmoLvUp,
	AddDisc,
	Dash,
	DashAttack,
	Run,
};


class CPlayer final: public CGameObject
{
private:
	// Reference [1/6/2021 wades]
	 class CKeyManager*			m_pKeyMgr;
	 class CPlayerCamera*		m_pPlayerCamera;
	 //Skill
	 vector<ESkillID>			m_vecSkillID;
	//weapon
	 vector<EWeaponType>		m_vecWeapons; 
	 EWeaponType			    m_eCurWeaponType;
	 EWeaponType			    m_ePreWeaponType;

	 _int						m_nSetWeaponID;


	 bool						m_bUseWeapon;
	 float						m_fBulletFireDelay = 0.f;
	 float						m_fBulletFireTime = 0.f;

	 float						m_fAmmo;
	 float						m_fAmmoMax;
	 _uint						m_nAmmoDecrease;

	 //UI
	 class CAmmoHUD*            m_pAmmoHud = nullptr;
	 class CHeartManager*		m_pHeartManager = nullptr;
	 CGameObject*				m_pCrossHair = nullptr;
	 class CGemText*			m_pGemText = nullptr;
	 class CDiscText*			m_pDiscText = nullptr;
	 class CFocus*				m_pFocus = nullptr;

	 //State
	 _int						m_nHp = 0;
	 _int						m_nHpMax = 0;
	 _int						m_nGem = 0;
	 _int						m_nDisc = 0;
	 _int						m_nDiscMax = 0;
	 _int						m_nSkillPoint = 0;
	 _int						m_nAmmoLv= 0;

	 float						m_fMoveSpeed = 0.f;
	 float						m_fDashSpeed = 0.f;
	 float						m_fRunSpeed = 0.f;
	 float						m_fMouseSpeedX = 0.f;

	 float						m_fHitDelay = 0.f;
	 float						m_fHitDelayTime = 0.f;
	 float						m_fHitAnimatTime = 0.f;

	 float						m_fDebuffDuration = 0.f;
	 float						m_fDebuffDurationTime = 0.f;
	 bool						m_bIsDeBuff = false;
	 bool						m_bEnableSkill = false;

	 // HighNoon [1/6/2021 wades]
	 float						m_fHighNoonDmg;
	 list<CGameObject*>			m_listHighNoon;
	 //Action
	 EState				m_eState;
	 float				m_fRunningTime = 0.f;
	 _vector			m_vMoveDir;

	 //  [1/7/2021 wades]
	 float						m_fDashAttackSpeed;
	 // �뽬���� ���ӽð�
	 float						m_fDashAttackDruation;
	 float						m_fDashMoveDuration;
	 //�뽬 ���� ���� �Է� ������ 
	 float						m_fDashActionDelay;
	 float						m_fDashActionDelayTime;

	 // �뽬���� �ൿ �ð� ��ĥ ���� ���⿡ ������
	 float						m_fDashDurationTime;

	 bool						m_bDashDmg;

	 //��ô� �뽬�������� ������ ���� ���� ������ �Ұ���
	 bool						m_bDashAttacked;

	 bool						m_bsfxStep; // �ȴ� �Ҹ��� �ΰ����� ǥ���ҰŶ� ����Ī��
	 bool						m_bSpotLightTrigger;

	 wstring					m_sStepsfx; //���߿� Ÿ�Ͽ� ���� ���� ��ȭ �ً� �׽�Ʈ��
	 ETileID					m_eTileID;

	 CGun*						m_pGun;

	 // by Woochan
	 bool						m_bIsBuyWeapon;
	 bool						m_bIsBuySkillRunning;
private:
	HRESULT KeyInput(const float _fDeltaTime);
	HRESULT MoveCheck();
	void	Move(float _fResultSpeed , const float _fDeltaTime);
	void	UpdateState(const float _fDeltaTime);
	
	void	TakeItem(const EItemID& _eID);
	void	BulletFire();
	void	ChangeWeaponUISetting();// ���� ��ü ui���� �� 
	void    ChangeWeapon();// ���� ���ð� ����
	void	UpdateLight();
	void	TileSound(const ETileID& _eID);
	void	SoundPlay(const ESoundID& _eID);
	void	AmmoLvUp();
public:
	//Getter Setter
	void	SetSpotLightTrigget(const bool& _bTrigger);
	void	SetsfxTileID(const ETileID& _bool);
	void	AddHp(_int _nHp);
	void	AddSkillGauge(_int _nPoint);
	void    AddHpMax();
	void	AddWeapon(const EWeaponType _eWeaponType);
	bool	GetDashAttack() { return m_bDashDmg; };

	// Prod By Woochan
	void	SetByWeapon() { m_bIsBuyWeapon = true; }
	void	SetBySkillRunning() { m_bIsBuySkillRunning = true; }
	void	SetBuyItem(_int _Price);
	_int	GetGem() { return m_nGem; }

protected:
	explicit CPlayer();
	explicit CPlayer(const CPlayer& _rOther);
public:
	virtual ~CPlayer() = default;
public:
	virtual HRESULT InitializePrototype() override;
	virtual HRESULT Awake() override;
	virtual HRESULT Start() override;
	virtual UINT Update(const float _fDeltaTime) override;
	virtual UINT LateUpdate(const float _fDeltaTime) override;
	virtual HRESULT Render() override;
	virtual CGameObject * Clone() override;

	virtual void OnCollision(CGameObject* _pGameObject) override;
public:
	static CPlayer* Create();
private:
	virtual void Free() override;
};
#define __PLAYER_H__
#endif
