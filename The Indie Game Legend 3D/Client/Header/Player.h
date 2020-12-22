#pragma once
#ifndef __PLAYER_H__
#include "GameObject.h"
USING(Engine)

#define NormalDelay 0.35f
#define BigDelay 1.5f
#define MultipleDelay 0.5f
#define RapidDelay 0.1f
#define RunCameraYCycle 0.3f

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
	NormalFire,
	BigFire,
	Hit,
	AddHeart,
	AddCoin,
	Dash,

	Run,

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
	 //Test
	 Image*						m_pAmmobar = nullptr;
	 class CHeartManager*		m_pHearManager = nullptr;


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

	void	SoundPlayer(const ESoundID& _eID);
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
