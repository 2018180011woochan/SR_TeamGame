#pragma once
#ifndef __PLAYER_H__
#include "GameObject.h"
USING(Engine)

#define NormalDelay 0.35f
#define BigDelay 1.5f
#define MultipleDelay 0.5f
#define RapidDelay 0.1f

class CPlayer final: public CGameObject
{
private:
	 class CKeyManager* m_pKeyMgr;
	 float				m_fMoveSpeed = 0.f;
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
	 Image*				m_pAmmobar;
private:
	HRESULT Key_Input(const float _fDeltaTime);
	void	BulletFire();
	void	ChangeWeaponUISetting();// ���� ��ü ui���� �� 
	void    ChangeWeapon();// ���� ���ð� ����
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
