#pragma once
#ifndef __PLAYER_H__
#include "GameObject.h"
USING(Engine)

#define NormalDelay 0.35f
#define BigDelay 1.5f
#define MultipleDelay 0.5f
#define RapidDelay 0.1f

enum class EState : _uint
{
	Move,
	Dash,
	Hurt,
	End
};
class CPlayer final: public CGameObject
{
private:
	 class CKeyManager* m_pKeyMgr;
	 float				m_fMoveSpeed = 0.f;
	 float				m_fDashSpeed = 0.f;
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
	 /*
	 그 변수명 다른분들 뭐로 하시는 궁금해서 물어보는건데
	 지금 데쉬 딜레이 판정 변수 두개 만드는데 
		delay 변수랑 시간 누적해서딜레이랑 비교하는 변수 이름 뭐로 하세요 보통?
	 */
	 //Test
	 Image*				m_pAmmobar;
	 //Action
	 EState				m_eState;


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

	void	BulletFire();
	void	ChangeWeaponUISetting();// 무기 교체 ui관련 콜 
	void    ChangeWeapon();// 관련 세팅값 설정
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
public:
	static CPlayer* Create();
private:
	virtual void Free() override;
};
#define __PLAYER_H__
#endif
