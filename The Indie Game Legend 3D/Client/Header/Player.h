#pragma once
#ifndef __PLAYER_H__
#include "GameObject.h"
USING(Engine)

#define NormalDelay 0.35f
#define BigDelay 1.5f
#define MultipleDelay 0.5f
#define RapidDelay 0.1f
#define RunCameraYCycle 0.3f
#define FlameDelay 0.1f
#define LaserDelay 0.5f


enum class EState : _uint
{
	Move,
	Dash,
	Run,
	Hit,
	End
};
enum class ESoundID :_uint
{
	NormaBullet,
	BigBullet,
	FlameBullet,
	LaserBullet,
	Hit,
	AddHeart,
	AddCoin,
	AddAmmo,
	AddEnergy,
	Dash,
	Run,
};

enum class ESectorTileID : _uint
{
	Sector1,
	Sector2,
	End
};
class CPlayer final: public CGameObject
{
private:
	 class CKeyManager* m_pKeyMgr;
	 float				m_fMoveSpeed = 0.f;
	 float				m_fDashSpeed = 0.f;
	 float				m_fRunSpeed = 0.f;
	 float				m_fMouseSpeedX = 0.f;

	//weapon
	 vector<EWeaponType>	m_vecWeapons; 
	 EWeaponType	    m_eCurWeaponType;
	 EWeaponType	    m_ePreWeaponType;

	 _int				m_nSetWeaponID;


	 bool				m_bUseWeapon;
	 float				m_fBulletFireDelay = 0.f;
	 float				m_fBulletFireTime = 0.f;

	 float				m_fAmmo;
	 float				m_fAmmoMax;
	 _uint				m_nAmmoDecrease;

	 //UI
	 Image*						m_pAmmobar = nullptr;
	 class CHeartManager*		m_pHeartManager = nullptr;
	 class CWeaponHUD*			m_pWeaponHud = nullptr;
	 class CWeaponHUD* m_pWeaponHUD;


	 //State
	 _int						m_nHp = 0;
	 _int						m_nHpMax = 0;
	 _int						m_nCoin = 0;

	 float						m_fHitDelay = 0.f;
	 float						m_fHitDelayTime = 0.f;
	 float						m_fDebuffDuration = 0.f;
	 float						m_fDebuffDurationTime = 0.f;
	 bool						m_bIsDeBuff = false;

	 //Action
	 EState				m_eState;
	 float				m_fRunningTime = 0.f;
	 _vector			m_vMoveDir;

	 float				m_fDashDelay;
	 float				m_fDashDelayTime;
	 float				m_fDashDuration;
	 float				m_fDashDurationTime;

	 bool				m_bsfxStep;
	 wstring			m_sStepsfx; //나중에 타일에 따른 사운드 변화 줄떄 테스트용
	 ESectorTileID		m_eSector;

private:
	HRESULT KeyInput(const float _fDeltaTime);
	HRESULT MoveCheck();
	void	Move(const float& _fSpeed , const float _fDeltaTime);
	void	UpdateState(const float _fDeltaTime);
	
	void	TakeItem(const EItemID& _eID);
	void	BulletFire();
	void	ChangeWeaponUISetting();// 무기 교체 ui관련 콜 
	void    ChangeWeapon();// 관련 세팅값 설정
	void	UpdateLight();
	void	AddHp(_int _nHp);
	void	TileSound(ESectorTileID _eID);
	void	SoundPlay(const ESoundID& _eID);
public:
	//Getter Setter
	const float& GetAmmo() { return m_fAmmo; }
	const float& GetAmmoMax() { return m_fAmmoMax; }
	void AddWeapon(const EWeaponType _eWeaponType);
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
